#pragma once

#include <random>

class Random
{
    std::mt19937 _prng;

public:
    Random() = default;
    Random(std::mt19937::result_type seed) : _prng(seed) {}

    void seed(std::mt19937::result_type seed)
    {
        _prng.seed(seed);
    }

    template<typename T>
    size_t randomIndex(const T& container)
    {
        if(container.size() == 0)
            return 0;
        return randomIntegerRange<size_t>(0, container.size() - 1);
    }

    template<typename T>
    typename const T& randomElement(const std::vector<T>& container)
    {
        size_t idx = randomIndex(container);
        return container[idx];
    }

    template<typename T>
    T randomIntegerRange(T max)
    {
        std::uniform_int_distribution<T> dist(0, max);
        return dist(_prng);
    }

    template<typename T>
    T randomIntegerRange(T min, T max)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(_prng);
    }

    template<typename T = double>
    T randomRealRange(T min, T max)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(_prng);
    }

    bool randomChance(double normalizedPercentage)
    {
        return randomRealRange<double>(0.0, 1.0) < normalizedPercentage;
    }

    bool randomChance(float normalizedPercentage)
    {
        return randomRealRange<double>(0.0, 1.0) < normalizedPercentage;
    }

    bool flipCoin()
    {
        return randomChance(0.5);
    }

};
