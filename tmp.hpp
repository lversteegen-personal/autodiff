namespace Rubbish{

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>

template <typename T>
class Array
{
private:
    std::vector<T> mData;

public:
    size_t getFlatLength() const;

private:
    std::vector<size_t> mShape;

private:
    std::vector<size_t> mIndexMultipliers;

private:
    size_t mDim;

public:
    size_t getDim() const;

private:
    void calculateStrides();

public:
    std::vector<size_t> getShape() const;

public:
    void reshape(std::initializer_list<size_t> shape);

public:
    void reshape(std::vector<size_t> shape);

public:
    Array<T>(std::vector<T> &data);

public:
    Array<T>(std::initializer_list<T> values);

public:
    static Array<T> range(T from, T to, T step);

public:
    static Array<T> range(T from, T to);

public:
    static Array<T> range(T to);
public:
    static Array<T> from(std::vector<T> data);

public:
    static Array<T> constant(const std::vector<size_t> &shape, const T constant);

private:
    static bool shapeMatch(const Array<T> &x, const Array<T> &y);

public:
    Array<T> operator+(const Array<T> &other) const;

    Array<T> operator*(const Array<T> &other) const;

    Array<T> operator-(const Array<T> &other) const;

    Array<T> operator/(const Array<T> &other) const;

    Array<T> matmul(const Array<T> &other, int axisThis=-1, int axisOther=-2) const;

    Array<T> pow(const unsigned int k);

    Array<T> pow(const Array<T> &y);

    T operator[](const std::vector<int> indices) const;

    T get(const std::vector<int> indices) const;

    T getFlat (size_t i) const;
};

template <typename T>
std::ostream &operator<<(std::ostream &s, const Array<T> &x);
}