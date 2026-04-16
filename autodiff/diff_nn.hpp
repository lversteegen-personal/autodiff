#ifndef DIFF_NN_H
#define DIFF_NN_H

#include <vector>
#include <stack>
#include <unordered_set>
#include <type_traits>

#include "diff_unit.hpp"
#include "diff_matmul.hpp"
#include "diff_binary_ptws.hpp"
#include "diff_basic.hpp"
#include "optimizer.hpp"

namespace AutoDiff
{
    namespace NeuralNetworks
    {
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

            void pullGradient() const override
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
        Softmax<T> &softmax(Unit<T> &unit)
        {
            auto axes = Coordinates(unit.getDim());
            for (long i = 0; i < axes.size(); i++)
                axes[i] = i;

            return *(new Softmax<T>(unit, axes));
        }

        template <DataType T>
        Softmax<T> &softmax(Unit<T> &unit, const Coordinates &axes)
        {
            return *(new Softmax<T>(unit, axes));
        }

        template <DataType T>
        class Softermax : public Unit<T>
        {
        private:
            Unit<T> &mSource;
            const Coordinates mAxes;

        public:
            Softermax(Unit<T> &source, const Coordinates &axes) : mSource(source), mAxes(axes), Unit<T>(source.getDiffTape(), source.refWildcardShape())
            {
            }

            static Softermax<T> &create(Unit<T> &source, const Coordinates &axes)
            {
                return *(new Softermax<T>(source, axes));
            }

            std::vector<Unit<T> *> getDependencies() const override
            {
                return {&mSource};
            }

            struct Function
            {
                static inline T f(const T x)
                {
                    constexpr T ONE = static_cast<T>(1);
                    constexpr T TWO = static_cast<T>(2);
                    return x < ONE ? ONE / (TWO - x) : x * x * x;
                }
            };

            struct Differential
            {
                static inline T f(const T x)
                {
                    constexpr T ONE = static_cast<T>(1);
                    constexpr T TWO = static_cast<T>(2);
                    return x < ONE ? ONE / ((TWO - x) * (TWO - x)) : 3 * x * x;
                }
            };

            void pullGradient() const override
            {
                Array<T> tmp = compute<Function>(mSource.refArray());
                Array<T> dTmp = compute<Differential>(mSource.refArray());
                Array<T> norm = tmp.reduceSum(mAxes, true);
                auto &g = this->mGradient;
                Array<T> innerProd = (tmp * g).reduceSum(mAxes, true);

                struct LocalComp
                {
                    static inline T f(const T a, const T b, const T c, const T d, const T e)
                    {
                        return a + b * (c - d / e) / e;
                    }

                    static inline Simd::Vector<T> fSimd(const Simd::Vector<T> a, const Simd::Vector<T> b, const Simd::Vector<T> c, const Simd::Vector<T> d, const Simd::Vector<T> e)
                    {
                        return a + b * (c - d / e) / e;
                    }
                };

                computeInPlace<LocalComp>(mSource.mGradient, mSource.mGradient, dTmp, g, innerProd, norm);
            }

            void calculate() override
            {

                Array<T> tmp = compute<Function>(mSource.refArray());
                this->mArray = tmp / tmp.reduceSum(mAxes, true);
                Unit<T>::calculate();
            }
        };

        template <DataType T>
            requires std::is_floating_point_v<T>
        struct LeakyReLU
        {
            T param;
            LeakyReLU(const T &alpha) : param(alpha) {}

            struct Function
            {
                T param;
                Simd::Vector<T> simdParam;
                Function(const T &alpha) : param(alpha), simdParam(Simd::broadcast_set(alpha)) {}

                static inline T f(const T &alpha, const T input)
                {
                    return input > 0 ? input : input * alpha;
                }

                static inline Simd::Vector<T> fSimd(const Simd::Vector<T> &alpha, const Simd::Vector<T> input)
                {
                    return Simd::max<T>(input, Simd::zero<T>()) + Simd::min<T>(input * alpha, Simd::zero<T>());
                }
            };

            struct Differential
            {
                T param;
                Simd::Vector<T> simdParam;

                Differential(const T &alpha) : param(alpha), simdParam(Simd::broadcast_set(alpha)) {}

                static inline T f(const T &alpha, const T input)
                {
                    return input > 0 ? 1 : alpha;
                }

                static inline Simd::Vector<T> fSimd(const Simd::Vector<T> &alpha, const Simd::Vector<T> input)
                {
                    return Simd::step<T>(input, alpha, Simd::broadcast_set<T>(1));
                }
            };
        };

        template <DataType T>
        Unit<T> &leakyReLu(Unit<T> &unit, T alpha)
        {
            return *(new ParamPointwise<LeakyReLU<T>, T>(unit, alpha));
        }

        RandomArrayGenerator randomArrayGenerator = RandomArrayGenerator(0);

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

            std::vector<Unit<T> *> getDependencies() const override
            {
                return {&mPrediction, &mTarget};
            }

            void pullGradient() const override
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

