#ifndef SIMD_VECTOR_H
#define SIMD_VECTOR_H

#include <immintrin.h>
#include <type_traits>
#include <utility>

namespace ArrayLibrary
{
    constexpr uint8_t SIMD_BYTES = 32;

    template <typename T>
    concept DataType = std::is_arithmetic_v<T>;

    template <DataType T>
    class SimdVector;

    template <DataType T>
    struct SimdClipBounds
    {
        SimdVector<T> lowerBound;
        SimdVector<T> upperBound;
        SimdClipBounds() = delete;

        SimdClipBounds(T lower, T upper) : lowerBound(SimdVector<T>::broadcast_set(lower)), upperBound(SimdVector<T>::broadcast_set(upper))
        {
            if (lower > upper)
                throw new std::invalid_argument("Lower bound must be below upper bound.");
        }
    };

    template <DataType T>
    class SimdVector
    {
    public:
        static constexpr bool supported = false;
        static constexpr uint8_t LENGTH = SIMD_BYTES / sizeof(T);
        static inline __m256i makePrefixMask(size_t prefixLength);

        static inline SimdVector<T> zero();
        static inline SimdVector<T> broadcast_set(const T value);
        static inline SimdVector<T> load(const T *pData);
        static inline SimdVector<T> maskedLoad(const T *pData, __m256i mask);
        static inline void store(T *pData, const SimdVector<T> &a);
        static inline void maskedStore(T *pData, __m256i mask, const SimdVector<T> &a);
        static inline SimdVector<T> add(const SimdVector<T> &a, const SimdVector<T> &b);
        static inline SimdVector<T> multiply(const SimdVector<T> &a, const SimdVector<T> &b);
        static inline SimdVector<T> subtract(const SimdVector<T> &a, const SimdVector<T> &b);
        static inline SimdVector<T> divide(const SimdVector<T> &a, const SimdVector<T> &b);
        // a * b + c
        static inline SimdVector<T> fusedAddMultiply(const SimdVector<T> &a, const SimdVector<T> &b, const SimdVector<T> &c);

        static inline SimdVector<T> max(const SimdVector<T> &a, const SimdVector<T> &b);
        static inline SimdVector<T> min(const SimdVector<T> &a, const SimdVector<T> &b);
        static inline SimdVector<T> clip(const SimdVector<T> &a, const SimdClipBounds<T> &bounds);

        static inline SimdVector<T> step(const SimdVector<T> &a, const SimdVector<T> threshold);
    };

#define CONCAT(prefix, core, suffix) prefix##core##suffix
#define LOAD COMMAND(load)
#define MASKLOAD COMMAND(maskload)
#define STORE COMMAND(store)
#define MASKSTORE COMMAND(maskstore)
#define SET_ZERO COMMAND(setzero)
#define BROADCAST_SET COMMAND(set1)
#define ADD COMMAND(add)
#define MUL COMMAND(mul)
#define SUB COMMAND(sub)
#define DIV COMMAND(div)
#define FMADD COMMAND(fmadd)
#define MAX COMMAND(max)
#define MIN COMMAND(min)

#define T float
#define INTERNAL __m256
#define COMMAND(core) CONCAT(_mm256_, core, _ps)
#define MASK_EXPAND _mm256_cvtepi8_epi32

    template <>
    class SimdVector<T>
    {
        INTERNAL mInternal;
        SimdVector(const INTERNAL &internal) : mInternal(internal) {}
        SimdVector(INTERNAL &&internal) : mInternal(std::forward<INTERNAL>(internal)) {}

    public:
        static constexpr bool supported = true;
        static constexpr uint8_t LENGTH = SIMD_BYTES / sizeof(T);
        // Needs significant changes for scalar types with other lengths
        static inline __m256i makePrefixMask(ptrdiff_t prefixLength)
        {
            static_assert(SIMD_BYTES == 32);
            static_assert(LENGTH >= 4); // Needs to be substantially rewritten for 16 bit types

            prefixLength = std::min(prefixLength, (ptrdiff_t)LENGTH);
            uint64_t intmask = prefixLength > 0 ? ((uint64_t)-1) >> (LENGTH - prefixLength) * 8 : 0;
            __m128i halfmask = _mm_cvtsi64_si128((int64_t)intmask);
            return MASK_EXPAND(halfmask);
        }

        SimdVector() : mInternal(SET_ZERO()) {}

