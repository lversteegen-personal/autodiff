#ifndef DIFF_MATMUL_H
#define DIFF_MATMUL_H

#include "diff_unit.hpp"

namespace AutoDiff
{
    template <DataType T>
    class MatrixProduct : public Unit<T>
    {
        static Coordinates wildcardMatmulShape(const Coordinates &leftShapeBase, Coordinates rightShapeBase, long leftProductAxis, long rightProductAxis, bool vectorRight)
        {
            rightShapeBase = vectorRight ? rightShapeBase + 1 : rightShapeBase;

            leftProductAxis = leftProductAxis % leftShapeBase.size();
            leftProductAxis = leftProductAxis < 0 ? leftShapeBase.size() + leftProductAxis : leftProductAxis;
            rightProductAxis = rightProductAxis % rightShapeBase.size();
            rightProductAxis = rightProductAxis < 0 ? rightShapeBase.size() + rightProductAxis : rightProductAxis;

            long dim = std::max(leftShapeBase.size(), rightShapeBase.size());

            auto leftShape = leftShapeBase.shiftRight(1, dim - leftShapeBase.size());
            leftProductAxis += dim - leftShapeBase.size();
            auto rightShape = rightShapeBase.shiftRight(1, dim - rightShapeBase.size());
            rightProductAxis += dim - rightShapeBase.size();

            if (leftProductAxis == rightProductAxis)
                throw std::invalid_argument("leftProductAxis must be different from rightProductAxis");

            if (leftShape[leftProductAxis] != rightShape[rightProductAxis])
                throw std::invalid_argument("Arrays do not have the same length in product dimension.");

            Coordinates resultShape(dim);
            bool hasWildcard = false;

            for (long i = 0; i < dim; i++)
            {
                if (i == rightProductAxis)
                {
                    if (leftShape[i] == -1)
                        throw std::invalid_argument("Product axis cannot be wildcard.");
                    else
                        resultShape[i] = leftShape[i];
                }
                else if (i == leftProductAxis)
                {
                    if (rightShape[i] == -1)
                        throw std::invalid_argument("Product axis cannot be wildcard.");
                    else
                        resultShape[i] = rightShape[i];
                }
                else if (leftShape[i] == -1 || rightShape[i] == -1)
                {
                    if (hasWildcard)
                        throw std::invalid_argument("There can be at most one wildcard dimension between the two shapes.");

                    if ((leftShape[i] != -1 && leftShape[i] != 1) || (rightShape[i] != -1 && rightShape[i] != 1))
                        throw std::invalid_argument("If dimension i is a wildcard for one shape, it must either be a wildcard for the other shape as well or the length of dimension i for the other shape must be 1.");

                    hasWildcard = true;
                    resultShape[i] = -1;
                }
                else if (leftShape[i] == rightShape[i] || leftShape[i] == 1)
                    resultShape[i] = rightShape[i];
                else if (rightShape[i] == 1)
                    resultShape[i] = leftShape[i];
                else
                    throw std::invalid_argument("Arrays do not have compatible shapes for matrix multiplication.");
            }

            return vectorRight ? resultShape.interval(0, resultShape.size() - 1) : resultShape;
        }

    private:
        Coordinates mLeftBroadcastedShape;
        Coordinates mRightBroadcastedShape;
        Unit<T> &mLeft;
        Unit<T> &mRight;
        long mLeftProductAxis;
        long mRightProductAxis;
        Coordinates mReductionAxesLeft;
        Coordinates mReductionAxesRight;
        bool mVectorRight;
        Matmul::MatmulSettings mForwardMatmulSettings;
        Matmul::MatmulSettings mLeftGradientSettings;
        Matmul::MatmulSettings mRightGradientSettings;

