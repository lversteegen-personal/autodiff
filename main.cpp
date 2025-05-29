#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>
#include <cstdlib>
#include <tuple>
#include <array>
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

#include "save_bmp.h"

#include "array/array_library.hpp"
#include "autodiff/autodiff.hpp"
#include "autodiff/diff_nn.hpp"
#include "array/random.hpp"
#include "data_loader.hpp"
#include "save_bmp.h"
#include "array/simd_vector.hpp"
#include "tests.hpp"

using namespace ArrayLibrary;
using namespace ArrayLibrary::Matmul;
using namespace AutoDiff;
using namespace AutoDiff::NeuralNetworks;

int main()
{
    Test::model<float>();
}