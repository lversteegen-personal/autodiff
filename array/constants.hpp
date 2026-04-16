#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <utility>

namespace ArrayLibrary
{
    constexpr uint8_t SIMD_BYTES = 32;

    template <typename T>
    concept DataType = std::is_arithmetic_v<T>;

    template <std::totally_ordered First, std::totally_ordered... Remainder>
        requires(... && std::is_same_v<First, Remainder>)
    First max(const First &first, const Remainder &...remainder)
    {
        if constexpr (sizeof...(Remainder) == 0)
            return first;
        else
            return std::max(first, max(remainder...));
    }

    template <size_t k, typename First, typename... Remainder>
        requires(k == 0)
    inline const First packGet(const First first, const Remainder... remainder)
    {
        return first;
    }

    template <size_t k, typename First, typename... Remainder>
        requires(k >= 1 && k < sizeof...(Remainder))
    inline const auto packGet(const First first, const Remainder... remainder)
    {
        return packGet<k - 1, Remainder...>(remainder...);
    }

    template <size_t k, typename First, typename... Remainder>
        requires(k == 0)
    inline const First &refPackGet(const First &first, const Remainder &...remainder)
    {
        return first;
    }

    template <size_t k, typename First, typename... Remainder>
        requires(k >= 1 && k <= sizeof...(Remainder))
    inline const auto &refPackGet(const First &first, const Remainder &...remainder)
    {
        return refPackGet<k - 1, Remainder...>(remainder...);
    }

    template <typename ReturnType, auto f, typename ArgType, typename... Prefix>
        requires(std::is_same_v<decltype(f), ReturnType (*)(Prefix...)>)
    inline ReturnType fillArgs(ArgType filler, Prefix... args)
    {
        return f(args...);
    }

    template <typename ReturnType, auto f, typename ArgType, typename... Prefix>
        requires(sizeof...(Prefix) <= 64 && !std::is_same_v<decltype(f), ReturnType (*)(Prefix...)>)
    inline ReturnType fillArgs(ArgType filler, Prefix... args)
    {
        return fillArgs<ReturnType, f>(filler, args..., filler);
    }
}

#endif