#include "Evaluator.h"
#include "Genome.h"
#include "Params.h"
#include "HashContext.h"
#include "Operator.h"
#include "TestData.h"
#include "BlockAllocator.h"
#include "HashModelGenome.h"

#include <assert.h>
#include <unordered_set>

struct BufferHash
{
    size_t operator()(const Buffer& data) const
    {
        if (data.size() == 8)
        {
            std::hash<uint64_t> hasher;
            return hasher(*reinterpret_cast<const uint64_t*>(data.data()));
        }
        else if (data.size() == 4)
        {
            std::hash<uint32_t> hasher;
            return hasher(*reinterpret_cast<const uint32_t*>(data.data()));
        }
        else if (data.size() == 2)
        {
            std::hash<uint16_t> hasher;
            return hasher(*reinterpret_cast<const uint16_t*>(data.data()));
        }

        std::hash<uint8_t> hasher;
        size_t seed = 0;
        for (auto&& elem : data)
        {
            seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct DataGenerator
{
    void resize(size_t n)
    {
        states.resize(n);
        state.resize(n);
    }

    const std::vector<uint8_t>& next()
    {
        std::vector<uint8_t> res;
        res.resize(states.size());

        int32_t n = (int32_t)states.size() - 1;
        int32_t updateIndex = n;

        while (n >= 0)
        {
            state[n] = (uint8_t)states[n];

            if (updateIndex == n)
            {
                states[n]++;
                if (states[n] > 0xFF)
                {
                    states[n] = 0;
                    updateIndex--;
                }
            }

            n--;
        }
        if (updateIndex < 0)
        {
            states.push_back(0);
            state.push_back(0);
        }

        return state;
    }

private:
    std::vector<int32_t> states;
    std::vector<uint8_t> state;
};

static std::vector<Buffer> CreateTestData(const HashMakerParams& params)
{
    std::vector<Buffer> data;
    return data;
}

Evaluator::Evaluator()
{
}

using HashCollection = std::unordered_set<Buffer, BufferHash, std::equal_to<Buffer>, BlockAllocator<Buffer>>;

Evaluator::Result_t Evaluator::evaluate(std::unique_ptr<IHashModel>& hashModel)
{
    Evaluator::Result_t res{};

    // Check if operators have effect via input first.
    {
        hashModel->reset();
        hashModel->update("\x00", 1);
        auto hash1 = hashModel->finish();

        hashModel->reset();
        hashModel->update("\xFF", 1);
        auto hash2 = hashModel->finish();

        if (hash1 == hash2)
        {
            // Input has no effect.
            return res;
        }
    }

    const TestData& testData = TestData::get();

    const double hashBitSize = (double)hashModel->hashBitSize();

    // Test against collisions.
    {
        HashCollection uniqueHashes;

        for (size_t testDataSample = 0; testDataSample < testData.size(); testDataSample++)
        {
            res.totalTests++;

            const TestData::Data& data = testData[testDataSample];
            const std::vector<uint8_t>& buffer = data.bytes;

            hashModel->reset();
            hashModel->update(buffer.data(), buffer.size());

            auto& hash = hashModel->finish();

            auto insertRes = uniqueHashes.insert(hash);
            if (insertRes.second == false)
            {
                res.totalCollisions++;
            }

            res.bitsTotal += hashBitSize;
            res.bitsWrite += hashModel->bitsWrite();
            res.bitsRead += hashModel->bitsRead();
        }
    }

    return res;
}
