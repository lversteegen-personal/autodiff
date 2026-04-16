#ifndef TEST_GRADIENT_H
#define TEST_GRADIENT_H

#include "test_util.hpp"

using namespace AutoDiff;
using namespace AutoDiff::NeuralNetworks;

namespace Test::Gradient
{
    //WARNING: The generated pseudorandom numbers may differ if compiler optimizations are applied, which may lead to false positive test failures

    template <DataType T>
    void gradientTest()
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
        auto &sftm = softmax(layer1, {-1});
        auto &dist = sftm - labels;
        auto &prod = dist * dist;
        auto &cost = reduceSum(prod);

        T learningRate = 1e-3;

        input.setValue(Array<T>::range(100).reshape({1, 100}).intPow(3).sin());
        labels.setValue(Array<T>::range(10).reshape({1, 10}).intPow(4).sin());

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units({&input, &labels, &layer1Weights, &layer1Bias, &layer1Pre, &layer1, &sftm, &dist, &prod, &cost});
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
    void gradientTest2()
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

        auto &sftm = softmax(layer2.output, {-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        input.setValue(generatePseudorandom<T, [](T x)
                                            { return 7 * x; }>({16, 784}));
        labels.setValue(generatePseudorandom<T, [](T x)
                                             { return 13 * x * x * x + 2 * x; }>({16, 10}));

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1.output, &layer2Weights, &layer2Bias, &layer2.output, &sftm, &cost};
        T targets[] = {-2.244379721624708, 43.50746962845771, -0.05304572445633804, -0.30693305592170655, 0.015767526079134953, 1.086805486745606, -13.121347531406986, 0.11099250483283919, 9.453542428028797, 0.010960845784722312, -0.9414516503052217, -0.996517497385196, 0.33048479830788235, -0.0030711601738893594, 146.8347763513801, -0.0727055500100524, 1.4816195117838626, 0.30693305592170655, 9.40982437133789, 1.0};

        const Array<T> &arr = labels.refArray();
        auto arr2 = Array<T>::constant({arr.getFlatLength()}, 0);

        for (long i = 0; i < arr.getFlatLength(); i++)
        {
            T x = i;
            if (arr.getFlat(i) != std::sin(13 * x * x * x + 2 * x))
            {
                std::cout << x << "\n";
                std::cout << arr.getFlat(i) << "\n";
                std::cout << std::sin(13 * x * x * x + 2 * x) << "\n";
                std::cout << arr.getFlat(i) - std::sin(13 * x * x * x + 2 * x) << "\n";
            }
            arr2[{i}] = std::sin(13 * x * x * x + 2 * x);
        }

        std::cout << checksum(arr) << "\n";
        std::cout << checksum(arr2) << "\n";

        for (int i = 0; i < units.size(); i++)
        { 
            TEST_LOG(approxEqual(checksum(units[i]->refArray()), targets[2 * i]), std::format("Unit {} does not have the correct values.", i));
        }

        for (int i = 0; i < units.size(); i++)
        {
            TEST_LOG(approxEqual(checksum(units[i]->refGradient()), targets[2 * i + 1]), std::format("Unit {} does not have the correct gradient.", i));
        }

        std::cout << "Gradient test 2 successful." << std::endl;
    }

    template <DataType T>
    void gradientTestMnist()
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

        auto &sftm = softmax(layer2.output, {-1});
        auto &cost = MeanSquaredError<T>::create(sftm, labels);

        input.setValue(mnist.data.slice({0}, {16}));
        labels.setValue(onehot.slice({0}, {16}));

        diffTape.calculateAll(cost);

        std::vector<Unit<T> *> units = {&input, &labels, &layer1Weights, &layer1Bias, &layer1.output, &layer2Weights, &layer2Bias, &layer2.output, &sftm, &cost};
        T targets1[] = {-0.3967533457503727, -0.008208444253053601, -0.8704844306745688, -0.29677117442197276, 27.574123520337626, -0.03632134417143752, 0.0, -0.10300240543684935, 17.481087726477313, -0.025083196563216396, 11.299552865185444, 0.22366353266626227, 0.0, 0.05524654861660283, 5.217154828019423, 0.05343362330526041, 0.6133714012233566, 0.29677117442197276, 1.4584695100784302, 1.0};

        for (int i = 0; i < units.size(); i++)
        {
            // std::cout << "Unit " << i << " (" << typeid(*(units[i])).name() << ")" << std::endl;
            // LOG(units[i]->refWildcardShape());
            TEST_LOG(approxEqual(checksum(units[i]->refArray()), targets1[2 * i]), std::format("Unit {} does not have the correct values.", i));
            TEST_LOG(approxEqual(checksum(units[i]->refGradient()), targets1[2 * i + 1]), std::format("Unit {} does not have the correct gradient.", i));
        }

        T targets2[] = {27.5741601085202, 0.00010300241060871736, 11.29932905014783, -5.524654736614849e-05};

        layer1.applyGradient(diffTape, cost, 1e-3, 1000);
        layer2.applyGradient(diffTape, cost, 1e-3, 1000);

        assert(approxEqual(checksum(layer1.refKernel().refArray()), targets2[0]));
        assert(approxEqual(checksum(layer1.refBias().refArray()), targets2[1]));
        assert(approxEqual(checksum(layer2.refKernel().refArray()), targets2[2]));
        assert(approxEqual(checksum(layer2.refBias().refArray()), targets2[3]));

        std::cout << "MNIST gradient test successful." << std::endl;
    }

    template <DataType T>
    void gradientTestMnist2()
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

        auto &sftm = softmax(layer2.output, {-1});
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

            /*LOG(i);
            LOG(cost.refArray().eval());
            for (int k = 0; k < units.size(); k++)
            {
                std::cout << "Unit " << k << " (" << typeid(*(units[k])).name() << ")" << std::endl;
                LOG(units[k]->refWildcardShape());
                LOG(checksum(units[k]->refArray()));
                LOG(checksum(units[k]->refGradient()));
            }*/

            layer1.applyGradient(diffTape, cost, 1e-3, 1000);
            layer2.applyGradient(diffTape, cost, 1e-3, 1000);

            totalCost += cost.refArray().eval();
        }

        std::cout << "MNIST gradient test 2 successful." << std::endl;
    }

    /// WARNING: The generated pseudorandom numbers may differ if compiler optimizations are applied, which may lead to false positive test failures
    void all()
    {
        gradientTest<float>();
        gradientTest2<float>();
        gradientTestMnist<float>();
        gradientTestMnist2<float>();
    }
}

#endif
