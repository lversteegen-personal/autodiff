#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "array_library.hpp"

namespace ArrayLibrary
{
    class RandomArrayGenerator
    {
        static std::random_device randomDevice;
        std::default_random_engine mRandomEngine;

    public:
        RandomArrayGenerator() : mRandomEngine(randomDevice()) {}

        RandomArrayGenerator(long seed) : mRandomEngine(seed) {}

        template <DataType T> requires std::is_floating_point_v<T>
        Array<T> uniform(const Coordinates &shape, T low = 0.0, T high = 1.0)
        {
            if (low >= high)
            {
                high = low;
                low = 0.0;
            }

            std::uniform_real_distribution<T> distribution(low, high);
            auto data = Data<T>(Array<T>::calculateFlatLength(shape));
            for (long i = 0; i < data.size(); i++)
                data[i] = distribution(mRandomEngine);

            return Array<T>(data, shape);
        }

        template <DataType T>
            requires std::is_floating_point_v<T>
        Array<T> normal(const Coordinates &shape, T mean = 0.0, T std = 1.0)
        {
            std::normal_distribution<T> distribution(mean, std);
            auto data = Data<T>(Array<T>::calculateFlatLength(shape));
            for (long i = 0; i < data.size(); i++)
                data[i] = distribution(mRandomEngine);

            return Array<T>(data, shape);
        }

        template <DataType T>
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

            std::uniform_int_distribution<long> distribution(low, high - 1);
            auto data = Data<T>(Array<T>::calculateFlatLength(shape));
            for (long i = 0; i < data.size(); i++)
                data[i] = distribution(mRandomEngine);

            return Array<T>(data, shape);
        }

        template <DataType T>
        /// @brief Draws an array of random integers from a uniform distribution.
        /// @param shape The shape of the resulting array.
        /// @param high The exclusive upper bound of the distribution.
        /// @return The random array.
        Array<T> uniformInteger(const Coordinates &shape, long high = 2)
        {
            return uniformInteger<T>(shape, 0, high);
        }

        template <DataType T>
        Array<T> binomial(const Coordinates &shape, long trials, double probability)
        {
            std::binomial_distribution<long> distribution(trials, probability);
            auto data = Data<T>(Array<T>::calculateFlatLength(shape));
            for (long i = 0; i < data.size(); i++)
                data[i] = distribution(mRandomEngine);

            return Array<long>(data, shape);
        }

        template <DataType T>
        Array<long> poisson(const Coordinates &shape, double lambda)
        {
            std::poisson_distribution<long> distribution(lambda);
            auto data = Data<long>(Array<long>::calculateFlatLength(shape));
            for (long i = 0; i < data.size(); i++)
                data[i] = distribution(mRandomEngine);

            return Array<long>(data, shape);
        }
    };

    std::random_device RandomArrayGenerator::randomDevice;
}
#endif