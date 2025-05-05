#ifndef STACK_BUFFER_H
#define STACK_BUFFER_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iterator>

#define assertm(exp, msg) assert((void(msg), exp))

template <typename T, long BUFFER_SIZE>
class StackBuffer
{
private:
    T mBuffer[BUFFER_SIZE];
    long mSize;

public:
    long size() const { return mSize; }

    StackBuffer()
    {
        mSize = 0;
    }

    explicit StackBuffer(long size)
    {
        if (size > BUFFER_SIZE)
            throw std::invalid_argument("Size cannot be bigger than buffer size.");
        mSize = size;
    }

    StackBuffer(long size, T init)
    {
        if (size > BUFFER_SIZE)
            throw std::invalid_argument("Size cannot be bigger than buffer size.");
        mSize = size;
        for (long i = 0; i < mSize; i++)
            mBuffer[i] = init;
    }

    explicit StackBuffer(const std::vector<T> &vector)
    {
        mSize = vector.size();
        std::copy(vector.data(), vector.data() + std::min(BUFFER_SIZE, vector.size()), mBuffer);
    }

    StackBuffer(const std::initializer_list<T> &values)
    {
        mSize = 0;
        for (const T &value : values)
        {
            if (mSize >= BUFFER_SIZE)
                break;
            mBuffer[mSize++] = value;
        }
    }

    std::vector<T> toVector() const
    {
        return std::vector<T>(mBuffer, mBuffer + mSize);
    }

    StackBuffer(const StackBuffer &other)
    {
        mSize = other.mSize;
        std::copy(other.mBuffer, other.mBuffer + BUFFER_SIZE, mBuffer);
    }

    StackBuffer(StackBuffer &&other)
    {
        mSize = other.mSize;
        std::move(other.mBuffer, other.mBuffer + BUFFER_SIZE, mBuffer);
    }

    StackBuffer &operator=(const T value)
    {
        for (long i = 0; i < mSize; i++)
            mBuffer[i] = value;

        return *this;
    }

    StackBuffer &operator=(const StackBuffer &other)
    {
        if (this == &other)
            return *this;

        mSize = other.mSize;
        std::copy(other.mBuffer, other.mBuffer + other.mSize, mBuffer);

        return *this;
    }

    StackBuffer &operator=(const StackBuffer &&other)
    {
        if (this == &other)
            return *this;

        mSize = other.mSize;
        std::move(other.mBuffer, other.mBuffer + other.mSize, mBuffer);

        return *this;
    }

    StackBuffer operator+(const StackBuffer<T, BUFFER_SIZE> &other) const
    {
        if (mSize + other.mSize > BUFFER_SIZE)
            throw std::out_of_range("The new size would be large than BUFFER_SIZE.");

        auto result = StackBuffer(mSize + other.mSize);

        std::copy(mBuffer, mBuffer + mSize, result.mBuffer);
        std::copy(other.mBuffer, other.mBuffer + other.mSize, result.mBuffer + mSize);

        return result;
    }

    StackBuffer operator+(const T append) const
    {
        if (mSize + 1 > BUFFER_SIZE)
            throw std::out_of_range("The new size would be large than BUFFER_SIZE.");

        auto result = StackBuffer(mSize + 1);

        std::copy(mBuffer, mBuffer + mSize, result.mBuffer);
        result[mSize] = append;

        return result;
    }

    bool operator==(const StackBuffer &other) const
    {
        if (mSize != other.mSize)
            return false;

        for (long i = 0; i < mSize; i++)
            if (mBuffer[i] != other.mBuffer[i])
                return false;

        return true;
    }

    bool operator!=(const StackBuffer &other) const
    {
        return !(*this == other);
    }

    /// @brief Lists the indices of the elements that are different between two buffers.
    /// @details The result is a buffer of indices, where the first index is the first element that is different. If the two buffers do not have the same size, the two buffers are aligned on the right and all indices up to max(a.size(), b.size())-min(a.size(), b.size()) are included.
    static StackBuffer<T, BUFFER_SIZE> findDifferences(const StackBuffer<T, BUFFER_SIZE> &a, const StackBuffer<T, BUFFER_SIZE> &b)
    {
        StackBuffer<T, BUFFER_SIZE> result;
        auto max = std::max(a.mSize, b.mSize);
        auto aShift = max - a.mSize;
        auto bShift = max - b.mSize;

        for (long i = 0; i < max; i++)
            if (i < aShift || i < bShift)
                result[result.mSize++] = i;
            else if (a.mBuffer[i - aShift] != b.mBuffer[i - bShift])
                result[result.mSize++] = i;

        return result;
    }

    StackBuffer<T, BUFFER_SIZE> interval(long from, long upto) const
    {
        if (from < 0 || from >= mSize)
            throw std::out_of_range("The from index is out of range.");
        if (upto < 0 || upto > mSize)
            throw std::out_of_range("The upto index is out of range.");
        if (from > upto)
            throw std::invalid_argument("The from index cannot be larger than the upto index.");

        auto result = StackBuffer(upto - from);
        std::copy(mBuffer + from, mBuffer + upto, result.mBuffer);

        return result;
    }

    /// @brief Shifts the buffer to the right by the specified amount, filling the new elements with the specified padding value.
    /// @details If the shift is negative, the buffer is shifted to the left. The new size of the buffer is mSize + shift. If the new size is larger than BUFFER_SIZE, an exception is thrown. If the new size is negative, an exception is thrown.
    StackBuffer<T, BUFFER_SIZE> shiftRight(T paddingValue, long shift = 1) const
    {
        if (shift < 0)
        {
            if (-shift > mSize)
                throw std::out_of_range("The new size would be negative.");

            auto result = StackBuffer(mSize + shift);
            std::copy(mBuffer - shift, mBuffer + mSize, result.mBuffer);
        }
        if (mSize + shift > BUFFER_SIZE)
            throw std::out_of_range("The new size would be large than BUFFER_SIZE.");

        auto result = StackBuffer(mSize + shift);

        for (long i = 0; i < shift; i++)
            result.mBuffer[i] = paddingValue;
        if (shift >= 0)
            std::copy(mBuffer, mBuffer + mSize, result.mBuffer + shift);
        else
            std::copy(mBuffer - shift, mBuffer + mSize, result.mBuffer);

        return result;
    }

    void pushBack(const T &value)
    {
        if (mSize >= BUFFER_SIZE)
            throw std::out_of_range("The buffer is full.");
        mBuffer[mSize++] = value;
    }

    inline T &operator[](long i) noexcept
    {
#ifdef DEBUG_MODE
        assertm(i >= 0 && i < mSize, "The index is out of range.");
#endif

        return mBuffer[i];
    }

    inline const T &operator[](long i) const noexcept
    {
#ifdef DEBUG_MODE
        assertm(i >= 0 && i < mSize, "The index is out of range.");
#endif

        return mBuffer[i];
    }

    T get(long i) const noexcept
    {
        i = i % mSize;
        if (i < 0)
            i += mSize;

        return mBuffer[i];
    }

    const T *begin() const
    {
        return mBuffer;
    }

    const T *end() const
    {
        return mBuffer + mSize;
    }
};

template <typename T, long BUFFER_SIZE>
std::ostream &operator<<(std::ostream &s, const StackBuffer<T, BUFFER_SIZE> &buffer)
{
    s.put('[');
    for (char comma[]{'\0', ' ', '\0'}; const T &e : buffer)
        s << comma << e, comma[0] = ',';
    return s << "]";
}

#endif