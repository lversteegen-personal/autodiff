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
#include "shape.hpp"
#include "../performance.hpp"
#include "simd.hpp"
#include "common_operations.hpp"

int main();

namespace ArrayLibrary
{

    template <DataType T>
    using SimdVector = Simd::Vector<T>;

    template <DataType T>
    class Array;

    template <DataType ResultType, DataType... InputTypes>
    class UniversalPointwise;

    namespace Matmul
    {
        struct MatmulSettings;

        template <DataType T>
        ArrayLibrary::Array<T> matmul(const ArrayLibrary::Array<T> &left, const ArrayLibrary::Array<T> &right, ArrayLibrary::Array<T> *const pDestArray, const ArrayLibrary::Matmul::MatmulSettings &settings);
    }

    namespace Convolution
    {
        struct Settings;

        template <DataType T>
        void simpleConvolveInner(const Array<T> &dest, const Array<T> &array, const Array<T> &kernel, const Settings &settings, const Coordinates &kernelPos);
    }

    template <DataType T>
    class Array
    {
        friend int main();

        friend ArrayLibrary::Array<T> Matmul::matmul<>(const ArrayLibrary::Array<T> &left, const ArrayLibrary::Array<T> &right, ArrayLibrary::Array<T> *const pDestArray, const ArrayLibrary::Matmul::MatmulSettings &settings);

        template <DataType ResultType, DataType... InputTypes>
        friend class UniversalPointwise;

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
        inline const T *readDataPointer() const
        {
            return mData.mRaw + mOffset;
        }

        long getFlatLength() const { return mFlatLength; }

    private:
        Coordinates mShape;
        Coordinates mStrides;
        long mOffset = 0;
        bool mContiguous = true;

    private:
        long mDim;

    public:
        inline long getDim() const { return mDim; }

    public:
        inline bool isContiguous() const { return mContiguous; }
        inline const Coordinates &refShape() const { return mShape; }
        inline const Coordinates &refStrides() const { return mStrides; }

        ~Array() = default;

        Array() = delete;

        Array(const Array<T> &other) : mData(other.mData),
                                       mFlatLength(other.mFlatLength), mShape(other.mShape), mStrides(other.mStrides), mDim(other.mDim), mOffset(other.mOffset), mContiguous(other.mContiguous)
        {
        }

        template <DataType U>
        explicit Array(const Array<U> &other)
            requires std::is_convertible_v<U, T>
            : mData(other.mFlatLength),
              mFlatLength(other.mFlatLength), mShape(other.mShape), mStrides(other.mStrides), mDim(other.mDim), mOffset(other.mOffset), mContiguous(other.mContiguous)
        {
            computeInPlace<Convert<T, U>>(*this, other);
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

        Array(const Data<T> &data, const Coordinates &shape, const long offset = 0) : mData(data), mFlatLength(calculateFlatLength(shape)), mShape(shape), mDim(shape.size()), mStrides(shape.size()), mOffset(offset), mContiguous(true)
        {
            calculateStrides();
        }

        Array(const Data<T> &data) : mData(data), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1), mContiguous(true) {}

        Array(const std::vector<T> &data) : mData(data), mShape({data.size()}), mFlatLength(data.size()), mStrides({1}), mDim(1), mContiguous(true) {}

        Array(const std::initializer_list<T> &values) : mData(values), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1), mContiguous(true) {}

