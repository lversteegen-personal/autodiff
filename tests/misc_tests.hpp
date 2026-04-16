#ifndef MISC_TESTS_H
#define MISC_TESTS_H

#include "test_util.hpp"

using namespace ArrayLibrary;
using namespace ArrayLibrary::Matmul;
using namespace AutoDiff;
using namespace AutoDiff::NeuralNetworks;

namespace Test
{
    template <DataType T>
    void matmulPerfShape()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        PerformanceMeasure shortProductAxisMeasure;
        PerformanceMeasure longProductAxisMeasure;

        T thinSum = 0;
        T sqaureSum = 0;

        for (int i = 0; i < 10; i++)
        {
            auto A = randomArrayGenerator.normal<T>({16, 200, 1}, 0, 1);
            auto B = randomArrayGenerator.normal<T>({16, 1, 784}, 0, 1);
            auto C = randomArrayGenerator.normal<T>({1, 200, 784}, 0, 1);
            auto D = randomArrayGenerator.normal<T>({16, 784, 1}, 0, 1);

            // assertm(A.refShape()[0]*A.refShape()[1]*B.refShape()[1] == C.refShape()[0]*C.refShape()[1]*D.refShape()[1], "Different number of idealised operations.");

            shortProductAxisMeasure.start();
            auto resultThin = matmul(A, B);
            shortProductAxisMeasure.stop();

            longProductAxisMeasure.start();
            auto resultSquare = matmul(C, D);
            longProductAxisMeasure.stop();

            thinSum += resultThin.sin().reduceSum().eval();
            sqaureSum += resultSquare.sin().reduceSum().eval();
        }