    public:
        MatrixProduct(Unit<T> &left, Unit<T> &right, long leftProductAxis = -1, long rightProductAxis = -2, bool vectorRight = false) : mVectorRight(vectorRight), mLeft(left), mRight(right), Unit<T>(left.getDiffTape(), wildcardMatmulShape(left.refWildcardShape(), right.refWildcardShape(), leftProductAxis, rightProductAxis, vectorRight))
        {
            if (vectorRight && leftProductAxis != -1)
                throw std::invalid_argument("For a matrix-vector multiplication, leftProductAxis must be -1.");

            Coordinates rightMatrixShape = vectorRight ? right.refWildcardShape() + 1 : right.refWildcardShape();

            leftProductAxis = leftProductAxis % left.getDim();
            leftProductAxis = leftProductAxis < 0 ? left.getDim() + leftProductAxis : leftProductAxis;
            rightProductAxis = rightProductAxis % rightMatrixShape.size();
            rightProductAxis = rightProductAxis < 0 ? rightMatrixShape.size() + rightProductAxis : rightProductAxis;

            long dim = std::max(left.getDim(), rightMatrixShape.size());
            if (vectorRight)
                assertm(dim == this->getDim() + 1, "For a matrix-vector multiplication, wildcardMatmulShape should have calculated the dimension as one less than the maximum of left and right dim.");
            else
                assertm(dim == this->getDim(), "wildcardMatmulShape should have calculated the dimension as maximum of left and right dim.");

            mLeftBroadcastedShape = left.refWildcardShape().shiftRight(1, dim - left.getDim());
            mLeftProductAxis = leftProductAxis + dim - left.getDim();
            mRightBroadcastedShape = rightMatrixShape.shiftRight(1, dim - rightMatrixShape.size());
            mRightProductAxis = rightProductAxis + dim - rightMatrixShape.size();

            for (long i = 0; i < this->getDim(); i++)
            {
                if (i != mLeftProductAxis && i != mRightProductAxis)
                {
                    if (mLeftBroadcastedShape[i] != this->mWildcardShape[i])
                        mReductionAxesLeft.pushBack(i);
                    if (mRightBroadcastedShape[i] != this->mWildcardShape[i])
                        mReductionAxesRight.pushBack(i);
                }
            }

            mForwardMatmulSettings.leftProductAxis = mLeftProductAxis;
            mForwardMatmulSettings.rightProductAxis = mRightProductAxis;

            mLeftGradientSettings.leftProductAxis = mLeftProductAxis;
            mLeftGradientSettings.rightProductAxis = mRightProductAxis;
            mLeftGradientSettings.reduceAxes = mReductionAxesLeft;
            mLeftGradientSettings.keepDims = false;
            mLeftGradientSettings.setzero = false;

            mRightGradientSettings.leftProductAxis = mLeftProductAxis;
            mRightGradientSettings.rightProductAxis = mRightProductAxis;
            mRightGradientSettings.reduceAxes = mReductionAxesRight;
            mRightGradientSettings.keepDims = false;
            mRightGradientSettings.setzero = false;
        }

        std::vector<Unit<T> *> getDependencies() const override
        {
            return {&mLeft, &mRight};
        }

        void pullGradient() const override
        {
            Array<T> leftTranspose = mLeft.refArray().reshape(mLeftBroadcastedShape).transpose(mLeftProductAxis, mRightProductAxis);
            Array<T> rightTranspose = mRight.refArray().reshape(mRightBroadcastedShape).transpose(mLeftProductAxis, mRightProductAxis);
            Array<T> grad = mVectorRight ? this->mGradient.reshape(this->mWildcardShape + 1) : this->mGradient;

            Matmul::matmul<T>(grad, rightTranspose, &mLeft.mGradient, mLeftGradientSettings);

            if (mVectorRight)
            {
                auto tmp = mRight.mGradient.reshape(mRight.refArrayShape() + 1);
                Matmul::matmul<T>(leftTranspose, grad, &tmp, mRightGradientSettings);
            }
            else
                Matmul::matmul<T>(leftTranspose, grad, &mRight.mGradient, mRightGradientSettings);
        }

        void calculate() override
        {
            if (mVectorRight)
                this->mArray = Matmul::matmul(mLeft.refArray().reshape(mLeftBroadcastedShape), mRight.refArray().reshape(mRightBroadcastedShape), mForwardMatmulSettings).reshape(this->mWildcardShape);
            else
                this->mArray = Matmul::matmul(mLeft.refArray().reshape(mLeftBroadcastedShape), mRight.refArray().reshape(mRightBroadcastedShape), mForwardMatmulSettings);
            Unit<T>::calculate();
        };
    };

    template <DataType T>
    MatrixProduct<T> &matmul(Unit<T> &left, Unit<T> &right, long leftProductAxis = -1, long rightProductAxis = -2)
    {
        return *(new MatrixProduct<T>(left, right, leftProductAxis, rightProductAxis));
    }

    template <DataType T>
    MatrixProduct<T> &matvecmul(Unit<T> &matrix, Unit<T> &vector)
    {
        return *(new MatrixProduct<T>(matrix, vector, -1, -2, true));
    }
}

#endif