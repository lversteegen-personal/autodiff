#ifndef ARRAY_BINARY_PTWS_H
#define ARRAY_BINARY_PTWS_H

#include "array.hpp"
#include "simd_vector.hpp"

namespace ArrayLibrary
{
    template <DataType T>
    template <DataType U, U (*f)(const T, const T)>
    inline void Array<T>::binaryCombineBoost(T *pLeftData, T *pRightData, U *pDestData, const long length, const long leftStride, const long rightStride, const long destStride)
    {
        for (long i = 0; i < length; i++)
        {
            *pDestData = f(*pLeftData, *pRightData);
            pLeftData += leftStride;
            pRightData += rightStride;
            pDestData += destStride;
        }
    }

    template <DataType T>
    template <DataType U, U (*f)(T, T)>
    void Array<T>::baseBinaryCombine(T *pLeftData, T *pRightData, U *pDestData, const Coordinates &leftShape, const Coordinates &rightShape, const Coordinates &destShape, const Coordinates &leftStrides, const Coordinates &rightStrides, const Coordinates &destStrides)
    {
        Coordinates outerShape = findOuterShape({leftShape, rightShape, destShape});
        const long dim = outerShape.size();
        long boostDim = -1;
        long boostDimLength = -1;

        for (int i = dim - 1; i >= 0; i--)
        {
            if (outerShape[i] > boostDimLength)
            {
                boostDim = i;
                boostDimLength = outerShape[i];
            }
        }

        const long leftBoostStride = leftShape[boostDim] == 1 ? 0 : leftStrides[boostDim];
        const long rightBoostStride = rightShape[boostDim] == 1 ? 0 : rightStrides[boostDim];
        const long destBoostStride = destShape[boostDim] == 1 ? 0 : destStrides[boostDim];

        Coordinates c(dim, 0);
        bool end = false;

        while (!end)
        {
            binaryCombineBoost<U, f>(pLeftData, pRightData, pDestData, boostDimLength, leftBoostStride, rightBoostStride, destBoostStride);

            end = true;

            for (long i = dim - 1; i >= 0; i--)
            {
                if (i == boostDim)
                    continue;

                c[i]++;

                if (c[i] != destShape[i])
                {
                    pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData + leftStrides[i];
                    pRightData = rightShape[i] == 1 ? pRightData : pRightData + rightStrides[i];
                    pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData - leftStrides[i] * (leftShape[i] - 1);
                    pRightData = rightShape[i] == 1 ? pRightData : pRightData - rightStrides[i] * (rightShape[i] - 1);
                    pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &), bool leftSkip, bool rightSkip>
    inline void Array<T>::simdBinaryCombineBoost(T *pLeftData, T *pRightData, U *pDestData, const long axisLength)
    {
        constexpr size_t LENGTH = SimdVector<T>::LENGTH;
        const long leftover = axisLength % LENGTH;

        SimdVector<T> left;
        if (leftSkip)
            left = SimdVector<T>::broadcast_set(*pLeftData);

        SimdVector<T> right;
        if (rightSkip)
            right = SimdVector<T>::broadcast_set(*pRightData);

        for (long i = 0; i + LENGTH <= axisLength; i += LENGTH)
        {
            if (!leftSkip)
            {
                left = SimdVector<T>::load(pLeftData);
                pLeftData += LENGTH;
            }
            if (!rightSkip)
            {
                right = SimdVector<T>::load(pRightData);
                pRightData += LENGTH;
            }
            SimdVector<U>::store(pDestData, fSimd(left, right));
            pDestData += LENGTH;
        }

        if (leftover)
        {
            auto mask = SimdVector<T>::makePrefixMask(leftover);
            if (!leftSkip)
                left = SimdVector<T>::maskedLoad(pLeftData, mask);
            if (!rightSkip)
                right = SimdVector<T>::maskedLoad(pRightData, mask);

            SimdVector<U>::maskedStore(pDestData, mask, fSimd(left, right));
        }
    }

    template <DataType T>
    template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &), bool leftSkip, bool rightSkip>
    void Array<T>::simdBinaryCombine(T *pLeftData, T *pRightData, U *pDestData, const Coordinates &leftShape, const Coordinates &rightShape, const Coordinates &destShape, const Coordinates &leftStrides, const Coordinates &rightStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength)
    {
        Coordinates c(destShape.size() - 1, 0);
        bool end = false;

        while (!end)
        {
            simdBinaryCombineBoost<U, fSimd, leftSkip, rightSkip>(pLeftData, pRightData, pDestData, flatBoostDimLength);

            end = true;

            for (long i = flatBoostDim; i >= 0; i--)
            {
                c[i]++;

                if (c[i] != destShape[i])
                {
                    pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData + leftStrides[i];
                    pRightData = rightShape[i] == 1 ? pRightData : pRightData + rightStrides[i];
                    pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData - leftStrides[i] * (leftShape[i] - 1);
                    pRightData = rightShape[i] == 1 ? pRightData : pRightData - rightStrides[i] * (rightShape[i] - 1);
                    pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, U (*f)(T, T), SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &)>
    Array<U> &Array<T>::binaryCombineDispatch(Array<U> &dest, const Array<T> &left, const Array<T> &right)
    {
        assertm(isSubshape(left.mShape, dest.mShape), "Left array is not a subshape of the destination array.");
        assertm(isSubshape(right.mShape, dest.mShape), "Right array is not a subshape of the destination array.");

        if (dest.getFlatLength() == 0)
            return dest;

        if (dest.mDim == 0)
        {
            *dest.getDataPointer() = f(left.mData[left.mOffset], right.mData[right.mOffset]);
            return dest;
        }

        const long dim = dest.mDim;

        U *pDestData = dest.getDataPointer();
        T *pLeftData = left.getDataPointer();
        T *pRightData = right.getDataPointer();

        Coordinates leftShape = left.mShape.shiftRight(1, dest.mDim - left.mDim);
        Coordinates leftStrides = left.mStrides.shiftRight(0, dest.mDim - left.mDim);
        Coordinates rightShape = right.mShape.shiftRight(1, dest.mDim - right.mDim);
        Coordinates rightStrides = right.mStrides.shiftRight(0, dest.mDim - right.mDim);
        const Coordinates &destShape = dest.refShape();
        const Coordinates &destStrides = dest.refStrides();

        if (fSimd == nullptr)
            baseBinaryCombine<U, f>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides);
        else
        {
            constexpr long flatLengthThreshold = 8;
            const bool leftSkip = leftShape[dim - 1] == 1;
            const bool rightSkip = rightShape[dim - 1] == 1;

            long i = dim - 1;
            long matchFlatLength = 1;

            while (i >= 0 && destStrides[i] == matchFlatLength && ((leftSkip && leftShape[i] == 1) || (!leftSkip && leftStrides[i] == matchFlatLength && leftShape[i] == destShape[i])) && ((rightSkip && rightShape[i] == 1) || (!rightSkip && rightStrides[i] == matchFlatLength && rightShape[i] == destShape[i])))
            {
                matchFlatLength *= destShape[i--];
            }

            if (matchFlatLength < 8)
                baseBinaryCombine<U, f>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides);
            else if (leftSkip && rightSkip)
                simdBinaryCombine<U, fSimd, true, true>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides, i, matchFlatLength);
            else if (leftSkip)
                simdBinaryCombine<U, fSimd, true, false>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides, i, matchFlatLength);
            else if (rightSkip)
                simdBinaryCombine<U, fSimd, false, true>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides, i, matchFlatLength);
            else
                simdBinaryCombine<U, fSimd, false, false>(pLeftData, pRightData, pDestData, leftShape, rightShape, destShape, leftStrides, rightStrides, destStrides, i, matchFlatLength);
        }

        return dest;
    }

