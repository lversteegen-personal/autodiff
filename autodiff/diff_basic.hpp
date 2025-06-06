#ifndef DIFF_BASIC_H
#define DIFF_BASIC_H

#include "diff_unit.hpp"

namespace AutoDiff
{
    template <DataType T>
    class Variables : public Unit<T>
    {
        Variables(DiffTape<T> &diffTape, const Coordinates &wildcardShape) : Unit<T>(diffTape, wildcardShape) {};

    public:
    public:
        std::vector<Unit<T> *> getDependencies() const override
        {
            return {};
        }

        static Variables<T> &create(DiffTape<T> &diffTape, const Coordinates &wildcardShape)
        {
            return *(new Variables<T>(diffTape, wildcardShape));
        }

        void setValue(const Array<T> &value)
        {
            if (!this->wildcardMatch(value.refShape()))
                throw std::invalid_argument("The shape of the value does not match the wildcard shape.");

            this->mArray = value;
            this->mDiffTape.reset();
        }

        void pullGradient() const override {};
    };

    template <DataType T>
    class Coefficients : public Unit<T>
    {
    public:
    private:
        Coefficients(DiffTape<T> &diffTape, const Array<T> &array) : Unit<T>(diffTape, array) {};

        Coefficients(DiffTape<T> &diffTape, const std::initializer_list<T> &values) : Unit<T>(diffTape, Array<T>(values)) {};

    public:
        static Coefficients<T> &create(DiffTape<T> &diffTape, const Array<T> &array)
        {
            return *(new Coefficients(diffTape, array));
        }

        static Coefficients<T> &create(DiffTape<T> &diffTape, const std::initializer_list<T> &values)
        {
            return *(new Coefficients(diffTape, values));
        }

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {};
        }

        Array<T> &refCoefficientArray()
        {
            return this->mArray;
        }

        void pullGradient() const override {};
    };

    template <DataType T>
    class Reshape : public Unit<T>
    {
    private:
        Unit<T> &mSource;

    public:
        Reshape(Unit<T> &source, const Coordinates &shape) : mSource(source), Unit<T>(source.getDiffTape(), shape) {}

        void pullGradient() override
        {
            mSource.mGradient += this->mGradient.copy().reshape(mSource.refWildcardShape());
        }

        void calculate() override
        {
            this->mArray = mSource.refArray().copy().reshape(this->refWildcardShape());
            Unit<T>::calculate();
        };
    };

    template <DataType T>
    Reshape<T> &Unit<T>::reshape(const Coordinates &newShape)
    {
        return *(new Reshape<T>(*this, newShape));
    }

    template <DataType T, T (*f)(T), T (*df)(T)>
    class Pointwise : public Unit<T>
    {
    private:
        Unit<T> &mSource;

    public:
        Pointwise(Unit<T> &source) : Unit<T>(source.getDiffTape(), source.refWildcardShape()), mSource(source) {}

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

        void pullGradient() override
        {
            mSource.mGradient += Array<T>::template unaryCompute<T, df>(mSource.mArray) * this->mGradient;
        }

        void calculate() override
        {
            this->mArray = Array<T>::template unaryCompute<T, f>(mSource.mArray);
            Unit<T>::calculate();
        };
    };

    template <DataType T, typename P, T (*f)(const T, const P &), T (*df)(const T, const P &)>
    class ParamPointwise : public Unit<T>
    {
    private:
        Unit<T> &mSource;
        const P mParam;

    public:
        ParamPointwise(Unit<T> &source, P param) : Unit<T>(source.getDiffTape(), source.refWildcardShape()), mSource(source), mParam(param) {}

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

        void pullGradient() const override
        {
            mSource.mGradient += Array<T>::template unaryParamCompute<T, P, df>(mSource.refArray(), mParam) * this->mGradient;
        }

        void calculate() override
        {
            this->mArray = Array<T>::template unaryParamCompute<T, P, f>(mSource.refArray(), mParam);
            Unit<T>::calculate();
        };
    };
}

#endif