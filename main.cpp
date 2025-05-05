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

#define DEBUG_MODE

#include "save_bmp.h"

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
    template <DataType T>
    static bool approxEqual(T v1, T v2, T eps = 1e-3)
    {
        auto max = std::max(std::abs(v1), std::abs(v2));
        max = max < 1 ? 1 : max;
        return std::abs(v2 - v1) / max < eps;
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

    template <DataType T, T (*f)(T)>
    static Array<T> generatePseudorandom(Coordinates shape)
    {
        long flatLength = 1;
        for (int i = 0; i < shape.size(); i++)
            flatLength *= shape[i];

        Array<T> arr = Array<T>::range(flatLength).template unaryApply<Array<T>::template compose<Array<T>::sin_ptw, f>>();
        return arr.reshape(shape);
    }

    template <DataType T>
    static void gradientTest()
    {
        auto layer1WeightsBare = Array<T>::range(1000).reshape({10, 100}).square().sin();
        auto layer1BiasBare = Array<T>::range(10).square().sin();

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 100});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});
        auto &layer1Weights = Coefficients<T>::create(diffTape, layer1WeightsBare);
        auto &layer1Bias = Coefficients<T>::create(diffTape, layer1BiasBare);

        auto &layer1Pre = matvecmul(layer1Weights, input);
        auto &layer1 = layer1Pre + layer1Bias;
        auto &sftm = layer1.softmax({-1});
        auto &dist = sftm - labels;
        auto &prod = dist * dist;
        auto &cost = prod.reduceSum();

        T learningRate = 1e-3;

        input.setValue(Array<T>::range(100).reshape({1, 100}).intPow(3).sin());
        labels.setValue(Array<T>::range(10).reshape({1, 10}).intPow(4).sin());

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1Pre, &layer1, &sftm, &dist, &prod, &cost};
        T targets[] = {
            1.571174239198852, -1.1359495664045085, 2.6065913261879787, 5.764624310437291, 29.770156227548874, -1.5742356314680626, -0.7190281841725917, -1.2722775033062013, -2.4253578383577237, -1.2722775033062013, -3.1443860356687523, -1.2722775033062013, -0.2757208861519817, -5.764624310437291, -2.8823121552186457, -5.764624310437291, -1.8356536757736093, 0.42162378262054656, 6.659353256225586, 1.0};

        for (int i = 0; i < units.size(); i++)
        {
            assert(approxEqual(checksum(units[i]->refArray()), targets[2 * i]));
            assert(approxEqual(checksum(units[i]->refGradient()), targets[2 * i + 1]));
        }

        std::cout << "Gradient test successful." << std::endl;
    }

    template <DataType T>
    static void gradientTest2()
    {

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        auto &layer1Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return x; }>({200, 784}));
        auto &layer1Bias = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                  { return 3 * x * x; }>({200}));

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(layer1Weights, layer1Bias, Activation::LEAKYRELU, T(0.01)));

        auto &layer2Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return 11 * x; }>({10, 200}));
        auto &layer2Bias = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                  { return 3 * x * x + 17 * x; }>({10}));
        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(layer2Weights, layer2Bias, Activation::NONE, T(0.01)));

        auto &sftm = layer2.output.softmax({-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        input.setValue(generatePseudorandom<T, [](T x)
                                            { return 7 * x; }>({16, 784}));
        labels.setValue(generatePseudorandom<T, [](T x)
                                             { return 13 * x * x * x + 2 * x; }>({16, 10}));

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1.output, &layer2Weights, &layer2Bias, &layer2.output, &sftm, &cost};
        T targets[] = {-2.244379721624708, 43.50746962845771, -0.05304572445633804, -0.30693305592170655, 0.015767526079134953, 1.086805486745606, -13.121347531406986, 0.11099250483283919, 9.453542428028797, 0.010960845784722312, -0.9414516503052217, -0.996517497385196, 0.33048479830788235, -0.0030711601738893594, 146.8347763513801, -0.0727055500100524, 1.4816195117838626, 0.30693305592170655, 9.40982437133789, 1.0};

        for (int i = 0; i < units.size(); i++)
        {
            assert(approxEqual(checksum(units[i]->refArray()), targets[2 * i]));
        }

        for (int i = 0; i < units.size(); i++)
        {
            assert(approxEqual(checksum(units[i]->refGradient()), targets[2 * i + 1]));
        }

        std::cout << "Gradient test successful." << std::endl;
    }

    /*template <DataType T>
    static void gradientTestMnist()
    {
        auto mnist = Loader<T>::loadMNIST(0x4000);
        Array<T> images(mnist.data.reshape({-1, 784}));
        images /= 255;
        auto onehot = mnist.label.template oneHot<T>();

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        auto &layer1Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return x * x; }>({200, 784}) *
                                                                    std::sqrt(6.0 / (784 + 200)));
        auto &layer1Bias = Coefficients<T>::create(diffTape, Array<T>::constant({200}, 0));

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(layer1Weights, layer1Bias, Activation::LEAKYRELU, T(0.01)));

        auto &layer2Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return x * x; }>({10, 200}) *
                                                                    std::sqrt(6.0 / (200 + 10)));
        auto &layer2Bias = Coefficients<T>::create(diffTape, Array<T>::constant({10}, 0));

        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(layer2Weights, layer2Bias, Activation::NONE, T(0.01)));

        auto &sftm = layer2.output.softmax({-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        input.setValue(mnist.data.slice({0}, {16}));
        labels.setValue(onehot.slice({0}, {16}));

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1.output, &layer2Weights, &layer2Bias, &layer2.output, &sftm, &cost};
        T targets1[] = {-0.3967533457503727, -0.008208444253053601, -0.8704844306745688, -0.29677117442197276, 27.574123520337626, -0.03632134417143752, 0.0, -0.10300240543684935, 17.481087726477313, -0.025083196563216396, 11.299552865185444, 0.22366353266626227, 0.0, 0.05524654861660283, 5.217154828019423, 0.05343362330526041, 0.6133714012233566, 0.29677117442197276, 1.4584695100784302, 1.0};

        for (int i = 0; i < units.size(); i++)
        {
            std::cout << "Unit " << i << " (" << typeid(*(units[i])).name() << ")" << std::endl;
            LOG(units[i]->refWildcardShape());
            assert(approxEqual(checksum(units[i]->refArray()), targets1[2 * i]));
            assert(approxEqual(checksum(units[i]->refGradient()), targets1[2 * i + 1]));
        }

        T targets2[] = {27.5741601085202, 0.00010300241060871736, 11.29932905014783, -5.524654736614849e-05};

        layer1.applyGradient(diffTape, cost, 1e-3, 1000);
        layer2.applyGradient(diffTape, cost, 1e-3, 1000);

        assert(approxEqual(checksum(layer1.mWeightMatrix.refArray()), targets2[0]));
        assert(approxEqual(checksum(layer1.mBiasVector.refArray()), targets2[0]));
        assert(approxEqual(checksum(layer2.mWeightMatrix.refArray()), targets2[0]));
        assert(approxEqual(checksum(layer2.mBiasVector.refArray()), targets2[0]));

        std::cout << "Gradient test successful." << std::endl;
    }*/

    template <DataType T>
    static void gradientTestMnist()
    {
        auto mnist = Loader<T>::loadMNIST(0x4000);
        Array<T> images(mnist.data.reshape({-1, 784}));
        images /= 255;
        auto onehot = mnist.label.template oneHot<T>();

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        auto &layer1Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return x * x; }>({200, 784}) *
                                                                    std::sqrt(6.0 / (784 + 200)));
        auto &layer1Bias = Coefficients<T>::create(diffTape, Array<T>::constant({200}, 0));

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(layer1Weights, layer1Bias, Activation::LEAKYRELU, T(0.01)));

        auto &layer2Weights = Coefficients<T>::create(diffTape, generatePseudorandom<T, [](T x)
                                                                                     { return x * x; }>({10, 200}) *
                                                                    std::sqrt(6.0 / (200 + 10)));
        auto &layer2Bias = Coefficients<T>::create(diffTape, Array<T>::constant({10}, 0));

        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(layer2Weights, layer2Bias, Activation::NONE, T(0.01)));

        auto &sftm = layer2.output.softmax({-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1.output, &layer2Weights, &layer2Bias, &layer2.output, &sftm, &cost}; //{&layer1Weights, &layer1Bias, &layer2Weights, &layer2Bias};
        T totalCost = 0;

        for (int i = 0; i < 5; i++)
        {
            auto batchStart = i * 0x10;
            auto batchEnd = (i + 1) * 0x10;

            input.setValue(images.slice({batchStart}, {batchEnd}));
            labels.setValue(onehot.slice({batchStart}, {batchEnd}));

            diffTape.calculateAll(cost);

            LOG(i);
            LOG(cost.refArray().eval());
            for (int k = 0; k < units.size(); k++)
            {
                std::cout << "Unit " << k << " (" << typeid(*(units[k])).name() << ")" << std::endl;
                LOG(units[k]->refWildcardShape());
                LOG(checksum(units[k]->refArray()));
                LOG(checksum(units[k]->refGradient()));
            }

            layer1.applyGradient(diffTape, cost, 1e-3, 1000);
            layer2.applyGradient(diffTape, cost, 1e-3, 1000);

            totalCost += cost.refArray().eval();
        }
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
        auto onehot = mnist.label.template oneHot<T>();

        DiffTape<T> diffTape = DiffTape<T>();
        auto &input = Variables<T>::create(diffTape, {-1, 784});
        auto &labels = Variables<T>::create(diffTape, {-1, 10});

        using LayerSettings = LinearLayer<T>::template Settings<T>;
        using Activation = LinearLayer<T>::Activation;

        auto layer1 = LinearLayer<T>::create(input, LayerSettings(200, Activation::LEAKYRELU, T(0.01)));
        auto layer2 = LinearLayer<T>::create(layer1, LayerSettings(10, Activation::NONE, T(0.01)));

        auto &sftm = layer2.output.softmax({-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        int epochs = 10;
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

    static void simdClipAsmTest()
    {
        const SimdClipBounds<float> bounds(0.3, 0.7);
        RandomArrayGenerator rng(0);
        auto source = rng.uniform<float>({0x1000}, 0, 1);
        auto dest = Array<float>::constant({0x1000}, 0);
        float *pSourceData = source.getDataPointer();
        float *pDestData = dest.getDataPointer();
        const float *const dataEnd = pSourceData + source.getFlatLength();

        // auto lowerBound = _mm256_set1_ps(0.3);
        // auto upperBound = _mm256_set1_ps(0.7);

        for (; pSourceData < dataEnd; pDestData += 8)
        {
            auto a = SimdVector<float>::load(pSourceData);
            auto b = SimdVector<float>::clip(a, bounds);
            SimdVector<float>::store(pDestData, b);
            // auto a = _mm256_load_ps(pSourceData);
            // auto b = _mm256_min_ps(_mm256_max_ps(lowerBound, a), upperBound);
            // auto(pDestData, b);
            pSourceData += 8;
        }

        assert(approxEqual(dest.reduceMin().eval(), 0.3f));
        assert(approxEqual(dest.reduceMax().eval(), 0.7f));

        std::cout << "Simd clip test successful." << std::endl;
    }

    static void mnistDataLoad()
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
};

int main()
{
    // Test::simdClipAsmTest();
    // Test::gradientTest2<float>();
    // Test::gradientTestMnist<float>();
    Test::mnistRun<float>();
    // Test::mnistDataLoad();

    return 0;
}