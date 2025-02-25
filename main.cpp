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

    auto x = randomArrayGenerator.uniformInteger<double>({3,3}, -1, 2);
    auto y = randomArrayGenerator.uniformInteger<double>({1,3}, -1, 2);
    auto a = x+x.transpose(0,1);
    //auto b = x*y;
    std::cout << x << std::endl;
    std::cout << a << std::endl;
    //std::cout << b << std::endl;

    /*auto diffTape = DiffTape<long>(true);
    auto& x = *(new Coefficients<long>(diffTape, randomArrayGenerator.uniformInteger<long>({3,3}, -1, 2)));
    auto& y = *(new Coefficients<long>(diffTape,  randomArrayGenerator.uniformInteger<long>({3,3}, -1, 2)));

    auto& z = reduceSum(x*(x+y));

    auto a = diffTape.getGradient(x, z);
    auto b = diffTape.getGradient(y, z);

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;*/
}