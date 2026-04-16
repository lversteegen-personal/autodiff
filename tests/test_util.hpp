#ifndef TEST_UTIL_H
#define TEST_UTIL_H

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

#include "../array/array_library.hpp"
#include "../autodiff/autodiff.hpp"
#include "../autodiff/diff_nn.hpp"
#include "../array/random.hpp"
#include "../data_loader.hpp"
#include "../save_bmp.h"

#define assertm(exp, msg) assert((void(msg), exp))
#define TEST_LOG(condition, message) if (!condition) {std::cout << message << ", file " << __FILE__ << ", line " << __LINE__ << std::endl; std::exit(1);}
#define LOG(x) std::cout << #x << ": \t" << x << std::endl
#define LOG_TIME(x) std::cout << #x << ": \t" << std::chrono::duration_cast<std::chrono::microseconds>(x) << std::endl
#define DEBUG_MODE

namespace Test
{
    template <DataType T>
    static bool approxEqual(T v1, T v2, T eps = 1e-3)
    {
        auto max = std::max(std::abs(v1), std::abs(v2));
        max = max < 1 ? 1 : max;
        return std::abs(v2 - v1) / max < eps;
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &s, const std::vector<T> &v)
    {
        s.put('[');
        for (char comma[]{'\0', ' ', '\0'}; const T &e : v)
            s << comma << e, comma[0] = ',';
        return s << "]";
    }

    template <DataType T>
    T checksum(const Array<T> &arr)
    {
        auto flatlength = arr.getFlatLength();
        auto checksumVector = Array<T>::range(flatlength).reshape(arr.refShape()).cos();
        return (arr * checksumVector).reduceSum().eval();
    }

    template <DataType T, T (*f)(T)>
    Array<T> generatePseudorandom(Coordinates shape)
    {
        long flatLength = 1;
        for (int i = 0; i < shape.size(); i++)
            flatLength *= shape[i];

        Array<T> arr = Array<T>::range(flatLength);

        using Pseudorandom = Compose<Sin<T>, Functional<T, T, f>, T>;
        return compute<Pseudorandom>(arr).reshape(shape);
    }
}

#endif