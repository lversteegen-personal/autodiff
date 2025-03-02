#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "array.hpp"

class RandomArrayGenerator
{
    static std::random_device randomDevice;
    std::default_random_engine mRandomEngine;

public:
    RandomArrayGenerator() : mRandomEngine(randomDevice()) {}

    RandomArrayGenerator(long seed) : mRandomEngine(seed) {}

    Array<double> uniform(const Coordinates &shape, double low = 0.0, double high = 1.0)
    {
        if (low >= high)
        {
            high = low;
            low = 0.0;
        }

        std::uniform_real_distribution<double> distribution(low, high);
        auto data = Data<double>(Array<double>::calculateFlatLength(shape));
        for (size_t i = 0; i < data.size(); i++)
            data[i] = distribution(mRandomEngine);

        return Array<double>(data, shape);
    }

    Array<double> normal(const Coordinates &shape, double mean = 0.0, double std = 1.0)
    {
        std::normal_distribution<double> distribution(mean, std);
        auto data = Data<double>(Array<double>::calculateFlatLength(shape));
        for (size_t i = 0; i < data.size(); i++)
            data[i] = distribution(mRandomEngine);

        return Array<double>(data, shape);
    }

    template <typename T>
    /// @brief Draws an array of random integers from a uniform distribution.
    /// @param shape The shape of the resulting array.
    /// @param low The inclusive lower bound of the distribution.
    /// @param high The exclusive upper bound of the distribution.
    /// @return The random array.
    Array<T> uniformInteger(const Coordinates &shape, long low, long high)
    {
        if (low >= high)
        {
            high = low;
            low = 0;
        }

        std::uniform_int_distribution<long> distribution(low, high-1);
        auto data = Data<T>(Array<T>::calculateFlatLength(shape));
        for (size_t i = 0; i < data.size(); i++)
            data[i] = distribution(mRandomEngine);

        return Array<T>(data, shape);
    }

    template <typename T>
    /// @brief Draws an array of random integers from a uniform distribution.
    /// @param shape The shape of the resulting array.
    /// @param high The exclusive upper bound of the distribution.
    /// @return The random array.
    Array<T> uniformInteger(const Coordinates &shape, long high =2)
    {
        return uniformInteger<T>(shape, 0, high);
    }

    template <typename T>
    Array<T> binomial(const Coordinates &shape, long trials, double probability)
    {
        std::binomial_distribution<long> distribution(trials, probability);
        auto data = Data<T>(Array<T>::calculateFlatLength(shape));
        for (size_t i = 0; i < data.size(); i++)
            data[i] = distribution(mRandomEngine);

        return Array<long>(data, shape);
    }

    template <typename T>
    Array<long> poisson(const Coordinates &shape, double lambda)
    {
        std::poisson_distribution<long> distribution(lambda);
        auto data = Data<long>(Array<long>::calculateFlatLength(shape));
        for (size_t i = 0; i < data.size(); i++)
            data[i] = distribution(mRandomEngine);

        return Array<long>(data, shape);
    }
};

std::random_device RandomArrayGenerator::randomDevice;

#endif