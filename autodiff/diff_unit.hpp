#ifndef DIFF_UNIT_H
#define DIFF_UNIT_H

#include <vector>

#include "difftape.hpp"

namespace AutoDiff
{
    template <DataType T>
    class Reshape;
    
    long findWildcardDimension(const Coordinates &shape)
    {
        long wildcardDimension = -1;
        for (long i = 0; i < shape.size(); i++)
            if (shape[i] == -1)
            {
                if (wildcardDimension != -1)
                    throw std::invalid_argument("Only one wildcard dimension allowed.");

                wildcardDimension = i;
            }
        return wildcardDimension;
    }

    Coordinates wildcardBroadcastShape(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long dim = std::max(dim1, dim2);
        long shift1 = dim - dim1, shift2 = dim - dim2;
        Coordinates result(dim);
        bool hasWildcard = false;

        for (long i = 0; i < dim; i++)
        {
            if (i < shift1)
            {
                result[i] = shape2[i];
                if (shape2[i] == -1)
                {
                    if (hasWildcard)
                        throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");

                    hasWildcard = true;
                }
            }
            else if (i < shift2)
            {
                result[i] = shape1[i];
                if (shape1[i] == -1)
                {
                    if (hasWildcard)
                        throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");

                    hasWildcard = true;
                }
            }
            else
            {
                auto s1 = shape1[i - shift1];
                auto s2 = shape2[i - shift2];
                if (s1 == -1 || s2 == -1)
                {
                    if (hasWildcard)
                        throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");

                    if ((s1 != -1 && s1 != 1) || (s2 != -1 && s2 != 1))
                        throw std::invalid_argument("If dimension i is a wildcard for one shape, it must either be a wildcard for the other shape as well or the length of dimension i for the other shape must be 1.");

                    hasWildcard = true;
                    result[i] = -1;
                }
                else if (s1 == s2 || s1 == 1)
                    result[i] = s2;
                else if (s2 == 1)
                    result[i] = s1;
                else
                    throw std::invalid_argument("Arrays cannot be broadcasted to match.");
            }
        }

        return result;
    }

    bool wildcardRemovalCheck(const Coordinates &wildcardShape, const Coordinates &concreteShape)
    {
        if (wildcardShape.size() != concreteShape.size())
            return false;

        for (long i = 0; i < wildcardShape.size(); i++)
            if (wildcardShape[i] != -1 && wildcardShape[i] != concreteShape[i])
                return false;

        return true;
    }

    template <DataType T>
    class Unit
    {

    protected:
        DiffTape<T> &mDiffTape;
        Coordinates mWildcardShape;
        Array<T> mArray = Array<T>::constant({}, 0);

    public:
        Array<T> mGradient = Array<T>::constant({}, 0);
        DiffTape<T> &getDiffTape() { return mDiffTape; }
        const Array<T> &refArray() const { return mArray; }
        const Array<T> &refGradient() const { return mGradient; }
        const Coordinates &refArrayShape() const { return mArray.refShape(); }
        const Coordinates &refWildcardShape() const { return mWildcardShape; }
        const long getDim() const { return mWildcardShape.size(); }
        const long wildcardDim;

    protected:
        Unit(DiffTape<T> &diffTape, const Coordinates &shape) : mDiffTape(diffTape), mWildcardShape(shape), wildcardDim(findWildcardDimension(shape))
        {
            mDiffTape.addVariable(this);
        }

        Unit(DiffTape<T> &diffTape, const Array<T> &array) : mDiffTape(diffTape), mArray(array), mWildcardShape(array.refShape()), wildcardDim(-1)
        {
            mDiffTape.addVariable(this);
        }

    public:
        Unit() = delete;
        Unit(const Unit<T> &other) = delete;
        Unit(Unit<T> &&other) = delete;

        virtual std::vector<Unit<T> *> getDependencies() const = 0;
        virtual void pullGradient() const = 0;

        Reshape<T> &reshape(const Coordinates &newShape);

        inline bool wildcardMatch(const Coordinates &shape)
        {
            return wildcardRemovalCheck(mWildcardShape, shape);
        }

        void resetGradient()
        {
            if (mArray.refShape() == mGradient.refShape())
                mGradient = 0; // Array<T>::constant(mArray.refShape(), 0);
            else
                mGradient = Array<T>::constant(mArray.refShape(), 0);
        }

        virtual void calculate() {}

        void initDiff()
        {
            mGradient = 1;
        }

        std::string to_string() const
        {
            return mArray.to_string();
        }

    };

    template <DataType T>
    std::ostream &operator<<(std::ostream &s, const Unit<T> &x)
    {
        return s << x.to_string();
    }
}

#endif