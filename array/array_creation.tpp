#ifndef ARRAY_CREATION_H
#define ARRAY_CREATION_H

#include "array.hpp"

namespace ArrayLibrary
{

    template <DataType T>
    Array<T> Array<T>::fromFlatLines(const std::vector<Data<T>> &lines)
    {
        if (lines.size() == 0)
            return Array<T>(Data<T>(0), {0});

        auto length = lines[0].size();

        for (long l = 0; l < lines.size(); l++)
        {
            if (lines[l].size() != length)
                throw std::invalid_argument("All lines must have the same size.");
        }

        auto result = Array<T>(Data<T>(lines.size() * length), {lines.size(), length});

        T *pResultData = result.getDataPointer();
        for (long l = 0; l < lines.size(); l++)
        {
            std::copy(lines[l].mRaw, lines[l].mRaw + length, pResultData);
            pResultData += length;
        }

        return result;
    }

    template <DataType T>
    Array<T> Array<T>::fromLines(const std::vector<Array<T>> &lines)
    {
        if (lines.size() == 0)
            return Array<T>(Data<T>(0), Coordinates(0));

        auto shape = lines[0].mShape;
        for (long l = 0; l < lines.size(); l++)
        {
            if (lines[l].mShape != shape)
                throw std::invalid_argument("All lines must have the same shape.");
        }

        auto flatLineLength = lines[0].mFlatLength;

        auto result = Array<T>(Data<T>(lines.size() * flatLineLength), Coordinates(lines.size(), shape));
        Coordinates c(shape.size(), 0);

        T *pResultData = result.getDataPointer();

        for (long l = 0; l < lines.size(); l++)
        {
            T *pLineData = lines[l].getDataPointer();
            c = 0;
            for (long k = 0; k < flatLineLength; k++)
            {
                *pResultData = *pLineData;

                for (long i = shape.size() - 1; i >= 0; i--)
                {
                    c[i]++;

                    if (shape[i] == 1 || c[i] % shape[i] == 0)
                        pLineData -= lines[l].mStrides[i] * (shape[i] - 1);
                    else
                        pLineData += lines[l].mStrides[i];

                    if (c[i] % shape[i] != 0)
                        break;
                }

                pResultData++;
            }
        }

        return result;
    }

    template <DataType T>
    Array<T> Array<T>::range(T from, T to, T step)
    {
        if (step == 0)
            step = 1;

        if ((to - from) / step < 0)
            step = -step;

        long count = std::ceil((to - from) / step);
        auto data = Data<T>(count);
        long i = 0;
        for (T a = from; a < to; a += step)
            data[i++] = a;

        return Array<T>(data);
    }

    template <DataType T>
    Array<T> Array<T>::range(T from, T to) { return Array<T>::range(from, to, 1); }

    template <DataType T>
    Array<T> Array<T>::range(T to) { return Array<T>::range(0, to, 1); }

    template <DataType T>
    Array<T> Array<T>::constant(const Coordinates &shape, const T constant)
    {
        long flatLength = 1;
        for (long i = 0; i < shape.size(); i++)
        {
            if (shape[i] < 0)
                throw std::invalid_argument("Entries of absolute shape vector cannot be negative.");
            flatLength *= shape[i];
        }
        auto data = Data<T>(flatLength);
        data = constant;
        Array<T> x(data, shape);

        return x;
    }
}

#endif
