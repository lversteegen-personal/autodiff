#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <unordered_set>

#include "diff_unit.hpp"
#include "diff_matmul.hpp"
#include "diff_binary_ptws.hpp"
#include "diff_basic.hpp"
#include "optimizer.hpp"
#include "performance.hpp"

class Test;

namespace AutoDiff
{
    namespace NeuralNetworks
    {
        template <DataType T, typename OPT>
            requires std::is_base_of_v<Optimizer<T>, OPT>
        class Model
        {

            friend Test;

        protected:
            const std::vector<Variables<T> *> mVariables;
            Unit<T> &mCost;
            const std::vector<Unit<T> *> mUnits;
            OPT mOptimizer;

            std::vector<PerformanceMeasure> mCalcPerformanceMeasures;
            std::vector<PerformanceMeasure> mGradientPerformanceMeasures;
            bool mMeasurePerformance = false;

            static void gatherRecursion(Unit<T> &unit, std::unordered_set<Unit<T> *> &visited, std::vector<Unit<T> *> &units)
            {
                for (auto dependency : unit.getDependencies())
                {
                    if (visited.find(dependency) == visited.end())
                    {
                        visited.insert(dependency);
                        gatherRecursion(*dependency, visited, units);
                    }
                }
                units.push_back(&unit);
            }

            /// @brief Peforms a depth-first search to gather all units necessary to compute the desired output unit.
            /// @details The units are gathered in order of their dependencies. The output unit is included in the list, and no unit is included more than once even if more than one other unit depends on it.
            /// @param output Output unit of the model.
            /// @return The list of units that are necessary to compute the output unit.
            static std::vector<Unit<T> *> gatherUnits(Unit<T> &output)
            {
                std::unordered_set<Unit<T> *> visited;
                std::vector<Unit<T> *> units;

                gatherRecursion(output, visited, units);

                return units;
            }

            inline void setVariables(const std::vector<Array<T>> &variableValues, long batchStart, long batchEnd)
            {
                if (variableValues.size() != mVariables.size())
                    throw std::invalid_argument("Number of input values must match number of inputs.");

                for (long i = 0; i < variableValues.size(); i++)
                {
                    if (!wildcardRemovalCheck(mVariables[i]->refWildcardShape(), variableValues[i].refShape()))
                        throw std::invalid_argument("The shape of the input value does not match the wildcard shape of the input unit.");

                    mVariables[i]->setValue(variableValues[i].sliceAxis(mVariables[i]->wildcardDim, batchStart, batchEnd));
                }
            }

        public:
            void setMeasurePerformance(bool measurePerformance)
            {
                mMeasurePerformance = measurePerformance;
                if (measurePerformance)
                {
                    mCalcPerformanceMeasures.resize(mUnits.size(), PerformanceMeasure());
                    mGradientPerformanceMeasures.resize(mUnits.size(), PerformanceMeasure());
                }
            }

            bool getMeasurePerformance() const
            {
                return mMeasurePerformance;
            }

            std::chrono::microseconds getCalcPerformance(int i)
            {
                if (!mMeasurePerformance)
                    throw std::logic_error("Performance measurement is not enabled.");
                else if (i < 0 || i >= mCalcPerformanceMeasures.size())
                    throw std::out_of_range("Index out of range.");
                else
                    return std::chrono::duration_cast<std::chrono::microseconds>(mCalcPerformanceMeasures[i].accumulated);
            }

            std::chrono::microseconds getGradientPerformance(int i)
            {
                if (!mMeasurePerformance)
                    throw std::logic_error("Performance measurement is not enabled.");
                else if (i < 0 || i >= mGradientPerformanceMeasures.size())
                    throw std::out_of_range("Index out of range.");
                else
                    return std::chrono::duration_cast<std::chrono::microseconds>(mGradientPerformanceMeasures[i].accumulated);
            }

            Model(const std::vector<Variables<T> *> &variables, Unit<T> &cost, OPT optimizer) : mVariables(variables), mCost(cost), mUnits(std::move(gatherUnits(cost))), mOptimizer(optimizer)
            {
                if (variables.size() == 0)
                    throw std::invalid_argument("The model must have at least one variable.");
                for (auto variable : mVariables)
                {
                    if (variable->wildcardDim == -1)
                        throw std::invalid_argument("Variables must have a wildcard dimension.");
                }

                for (auto unit : mUnits)
                {
                    if (auto *coefficients = dynamic_cast<Coefficients<T> *>(unit))
                    {
                        mOptimizer.addUnit(*coefficients);
                    }
                }
            }

            Model() = delete;

            const Unit<T> &getCostUnit() const { return mCost; }

            inline Unit<T> &forwardPass()
            {
                if (mMeasurePerformance)
                {
                    for (long i = 0; i < mUnits.size(); i++)
                    {
                        mCalcPerformanceMeasures[i].start();
                        mUnits[i]->calculate();
                        mCalcPerformanceMeasures[i].stop();
                    }
                }
                else
                {
                    for (long i = 0; i < mUnits.size(); i++)
                        mUnits[i]->calculate();
                }

                return mCost;
            }

            inline void backwardPass()
            {
                for (long i = mUnits.size() - 1; i >= 0; i--)
                    mUnits[i]->resetGradient();

                mCost.initDiff();

                if (mMeasurePerformance)
                {
                    for (long i = mUnits.size() - 1; i >= 0; i--)
                    {
                        mGradientPerformanceMeasures[i].start();
                        mUnits[i]->pullGradient();
                        mGradientPerformanceMeasures[i].stop();
                    }
                }
                else
                {
                    for (long i = mUnits.size() - 1; i >= 0; i--)
                        mUnits[i]->pullGradient();
                }
            }

            void fit(const std::vector<Array<T>> &variableValues, long epochs, long batchSize, T learningRate = 1e-3, bool verbose = true)
            {
                if (variableValues.size() != mVariables.size())
                    throw std::invalid_argument("The number of passed values does not match the number of variables of the model.");

                long sampleSize = variableValues[0].refShape()[mVariables[0]->wildcardDim];
                for (long i = 0; i < mVariables.size(); i++)
                    if (variableValues[i].refShape()[mVariables[i]->wildcardDim] != sampleSize)
                        throw std::invalid_argument("The number of samples must be the same for all variable values.");

                T totalCost = 0;

                for (long epoch = 0; epoch < epochs; epoch++)
                {
                    totalCost = 0;
                    for (long batchStart = 0; batchStart < sampleSize; batchStart += batchSize)
                    {
                        long batchEnd = std::min(batchStart + batchSize, sampleSize);
                        setVariables(variableValues, batchStart, batchEnd);
                        passMeasure.start();
                        forwardPass();
                        backwardPass();
                        passMeasure.stop();
                        optMeasure.start();
                        mOptimizer.update(learningRate);
                        optMeasure.stop();
                        totalCost += mCost.refArray().eval();

                        if (verbose && batchStart % 256 < batchSize)
                        {
                            std::cout << "\r\t\r";
                            std::cout << batchEnd << "/" << sampleSize << " samples, mse: " << totalCost / batchEnd << "\t";
                        }
                    }
                    std::cout << "\r\t\r";
                    std::cout << "Epoch: " << epoch << ", mse: " << totalCost / sampleSize << std::endl;
                }
            }
        };
    }
}

#endif // MODEL_H