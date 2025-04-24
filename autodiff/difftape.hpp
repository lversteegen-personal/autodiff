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

#include "../array/array_library.hpp"
#include "performance.hpp"

using namespace ArrayLibrary;

template <DataType T>
class Unit;

class Test;

template <DataType T>
class DiffTape
{
    friend int main();
    friend Test;
    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

private:
    std::vector<Unit<T> *> mUnits;
    std::unordered_map<Unit<T> *, long> mOrder;

    const bool mEager = false;
    long mCalcProgress = -1;
    Unit<T> *pGradientTarget = nullptr;

    std::vector<PerformanceMeasure> mCalcPerformanceMeasures;
    std::vector<PerformanceMeasure> mGradientPerformanceMeasures;
    bool mMeasurePerformance = false;

public:
    DiffTape() = default;
    // DiffTape<T>(bool eager) : mEager(eager) {}

    ~DiffTape()
    {
        for (auto *pUnit : mUnits)
            delete pUnit;
    }

    void reset()
    {
        mCalcProgress = -1;
        pGradientTarget = nullptr;
    }

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

    void addVariable(Unit<T> *px)
    {
        mOrder[px] = mUnits.size();
        mUnits.push_back(px);
        pGradientTarget = nullptr;
    }

    Array<T> getValue(Unit<T> &unit)
    {
        long position = mOrder[&unit];
        if (mCalcProgress < position)
        {
            for (long i = mCalcProgress + 1; i <= position; i++)
                mUnits[i]->calculate();
            mCalcProgress = position;
        }

        return Array<T>(unit.mArray);
    }

    void calculateAll(Unit<T> &target)
    {
        if (mOrder.find(&target) == mOrder.end())
            throw std::invalid_argument("Output not found in the tape!");

        for (long i = 0; i < mUnits.size(); i++)
        {
            if (mMeasurePerformance)
            {
                mCalcPerformanceMeasures[i].start();
                mUnits[i]->calculate();
                mCalcPerformanceMeasures[i].stop();
            }
            else
                mUnits[i]->calculate();
            // std::cout << "Calculating unit " << typeid(*mUnits[i]).name() << std::endl;
            // std::cout << mUnits[i]->refArrayShape() << std::endl;
        }
        mCalcProgress = mUnits.size();

        pGradientTarget = &target;
        for (long i = mUnits.size() - 1; i >= 0; i--)
            mUnits[i]->resetGradient();

        target.initDiff();

        for (long i = mUnits.size() - 1; i >= 0; i--)
        {
            if (mMeasurePerformance)
            {
                mGradientPerformanceMeasures[i].start();
                mUnits[i]->pullGradient();
                mGradientPerformanceMeasures[i].stop();
            }
            else
                mUnits[i]->pullGradient();
        }
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
            for (long i = mCalcProgress + 1; i <= outputPosition; i++)
            {
                if (mMeasurePerformance)
                {
                    mCalcPerformanceMeasures[i].start();
                    mUnits[i]->calculate();
                    mCalcPerformanceMeasures[i].stop();
                }
                else
                    mUnits[i]->calculate();
                // std::cout << "Calculating unit " << typeid(*mUnits[i]).name() << std::endl;
                // std::cout << mUnits[i]->refArrayShape() << std::endl;
            }
            mCalcProgress = outputPosition;
        }

        if (outputPosition < inputPosition)
            return Array<T>::constant(input.refArrayShape(), 0);

        if (pGradientTarget != &output)
        {
            pGradientTarget = &output;
            for (long i = outputPosition; i >= 0; i--)
                mUnits[i]->resetGradient();

            output.initDiff();

            for (long i = outputPosition; i >= 0; i--)
            {
                if (mMeasurePerformance)
                {
                    mGradientPerformanceMeasures[i].start();
                    mUnits[i]->pullGradient();
                    mGradientPerformanceMeasures[i].stop();
                }
                else
                    mUnits[i]->pullGradient();
            }
        }

        return input.mGradient;
    }
};

#endif