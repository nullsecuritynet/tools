#pragma once

#include <vector>
#include <memory>

class IHashOperator;

struct Genome_t
{
    std::vector<std::unique_ptr<IHashOperator>> operators;
    size_t genomeSizeBytes;
    double fitness = 0.0;

    // Statistics.
    double totalCollisions = 0.0;
    double collisionRate = 0.0;
    double stateUsage = 0.0;
};
