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

    template <typename Operation, typename T>
    concept IsParametrizedDifferentiableOperation = IsParametrizedOperation<typename Operation::Function, T> && IsParametrizedOperation<typename Operation::Differential, T> && requires(Operation info, Operation::Function fInfo, Operation::Differential dfInfo) {
        requires std::is_same_v<decltype(info.param), decltype(fInfo.param)> && std::is_same_v<decltype(info.param), decltype(dfInfo.param)>;
    };

    template <typename Operation, typename T>
    concept IsNonParametrizedDifferentiableOperation = IsNonParametrizedOperation<typename Operation::Function, T> && IsNonParametrizedOperation<typename Operation::Differential, T>;

    template <typename Operation, DataType T>
        requires IsNonParametrizedDifferentiableOperation<Operation, T>
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
            computeInPlace<FusedMultiplyAdd<T>, true, true, true>(mSource.mGradient, this->mGradient, compute<Operation::Differential>(mSource.mArray), mSource.mGradient);
        }

        void calculate() override
        {
            ArrayLibrary::computeInPlace<Operation::Function>(this->mArray, mSource.mArray);
            Unit<T>::calculate();
        };
    };

    template <typename Operation, DataType T>
        requires IsParametrizedDifferentiableOperation<Operation, T>
    class ParamPointwise : public Unit<T>
    {
    private:
        Unit<T> &mSource;
        Operation mOp;

    public:
        ParamPointwise(Unit<T> &source, Operation opInfo) : Unit<T>(source.getDiffTape(), source.refWildcardShape()), mSource(source), mOp(opInfo) {}

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mSource};
        }

        void pullGradient() const override
        {
            computeInPlace<FusedMultiplyAdd<T>, true, true, true>(mSource.mGradient, this->mGradient, compute<typename Operation::Differential>(mOp.param, mSource.refArray()), mSource.mGradient);
        }

        void calculate() override
        {
            this->mArray = ArrayLibrary::compute<typename Operation::Function>(mOp.param, mSource.refArray());
            Unit<T>::calculate();
        };
    };
}

#endif