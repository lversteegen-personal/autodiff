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

template <DataType T>
class Unit;
template <DataType T>
class Reshape;
template <DataType T>
class ReduceSum;
template <DataType T>
class Softmax;
template <DataType T, typename P, T (*f)(T, P), T (*df)(T, P)>
class ParamElementwise;

template <DataType T>
class DiffTape
{
    friend int main();
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

private:
    std::vector<Unit<T> *> mUnits;
    std::unordered_map<Unit<T> *, long> mOrder;

    const bool mEager = false;
    long mCalcProgress = -1;
    Unit<T> *gradientTarget = nullptr;

public:
    DiffTape<T>() = default;
    // DiffTape<T>(bool eager) : mEager(eager) {}

    ~DiffTape()
    {
        for (auto *pUnit : mUnits)
            delete pUnit;
    }

    void addVariable(Unit<T> *px)
    {
        mOrder[px] = mUnits.size();
        mUnits.push_back(px);
        gradientTarget = nullptr;
    }

    Array<T> getValue(Unit<T> &unit)
    {
        size_t position = mOrder[&unit];
        if (mCalcProgress < position)
        {
            for (size_t i = mCalcProgress + 1; i <= position; i++)
                mUnits[i]->calculate();
            mCalcProgress = position;
        }

        return Array<T>(unit.mArray);
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
            {
                mUnits[i]->calculate();
            }
            mCalcProgress = outputPosition;
        }

        if (outputPosition < inputPosition)
            return Array<T>::constant(input.refArrayShape(), 0);

        if (gradientTarget != &output)
        {
            gradientTarget = &output;
            for (long i = outputPosition; i >= 0; i--)
                mUnits[i]->resetGradient();

            output.initDiff();

            for (long i = outputPosition; i >= 0; i--)
                mUnits[i]->pullGradient();
        }

        return input.mGradient;
    }
};

template <DataType T>
class Unit
{

protected:
    DiffTape<T> &mDiffTape;
    bool mIsCaclulated = false;
    Coordinates mWildcardShape;
    Array<T> mArray = Array<T>::constant({}, 0);

    static Coordinates broadcastWildcardShape(const Coordinates &shape1, const Coordinates &shape2)
    {
        long dim1 = shape1.size(), dim2 = shape2.size();
        long dim = std::max(dim1, dim2);
        long offset1 = dim - dim1, offset2 = dim - dim2;
        Coordinates result(dim);
        long wildcardDimension = -1;

        for (long i = 0; i < dim; i++)
        {
            if (i < offset1)
                result[i] = shape2[i];
            else if (i < offset2)
                result[i] = shape1[i];
            else if (shape1[i] == -1 || shape2[i] == -1)
            {
                if (wildcardDimension != -1)
                    throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");

                wildcardDimension == i;
                result[i] = -1;
            }
            else
                result[i] = std::max(shape1[i + offset1], shape2[i + offset2]);
        }

        return result;
    }

public:

    Array<T> mGradient = Array<T>::constant({}, 0);
    DiffTape<T> &getDiffTape() { return mDiffTape; }
    const Array<T> &refArray() const { return mArray; }
    const Array<T> &refGradient() const { return mGradient; }
    const Coordinates refArrayShape() const { return mArray.refShape(); }
    const Coordinates refWildcardShape() const { return mWildcardShape; }

    Unit<T>(DiffTape<T> &diffTape, const Coordinates &shape) : mDiffTape(diffTape), mWildcardShape(shape)
    {
        mDiffTape.addVariable(this);
    };

    Unit<T>(DiffTape<T> &diffTape, const Array<T> &array) : mDiffTape(diffTape), mArray(array), mWildcardShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

    Unit<T>(DiffTape<T> &diffTape, Array<T> &&array) : mDiffTape(diffTape), mArray(std::forward<Array<T>>(array)), mWildcardShape(array.refShape())
    {
        mDiffTape.addVariable(this);
    };

public:
    Unit<T>() = delete;
    Unit<T>(const Unit<T> &other) = delete;
    Unit<T>(Unit<T> &&other) = delete;

    const size_t getDim() const { return mWildcardShape.size(); }

    virtual void pullGradient() = 0;

    Reshape<T> &reshape(const Coordinates &newShape);

