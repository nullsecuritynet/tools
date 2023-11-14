#pragma once

#include "HashModel.h"
#include "Genome.h"

std::unique_ptr<IHashModel> CreateHashModelGenome(Genome_t& genome);
