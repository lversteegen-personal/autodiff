#ifndef ARRAY_UNARY_PTW
#define ARRAY_UNARY_PTW

#include "array.hpp"
#include "simd_vector.hpp"

namespace ArrayLibrary
{
    template <DataType T>
    template <T (*f)(const T), SimdVector<T> (*fSimd)(const SimdVector<T> &)>
    Array<T> &Array<T>::unaryApply()
    {
        if (SimdVector<T>::supported)
            return unaryDestDispatch<T, f, fSimd>(*this, *this);
        else
            return unaryDestDispatch<T, f>(*this, *this);
    }

    template <DataType T>
    template <typename P, T (*f)(const T, const P &), typename Q, SimdVector<T> (*fSimd)(const SimdVector<T> &, const Q &)>
    Array<T> &Array<T>::unaryParamApply(const P &param, const Q &simdParam)
    {
        if (SimdVector<T>::supported)
            return unaryParamDestDispatch<T, P, f, Q, fSimd>(*this, *this, param, simdParam);
        else
            return unaryParamDestDispatch<T, P, f>(*this, *this, param, simdParam);
    }

    template <DataType T>
    template <typename P, T (*f)(const T, const P &)>
    Array<T> &Array<T>::unaryParamApply(const P &param)
    {
        return unaryParamDestDispatch<T, P, f, bool>(*this, *this, param, false);
    }

    template <DataType T>
    template <DataType U, U (*f)(const T), SimdVector<U> (*fSimd)(const SimdVector<T> &)>
    Array<U> Array<T>::unaryCompute(const Array<T> &source)
    {
        auto result = Array<U>(Data<U>(source.mFlatLength), source.mShape, source.mStrides, 0, true);
        if (SimdVector<U>::supported && SimdVector<T>::supported)
            return unaryDestDispatch<U, f, fSimd>(source, result);
        else
            return unaryDestDispatch<U, f, nullptr>(source, result);
    }

