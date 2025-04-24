#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>
#include <cstdlib>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <immintrin.h>
#include <type_traits>
#include <utility>
#include <memory>
#include <bitset>

#define assertm(exp, msg) assert((void(msg), exp))
#define LOG(x) std::cout << #x << ": \t" << x << std::endl
#define LOG_TIME(x) std::cout << #x << ": \t" << std::chrono::duration_cast<std::chrono::microseconds>(x) << std::endl
#define DATATYPE float

#include "array/array_library.hpp"
#include "autodiff/autodiff.hpp"
#include "autodiff/diff_nn.hpp"
#include "array/random.hpp"
#include "data_loader.hpp"
#include "save_bmp.h"
#include "array/simd_vector.hpp"

using namespace ArrayLibrary;
using namespace ArrayLibrary::Matmul;

template <typename T>
std::ostream &operator<<(std::ostream &s, const std::vector<T> &v)
{
    s.put('[');
    for (char comma[]{'\0', ' ', '\0'}; const T &e : v)
        s << comma << e, comma[0] = ',';
    return s << "]";
}

class Test
{
public:
    static bool approxEqual(DATATYPE v1, DATATYPE v2, DATATYPE eps = 1e-5)
    {
        return std::abs(v2 - v1) < eps;
    }

    static void matmulPerfShape()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        PerformanceMeasure shortProductAxisMeasure;
        PerformanceMeasure longProductAxisMeasure;

        DATATYPE thinSum = 0;
        DATATYPE sqaureSum = 0;

