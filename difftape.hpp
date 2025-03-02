#ifndef DIFFTAPE_H
#define DIFFTAPE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <cstdarg>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <optional>

#include "array.hpp"

template <typename T>
class Unit;

template <typename T>
class DiffTape
{
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

private:
    std::vector<Unit<T> *> mUnits;
    std::unordered_map<Unit<T> *, long> mOrder;

    bool mEager = false;
    long mCalcProgress = -1;
    Unit<T> *gradientTarget = nullptr;

public:
    DiffTape<T>() = default;
    DiffTape<T>(bool eager)
    {
        mEager = eager;
    }

    ~DiffTape()
    {
        for (auto *pUnit : mUnits)
            delete pUnit;
    }

    void addVariable(Unit<T> *px)
    {
        mOrder[px] = mUnits.size();
        mUnits.push_back(px);
        if (mEager)
            px->calculate();
        gradientTarget = nullptr;
    }

    Array<T> getValue(Unit<T> *px)
    {
        size_t position = mOrder[px];
        if (mCalcProgress < position)
        {
            for (size_t i = mCalcProgress + 1; i <= position; i++)
                mUnits[i]->calculate();
            mCalcProgress = position;
        }

        return Array<T>(px->mArray);
    }

    Array<T> getGradient(Unit<T> &input, Unit<T> &output)
    {
        if (mOrder.find(&input) == mOrder.end())
            throw std::invalid_argument("Input not found in the tape!");
        if (mOrder.find(&output) == mOrder.end())
            throw std::invalid_argument("Output not found in the tape!");

        auto inputPosition = mOrder[&input];
        auto outputPosition = mOrder[&output];

        if (mCalcProgress < outputPosition)
        {
            for (size_t i = mCalcProgress + 1; i <= outputPosition; i++)
                mUnits[i]->calculate();
            mCalcProgress = outputPosition;
        }

        if (outputPosition < inputPosition)
            return Array<T>::constant(input.refShape(), 0);

        if (gradientTarget != &output)
        {
            gradientTarget = &output;
            for (long i = outputPosition; i >= 0; i--)
                mUnits[i]->resetGradient();

            output.initDiff();

            for (long i = outputPosition; i >= 0; i--)
                mUnits[i]->pullGradient();
        }

        return input.mGradient.copy();
    }
};

template <typename T>
class Unit
{
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

protected:
    DiffTape<T> &mDiffTape;
    bool mIsCaclulated = false;
    Coordinates mShape;

public:
    Array<T> mArray = Array<T>::constant({}, 0);
    Array<T> mGradient = Array<T>::constant({}, 0);

    DiffTape<T> &getDiffTape() { return mDiffTape; }
    Array<T> getArray() const { return mArray; }
    Array<T> getGradient() const { return mGradient; }

    Unit<T>(DiffTape<T> &diffTape, const Coordinates &shape) : mDiffTape(diffTape), mShape(shape)
    {
        mDiffTape.addVariable(this);
    };

    Unit<T>(DiffTape<T> &diffTape, const Array<T> &array) : mDiffTape(diffTape), mArray(array), mShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

    Unit<T>(DiffTape<T> &diffTape, Array<T> &&array) : mDiffTape(diffTape), mArray(std::forward<Array<T>>(array)), mShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

public:
    Unit<T>() = delete;
    Unit<T>(const Unit<T> &other) = delete;
    Unit<T>(Unit<T> &&other) = delete;

    const Coordinates &refShape() const { return mShape; }

    virtual void pullGradient() = 0;

    void resetGradient()
    {
        mGradient = Array<T>::constant(mShape, 0);
    }

    virtual void calculate()
    {
        resetGradient();
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
};

template <typename T>
std::ostream &operator<<(std::ostream &s, const Unit<T> &x)
{
    return s << x.to_string();
}

template <typename T>
class Coefficients : public Unit<T>
{
public:
    Coefficients<T>(DiffTape<T> &diffTape, const Array<T> &array) : Unit<T>(diffTape, array){};

    Coefficients<T>(DiffTape<T> &diffTape, std::initializer_list<T> values) : Unit<T>(diffTape, Array<T>(values)){};

    void pullGradient() override {};
};

template <typename T>
class ReduceSum : public Unit<T>
{
private:
    Unit<T> *mSource;

public:
    ReduceSum<T>(Unit<T> *source) : mSource(source), Unit<T>(source->getDiffTape(), Coordinates(1, 1)) {}

