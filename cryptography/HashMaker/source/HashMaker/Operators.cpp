#include "Operator.h"
#include "Genome.h"

#include <assert.h>
#include <intrin.h>

enum HashOperatorTypes : size_t
{
    HashOperatorTypeStateMovInput = 0,
    HashOperatorTypeStateMovMagic,
    HashOperatorTypeStateRolMagic,
    HashOperatorTypeStateRorMagic,
    HashOperatorTypeStateMulMagic,
    HashOperatorTypeStateAndMagic,
    HashOperatorTypeStateShlMagic,
    HashOperatorTypeStateShrMagic,
    HashOperatorTypeStateOrMagic,
    HashOperatorTypeStateXorMagic,
    HashOperatorTypeStateXorInput,
    HashOperatorTypeStateAndInput,
    HashOperatorTypeStateAddInput,
    HashOperatorTypeStateSubInput,
    HashOperatorTypeStateMovState,
    HashOperatorTypeStateXorState,
    HashOperatorTypeStateSubState,
    HashOperatorTypeStateAddState,
    HashOperatorTypeStateNot,
};

template<typename T>
inline size_t mutateOffset(size_t offset, HashMakerParams& params, Random& random)
{
    if (params.hashSize > sizeof(T))
    {
        size_t offsetRange = params.hashSize - sizeof(T);
        offset = random.randomIntegerRange<size_t>(0, offsetRange);
#ifdef _DEBUG
        if (offset + sizeof(T) > params.hashSize)
        {
            __debugbreak();
        }
#endif
        return offset;
    }
    return offset;
}

class HashOperatorBase
{
protected:
    void markWritten(HashContext_t& context, size_t offset, size_t len)
    {
        for (size_t i = 0; i < len; i++)
            context.dataFlags[offset + i] |= HashContext_t::k_DataFlagWrite;
    }
    void markRead(HashContext_t& context, size_t offset, size_t len)
    {
        for (size_t i = 0; i < len; i++)
            context.dataFlags[offset + i] |= HashContext_t::k_DataFlagRead;
    }

    template<typename T>
    void write(HashContext_t& context, size_t offset, T val)
    {
        *reinterpret_cast<T*>(context.data.data() + offset) = val;
        markWritten(context, offset, sizeof(T));
    }

    template<typename T>
    T read(HashContext_t& context, size_t offset)
    {
        markRead(context, offset, sizeof(T));
        return *reinterpret_cast<T*>(context.data.data() + offset);
    }

    bool isOffsetValid(HashContext_t& context, size_t offset, size_t len)
    {
        return offset + len <= context.data.size();
    }
};

template<typename T>
class HashOperatorStateMovInput : public IHashOperator, HashOperatorBase
{
    size_t _offset;
public:
    enum {
        k_Type = HashOperatorTypeStateMovInput
    };
    HashOperatorStateMovInput(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        write<T>(context, _offset, context.currentInput);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMovInput<T>>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V[%zu] = IN", _offset);
        return str;
    }
};

template<typename T>
class HashOperatorStateRolMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateRolMagic
    };
    HashOperatorStateRolMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    template<typename _T> struct RotateLeft;

    template<> struct RotateLeft<uint8_t> {
        static uint8_t rotate(uint8_t val, int len) {
            return _rotl8(val, len);
        }
    };

    template<> struct RotateLeft<uint16_t> {
        static uint16_t rotate(uint16_t val, int len) {
            return _rotl16(val, len);
        }
    };

    template<> struct RotateLeft<uint32_t> {
        static uint32_t rotate(uint32_t val, int len) {
            return _rotl(val, len);
        }
    };

    template<> struct RotateLeft<uint64_t> {
        static uint64_t rotate(uint64_t val, int len) {
            return _rotl64(val, len);
        }
    };

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val = RotateLeft<T>::rotate(val, (int)_value);
        write<T>(context, _offset, _value);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateRolMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] ROL %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateRorMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateRolMagic
    };
    HashOperatorStateRorMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    template<typename _T> struct RotateRight;

    template<> struct RotateRight<uint8_t> {
        static uint8_t rotate(uint8_t val, int len) {
            return _rotr8(val, len);
        }
    };

    template<> struct RotateRight<uint16_t> {
        static uint16_t rotate(uint16_t val, int len) {
            return _rotr16(val, len);
        }
    };

    template<> struct RotateRight<uint32_t> {
        static uint32_t rotate(uint32_t val, int len) {
            return _rotr(val, len);
        }
    };

    template<> struct RotateRight<uint64_t> {
        static uint64_t rotate(uint64_t val, int len) {
            return _rotr64(val, len);
        }
    };

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val = RotateRight<T>::rotate(val, (int)_value);
        write<T>(context, _offset, _value);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateRorMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] ROR %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateMovMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateMovMagic
    };
    HashOperatorStateMovMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    virtual void run(HashContext_t& context) override
    {
        write<T>(context, _offset, _value);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMovMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] *= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateMulMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateMulMagic
    };
    HashOperatorStateMulMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val *= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMulMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] *= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateAndMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateAndMagic
    };
    HashOperatorStateAndMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val &= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAndMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] &= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateShlMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;

