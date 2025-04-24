#ifndef SHAPE_H
#define SHAPE_H

#include "stack_buffer.hpp"
#include <initializer_list>

#define MAX_DIM 8ul

namespace ArrayLibrary
{
    typedef StackBuffer<long, MAX_DIM> Coordinates;

    struct ReduceInformation
    {
        const Coordinates keepDimsShape;
        const Coordinates keepDimsStrides;
        const Coordinates reducedShape;
        const long flatLength;

        ReduceInformation() = delete;

        ReduceInformation(const Coordinates &keepDimsShape, const Coordinates &keepDimsStrides, const Coordinates &reducedShape, long flatLength) : keepDimsShape(keepDimsShape), keepDimsStrides(keepDimsStrides), reducedShape(reducedShape), flatLength(flatLength)
        {
        }
    };

    /// @brief Computes information about the resulting shape for a reduction along axes applied to an Array<T> object
    /// @details If the initial shape has a wildcard dimension, the flatLength in the returned ReduceInformation object will be negative.
    ReduceInformation reduceShape(const Coordinates &shape, const Coordinates reduceAxes, bool keepDims)
    {
        bool reduce[MAX_DIM] = {false};
        long dim = shape.size();

        long j = 0;
        for (int i = 0; i < reduceAxes.size(); i++)
        {
            j = reduceAxes[i] % dim;
            j = j < 0 ? dim + j : j;
            reduce[j] = true;
        }

        Coordinates keepDimShape(shape.size());
        Coordinates keepDimStrides(shape.size());
        Coordinates reducedShape(keepDims ? dim : dim - reduceAxes.size());

        long flatLength = 1;
        j = 0;

        for (int i = 0; i < dim; i++)
        {
            if (reduce[i])
            {
                if (keepDims)
                    reducedShape[j++] = shape[i];
                keepDimShape[i] = 1;
                keepDimStrides[i] = 0;
            }
            else
            {
                reducedShape[j++] = shape[i];
                keepDimShape[i] = shape[i];
                keepDimStrides[i] = flatLength;
                flatLength *= shape[i];
            }
        }

        return ReduceInformation(std::move(keepDimShape), std::move(keepDimStrides), std::move(reducedShape), flatLength);
    }

    Coordinates findOuterShape(const std::initializer_list<Coordinates> &list)
    {
        if (list.size() == 0)
            return Coordinates(0);

        const Coordinates *pShape = list.begin();
        Coordinates result(*(pShape++));

        for (; pShape != list.end(); pShape++)
        {
            auto &shape = *pShape;
            if (shape.size() != result.size())
                throw std::invalid_argument("The shapes must have the same length.");
            for (size_t i = 0; i < result.size(); i++)
            {
                if (shape[i] != 1)
                {
                    if (result[i] == 1)
                        result[i] == shape[i];
                    else if (result[i] != shape[i])
                        throw std::invalid_argument("All shapes must agree in all dimensions in which they are non-trivial.");
                }
            }
        }

        return result;
    }

    Coordinates broadcastShape(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long dim = std::max(dim1, dim2);
        long shift1 = dim - dim1, shift2 = dim - dim2;
        Coordinates result(dim);

        for (long i = 0; i < dim; i++)
        {
            if (i < shift1)
                result[i] = shape2[i];
            else if (i < shift2)
                result[i] = shape1[i];
            else if (shape1[i - shift1] == -1 || shape2[i - shift2] == -1)
                throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");
            else
                result[i] = std::max(shape1[i - shift1], shape2[i - shift2]);
        }

        return result;
    }
}

#endif