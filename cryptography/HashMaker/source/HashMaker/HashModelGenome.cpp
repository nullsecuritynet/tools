#include "HashModelGenome.h"
#include "HashContext.h"
#include "Operator.h"

class HashModelGenome : public IHashModel
{
public:
    HashContext_t _ctx;
    Genome_t& _genome;

    HashModelGenome(Genome_t& genome)
        : _ctx(genome.genomeSizeBytes), _genome(genome)
    {
    }

    virtual void reset() override final
    {
        _ctx.reset(_genome.genomeSizeBytes);
    }

    virtual void update(const void *data, size_t len) override final
    {
        for (size_t i = 0; i < len; i++)
        {
            _ctx.currentInput = ((const uint8_t*)data)[i];
            for (auto&& op : _genome.operators)
            {
                op->run(_ctx);
            }
        }
    }

    virtual size_t hashBitSize() const override final
    {
        return _ctx.data.size() * 8;
    }

    virtual size_t bitsRead() const override final
    {
        return _ctx.countReads() * 8;
    }

    virtual size_t bitsWrite() const override final
    {
        return _ctx.countWrites() * 8;
    }

    virtual const std::vector<uint8_t>& finish() override final
    {
        return _ctx.data;
    }
};

std::unique_ptr<IHashModel> CreateHashModelGenome(Genome_t& genome)
{
    return std::make_unique<HashModelGenome>(genome);
}
