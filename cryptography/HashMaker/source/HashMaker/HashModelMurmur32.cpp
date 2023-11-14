#include "HashModelMurmur32.h"

class HashModelMurmur32 : public IHashModel
{
public:
    std::vector<uint8_t> _data;

    virtual void reset() override final
    {
        _data.resize(4);

        uint32_t& hash = *reinterpret_cast<uint32_t*>(_data.data());
        hash = 0;
    }

    virtual void update(const void* input, size_t len) override final
    {
        uint32_t& hash = *reinterpret_cast<uint32_t*>(_data.data());
        
        const uint8_t * data = (const uint8_t*)input;
        const int nblocks = len / 4;

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);

        for (int i = -nblocks; i; i++)
        {
            uint32_t k1 = blocks[i];

            k1 *= c1;
            k1 = _rotl(k1, 15);
            k1 *= c2;

            hash ^= k1;
            hash = _rotl(hash, 13);
            hash = hash * 5 + 0xe6546b64;
        }

        const uint8_t * tail = (const uint8_t*)(data + nblocks * 4);

        uint32_t k1 = 0;

        switch (len & 3)
        {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1; k1 = _rotl(k1, 15); k1 *= c2; hash ^= k1;
        };

        hash ^= len;
        hash ^= hash >> 16;
        hash *= 0x85ebca6b;
        hash ^= hash >> 13;
        hash *= 0xc2b2ae35;
        hash ^= hash >> 16;
    }

    virtual size_t hashBitSize() const override final
    {
        return 4 * 8;
    }

    virtual size_t bitsRead() const override final
    {
        return 4 * 8;
    }

    virtual size_t bitsWrite() const override final
    {
        return 4 * 8;
    }

    virtual const std::vector<uint8_t>& finish() override final
    {
        return _data;
    }
};

std::unique_ptr<IHashModel> CreateHashModelMurmur32()
{
    return std::make_unique<HashModelMurmur32>();
}