    void resetGradient()
    {
        mGradient = Array<T>::constant(mArray.refShape(), 0);
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

    ReduceSum<T> &reduceSum(bool keepDims = false)
    {
        auto axes = Coordinates(getDim());
        for (size_t i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new ReduceSum<T>(*this, axes, keepDims));
    }

    ReduceSum<T> &reduceSum(Coordinates axes, bool keepDims = false)
    {
        return *(new ReduceSum<T>(*this, axes, keepDims));
    }

    Softmax<T> &softmax()
    {
        auto axes = Coordinates(getDim());
        for (size_t i = 0; i < axes.size(); i++)
            axes[i] = i;

        return *(new Softmax<T>(*this, axes));
    }

    Softmax<T> &softmax(const Coordinates &axes)
    {
        return *(new Softmax<T>(*this, axes));
    }

private:
    inline T leakyReLUPtws(T value, T alpha)
    {
        return value > 0 ? value : value * alpha;
    }

    inline T dleakyReLUPtws(T value, T alpha)
    {
        return value > 0 ? 1 : alpha;
    }

public:
    Unit<T> &leakyReLu(T alpha)
    {
        return *(new ParamElementwise<T, T, leakyReLUPtws, dleakyReLUPtws>(this, alpha));
    }
};

template <DataType T>
std::ostream &operator<<(std::ostream &s, const Unit<T> &x)
{
    return s << x.to_string();
}

template <DataType T>
class Coefficients : public Unit<T>
{
public:
    Coefficients<T>(DiffTape<T> &diffTape, const Array<T> &array) : Unit<T>(diffTape, array){};

    Coefficients<T>(DiffTape<T> &diffTape, std::initializer_list<T> values) : Unit<T>(diffTape, Array<T>(values)){};

    void pullGradient() override {};
};

template <DataType T>
class ReduceSum : public Unit<T>
{
private:
    Unit<T> &mSource;
    const Coordinates mAxes;
    Coordinates mKeepDimsShape;
    const bool mKeepDims;

public:
    ReduceSum<T>(Unit<T> &source, const Coordinates &axes, bool keepDims = false) : mSource(source), mAxes(axes), mKeepDims(keepDims), Unit<T>(source.getDiffTape(), Array<T>::reducedShape(source.refWildcardShape(), axes, keepDims)),
                                                                                    mKeepDimsShape(mKeepDims ? Unit<T>::mWildcardShape : Array<T>::reducedShape(source.refWildcardShape(), axes, true)) {}

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
class Reshape : public Unit<T>
{
private:
    Unit<T> &mSource;

public:
    Reshape<T>(Unit<T> &source, const Coordinates &shape) : mSource(source), Unit<T>(source.getDiffTape(), shape) {}

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

template <DataType T>
class MatrixProduct : public Unit<T>
{
private:
    Unit<T> &mLeft;
    Unit<T> &mRight;
    const long mLeftProductAxis;
    const long mRightProductAxis;
    Coordinates mReductionAxesLeft;
    Coordinates mReductionAxesRight;

public:
    MatrixProduct<T>(Unit<T> &left, Unit<T> &right, long leftProductAxis = -1, long rightProductAxis = -2) : mLeft(left), mRight(right), Unit<T>(left.getDiffTape(), Array<T>::matmulShape(left.refWildcardShape(), right.refWildcardShape(), leftProductAxis, rightProductAxis)), mLeftProductAxis(leftProductAxis), mRightProductAxis(rightProductAxis)
    {
        for (long i = 0; i < this->mWildcardShape.size(); i++)
        {
            if (i != mLeftProductAxis && i != mRightProductAxis)
            {
                //TODO: Take care of wildcards, also for Array<T>::matmulShape
                if (left.refWildcardShape()[i] < this->mWildcardShape[i])
                    mReductionAxesLeft.pushBack(i);
                if (right.refWildcardShape()[i] < this->mWildcardShape[i])
                    mReductionAxesRight.pushBack(i);
            }
        }
    }