        static inline SimdVector<T> zero() { return SET_ZERO(); }
        static inline SimdVector<T> broadcast_set(const T value) { return BROADCAST_SET(value); }
        static inline SimdVector<T> load(const T *pData) { return LOAD(pData); }
        static inline SimdVector<T> maskedLoad(const T *pData, const __m256i &mask) { return MASKLOAD(pData, mask); }
        static inline void store(T *pData, const SimdVector<T> &a) { return STORE(pData, a.mInternal); }
        static inline void maskedStore(T *pData, const __m256i &mask, const SimdVector<T> &a) { MASKSTORE(pData, mask, a.mInternal); }

        static inline SimdVector<T> add(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return SimdVector<T>(ADD(a.mInternal, b.mInternal));
        }
        static inline SimdVector<T> multiply(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return MUL(a.mInternal, b.mInternal);
        }
        static inline SimdVector<T> subtract(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return SUB(a.mInternal, b.mInternal);
        }
        static inline SimdVector<T> divide(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return DIV(a.mInternal, b.mInternal);
        }
        static inline SimdVector<T> fusedAddMultiply(const SimdVector<T> &a, const SimdVector<T> &b, const SimdVector<T> &c)
        {
            return FMADD(a.mInternal, b.mInternal, c.mInternal);
        }

        static inline SimdVector<T> max(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return MAX(a.mInternal, b.mInternal);
        }

        static inline SimdVector<T> min(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return MIN(a.mInternal, b.mInternal);
        }
        
        // Clip the values of a to be between least and most
        static inline SimdVector<T> clip(const SimdVector<T> &a, const SimdClipBounds<T> &bounds)
        {
            return max(bounds.lowerBound, min(bounds.upperBound, a));
        }
    };

#define LOAD INT_COMMAND(load)
#define STORE INT_COMMAND(store)
#define SET_ZERO INT_COMMAND(setzero)

#define T bool
#define SCALAR_TYPE int32_t
#define INTERNAL __m256i
#define COMMAND(core) CONCAT(_mm256_, core, _epi32)
#define INT_COMMAND(core) CONCAT(_mm256_, core, _si256)
#define MASK_EXPAND _mm256_cvtepi8_epi32

    template <>
    class SimdVector<T>
    {
        INTERNAL mInternal;
        SimdVector(const INTERNAL &internal) : mInternal(internal) {}
        SimdVector(INTERNAL &&internal) : mInternal(std::forward<INTERNAL>(internal)) {}

    public:
        static constexpr bool supported = false;
        static constexpr uint8_t LENGTH = SIMD_BYTES / sizeof(T);
        // Needs significant changes for scalar types with other lengths
        static inline __m256i makePrefixMask(ptrdiff_t prefixLength)
        {
            static_assert(SIMD_BYTES == 32);
            static_assert(LENGTH >= 4); // Needs to be substantially rewritten for 16 bit types

            prefixLength = std::min(prefixLength, (ptrdiff_t)LENGTH);
            uint64_t intmask = prefixLength > 0 ? ((uint64_t)-1) >> (LENGTH - prefixLength) * 8 : 0;
            __m128i halfmask = _mm_cvtsi64_si128((int64_t)intmask);
            return MASK_EXPAND(halfmask);
        }

        SimdVector() : mInternal(SET_ZERO()) {}

        static inline SimdVector<T> zero() { return SET_ZERO(); }
        static inline SimdVector<T> broadcast_set(const T value) { return BROADCAST_SET(value); }
        static inline SimdVector<T> load(const T *pData) { return LOAD((INTERNAL *)(pData)); }
        static inline SimdVector<T> maskedLoad(const T *pData, const __m256i &mask) { return MASKLOAD((SCALAR_TYPE *)pData, mask); }
        static inline void store(T *pData, const SimdVector<T> &a) { return STORE((INTERNAL *)pData, a.mInternal); }
        static inline void maskedStore(T *pData, const __m256i &mask, const SimdVector<T> &a) { MASKSTORE((SCALAR_TYPE *)pData, mask, a.mInternal); }

        static inline SimdVector<T> max(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return MAX(a.mInternal, b.mInternal);
        }

        static inline SimdVector<T> min(const SimdVector<T> &a, const SimdVector<T> &b)
        {
            return MIN(a.mInternal, b.mInternal);
        }

        static inline SimdVector<T> clip(const SimdVector<T> &a, const SimdClipBounds<T> &bounds)
        {
            return max(bounds.lowerBound, min(bounds.upperBound, a));
        }
    };

#undef INTERNAL
#undef T
#undef COMMAND
#undef MASK_EXPAND

#undef FULL_COMMAND
#undef LOAD
#undef MASKLOAD
#undef STORE
#undef MASKSTORE
#undef SET_ZERO
#undef BROADCAST_SET
#undef ADD
#undef MUL
#undef FMADD

}

#endif