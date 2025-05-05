#ifndef DIFF_NN_H
#define DIFF_NN_H

#include <type_traits>
#include "diff_unit.hpp"
#include "diff_matmul.hpp"
#include "diff_binary_ptws.hpp"
#include "diff_basic.hpp"

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

public:
    static MeanSquaredError<T> &create(Unit<T> &prediction, Variables<T> &target)
    {
        return *(new MeanSquaredError<T>(prediction, target));
    }

    void pullGradient() override
    {
        Array<T> grad = (mPrediction.refArray() - mTarget.refArray()) * ((static_cast<T>(2) / mDivisor) * (this->mGradient));
        mPrediction.mGradient += grad;
        mTarget.mGradient -= grad;
    }

    void calculate() override
    {
        this->mArray = (mPrediction.refArray() - mTarget.refArray()).square().reduceSum() / mDivisor;
        Unit<T>::calculate();
    };
};

template <DataType T>
class LinearLayer
{
public:
    enum class WeightInitialization : uint64_t
    {
        ZERO = 0,
        TRUNCATED = 0b1,
        UNIFORM = 0b10,
        NORMAL = 0b100,
        NORMAL_TRUNCATED = 0b101,
        GLOROT = 0b1000,
        GLOROT_UNIFORM = 0b1010,
        GLOROT_NORMAL = 0b1100,
        GLOROT_NORMAL_TRUNCATED = 0b1101
    };

    enum class Activation
    {
        NONE = 0,
        LEAKYRELU = 2,
    };

    template <typename P = T>
    struct Settings
    {
        WeightInitialization weightMatrixInit = WeightInitialization::GLOROT_UNIFORM;

        bool hasFlag(WeightInitialization flag)
        {
            return static_cast<uint64_t>(weightMatrixInit) & static_cast<uint64_t>(flag);
        }

        Activation activation = Activation::NONE;
        P activationParam;
        T clipBound = 1;

        Coefficients<T> *pWeightMatrix = nullptr;
        Coefficients<T> *pBiasVector = nullptr;

        long nodes = 0;

        Settings() = delete;
        const bool hasCoefficients;

        Settings(long nodes, Activation activation = Activation::None, P activationParam = P()) : nodes(nodes), activation(activation), activationParam(activationParam), hasCoefficients(false)
        {
        }

        Settings(Coefficients<T> &weightMatrix, Coefficients<T> &biasVector, Activation activation = Activation::None, P activationParam = P()) : nodes(weightMatrix.refWildcardShape()[weightMatrix.refWildcardShape().size()-2]), pWeightMatrix(&weightMatrix), pBiasVector(&biasVector), activation(activation), activationParam(activationParam), hasCoefficients(true)
        {
        }
    };

private:
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

public:
    template <typename P>
    static LinearLayer<T> create(Unit<T> &input, Settings<P> settings)
    {
        Coefficients<T> *pWeightMatrix = settings.pWeightMatrix;
        Coefficients<T> *pBiasVector = settings.pBiasVector;

        if (!settings.hasCoefficients)
        {
            long nodes = settings.nodes;
            long inputLength = input.refWildcardShape()[input.getDim() - 1];
            Array<T> rawWeights(0);

            if (settings.hasFlag(WeightInitialization::UNIFORM))
            {
                T limit = settings.hasFlag(WeightInitialization::GLOROT) ? std::sqrt(6.0 / (nodes + inputLength)) : 1;
                rawWeights = Model::randomArrayGenerator.uniform<T>({nodes, inputLength}, -limit, limit);
            }
            else if (settings.hasFlag(WeightInitialization::NORMAL))
            {
                T stddev = settings.hasFlag(WeightInitialization::GLOROT) ? std::sqrt(2.0 / (nodes + inputLength)) : 1;
                rawWeights = Model::randomArrayGenerator.normal<T>({nodes, inputLength}, 0, stddev);
                if (settings.hasFlag(WeightInitialization::TRUNCATED))
                    rawWeights = rawWeights.clip(-settings.clipBound, settings.clipBound);
            }
            else
                rawWeights = Array<T>::constant({nodes, inputLength}, 0);

            pWeightMatrix = &Coefficients<T>::create(input.getDiffTape(), rawWeights);
            pBiasVector = &Coefficients<T>::create(input.getDiffTape(), Array<T>::constant({settings.nodes}, 0));
        }
        else
        {
            if (settings.pWeightMatrix == nullptr || settings.pBiasVector == nullptr)
                throw std::invalid_argument("Weight matrix and bias vector must be provided if hasCoefficients is true.");
            if (settings.pWeightMatrix->refWildcardShape().get(-2) != settings.nodes)
                throw std::invalid_argument("Weight matrix must have the same number of rows as the number of nodes.");
            if (settings.pWeightMatrix->refWildcardShape().get(-1) != input.refWildcardShape().get(-1))
                throw std::invalid_argument("Weight matrix must have the same number of columns as the input length.");
            if (settings.pBiasVector->refWildcardShape().get(-1) != settings.nodes)
                throw std::invalid_argument("Bias vector must have the same number of elements as the number of nodes.");
        }

        Sum<T> &intermediate = matvecmul(*pWeightMatrix, input) + *pBiasVector;
        switch (settings.activation)
        {
        case Activation::NONE:
            return LinearLayer<T>(input, *pWeightMatrix, *pBiasVector, intermediate);

        case Activation::LEAKYRELU:
            if (!std::is_same_v<T, P>)
                throw std::invalid_argument("The activation parameter for leaky relu activation in a layer of type T must also be of type T");
            return LinearLayer<T>(input, *pWeightMatrix, *pBiasVector, intermediate.leakyReLu(settings.activationParam));

        default:
            throw std::invalid_argument("Unsupported activation function.");
        }
    }

    template <typename P = T>
    static LinearLayer<T> create(Unit<T> &input, long nodes, Activation activation, P activationParam)
    {
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