    template <DataType T>
    template <T (*f)(T, T), SimdVector<T> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &)>
    Array<T> &Array<T>::binaryApply(const Array<T> &other)
    {
        BroadcastType broadcastType = broadcastRelationship(mShape, other.mShape);

        if ((broadcastType & BroadcastType::LEFT) == 0)
            throw std::invalid_argument("Other array is not a subshape of this array.");

        if (SimdVector<T>::supported)
            return binaryCombineDispatch<T, f, fSimd>(*this, *this, other);
        else
            return binaryCombineDispatch<T, f, nullptr>(*this, *this, other);
    }

    template <DataType T>
    template <DataType U, U (*f)(T, T), SimdVector<U> (*fSimd)(const SimdVector<T> &, const SimdVector<T> &)>
    Array<U> Array<T>::binaryCombine(const Array<T> &left, const Array<T> &right)
    {
        BroadcastType broadcastType = broadcastRelationship(left.mShape, right.mShape);

        if (broadcastType == BroadcastType::NONE)
            throw std::invalid_argument("Shapes cannot be broadcasted to match.");

        auto resultShape = broadcastShape(left.mShape, right.mShape);
        auto result = Array<U>(Data<U>(calculateFlatLength(resultShape)), resultShape);

        if (SimdVector<U>::supported && SimdVector<T>::supported)
            return binaryCombineDispatch<U, f, fSimd>(result, left, right);
        else
            return binaryCombineDispatch<U, f, nullptr>(result, left, right);
    }
}

#endif