public:
    enum {
        k_Type = HashOperatorTypeStateShlMagic
    };
    HashOperatorStateShlMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value) / 4) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val <<= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, 32);
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateShlMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] <<= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateShrMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateShrMagic
    };
    HashOperatorStateShrMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value) / 4) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val >>= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, 32);
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateShrMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] >>= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateOrMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateOrMagic
    };
    HashOperatorStateOrMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val |= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateOrMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] |= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateXorMagic : public IHashOperator, HashOperatorBase
{
    size_t _offset;
    T _value;
public:
    enum {
        k_Type = HashOperatorTypeStateXorMagic
    };
    HashOperatorStateXorMagic(size_t offset, uint64_t value) : _offset(offset), _value(T(value)) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val ^= _value;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);

        uint64_t val = random.randomIntegerRange<uint64_t>(0, std::numeric_limits<T>::max());
        _value = (T)val;
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateXorMagic<T>>(_offset, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] ^= %llu", sizeof(T) * 8, _offset, (uint64_t)_value);
        return str;
    }
};

template<typename T>
class HashOperatorStateXorInput : public IHashOperator, HashOperatorBase
{
    size_t _offset;

public:
    enum {
        k_Type = HashOperatorTypeStateXorInput
    };
    HashOperatorStateXorInput(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val ^= static_cast<T>(context.currentInput);
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateXorInput<T>>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] ^= IN", sizeof(T) * 8, _offset);
        return str;
    }
};

template<typename T>
class HashOperatorStateAndInput : public IHashOperator, HashOperatorBase
{
    size_t _offset;

public:
    enum {
        k_Type = HashOperatorTypeStateAndInput
    };
    HashOperatorStateAndInput(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val &= static_cast<T>(context.currentInput);
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAndInput>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] &= IN", sizeof(T) * 8, _offset);
        return str;
    }
};

template<typename T>
class HashOperatorStateAddInput : public IHashOperator, HashOperatorBase
{
    size_t _offset;

public:
    enum {
        k_Type = HashOperatorTypeStateAddInput
    };
    HashOperatorStateAddInput(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val += static_cast<T>(context.currentInput);
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAddInput<T>>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] += IN", sizeof(T) * 8, _offset);
        return str;
    }
};

template<typename T>
class HashOperatorStateSubInput : public IHashOperator, HashOperatorBase
{
    size_t _offset;

public:
    enum {
        k_Type = HashOperatorTypeStateSubInput
    };
    HashOperatorStateSubInput(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset);
        val -= static_cast<T>(context.currentInput);
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateSubInput<T>>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] -= IN", sizeof(T) * 8, _offset);
        return str;
    }
};

template<typename T>
class HashOperatorStateMovState : public IHashOperator, HashOperatorBase
{
    size_t _offset1;
    size_t _offset2;
public:
    enum {
        k_Type = HashOperatorTypeStateMovState,
    };
    HashOperatorStateMovState(size_t offset1, size_t offset2) : _offset1(offset1), _offset2(offset2) {}

