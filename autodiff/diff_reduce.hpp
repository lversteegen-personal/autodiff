#ifndef DIFF_REDUCE_H
#define DIFF_REDUCE_H

#include "diff_unit.hpp"

namespace AutoDiff
{
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

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

        void pullGradient() const override
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

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
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

            this->mArray = mSource.refArray().reduceSum(mAxes, mKeepDims) / divisor;
            Unit<T>::calculate();
        }
    };

    template <DataType T>
    ReduceSum<T> &reduceSum(Unit<T>& unit, bool keepDims = false)
    {
        auto axes = Coordinates(unit.getDim());
        for (long i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new ReduceSum<T>(unit, axes, keepDims));
    }

    template <DataType T>
    ReduceSum<T> &reduceSum(Unit<T>& unit, Coordinates axes, bool keepDims = false)
    {
        return *(new ReduceSum<T>(unit, axes, keepDims));
    }

    template <DataType T>
    ReduceMean<T> &reduceMean(Unit<T>& unit, bool keepDims = false)
    {
        auto axes = Coordinates(unit.getDim());
        for (long i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new ReduceMean<T>(unit, axes, keepDims));
    }

    template <DataType T>
    ReduceMean<T> &reduceMean(Unit<T>& unit, Coordinates axes, bool keepDims = false)
    {
        return *(new ReduceMean<T>(unit, axes, keepDims));
    }
}
#endif