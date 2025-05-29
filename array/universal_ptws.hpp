#ifndef ARRAY_UNIVERSAL_PTWS_H
#define ARRAY_UNIVERSAL_PTWS_H

#include "array.hpp"
#include "simd_vector.hpp"

// This is very much WIP

namespace ArrayLibrary
{
    template <DataType ResultType, DataType... InputTypes>
    class UniversalOperation
    {
        static constexpr size_t N = sizeof...(InputTypes);

        template <DataType T, bool isMoving>
        struct InputInfo
        {
            const T *pData;
            const Coordinates &shape;
            const Coordinates &strides;

            InputInfo(const T *pStart, const Coordinates &shape, const Coordinates &strides) : pData(pStart), shape(shape), strides(strides) {}
        };

        template <auto f, bool... Moving>
            requires(std::is_same_v<decltype(f), ResultType (*)(const InputTypes...)>)
        static void innerLoop(long size, ResultType *pDest, const InputInfo<InputTypes, Moving>... &inputInfos)
        {

            // #pragma GCC novector pragma
            for (long j = 0; j < size; j++)
            {
                *pDest++ = f((*inputInfos.pData)...);
                ((inputInfos.pData = Moving ? inputInfos.pData + 1 : inputInfos.pData), ...);
            }
        }

        template <auto f, bool... Moving>
            requires(std::is_same_v<decltype(f), ResultType (*)(const InputTypes...)>)
        static void outerLoop(const long flatBoostDim, const long flatBoostDimLength, Array<ResultType> dest, InputInfo<InputTypes, Moving>... inputInfos)
        {
            const ResultType *pDestData = dest.getDataPointer();
            const Coordinates &destShape = dest.refShape();
            const Coordinates &destStrides = dest.refStrides();

            Coordinates c(destShape.size() - 1, 0);
            bool end = false;

            while (!end)
            {
                innerLoop<f, Moving...>(flatBoostDimLength, pDestData, inputInfos);

                end = true;

                for (long i = flatBoostDim; i >= 0; i--)
                {
                    c[i]++;

                    if (c[i] != destShape[i])
                    {
                        (inputInfos.pData = inputInfos.strides[i] == 1 ? inputInfos.pData[i] : inputInfos.pData[i] + inputInfos.strides[i])...;
                        pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                        end = false;
                        break;
                    }
                    else
                    {
                        (inputInfos.pData = inputInfos.strides[i] == 1 ? inputInfos.pData[i] : inputInfos.pData[i] - inputInfos.strides[i] * (inputInfos.shape[i] - 1))...;
                        pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                        c[i] = 0;
                    }
                }
            }
        }

        /*template <auto f, size_t k, DataType... InputTypesPrefix, bool... Moving>
            requires(k == N, sizeof...(InputTypesPrefix) == k && sizeof...(Moving) == k && std::is_same_v<decltype(f), ResultType (*)(const InputTypes...)>)
        static void execute(const long flatBoostDim, const long flatBoostDimLength, Array<ResultType> dest, Array<InputTypes>... sources, InputInfo<InputTypesPrefix, Moving>... inputInfos)
        {
            outerLoop(flatBoostDim, flatBoostDimLength, dest, inputInfos...);
        }*/

        template <auto f, size_t k, DataType... InputTypesPrefix, bool... Moving>
            requires(sizeof...(InputTypesPrefix) == k && sizeof...(Moving) == k && std::is_same_v<decltype(f), ResultType (*)(const InputTypes...)>)
        static void execute(const long flatBoostDim, const long flatBoostDimLength, Array<ResultType> dest, Array<InputTypes>... sources, InputInfo<InputTypesPrefix, Moving>... inputInfos)
        {
            if (k == N)
                outerLoop(flatBoostDim, flatBoostDimLength, dest, inputInfos...);
            else
            {
                using T = InputTypes[k];
                auto source = sources[k];
                Coordinates shape = source.refShape().shiftRight(1, dest.gerDim() - source.getDim());
                Coordinates strides = source.refShape().shiftRight(0, dest.getDim() - source.getDim());

                if (strides[flatBoostDim] == dest.refStrides()[flatBoostDim])
                    execute<f, k + 1, InputTypesPrefix..., T, Moving..., true>(flatBoostDim, flatBoostDimLength, dest, inputInfos..., InputInfo<T, true>(source.readDataPointer(), shape, strides));
                else
                    execute<f, k + 1, InputTypesPrefix..., T, Moving..., false>(flatBoostDim, flatBoostDimLength, dest, inputInfos..., InputInfo<T, false>(source.readDataPointer(), shape, strides));
            }
        }

        template <DataType T>
        inline bool checkMatch(const Array<T> &source, const long i, const long lastNonTrivial, const long lengthTarget, const long matchFlatLength)
        {
            bool skip = source.refShape()[lastNonTrivial] == 1;
            return (skip && sourceShape[i] == 1) || (!skip && sourceStrides[i] == matchFlatLength && sourceShape[i] == destShape[i])
        }

    public:
        template <auto f>
            requires(std::is_same_v<decltype(f), ResultType (*)(const InputTypes...)>)
        static Array<ResultType> &dispatch(Array<RestultType> &dest, const Array<InputTypes>... &sources)
        {
            for (size_t i = 0; i < N; i++)
                assertm(isSubshape(sources[i].refShape(), dest.refShape()), "Source array is not a subshape of the destination array.");

            if (dest.mDim == 0)
            {
                dest.mData[dest.mOffset] = f(sources[0]...);
                return dest;
            }

            const Coordinates &destShape = dest.refShape();
            const Coordinates &destStrides = dest.refStrides();

            constexpr long flatLengthThreshold = 8;

            long lastNonTrivialAxis = dest.mDim;
            while (destShape[--lastNonTrivialAxis] == 1)
                ;

            long i = dest.mDim - 1;
            long matchFlatLength = 1;

            for (; i >= 0; i--)
            {
                if (destStrides[i] != matchFlatLength)
                    break;

                if !(... && checkMatch(sources, i, lastNonTrivialAxis, destShape[i], matchFlatLength))
                    break;

                matchFlatLength *= destShape[i];
            }

            execute<f, 0>(i, matchFlatLength, dest, sources...);

            return dest;
        }
    };
}
#endif