    void pullGradient() override
    {
        auto leftTranspose = mLeft.refArray().transpose(mLeftProductAxis, mRightProductAxis);
        auto rightTranspose = mRight.refArray().transpose(mLeftProductAxis, mRightProductAxis);
        mLeft.mGradient += Array<T>::matmul(this->mGradient, rightTranspose, mLeftProductAxis, mRightProductAxis).reduceSum(mReductionAxesLeft, true);
        mRight.mGradient += Array<T>::matmul(leftTranspose, this->mGradient, mLeftProductAxis, mRightProductAxis).reduceSum(mReductionAxesRight, true);
    }

    void calculate() override
    {
        this->mArray = Array<T>::matmul(mLeft.refArray(), mRight.refArray(), mLeftProductAxis, mRightProductAxis);
        Unit<T>::calculate();
    };
};

template <DataType T>
class BinaryPointwiseOperation : public Unit<T>
{
protected:
    Unit<T> &mLeft;
    Unit<T> &mRight;
    Coordinates mReductionAxesLeft;
    Coordinates mReductionAxesRight;

    BinaryPointwiseOperation<T>(Unit<T> &left, Unit<T> &right) : mLeft(left), mRight(right), Unit<T>(left.getDiffTape(), Unit<T>::broadcastWildcardShape(left.refWildcardShape(), right.refWildcardShape()))
    {
        mReductionAxesLeft = Coordinates::findDifferences(left.refWildcardShape(), this->mWildcardShape);
        mReductionAxesRight = Coordinates::findDifferences(right.refWildcardShape(), this->refWildcardShape());
    }
};

template <DataType T>
class Sum : public BinaryPointwiseOperation<T>
{

public:
    Sum<T>(Unit<T> &addend1, Unit<T> &addend2) : BinaryPointwiseOperation<T>(addend1, addend2) {}

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
    Difference<T>(Unit<T> &minuend, Unit<T> &subtrahend) : BinaryPointwiseOperation<T>(minuend, subtrahend) {}

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
    Product<T>(Unit<T> &factor1, Unit<T> &factor2) : BinaryPointwiseOperation<T>(factor1, factor2) {}

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
    Quotient<T>(Unit<T> &dividend, Unit<T> &divisor) : BinaryPointwiseOperation<T>(dividend, divisor) {}

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
    Scale<T>(Unit<T> &source, T scalar) : Unit<T>(source.mDiffTape, source.refShape())
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
    Translate<T>(Unit<T> &source, T translate) : Unit<T>(source.mDiffTape, source.refShape())
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

template <DataType T, T (*f)(T), T (*df)(T)>
class Elementwise : public Unit<T>
{
private:
    Unit<T> &mSource;

public:
    Elementwise(Unit<T> &source) : Unit<T>(source.getDiffTape(), source.refShape()), mSource(source) {}

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

template <DataType T, typename P, T (*f)(T, P), T (*df)(T, P)>
class ParamElementwise : public Unit<T>
{
private:
    Unit<T> &mSource;
    const P mParam;

public:
    ParamElementwise(Unit<T> &source, P param) : Unit<T>(source.getDiffTape(), source.refShape()), mSource(source), mParam(param) {}

    void pullGradient() override
    {
        mSource.mGradient += Array<T>::template unaryParamCompute<T, P, df>(mSource.mArray, mParam) * this->mGradient;
    }

    void calculate() override
    {
        this->mArray = Array<T>::template unaryParamCompute<T, P, f>(mSource.mArray, mParam);
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
    Softmax<T>(Unit<T> &source, const Coordinates &axes) : mSource(source), mAxes(axes), Unit<T>(source.getDiffTape(), source.refWildcardShape()) {}

    void pullGradient() override
    {
        Array<T> expd = mSource.refArray().exp();
        Array<T> norm = expd.reduceSum(mAxes, true);
        auto &g = this->mGradient;
        Array<T> prod = (expd * g);
        Array<T> tmp = prod.reduceSum(mAxes, true) / norm;

        mSource.mGradient += prod / norm * (1.0 - tmp);
    }

    void calculate() override
    {
        Array<T> expd = mSource.refArray().exp();
        this->mArray = expd / expd.reduceSum(mAxes, true);
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

template <DataType T>
MatrixProduct<T> &matmul(Unit<T> &left, Unit<T> &right, long leftProductAxis = -1, long rightProductAxis = -2)
{
    return *(new MatrixProduct<T>(left, right, leftProductAxis, rightProductAxis));
}

#endif