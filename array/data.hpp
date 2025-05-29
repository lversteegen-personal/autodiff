#ifndef ARRAY_DATA_H
#define ARRAY_DATA_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "simd_vector.hpp"

namespace ArrayLibrary
{
    template <DataType T>
    class Array;

    template <DataType T>
    class Data
    {
    public:
        // ALIGNMENT is the smallest multiple of alignof(T) that is at least as large as SIMD_BYTES
        static constexpr size_t ALIGNMENT = SIMD_BYTES % alignof(T) == 0 ? std::max((size_t)SIMD_BYTES, alignof(T)) : SIMD_BYTES + alignof(T) - (SIMD_BYTES % alignof(T));

        template <DataType U>
        friend class Array;

    private:
        class Control
        {
            friend class Data<T>;

        private:
            T * mRaw;
            size_t mAccessCount;
            const size_t mSize;
            bool mRealeased;

            void release()
            {
                assertm(mAccessCount != 0, "Data has already been fully released!");
                mAccessCount--;

                if (mAccessCount == 0)
                {
                    _aligned_free(mRaw);
                    mRealeased = true;
                }
            }

        public:
            Control(size_t size) : mSize(size)
            {
                mRaw = static_cast<T *>(_aligned_malloc(size * sizeof(T), ALIGNMENT));
                mAccessCount = 1;
                mRealeased = false;
            }

            Control(const Control &other) = delete;
            Control &operator=(const Control &other) = delete;
            Control(Control &&other) = delete;
            Control &operator=(Control &&other) = delete;
            Control() = delete;

            ~Control()
            {
                assertm(mRealeased, "Data has not been fully released!");
            }
        };

        static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

        // Data<T> does not own the data, Control T does
    private:
        T *mRaw;
        Control *mControl;

        void release()
        {
            assertm(mControl != nullptr, "This Data pointer has already released its data!");
            mControl->release();
            if (mControl->mRealeased)
                delete mControl;
        }

    public:
        size_t size() const { return mControl->mSize; }

        /// @detail This constructor does not put initial values in the array, use with caution!
        Data(size_t size)
        {
            mControl = new Control(size);
            mRaw = mControl->mRaw;
        }

        Data(std::vector<T> &vector)
        {
            mControl = new Control(vector.size());
            mRaw = mControl->mRaw;
            std::copy(vector.begin(), vector.end(), mRaw);
        }

        Data(const std::initializer_list<T> &values)
        {
            mControl = new Control(values.size());
            mRaw = mControl->mRaw;
            size_t i = 0;
            for (const T &value : values)
                mRaw[i++] = value;
        }

        Data(const Data<T> &other) : mControl(other.mControl), mRaw(other.mRaw)
        {
            mControl->mAccessCount++;
        }

        Data<T> &operator=(const Data<T> &other)
        {
            if (this == &other)
                return *this;

            mControl->release();
            mRaw = other.mRaw;
            mControl = other.mControl;
            mControl->mAccessCount++;
            return *this;
        }

        Data<T> &operator=(const T value)
        {
            std::fill(mRaw, mRaw + size(), value);
            return *this;
        }

        Data<T> copy(long start = 0, long end = -1) const
        {
            if (end == -1)
                end = size();
            assertm(start >= 0 && end <= size() && start <= end, "Invalid copy parameters!");

            size_t size = end - start;
            Data<T> copyData(size);
            std::copy(mRaw + start, mRaw + end, copyData.mRaw);
            return copyData;
        }

        T &operator[](long i) const
        {
            assertm(i >= 0 && i < size(), "Index out of bounds!");
            return mRaw[i];
        }

        ~Data()
        {
            if (mControl != nullptr)
                release();
        }
    };
}
#endif