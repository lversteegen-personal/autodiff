#ifndef SIMD_OPERATION_H
#define SIMD_OPERATION_H

#include <immintrin.h>

#include "constants.hpp"
#include "shape.hpp"

namespace ArrayLibrary
{
    namespace Simd
    {
        /// @brief The number of sclars of type T per vector
        template <DataType T>
        constexpr size_t LENGTH = SIMD_BYTES / sizeof(T);

        inline __m256i makePrefixMask(uint64_t bytes)
        {

#ifdef DEBUG_MODE
            if (bytes > SIMD_BYTES)
                throw std::invalid_argument("Cannot make a prefix mask with more set bytes than SIMD vector length");
#endif

            constexpr uint64_t ONES = (uint64_t)-1;

            const int shift = (SIMD_BYTES - bytes) * 8;
            const uint64_t x3 = ONES >> std::min(shift, 63);
            const uint64_t x2 = ONES >> std::min(std::max(0, shift - 64), 63);
            const uint64_t x1 = ONES >> std::min(std::max(0, shift - 128), 63);
            const uint64_t x0 = ONES >> std::min(std::max(0, shift - 192), 63);
            return _mm256_set_epi64x(x3, x2, x1, x0);
        }

        template <DataType T>
        inline __m256i makeTypePrefixMask(long length)
        {
            return makePrefixMask(std::min((uint64_t)std::max(length, 0L), LENGTH<T>) * sizeof(T));
        }

        template <DataType T>
        struct Internal
        {
            typedef __m256i Type;
            static constexpr bool supported = false;

            static inline Type zero();
            static inline Type broadcast_set(const T value);
            template <DataType... Ts>
                requires((... && std::is_same_v<T, Ts>))
            static inline Type set(Ts... data);
            template <DataType... Ts>
                requires((... && std::is_same_v<T, Ts>))
            static inline Type setr(Ts... data);
            static inline Type load(const T *pData);
            static inline Type maskedLoad(const T *pData, const __m256i &mask);

            static inline void store(T *pData, const Type &a);
            static inline void maskedStore(T *pData, const __m256i &mask, const Type &a);

            static inline Type add(const Type &a, const Type &b);
            static inline Type multiply(const Type &a, const Type &b);
            static inline Type subtract(const Type &a, const Type &b);
            static inline Type divide(const Type &a, const Type &b);
            static inline Type fusedMultiplyAdd(const Type &a, const Type &b, const Type &c);
            static inline Type max(const Type &a, const Type &b);
            static inline Type min(const Type &a, const Type &b);

            static inline Type cmpGe(const Type &a, const Type &b);

            static inline __m256i castToInt(const Type &a);
            static inline Type castFromInt(const __m256i &a);

            static inline Type sqrt(const Type &a);
        };

        template <DataType T>
        using Vector = typename Internal<T>::Type;

        template <DataType T>
        constexpr bool supported = Internal<T>::supported;

        template <DataType T>
        inline Vector<T> zero() { return Internal<T>::zero(); }
        template <DataType T>
        inline Vector<T> broadcast_set(const T value) { return Internal<T>::broadcast_set(value); }
        template <DataType T, DataType... Ts>
            requires((... && std::is_same_v<T, Ts>) && sizeof...(Ts) + 1 <= LENGTH<T>)
        inline Vector<T> set(T first, Ts... remainder)
        {
            return fillArgs<Vector<T>, Internal<T>::set>((T)0, first, remainder...);
        }

        template <DataType T, DataType... Ts>
            requires((... && std::is_same_v<T, Ts>) && sizeof...(Ts) + 1 <= LENGTH<T>)
        inline Vector<T> setr(T first, Ts... remainder)
        {
            return fillArgs<Vector<T>, Internal<T>::setr>((T)0, first, remainder...);
        }
        template <DataType T>
        inline Vector<T> load(const T *pData) { return Internal<T>::load(pData); }

        template <DataType T>
        inline Vector<T> maskedLoad(const T *pData, const __m256i &mask)
        {
            return Internal<T>::maskedLoad(pData, mask);
        }

        template <DataType T>
        inline void prefixLoad(T *pData, long prefixLength)
        {
            return maskedLoad<T>(pData, makeTypePrefixMask<T>(prefixLength));
        }

