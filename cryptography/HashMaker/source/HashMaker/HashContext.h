#pragma once

#include <vector>
#include <stdint.h>

struct HashContext_t
{
    enum { 
        k_DataFlagRead = (1 << 1), 
        k_DataFlagWrite = (1 << 2) 
    };

    explicit HashContext_t(size_t hashSize)
    {
        reset(hashSize);
    }

    std::vector<uint8_t> data;
    std::vector<uint8_t> dataFlags;
    uint8_t currentInput;

    void reset(size_t size)
    {
        data.resize(size);
        dataFlags.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = 0;
            dataFlags[i] = 0;
        }
    }

    size_t countReads() const
    {
        size_t res = 0;
        for (auto&& n : dataFlags)
        {
            if((n & k_DataFlagRead) != 0)
                res++;
        }
        return res;
    }

    size_t countWrites() const
    {
        size_t res = 0;
        for (auto&& n : dataFlags)
        {
            if ((n & k_DataFlagWrite) != 0)
                res++;
        }
        return res;
    }
};
