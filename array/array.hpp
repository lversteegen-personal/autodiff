#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>
#include <sstream>
#include <type_traits>
#include <memory>
#include <functional>
#include <array>
#include <limits>
#include <thread>
#include <optional>

#include <cassert>
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert((void(msg), exp))

#include "data.hpp"
#include "broadcast_type.hpp"
#include "shape.hpp"
#include "../performance.hpp"
#include "simd_vector.hpp"

class Test;

namespace ArrayLibrary
{
    template <DataType T>
    class Array;

    namespace Matmul
    {
        struct MatmulSettings;

        template <DataType T>
        ArrayLibrary::Array<T> matmul(const ArrayLibrary::Array<T> &left, const ArrayLibrary::Array<T> &right, ArrayLibrary::Array<T> *const pDestArray, const ArrayLibrary::Matmul::MatmulSettings &settings);
    }

    template <DataType T>
    class Array
    {
        friend Test;

        friend ArrayLibrary::Array<T> Matmul::matmul<>(const ArrayLibrary::Array<T> &left, const ArrayLibrary::Array<T> &right, ArrayLibrary::Array<T> *const pDestArray, const ArrayLibrary::Matmul::MatmulSettings &settings);

        template <DataType U>
        friend class Array;

        static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

    public:
        static long calculateFlatLength(const Coordinates &shape)
        {
            long flatLength = 1;
            for (long i = 0; i < shape.size(); i++)
            {
                if (shape[i] < 0)
                    throw std::invalid_argument("Entries of absolute shape vector cannot be negative.");
                flatLength *= shape[i];
            }
            return flatLength;
        }

    private:
        Data<T> mData;

        inline T *getDataPointer() const
        {
            return mData.mRaw + mOffset;
        }

        long mFlatLength;

    public:
        long getFlatLength() const { return mFlatLength; }

    private:
        Coordinates mShape;
        Coordinates mStrides;
        long mOffset = 0;
        bool mContiguous = true;

    private:
        long mDim;

    public:
        long getDim() const { return mDim; }

    public:
        bool isContiguous() const { return mContiguous; }
        const Coordinates &refShape() const { return mShape; }
        const Coordinates &refStrides() const { return mStrides; }

        ~Array() = default;

        Array() = delete;

        Array(const Array<T> &other) : mData(other.mData),
                                       mFlatLength(other.mFlatLength), mShape(other.mShape), mStrides(other.mStrides), mDim(other.mDim), mOffset(other.mOffset), mContiguous(other.mContiguous)
        {
        }

        template <DataType U>
        Array(const Array<U> &other)
            requires std::is_convertible_v<U, T>
            : mData(other.mFlatLength),
              mFlatLength(other.mFlatLength), mShape(other.mShape), mStrides(other.mStrides), mDim(other.mDim), mOffset(other.mOffset), mContiguous(other.mContiguous)
        {
            Array<U>::template unaryDestDispatch<T, convert<U>>(other, *this);
        }

        Array(Array<T> &&other) = default;

    private:
        Array(const Data<T> &data, const Coordinates &shape, const Coordinates &strides, long offset, bool contiguous) : mData(data), mFlatLength(calculateFlatLength(shape)), mDim(shape.size()), mShape(shape), mStrides(strides), mOffset(offset), mContiguous(contiguous)
        {
        }

    public:
        Array(const T &single) : mData(Data<T>(1)), mFlatLength(1), mShape(0), mStrides(0), mDim(0), mContiguous(true)
        {
            mData[0] = single;
        }

        Array(const Data<T> &data, const Coordinates &shape, const long offset=0) : mData(data), mFlatLength(calculateFlatLength(shape)), mShape(shape), mDim(shape.size()), mStrides(shape.size()), mOffset(offset), mContiguous(true)
        {
            calculateStrides();
        }

        Array(const Data<T> &data) : mData(data), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1), mContiguous(true)  {}

        Array(const std::vector<T> &data) : mData(data), mShape({data.size()}), mFlatLength(data.size()), mStrides({1}), mDim(1), mContiguous(true)  {}

