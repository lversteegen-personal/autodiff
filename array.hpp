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

#include <cassert>
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert((void(msg), exp))

#include "data.hpp"
#include "broadcast_type.hpp"
#include "stack_buffer.hpp"

#define MAX_DIM 8ul
typedef StackBuffer<long, MAX_DIM> Coordinates;

template <typename T>
class Array
{
    template <typename U>
    friend class Array;

    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

public:
    static long calculateFlatLength(const Coordinates &shape)
    {
        long flatLength = 1;
        for (long i = 0; i < shape.size(); i++)
        {
            flatLength *= shape[i];
        }
        return flatLength;
    }

private:
    Data<T> mData;

    long mFlatLength;

public:
    long getFlatLength() const { return mFlatLength; }

private:
    Coordinates mShape;
    Coordinates mStrides;
    bool transposed = false;

private:
    int mDim;

public:
    int getDim() const { return mDim; }

public:
    const Coordinates &refShape() const { return mShape; }

    ~Array() = default;

    Array<T>() = delete;

    Array<T>(const Array<T> &other) : mData(other.mData),
                                      mFlatLength(other.mFlatLength), mShape(other.mShape), mStrides(other.mStrides), mDim(other.mDim)
    {
    }

    Array<T>(Array<T> &&other) = default;

private:
    Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
    {
    }

public:
    Array<T>(T single) : mData(Data<T>(1)), mFlatLength(1), mShape(0), mStrides(0), mDim(0)
    {
        mData[0] = single;
    }

    Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
    {
        calculateStrides();
    }

