#ifndef DIFF_BINARY_PTWS_H
#define DIFF_BINARY_PTWS_H

#include "diff_unit.hpp"

template <DataType T>
class BinaryPointwiseOperation : public Unit<T>
{
protected:
    Unit<T> &mLeft;
    Unit<T> &mRight;
    Coordinates mReductionAxesLeft;
    Coordinates mReductionAxesRight;

    BinaryPointwiseOperation(Unit<T> &left, Unit<T> &right) : mLeft(left), mRight(right), Unit<T>(left.getDiffTape(), Unit<T>::wildcardBroadcastShape(left.refWildcardShape(), right.refWildcardShape()))
    {
        mReductionAxesLeft = Coordinates::findDifferences(left.refWildcardShape(), this->refWildcardShape());
        mReductionAxesRight = Coordinates::findDifferences(right.refWildcardShape(), this->refWildcardShape());
    }
};

template <DataType T>
class Sum : public BinaryPointwiseOperation<T>
{

public:
    Sum(Unit<T> &addend1, Unit<T> &addend2) : BinaryPointwiseOperation<T>(addend1, addend2) {}

    void pullGradient() override
    {
        this->mLeft.mGradient += this->mGradient.reduceSum(this->mReductionAxesLeft, true);
        this->mRight.mGradient += this->mGradient.reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft.refArray() + this->mRight.refArray();
        Unit<T>::calculate();
    };
};

template <DataType T>
class Difference : public BinaryPointwiseOperation<T>
{
public:
    Difference(Unit<T> &minuend, Unit<T> &subtrahend) : BinaryPointwiseOperation<T>(minuend, subtrahend) {}

    void pullGradient() override
    {
        this->mLeft.mGradient += this->mGradient.reduceSum(this->mReductionAxesLeft, true);
        this->mRight.mGradient -= this->mGradient.reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft.refArray() - this->mRight.refArray();
        Unit<T>::calculate();
    };
};

template <DataType T>
class Product : public BinaryPointwiseOperation<T>
{

public:
    Product(Unit<T> &factor1, Unit<T> &factor2) : BinaryPointwiseOperation<T>(factor1, factor2) {}

    void pullGradient() override
    {
        this->mLeft.mGradient += (this->mGradient * this->mRight.refArray()).reduceSum(this->mReductionAxesLeft, true);
        this->mRight.mGradient += (this->mGradient * this->mLeft.refArray()).reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft.refArray() * this->mRight.refArray();
        Unit<T>::calculate();
    };
};

template <DataType T>
class Quotient : public BinaryPointwiseOperation<T>
{
public:
    Quotient(Unit<T> &dividend, Unit<T> &divisor) : BinaryPointwiseOperation<T>(dividend, divisor) {}

    void pullGradient() override
    {
        this->mLeft.mGradient += (this->mGradient / this->mRight.mArray).reduceSum(this->mReductionAxesLeft, true);

        this->mRight.mGradient -= (this->mGradient * this->mLeft.mArray / this->mRight.mArray.square()).reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft.mArray / this->mRight.mArray;
        Unit<T>::calculate();
    };
};

template <DataType T>
class Scale : public Unit<T>
{
private:
    Unit<T> &mSource;
    const T mScalar;

public:
    Scale(Unit<T> &source, T scalar) : Unit<T>(source.mDiffTape, source.refShape())
    {
        mSource = source;
        mScalar = scalar;
    };

    void pullGradient() override
    {
        mSource.mGradient += this->mGradient * mScalar;
    }

    void calculate() override
    {
        this->mArray = mScalar * mSource.mArray;
        Unit<T>::calculate();
    };
};

template <DataType T>
class Translate : public Unit<T>
{
private:
    Unit<T> &mSource;
    const T mTranslate;

public:
    Translate(Unit<T> &source, T translate) : Unit<T>(source.mDiffTape, source.refShape())
    {
        mSource = source;
        mTranslate = translate;
    };

    void pullGradient() override
    {
        mSource.mGradient += this->mGradient;
    }

    void calculate() override
    {
        this->mArray = mSource.mArray + mTranslate;
        Unit<T>::calculate();
    };
};

template <DataType T>
Sum<T> &operator+(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Sum<T>(lhs, rhs));
}

template <DataType T>
Difference<T> &operator-(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Difference<T>(lhs, rhs));
}

template <DataType T>
Product<T> &operator*(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Product<T>(lhs, rhs));
}

template <DataType T>
Quotient<T> &operator/(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Quotient<T>(lhs, rhs));
}

template <DataType T>
Scale<T> &operator*(T scalar, Unit<T> &source)
{
    return *(new Scale<T>(source, scalar));
}

template <DataType T>
Scale<T> &operator*(Unit<T> &source, T scalar)
{
    return *(new Scale<T>(source, scalar));
}

template <DataType T>
Scale<T> &operator/(Unit<T> &source, T scalar)
{
    return *(new Scale<T>(source, 1 / scalar));
}

template <DataType T>
Translate<T> &operator+(Unit<T> &source, T translate)
{
    return *(new Translate<T>(source, translate));
}

template <DataType T>
Translate<T> &operator+(T translate, Unit<T> &source)
{
    return *(new Translate<T>(source, translate));
}

template <DataType T>
Translate<T> &operator-(Unit<T> &source, T translate)
{
    return *(new Translate<T>(source, -translate));
}

#endif