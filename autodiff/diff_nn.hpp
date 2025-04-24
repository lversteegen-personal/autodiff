#ifndef DIFF_NN_H
#define DIFF_NN_H

#include <type_traits>
#include "diff_unit.hpp"
#include "diff_matmul.hpp"
#include "diff_binary_ptws.hpp"
#include "diff_basic.hpp"

enum class Activation
{
    NONE = 0,
    LEAKYRELU = 2,
};

class Test;

// template <DataType T>
class Model
{
public:
    static RandomArrayGenerator randomArrayGenerator;
};

RandomArrayGenerator Model::randomArrayGenerator(0);

template <DataType T>
class MeanSquaredError : public Unit<T>
{
private:
    Unit<T> &mPrediction;
    Unit<T> &mTarget;
    long mDivisor = 1;

public:
    MeanSquaredError(Unit<T> &prediction, Variables<T> &target) : mPrediction(prediction), mTarget(target), Unit<T>(prediction.getDiffTape(), Coordinates(0))
    {
        if (prediction.refWildcardShape() != target.refWildcardShape())
            throw std::invalid_argument("Prediction must have the same shape as target.");
        auto &sourceShape = prediction.refWildcardShape();
        for (auto l : sourceShape)
        {
            if (l == 0)
                std::invalid_argument("Cannot take mean of empty array");
            mDivisor = l > 0 ? mDivisor * l : mDivisor;
        }
    }

    void pullGradient() override
    {
        Array<T> grad = (mPrediction.refArray()-mTarget.refArray())* (static_cast<T>(2)*(this->mGradient));
        mPrediction.mGradient += grad;
        mTarget.mGradient -= grad;
    }

    void calculate() override
    {
        this->mArray = (mPrediction.refArray()-mTarget.refArray()).square().reduceSum() / mDivisor;
        Unit<T>::calculate();
    };
};

template <DataType T>
class LinearLayer
{
    friend class Test;

private:
    Unit<T> &mInput;
    Coefficients<T> &mWeightMatrix;
    Coefficients<T> &mBiasVector;

public:
    Unit<T> &output;

    operator Unit<T> &()
    {
        return output;
    }

    LinearLayer(Unit<T> &input, Coefficients<T> &weightMatrix, Coefficients<T> &biasVector, Unit<T> &output) : mInput(input), mWeightMatrix(weightMatrix), mBiasVector(biasVector), output(output)
    {
    }

    static LinearLayer<T> create(Unit<T> &input, Coefficients<T> &weightMatrix, Coefficients<T> &biasVector, Activation activation)
    {
        auto &intermediate = matvecmul(weightMatrix, input) + biasVector;
        switch (activation)
        {
        case Activation::NONE:
            return *(new LinearLayer<T>(input, weightMatrix, biasVector, intermediate));

        case Activation::LEAKYRELU:
            throw std::invalid_argument("LeakyReLU activation requires a parameter.");

        default:
            throw std::invalid_argument("Unsupported activation function.");
        }
    }

    template <typename P = T>
    static LinearLayer<T> create(Unit<T> &input, Coefficients<T> &weightMatrix, Coefficients<T> &biasVector, Activation activation, P activationParam)
    {
        auto &intermediate = matvecmul(weightMatrix, input) + biasVector;
        switch (activation)
        {
        case Activation::NONE:
            return LinearLayer<T>(input, weightMatrix, biasVector, intermediate);

        case Activation::LEAKYRELU:
            if (std::is_same_v<T, P>)
                return LinearLayer<T>(input, weightMatrix, biasVector, intermediate.leakyReLu((T)activationParam));
            else
                throw std::invalid_argument("Activation parameter type for LeakyReLu must be the same as the type of the units.");

        default:
            throw std::invalid_argument("Unsupported activation function.");
        }
    }

    template <typename P = T>
    static LinearLayer<T> create(Unit<T> &input, long nodes, Activation activation, P activationParam)
    {
        long inputLength = input.refWildcardShape()[input.getDim() - 1];
        auto rawWeights = Model::randomArrayGenerator.normal<T>({nodes, inputLength}, 0, 1).clip(-1, 1);
        auto rawBias = Model::randomArrayGenerator.normal<T>({nodes}, 0, 1).clip(-1, 1);
        auto &weightMatrix = *(new Coefficients<T>(input.getDiffTape(), rawWeights));
        auto &biasVector = *(new Coefficients<T>(input.getDiffTape(), rawBias));

        return create<P>(input, weightMatrix, biasVector, activation, activationParam);
    }

    static LinearLayer<T> create(Unit<T> &input, long nodes, Activation activation)
    {
        return create<T>(input, nodes, activation, 0);
    }

    void applyGradient(DiffTape<T> &diffTape, Unit<T> &target, T learningRate, T clipValue)
    {
        auto &weightGradient = mWeightMatrix.refGradient();
        auto &biasGradient = mBiasVector.refGradient();

        mathmeasure.start();
        auto tmpMat = (weightGradient * learningRate);
        auto tmpBias = (biasGradient * learningRate);
        mathmeasure.stop();

        clipMeasure.start();
        tmpMat = tmpMat.clip(-clipValue, clipValue);
        tmpBias = tmpBias.clip(-clipValue, clipValue);
        clipMeasure.stop();

        mathmeasure.start();
        mWeightMatrix.refCoefficientArray() -= tmpMat;
        mBiasVector.refCoefficientArray() -= tmpBias;
        mathmeasure.stop();
    }

    void applyGradient(DiffTape<T> &diffTape, Unit<T> &target, T learningRate)
    {
        applyGradient(diffTape, target, learningRate, learningRate);
    }
};

#endif