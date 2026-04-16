#ifndef SHAPE_H
#define SHAPE_H

#include "stack_buffer.hpp"
#include "constants.hpp"
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

    template <std::same_as<Coordinates>... Shapes>
    Coordinates findOuterShape(const Shapes &...shapes)
    {
        if constexpr (sizeof...(Shapes) == 0)
            return Coordinates(0);

        long dim = max(shapes.size()...);

        Coordinates result(dim);

        for (long i = 0; i < dim; i++)
        {
            long axisLength = max((i >= dim - shapes.size() ? shapes[i - dim + shapes.size()] : 1)...);
            result[i] = axisLength;
            if (!(... && (i < dim - shapes.size() || shapes[i - dim + shapes.size()] == 1 || shapes[i - dim + shapes.size()] == axisLength)))
                throw std::invalid_argument("All shapes must agree in all dimensions in which they are non-trivial.");
        }

        return result;
    }

    template <std::same_as<Coordinates>... Shapes>
    bool isExtensionBroadcastable(const Shapes &...shapes)
    {
        long dim = max(shapes.size()...);
        for (long i = 0; i < dim; i++)
        {
            long axisLength = max((i >= dim - shapes.size() ? shapes[i - dim + shapes.size()] : 1)...);
            bool axisCheck = (... && (i < dim - shapes.size() || shapes[i - dim + shapes.size()] == 1 || shapes[i - dim + shapes.size()] == axisLength));

            if (!axisCheck)
                return false;
        }

        return true;
    }

    template <std::same_as<Coordinates>... Shapes>
    bool isBroadcastable(const Shapes &...shapes)
    {
        long dim = max(shapes.size()...);
        if ((... || (shapes.size() != dim)))
            return false;

        for (long i = 0; i < dim; i++)
        {
            long axisLength = max(shapes[i]...);
            bool axisCheck = (... && (shapes[i] == 1 || shapes[i] == axisLength));

            if (!axisCheck)
                return false;
        }

        return true;
    }

    /// @brief Determines if the first argument can be made into an immediate subshape of the second argument by adding trivial axes on the left of either argument
    /// @param subshape
    /// @param supershape
    /// @return
    bool isExtensionSubshape(const Coordinates &subshape, const Coordinates &supershape)
    {
        if (subshape.size() > supershape.size())
        {
            long diff = subshape.size() - supershape.size();
            
            for (long i = 0; i < diff; i++)
                if (subshape[i] != 1)
                    return false;

            for (long i = 0; i < supershape.size(); i++)
                if (subshape[i] != 1 && subshape[i] != supershape[i])
                    return false;

            return true;
        }
        else
        {
            for (long i = 0; i < subshape.size(); i++)
                if (subshape[i] != 1 && subshape[i] != supershape[i + supershape.size() - subshape.size()])
                    return false;

            return true;
        }
    }

    /// @brief Determines if the first argument can be made into an immediate subshape of the second argument by adding trivial axes on the left of the first argument
    /// @param subshape
    /// @param supershape
    /// @return
    bool isHalfExtensionSubshape(const Coordinates &subshape, const Coordinates &supershape)
    {
        if (subshape.size() > supershape.size())
            return false;

        for (long i = 0; i < subshape.size(); i++)
            if (subshape[i] != 1 && subshape[i] != supershape[i + supershape.size() - subshape.size()])
                return false;

        return true;
    }

    /// @brief Determines if the first argument is an immediate (i.e., without adding trivial axes) subshape of the second argument
    /// @param subshape
    /// @param supershape
    /// @return
    bool isSubshape(const Coordinates &subshape, const Coordinates &supershape)
    {
        if (subshape.size() != supershape.size())
            return false;

        for (long i = 0; i < subshape.size(); i++)
            if (subshape[i] != 1 && subshape[i] != supershape[i])
                return false;

        return true;
    }

    class ShapeIterator
    {
        const Coordinates mShape;
        Coordinates mPosition;
        bool mFinished;

    public:
        ShapeIterator(const Coordinates &shape) : mShape(shape), mPosition(shape.size(), 0), mFinished(mShape == mPosition) {}

        bool isFinished() const { return mFinished; }

        const Coordinates &refPosition() const { return mPosition; }

        const ShapeIterator &operator++()
        {
            for (long i = mShape.size() - 1; i >= 0; i--)
            {
                if (mPosition[i] == mShape[i] - 1)
                    mPosition[i] = 0;
                else
                {
                    mPosition[i]++;
                    return *this;
                }
            }

            mFinished = true;
            return *this;
        }
    };
}

#endif