        template <DataType T, size_t SpreadTypeSize>
            requires(supported<T> && (SpreadTypeSize % sizeof(T) == 0) && (SpreadTypeSize == 1 || SpreadTypeSize == 2 || SpreadTypeSize == 4 || SpreadTypeSize == 8))
        inline Vector<T> spreadLoad(const T *pData)
        {
            Vector<T> a = Internal<T>::load(pData);

            // If T is int64 we always have SpreadTypeSize == sizeof(T)
            if constexpr (SpreadTypeSize == sizeof(T))
                return a;

            // If T is int32 we always have SpreadTypeSize == 8
            if constexpr (std::is_same_v<T, int32_t>)
            {
                return _mm256_cvtepi32_epi64(_mm256_castsi256_si128(a));
            }

            if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, int8_t>)
            {
                if constexpr (SpreadTypeSize == 8)
                    return _mm256_cvtepi8_epi64(_mm256_castsi256_si128(a));

                if constexpr (SpreadTypeSize == 4)
                    return _mm256_cvtepi8_epi32(_mm256_castsi256_si128(a));

                if constexpr (SpreadTypeSize == 2)
                    return _mm256_cvtepi8_epi16(_mm256_castsi256_si128(a));
            }
        }

        template <DataType T>
        inline void store(T *pData, const Vector<T> &a) { return Internal<T>::store(pData, a); }
        template <DataType T>
        inline void maskedStore(T *pData, const __m256i &mask, const Vector<T> &a) { Internal<T>::maskedStore(pData, mask, a); }

        template <DataType T>
        inline void prefixStore(T *pData, const Vector<T> &a, long prefixLength)
        {
            maskedStore<T>(pData, makeTypePrefixMask<T>(prefixLength), a);
        }

        template <DataType T, size_t SpreadTypeSize>
            requires((SpreadTypeSize % sizeof(T) == 0) && (SpreadTypeSize == 1 || SpreadTypeSize == 2 || SpreadTypeSize == 4 || SpreadTypeSize == 8) && std::is_floating_point_v<T>)
        inline void spreadStore(T *pData, const Vector<T> &a)
        {
            if constexpr (SpreadTypeSize == sizeof(T))
                store<T>(pData, a);
            else
                prefixStore<T>(pData, a, SIMD_BYTES / SpreadTypeSize);
        }

        template <DataType T, size_t SpreadTypeSize>
            requires((SpreadTypeSize % sizeof(T) == 0) && (SpreadTypeSize == 1 || SpreadTypeSize == 2 || SpreadTypeSize == 4 || SpreadTypeSize == 8) && std::is_floating_point_v<T>)
        inline void spreadStore(T *pData, const Vector<T> &a, size_t prefixLength)
        {
            prefixLength = std::min(prefixLength, SIMD_BYTES / SpreadTypeSize);
            prefixStore<T>(pData, a, prefixLength);
        }

        template <DataType T, size_t SpreadTypeSize>
            requires(supported<T> && (SpreadTypeSize % sizeof(T) == 0) && (SpreadTypeSize == 1 || SpreadTypeSize == 2 || SpreadTypeSize == 4 || SpreadTypeSize == 8) && std::is_integral_v<T>)
        inline void spreadStore(T *pData, const Vector<T> &a)
        {
            if constexpr (SpreadTypeSize == sizeof(T))
            {
                store<T>(pData, a);
            }
            else
            {
                spreadStore(pData, a, SIMD_BYTES / SpreadTypeSize);
            }
        }