        Array(const std::initializer_list<T> &values) : mData(values), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1), mContiguous(true) {}

        Array<T> copy() const
        {
            if (mContiguous)
                return Array<T>(mData.copy(mOffset, mOffset + mFlatLength), mShape, mStrides, 0, true);
            else
            {
                auto result = Array<T>(Data<T>(mFlatLength), mShape, mStrides, 0, true);
                return unaryDestDispatch<T, copy>(*this, result);
            }
        }

        bool isTransposed() const { return mContiguous; }
        Array<T> transpose(int axis1, int axis2) const
        {
            axis1 = axis1 % mDim;
            axis2 = axis2 % mDim;
            axis1 = axis1 < 0 ? mDim + axis1 : axis1;
            axis2 = axis2 < 0 ? mDim + axis2 : axis2;

            Array<T> result(*this);
            result.mContiguous = false;
            std::swap(result.mShape[axis1], result.mShape[axis2]);
            std::swap(result.mStrides[axis1], result.mStrides[axis2]);
            return result;
        }

        Array<T> leftExpandDim(long addedDims) const
        {
            if (addedDims < 0)
                throw std::invalid_argument("Number of dimensions to add has to be positive.");

            if (addedDims + mDim > MAX_DIM)
                throw std::invalid_argument("The dimensionality of the resulting array would exceed MAX_DIM.");

            Coordinates newShape = mShape.shiftRight(1, addedDims);
            Coordinates newStrides = mStrides.shiftRight(0, addedDims);

            return Array<T>(mData, newShape, newStrides, mOffset, mContiguous);
        }

        Array<T> rightExpandDim(long addedDims) const
        {
            if (addedDims < 0)
                throw std::invalid_argument("Number of dimensions to add has to be positive.");

            if (addedDims + mDim > MAX_DIM)
                throw std::invalid_argument("The dimensionality of the resulting array would exceed MAX_DIM.");

            Coordinates newShape(mDim + addedDims, 1);
            Coordinates newStrides(mDim + addedDims, 0);
            for (long i = 0; i < mDim; i++)
            {
                newShape[i] = mShape[i];
                newStrides[i] = mStrides[i];
            }

            return Array<T>(mData, newShape, newStrides, mOffset, mContiguous);
        }

        Array<T> reshape(const Coordinates &shape) const
        {
            if (!mContiguous)
                throw std::logic_error("Cannot reshape a non-contiguous array.");

            long flatLength = 1;
            long wildcardDimension = -1;
            for (long i = 0; i < shape.size(); i++)
            {
                if (shape[i] == -1)
                {
                    if (wildcardDimension != -1)
                        throw std::invalid_argument("Only one dimension can be -1.");
                    wildcardDimension = i;
                }
                else
                    flatLength *= shape[i];
            }
            if (wildcardDimension == -1 && flatLength == mFlatLength)
            {
                return Array<T>(mData, shape, mOffset);
            }
            else if (wildcardDimension != -1 && flatLength <= mFlatLength && mFlatLength % flatLength == 0)
            {
                Coordinates newShape(shape);
                newShape[wildcardDimension] = mFlatLength / flatLength;
                return Array<T>(mData, newShape, mOffset);
            }
            else
                throw std::invalid_argument("Shape does not match data size.");
        }

        Array<T> &operator=(const Array<T> &other) = default;

        Array<T> &operator=(Array<T> &&other) = default;

        Array<T> &operator=(const T value)
        {
            unaryParamApply<T, assign>(value);
            return *this;
        }

        static Coordinates calculateStrides(const Coordinates &shape)
        {
            int dim = shape.size();
            Coordinates strides(dim);
            long multiplier = 1;

            for (long i = 0; i < dim; i++)
            {
                if (shape[i] < 0)
                    throw std::invalid_argument("Entries of absolute shape vector cannot be negative.");

                if (shape[dim - i - 1] == 1)
                    strides[dim - i - 1] = 0;
                else
                {
                    strides[dim - i - 1] = multiplier;
                    multiplier *= shape[dim - i - 1];
                }
            }

            return strides;
        }

    private:
        void calculateStrides()
        {
            long multiplier = 1;
            for (long i = 0; i < mDim; i++)
            {
                if (mShape[mDim - i - 1] == 1)
                    mStrides[mDim - i - 1] = 0;
                else
                {
                    mStrides[mDim - i - 1] = multiplier;
                    multiplier *= mShape[mDim - i - 1];
                }
            }
        }

    public:
        static Array<T> fromFlatLines(const std::vector<Data<T>> &lines);
        static Array<T> fromLines(const std::vector<Array<T>> &lines);
        static Array<T> range(T from, T to, T step);
        static Array<T> range(T from, T to);
        static Array<T> range(T to);
        static Array<T> constant(const Coordinates &shape, const T constant);

        Array<T> extend(const Coordinates &shape) const
        {
            if (shape.size() != mDim)
                throw std::invalid_argument("The new shape must have the same number of dimensions as the old shape");
            if (!isSubshape(mShape, shape))
                throw std::invalid_argument("Other array is not a subshape of this array.");

            long flatLength = 1;
            Coordinates newStrides(mDim);

            for (long i = mDim - 1; i >= 0; i--)
            {
                if (shape[i] == 1)
                    newStrides[i] = 0;
                else
                {
                    newStrides[i] = flatLength;
                    flatLength *= shape[i];
                }
            }

            auto result = Array<T>(Data<T>(flatLength), shape, newStrides);
            return unaryDestDispatch<T, copy>(result, *this);
        }

        template <typename U>
        Array<U> oneHot() const
        {
            static_assert(std::is_integral<T>(), "T of Array<T> must be an integer.");
            return oneHot<U>(Array<T>::range(reduceMin().get(0), reduceMax().get(0) + 1));
        }

        template <typename U>
        Array<U> oneHot(T from, T to) const
        {
            return oneHot<U>(Array<T>::range(from, to));
        }

        template <typename U>
        Array<U> oneHot(Array<T> valueRange) const
        {
            return Array<U>(reshape(mShape + 1) == valueRange.reshape(valueRange.mShape.shiftRight(1, mDim)));
        }

    private:
        template <DataType U, U (*f)(const T), SimdVector<U> (*fSimd)(const SimdVector<T> &) = nullptr>
        static Array<U> &unaryDestDispatch(const Array<T> &source, Array<U> &dest);

        template <DataType U, typename P, U (*f)(const T, const P &), typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &) = nullptr>
        static Array<U> &unaryParamDestDispatch(const Array<T> &source, Array<U> &dest, const P &param, const Q &simdParam = Q());

        template <DataType U, U (*f)(const T)>
        static void baseUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides);

        /// @param param The parameter is NOT passed by reference to allow the compiler to keep it in the register
        template <DataType U, typename P, U (*f)(const T, const P &)>
        static void baseParamUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const P param);

        template <DataType U, U (*f)(const T)>
        static inline void unaryBoost(const T *pSourceData, U *pDestData, const long length, const long sourceStride, const long destStride);

        template <DataType U, typename P, U (*f)(const T, const P &)>
        static inline void unaryParamBoost(const T *pSourceData, U *pDestData, const long length, const long sourceStride, const long destStride, const P &param);

        template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &), bool sourceSkip>
        static void simdUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength);

        /// @brief
        /// @tparam Q The Simd version of the parameter
        /// @tparam U The scalar type of the output
        /// @tparam fSimd The Simd function operating on the data
        /// @tparam sourceSkip Whether the source is broadcasted in the contiguous dimensions of the two arrays
        /// @param pSourceData
        /// @param pDestData
        /// @param sourceShape
        /// @param destShape
        /// @param sourceStrides
        /// @param destStrides
        /// @param flatBoostDim
        /// @param flatBoostDimLength
        /// @param param The parameter is NOT passed by reference to allow the compiler to keep it in the register
        template <DataType U, typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &), bool sourceSkip>
        static void simdUnaryParam(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength, const Q param);

        template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &), bool sourceSkip>
        static inline void simdUnaryBoost(const T *pSourceData, U *pDestData, const long axisLength);

        template <DataType U, typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &), bool sourceSkip>
        static inline void simdUnaryParamBoost(const T *pSourceData, U *pDestData, const long axisLength, const Q &param);

    public:
        static inline T add(const T a, const T b) { return a + b; }
        static inline T multiply(const T a, const T b) { return a * b; }
        static inline T subtract(const T a, const T b) { return a - b; }
        static inline T divide(const T a, const T b) { return a / b; }
        static inline T modulo(const T a, const T b) { return a % b; }
        static inline T max(const T a, const T b) { return a > b ? a : b; }
        static inline T min(const T a, const T b) { return a < b ? a : b; }
        static inline bool any(bool a, const T b) { return a || (b != 0); }
        static inline bool all(bool a, const T b) { return a && (b != 0); }
        static inline T logical_and(const T a, const T b) { return a && b; }
        static inline T logical_or(const T a, const T b) { return a || b; }

        static inline bool equal(const T a, const T b) { return a == b; }
        static inline bool notEqual(const T a, const T b) { return a != b; }
        static inline bool less(const T a, const T b) { return a < b; }
        static inline bool lessEqual(const T a, const T b) { return a <= b; }
        static inline bool greater(const T a, const T b) { return a > b; }
        static inline bool greaterEqual(const T a, const T b) { return a >= b; }

        template <DataType U, DataType V, U (*g)(V), V (*f)(const T)>
        static inline U compose(const T x)
        {
            return g(f(x));
        }

        template <T (*g)(T), T (*f)(const T)>
        static inline T compose(const T x)
        {
            return g(f(x));
        }

        template <DataType U>
        static inline U pow_ptw(const T a, const T b) { return (U)std::pow(a, b); }

        static inline T exp_ptw(const T a) { return std::exp(a); }
        static inline T sin_ptw(const T a) { return std::sin(a); }
        static inline T cos_ptw(const T a) { return std::cos(a); }
        static inline T abs_ptw(const T a) { return std::abs(a); }
        static inline T copy(const T s) { return s; }

        template <DataType U>
        static inline T convert(U s)
        {
            return (T)s;
        }

        struct ClipBounds
        {
            T lowerBound;
            T upperBound;
            ClipBounds() = delete;

            ClipBounds(T lower, T upper) : lowerBound(lower), upperBound(upper)
            {
                if (lowerBound > upperBound)
                    throw new std::invalid_argument("Lower bound must be below upper bound.");
            }
        };

        static inline T scalarClip(const T a, const ClipBounds &bounds)
        {
            return a < bounds.lowerBound ? bounds.lowerBound : (a > bounds.upperBound ? bounds.upperBound : a);
        }

        static inline T assign(const T dummy, const T &value) { return value; }

    private:
        template <DataType U, U (*f)(const T, const T), SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &) = nullptr>
        static Array<U> &binaryDestCombineDispatch(Array<U> &dest, const Array<T> &left, const Array<T> &right);

        template <DataType U, U (*f)(const T, const T)>
        static void baseBinaryCombine(T *pLeftData, T *pRightData, U *pDestData, const Coordinates &leftShape, const Coordinates &rightShape, const Coordinates &destShape, const Coordinates &leftStrides, const Coordinates &rightStrides, const Coordinates &destStrides);

        template <DataType U, U (*f)(const T, const T)>
        static inline void binaryCombineBoost(T *pLeftData, T *pRightData, U *pDestData, const long length, const long leftStride, const long rightStride, const long destStride);

        template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &), bool leftSkip, bool rightSkip>
        static inline void simdBinaryCombineBoost(T *pLeftData, T *pRightData, U *pDestData, const long axisLength);

        template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &), bool leftSkip, bool rightSkip>
        static void simdBinaryCombine(T *pLeftData, T *pRightData, U *pDestData, const Coordinates &leftShape, const Coordinates &rightShape, const Coordinates &destShape, const Coordinates &leftStrides, const Coordinates &rightStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength);

        template <DataType U, U (*f)(const U, const T)>
        Array<U> reduce(const U &initial, const Coordinates &axes, bool keepDims = false) const
        {
            if (mDim == 0)
                return f(initial, *(getDataPointer()));
            if (axes.size() == 0)
                return copy();

            const ReduceInformation reduceInfo = reduceShape(mShape, axes, keepDims);
            const Coordinates &keepDimShape = reduceInfo.keepDimsShape;
            const Coordinates &keepDimStrides = reduceInfo.keepDimsStrides;
            const long flatLength = reduceInfo.flatLength;

            long boostDim = -1;
            long boostDimLength = -1;
            for (int i = mDim - 1; i >= 0; i--)
            {
                if (mShape[i] > boostDimLength)
                {
                    boostDim = i;
                    boostDimLength = mShape[i];
                }
            }

            Data<U> data(flatLength);
            data = initial;
            auto dest = Array<U>(data, keepDimShape, keepDimStrides, 0, true);

            U *pDestData = dest.getDataPointer();
            T *pSourceData = getDataPointer();
            const long sourceBoostStride = mStrides[boostDim];
            const long destBoostStride = keepDimStrides[boostDim];

            Coordinates c(mDim, 0);

            bool end = false;

            while (!end)
            {
                reduceBoost<U, f>(pSourceData, pDestData, boostDimLength, sourceBoostStride, destBoostStride);
                end = true;

                for (long i = mDim - 1; i >= 0; i--)
                {
                    if (i == boostDim)
                        continue;

                    c[i]++;

                    if (c[i] != mShape[i])
                    {
                        pDestData = keepDimShape[i] == 1 ? pDestData : pDestData + keepDimStrides[i];
                        pSourceData += mStrides[i];
                        end = false;
                        break;
                    }
                    else
                    {
                        pDestData = keepDimShape[i] == 1 ? pDestData : pDestData - keepDimStrides[i] * (mShape[i] - 1);
                        pSourceData -= mStrides[i] * (mShape[i] - 1);
                        c[i] = 0;
                    }
                }
            }

            if (keepDims)
            {
                return dest;
            }
            else
            {
                return dest.reshape(reduceInfo.keepDimsShape);
            }
        }

        template <typename U, U (*f)(const U, const T)>
        static inline void reduceBoost(T *pSourceData, T *pDestData, const long length, const long sourceStride, const long destStride)
        {
            for (long i = 0; i < length; i++)
            {
                *pDestData = f(*pDestData, *pSourceData);
                pSourceData += sourceStride;
                pDestData += destStride;
            }
        }

    public:
        template <T (*f)(const T), SimdVector<T> (*fSimd)(const SimdVector<T> &) = nullptr>
        Array<T> &unaryApply();

        template <typename P, T (*f)(const T, const P &), typename Q = bool, SimdVector<T> (*fSimd)(const SimdVector<T> &, const Q &) = nullptr>
        Array<T> &unaryParamApply(const P &param, const Q &simdParam);

        template <typename P, T (*f)(const T, const P &)>
        Array<T> &unaryParamApply(const P &param);

        template <DataType U, U (*f)(const T), SimdVector<U> (*fSimd)(const SimdVector<T> &) = nullptr>
        static Array<U> unaryCompute(const Array<T> &source);

        template <DataType U, typename P, U (*f)(const T, const P &), typename Q = bool, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &) = nullptr>
        static Array<U> unaryParamCompute(const Array<T> &source, const P &param, const Q &simdParam);

        template <DataType U, typename P, U (*f)(const T, const P &)>
        static Array<U> unaryParamCompute(const Array<T> &source, const P &param);

        template <T (*f)(const T, const T), SimdVector<T> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &) = nullptr>
        Array<T> &binaryApply(const Array<T> &other);

        template <DataType U, U (*f)(const T, const T), SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &) = nullptr>
        static Array<U> binaryCombine(const Array<T> &left, const Array<T> &right);

    public:
        /// @brief Returns a broadcast type stating how the two shapes can be broadcasted to match each other. If one shape has shorter dimension than the other, the shapes are aligned on the right, and the method checks whether the two shapes can broadcasted to match by adding more dimensions to the left of the shorter shape.
        /// @param shape1 First shape
        /// @param shape2 Second shape
        /// @return The broadcast type, see commentary of BroadcastType for explanation of flags.
        static BroadcastType broadcastRelationship(const Coordinates &shape1, const Coordinates &shape2)
        {
            long dim1 = shape1.size(), dim2 = shape2.size();
            long minDim = std::min(dim1, dim2);
            long shift1 = dim1 - minDim, shift2 = dim2 - minDim;
            BroadcastType result = BroadcastType::MATCH;

            for (long i = 0; i < shift1; i++)
                if (shape1[i] != 1)
                {
                    result &= BroadcastType::LEFTMIX;
                    break;
                }
            for (long i = 0; i < shift2; i++)
                if (shape2[i] != 1)
                {
                    result &= BroadcastType::RIGHTMIX;
                    break;
                }

            for (long i = minDim - 1; i >= 0; i--)
            {
                if (shape1[i + shift1] != shape2[i + shift2])
                {
                    if (shape1[i + shift1] == 1)
                        result &= BroadcastType::RIGHTMIX;
                    else if (shape2[i + shift2] == 1)
                        result &= BroadcastType::LEFTMIX;
                    else
                        return BroadcastType::NONE;
                }
            }

            return result;
        }

        static bool isSubshape(const Coordinates &shape1, const Coordinates &shape2)
        {
            return (broadcastRelationship(shape1, shape2) & BroadcastType::RIGHT) != 0;
        }

        static bool isShapeMatch(const Coordinates &shape1, const Coordinates &shape2)
        {
            return (broadcastRelationship(shape1, shape2) & BroadcastType::MATCH) != 0;
        }

        Array<bool> operator==(const Array<T> &other) const
        {
            return binaryCombine<bool, equal>(*this, other);
        }

        Array<bool> operator!=(const Array<T> &other) const
        {
            return binaryCombine<bool, notEqual>(*this, other);
        }

        Array<bool> operator<(const Array<T> &other) const
        {
            return binaryCombine<bool, less>(*this, other);
        }

        Array<bool> operator<=(const Array<T> &other) const
        {
            return binaryCombine<bool, lessEqual>(*this, other);
        }

        Array<bool> operator>(const Array<T> &other) const
        {
            return binaryCombine<bool, greater>(*this, other);
        }

        Array<bool> operator>=(const Array<T> &other) const
        {
            return binaryCombine<bool, greaterEqual>(*this, other);
        }

        Array<T> operator&&(const Array<T> &other) const
        {
            return binaryCombine<T, logical_and>(*this, other);
        }

        Array<T> operator||(const Array<T> &other) const
        {
            return binaryCombine<T, logical_or>(*this, other);
        }

        Array<T> &operator+=(const Array<T> &other)
        {
            if (SimdVector<T>::supported)
                return binaryApply<add, SimdVector<T>::add>(other);
            else
                return binaryApply<add>(other);
        }

        Array<T> operator+(const Array<T> &other) const
        {
            if (SimdVector<T>::supported)
                return binaryCombine<T, add, SimdVector<T>::add>(*this, other);
            else
                return binaryCombine<T, add>(*this, other);
        }

        Array<T> &operator+=(const T other)
        {
            return *this += Array<T>(other);
        }

        Array<T> operator+(const T other) const
        {
            return *this + Array<T>(other);
        }

        Array<T> &operator*=(const Array<T> &other)
        {
            if (SimdVector<T>::supported)
                return binaryApply<multiply, SimdVector<T>::multiply>(other);
            else
                return binaryApply<multiply>(other);
        }

        Array<T> operator*(const Array<T> &other) const
        {
            if (SimdVector<T>::supported)
                return binaryCombine<T, multiply, SimdVector<T>::multiply>(*this, other);
            else
                return binaryCombine<T, multiply>(*this, other);
        }

        Array<T> &operator*=(const T other)
        {
            return *this *= Array<T>(other);
        }

        Array<T> operator*(const T other) const
        {
            return *this * Array<T>(other);
        }

        Array<T> &operator-=(const Array<T> &other)
        {
            if (SimdVector<T>::supported)
                return binaryApply<subtract, SimdVector<T>::subtract>(other);
            else
                return binaryApply<subtract>(other);
        }

        Array<T> operator-(const Array<T> &other) const
        {
            if (SimdVector<T>::supported)
                return binaryCombine<T, subtract, SimdVector<T>::subtract>(*this, other);
            else
                return binaryCombine<T, subtract>(*this, other);
        }

        Array<T> &operator-=(const T &other)
        {
            return *this -= Array<T>(other);
        }

        Array<T> operator-(const T &other) const
        {
            return *this - Array<T>(other);
        }

        Array<T> &operator/=(const Array<T> &other)
        {
            if (SimdVector<T>::supported)
                return binaryApply<divide, SimdVector<T>::divide>(other);
            else
                return binaryApply<divide>(other);
        }

        Array<T> operator/(const Array<T> &other) const
        {
            if (SimdVector<T>::supported)
                return binaryCombine<T, divide, SimdVector<T>::divide>(*this, other);
            else
                return binaryCombine<T, divide>(*this, other);
        }

        Array<T> &operator/=(const T other)
        {
            return *this /= Array<T>(other);
        }

        Array<T> operator/(const T other) const
        {
            return *this / Array<T>(other);
        }

        Array<T> operator%(const Array<T> &other) const
        {
            static_assert(std::is_integral_v<T>, "Modulo operator is only defined for integral types.");
            return binaryCombine<T, modulo>(*this, other);
        }

    private:
        static inline bool find_isZero(T a) { return a == 0; }
        static inline bool find_isNonZero(T a) { return a != 0; }

    public:
        Array<bool> isNaN() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be NaN.");
            return unaryCompute<bool, std::isnan>(*this);
        }

        Array<bool> isInf() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be Inf.");
            return unaryCompute<bool, std::isinf>(*this);
        }

        bool checkNumerics() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be NaN or Inf.");
            return isNaN().reduceAny().eval() || isInf().reduceAny().eval();
        }

        template <bool (*f)(T)>
        Array<long> findWhere() const
        {
            auto list = std::vector<long>();
            Coordinates c(mDim, 0);

            T *pData = getDataPointer();

            for (long k = 0; k < mFlatLength; k++)
            {
                if (f(*(pData++)))
                    for (long i = 0; i < mDim; i++)
                        list.push_back(c[i]);

                for (long i = mDim - 1; i >= 0; i--)
                {
                    c[i]++;

                    if (c[i] % mShape[i] == 0)
                        c[i] = 0;
                    else
                        break;
                }
            }

            return Array<long>(Data<long>(list), Coordinates({list.size() / mDim, mDim}));
        }

        Array<long> findZero() const
        {
            return findWhere<find_isZero>();
        }

        Array<long> findNonZero() const
        {
            return findWhere<find_isNonZero>();
        }

        Array<T> square() const
        {
            return (*this) * (*this);
        }

        Array<T> intPow(const unsigned int k) const
        {
            if (k > 4)
                return this->pow((double)k);

            auto resultData = Data<T>(mFlatLength);
            resultData = 1;

            for (long i = 0; i < getFlatLength(); i++)
                for (long j = 0; j < k; j++)
                    resultData[i] *= mData[i];

            return Array<T>(resultData, mShape);
        }

    public:
        Array<T> pow(const Array<T> &other) const
        {
            return binaryCombine<T, pow_ptw<T>>(*this, other);
        }

        Array<T> exp() const
        {
            return unaryCompute<T, exp_ptw>(*this);
        }

        Array<T> sin() const
        {
            return unaryCompute<T, sin_ptw>(*this);
        }

        Array<T> cos() const
        {
            return unaryCompute<T, cos_ptw>(*this);
        }

        Array<T> abs() const
        {
            return unaryCompute<T, abs_ptw>(*this);
        }

        Array<T> clip(ClipBounds bounds) const
        {
            if (SimdVector<T>::supported)
                return unaryParamCompute<T, ClipBounds, scalarClip, SimdClipBounds<T>, SimdVector<T>::clip>(*this, bounds, SimdClipBounds(bounds.lowerBound, bounds.upperBound));
            else
                return unaryParamCompute<T, ClipBounds, scalarClip>(*this, bounds);
        }

        Array<T> clip(T lower, T upper) const
        {
            if (SimdVector<T>::supported)
                return unaryParamCompute<T, ClipBounds, scalarClip, SimdClipBounds<T>, SimdVector<T>::clip>(*this, ClipBounds(lower, upper), SimdClipBounds(lower, upper));
            else
                return unaryParamCompute<T, ClipBounds, scalarClip>(*this, ClipBounds(lower, upper));
        }

        Array<T> reduceSum(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<T, add>(0, axes, keepDims);
        }

        Array<T> reduceSum() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<T, add>(0, axes, false);
        }

        Array<T> reduceMean(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            long divisor = 1;

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");
                else
                {
                    long a = axes[i] % mDim;
                    a = a >= 0 ? a : a + mDim;
                    divisor *= mShape[a];
                }

            return reduce<T, add>(0, axes, keepDims) / divisor;
        }

        Array<T> reduceMean() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduceMean(axes, false);
        }

        Array<T> reduceProduct(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<T, multiply>(1, axes, keepDims);
        }

        Array<T> reduceProduct() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<T, multiply>(1, axes);
        }

        Array<T> reduceMax(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<T, max>(std::numeric_limits<T>::lowest(), axes, keepDims);
        }

        Array<T> reduceMax() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<T, max>(std::numeric_limits<T>::lowest(), axes);
        }

        Array<T> reduceMin(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<T, min>(std::numeric_limits<T>::max(), axes, keepDims);
        }

        Array<T> reduceMin() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<T, min>(std::numeric_limits<T>::max(), axes);
        }

        Array<bool> reduceAny(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<bool, any>(false, axes, keepDims);
        }

        Array<bool> reduceAny() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<bool, any>(false, axes);
        }

        Array<bool> reduceAll(const Coordinates &axes, bool keepDims = false) const
        {
            if (axes.size() > mDim)
                throw std::invalid_argument("Too many axes for array dimension.");

            for (int i = 0; i < axes.size(); i++)
                if (axes[i] < -mDim || axes[i] >= mDim)
                    throw std::invalid_argument("Axis out of bounds.");

            return reduce<bool, all>(true, axes, keepDims);
        }

        Array<bool> reduceAll() const
        {
            Coordinates axes(mDim);
            for (int i = 0; i < mDim; i++)
                axes[i] = i;

            return reduce<bool, all>(true, axes);
        }

        Array<T> take(Coordinates at, bool keepDims = false) const
        {
            Coordinates to(at.size());
            for (int i = 0; i < at.size(); i++)
                to[i] = at[i];

            return slice(at, to, keepDims);
        }

        Array<T> slice(Coordinates from, Coordinates upto, bool keepDims = false) const
        {
            if (from.size() > mDim || upto.size() > mDim)
                throw std::invalid_argument("The index tuples cannot be longer than the array dimension.");
            if (from.size() != upto.size())
                throw std::invalid_argument("The index tuples must have the same length.");

            Coordinates newShape(0);
            Coordinates newStrides(0);
            long flatlength = 1;
            long offset = 0;
            bool contiguous = mContiguous;

            for (long i = 0; i < mDim; i++)
            {
                if (i < from.size())
                {
                    if (from[i] > mShape[i] || from[i] < -mShape[i] || upto[i] > mShape[i] || upto[i] < -mShape[i])
                        throw std::invalid_argument("Bounds have to be between -shape and shape.");

                    if (from[i] < 0)
                        from[i] += mShape[i];
                    if (upto[i] < 0)
                        upto[i] += mShape[i];

                    if (from[i] > upto[i])
                        throw std::invalid_argument("upto cannot be smaller than from mod shape.");

                    offset += from[i] * mStrides[i];

                    if (from[i] != upto[i])
                    {
                        if (i != 0)
                            contiguous = false;
                        newShape.pushBack(upto[i] - from[i]);
                        newStrides.pushBack(mStrides[i]);
                        flatlength *= upto[i] - from[i];
                    }
                    else if (keepDims)
                    {
                        newShape.pushBack(1);
                        newStrides.pushBack(0);
                    }
                }
                else
                {
                    newShape.pushBack(mShape[i]);
                    newStrides.pushBack(mStrides[i]);
                    flatlength *= mShape[i];
                }
            }

            return Array<T>(mData, newShape, newStrides, offset, contiguous);
        }

        explicit operator T() const
        {
            if (mFlatLength != 0)
                std::logic_error("Array has more than one entry.");

            return *getDataPointer();
        }

        T eval() const
        {
            if (mFlatLength != 0)
                std::logic_error("Array has more than one entry.");

            return *getDataPointer();
        }

        T &operator[](const Coordinates indices) const { return get(indices); }

        // This is too easily confused with taking a slice
        // T &operator[](const int i) const { return get({i}); }

        T &get(const Coordinates indices) const
        {
            if (indices.size() != mDim)
                throw std::invalid_argument("The index tuple does not match the array shape");
            long combinedIndex = mOffset;
            long ix;

            for (long i = 0; i < mDim; i++)
            {
                ix = indices[i] % mShape[i];
                if (ix < 0)
                    ix += mShape[i];
                combinedIndex += mStrides[i] * ix;
            }

            return mData[combinedIndex];
        }

        T &get(long i) const
        {
            long k = 0;
            for (long j = mDim - 1; j >= 0; j--)
            {
                k += mStrides[j] * (i % mShape[j]);
                i /= mShape[j];
            }

            return mData[k];
        }

        std::string to_string() const
        {
            if (getFlatLength() > 10000)
                return "Output too long. Flat length is " + std::to_string(getFlatLength());

            if (mDim == 0)
                return std::to_string(*getDataPointer());

            std::ostringstream oss;
            Coordinates c(mDim, 0);

            while (c[0] < mShape[0])
            {
                for (long i = mDim - 1; i >= 0; i--)
                {
                    if (mShape[i] == 1 || c[i] % mShape[i] == 0)
                        oss << "[";
                    else
                        break;
                }

                oss << "\t" << get(c);

                for (long i = mDim - 1; i >= 0; i--)
                {
                    c[i]++;

                    if (mShape[i] == 1 || c[i] % mShape[i] == 0)
                        oss << "]";
                    else
                    {
                        oss << "," << std::string(mDim - 1 - i, '\n');
                        break;
                    }
                }
            }

            return oss.str();
        }
    };
}

#endif