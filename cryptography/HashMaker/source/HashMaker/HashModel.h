#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

class IHashModel
{
public:
    virtual void reset() = 0;

    virtual void update(const void* data, size_t len) = 0;

    virtual size_t hashBitSize() const = 0;

    virtual size_t bitsRead() const = 0;

    virtual size_t bitsWrite() const = 0;

    virtual const std::vector<uint8_t>& finish() = 0;
};
