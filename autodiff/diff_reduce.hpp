#ifndef DIFF_REDUCE_H
#define DIFF_REDUCE_H

#include "diff_unit.hpp"

template <DataType T>
class ReduceSum : public Unit<T>
{
private:
    Unit<T> &mSource;
    const Coordinates mAxes;
    Coordinates mKeepDimsShape;
    const bool mKeepDims;

public:
    ReduceSum(Unit<T> &source, const Coordinates &axes, bool keepDims = false) : mSource(source), mAxes(axes), mKeepDims(keepDims), Unit<T>(source.getDiffTape(), reduceShape(source.refWildcardShape(), axes, keepDims).reducedShape),
                                                                                 mKeepDimsShape(mKeepDims ? Unit<T>::mWildcardShape : reduceShape(source.refWildcardShape(), axes, true).reducedShape) {}

    void pullGradient() override
    {
        mSource.mGradient += this->mGradient.reshape(mKeepDimsShape);
    }

    void calculate() override
    {
        this->mArray = mSource.refArray().reduceSum(mAxes, mKeepDims);
        Unit<T>::calculate();
    };
};

template <DataType T>
class Softmax : public Unit<T>
{
private:
    Unit<T> &mSource;
    const Coordinates mAxes;

public:
    Softmax(Unit<T> &source, const Coordinates &axes) : mSource(source), mAxes(axes), Unit<T>(source.getDiffTape(), source.refWildcardShape()) {}

    void pullGradient() override
    {
        Array<T> centered = mSource.refArray() - mSource.refArray().reduceMax(mAxes, true);
        Array<T> expd = centered.exp();
        Array<T> norm = expd.reduceSum(mAxes, true);
        auto &g = this->mGradient;
        Array<T> prod = (expd * g);

        mSource.mGradient += (prod - expd * prod.reduceSum(mAxes, true) / norm) / norm;
    }

    void calculate() override
    {
        Array<T> centered = mSource.refArray() - mSource.refArray().reduceMax(mAxes, true);
        Array<T> expd = centered.exp();
        this->mArray = expd / expd.reduceSum(mAxes, true);
        Unit<T>::calculate();
    };
};

template <DataType T>
class ReduceMean : public Unit<T>
{
private:
    Unit<T> &mSource;
    const Coordinates mAxes;
    Coordinates mKeepDimsShape;
    const bool mKeepDims;
    long mReducedWildcardDim = -1;
    long mBaseDivisor = 1;

public:
    ReduceMean(Unit<T> &source, const Coordinates &axes, bool keepDims = false) : mSource(source), mAxes(axes), mKeepDims(keepDims), Unit<T>(source.getDiffTape(), reduceShape(source.refWildcardShape(), axes, keepDims).reducedShape),
                                                                                        mKeepDimsShape(mKeepDims ? Unit<T>::mWildcardShape : reduceShape(source.refWildcardShape(), axes, true).reducedShape), mBaseDivisor(1)
    {
        auto &sourceShape = source.refWildcardShape();
        for (auto d : axes)
        {
            if (sourceShape[d] < 0)
                mReducedWildcardDim = d;
            else
                mBaseDivisor *= sourceShape[d];
        }
    }

    void pullGradient() override
    {
        long divisor = mBaseDivisor;
        if (mReducedWildcardDim != -1)
            divisor *= mSource.refArray().refShape()[mReducedWildcardDim];

        mSource.mGradient += (this->mGradient.reshape(mKeepDimsShape)) / divisor;
    }

    void calculate() override
    {
        long divisor = mBaseDivisor;
        if (mReducedWildcardDim != -1)
            divisor *= mSource.refArray().refShape()[mReducedWildcardDim];

        this->mArray = mSource.refArray().reduceSum(mAxes, mKeepDims)/divisor;
        Unit<T>::calculate();
    };
};

#endif