    Array<T>(const Data<T> &data) : mData(data), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1) {}

    Array<T>(const std::vector<T> &data) : mData(data), mShape({data.size()}), mFlatLength(data.size()), mStrides({1}), mDim(1) {}

    Array<T>(const std::initializer_list<T> &values) : mData(values), mShape({mData.size()}), mFlatLength(mData.size()), mStrides({1}), mDim(1) {}

    Array<T> copy() const
    {
        if (transposed)
        {
            auto result = Array<T>(Data<T>(mFlatLength), mShape, mStrides);
            return destUnary<copy>(result, *this);
        }
        else
            return Array<T>(mData.copy(), mShape, mStrides);
    }

    bool isTransposed() const { return transposed; }
    Array<T> transpose(int axis1, int axis2) const
    {
        axis1 = axis1 % mDim;
        axis2 = axis2 % mDim;
        axis1 = axis1 < 0 ? mDim + axis1 : axis1;
        axis2 = axis2 < 0 ? mDim + axis2 : axis2;

        Array<T> result(*this);
        result.transposed = true;
        std::swap(result.mShape[axis1], result.mShape[axis2]);
        std::swap(result.mStrides[axis1], result.mStrides[axis2]);
        return result;
    }

    Array<T> reshape(const Coordinates &shape) const
    {
        if (transposed)
            throw std::logic_error("Cannot reshape a transposed array.");

        long flatLength = 1;
        long slackDimension = -1;
        for (size_t i = 0; i < shape.size(); i++)
        {
            if (shape[i] == -1)
            {
                if (slackDimension != -1)
                    throw std::invalid_argument("Only one dimension can be -1.");
                slackDimension = i;
            }
            else
                flatLength *= shape[i];
        }
        if (slackDimension == -1 && flatLength == mFlatLength)
        {
            return Array<T>(mData, shape);
        }
        else if (slackDimension != -1 && flatLength <= mFlatLength && mFlatLength % flatLength == 0)
        {
            Coordinates newShape(shape);
            newShape[slackDimension] = mFlatLength / flatLength;
            return Array<T>(mData, newShape);
        }
        else
            throw std::invalid_argument("Shape does not match data size.");
    }

    Array<T> &operator=(const Array<T> &other) = default;

    Array<T> &operator=(Array<T> &&other) = default;

    Array<T> &operator=(const T value)
    {
        for (size_t i = 0; i < getFlatLength(); i++)
        {
            mData[i] = value;
        }

        return *this;
    }

    static Coordinates calculateStrides(const Coordinates &shape)
    {
        int dim = shape.size();
        Coordinates strides(dim);
        size_t multiplier = 1;

        for (size_t i = 0; i < dim; i++)
        {
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
        size_t multiplier = 1;
        for (size_t i = 0; i < mDim; i++)
        {
            mStrides[mDim - i - 1] = multiplier;
            multiplier *= mShape[mDim - i - 1];
        }
    }

public:

    static Array<T> fromFlatLines(const std::vector<Data<T>> &lines)
    {
        if (lines.size() == 0)
            return Array<T>(Data<T>(0), Coordinates(0));

        auto length = lines[0].size();

        for (long l = 0; l < lines.size(); l++)
        {
            if (lines[l].size() != length)
                throw std::invalid_argument("All lines must have the same size.");
        }

        auto result = Array<T>(Data<T>(lines.size() * length), Coordinates({lines.size(), length}));

        T *pResultData = result.mData.mRaw;
        for (long l = 0; l < lines.size(); l++)
        {
            std::copy(lines[l].mRaw, lines[l].mRaw + length, pResultData);
            pResultData += length;
        }

        return result;
    }

    static Array<T> fromLines(const std::vector<Array<T>> &lines)
    {
        if (lines.size() == 0)
            return Array<T>(Data<T>(0), Coordinates(0));

        auto shape = lines[0].mShape;
        for (long l = 0; l < lines.size(); l++)
        {
            if (lines[l].mShape != shape)
                throw std::invalid_argument("All lines must have the same shape.");
        }

        auto flatLineLength = lines[0].mFlatLength;

        auto result = Array<T>(Data<T>(lines.size() * flatLineLength), Coordinates(lines.size(), shape));
        Coordinates c(shape.size(), 0);

        T *pResultData = result.mData.mRaw;

        for (long l = 0; l < lines.size(); l++)
        {
            T *pLineData = lines[l].mData.mRaw;
            c = 0;
            for (long k = 0; k < flatLineLength; k++)
            {
                *pResultData = *pLineData;

                for (long i = shape.size() - 1; i >= 0; i--)
                {
                    c[i]++;

                    if (shape[i] == 1 || c[i] % shape[i] == 0)
                        pLineData -= lines[l].mStrides[i] * (shape[i] - 1);
                    else
                        pLineData += lines[l].mStrides[i];

                    if (c[i] % shape[i] != 0)
                        break;
                }

                pResultData++;
            }
        }

        return result;
    }

    static Array<T> range(T from, T to, T step)
    {
        if (step == 0)
            step = 1;

        if ((to - from) / step < 0)
            step = -step;

        size_t count = std::ceil((to - from) / step);
        auto data = Data<T>(count);
        long i = 0;
        for (T a = from; a < to; a += step)
            data[i++] = a;

        return Array<T>(data);
    }

    static Array<T> range(T from, T to) { return Array<T>::range(from, to, 1); }

    static Array<T> range(T to) { return Array<T>::range(0, to, 1); }

    static Array<T> constant(const Coordinates &shape, const T constant)
    {
        size_t flatLength = 1;
        for (size_t i = 0; i < shape.size(); i++)
        {
            flatLength *= shape[i];
        }
        auto data = Data<T>(flatLength);
        data = constant;
        Array<T> x(data, shape);

        return x;
    }

    Array<T> extend(const Coordinates &shape) const
    {
        if (shape.size() != mDim)
            throw std::invalid_argument("The new shape must have the same number of dimensions as the old shape");
        if (!isSubshape(mShape, shape))
            throw std::invalid_argument("Other array is not a subshape of this array.");

        size_t flatLength = 1;
        Coordinates newStrides(mDim);

        for (long i = mDim - 1; i >= 0; i--)
        {
            newStrides[i] = flatLength;
            flatLength *= shape[i];
        }

        auto data = Data<T>(flatLength);
        size_t j = 0;

        Coordinates c(mDim, 0);

        for (size_t k = 0; k < flatLength; k++)
        {
            data[k] = mData[j];

            for (long i = mDim - 1; i >= 0; i--)
            {
                c[i]++;

                if (mShape[i] == 1 || c[i] % mShape[i] == 0)
                    j -= mStrides[i] * (mShape[i] - 1);
                else
                    j += mStrides[i];

                if (c[i] % shape[i] != 0)
                    break;
            }
        }

        return Array<T>(data, shape, newStrides);
    }

private:
    static inline T copy(T s) { return s; }

    template <typename U, T (*f)(T)>
    static Array<U> &destUnary(Array<U> &dest, const Array<T> &source)
    {
        assertm(isSubshape(source.mShape, dest.mShape), "Source array is not a subshape of the destination array.");

        U *pDestData = dest.mData.mRaw;
        T *pSourceData = source.mData.mRaw;

        Coordinates c(dest.mDim, 0);

        while (c[0] < dest.mShape[0])
        {
            *pDestData = f(*pSourceData);

            for (long i = dest.mDim - 1; i >= 0; i--)
            {
                c[i]++;

                if (dest.mShape[i] == 1 || c[i] % dest.mShape[i] == 0)
                    pDestData -= dest.mStrides[i] * (dest.mShape[i] - 1);
                else
                    pDestData += dest.mStrides[i];

                if (source.mShape[i] == 1 || c[i] % source.mShape[i] == 0)
                    pSourceData -= source.mStrides[i] * (source.mShape[i] - 1);
                else
                    pSourceData += source.mStrides[i];

                if (c[i] % dest.mShape[i] != 0)
                    break;
            }
        }

        return dest;
    }

    static inline T add(T a, T b) { return a + b; }
    static inline T multiply(T a, T b) { return a * b; }
    static inline T subtract(T a, T b) { return a - b; }
    static inline T divide(T a, T b) { return a / b; }
    static inline T modulo(T a, T b) { return a % b; }
    static inline T logical_and(T a, T b) { return a && b; }
    static inline T logical_or(T a, T b) { return a || b; }

    static inline bool equal(T a, T b) { return a == b; }
    static inline bool notEqual(T a, T b) { return a != b; }
    static inline bool less(T a, T b) { return a < b; }
    static inline bool lessEqual(T a, T b) { return a <= b; }
    static inline bool greater(T a, T b) { return a > b; }
    static inline bool greaterEqual(T a, T b) { return a >= b; }

    template <typename U, U (*f)(T, T)>
    static Array<U> &binaryDestCombine(Array<U> &dest, const Array<T> &left, const Array<T> &right)
    {
        assertm(isSubshape(left.mShape, dest.mShape), "Left array is not a subshape of the destination array.");
        assertm(isSubshape(right.mShape, dest.mShape), "Right array is not a subshape of the destination array.");

        U *pDestData = dest.mData.mRaw;
        T *pLeftData = left.mData.mRaw;
        T *pRightData = right.mData.mRaw;

        long maxDim = std::max({dest.mDim, left.mDim, right.mDim});
        long minDimDest = maxDim - dest.mDim;
        long minDimLeft = maxDim - left.mDim;
        long minDimRight = maxDim - right.mDim;

        Coordinates c(dest.mDim, 0);

        while (c[0] < dest.mShape[0])
        {
            *pDestData = f(*pLeftData, *pRightData);

            for (long i = dest.mDim - 1; i >= 0; i--)
            {
                c[i]++;

                if (i < minDimDest)
                    ;
                else if (dest.mShape[i] == 1 || c[i] % dest.mShape[i] == 0)
                    pDestData -= dest.mStrides[i] * (dest.mShape[i] - 1);
                else
                    pDestData += dest.mStrides[i];

                if (i < minDimLeft)
                    ;
                else if (left.mShape[i] == 1 || c[i] % left.mShape[i] == 0)
                    pLeftData -= left.mStrides[i] * (left.mShape[i] - 1);
                else
                    pLeftData += left.mStrides[i];

                if (i < minDimRight)
                    ;
                else if (right.mShape[i] == 1 || c[i] % right.mShape[i] == 0)
                    pRightData -= right.mStrides[i] * (right.mShape[i] - 1);
                else
                    pRightData += right.mStrides[i];

                if (c[i] % dest.mShape[i] != 0)
                    break;
            }
        }

        return dest;
    }

public:
    Array<T> &applyFunction(const std::function<T(T)> f)
    {
        for (size_t k = 0; k < mFlatLength; k++)
        {
            mData[k] = f(mData[k]);
        }

        return *this;
    }

    static Array<T> applyFunction(const Array<T> &array, const std::function<T(T)> f)
    {
        auto data = Data<T>(array.mFlatLength);
        for (size_t k = 0; k < array.mFlatLength; k++)
        {
            data[k] = f(array.mData[k]);
        }

        return Array<T>(data, array.mShape, array.mStrides);
    }

    template <T (*f)(T, T)>
    Array<T> &binaryApply(const Array<T> &other)
    {
        BroadcastType broadcastType = broadcastRelationship(mShape, other.mShape);

        if ((broadcastType & BroadcastType::LEFT) == 0)
            throw std::invalid_argument("Other array is not a subshape of this array.");

        binaryDestCombine<T, f>(*this, *this, other);

        return *this;
    }

    template <typename U, U (*f)(T, T)>
    static Array<U> binaryCombine(const Array<T> &left, const Array<T> &right)
    {
        BroadcastType broadcastType = broadcastRelationship(left.mShape, right.mShape);

        if (broadcastType == BroadcastType::NONE)
            throw std::invalid_argument("Shapes cannot be broadcasted to match.");

        auto resultShape = broadcastShape(left.mShape, right.mShape);
        auto result = Array<U>(Data<U>(calculateFlatLength(resultShape)), resultShape);

        return binaryDestCombine<U, f>(result, left, right);
    }

    static Coordinates broadcastShape(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long dim = std::max(dim1, dim2);
        long offset1 = dim - dim1, offset2 = dim - dim2;
        Coordinates result(dim);

        for (long i = 0; i < dim; i++)
        {
            if (i < offset1)
                result[i] = shape2[i];
            else if (i < offset2)
                result[i] = shape1[i];
            else
                result[i] = std::max(shape1[i + offset1], shape2[i + offset2]);
        }

        return result;
    }

public:
    /// @brief Returns a broadcast type stating how the two shapes can be broadcasted to match each other.
    /// @param shape1 Left shape
    /// @param shape2 Right shape
    /// @return The broadcast type, see commentary of BroadcastType for explanation of flags.
    static BroadcastType broadcastRelationship(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long minDim = std::min(dim1, dim2);
        long offset1 = dim1 - minDim, offset2 = dim2 - minDim;
        BroadcastType result = BroadcastType::MATCH;

        if (offset1 > 0)
            result &= BroadcastType::LEFTMIX;
        if (offset2 > 0)
            result &= BroadcastType::RIGHTMIX;

        for (long i = minDim - 1; i >= 0; i--)
        {
            if (shape1[i + offset1] != shape2[i + offset2])
            {
                if (shape1[i + offset1] == 1)
                    result &= BroadcastType::RIGHTMIX;
                else if (shape2[i + offset2] == 1)
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
        return binaryApply<add>(other);
    }

    Array<T> operator+(const Array<T> &other) const
    {
        return binaryCombine<T, add>(*this, other);
    }

    Array<T> &operator+=(const T other)
    {
        for (size_t i = 0; i < getFlatLength(); i++)
            mData[i] += other;

        return *this;
    }

    Array<T> operator+(const T other) const
    {
        auto result = copy();
        result += other;

        return result;
    }

    Array<T> &operator*=(const Array<T> &other)
    {
        return binaryApply<multiply>(other);
    }

    Array<T> operator*(const Array<T> &other) const
    {
        return binaryCombine<T, multiply>(*this, other);
    }

    Array<T> &operator*=(const T other)
    {
        for (size_t i = 0; i < getFlatLength(); i++)
            mData[i] *= other;

        return *this;
    }

    Array<T> operator*(const T other) const
    {
        auto result = copy();
        result *= other;

        return result;
    }

    Array<T> &operator-=(const Array<T> &other)
    {
        return binaryApply<subtract>(other);
    }

    Array<T> operator-(const Array<T> &other) const
    {
        return binaryCombine<T, subtract>(other);
    }

    Array<T> &operator-=(const T other)
    {
        for (size_t i = 0; i < getFlatLength(); i++)
            mData[i] -= other;

        return *this;
    }

    Array<T> operator-(const T other) const
    {
        auto result = copy();
        result -= other;

        return result;
    }

    Array<T> &operator/=(const Array<T> &other)
    {
        return binaryApply<divide>(other);
    }

    Array<T> operator/(const Array<T> &other) const
    {
        return binaryCombine<T, divide>(other);
    }

    Array<T> &operator/=(const T other)
    {
        for (size_t i = 0; i < getFlatLength(); i++)
            mData[i] /= other;

        return *this;
    }

    Array<T> operator/(const T other) const
    {
        auto result = copy();
        result /= other;

        return result;
    }

    Array<T> operator%(const Array<T> &other) const
    {
        static_assert(std::is_integral_v<T>, "Modulo operator is only defined for integral types.");
        return binaryCombine<T, modulo>(*this, other);
    }

    static Coordinates matmulShape(const Coordinates &leftShape, const Coordinates &rightShape, long leftProductAxis = -1, long rightProductAxis = -2)
    {
        if (leftShape.size() != rightShape.size())
            throw std::invalid_argument("Arrays do not have the same number of dimensions");
        if (leftProductAxis == rightProductAxis)
            throw std::invalid_argument("leftProductAxis must be different from rightProductAxis");
        long dim = leftShape.size();

        rightProductAxis = rightProductAxis % dim;
        leftProductAxis = leftProductAxis % dim;
        rightProductAxis = rightProductAxis < 0 ? dim + rightProductAxis : rightProductAxis;
        leftProductAxis = leftProductAxis < 0 ? dim + leftProductAxis : leftProductAxis;

        if (leftShape[leftProductAxis] != rightShape[rightProductAxis])
            throw std::invalid_argument("Arrays do not have the same length in product dimension.");

        Coordinates resultShape(dim);

        for (size_t i = 0; i < dim; i++)
        {
            if (i == rightProductAxis)
                resultShape[i] = leftShape[i];
            else if (i == leftProductAxis)
                resultShape[i] = rightShape[i];
            else if (leftShape[i] == rightShape[i] || leftShape[i] == 1)
                resultShape[i] = rightShape[i];
            else if (rightShape[i] == 1)
                resultShape[i] = leftShape[i];
            else
                throw std::invalid_argument("Arrays do not have compatible shapes for matrix multiplication.");
        }

        return resultShape;
    }

    static Array<T> matmul(const Array<T> &left, const Array<T> &right, long leftProductAxis = -1, long rightProductAxis = -2)
    {
        if (left.mDim != right.mDim)
            throw std::invalid_argument("Arrays do not have the same number of dimensions");
        if (leftProductAxis == rightProductAxis)
            throw std::invalid_argument("leftProductAxis must be different from rightProductAxis");
        int dim = left.mDim;

        rightProductAxis = rightProductAxis % dim;
        leftProductAxis = leftProductAxis % dim;
        rightProductAxis = rightProductAxis < 0 ? dim + rightProductAxis : rightProductAxis;
        leftProductAxis = leftProductAxis < 0 ? dim + leftProductAxis : leftProductAxis;

        if (left.mShape[leftProductAxis] != right.mShape[rightProductAxis])
            throw std::invalid_argument("Arrays do not have the same length in product dimension.");

        Coordinates leftOuterShape(dim - 2);
        Coordinates rightOuterShape(dim - 2);
        size_t j = 0;
        for (size_t i = 0; i < dim; i++)
        {
            if (i != rightProductAxis && i != leftProductAxis)
            {
                leftOuterShape[j] = left.mShape[i];
                rightOuterShape[j] = right.mShape[i];
                j++;
            }
        }

        BroadcastType broadcastType = broadcastRelationship(leftOuterShape, rightOuterShape);
        if (broadcastType == BroadcastType::NONE)
            throw std::invalid_argument("Arrays do not have compatible shapes for matrix multiplication.");

        auto outerBroadcastShape = broadcastShape(leftOuterShape, rightOuterShape);
        Coordinates resultShape(dim);
        size_t resultFlatLength = 1;

        j = 0;
        for (size_t i = 0; i < dim; i++)
        {
            if (i == rightProductAxis)
                resultShape[i] = left.mShape[i];
            else if (i == leftProductAxis)
                resultShape[i] = right.mShape[i];
            else
            {
                resultShape[i] = outerBroadcastShape[j];
                j++;
            }

            resultFlatLength *= resultShape[i];
        }

        auto resultStrides = calculateStrides(resultShape);

        auto resultData = Data<T>(resultFlatLength);
        T *pLeftData = left.mData.mRaw, *pRightData = right.mData.mRaw, *pResultData = resultData.mRaw;
        size_t productAxisLength = left.mShape[leftProductAxis];
        size_t leftProductStride = left.mStrides[leftProductAxis], rightProductStride = right.mStrides[rightProductAxis];
        Coordinates c(dim, 0);

        for (size_t k = 0; k < resultFlatLength; k++)
        {
            resultData[k] = innerProduct(pLeftData, pRightData, leftProductStride, rightProductStride, productAxisLength);

            for (long i = dim - 1; i >= 0; i--)
            {
                c[i]++;

                if (i != rightProductAxis)
                {
                    if (right.mShape[i] == 1 || c[i] % right.mShape[i] == 0)
                        pRightData -= right.mStrides[i] * (right.mShape[i] - 1);
                    else
                        pRightData += right.mStrides[i];
                }
                if (i != leftProductAxis)
                {
                    if (left.mShape[i] == 1 || c[i] % left.mShape[i] == 0)
                        pLeftData -= left.mStrides[i] * (left.mShape[i] - 1);
                    else
                        pLeftData += left.mStrides[i];
                }

                if (c[i] % resultShape[i] != 0)
                    break;
            }
        }

        return Array<T>(resultData, resultShape, resultStrides);
    }

private:
    inline static T innerProduct(T *pLeftData, T *pRightData, size_t leftStride, size_t rightStride, size_t length)
    {
        T sum = 0;

        for (size_t k = 0; k < length; k++)
        {
            sum += *pLeftData * *pRightData;
            pLeftData += leftStride;
            pRightData += rightStride;
        }

        pLeftData -= leftStride * length;
        pRightData -= rightStride * length;

        return sum;
    }

    static inline bool find_isZero(T a) { return a == 0; }
    static inline bool find_isNonZero(T a) { return a != 0; }

public:
    template <bool (*f)(T)>
    Array<long> findWhere() const
    {
        auto list = std::vector<long>();
        Coordinates c(mDim, 0);

        T *pData = mData.mRaw;

        for (size_t k = 0; k < mFlatLength; k++)
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

    Array<T> pow(const unsigned int k) const
    {
        if (k > 4)
            return this->pow((double)k);

        auto resultData = Data<T>(mData);

        for (size_t i = 0; i < getFlatLength(); i++)
            for (size_t j = 0; j < k; j++)
                resultData[i] *= mData[i];

        auto result = Array<T>(resultData, mShape);

        return result;
    }

    Array<T> pow(double y) const
    {
        auto resultData = Data<T>(mFlatLength);

        for (size_t i = 0; i < getFlatLength(); i++)
            resultData[i] = (T)std::pow(mData[i], y);

        auto result = Array<T>(resultData, mShape);

        return result;
    }

public:
    Array<T> pow(const Array<T> &y) const
    {
        auto resultData = Data<T>(getFlatLength());

        for (size_t i = 0; i < getFlatLength(); i++)
            resultData[i] = (T)std::pow(mData[i], y.mData[i]);

        return Array<T>(resultData, mShape);
    }

    Array<T> reduceSum(const Coordinates &axes, bool leaveOnes = false) const
    {
        bool reduce[MAX_DIM] = {false};
        long j = 0;
        for (int i = 0; i < axes.size(); i++)
        {
            j = axes[i] % mDim;
            j = j < 0 ? mDim + j : j;
            reduce[j] = true;
        }

        int newDim;
        if (leaveOnes)
            newDim = mDim;
        else
        {
            newDim = mDim - axes.size();
            if (newDim == 0)
            {
                T sum = 0;
                for (size_t i = 0; i < mFlatLength; i++)
                    sum += mData[i];
                return Array<T>(Data<T>({sum}));
            }
        }

        Coordinates newShape(newDim);
        Coordinates putStrides(mDim, 0);
        j = newDim - 1;
        long multiplier = 1;

        for (int i = mDim - 1; i >= 0; i--)
            if (!reduce[i])
            {
                newShape[j--] = mShape[i];
                putStrides[i] = multiplier;
                multiplier *= mShape[i];
            }
            else if (leaveOnes)
                newShape[j--] = 1;

        long flatLength = calculateFlatLength(newShape);
        auto data = Data<T>(flatLength);
        data = 0;

        Coordinates c(mDim, 0);
        long m = 0, n = 0;

        for (long k = 0; k < mFlatLength; k++)
        {
            data[m] += mData[n];

            for (int i = mDim - 1; i >= 0; i--)
            {
                c[i]++;

                if (c[i] % mShape[i] == 0)
                {
                    c[i] = 0;
                    n -= mStrides[i] * (mShape[i] - 1);
                    m -= putStrides[i] * (mShape[i] - 1);
                }
                else
                {
                    n += mStrides[i];
                    m += putStrides[i];
                    break;
                }
            }
        }

        return Array<T>(data, newShape);
    }

    Array<T> reduceSum() const
    {
        Coordinates axes(mDim);
        for (int i = 0; i < mDim; i++)
            axes[i] = i;

        return reduceSum(axes);
    }

    T &operator[](const Coordinates indices) const { return get(indices); }

    T &operator[](const int i) const { return getFlat(i); }

    T &get(const Coordinates indices) const
    {
        if (indices.size() != mDim)
            throw std::invalid_argument("The index tuple does not match the array shape");
        size_t combinedIndex = 0;
        long ix;

        for (size_t i = 0; i < mDim; i++)
        {
            ix = indices[i] % mShape[i];
            if (ix < 0)
                ix += mShape[i];
            combinedIndex += mStrides[i] * ix;
        }

        return mData[combinedIndex];
    }

    T &getFlat(size_t i) const
    {
        size_t k = 0;
        for (long j = mDim - 1; j >= 0; j--)
        {
            k += mStrides[j] * (i % mShape[j]);
            i /= mShape[j];
        }

        return mData[k];
    }

    std::string to_string() const
    {
        if (getFlatLength() > 100)
            return "Output too long. Flat length is " + std::to_string(getFlatLength());

        std::ostringstream oss;
        size_t m = 0;

        for (size_t i = 0; i < getFlatLength(); i++)
        {
            m = i;
            for (long j = mDim - 1; j >= 0; j--)
            {
                if (m % mShape[j] == 0)
                    oss << "[";
                else
                    break;
                m /= mShape[j];
            }

            oss << "\t" << mData[i];

            m = i;

            for (long j = mDim - 1; j >= 0; j--)
            {
                if ((m + 1) % mShape[j] == 0)
                    oss << "]";
                else
                {
                    oss << "," << std::string(mDim - 1 - j, '\n');
                    break;
                }
                m /= mShape[j];
            }
        }

        return oss.str();
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &s, const Array<T> &x)
{
    return s << x.to_string();
}

template <typename INT, typename T>
Array<T> operator+(const INT single, const Array<T> &array)
{
    static_assert(std::is_arithmetic_v<INT>, "INT must be arithmetic type!");
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");
    return array + single;
}

template <typename INT, typename T>
Array<T> operator*(const INT single, const Array<T> &array)
{
    static_assert(std::is_arithmetic_v<INT>, "INT must be arithmetic type!");
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");
    return array * single;
}

template <typename INT, typename T>
Array<T> operator-(const INT single, const Array<T> &array)
{
    static_assert(std::is_arithmetic_v<INT>, "INT must be arithmetic type!");
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");
    return array - single;
}

template <typename INT, typename T>
Array<T> operator/(const INT single, const Array<T> &array)
{
    static_assert(std::is_arithmetic_v<INT>, "INT must be arithmetic type!");
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");
    return array / single;
}

#endif