        LOG(thinSum);
        LOG(sqaureSum);
        LOG_TIME(shortProductAxisMeasure.accumulated);
        LOG_TIME(longProductAxisMeasure.accumulated);
    }

    template <DataType T>
    void outerProductPerf()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<T>({4096, 1}, 0, 1);
        auto B = randomArrayGenerator.normal<T>({1, 4096}, 0, 1);

        // assertm(A.refShape()[0]*A.refShape()[1]*B.refShape()[1] == C.refShape()[0]*C.refShape()[1]*D.refShape()[1], "Different number of idealised operations.");

        PerformanceMeasure measureMatmul;
        PerformanceMeasure measureProd;

        measureMatmul.start();
        auto resultMatmul = matmul(A, B);
        measureMatmul.stop();

        measureProd.start();
        auto resultProd = A * B;
        measureProd.stop();

        LOG_TIME(measureMatmul.accumulated);
        LOG_TIME(measureProd.accumulated);
    }

    template <DataType T>
    void reducePerf()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<T>({4096, 1}, 0, 1);
        auto B = randomArrayGenerator.normal<T>({1, 4096}, 0, 1);

        // assertm(A.refShape()[0]*A.refShape()[1]*B.refShape()[1] == C.refShape()[0]*C.refShape()[1]*D.refShape()[1], "Different number of idealised operations.");

        PerformanceMeasure prodMeasure;
        PerformanceMeasure reduceMeasure;

        prodMeasure.start();
        auto resultMatmul = matmul(A, B);
        prodMeasure.stop();

        resultMatmul = resultMatmul.sin();

        reduceMeasure.start();
        auto reduced = resultMatmul.reduceSum();
        reduceMeasure.stop();

        LOG(reduced.eval());
        LOG_TIME(prodMeasure.accumulated);
        LOG_TIME(reduceMeasure.accumulated);
    }

    template <DataType T>
    void concurrencyTest()
    {
        PerformanceMeasure concMeasure;
        PerformanceMeasure singleMeasure;

        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<T>({1, 100, 800}, 0, 1);
        auto B = randomArrayGenerator.normal<T>({20, 800, 100}, 0, 1);

        concMeasure.start();

        auto resultConcurrency = matmul(A, B);

        concMeasure.stop();

        singleMeasure.start();
        auto resultSingleThread = matmul(A, B);
        singleMeasure.stop();

        assertm((resultConcurrency - resultSingleThread).abs().reduceMax() < 1e-6, "Concurrent matrix product does not agree with single thread matrix product.");
        std::cout << concMeasure.accumulated << std::endl;
        std::cout << singleMeasure.accumulated << std::endl;
    }

    void printingTest()
    {
        auto A = Array<int>::range(18).reshape({3, 2, 3});
        std::cout << A << std::endl;
    }


    template <DataType T>
    void performanceMeasureTest()
    {
        PerformanceMeasure outer;
        PerformanceMeasure inner;

        T dummy = 0;

        outer.start();
        for (int i = 0; i < 10000; i++)
        {
            inner.start();
            for (int j = 0; j < 1000; j++)
            {
                dummy += i * j;
            }
            inner.stop();
        }
        outer.stop();

        LOG(dummy);
        LOG(outer.accumulated);
        LOG(inner.accumulated);
        LOG(outer.accumulated / inner.accumulated);
    }

    template <DataType T>
    void mnistRun()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto mnist = Loader<T>::loadMNIST(0x4000);
        Array<T> images(mnist.data.reshape({-1, 784}));
        images /= 255;
        auto onehot = mnist.label.template oneHot<T>();

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(200, Activation::LEAKYRELU, T(0.01)));
        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(10, Activation::NONE, T(0.01)));

        auto &sftm = Softermax<T>::create(layer2, {-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        int epochs = 3;
        long batchSize = 16;
        long batchCount = mnist.getLength() / batchSize;
        T learningRate = 1e-3;
        T clipValue = 1000;
        T totalCost = 0;

        PerformanceMeasure overallMeasure;
        PerformanceMeasure calculateMeasure;
        PerformanceMeasure applyMeasure;

        overallMeasure.start();
        diffTape.setMeasurePerformance(true);

        for (int e = 0; e < epochs; e++)
        {
            totalCost = 0;
            for (int i = 0; i < batchCount; i++)
            {
                auto batchStart = i * batchSize;
                auto batchEnd = (i + 1) * batchSize;

                input.setValue(images.slice({batchStart}, {batchEnd}));
                labels.setValue(onehot.slice({batchStart}, {batchEnd}));

                calculateMeasure.start();
                diffTape.calculateAll(cost);
                calculateMeasure.stop();

                applyMeasure.start();
                layer1.applyGradient(diffTape, cost, learningRate, clipValue);
                layer2.applyGradient(diffTape, cost, learningRate, clipValue);
                applyMeasure.stop();

                totalCost += cost.refArray().eval();

                if (i % 32 == 0)
                {
                    std::cout << "\r\t\r";
                    std::cout << "Batch: " << i << ", mse: " << totalCost / ((i + 1) * batchSize) << "\t";
                }
            }
            std::cout << "\r\t\r";
            std::cout << "Epoch: " << e << ", mse: " << totalCost / mnist.getLength() << std::endl;
        }

        overallMeasure.stop();

        std::cout << std::endl;

        for (int i = 0; i < diffTape.mUnits.size(); i++)
        {
            std::cout << typeid(*(diffTape.mUnits[i])).name() << std::endl;
            LOG(diffTape.mUnits[i]->refArray().checkNumerics());
            std::cout << diffTape.mUnits[i]->refArrayShape() << std::endl;
            LOG(diffTape.getCalcPerformance(i));
            LOG(diffTape.getGradientPerformance(i));
        }
        LOG_TIME(overallMeasure.accumulated);
        LOG_TIME(calculateMeasure.accumulated);
        LOG_TIME(applyMeasure.accumulated);
    }

    template <DataType T>
    Array<T> simd_test()
    {
        RandomArrayGenerator randomArrayGenerator(0);

        int n = 0x1000000;
        constexpr int LENGTH = 32 / sizeof(T);

        auto A = randomArrayGenerator.uniform<T>({n});
        auto B = randomArrayGenerator.uniform<T>({n});
        auto C = Array<T>::constant({n}, 0);
        T *pDataA = A.getDataPointer();
        T *pDataB = B.getDataPointer();
        T *pDataC = C.getDataPointer();

        PerformanceMeasure simdMeasure;

        simdMeasure.start();
        for (int i = 0; i < n; i += LENGTH)
        {
            auto a = Simd::load<T>(pDataA);
            auto b = Simd::load<T>(pDataB);
            Simd::store<T>(pDataC, Simd::multiply(a, b));

            pDataA += LENGTH;
            pDataB += LENGTH;
            pDataC += LENGTH;
        }
        simdMeasure.stop();

        return C;
    }

    template <DataType T>
    Array<T> simd_benchmark()
    {
        RandomArrayGenerator randomArrayGenerator(0);

        int n = 0x1000000;

        auto A = randomArrayGenerator.uniform<T>({n});
        auto B = randomArrayGenerator.uniform<T>({n});
        auto C = Array<T>::constant({n}, 0);
        T *pDataA = A.getDataPointer();
        T *pDataB = B.getDataPointer();
        T *pDataC = C.getDataPointer();

        PerformanceMeasure benchmarkMeasure;

        benchmarkMeasure.start();

#pragma GCC novector pragma
        for (int i = 0; i < n; i++)
        {
            *pDataC = *pDataA * *pDataB;
            *pDataA++;
            *pDataB++;
            *pDataC++;
        }
        benchmarkMeasure.stop();

        return C;
    }

    template <DataType T>
    Array<T> simd_matmul()
    {
        RandomArrayGenerator randomArrayGenerator(0);

        int n = 0x200;
        constexpr int LENGTH = Simd::LENGTH<T>;

        auto A = randomArrayGenerator.uniform<T>({n, n});
        auto B = randomArrayGenerator.uniform<T>({n, n});
        auto C = Array<T>::constant({n, n}, 0);
        T *pDataA = A.getDataPointer();
        T *pDataB = B.getDataPointer();
        T *pDataC = C.getDataPointer();

        PerformanceMeasure simdMeasure;

        simdMeasure.start();
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                auto a = Simd::broadcast_set<T>(*pDataA);
                for (int j = 0; j < n; j += LENGTH)
                {
                    auto b = Simd::load<T>(pDataB);
                    auto c = Simd::load<T>(pDataC);
                    Simd::store<T>(pDataC, Simd::fusedMultiplyAdd<T>(a, b, c));

                    pDataB += LENGTH;
                    pDataC += LENGTH;
                }

                pDataC -= n;
                pDataA++;
            }
            pDataC += n;
            pDataB -= n * n;
        }
        simdMeasure.stop();

        return C;
    }

    template <DataType T>
    void simd_array_matmul()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        PerformanceMeasure simdMeasure1;
        PerformanceMeasure simdMeasure2;
        PerformanceMeasure classicMeasure1;
        PerformanceMeasure classicMeasure2;

        long n = 0x215;

        Array<T> A = randomArrayGenerator.uniform<T>({n, n});
        Array<T> B = randomArrayGenerator.uniform<T>({n, n});
        Array<T> C = Array<T>::constant({n, n}, 0);
        Array<T> D = Array<T>::constant({n, n}, 0);

        simdMeasure1.start();
        simdMatmulAlongProductAxis<float, 4>(A.getDataPointer(), B.getDataPointer(), C.getDataPointer(), n, n, n, n, n, n, 1ull);
        simdMeasure1.stop();

        simdMeasure2.start();
        simdMatmulAlongRightFreeAxis<float>(A.getDataPointer(), B.getDataPointer(), D.getDataPointer(), n, n, n, n, 1, n, n);
        simdMeasure2.stop();

        classicMeasure1.start();
        auto E = Array<T>::matmul(A, B.transpose(-1, -2));
        classicMeasure1.stop();

        classicMeasure2.start();
        auto F = Array<T>::matmul(A, B);
        classicMeasure2.stop();

        assert((C - E).abs().reduceMax().eval() < 1e-3);
        assert((D - F).abs().reduceMax().eval() < 1e-3);

        LOG_TIME(simdMeasure1.accumulated);
        LOG_TIME(simdMeasure2.accumulated);
        LOG_TIME(classicMeasure1.accumulated);
        LOG_TIME(classicMeasure2.accumulated);
    }

    template <DataType T>
    void simd_array_integrated_test()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        PerformanceMeasure simdMeasure1;
        PerformanceMeasure classicMeasure1;

        long n = 0x215;

        Array<T> A = randomArrayGenerator.uniform<T>({n, n});
        Array<T> B = randomArrayGenerator.uniform<T>({n, n});

        simdMeasure1.start();
        auto C = matmul(A, B);
        simdMeasure1.stop();

        MatmulSettings settings;
        settings.useSimd = false;
        classicMeasure1.start();
        auto D = matmul(A, B, settings);
        classicMeasure1.stop();

        auto diff = (C - D).abs().reduceMax().eval();
        std::cout << diff << std::endl;
        assert(diff < 1e-3);

        LOG_TIME(simdMeasure1.accumulated);
        LOG_TIME(classicMeasure1.accumulated);
    }

    template <DataType T>
    Array<T> benchmark_matmul()
    {
        RandomArrayGenerator randomArrayGenerator(0);

        int n = 0x200;
        constexpr int LENGTH = 32 / sizeof(T);

        auto A = randomArrayGenerator.uniform<T>({n, n});
        auto B = randomArrayGenerator.uniform<T>({n, n});
        auto C = Array<T>::constant({n, n}, 0);
        T *pDataA = A.getDataPointer();
        T *pDataB = B.getDataPointer();
        T *pDataC = C.getDataPointer();

        PerformanceMeasure benchmarkMeasure;

        benchmarkMeasure.start();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    *pDataC += *pDataA * *pDataB;

                    pDataA++;
                    pDataB += n;
                }

                pDataC++;
                pDataA -= n;
                pDataB -= n * n;
                pDataB++;
            }
            pDataB -= n;
            pDataA += n;
        }
        benchmarkMeasure.stop();

        return C;
    }

    void simd_masking()
    {
        uint64_t i = -1;
        std::bitset<64> bitset1{i};
        LOG(bitset1);
        i >>= 5 * 8;
        std::bitset<64> bitset2{i};
        LOG(bitset2);
        __m128i halfmask = _mm_cvtsi64_si128((int64_t)i);
        __m256i mask = _mm256_cvtepi8_epi32(halfmask);
        float array[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        __m256 v = _mm256_maskload_ps(array, mask);
        _mm256_storeu_ps(array, v);
        for (int j = 0; j < 8; j++)
            LOG(array[j]);
    }

    void mnistDataLoad()
    {
        auto mnist = Loader<float>::loadMNIST(0x4000);

        uint8_t image[784 * 3];

        for (int y = 0; y < 28; y++)
            for (int x = 0; x < 28; x++)
            {
                uint8_t brightness = 255 - mnist.data[{0, 28 * y + x}];
                image[3 * (28 * y + x)] = brightness;
                image[3 * (28 * y + x) + 1] = brightness;
                image[3 * (28 * y + x) + 2] = brightness;
            }

        bitmap::save_bmp("data/image0.bmp", 28, 28, image);

        assert(approxEqual(checksum(mnist.data / 255), 225.26734378538023f));
        assert(approxEqual(checksum(mnist.label.oneHot<float>()), 28.917950711077776f));

        std::cout << "MNIST data load test successful." << std::endl;
    }

    template <DataType T>
    void mnistModel()
    {
        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(200, Activation::LEAKYRELU, T(0.01)));
        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(10, Activation::NONE, T(0.01)));

        auto &sftm = Softermax<T>::create(layer2, {-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        Adam<T> opt;
        Model model({&input, &labels}, cost, opt);
        model.setMeasurePerformance(true);

        auto mnist = Loader<T>::loadMNIST(0x4000);
        Array<T> images(mnist.data.reshape({-1, 784}));
        images /= 255;
        auto onehotLabels = mnist.label.template oneHot<T>();

        PerformanceMeasure overallMeasure;
        overallMeasure.start();
        model.fit({images, onehotLabels}, 4, 16);
        overallMeasure.stop();

        for (long i = 0; i < model.refUnits().size(); i++)
        {
            auto *pUnit = model.refUnits()[i];
            std::cout << typeid(*(pUnit)).name() << std::endl;
            LOG(pUnit->refArray().checkNumerics());
            std::cout << pUnit->refArrayShape() << std::endl;
            LOG(model.getCalcPerformance(i));
            LOG(model.getGradientPerformance(i));
        }
        LOG_TIME(overallMeasure.accumulated);
        LOG_TIME(passMeasure.accumulated);
        LOG_TIME(optMeasure.accumulated);
    }
};

#endif