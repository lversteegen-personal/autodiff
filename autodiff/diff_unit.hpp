#ifndef DIFF_UNIT_H
#define DIFF_UNIT_H

#include "difftape.hpp"

template <DataType T>
class Reshape;
template <DataType T>
class ReduceSum;
template <DataType T>
class ReduceMean;
template <DataType T>
class Softmax;
template <DataType T, typename P, T (*f)(const T, const P &), T (*df)(const T, const P &)>
class ParamPointwise;

template <DataType T>
class Unit
{

protected:
    DiffTape<T> &mDiffTape;
    bool mIsCaclulated = false;
    Coordinates mWildcardShape;
    Array<T> mArray = Array<T>::constant({}, 0);

    static BroadcastType wildcardBroadcastRelationship(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long minDim = std::min(dim1, dim2);
        long shift1 = dim1 - minDim, shift2 = dim2 - minDim;
        BroadcastType result = BroadcastType::MATCH;
        bool hasWildcard = false;

        for (long i = 0; i < shift1; i++)
            if (shape1[i] != 1)
            {
                result &= BroadcastType::LEFTMIX;
                if (shape1[i] == -1)
                {
                    if (hasWildcard)
                        return BroadcastType::NONE;

                    hasWildcard = true;
                }
            }
        for (long i = 0; i < shift2; i++)
            if (shape2[i] != 1)
            {
                result &= BroadcastType::RIGHTMIX;
                if (shape2[i] == -1)
                {
                    if (hasWildcard)
                        return BroadcastType::NONE;

                    hasWildcard = true;
                }
            }

        long s1, s2;

        for (long i = 0; i < minDim; i++)
        {
            s1 = shape1[i + shift1];
            s2 = shape2[i + shift2];

            if (s1 == -1 || s2 == -1)
            {
                if (hasWildcard || (s1 != -1 && s1 != 1) || (s2 != -1 && s2 != 1))
                    return BroadcastType::NONE;

                hasWildcard = true;
            }
            else if (s1 != s2)
            {
                if (s1 == 1)
                    result &= BroadcastType::RIGHTMIX;
                else if (s2 == 1)
                    result &= BroadcastType::LEFTMIX;
                else
                    return BroadcastType::NONE;
            }
        }

        return result;
    }

    static Coordinates wildcardBroadcastShape(const Coordinates &shape1, const Coordinates &shape2)
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

public:
    Array<T> mGradient = Array<T>::constant({}, 0);
    DiffTape<T> &getDiffTape() { return mDiffTape; }
    const Array<T> &refArray() const { return mArray; }
    const Array<T> &refGradient() const { return mGradient; }
    const Coordinates &refArrayShape() const { return mArray.refShape(); }
    const Coordinates &refWildcardShape() const { return mWildcardShape; }
    const long getDim() const { return mWildcardShape.size(); }

    Unit(DiffTape<T> &diffTape, const Coordinates &shape) : mDiffTape(diffTape), mWildcardShape(shape)
    {
        bool hasWildcard = false;
        for (long i = 0; i < shape.size(); i++)
            if (shape[i] < 0)
            {
                if (hasWildcard)
                    throw std::invalid_argument("At most one wildcard dimension is allowed");
                else
                    hasWildcard = true;
            }

        mDiffTape.addVariable(this);
    };

    Unit(DiffTape<T> &diffTape, const Array<T> &array) : mDiffTape(diffTape), mArray(array), mWildcardShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

    Unit(DiffTape<T> &diffTape, Array<T> &&array) : mDiffTape(diffTape), mArray(std::forward<Array<T>>(array)), mWildcardShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

public:
    Unit() = delete;
    Unit(const Unit<T> &other) = delete;
    Unit(Unit<T> &&other) = delete;

    virtual void pullGradient() = 0;

    Reshape<T> &reshape(const Coordinates &newShape);

    void resetGradient()
    {
        if (mArray.refShape() == mGradient.refShape())
            mGradient = 0; // Array<T>::constant(mArray.refShape(), 0);
        else
            mGradient = Array<T>::constant(mArray.refShape(), 0);
    }

    virtual void calculate()
    {
        mIsCaclulated = true;
    }

    void initDiff()
    {
        mGradient = 1;
    }

    std::string to_string() const
    {
        return mArray.to_string();
    }

    ReduceSum<T> &reduceSum(bool keepDims = false)
    {
        auto axes = Coordinates(getDim());
        for (long i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new ReduceSum<T>(*this, axes, keepDims));
    }

    ReduceSum<T> &reduceSum(Coordinates axes, bool keepDims = false)
    {
        return *(new ReduceSum<T>(*this, axes, keepDims));
    }

    ReduceMean<T> &reduceMean(bool keepDims = false)
    {
        auto axes = Coordinates(getDim());
        for (long i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new ReduceMean<T>(*this, axes, keepDims));
    }

    ReduceMean<T> &reduceMean(Coordinates axes, bool keepDims = false)
    {
        return *(new ReduceMean<T>(*this, axes, keepDims));
    }

    Softmax<T> &softmax()
    {
        auto axes = Coordinates(getDim());
        for (long i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new Softmax<T>(*this, axes));
    }

    Softmax<T> &softmax(const Coordinates &axes)
    {
        return *(new Softmax<T>(*this, axes));
    }

private:
    inline static T leakyReLUPtws(const T value, const T &alpha)
    {
        return value > 0 ? value : value * alpha;
    }

    inline static T dleakyReLUPtws(const T value, const T &alpha)
    {
        return value > 0 ? 1 : alpha;
    }

public:
    Unit<T> &leakyReLu(T alpha)
    {
        return *(new ParamPointwise<T, T, leakyReLUPtws, dleakyReLUPtws>(*this, alpha));
    }
};

template <DataType T>
std::ostream &operator<<(std::ostream &s, const Unit<T> &x)
{
    return s << x.to_string();
}

#endif