#pragma once

#include "HashContext.h"
#include "Random.h"
#include "Params.h"

struct Genome_t;

class IHashOperator
{
public:
    virtual void run(HashContext_t& context) = 0;
    virtual bool isValid(HashContext_t& context) = 0;

    virtual void mutate(HashMakerParams& params, Random& random) = 0;

    virtual std::unique_ptr<IHashOperator> clone() = 0;

    virtual std::string toString() = 0;
};

std::unique_ptr<IHashOperator> CreateRandomOperator(const HashMakerParams& params, Random& random);
void CreateOperators(const HashMakerParams& params, Genome_t& genome, Random& random);
