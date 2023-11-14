#include "HashModelFNV1a.h"

class HashModelFNV1a : public IHashModel
{
    static constexpr uint32_t k_OffsetBasis = 0x811c9dc5;
    static constexpr uint32_t k_Prime = 16777619;

public:
    std::vector<uint8_t> _data;

    virtual void reset() override final
    {
        _data.resize(4);

        uint32_t& hash = *reinterpret_cast<uint32_t*>(_data.data());
        hash = k_OffsetBasis;
    }

    virtual void update(const void* data, size_t len) override final
    {
        uint32_t& hash = *reinterpret_cast<uint32_t*>(_data.data());
        for (size_t i = 0; i < len; i++)
        {
            hash = hash ^ ((const uint8_t*)data)[i];
            hash = hash * k_Prime;
        }
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

std::unique_ptr<IHashModel> CreateHashModelFNV1a()
{
    return std::make_unique<HashModelFNV1a>();
}