    void pullGradient() override
    {
        mSource->mGradient += this->mGradient[0];
    }

    void calculate() override
    {
        this->mArray = Array<T>({mSource->mArray.reduceSum()});
        Unit<T>::calculate();
    };
};

template <typename T>
class MatrixProduct : public Unit<T>
{
private:
    Unit<T> *mLeft;
    Unit<T> *mRight;
    long mLeftProductAxis;
    long mRightProductAxis;
    Coordinates mReductionAxesLeft;
    Coordinates mReductionAxesRight;

public:
    MatrixProduct<T>(Unit<T> *left, Unit<T> *right, long leftProductAxis = -1, long rightProductAxis = -2) : mLeft(left), mRight(right), Unit<T>(left->getDiffTape(), Array<T>::matmulShape(left->refShape(), right->refShape(), leftProductAxis, rightProductAxis)), mLeftProductAxis(leftProductAxis), mRightProductAxis(rightProductAxis) {
        for (long i = 0; i < this->mShape.size(); i++)
        {
            if (i != mLeftProductAxis && i != mRightProductAxis)
            {
                if (left->refShape()[i] < this->mShape[i])
                    mReductionAxesLeft.pushBack(i);
                if (right->refShape()[i] < this->mShape[i])
                    mReductionAxesRight.pushBack(i);
            }
        }
    }

    void pullGradient() override
    {
        auto leftTranspose = mLeft->mArray.transpose(mLeftProductAxis, mRightProductAxis);
        auto rightTranspose = mRight->mArray.transpose(mLeftProductAxis, mRightProductAxis);
        mLeft->mGradient += Array<T>::matmul(this->mGradient, rightTranspose, mLeftProductAxis, mRightProductAxis).reduceSum(mReductionAxesLeft, true);
        mRight->mGradient += Array<T>::matmul(leftTranspose, this->mGradient, mLeftProductAxis, mRightProductAxis).reduceSum(mReductionAxesRight, true);;
    }

    void calculate() override
    {
        this->mArray = Array<T>::matmul(mLeft->mArray, mRight->mArray, mLeftProductAxis, mRightProductAxis);
        Unit<T>::calculate();
    };
};

template <typename T>
class BinaryPointwiseOperation : public Unit<T>
{
protected:
    Unit<T> *const mLeft;
    Unit<T> *const mRight;
    Coordinates mReductionAxesLeft;
    Coordinates mReductionAxesRight;

    BinaryPointwiseOperation<T>(Unit<T> *left, Unit<T> *right) : mLeft(left), mRight(right), Unit<T>(left->getDiffTape(), Array<T>::broadcastShape(left->refShape(), right->refShape()))
    {
        mReductionAxesLeft = Coordinates::findDifferences(left->refShape(), this->mShape);
        mReductionAxesRight = Coordinates::findDifferences(right->refShape(), this->mShape);
    }
};

template <typename T>
class Sum : public BinaryPointwiseOperation<T>
{

public:
    Sum<T>(Unit<T> *addend1, Unit<T> *addend2) : BinaryPointwiseOperation<T>(addend1, addend2) {}

    void pullGradient() override
    {
        this->mLeft->mGradient += this->mGradient.reduceSum(this->mReductionAxesLeft, true);
        this->mRight->mGradient += this->mGradient.reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft->mArray + this->mRight->mArray;
        Unit<T>::calculate();
    };
};

template <typename T>
class Difference : public BinaryPointwiseOperation<T>
{
public:
    Difference<T>(Unit<T> *minuend, Unit<T> *subtrahend) : BinaryPointwiseOperation<T>(minuend, subtrahend) {}

    void pullGradient() override
    {
        this->mLeft->mGradient += this->mGradient.reduceSum(this->mReductionAxesLeft, true);
        ;
        this->mRight->mGradient -= this->mGradient.reduceSum(this->mReductionAxesRight, true);
        ;
    }

    void calculate() override
    {
        this->mArray = this->mLeft->mArray - this->mRight->mArray;
        Unit<T>::calculate();
    };
};

template <typename T>
class Product : public BinaryPointwiseOperation<T>
{

public:
    Product<T>(Unit<T> *factor1, Unit<T> *factor2) : BinaryPointwiseOperation<T>(factor1, factor2) {}

