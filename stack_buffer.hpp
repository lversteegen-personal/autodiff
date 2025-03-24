#ifndef STACK_BUFFER_H
#define STACK_BUFFER_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iterator>

#define assertm(exp, msg) assert((void(msg), exp))

template <typename T, size_t BUFFER_SIZE>
class StackBuffer
{
private:
    T mBuffer[BUFFER_SIZE];
    size_t mSize;

public:
    size_t size() const { return mSize; }

    StackBuffer()
    {
        mSize = 0;
    }

    explicit StackBuffer(size_t size)
    {
        if (size > BUFFER_SIZE)
            throw std::invalid_argument("Size cannot be bigger than buffer size.");
        mSize = size;
    }

    StackBuffer(size_t size, T init)
    {
        if (size > BUFFER_SIZE)
            throw std::invalid_argument("Size cannot be bigger than buffer size.");
        mSize = size;
        for (size_t i = 0; i < mSize; i++)
            mBuffer[i] = init;
    }

    /*StackBuffer(size_t size, const T *data)
    {
        if (size > BUFFER_SIZE)
            throw std::invalid_argument("Size cannot be bigger than buffer size.");
        mSize = size;
        std::copy(data, data + size, mBuffer);
    }*/

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
        for (size_t i = 0; i < mSize; i++)
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

    bool operator==(const StackBuffer &&other)
    {
        if (mSize != other.mSize)
            return false;

        for (size_t i = 0; i < mSize; i++)
            if (mBuffer[i] != other.mBuffer[i])
                return false;

        return true;
    }

    static StackBuffer<T, BUFFER_SIZE> findDifferences(const StackBuffer<T, BUFFER_SIZE> &a, const StackBuffer<T, BUFFER_SIZE> &b)
    {
        StackBuffer<T, BUFFER_SIZE> result;
        auto max = std::max(a.mSize, b.mSize);
        for (size_t i = 0; i < max; i++)
            if (i >= a.mSize || i >= b.mSize || a.mBuffer[i] != b.mBuffer[i])
                result[result.mSize++] = i;
        return result;
    }

    StackBuffer<T, BUFFER_SIZE> rightShift(T value, size_t reps = 1) const
    {
        if (mSize + reps > BUFFER_SIZE)
            throw std::out_of_range("The new size would be large than BUFFER_SIZE.");

        auto result = StackBuffer(mSize + reps);

        for (size_t i = 0; i < reps; i++)
            result.mBuffer[i] = value;
        std::copy(mBuffer, mBuffer + mSize, result.mBuffer + reps);

        return result;
    }

    void pushBack(const T &value)
    {
        if (mSize >= BUFFER_SIZE)
            throw std::out_of_range("The buffer is full.");
        mBuffer[mSize++] = value;
    }

    T &operator[](size_t i)
    {
        assertm(i < mSize, "The index is out of range.");
        return mBuffer[i];
    }

    const T &operator[](size_t i) const
    {
        assertm(i < mSize, "The index is out of range.");
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

template <typename T, size_t BUFFER_SIZE>
std::ostream &operator<<(std::ostream &s, const StackBuffer<T, BUFFER_SIZE> &buffer)
{
    s.put('[');
    for (char comma[]{'\0', ' ', '\0'}; const T &e : buffer)
        s << comma << e, comma[0] = ',';
    return s << "]";
}

#endif