    template <DataType T>
    template <DataType U, typename P, U (*f)(const T, const P &), typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &)>
    Array<U> Array<T>::unaryParamCompute(const Array<T> &source, const P &param, const Q &simdParam)
    {
        auto result = Array<U>(Data<U>(source.mFlatLength), source.mShape, source.mStrides, 0, true);
        if (SimdVector<U>::supported && SimdVector<T>::supported)
            return unaryParamDestDispatch<U, P, f, Q, fSimd>(source, result, param, simdParam);
        else
            return unaryParamDestDispatch<U, P, f, Q>(source, result, param, simdParam);
    }

    template <DataType T>
    template <DataType U, typename P, U (*f)(const T, const P &)>
    Array<U> Array<T>::unaryParamCompute(const Array<T> &source, const P &param)
    {
        auto result = Array<U>(Data<U>(source.mFlatLength), source.mShape, source.mStrides, 0, true);
        return unaryParamDestDispatch<U, P, f, bool>(source, result, param, false);
    }

    template <DataType T>
    template <DataType U, U (*f)(const T), SimdVector<U> (*fSimd)(const SimdVector<T> &)>
    Array<U> &Array<T>::unaryDestDispatch(const Array<T> &source, Array<U> &dest)
    {
        assertm(isSubshape(source.mShape, dest.mShape), "Source array is not a subshape of the destination array.");

        if (dest.mDim == 0)
        {
            dest.mData[dest.mOffset] = f(source.mData[source.mOffset]);
            return dest;
        }

        U *pDestData = dest.getDataPointer();
        T *pSourceData = source.getDataPointer();

        Coordinates sourceShape = source.mShape.shiftRight(1, dest.mDim - source.mDim);
        Coordinates sourceStrides = source.mStrides.shiftRight(0, dest.mDim - source.mDim);
        const Coordinates &destShape = dest.refShape();
        const Coordinates &destStrides = dest.refStrides();

        if (fSimd == nullptr)
            baseUnary<U, f>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides);
        else
        {
            constexpr long flatLengthThreshold = 8;
            const bool sourceSkip = sourceShape[dest.mDim - 1] == 1;

            long i = dest.mDim - 1;
            long matchFlatLength = 1;

            while (i >= 0 && destStrides[i] == matchFlatLength && ((sourceSkip && sourceShape[i] == 1) || (!sourceSkip && sourceStrides[i] == matchFlatLength && sourceShape[i] == destShape[i])))
            {
                matchFlatLength *= destShape[i--];
            }

            if (matchFlatLength < 8)
                baseUnary<U, f>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides);
            else if (sourceSkip)
                simdUnary<U, fSimd, true>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, i, matchFlatLength);
            else
                simdUnary<U, fSimd, false>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, i, matchFlatLength);
        }

        return dest;
    }

    template <DataType T>
    template <DataType U, typename P, U (*f)(const T, const P &), typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &)>
    Array<U> &Array<T>::unaryParamDestDispatch(const Array<T> &source, Array<U> &dest, const P &param, const Q &simdParam)
    {
        assertm(isSubshape(source.mShape, dest.mShape), "Source array is not a subshape of the destination array.");

        if (dest.mDim == 0)
        {
            dest.mData[dest.mOffset] = f(source.mData[source.mOffset], param);
            return dest;
        }

        U *pDestData = dest.getDataPointer();
        T *pSourceData = source.getDataPointer();

        Coordinates sourceShape = source.mShape.shiftRight(1, dest.mDim - source.mDim);
        Coordinates sourceStrides = source.mStrides.shiftRight(0, dest.mDim - source.mDim);
        const Coordinates &destShape = dest.refShape();
        const Coordinates &destStrides = dest.refStrides();

        if (fSimd == nullptr)
            baseParamUnary<U, P, f>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, param);
        else
        {
            constexpr long flatLengthThreshold = 8;
            const bool sourceSkip = sourceShape[dest.mDim - 1] == 1;

            long i = dest.mDim - 1;
            long matchFlatLength = 1;

            while (i >= 0 && destStrides[i] == matchFlatLength && ((sourceSkip && sourceShape[i] == 1) || (!sourceSkip && sourceStrides[i] == matchFlatLength && sourceShape[i] == destShape[i])))
            {
                matchFlatLength *= destShape[i--];
            }

            if (matchFlatLength < 8)
                baseParamUnary<U, P, f>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, param);
            else if (sourceSkip)
                simdUnaryParam<U, Q, fSimd, true>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, i, matchFlatLength, simdParam);
            else
                simdUnaryParam<U, Q, fSimd, false>(pSourceData, pDestData, sourceShape, destShape, sourceStrides, destStrides, i, matchFlatLength, simdParam);
        }

        return dest;
    }

    template <DataType T>
    template <DataType U, U (*f)(const T)>
    void Array<T>::baseUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides)
    {
        long boostDim = -1;
        long boostDimLength = -1;

        for (int i = destShape.size() - 1; i >= 0; i--)
        {
            if (destShape[i] > boostDimLength)
            {
                boostDim = i;
                boostDimLength = destShape[i];
            }
        }

        const long dim = destShape.size();
        const long sourceBoostStride = sourceStrides[boostDim];
        const long destBoostStride = destStrides[boostDim];

        Coordinates c(dim, 0);
        bool end = false;

        while (!end)
        {
            unaryBoost<U, f>(pSourceData, pDestData, boostDimLength, sourceBoostStride, destBoostStride);
            end = true;

            for (long i = dim - 1; i >= 0; i--)
            {
                if (i == boostDim)
                    continue;

                c[i]++;

                if (c[i] != destShape[i])
                {
                    pDestData += destStrides[i];
                    pSourceData += sourceStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pDestData -= destStrides[i] * (destShape[i] - 1);
                    pSourceData -= sourceStrides[i] * (sourceShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, typename P, U (*f)(const T, const P &)>
    void Array<T>::baseParamUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const P param)
    {
        long boostDim = -1;
        long boostDimLength = -1;

        for (int i = destShape.size() - 1; i >= 0; i--)
        {
            if (destShape[i] > boostDimLength)
            {
                boostDim = i;
                boostDimLength = destShape[i];
            }
        }

        const long dim = destShape.size();
        const long sourceBoostStride = sourceStrides[boostDim];
        const long destBoostStride = destStrides[boostDim];

        Coordinates c(dim, 0);
        bool end = false;

        while (!end)
        {
            unaryParamBoost<U, P, f>(pSourceData, pDestData, boostDimLength, sourceBoostStride, destBoostStride, param);
            end = true;

            for (long i = dim - 1; i >= 0; i--)
            {
                if (i == boostDim)
                    continue;

                c[i]++;

                if (c[i] != destShape[i])
                {
                    pDestData += destStrides[i];
                    pSourceData += sourceStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pDestData -= destStrides[i] * (destShape[i] - 1);
                    pSourceData -= sourceStrides[i] * (sourceShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, U (*f)(const T)>
    inline void Array<T>::unaryBoost(const T *pSourceData, U *pDestData, const long length, const long sourceStride, const long destStride)
    {
        for (long i = 0; i < length; i++)
        {
            *pDestData = f(*pSourceData);
            pSourceData += sourceStride;
            pDestData += destStride;
        }
    }

    template <DataType T>
    template <DataType U, typename P, U (*f)(const T, const P &)>
    inline void Array<T>::unaryParamBoost(const T *pSourceData, U *pDestData, const long length, const long sourceStride, const long destStride, const P &param)
    {
        for (long i = 0; i < length; i++)
        {
            *pDestData = f(*pSourceData, param);
            pSourceData += sourceStride;
            pDestData += destStride;
        }
    }

    template <DataType T>
    template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &), bool sourceSkip>
    void Array<T>::simdUnary(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength)
    {
        Coordinates c(destShape.size() - 1, 0);
        bool end = false;

        while (!end)
        {
            simdUnaryBoost<U, fSimd, sourceSkip>(pSourceData, pDestData, flatBoostDimLength);

            end = true;

            for (long i = flatBoostDim; i >= 0; i--)
            {
                c[i]++;

                if (c[i] != destShape[i])
                {
                    pSourceData = sourceShape[i] == 1 ? pSourceData : pSourceData + sourceShape[i];
                    pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pSourceData = sourceShape[i] == 1 ? pSourceData : pSourceData - sourceStrides[i] * (sourceShape[i] - 1);
                    pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &), bool sourceSkip>
    void Array<T>::simdUnaryParam(const T *pSourceData, U *pDestData, const Coordinates &sourceShape, const Coordinates &destShape, const Coordinates &sourceStrides, const Coordinates &destStrides, const long flatBoostDim, const long flatBoostDimLength, const Q param)
    {
        Coordinates c(destShape.size() - 1, 0);
        bool end = false;

        while (!end)
        {
            simdUnaryParamBoost<U, Q, fSimd, sourceSkip>(pSourceData, pDestData, flatBoostDimLength, param);

            end = true;

            for (long i = flatBoostDim; i >= 0; i--)
            {
                c[i]++;

                if (c[i] != destShape[i])
                {
                    pSourceData = sourceShape[i] == 1 ? pSourceData : pSourceData + sourceShape[i];
                    pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                    end = false;
                    break;
                }
                else
                {
                    pSourceData = sourceShape[i] == 1 ? pSourceData : pSourceData - sourceStrides[i] * (sourceShape[i] - 1);
                    pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                    c[i] = 0;
                }
            }
        }
    }

    template <DataType T>
    template <DataType U, SimdVector<U> (*fSimd)(const SimdVector<T> &), bool sourceSkip>
    inline void Array<T>::simdUnaryBoost(const T *pSourceData, U *pDestData, const long axisLength)
    {
        constexpr size_t LENGTH = SimdVector<T>::LENGTH;
        const long leftover = axisLength % LENGTH;

        SimdVector<T> source;
        if (sourceSkip)
            source = SimdVector<T>::broadcast_set(*pSourceData);

        for (long i = 0; i + LENGTH <= axisLength; i += LENGTH)
        {
            if (!sourceSkip)
            {
                source = SimdVector<T>::load(pSourceData);
                pSourceData += LENGTH;
            }

            SimdVector<U>::store(pDestData, fSimd(source));
            pDestData += LENGTH;
        }

        if (leftover)
        {
            auto mask = SimdVector<T>::makePrefixMask(leftover);
            if (!sourceSkip)
                source = SimdVector<T>::maskedLoad(pSourceData, mask);

            SimdVector<U>::maskedStore(pDestData, mask, fSimd(source));
        }
    }

    template <DataType T>
    template <DataType U, typename Q, SimdVector<U> (*fSimd)(const SimdVector<T> &, const Q &), bool sourceSkip>
    inline void Array<T>::simdUnaryParamBoost(const T *pSourceData, U *pDestData, const long axisLength, const Q &param)
    {
        constexpr size_t LENGTH = SimdVector<T>::LENGTH;
        const long leftover = axisLength % LENGTH;

        SimdVector<T> source;
        if (sourceSkip)
            source = SimdVector<T>::broadcast_set(*pSourceData);

#pragma GCC unroll 4
        for (U *const last = pDestData + axisLength - LENGTH; pDestData <= last;)
        {
            if (!sourceSkip)
            {
                source = SimdVector<T>::load(pSourceData);
                pSourceData += LENGTH;
            }

            SimdVector<U>::store(pDestData, fSimd(source, param));
            pDestData += LENGTH;
        }

        if (leftover)
        {
            auto mask = SimdVector<T>::makePrefixMask(leftover);
            if (!sourceSkip)
                source = SimdVector<T>::maskedLoad(pSourceData, mask);

            SimdVector<U>::maskedStore(pDestData, mask, fSimd(source, param));
        }
    }
}

#endif