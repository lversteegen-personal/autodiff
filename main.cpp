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
    auto mnist = Loader::loadMNIST(10);
    Array<double> images(mnist.data.reshape({-1, 784}));
    Array<int> labels(mnist.label.reshape({-1}));
    auto onehot = labels.oneHot<double>();

    auto s1 = images.slice({0,0},{3,-1});
    auto s2 = images.slice({3,0},{6,-1});
    std::cout << s1.refShape() << std::endl;
    std::cout << s2.refShape() << std::endl;
    auto s3 = s1 + s2;
    std::cout << s3.refShape() << std::endl;

    // auto argmax = (image0 == max).findNonZero();
    // std::cout << argmax << std::endl;

    auto diffTape = DiffTape<double>();
    auto &x = *(new Coefficients<double>(diffTape, Array<double>::range(2)));

    auto &z = leakyReLu(x, 0.1); 

    auto a = diffTape.getGradient(x, z);

    for (int i = 0; i < diffTape.mUnits.size(); i++)
        std::cout << diffTape.mUnits[i]->mArray << std::endl;

    for (int i = 0; i < diffTape.mUnits.size(); i++)
        std::cout << diffTape.mUnits[i]->mGradient << std::endl;
}