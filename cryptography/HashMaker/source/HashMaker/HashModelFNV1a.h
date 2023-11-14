#pragma once

#include "HashModel.h"
#include "Genome.h"

std::unique_ptr<IHashModel> CreateHashModelFNV1a();
