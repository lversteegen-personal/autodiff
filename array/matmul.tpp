#ifndef ARRAY_MATMUL_H
#define ARRAY_MATMUL_H

#include "array.hpp"
#include "simd_vector.hpp"
#include "array_creation.tpp"

namespace ArrayLibrary
{
    namespace Matmul
    {
        Coordinates matmulShape(const Coordinates &leftShapeBase, const Coordinates &rightShapeBase, long leftProductAxis, long rightProductAxis)
        {
            leftProductAxis = leftProductAxis % leftShapeBase.size();
            leftProductAxis = leftProductAxis < 0 ? leftShapeBase.size() + leftProductAxis : leftProductAxis;
            rightProductAxis = rightProductAxis % rightShapeBase.size();
            rightProductAxis = rightProductAxis < 0 ? rightShapeBase.size() + rightProductAxis : rightProductAxis;

            long dim = std::max(leftShapeBase.size(), rightShapeBase.size());

            auto leftShape = leftShapeBase.shiftRight(1, dim - leftShapeBase.size());
            leftProductAxis += dim - leftShapeBase.size();
            auto rightShape = rightShapeBase.shiftRight(1, dim - rightShapeBase.size());
            rightProductAxis += dim - rightShapeBase.size();

            if (leftShape[leftProductAxis] != rightShape[rightProductAxis])
                throw std::invalid_argument("Arrays do not have the same length in product dimension.");

            Coordinates resultShape(dim);

            for (long i = 0; i < dim; i++)
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

        template <DataType T>
        void matmulBoost(const T *pLeftData, const T *pRightData, T *pResultData, const long leftLength, const long rightLength, const long productLength, const long leftFreeStride, const long leftProductStride, const long rightFreeStride, const long rightProductStride, const long resultLeftStride, const long resultRightStride)
        {
            if (productLength > 1)
            {
                const T *pRightSave;
                const T *pLeftSave;

                for (long i = 0; i < leftLength; i++)
                {
                    for (long j = 0; j < rightLength; j++)
                    {
                        T sum = 0;
                        pLeftSave = pLeftData;
                        pRightSave = pRightData;

                        for (long k = 0; k < productLength; k++)
                        {
                            sum += *pLeftData * *pRightData;
                            pLeftData += leftProductStride;
                            pRightData += rightProductStride;
                        }
                        pLeftData = pLeftSave;
                        pRightData = pRightSave + rightFreeStride;
                        *pResultData = sum;
                        pResultData += resultRightStride;
                    }
                    pLeftData += leftFreeStride;
                    pRightData -= rightFreeStride * rightLength;
                    pResultData += resultLeftStride - resultRightStride * rightLength;
                }
            }
            else
            {
                for (long i = 0; i < leftLength; i++)
                {
                    for (long j = 0; j < rightLength; j++)
                    {
                        *pResultData = *pLeftData * *pRightData;
                        pRightData += rightFreeStride;
                        pResultData += resultRightStride;
                    }
                    pLeftData += leftFreeStride;
                    pRightData -= rightFreeStride * rightLength;
                    pResultData += resultLeftStride - resultRightStride * rightLength;
                }
            }
        }

        /// @brief This matrix product function accelerates computation by assuming that rightFreeStride and resultRightStride are 1
        template <DataType T>
        void inline simdMatmulAlongRightFreeAxis(const T *pLeftData, const T *pRightData, T *pResultData, const long leftLength, const long rightLength, const long productLength, const long leftFreeStride, const long leftProductStride, const long rightFreeStride, const long rightProductStride, const long resultLeftStride, const long resultRightStride)
        {
            const T *const pRightStart = pRightData;
            constexpr size_t LENGTH = SimdVector<T>::LENGTH;
            const long leftover = rightLength % LENGTH;

            for (long i = 0; i < leftLength; i++)
            {
                for (long k = 0; k < productLength; k++)
                {
                    auto a = SimdVector<T>::broadcast_set(*pLeftData);

                    for (long j = 0; j + LENGTH <= rightLength; j += LENGTH)
                    {
                        auto b = SimdVector<T>::load(pRightData);
                        auto c = SimdVector<T>::load(pResultData);
                        SimdVector<T>::store(pResultData, SimdVector<T>::fusedAddMultiply(a, b, c));

                        pRightData += LENGTH;
                        pResultData += LENGTH;
                    }

                    if (leftover)
                    {
                        auto mask = SimdVector<T>::makePrefixMask(leftover);
                        auto b = SimdVector<T>::maskedLoad(pRightData, mask);
                        auto c = SimdVector<T>::maskedLoad(pResultData, mask);
                        SimdVector<T>::maskedStore(pResultData, mask, SimdVector<T>::fusedAddMultiply(a, b, c));
                    }
                    pResultData -= rightLength - leftover;
                    pLeftData += leftProductStride;
                    pRightData += -rightLength + leftover + rightProductStride;
                }

                pLeftData += leftFreeStride - productLength * leftProductStride;
                pResultData += resultLeftStride;
                pRightData = pRightStart;
            }
        }

        /// @brief This matrix product function accelerates computation by assuming that leftFreeStride and resultLeftStride are 1
        template <DataType T>
        void inline simdMatmulAlongLeftFreeAxis(const T *pLeftData, const T *pRightData, T *pResultData, const long leftLength, const long rightLength, const long productLength, const long leftFreeStride, const long leftProductStride, const long rightFreeStride, const long rightProductStride, const long resultLeftStride, const long resultRightStride)
        {
            const T *const pLeftStart = pLeftData;
            constexpr size_t LENGTH = SimdVector<T>::LENGTH;
            const long leftover = leftLength % LENGTH;

            for (long j = 0; j < rightLength; j++)
            {
                for (long k = 0; k < productLength; k++)
                {
                    auto b = SimdVector<T>::broadcast_set(*pRightData);

                    for (long i = 0; i + LENGTH <= leftLength; i += LENGTH)
                    {
                        auto a = SimdVector<T>::load(pLeftData);
                        auto c = SimdVector<T>::load(pResultData);
                        SimdVector<T>::store(pResultData, SimdVector<T>::fusedAddMultiply(a, b, c));

                        pLeftData += LENGTH;
                        pResultData += LENGTH;
                    }

                    if (leftover)
                    {
                        auto mask = SimdVector<T>::makePrefixMask(leftover);
                        auto a = SimdVector<T>::maskedLoad(pLeftData, mask);
                        auto c = SimdVector<T>::maskedLoad(pResultData, mask);
                        SimdVector<T>::maskedStore(pResultData, mask, SimdVector<T>::fusedAddMultiply(a, b, c));
                    }
                    pResultData -= leftLength - leftover;
                    pRightData += rightProductStride;
                    pLeftData += -leftLength + leftover + leftProductStride;
                }

                pRightData += rightFreeStride - productLength * rightProductStride;
                pResultData += resultRightStride;
                pLeftData = pLeftStart;
            }
        }

        template <DataType T, uint8_t LANES>
        inline T simdInnerProduct(const T *pLeftData, const T *pRightData, const long axisLength)
        {
            constexpr uint8_t LENGTH = SimdVector<T>::LENGTH;
            SimdVector<T> acc[LANES];

            for (long k = 0; k + LENGTH * LANES <= axisLength; k += LENGTH * LANES)
            {
#pragma GCC unroll LANES
                for (uint8_t i = 0; i < LANES; i++)
                {
                    auto left = SimdVector<T>::load(pLeftData + LENGTH * i);
                    auto right = SimdVector<T>::load(pRightData + LENGTH * i);
                    acc[i] = SimdVector<T>::fusedAddMultiply(left, right, acc[i]);
                }

                pLeftData += LENGTH * LANES;
                pRightData += LENGTH * LANES;
            }

            long leftover = axisLength % (LENGTH * LANES);
            auto mask = SimdVector<T>::makePrefixMask(leftover);
            auto left = SimdVector<T>::maskedLoad(pLeftData, mask);
            auto right = SimdVector<T>::maskedLoad(pRightData, mask);
            acc[0] = SimdVector<T>::fusedAddMultiply(left, right, acc[0]);

#pragma GCC unroll LANES
            for (uint8_t i = 1; i < LANES; i++)
            {
                auto mask = SimdVector<T>::makePrefixMask(leftover - LENGTH * i);
                auto left = SimdVector<T>::maskedLoad(pLeftData + LENGTH * i, mask);
                auto right = SimdVector<T>::maskedLoad(pRightData + LENGTH * i, mask);
                acc[i] = SimdVector<T>::fusedAddMultiply(left, right, acc[i]);
                acc[0] = SimdVector<T>::add(acc[0], acc[i]);
            }

            T alignas(SIMD_BYTES) extract[LENGTH];
            SimdVector<T>::store(extract, acc[0]);

            // #pragma GCC unroll LENGTH
            for (uint8_t i = LENGTH - 1; i > 0; i--)
            {
                extract[i - 1] += extract[i];
            }

            return extract[0];
        }

        /// @brief This matrix product function accelerates computation by assuming that leftProductStride and rightProductStride are 1 (use for matvec multiplication and inner products)
        template <DataType T, uint8_t LANES>
        void inline simdMatmulAlongProductAxis(const T *pLeftData, const T *pRightData, T *pResultData, const long leftLength, const long rightLength, const long productLength, const long leftFreeStride, const long leftProductStride, const long rightFreeStride, const long rightProductStride, const long resultLeftStride, const long resultRightStride)
        {
            const T *const pRightStart = pRightData;

            for (long i = 0; i < leftLength; i++)
            {
                for (long j = 0; j < rightLength; j++)
                {
                    *pResultData = simdInnerProduct<T, LANES>(pLeftData, pRightData, productLength);

                    pResultData += resultRightStride;
                    pRightData += rightFreeStride;
                }

                pResultData += resultLeftStride - rightLength * resultRightStride;
                pLeftData += leftFreeStride;
                pRightData = pRightStart;
            }
        }

        template <DataType T>
        using f2DimMultiplier_t = decltype(matmulBoost<T>);

        template <DataType T, f2DimMultiplier_t<T> f>
        void baseMatmul(const Coordinates &leftShape, const Coordinates &leftStrides, const T *pLeftData, const Coordinates &rightShape, const Coordinates &rightStrides, const T *pRightData, const Coordinates &resultShape, const Coordinates &resultStrides, T *pResultData, long leftProductAxis, long rightProductAxis)
        {
            const long productAxisLength = leftShape[leftProductAxis];
            const long leftProductStride = leftStrides[leftProductAxis], rightProductStride = rightStrides[rightProductAxis];
            const long leftLength = leftShape[rightProductAxis], rightLength = rightShape[leftProductAxis];
            const long leftFreeStride = leftStrides[rightProductAxis], rightFreeStride = rightStrides[leftProductAxis];
            const long resultLeftStride = resultStrides[rightProductAxis], resultRightStride = resultStrides[leftProductAxis];

            const long dim = leftShape.size();
            Coordinates outerShape(dim);

            for (size_t i = 0; i < dim; i++)
            {
                if (i == leftProductAxis)
                    outerShape[i] = std::max(rightShape[i], resultShape[i]);
                else if (i == rightProductAxis)
                    outerShape[i] = std::max(leftShape[i], resultShape[i]);
                else
                    outerShape[i] = std::max({leftShape[i], rightShape[i], resultShape[i]});
            }

            Coordinates c(dim, 0);

            bool end = false;
            while (!end)
            {
                f(pLeftData, pRightData, pResultData, leftLength, rightLength, productAxisLength, leftFreeStride, leftProductStride, rightFreeStride, rightProductStride, resultLeftStride, resultRightStride);

                end = true;

                for (long i = dim - 1; i >= 0; i--)
                {
                    if (i == leftProductAxis || i == rightProductAxis)
                        continue;

                    c[i]++;

                    if (c[i] != outerShape[i])
                    {
                        pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData + leftStrides[i];
                        pRightData = rightShape[i] == 1 ? pRightData : pRightData + rightStrides[i];
                        pResultData = resultShape[i] == 1 ? pResultData : pResultData + resultStrides[i];
                        end = false;
                        break;
                    }
                    else
                    {
                        pLeftData = leftShape[i] == 1 ? pLeftData : pLeftData - leftStrides[i] * (leftShape[i] - 1);
                        pRightData = rightShape[i] == 1 ? pRightData : pRightData - rightStrides[i] * (rightShape[i] - 1);
                        pResultData = resultShape[i] == 1 ? pResultData : pResultData - resultStrides[i] * (resultShape[i] - 1);
                        c[i] = 0;
                    }
                }
            }
        }

        template <DataType T>
        void matmulDispatcher(const Coordinates &leftShape, const Coordinates &leftStrides, const T *pLeftData, const Coordinates &rightShape, const Coordinates &rightStrides, const T *pRightData, const Coordinates &resultShape, const Coordinates &resultStrides, T *pResultData, long leftProductAxis, long rightProductAxis, bool useSimd, bool multiThread)
        {
            static_assert(!SimdVector<T>::supported || std::is_same_v<decltype(simdMatmulAlongRightFreeAxis<T>), f2DimMultiplier_t<T>>);
            static_assert(!SimdVector<T>::supported || std::is_same_v<decltype(simdMatmulAlongProductAxis<T, 1>), f2DimMultiplier_t<T>>);
            static_assert(!SimdVector<T>::supported || std::is_same_v<decltype(simdMatmulAlongLeftFreeAxis<T>), f2DimMultiplier_t<T>>);

            const long productAxisLength = leftShape[leftProductAxis];
            const long leftProductStride = leftStrides[leftProductAxis], rightProductStride = rightStrides[rightProductAxis];
            const long leftLength = leftShape[rightProductAxis], rightLength = rightShape[leftProductAxis];
            const long leftFreeStride = leftStrides[rightProductAxis], rightFreeStride = rightStrides[leftProductAxis];
            const long resultLeftStride = resultStrides[rightProductAxis], resultRightStride = resultStrides[leftProductAxis];

            const long dim = leftShape.size();
            if (!SimdVector<T>::supported || !useSimd)
                baseMatmul<T, matmulBoost<T>>(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
            else if (leftProductStride == 1 && rightProductStride == 1)
                baseMatmul<T, simdMatmulAlongProductAxis<T, 4>>(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
            else if (rightFreeStride == 1 && resultRightStride == 1)
                baseMatmul<T, simdMatmulAlongRightFreeAxis<T>>(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
            else if (leftFreeStride == 1 && resultLeftStride == 1)
                baseMatmul<T, simdMatmulAlongLeftFreeAxis<T>>(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
            else
                baseMatmul<T, matmulBoost<T>>(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
        }

        struct MatmulSettings
        {
            bool setzero = true;
            long leftProductAxis = -1;
            long rightProductAxis = -2;
            bool useSimd = true;
            bool multiThread = false;
            Coordinates reduceAxes;
            bool keepDims = false;
        };

        /// @brief Computes the matrix product of two arrays along the specified product axes lpa and rpa. If the argument matrices have different dimension, their shapes will be padded with 1s from the left to match the dimensions. For the padded shape, the corresponding product axis will be adjusted accordingly if the product axis was positive; otherwise the product axis will not be changed. The padded shapes sl and sr must be broadcastable to match outside of the adjusted lpa and rpa, and they must satisfy left.getShape()[leftProductAxis]==right.getShape()[rightProductAxis].
        /// @return The matrix product of left and right axes specified in settings.
        template <DataType T>
        ArrayLibrary::Array<T> matmul(const Array<T> &left, const Array<T> &right, Array<T> *const pDestArray, const MatmulSettings &settings)
        {
            long leftProductAxis = settings.leftProductAxis % left.mDim;
            leftProductAxis = leftProductAxis < 0 ? left.mDim + leftProductAxis : leftProductAxis;
            long rightProductAxis = settings.rightProductAxis % right.mDim;
            rightProductAxis = rightProductAxis < 0 ? right.mDim + rightProductAxis : rightProductAxis;

            long dim = std::max(left.mDim, right.mDim);

            auto leftShape = left.mShape.shiftRight(1, dim - left.mDim);
            auto leftStrides = left.mStrides.shiftRight(0, dim - left.mDim);
            leftProductAxis += dim - left.mDim;

            auto rightShape = right.mShape.shiftRight(1, dim - right.mDim);
            auto rightStrides = right.mStrides.shiftRight(0, dim - right.mDim);
            rightProductAxis += dim - right.mDim;

            if (leftProductAxis == rightProductAxis)
                throw std::invalid_argument("leftProductAxis must be different from rightProductAxis");

            if (leftShape[leftProductAxis] != rightShape[rightProductAxis])
                throw std::invalid_argument("Arrays do not have the same length in product dimension.");

            ReduceInformation reduceInfo = reduceShape(matmulShape(leftShape, rightShape, leftProductAxis, rightProductAxis), settings.reduceAxes, settings.keepDims);

            if (pDestArray == nullptr)
            {
                Array<T> result = Array<T>::constant(reduceInfo.keepDimsShape, 0);

                T *pLeftData = left.getDataPointer(), *pRightData = right.getDataPointer(), *pDestData = result.getDataPointer();

                matmulDispatcher(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, result.refShape(), result.refStrides(), pDestData, leftProductAxis, rightProductAxis, settings.useSimd, settings.multiThread);

                return settings.keepDims ? result : result.reshape(reduceInfo.reducedShape);
            }
            else
            {
                if (reduceInfo.reducedShape != pDestArray->refShape().shiftRight(1, reduceInfo.reducedShape.size() - pDestArray->getDim()))
                    throw std::invalid_argument("The shape of the destination array does not fit the product shape of left and right.");

                Array<T> dest = settings.keepDims ? (*pDestArray) : pDestArray->reshape(reduceInfo.keepDimsShape);
                if (settings.setzero)
                    dest = 0;

                T *pLeftData = left.getDataPointer(), *pRightData = right.getDataPointer(), *pDestData = dest.getDataPointer();
                matmulDispatcher(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, dest.refShape(), dest.refStrides(), pDestData, leftProductAxis, rightProductAxis, settings.useSimd, settings.multiThread);

                return dest;
            }
        }

        template <DataType T>
        inline ArrayLibrary::Array<T> matmul(const Array<T> &left, const Array<T> &right, const MatmulSettings &settings)
        {
            return matmul<T>(left, right, nullptr, settings);
        }

        template <DataType T>
        Array<T> matmul(const Array<T> &left, const Array<T> &right)
        {
            return matmul<T>(left, right, nullptr, MatmulSettings());
        }

        // Currently not in use
        template <DataType T>
        void matmulThreadControl(const Coordinates &leftShape, const Coordinates &leftStrides, T *pLeftData, const Coordinates &rightShape, const Coordinates &rightStrides, T *pRightData, const Coordinates &resultShape, const Coordinates &resultStrides, T *pResultData, long leftProductAxis, long rightProductAxis)
        {
            static long maxThreads = std::max(1u, std::thread::hardware_concurrency());
            static const long CONCURRENCY_THRESHOLD = 0x10000;

            long flatLength = 1, longestAxis = -1, longestAxisLength = -1;
            for (long i = 0; i < resultShape.size(); i++)
            {
                if (resultShape[i] > longestAxisLength)
                {
                    longestAxisLength = resultShape[i];
                    longestAxis = i;
                }
                flatLength *= resultShape[i];
            }

            if (flatLength * leftShape[leftProductAxis] < CONCURRENCY_THRESHOLD)
            {
                baseMatmul(leftShape, leftStrides, pLeftData, rightShape, rightStrides, pRightData, resultShape, resultStrides, pResultData, leftProductAxis, rightProductAxis);
                return;
            }

            long threads = std::min({maxThreads, longestAxisLength, flatLength / CONCURRENCY_THRESHOLD + 1});
            long progress = 0;
            std::vector<std::thread> threadPool(threads);

            for (; threads > 0; threads--)
            {
                long threadLength = (longestAxisLength - progress) / threads;

                Coordinates threadLeftShape(leftShape);
                Coordinates threadRightShape(rightShape);
                Coordinates threadResultShape(resultShape);

                T *pThreadLeftData = pLeftData;
                T *pThreadRightData = pRightData;
                T *pThreadResultData = pResultData + progress * resultStrides[longestAxis];

                if (longestAxis != leftProductAxis)
                {
                    threadLeftShape[longestAxis] = threadLength;
                    pThreadLeftData = pLeftData + progress * leftStrides[longestAxis];
                }
                if (longestAxis != rightProductAxis)
                {
                    threadRightShape[longestAxis] = threadLength;
                    pThreadRightData = pRightData + progress * rightStrides[longestAxis];
                }

                threadResultShape[longestAxis] = threadLength;

                progress += threadLength;

                // baseMatmul(threadLeftShape, leftStrides, pThreadLeftData, threadRightShape, rightStrides, pThreadRightData, threadResultShape, resultStrides, pThreadResultData, leftProductAxis, rightProductAxis);
                std::thread t(baseMatmul, threadLeftShape, leftStrides, pThreadLeftData, threadRightShape, rightStrides, pThreadRightData, threadResultShape, resultStrides, pThreadResultData, leftProductAxis, rightProductAxis);
                threadPool[threads - 1] = std::move(t);
            }

            for (auto &t : threadPool)
                t.join();
        }
    }
}
#endif