    virtual void run(HashContext_t& context) override
    {
        T val = read<T>(context, _offset2);
        write<T>(context, _offset1, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset1 = mutateOffset<T>(_offset1, params, random);
        _offset2 = mutateOffset<T>(_offset1, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset1, sizeof(T)) &&
            isOffsetValid(context, _offset2, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMovState<T>>(_offset1, _offset2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] = V%zu[%zu]", sizeof(T) * 8, _offset1, sizeof(T) * 8, _offset2);
        return str;
    }
};

template<typename T>
class HashOperatorStateXorState : public IHashOperator, HashOperatorBase
{
    size_t _offset1;
    size_t _offset2;
public:
    enum {
        k_Type = HashOperatorTypeStateXorState
    };
    HashOperatorStateXorState(size_t offset1, size_t offset2) : _offset1(offset1), _offset2(offset2) {}

    virtual void run(HashContext_t& context) override
    {
        T val1 = read<T>(context, _offset1);
        T val2 = read<T>(context, _offset2);
        val1 ^= val2;
        write<T>(context, _offset1, val1);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset1 = mutateOffset<T>(_offset1, params, random);
        _offset2 = mutateOffset<T>(_offset2, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset1, sizeof(T)) &&
            isOffsetValid(context, _offset2, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateXorState<T>>(_offset1, _offset2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] ^= V%zu[%zu]", sizeof(T) * 8, _offset1, sizeof(T) * 8, _offset2);
        return str;
    }
};

template<typename T>
class HashOperatorStateSubState : public IHashOperator, HashOperatorBase
{
    size_t _offset1;
    size_t _offset2;
public:
    enum {
        k_Type = HashOperatorTypeStateSubState
    };
    HashOperatorStateSubState(size_t offset1, size_t offset2) : _offset1(offset1), _offset2(offset2) {}

    virtual void run(HashContext_t& context) override
    {
        T val1 = read<T>(context, _offset1);
        T val2 = read<T>(context, _offset2);
        val1 -= val2;
        write<T>(context, _offset1, val1);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset1 = mutateOffset<T>(_offset1, params, random);
        _offset2 = mutateOffset<T>(_offset2, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset1, sizeof(T)) &&
            isOffsetValid(context, _offset2, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateSubState<T>>(_offset1, _offset2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] -= V%zu[%zu]", sizeof(T) * 8, _offset1, sizeof(T) * 8, _offset2);
        return str;
    }
};

template<typename T>
class HashOperatorStateAddState : public IHashOperator, HashOperatorBase
{
    size_t _offset1;
    size_t _offset2;
public:
    enum {
        k_Type = HashOperatorTypeStateAddState
    };
    HashOperatorStateAddState(size_t offset1, size_t offset2) : _offset1(offset1), _offset2(offset2) {}

    virtual void run(HashContext_t& context) override
    {
        T val1 = read<T>(context, _offset1);
        T val2 = read<T>(context, _offset2);
        val1 += val2;
        write<T>(context, _offset1, val1);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset1 = mutateOffset<T>(_offset1, params, random);
        _offset2 = mutateOffset<T>(_offset2, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset1, sizeof(T)) &&
            isOffsetValid(context, _offset2, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAddState<T>>(_offset1, _offset2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] += V%zu[%zu]", sizeof(T) * 8, _offset1, sizeof(T) * 8, _offset2);
        return str;
    }
};

template<typename T>
class HashOperatorStateNot : public IHashOperator, HashOperatorBase
{
    size_t _offset;
public:
    enum {
        k_Type = HashOperatorTypeStateNot
    };
    HashOperatorStateNot(size_t offset) : _offset(offset) {}

    virtual void run(HashContext_t& context) override
    {
        T val= read<T>(context, _offset);
        val = ~val;
        write<T>(context, _offset, val);
    }

    virtual void mutate(HashMakerParams& params, Random& random) override
    {
        _offset = mutateOffset<T>(_offset, params, random);
    }

    virtual bool isValid(HashContext_t& context)
    {
        return isOffsetValid(context, _offset, sizeof(T));
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateNot<T>>(_offset);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "V%zu[%zu] = ~V%zu[%zu]", sizeof(T) * 8, _offset, sizeof(T) * 8, _offset);
        return str;
    }
};

constexpr std::pair<size_t, double> k_Operators[] = 
{
    { HashOperatorTypeStateMovInput, 1.0 },
    { HashOperatorTypeStateMovMagic, 1.0 },
    { HashOperatorTypeStateRolMagic, 1.0 },
    { HashOperatorTypeStateRorMagic, 1.0 },
    { HashOperatorTypeStateMulMagic, 1.0 },
    { HashOperatorTypeStateAndMagic, 1.0 },
    { HashOperatorTypeStateShlMagic, 1.0 },
    { HashOperatorTypeStateShrMagic, 1.0 },
    { HashOperatorTypeStateOrMagic, 1.0 },
    { HashOperatorTypeStateXorMagic, 1.0 },
    { HashOperatorTypeStateXorInput, 1.0 },
    { HashOperatorTypeStateAndInput, 1.0 },
    { HashOperatorTypeStateAddInput, 1.0 },
    { HashOperatorTypeStateSubInput, 1.0 },
    { HashOperatorTypeStateMovState, 1.0 },
    { HashOperatorTypeStateXorState, 1.0 },
    { HashOperatorTypeStateSubState, 1.0 },
    { HashOperatorTypeStateAddState, 1.0 },
    { HashOperatorTypeStateNot, 1.0 },
};

static std::vector<HashOperatorTypes>& GetOperatorList()
{
    static std::vector<HashOperatorTypes> res;
    if (res.empty())
    {
        for (size_t i = 0; i < std::size(k_Operators); i++)
        {
            size_t count = (size_t)std::ceil(k_Operators[i].second * 100.0);
            for (size_t n = 0; n < count; n++)
            {
                res.push_back((HashOperatorTypes)k_Operators[i].first);
            }
        }
    }
    return res;
}

template<template<typename> class C, typename... Args>
inline std::unique_ptr<IHashOperator> createOperator(Random& random, Args&&... args)
{
    std::unique_ptr<IHashOperator> op;

    switch (random.randomIntegerRange(0, 3))
    {
    case 0:
        return std::make_unique<C<uint8_t>>(std::forward<Args>(args)...);
    case 1:
        return std::make_unique<C<uint16_t>>(std::forward<Args>(args)...);
    case 2:
        return std::make_unique<C<uint32_t>>(std::forward<Args>(args)...);
    case 3:
        return std::make_unique<C<uint64_t>>(std::forward<Args>(args)...);
    default:
        assert(false);
    }

    return op;
}

static size_t getRandomOffset(const HashMakerParams& params, Random& random)
{
    return random.randomIntegerRange(params.hashSize - 1);
}

std::unique_ptr<IHashOperator> CreateRandomOperator(const HashMakerParams& params, Random& random)
{
    const std::vector<HashOperatorTypes>& operators = GetOperatorList();

    std::unique_ptr<IHashOperator> op;

    const HashOperatorTypes& opType = random.randomElement(operators);
    switch (opType)
    {
    case HashOperatorTypeStateMovInput:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateMovInput>(random, offset);
        }
        break;
    case HashOperatorTypeStateMovMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0x00, std::numeric_limits<uint64_t>::max());
            op = createOperator<HashOperatorStateMovMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateRolMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0, 64);
            op = createOperator<HashOperatorStateRolMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateRorMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0, 64);
            op = createOperator<HashOperatorStateRorMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateMulMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0x02, std::numeric_limits<uint64_t>::max());
            op = createOperator<HashOperatorStateMulMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateAndMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0x01, std::numeric_limits<uint64_t>::max());
            op = createOperator<HashOperatorStateAndMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateShlMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint8_t value = random.randomIntegerRange(4, 32);
            op = createOperator<HashOperatorStateShlMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateShrMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint8_t value = random.randomIntegerRange(4, 32);
            op = createOperator<HashOperatorStateShrMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateOrMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0x01, std::numeric_limits<uint64_t>::max());
            op = createOperator<HashOperatorStateOrMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateXorMagic:
        {
            size_t offset = getRandomOffset(params, random);
            uint64_t value = random.randomIntegerRange<uint64_t>(0x00, std::numeric_limits<uint64_t>::max());
            op = createOperator<HashOperatorStateXorMagic>(random, offset, value);
        }
        break;
    case HashOperatorTypeStateXorInput:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateXorInput>(random, offset);
        }
        break;
    case HashOperatorTypeStateAndInput:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateAndInput>(random, offset);
        }
        break;
    case HashOperatorTypeStateAddInput:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateAddInput>(random, offset);
        }
        break;
    case HashOperatorTypeStateSubInput:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateSubInput>(random, offset);
        }
        break;
    case HashOperatorTypeStateMovState:
        {
            size_t offset1;
            size_t offset2;
            do
            {
                offset1 = getRandomOffset(params, random);
                offset2 = getRandomOffset(params, random);
            } while (offset1 == offset2);
            op = createOperator<HashOperatorStateMovState>(random, offset1, offset2);
        }
        break;
    case HashOperatorTypeStateXorState:
        {
            size_t offset1;
            size_t offset2;
            do
            {
                offset1 = getRandomOffset(params, random);
                offset2 = getRandomOffset(params, random);
            } while (offset1 == offset2);
            op = createOperator<HashOperatorStateXorState>(random, offset1, offset2);
        }
        break;
    case HashOperatorTypeStateSubState:
        {
            size_t offset1;
            size_t offset2;
            do
            {
                offset1 = getRandomOffset(params, random);
                offset2 = getRandomOffset(params, random);
            } while (offset1 == offset2);
            op = createOperator<HashOperatorStateSubState>(random, offset1, offset2);
        }
        break;
    case HashOperatorTypeStateAddState:
        {
            size_t offset1;
            size_t offset2;
            do
            {
                offset1 = getRandomOffset(params, random);
                offset2 = getRandomOffset(params, random);
            } while (offset1 == offset2);
            op = createOperator<HashOperatorStateAddState>(random, offset1, offset2);
        }
        break;
    case HashOperatorTypeStateNot:
        {
            size_t offset = getRandomOffset(params, random);
            op = createOperator<HashOperatorStateNot>(random, offset);
        }
        break;
    default:
        assert(false);
        break;
    }

    return op;
}

void CreateOperators(const HashMakerParams& params, Genome_t& genome, Random& random)
{
    HashContext_t ctx(params.hashSize);

    size_t count = random.randomIntegerRange<size_t>(2, params.hashSize * 4);
    while(genome.operators.size() < count)
    {
        std::unique_ptr<IHashOperator> op = CreateRandomOperator(params, random);

        // Only allow valid operators.
        if(op->isValid(ctx) == false)
            continue;

        genome.operators.emplace_back(std::move(op));
    }
}
