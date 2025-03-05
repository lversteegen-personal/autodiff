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
    Array<double> images(mnist.data.reshape({-1, 28, 28}));
    Array<int> labels(mnist.label.reshape({-1}));
    images /= 255;
    auto image0 = images.take({0},false);
    std::cout << image0 << std::endl;
    auto max = image0.reduceMax({0}, true);
    std::cout << max << std::endl;
    //auto argmax = (image0 == max).findNonZero();
    //std::cout << argmax << std::endl;

    /*auto diffTape = DiffTape<double>(true);
    auto& x = *(new Coefficients<double>(diffTape, randomArrayGenerator.uniformInteger<double>({1,3,3}, 0, 3)));
    auto& y = *(new Coefficients<double>(diffTape,  Array<double>::range(900).reshape({100,3,3})));

    auto& u = matmul(x,y);
    auto& z = reduceSum(u);

    auto a = diffTape.getGradient(x, z);
    auto b = diffTape.getGradient(y, z);

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << u << std::endl;
    std::cout << z << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;*/
}