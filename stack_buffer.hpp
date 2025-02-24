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

    StackBuffer(size_t size)
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

    StackBuffer(const std::vector<T> &vector)
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

    StackBuffer &operator=(const StackBuffer &other)
    {
        if (this == &other)
            return *this;

        mSize = other.mSize;
        std::copy(other.mBuffer, other.mBuffer + BUFFER_SIZE, mBuffer);

        return *this;
    }

    StackBuffer &operator=(const StackBuffer &&other)
    {
        if (this == &other)
            return *this;

        mSize = other.mSize;
        std::move(other.mBuffer, other.mBuffer + BUFFER_SIZE, mBuffer);

        return *this;
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

    const T* begin() const {
        return mBuffer;
    }

    const T* end() const
    {
        return mBuffer+mSize;
    }

};

template <typename T, size_t BUFFER_SIZE>
std::ostream &operator<<(std::ostream &s, const StackBuffer<T,BUFFER_SIZE> &buffer)
{
    s.put('[');
    for (char comma[]{'\0', ' ', '\0'}; const T &e : buffer)
        s << comma << e, comma[0] = ',';
    return s << "]";
}

#endif