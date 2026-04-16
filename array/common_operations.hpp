#ifndef COMMON_UNIVERSAL_OPERATIONS_H
#define COMMON_UNIVERSAL_OPERATIONS_H

#include "constants.hpp"
#include "simd.hpp"
#include "universal_ptws.hpp"

namespace ArrayLibrary
{
    //////////////////////////////////////////////////
    // Nullary
    //////////////////////////////////////////////////

    template <DataType T>
    struct Assign
    {
        const T param;
        const Simd::Vector<T> simdParam;

        Assign(T param) : param(param), simdParam(Simd::broadcast_set(param)) {}

        static inline T f(T param) { return param; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> param) { return param; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    //////////////////////////////////////////////////
    // Arithmetic
    //////////////////////////////////////////////////

    template <DataType T>
    struct Addition
    {
        static inline T f(const T x, const T y) { return x + y; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> x, const Simd::Vector<T> y) { return x + y; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType T>
    struct Subtraction
    {
        static inline T f(const T x, const T y) { return x - y; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> x, const Simd::Vector<T> y) { return x - y; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType T>
    struct Multiplication
    {
        static inline T f(const T x, const T y) { return x * y; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> x, const Simd::Vector<T> y) { return x * y; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType T>
    struct Division
    {
        static inline T f(const T x, const T y) { return x / y; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> x, const Simd::Vector<T> y) { return x / y; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType T>
        requires std::is_integral_v<T>
    struct Modulo
    {
        static inline T f(const T x, const T y) { return x % y; }
    };

    /// @brief Computes x*y+z
    /// @tparam T
    template <DataType T>
    struct FusedMultiplyAdd
    {
        static inline T f(const T x, const T y, const T z)
        {
            return x * y + z;
        }

        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> x, const Simd::Vector<T> y, const Simd::Vector<T> z)
        {
            return Simd::fusedMultiplyAdd<T>(x, y, z);
        }
    };

    //////////////////////////////////////////////////
    // Boolean
    //////////////////////////////////////////////////

    template <DataType T>
    struct Equality
    {
        static inline bool f(const T x, const T y) { return x == y; }
    };

    template <DataType T>
    struct Inequality
    {
        static inline bool f(const T x, const T y) { return x != y; }
    };

    template <DataType T>
    struct LessThan
    {
        static inline bool f(const T x, const T y) { return x < y; }
    };

    template <DataType T>
    struct LessThanEqual
    {
        static inline bool f(const T x, const T y) { return x <= y; }
    };

    template <DataType T>
    struct LogicalAnd
    {
        static inline T f(const T x, const T y) { return x && y; }
    };

    template <DataType T>
    struct LogicalOr
    {
        static inline T f(const T x, const T y) { return x || y; }
    };

    //////////////////////////////////////////////////
    // Basic unary
    //////////////////////////////////////////////////

    template <DataType T>
    struct Copy
    {
        static inline T f(const T x) { return x; }
        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> &x) { return x; }
        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType U, DataType T>
        requires std::is_convertible_v<U, T>
    struct Convert
    {
        static inline U f(const T x) { return (U)x; }
    };

    template <DataType U, DataType T, U (*F)(const T)>
    struct Functional
    {
        static inline U f(const T x) { return F(x); }
    };

    /// @brief Takes the composition Op2::f(Op1::f(x))
    /// @tparam Op2
    /// @tparam Op1
    /// @tparam T
    template <typename Op2, typename Op1, DataType T>
        requires(IsOperation<Op1, T> && IsOperation<Op2, OperationResultType<Op1>>)
    struct Compose
    {
        static inline auto f(const T x) { return Op2::f(Op1::f(x)); }
        static inline auto fSimd(const Simd::Vector<T> x)
        {
            if constexpr (HasSimd<Op1> && HasSimd<Op2>)
                return Op2::fSimd(Op1::fSimd(x));
            else
                return Simd::zero<T>();
        }
        constexpr static bool ignoreSimd = !(HasSimd<Op1> && HasSimd<Op2>);
    };

    //////////////////////////////////////////////////
    // Numerical checks
    //////////////////////////////////////////////////

    template <DataType T>
        requires std::is_floating_point_v<T>
    struct IsNan
    {
        static inline bool f(const T x) { return std::isnan(x); }
    };

    template <DataType T>
        requires std::is_floating_point_v<T>
    struct IsInf
    {
        static inline bool f(const T x) { return std::isinf(x); }
    };

    //////////////////////////////////////////////////
    // Unary analytical
    //////////////////////////////////////////////////

    template <DataType T>
        requires std::is_floating_point_v<T>
    struct Sqrt
    {
        static inline T f(const T x) { return std::sqrt(x); }
    };

    template <DataType T, uint8_t Power>
        requires(std::is_floating_point_v<T> && Power <= 4)
    struct IntPow
    {
        static inline T f(const T x)
        {
            T result = x;

#pragma GCC unroll 4
            for (uint8_t i = 1; i < Power; i++)
                result *= x;

            return result;
        }
    };

    template <DataType T>
        requires std::is_floating_point_v<T>
    struct Exp
    {
        static inline T f(const T x) { return std::exp(x); }
    };

    template <DataType U, DataType T = U>
        requires std::is_floating_point_v<U>
    struct Sin
    {
        static inline U f(const T x) { return std::sin(x); }
    };

    template <DataType T>
        requires std::is_floating_point_v<T>
    struct Cos
    {
        static inline T f(const T x) { return std::cos(x); }
    };

    template <DataType T>
    struct Abs
    {
        static inline T f(const T x) { return std::abs(x); }
    };

    //////////////////////////////////////////////////
    // Other
    //////////////////////////////////////////////////

    template <DataType U, DataType T>
        requires std::is_floating_point_v<T>
    struct Pow
    {
        static inline U f(const T x, const T y) { return (U)std::pow(x, y); }
    };

    template <DataType T>
    struct ClipBounds
    {
        T lowerBound;
        T upperBound;
        ClipBounds() = delete;

        ClipBounds(T lower, T upper) : lowerBound(lower), upperBound(upper)
        {
            if (lowerBound > upperBound)
                throw new std::invalid_argument("Lower bound must be below upper bound.");
        }
    };

    template <DataType T>
    struct Clip
    {
        const ClipBounds<T> param;
        const Simd::ClipBounds<T> simdParam;

        Clip(T lower, T upper) : param(lower, upper), simdParam(lower, upper) {}

        static inline T f(ClipBounds<T> bounds, T x)
        {
            return x < bounds.lowerBound ? bounds.lowerBound : (x > bounds.upperBound ? bounds.upperBound : x);
        }

        inline Simd::Vector<T> clip(const Simd::ClipBounds<T> bounds, const Simd::Vector<T> x)
        {
            return Simd::clip(bounds, x);
        }

        constexpr static bool ignoreSimd = !Simd::supported<T>;
    };

    template <DataType T>
    struct ConvexCombination
    {
        const float param = 0;
        const Simd::Vector<T> simdParam;

        ConvexCombination() {}
        ConvexCombination(T param) : param(param), simdParam(Simd::broadcast_set<T>(param)) {}

        static inline T f(const T lambda, const T x, const T y)
        {
            return lambda * x + (1 - lambda) * y;
        }

        static inline Simd::Vector<T> fSimd(const Simd::Vector<T> lambda, const Simd::Vector<T> x, const Simd::Vector<T> y)
        {
            return lambda * x + (Simd::broadcast_set<T>(1) - lambda) * y;
        }
    };
}

#endif