        for (int i = 0; i < 10; i++)
        {
            auto A = randomArrayGenerator.normal<DATATYPE>({16, 200, 1}, 0, 1);
            auto B = randomArrayGenerator.normal<DATATYPE>({16, 1, 784}, 0, 1);
            auto C = randomArrayGenerator.normal<DATATYPE>({1, 200, 784}, 0, 1);
            auto D = randomArrayGenerator.normal<DATATYPE>({16, 784, 1}, 0, 1);

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

    static void outerProductPerf()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<DATATYPE>({4096, 1}, 0, 1);
        auto B = randomArrayGenerator.normal<DATATYPE>({1, 4096}, 0, 1);

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

    static void reducePerf()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<DATATYPE>({4096, 1}, 0, 1);
        auto B = randomArrayGenerator.normal<DATATYPE>({1, 4096}, 0, 1);

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

    static void concurrencyTest()
    {
        PerformanceMeasure concMeasure;
        PerformanceMeasure singleMeasure;

        RandomArrayGenerator randomArrayGenerator(0);
        auto A = randomArrayGenerator.normal<DATATYPE>({1, 100, 800}, 0, 1);
        auto B = randomArrayGenerator.normal<DATATYPE>({20, 800, 100}, 0, 1);

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

    static void printingTest()
    {
        auto A = Array<int>::range(18).reshape({3, 2, 3});
        std::cout << A << std::endl;
    }

    template <DataType T>
    static T checksum(const Array<T> &arr)
    {
        auto flatlength = arr.getFlatLength();
        auto checksumVector = Array<DATATYPE>::range(flatlength).reshape(arr.refShape()).cos();
        return (arr * checksumVector).reduceSum().eval();
    }

    template <DataType T>
    static void gradientTest()
    {
        auto layer1WeightsBare = Array<T>::range(1000).reshape({10, 100}).square().sin();
        auto layer1BiasBare = Array<T>::range(10).square().sin();

        auto diffTape = DiffTape<T>();
        auto &input = *(new Variables<T>(diffTape, {-1, 100}));
        auto &labels = *(new Variables<T>(diffTape, {-1, 10}));
        auto &layer1Weights = *(new Coefficients<T>(diffTape, layer1WeightsBare));
        auto &layer1Bias = *(new Coefficients<T>(diffTape, layer1BiasBare));

        auto &layer1Pre = matvecmul(layer1Weights, input);
        auto &layer1 = layer1Pre + layer1Bias;
        auto &sftm = layer1.softmax({-1});
        auto &dist = sftm - labels;
        auto &prod = dist * dist;
        auto &cost = prod.reduceSum();

        T learningRate = 1e-3;

        input.setValue(Array<T>::range(100).reshape({1, 100}).intPow(3).sin());
        labels.setValue(Array<T>::range(10).reshape({1, 10}).intPow(4).sin());

        auto layer1WeightsGradient = diffTape.getGradient(layer1Weights, cost);
        auto layer1BiasGradient = diffTape.getGradient(layer1Bias, cost);

        /*        [input, labels, layer1Weights, layer1Bias, layer1Pre, layer1, sftm, dist, prod, cost]

        1.571174239198852
        -1.1359495664045085
        2.6065913261879787
        5.764624310437291
        29.770156227548874
        -1.5742356314680626
        -0.7190281841725917
        -1.2722775033062013
        -2.4253578383577237
        -1.2722775033062013
        -3.1443860356687523
        -1.2722775033062013
        -0.2757208861519817
        -5.764624310437291
        -2.8823121552186457
        -5.764624310437291
        -1.8356536757736093
        0.42162378262054656
        6.659353256225586
        1.0*/

        assert(approxEqual(checksum(input.refArray()), 1.571174239198852));
        assert(approxEqual(checksum(input.refGradient()), -1.1359495664045085));
        assert(approxEqual(checksum(labels.refArray()), 2.6065913261879787));
        assert(approxEqual(checksum(labels.refGradient()), 5.764624310437291));
        assert(approxEqual(checksum(layer1Weights.refArray()), 29.770156227548874));
        assert(approxEqual(checksum(layer1Weights.refGradient()), -1.5742356314680626));
        assert(approxEqual(checksum(layer1Bias.refArray()), -0.7190281841725917));
        assert(approxEqual(checksum(layer1Bias.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(layer1Pre.refArray()), -2.4253578383577237));
        assert(approxEqual(checksum(layer1Pre.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(layer1.refArray()), -3.1443860356687523));
        assert(approxEqual(checksum(layer1.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(sftm.refArray()), -0.2757208861519817));
        assert(approxEqual(checksum(sftm.refGradient()), -5.764624310437291));
        assert(approxEqual(checksum(dist.refArray()), -2.8823121552186457));
        assert(approxEqual(checksum(dist.refGradient()), -5.764624310437291));
        assert(approxEqual(checksum(prod.refArray()), -1.8356536757736093));
        assert(approxEqual(checksum(prod.refGradient()), 0.42162378262054656));
        assert(approxEqual(checksum(cost.refArray()), 6.659353256225586));
        assert(approxEqual(checksum(cost.refGradient()), 1.0));

        /*for (int i = 0; i < diffTape.mUnits.size(); i++)
        {
            auto &unit = *(diffTape.mUnits[i]);
            std::cout << typeid(unit).name() << std::endl;
            auto &shape = unit.refArrayShape();
            auto &array = unit.refArray();
            auto &gradient = unit.refGradient();
            auto flatlength = array.getFlatLength();
            std::cout << shape << std::endl;
            auto checksumVector = Array<DATATYPE>::range(flatlength).reshape(shape).cos();
            std::cout << "Array checksum: " << (array * checksumVector).reduceSum() << std::endl;
            std::cout << "Gradient checksum: " << (gradient * checksumVector).reduceSum() << std::endl;
        }*/

        std::cout << "Gradient test successful." << std::endl;
    }

    template <DataType T>
    static void gradientTest2()
    {
        auto layer1WeightsBare = Array<T>::range(1000).reshape({10, 100}).square().sin();
        auto layer1BiasBare = Array<T>::range(10).square().sin();

        auto diffTape = DiffTape<T>();
        auto &input = *(new Variables<T>(diffTape, {-1, 100}));
        auto &labels = *(new Variables<T>(diffTape, {-1, 10}));
        auto &layer1Weights = *(new Coefficients<T>(diffTape, layer1WeightsBare));
        auto &layer1Bias = *(new Coefficients<T>(diffTape, layer1BiasBare));

        auto &layer1Pre = matvecmul(layer1Weights, input);
        auto &layer1 = layer1Pre + layer1Bias;
        auto &sftm = layer1.softmax({-1});
        auto &dist = sftm - labels;
        auto &prod = dist * dist;
        auto &cost = prod.reduceSum();

        T learningRate = 1e-3;

        input.setValue(Array<T>::range(100).reshape({1, 100}).intPow(3).sin());
        labels.setValue(Array<T>::range(10).reshape({1, 10}).intPow(4).sin());

        auto layer1WeightsGradient = diffTape.getGradient(layer1Weights, cost);
        auto layer1BiasGradient = diffTape.getGradient(layer1Bias, cost);

        /*        [input, labels, layer1Weights, layer1Bias, layer1Pre, layer1, sftm, dist, prod, cost]

        1.571174239198852
        -1.1359495664045085
        2.6065913261879787
        5.764624310437291
        29.770156227548874
        -1.5742356314680626
        -0.7190281841725917
        -1.2722775033062013
        -2.4253578383577237
        -1.2722775033062013
        -3.1443860356687523
        -1.2722775033062013
        -0.2757208861519817
        -5.764624310437291
        -2.8823121552186457
        -5.764624310437291
        -1.8356536757736093
        0.42162378262054656
        6.659353256225586
        1.0*/

        assert(approxEqual(checksum(input.refArray()), 1.571174239198852));
        assert(approxEqual(checksum(input.refGradient()), -1.1359495664045085));
        assert(approxEqual(checksum(labels.refArray()), 2.6065913261879787));
        assert(approxEqual(checksum(labels.refGradient()), 5.764624310437291));
        assert(approxEqual(checksum(layer1Weights.refArray()), 29.770156227548874));
        assert(approxEqual(checksum(layer1Weights.refGradient()), -1.5742356314680626));
        assert(approxEqual(checksum(layer1Bias.refArray()), -0.7190281841725917));
        assert(approxEqual(checksum(layer1Bias.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(layer1Pre.refArray()), -2.4253578383577237));
        assert(approxEqual(checksum(layer1Pre.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(layer1.refArray()), -3.1443860356687523));
        assert(approxEqual(checksum(layer1.refGradient()), -1.2722775033062013));
        assert(approxEqual(checksum(sftm.refArray()), -0.2757208861519817));
        assert(approxEqual(checksum(sftm.refGradient()), -5.764624310437291));
        assert(approxEqual(checksum(dist.refArray()), -2.8823121552186457));
        assert(approxEqual(checksum(dist.refGradient()), -5.764624310437291));
        assert(approxEqual(checksum(prod.refArray()), -1.8356536757736093));
        assert(approxEqual(checksum(prod.refGradient()), 0.42162378262054656));
        assert(approxEqual(checksum(cost.refArray()), 6.659353256225586));
        assert(approxEqual(checksum(cost.refGradient()), 1.0));

        std::cout << "Gradient test successful." << std::endl;
    }

    static void performanceMeasureTest()
    {
        PerformanceMeasure outer;
        PerformanceMeasure inner;

        DATATYPE dummy = 0;

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
    static void mnistRun()
    {
        RandomArrayGenerator randomArrayGenerator(0);
        auto mnist = Loader<T>::loadMNIST(0x4000);
        Array<T> images(mnist.data.reshape({-1, 784}));
        images /= 255;
        auto onehot = mnist.label.reshape({-1}). template oneHot<T>();

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = *(new Variables<T>(diffTape, {-1, 784}));
        auto &labels = *(new Variables<T>(diffTape, {-1, 10}));

        auto layer1 = LinearLayer<T>::create(input, 200, Activation::LEAKYRELU, T(0.01));
        auto layer2 = LinearLayer<T>::create(layer1, 10, Activation::NONE);

        auto &sftm = layer2.output.softmax({-1});
        // auto &dist = sftm - labels;
        // auto &cost = (dist * dist).reduceMean();
        auto &cost = *(new MeanSquaredError<T>(sftm, labels));

        int epochs = 1;
        long batchSize = 16;
        long batchCount = mnist.getLength() / batchSize;
        T learningRate = 1e-3;
        T clipValue = 1;
        T totalCost = 0;

        PerformanceMeasure overallMeasure;
        PerformanceMeasure setMeasure;
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

                setMeasure.start();
                input.setValue(images.slice({batchStart}, {batchEnd}));
                labels.setValue(onehot.slice({batchStart}, {batchEnd}));
                setMeasure.stop();

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
        LOG_TIME(setMeasure.accumulated);
        LOG_TIME(calculateMeasure.accumulated);
        LOG_TIME(applyMeasure.accumulated);
    }

    template <DataType T>
    static Array<T> simd_test()
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
            auto a = SimdVector<T>::load(pDataA);
            auto b = SimdVector<T>::load(pDataB);
            SimdVector<T>::store(pDataC, SimdVector<T>::multiply(a, b));

            pDataA += LENGTH;
            pDataB += LENGTH;
            pDataC += LENGTH;
        }
        simdMeasure.stop();

        return C;
    }

    template <DataType T>
    static Array<T> simd_benchmark()
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
    static Array<T> simd_matmul()
    {
        RandomArrayGenerator randomArrayGenerator(0);

        int n = 0x200;
        constexpr int LENGTH = SimdVector<T>::LENGTH;

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
                auto a = SimdVector<T>::broadcast_set(*pDataA);
                for (int j = 0; j < n; j += LENGTH)
                {
                    auto b = SimdVector<T>::load(pDataB);
                    auto c = SimdVector<T>::load(pDataC);
                    SimdVector<T>::store(pDataC, SimdVector<T>::fused_add_multipy(a, b, c));

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
    static void simd_array_matmul()
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
    static void simd_array_integrated_test()
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
    static Array<T> benchmark_matmul()
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

    static void simd_masking()
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
};

int main()
{
    Test::mnistRun<float>();
    LOG_TIME(clipMeasure.accumulated);
    LOG_TIME(mathmeasure.accumulated);
    return 0;
}