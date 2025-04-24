#ifndef ARRAY_LIBRARY_H
#define ARRAY_LIBRARY_H

#include "array.hpp"
#include "array_creation.tpp"
#include "binary_ptws.tpp"
#include "unary_ptws.tpp"
#include "matmul.tpp"
#include "random.hpp"

namespace ArrayLibrary
{

    template <DataType T>
    std::ostream &operator<<(std::ostream &s, const Array<T> &x)
    {
        return s << x.to_string();
    }

    template <DataType T>
    Array<T> operator+(const T &left, const Array<T> &right)
    {
        return Array<T>(left) + right;
    }

    template <DataType T>
    Array<T> operator+(const Array<T> &left, const T &right)
    {
        return left + Array<T>(right);
    }

    template <DataType T>
    Array<T> operator-(const T &left, const Array<T> &right)
    {
        return Array<T>(left) - right;
    }

    template <DataType T>
    Array<T> operator-(const Array<T> &left, const T &right)
    {
        return left - Array<T>(right);
    }

    template <DataType T>
    Array<T> operator*(const T &left, const Array<T> &right)
    {
        return Array<T>(left) * right;
    }

    template <DataType T>
    Array<T> operator*(const Array<T> &left, const T &right)
    {
        return left * Array<T>(right);
    }

    template <DataType T>
    Array<T> operator/(const T &left, const Array<T> &right)
    {
        return Array<T>(left) / right;
    }

    template <DataType T>
    Array<T> operator/(const Array<T> &left, const T &right)
    {
        return left / Array<T>(right);
    }

    template <DataType T>
        requires std::is_integral_v<T>
    Array<T> operator%(const T &left, const Array<T> &right)
    {
        return Array<T>(left) % right;
    }

    template <DataType T>
        requires std::is_integral_v<T>
    Array<T> operator%(const Array<T> &left, const T &right)
    {
        return left % Array<T>(right);
    }
}
#endif