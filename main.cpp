#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include "array.hpp"
#include "difftape.hpp"
#include "random.hpp"
#include "data_loader.hpp"
#include "save_bmp.h"

template <typename T>
std::ostream &operator<<(std::ostream &s, const std::vector<T> &v)
{
    s.put('[');
    for (char comma[]{'\0', ' ', '\0'}; const T &e : v)
        s << comma << e, comma[0] = ',';
    return s << "]";
}

int main()
{
    RandomArrayGenerator randomArrayGenerator(0);
    auto mnist = Loader::loadMNIST(20);
    Array<double> images(mnist.data.reshape({-1, 784}));
    Array<int> labels(mnist.label.reshape({-1}));
    auto onehot = labels.oneHot<double>();

    auto layer1Weights = randomArrayGenerator.normal({1, 10, 784});
    auto layer1Bias = randomArrayGenerator.normal({1, 10});

    auto diffTape = DiffTape<double>();
    auto &input = *(new Coefficients<double>(diffTape, images.reshape({-1, 784, 1}) / 255));
    auto &labelsD = *(new Coefficients<double>(diffTape, onehot.reshape({-1, 10})));
    auto &layer1WeightsD = *(new Coefficients<double>(diffTape, layer1Weights));
    auto &layer1BiasD = *(new Coefficients<double>(diffTape, layer1Bias));

    auto &layer1 = matmul(layer1WeightsD, input).reshape({-1, 10}) + layer1BiasD; // leakyReLu(,0.1);
    auto &sftm = layer1.softmax({-1});
    auto &dist = sftm - labelsD;
    auto &cost = (dist * dist).reduceSum();

    auto dlayer1Bias = diffTape.getGradient(layer1BiasD, cost);

    for (int i = 0; i < diffTape.mUnits.size(); i++)
    {
        std::cout << typeid(*(diffTape.mUnits[i])).name() << std::endl;
        std::cout << diffTape.mUnits[i]->refArrayShape() << std::endl;
        std::cout << diffTape.mUnits[i]->refArray().reduceMax() << std::endl;
        std::cout << diffTape.mUnits[i]->refArray().reduceMin() << std::endl;
        std::cout << diffTape.mUnits[i]->mGradient.reduceMax() << std::endl;
        std::cout << diffTape.mUnits[i]->mGradient.reduceMin() << std::endl;
    }
}