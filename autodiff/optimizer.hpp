#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

#include "diff_unit.hpp"
#include "diff_basic.hpp"

namespace AutoDiff
{

    template <DataType T>
    class Optimizer
    {
    public:
        virtual ~Optimizer() = default;
        virtual void update(T learningRate) = 0;
        virtual void addUnit(Coefficients<T> &coefficients) = 0;
    };

    template <DataType T>
    class SGD : public Optimizer<T>
    {
        std::vector<Coefficients<T> *> mCoefficientsList;

    public:
        SGD() {}

        void addUnit(Coefficients<T> &coefficients) override
        {
            mCoefficientsList.push_back(&coefficients);
        }

        void update(T learningRate) override
        {
            for (Coefficients<T> *coefficients : mCoefficientsList)
            {
                coefficients->refCoefficientArray() -= learningRate * coefficients->refGradient();
            }
        }
    };

    template <DataType T>
        requires std::is_floating_point_v<T>
    class Adam : public Optimizer<T>
    {
        const T beta1 = 0.9;
        const T beta2 = 0.999;
        const T epsilon = 1e-8;

        struct UnitData
        {
            Coefficients<T> &coefficients;
            Array<T> firstMoment;
            Array<T> secondMoment;
            long step = 0;
        };

        std::vector<UnitData> mUnitDataList;

    public:
        Adam(const T beta1 = 0.9, const T beta2 = 0.999, const T epsilon = 1e-8) : beta1(beta1), beta2(beta2), epsilon(epsilon) {}

        void addUnit(Coefficients<T> &coefficients) override
        {
            mUnitDataList.push_back(UnitData{coefficients, Array<T>::constant(coefficients.refWildcardShape(), 0), Array<T>::constant(coefficients.refWildcardShape(), 0), 0});
        }

        struct FirstMomentCombo
        {
            const float param = 0;
            const Simd::Vector<T> simdParam;

            FirstMomentCombo(T lambda) : param(lambda), simdParam(Simd::broadcast_set<T>(lambda)) {}

            static inline T f(const T lambda, const T momentum, const T gradient)
            {
                return lambda * gradient + (1 - lambda) * momentum;
            }

            static inline Simd::Vector<T> fSimd(const Simd::Vector<T> lambda, const Simd::Vector<T> momentum, const Simd::Vector<T> gradient)
            {
                return lambda * gradient + (Simd::broadcast_set<T>(1) - lambda) * momentum;
            }
        };

        struct SecondMomentCombo
        {
            const float param = 0;
            const Simd::Vector<T> simdParam;

            SecondMomentCombo(T lambda) : param(lambda), simdParam(Simd::broadcast_set<T>(lambda)) {}

            static inline T f(const T lambda, const T momentum, const T gradient)
            {
                return lambda * gradient * gradient + (1 - lambda) * momentum;
            }

            static inline Simd::Vector<T> fSimd(const Simd::Vector<T> lambda, const Simd::Vector<T> momentum, const Simd::Vector<T> gradient)
            {
                return lambda * gradient * gradient + (Simd::broadcast_set<T>(1) - lambda) * momentum;
            }
        };

        struct UpdateWeights
        {
            struct Param
            {
                const T learningRate;
                const T eps;

                Param(const T learningRate, const T eps) : learningRate(learningRate), eps(eps) {}
            };

            struct SimdParam
            {
                const Simd::Vector<T> learningRate;
                const Simd::Vector<T> eps;

                SimdParam(const T learningRate, const T eps) : learningRate(Simd::broadcast_set<T>(learningRate)), eps(Simd::broadcast_set<T>(eps)) {}
            };

            const Param param = 0;
            const SimdParam simdParam;

            UpdateWeights(const T learningRate, const T eps) : param(learningRate, eps), simdParam(learningRate, eps) {}

            static inline T f(const Param param, const T weights, const T firstMoment, const T secondMoment)
            {
                return weights - param.learningRate * firstMoment / (std::sqrt(secondMoment) + param.eps);
            }

            static inline Simd::Vector<T> fSimd(const SimdParam param, const Simd::Vector<T> weights, const Simd::Vector<T> firstMoment, const Simd::Vector<T> secondMoment)
            {
                return weights - param.learningRate * firstMoment / (Simd::sqrt<float>(secondMoment) + param.eps);
            }
        };

        void update(T learningRate) override
        {
            for (UnitData data : mUnitDataList)
            {
                data.step++;
                const auto &g = data.coefficients.refGradient();

                T beta1Pow = std::pow(beta1, data.step);
                T gamma1 = (1 - beta1) / (1 - beta1Pow);
                T beta2Pow = std::pow(beta2, data.step);
                T gamma2 = (1 - beta2) / (1 - beta2Pow);

                computeInPlace<FirstMomentCombo, true, true>(FirstMomentCombo(gamma1), data.firstMoment, data.firstMoment, g);
                computeInPlace<SecondMomentCombo, true, true>(SecondMomentCombo(gamma2), data.secondMoment, data.secondMoment, g);
                auto &w = data.coefficients.refCoefficientArray();
                computeInPlace<UpdateWeights, true, true, true>(UpdateWeights(learningRate, epsilon), w, w, data.firstMoment, data.secondMoment);
            }
        }
    };

    template <DataType T>
        requires std::is_floating_point_v<T>
    class NaiveAdam : public Optimizer<T>
    {
        const T beta1 = 0.9;
        const T beta2 = 0.999;
        const T epsilon = 1e-8;

        struct UnitData
        {
            Coefficients<T> &coefficients;
            Array<T> firstMoment;
            Array<T> secondMoment;
            long step = 0;
        };

        std::vector<UnitData> mUnitDataList;

    public:
        NaiveAdam(const T beta1 = 0.9, const T beta2 = 0.999, const T epsilon = 1e-8) : beta1(beta1), beta2(beta2), epsilon(epsilon) {}

        void addUnit(Coefficients<T> &coefficients) override
        {
            mUnitDataList.push_back(UnitData{coefficients, Array<T>::constant(coefficients.refWildcardShape(), 0), Array<T>::constant(coefficients.refWildcardShape(), 0), 0});
        }

        void update(T learningRate) override
        {
            for (UnitData data : mUnitDataList)
            {
                data.step++;
                const auto &g = data.coefficients.refGradient();

                T beta1Pow = std::pow(beta1, data.step);
                T gamma1 = (1 - beta1) / (1 - beta1Pow);
                T beta2Pow = std::pow(beta2, data.step);
                T gamma2 = (1 - beta2) / (1 - beta2Pow);

                data.firstMoment = (1 - gamma1) * data.firstMoment + gamma1 * g;
                data.secondMoment = (1 - gamma2) * data.secondMoment + gamma2 * g.square();

                data.coefficients.refCoefficientArray() -= learningRate * data.firstMoment / (data.secondMoment.sqrt() + epsilon);
            }
        }
    };
}

#endif
