#pragma once

#include "HashModel.h"
#include "Genome.h"

std::unique_ptr<IHashModel> CreateHashModelMurmur32();