        template <DataType T, size_t SpreadTypeSize>
            requires(supported<T> && (SpreadTypeSize % sizeof(T) == 0) && (SpreadTypeSize == 1 || SpreadTypeSize == 2 || SpreadTypeSize == 4 || SpreadTypeSize == 8) && std::is_integral_v<T>)
        inline void spreadStore(T *pData, const Vector<T> &a, size_t prefixLength)
        {
            prefixLength = std::min(prefixLength, SIMD_BYTES / SpreadTypeSize);
            if constexpr (SpreadTypeSize == sizeof(T))
            {
                prefixStore<T>(pData, a, prefixLength);
                return;
            }

            const __m256i crossHalfIdx = _mm256_setr_epi32(0, 2, 4, 6, 0, 0, 0, 0);

            if constexpr (SpreadTypeSize == 8)
            {
                const __m256i b = _mm256_permutevar8x32_epi32(a, crossHalfIdx);

                if constexpr (sizeof(T) == 4)
                {
                    prefixStore<T>(pData, b, prefixLength);
                }

                if constexpr (sizeof(T) == 2)
                {
                    const __m256i idx = _mm256_setr_epi16(0 + 0x100, 4 + 5 * 0x100, 8 + 9 * 0x100, 12 + 13 * 0x100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                    prefixStore<T>(pData, _mm256_shuffle_epi8(b, idx), prefixLength);
                }

                if constexpr (sizeof(T) == 1)
                {
                    const __m256i idx = _mm256_setr_epi64x(0x100 * 0b100 + 0x10000 * 0b1000 + 0x1000000 * 0b1100, 0, 0, 0);
                    prefixStore<T>(pData, _mm256_shuffle_epi8(b, idx), prefixLength);
                }
            }

            if constexpr (SpreadTypeSize == 4)
            {
                if constexpr (sizeof(T) == 2)
                {
                    const __m256i idx2 = _mm256_setr_epi16(0 + 0x100, 4 + 5 * 0x100, 8 + 9 * 0x100, 12 + 13 * 0x100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                    auto b = _mm256_shuffle_epi8(a, crossHalfIdx);
                    prefixStore<T>(pData, _mm256_permutevar8x32_epi32(b, crossHalfIdx), prefixLength);
                }

                if constexpr (sizeof(T) == 1)
                {
                    const __m256i idx = _mm256_setr_epi64x(0x100 * 0b100 + 0x10000 * 0b1000 + 0x1000000 * 0b1100, 0, 0, 0);
                    auto b = _mm256_shuffle_epi8(a, idx);
                    prefixStore<T>(pData, _mm256_permutevar8x32_epi32(b, crossHalfIdx), prefixLength);
                }
            }

            if constexpr (SpreadTypeSize == 2)
            {
                const __m256i idx = _mm256_setr_epi32(0x100 * 0b10 + 0x10000 * 0b100 + 0x1000000 * 0b110, 0b1000 + 0x100 * 0b1010 + 0x10000 * 0b1100 + 0x1000000 * 0b1110, 0, 0, 0, 0, 0, 0);
                auto b = _mm256_shuffle_epi8(a, idx);
                prefixStore<T>(pData, _mm256_permutevar8x32_epi32(b, crossHalfIdx), prefixLength);
            }
        }

        template <DataType T>
        inline Vector<T> add(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::add(a, b);
        }
        template <DataType T>
        inline Vector<T> operator+(const Vector<T> &a, const Vector<T> &b)
        {
            return add(a, b);
        }
        template <DataType T>
        inline Vector<T> multiply(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::multiply(a, b);
        }
        template <DataType T>
        inline Vector<T> operator*(const Vector<T> &a, const Vector<T> &b)
        {
            return multiply(a, b);
        }
        template <DataType T>
        inline Vector<T> subtract(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::subtract(a, b);
        }
        template <DataType T>
        inline Vector<T> operator-(const Vector<T> &a, const Vector<T> &b)
        {
            return subtract(a, b);
        }
        template <DataType T>
        inline Vector<T> divide(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::divide(a, b);
        }
        template <DataType T>
        inline Vector<T> operator/(const Vector<T> &a, const Vector<T> &b)
        {
            return divide(a, b);
        }
        template <DataType T>
        inline Vector<T> fusedMultiplyAdd(const Vector<T> &a, const Vector<T> &b, const Vector<T> &c)
        {
            return Internal<T>::fusedMultiplyAdd(a, b, c);
        }
        template <DataType T>
        inline Vector<T> max(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::max(a, b);
        }
        template <DataType T>
        inline Vector<T> min(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::min(a, b);
        }
        template <DataType T>
        static inline Vector<T> cmp_ge(const Vector<T> &a, const Vector<T> &b)
        {
            return Internal<T>::cmp_ge(a, b);
        }

        template <DataType T>
        static inline __m256i castToInt(const Vector<T> &a)
        {
            return _mm256_castps_si256(a);
        }

        template <DataType T>
        static inline Vector<T> castFromInt(const __m256i &a)
        {
            return _mm256_castsi256_ps(a);
        }

        /// @brief Returns the positive part max(a_i,0) for each coordinate of a
        /// @return
        template <DataType T>
        static inline Vector<T> pos(const Vector<T> &a)
        {
            auto cmp = cmp_ge(a, Simd::zero<T>());
            return castFromInt<T>(_mm256_and_si256(castToInt(cmp), castToInt(a)));
        }

        /// @brief Returns the negative part min(a_i,0) for each coordinate of a
        /// @return
        template <DataType T>
        static inline Vector<T> neg(const Vector<T> &a)
        {
            auto cmp = cmp_ge<T>(Simd::zero<T>(), a);
            return castFromInt<T>(_mm256_and_si256(castToInt<T>(cmp), castToInt<T>(a)));
        }

        /// @brief Returns the absolute value abs(a_i) for each coordinate of a
        /// @return
        template <DataType T>
        static inline Vector<T> abs(const Vector<T> &a)
        {
            if constexpr (std::is_floating_point_v<T>)
                return castFromInt<T>(_mm256_xor_si256(castToInt<T>(broadcast_set<T>(-(T(0)))), castToInt<T>(a)));
            else
                return castFromInt<T>(_mm256_xor_si256(castToInt<T>(broadcast_set<T>(-(T(1))), castToInt<T>(a))));
        }

        /// @brief For each scalar a_i returns 1 if a_i>= 0 and 0 otherwise
        /// @return
        template <DataType T>
        static inline Vector<T> step(const Vector<T> &a)
        {
            return castFromInt<T>(_mm256_and_si256(castToInt<T>(broadcast_set<T>(1)), castToInt<T>(pos(a))));
        }

        /// @brief For each scalar a_i returns beta_i if a_i>= 0 and alpha_i otherwise
        /// @return
        template <DataType T>
        static inline Vector<T> step(const Vector<T> &a, const Vector<T> alpha, const Vector<T> beta)
        {
            auto cmp = castToInt<T>(cmp_ge<T>(a, Simd::zero<T>()));

            return castFromInt<T>(_mm256_or_si256(
                _mm256_andnot_si256(cmp, castToInt<T>(alpha)),
                _mm256_and_si256(cmp, castToInt<T>(beta))
                ));
        }

        template <DataType T>
            requires std::is_floating_point_v<T>
        inline Vector<T> sqrt(const Vector<T> &a)
        {
            return Internal<T>::sqrt(a);
        }

        template <DataType T>
        struct ClipBounds
        {
            const Vector<T> lowerBound;
            const Vector<T> upperBound;
            ClipBounds() = delete;

            ClipBounds(T lower, T upper) : lowerBound(broadcast_set(lower)), upperBound(broadcast_set(upper))
            {
                if (lower > upper)
                    throw new std::invalid_argument("Lower bound must be below upper bound.");
            }
        };

        // Clip the values of a to be between least and most
        template <DataType T>
        inline Vector<T> clip(const ClipBounds<T> &bounds, const Vector<T> &a)
        {
            return min<T>(bounds.upperBound, max<T>(bounds.lowerBound, a));
        }

        template <>
        struct Internal<float>
        {
            typedef __m256 Type;
            static constexpr bool supported = true;
            using T = float;

            static inline Type zero() { return _mm256_setzero_ps(); }
            static inline Type broadcast_set(const T value) { return _mm256_set1_ps(value); }
            static constexpr auto set = _mm256_set_ps;
            static constexpr auto setr = _mm256_setr_ps;
            static inline Type load(const T *pData) { return _mm256_load_ps(pData); }
            static inline Type maskedLoad(const T *pData, const __m256i &mask) { return _mm256_maskload_ps(pData, mask); }
            static inline void store(T *pData, const Type &a) { return _mm256_store_ps(pData, a); }
            static inline void maskedStore(T *pData, const __m256i &mask, const Type &a) { _mm256_maskstore_ps(pData, mask, a); }

            static inline Type add(const Type &a, const Type &b)
            {
                return Type(_mm256_add_ps(a, b));
            }
            static inline Type multiply(const Type &a, const Type &b)
            {
                return Type(_mm256_mul_ps(a, b));
            }
            static inline Type subtract(const Type &a, const Type &b)
            {
                return Type(_mm256_sub_ps(a, b));
            }
            static inline Type divide(const Type &a, const Type &b)
            {
                return Type(_mm256_div_ps(a, b));
            }
            static inline Type fusedMultiplyAdd(const Type &a, const Type &b, const Type &c)
            {
                return _mm256_fmadd_ps(a, b, c);
            }

            static inline Type max(const Type &a, const Type &b)
            {
                return _mm256_max_ps(a, b);
            }
            static inline Type min(const Type &a, const Type &b)
            {
                return _mm256_min_ps(a, b);
            }

            static inline Type cmp_ge(const Type &a, const Type &b)
            {
                return _mm256_cmp_ps(a, b, _CMP_GE_OQ);
            }

            static inline __m256i castToInt(const Type &a)
            {
                return _mm256_castps_si256(a);
            }
            static inline Type castFromInt(const __m256i &a)
            {
                return _mm256_castsi256_ps(a);
            }

            static inline Type sqrt(const Type &a)
            {
                return _mm256_sqrt_ps(a);
            }
        };

    }
}

#endif