    void pullGradient() override
    {
        this->mLeft->mGradient += (this->mGradient * this->mRight->mArray).reduceSum(this->mReductionAxesLeft, true);
        this->mRight->mGradient += (this->mGradient * this->mLeft->mArray).reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft->mArray * this->mRight->mArray;
        Unit<T>::calculate();
    };
};

template <typename T>
class Quotient : public BinaryPointwiseOperation<T>
{
public:
    Quotient<T>(Unit<T> *dividend, Unit<T> *divisor) : BinaryPointwiseOperation<T>(dividend, divisor) {}

    void pullGradient() override
    {
        this->mLeft->mGradient += (this->mGradient / this->mRight->mArray).reduceSum(this->mReductionAxesLeft, true);
        ;
        this->mRight->mGradient -= (this->mGradient * this->mLeft->mArray / this->mRight->mArray.pow(2)).reduceSum(this->mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = this->mLeft->mArray / this->mRight->mArray;
        Unit<T>::calculate();
    };
};

template <typename T>
class Scale : public Unit<T>
{
private:
    Unit<T> *mSource;
    T mScalar;

public:
    Scale<T>(Unit<T> *source, T scalar) : Unit<T>(source->mDiffTape, source->refShape())
    {
        mSource = source;
        mScalar = scalar;
    };

    void pullGradient() override
    {
        mSource->mGradient += this->mGradient * mScalar;
    }

    void calculate() override
    {
        this->mArray = mScalar * mSource->mArray;
        Unit<T>::calculate();
    };
};

template <typename T>
class Translate : public Unit<T>
{
private:
    Unit<T> *mSource;
    T mTranslate;

public:
    Translate<T>(Unit<T> *source, T translate) : Unit<T>(source->mDiffTape, source->refShape())
    {
        mSource = source;
        mTranslate = translate;
    };

    void pullGradient() override
    {
        mSource->mGradient += this->mGradient + mTranslate;
    }

    void calculate() override
    {
        this->mArray = mSource->mArray + mTranslate;
        Unit<T>::calculate();
    };
};

template <typename T>
class Elementwise : public Unit<T>
{
private:
    Unit<T> *mSource;
    const std::function<T(T, T)> mF;
    const std::function<T(T, T)> mdF;

public:
    Elementwise<T>(Unit<T> *source, std::function<T(T, T)> f, std::function<T(T, T)> dF) : Unit<T>(source->mDiffTape, source->refShape()), mSource(source), mF(f), mdF(dF) {}

    void pullGradient() override
    {
        mSource->mGradient += this->mGradient + Array<T>::applyFunction(mSource->mArray, mdF)*this->mGradient;
    }

    void calculate() override
    {
        this->mArray = mSource->mArray + Array<T>::applyFunction(mSource->mArray, mF);
        Unit<T>::calculate();
    };
};

template <typename T>
Sum<T> &operator+(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Sum<T>(&lhs, &rhs));
}

template <typename T>
Difference<T> &operator-(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Difference<T>(&lhs, &rhs));
}

template <typename T>
Product<T> &operator*(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Product<T>(&lhs, &rhs));
}

template <typename T>
Quotient<T> &operator/(Unit<T> &lhs, Unit<T> &rhs)
{
    return *(new Quotient<T>(&lhs, &rhs));
}

template <typename T>
Scale<T> &operator*(T scalar, Unit<T> &source)
{
    return *(new Scale<T>(&source, scalar));
}

template <typename T>
Scale<T> &operator*(Unit<T> &source, T scalar)
{
    return *(new Scale<T>(&source, scalar));
}

template <typename T>
Scale<T> &operator/(Unit<T> &source, T scalar)
{
    return *(new Scale<T>(&source, 1 / scalar));
}

template <typename T>
Translate<T> &operator+(Unit<T> &source, T translate)
{
    return *(new Translate<T>(&source, translate));
}

template <typename T>
Translate<T> &operator+(T translate, Unit<T> &source)
{
    return *(new Translate<T>(&source, translate));
}

template <typename T>
Translate<T> &operator-(Unit<T> &source, T translate)
{
    return *(new Translate<T>(&source, -translate));
}

template <typename T>
ReduceSum<T> &reduceSum(Unit<T> &source)
{
    return *(new ReduceSum<T>(&source));
}

template <typename T>
MatrixProduct<T> &matmul(Unit<T> &left, Unit<T> &right, long leftProductAxis = -1, long rightProductAxis = -2)
{
    return *(new MatrixProduct<T>(&left, &right, leftProductAxis, rightProductAxis));
}
#endif