#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include "HashContext.h"
#include "Evaluator.h"
#include "Random.h"
#include "Operator.h"
#include "Genome.h"
#include "Params.h"

class HashMaker
{
public:
    HashMaker();

    void reset(const HashMakerParams& params);

    void evaluate();
    void evaluatePopulation();
    void epoch();
    void printStats();

private:
    
    void initializeGenome(Genome_t& genome);
    void randomizeGenomeOperators(Genome_t& genome);
    void evaluateGenome(Genome_t& genome);
    void epoch1();

    void crossover(Genome_t& child, const Genome_t& parentA, const Genome_t& parentB);
    void mutate(Genome_t& genome);

private:
    Random _random;
    HashMakerParams _parameters;
    Evaluator _evaluator;
    Genome_t _bestSolution;
    std::vector<Genome_t> _population;
    size_t _generation;
};
