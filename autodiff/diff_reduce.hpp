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

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

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
    class Softermax : public Unit<T>
    {
    private:
        Unit<T> &mSource;
        const Coordinates mAxes;

        static inline T softFunc(const T x)
        {
            constexpr T ONE = static_cast<T>(1);
            constexpr T TWO = static_cast<T>(2);
            return x < ONE ? ONE / (TWO - x) : x * x * x;
        }

        static inline T dSoftFunc(const T x)
        {
            constexpr T ONE = 1;
            constexpr T TWO = 2;
            return x < ONE ? ONE / ((TWO - x) * (TWO - x)) : 3 * x * x;
        }

    public:
        Softermax(Unit<T> &source, const Coordinates &axes) : mSource(source), mAxes(axes), Unit<T>(source.getDiffTape(), source.refWildcardShape()) {}

        static Softermax<T> &create(Unit<T> &source, const Coordinates &axes)
        {
            return *(new Softermax<T>(source, axes));
        }

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

        void pullGradient() const override
        {
            Array<T> tmp = Array<T>::template unaryCompute<T, softFunc>(mSource.refArray());
            Array<T> dTmp = Array<T>::template unaryCompute<T, dSoftFunc>(mSource.refArray());
            Array<T> norm = tmp.reduceSum(mAxes, true);
            auto &g = this->mGradient;

            mSource.mGradient += dTmp * (g - (tmp * g).reduceSum(mAxes, true) / norm) / norm;
        }

        void calculate() override
        {
            Array<T> tmp = Array<T>::template unaryCompute<T, softFunc>(mSource.refArray());
            this->mArray = tmp / tmp.reduceSum(mAxes, true);
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
        };
    };
}
#endif