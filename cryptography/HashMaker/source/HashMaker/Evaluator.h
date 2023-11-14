#pragma once

#include <vector>
#include <stdint.h>

#include "HashModel.h"

struct HashMakerParams;
struct Genome_t;

using Buffer = std::vector<uint8_t>;

class Evaluator
{
    std::vector<Buffer> _testData;

public:
    struct Result_t
    {
        double fitness = 0.0;
        double totalTests = 0.0;
        double totalCollisions = 0.0;
        double bitsRead = 0.0;
        double bitsWrite = 0.0;
        double bitsTotal = 0.0;
    };

public:
    Evaluator();

    Result_t evaluate(std::unique_ptr<IHashModel>& hashModel);

    size_t testSize() const;
};