                Coefficients<T> *pKernel = nullptr;
                Coefficients<T> *pBias = nullptr;

                long nodes = 0;

                Settings() = delete;
                const bool hasCoefficients;

                Settings(long nodes, Activation activation = Activation::None, P activationParam = P()) : nodes(nodes), activation(activation), activationParam(activationParam), hasCoefficients(false)
                {
                }

                Settings(Coefficients<T> &weightMatrix, Coefficients<T> &biasVector, Activation activation = Activation::None, P activationParam = P()) : nodes(weightMatrix.refWildcardShape()[weightMatrix.refWildcardShape().size() - 2]), pKernel(&weightMatrix), pBias(&biasVector), activation(activation), activationParam(activationParam), hasCoefficients(true)
                {
                }
            };

        private:
            Unit<T> &mInput;
            Coefficients<T> &mKernel;
            Coefficients<T> &mBias;

        public:
            const Coefficients<T> &refKernel() const { return mKernel; }
            const Coefficients<T> &refBias() const { return mBias; }

            Unit<T> &output;

            operator Unit<T> &()
            {
                return output;
            }

            LinearLayer(Unit<T> &input, Coefficients<T> &kernel, Coefficients<T> &bias, Unit<T> &output) : mInput(input), mKernel(kernel), mBias(bias), output(output)
            {
            }

        public:
            template <typename P>
            static LinearLayer<T> create(Unit<T> &input, Settings<P> settings)
            {
                Coefficients<T> *pKernel = settings.pKernel;
                Coefficients<T> *pBias = settings.pBias;

                if (!settings.hasCoefficients)
                {
                    long nodes = settings.nodes;
                    long inputLength = input.refWildcardShape()[input.getDim() - 1];
                    Array<T> rawWeights(0);

                    if (settings.hasFlag(WeightInitialization::UNIFORM))
                    {
                        T limit = settings.hasFlag(WeightInitialization::GLOROT) ? std::sqrt(6.0 / (nodes + inputLength)) : 1;
                        rawWeights = randomArrayGenerator.uniform<T>({nodes, inputLength}, -limit, limit);
                    }
                    else if (settings.hasFlag(WeightInitialization::NORMAL))
                    {
                        T stddev = settings.hasFlag(WeightInitialization::GLOROT) ? std::sqrt(2.0 / (nodes + inputLength)) : 1;
                        rawWeights = randomArrayGenerator.normal<T>({nodes, inputLength}, 0, stddev);
                        if (settings.hasFlag(WeightInitialization::TRUNCATED))
                            rawWeights = rawWeights.clip(-settings.clipBound, settings.clipBound);
                    }
                    else
                        rawWeights = Array<T>::constant({nodes, inputLength}, 0);

                    pKernel = &Coefficients<T>::create(input.getDiffTape(), rawWeights);
                    pBias = &Coefficients<T>::create(input.getDiffTape(), Array<T>::constant({settings.nodes}, 0));
                }
                else
                {
                    if (settings.pKernel == nullptr || settings.pBias == nullptr)
                        throw std::invalid_argument("Weight matrix and bias vector must be provided if hasCoefficients is true.");
                    if (settings.pKernel->refWildcardShape().get(-2) != settings.nodes)
                        throw std::invalid_argument("Weight matrix must have the same number of rows as the number of nodes.");
                    if (settings.pKernel->refWildcardShape().get(-1) != input.refWildcardShape().get(-1))
                        throw std::invalid_argument("Weight matrix must have the same number of columns as the input length.");
                    if (settings.pBias->refWildcardShape().get(-1) != settings.nodes)
                        throw std::invalid_argument("Bias vector must have the same number of elements as the number of nodes.");
                }

                Sum<T> &intermediate = matvecmul(*pKernel, input) + *pBias;
                switch (settings.activation)
                {
                case Activation::NONE:
                    return LinearLayer<T>(input, *pKernel, *pBias, intermediate);

                case Activation::LEAKYRELU:
                    if (!std::is_same_v<T, P>)
                        throw std::invalid_argument("The activation parameter for leaky relu activation in a layer of type T must also be of type T");
                    return LinearLayer<T>(input, *pKernel, *pBias, leakyReLu(intermediate, settings.activationParam));

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
                auto &weightGradient = mKernel.refGradient();
                auto &biasGradient = mBias.refGradient();

                mathmeasure.start();
                auto tmpMat = (weightGradient * learningRate);
                auto tmpBias = (biasGradient * learningRate);
                mathmeasure.stop();

                clipMeasure.start();
                tmpMat = tmpMat.clip(-clipValue, clipValue);
                tmpBias = tmpBias.clip(-clipValue, clipValue);
                clipMeasure.stop();

                mathmeasure.start();
                mKernel.refCoefficientArray() -= tmpMat;
                mBias.refCoefficientArray() -= tmpBias;
                mathmeasure.stop();
            }

            void applyGradient(DiffTape<T> &diffTape, Unit<T> &target, T learningRate)
            {
                applyGradient(diffTape, target, learningRate, learningRate);
            }
        };
    }
}

#endif