        Array<T> copy() const
        {
            return compute<Copy<T>, true>(*this);
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

        /// @brief Adds or removes trivial axes to reshape the array to a desired dimension
        /// @param desiredDim
        /// @return
        Array<T> adjustDimension(long desiredDim) const
        {
            if (desiredDim < 0)
                throw std::invalid_argument("Dimension has to be non-negative");

            if (desiredDim > MAX_DIM)
                throw std::invalid_argument("The dimension cannot exceed MAX_DIM");

            if (desiredDim == mDim)
                return *this;

            if (desiredDim > mDim)
            {
                Coordinates newShape = mShape.shiftRight(1, desiredDim - mDim);
                Coordinates newStrides = mStrides.shiftRight(0, desiredDim - mDim);

                return Array<T>(mData, newShape, newStrides, mOffset, mContiguous);
            }
            else
            {
#ifdef DEBUG_MODE
                for (long i = 0; i < mDim - desiredDim; i++)
                    if (mShape[i] != 1)
                        throw std::invalid_argument("Pruning would remove non-trivial axes");
#endif

                Coordinates newShape = mShape.interval(mDim - desiredDim, mDim);
                Coordinates newStrides = mStrides.interval(mDim - desiredDim, mDim);

                return Array<T>(mData, newShape, newStrides, mOffset, mContiguous);
            }
        }

        Array<T> leftExpandDim(long addedDims) const
        {
            if (addedDims < 0)
                throw std::invalid_argument("Number of dimensions to add has to be non-negative.");

            if (addedDims + mDim > MAX_DIM)
                throw std::invalid_argument("The dimensionality of the resulting array would exceed MAX_DIM.");

            Coordinates newShape = mShape.shiftRight(1, addedDims);
            Coordinates newStrides = mStrides.shiftRight(0, addedDims);

            return Array<T>(mData, newShape, newStrides, mOffset, mContiguous);
        }

        Array<T> rightExpandDim(long addedDims) const
        {
            if (addedDims < 0)
                throw std::invalid_argument("Number of dimensions to add has to be non-negative.");

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

        template <std::convertible_to<long>... Pack>
        Array<T> reshape(Pack... shape)
        {
            return reshape(Coordinates({shape...}));
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
            Assign<T> op(value);
            computeInPlace<Assign<T>>(op, *this);
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
            return oneHot<U>(Array<T>::range(reduceMin().eval(), reduceMax().eval() + 1));
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

    public:
        static inline T add(const T a, const T b) { return a + b; }
        static inline T multiply(const T a, const T b) { return a * b; }
        static inline T modulo(const T a, const T b) { return a % b; }
        static inline T max(const T a, const T b) { return a > b ? a : b; }
        static inline T min(const T a, const T b) { return a < b ? a : b; }
        static inline bool any(bool a, const T b) { return a || (b != 0); }
        static inline bool all(bool a, const T b) { return a && (b != 0); }

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
        Array<bool> operator==(const Array<T> &other) const
        {
            return compute<Equality<T>>(*this, other);
        }

        Array<bool> operator!=(const Array<T> &other) const
        {
            return compute<Inequality<T>>(*this, other);
        }

        Array<bool> operator<(const Array<T> &other) const
        {
            return compute<LessThan<T>>(*this, other);
        }

        Array<bool> operator<=(const Array<T> &other) const
        {
            return compute<LessThanEqual<T>>(*this, other);
        }

        Array<bool> operator>(const Array<T> &other) const
        {
            return compute<LessThan<T>>(other, *this);
        }

        Array<bool> operator>=(const Array<T> &other) const
        {
            return compute<LessThanEqual<T>>(other, *this);
        }

        Array<T> operator&&(const Array<T> &other) const
        {
            return compute<LogicalAnd<T>>(*this, other);
        }

        Array<T> operator||(const Array<T> &other) const
        {
            return compute<LogicalOr<T>>(*this, other);
        }

        Array<T> &operator+=(const Array<T> &other)
        {
            return computeInPlace<Addition<T>>(*this, *this, other);
        }

        Array<T> operator+(const Array<T> &other) const
        {
            return compute<Addition<T>>(*this, other);
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
            return computeInPlace<Multiplication<T>>(*this, *this, other);
        }

        Array<T> operator*(const Array<T> &other) const
        {
            return compute<Multiplication<T>>(*this, other);
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
            return computeInPlace<Subtraction<T>>(*this, *this, other);
        }

        Array<T> operator-(const Array<T> &other) const
        {
            return compute<Subtraction<T>>(*this, other);
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
            return computeInPlace<Division<T>>(*this, *this, other);
        }

        Array<T> operator/(const Array<T> &other) const
        {
            return compute<Division<T>>(*this, other);
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
            return compute<Modulo>(*this, other);
        }

    private:
        static inline bool find_isZero(T a) { return a == 0; }
        static inline bool find_isNonZero(T a) { return a != 0; }

    public:
        Array<bool> isNaN() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be NaN.");
            return compute<IsNan<T>>(*this);
        }

        Array<bool> isInf() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be Inf.");
            return compute<IsInf<T>>(*this);
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

        // I know, I know, this is probably not worth it
        Array<T> intPow(const unsigned int k) const
        {
            if (k == 0)
                return Array<T>::constant(mShape, 1);

            if (k == 1)
                return copy();

            if (k == 2)
                return compute<IntPow<T, 2>>(*this);

            if (k == 3)
                return compute<IntPow<T, 3>>(*this);

            if (k == 4)
                return compute<IntPow<T, 4>>(*this);

            return this->pow(Array<T>(k));
        }

        Array<T> square() const
        {
            return (*this) * (*this);
        }

    public:
        Array<T> pow(const Array<T> &other) const
        {
            return compute<Pow<T, T>>(*this, other);
        }

        Array<T> exp() const
        {
            static_assert(std::is_floating_point_v<T>, "Only floating points can be exponentiated.");
            return compute<Exp<T>>(*this);
        }

        Array<T> sqrt() const
        {
            return compute<Sqrt<T>>(*this);
        }

        Array<T> sin() const
        {
            return compute<Sin<T>>(*this);
        }

        Array<T> cos() const
        {
            return compute<Cos<T>>(*this);
        }

        Array<T> abs() const
        {
            return compute<Abs<T>>(*this);
        }

        Array<T> clip(T lower, T upper) const
        {
            return compute<Clip<T>>(Clip(lower, upper), *this);
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

        Array<T> sliceAxis(long axis, long from, long upto) const
        {
            if (mDim == 0)
                throw std::invalid_argument("Cannot slice a scalar.");

            if (from > mShape[axis] || from < -mShape[axis] || upto > mShape[axis] || upto < -mShape[axis])
                throw std::invalid_argument("Bounds have to be between -shape and shape.");

            axis = axis % mDim;
            if (axis < 0)
                axis += mDim;
            if (from < 0)
                from += mShape[axis];
            if (upto < 0)
                upto += mShape[axis];

            if (from > upto)
                throw std::invalid_argument("upto cannot be smaller than from mod shape.");

            Coordinates newShape(mShape);
            newShape[axis] = upto - from;
            Coordinates newStrides(mStrides);
            long flatlength = mFlatLength / mShape[axis] * newShape[axis];
            long offset = mOffset + from * mStrides[axis];
            bool contiguous = mContiguous;

            return Array<T>(mData, newShape, newStrides, offset, contiguous);
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
            long offset = mOffset;
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
            return *getDataPointer();
        }

        T &operator[](const Coordinates indices) const { return get(indices); }

        T &get(const Coordinates &indices) const
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

        T &getFlat(long i) const
        {
            long k = 0;
            for (long j = mDim - 1; j >= 0; j--)
            {
                k += mStrides[j] * (i % mShape[j]);
                i /= mShape[j];
            }

            return mData[k + mOffset];
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