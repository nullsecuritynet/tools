#include <limits>
#include <iostream>
#include <memory>

static constexpr size_t BLOCK_SIZE = 0xFFFFF;

template<class _Ty>
class BlockAllocator
{
    struct Block
    {
        uintptr_t base;
        size_t used;
        size_t space;
    };

    std::vector<Block> _blocks;
    Block *_last = nullptr;

public:
    typedef BlockAllocator<_Ty> other;

    typedef _Ty value_type;

    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef void *void_pointer;
    typedef const void *const_void_pointer;

    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef size_t size_type;

    BlockAllocator<_Ty> select_on_container_copy_construction() const
    {	// return this MyAllocator
        return (*this);
    }

    template<class _Other>
    struct rebind
    {	// convert this type to MyAllocator<_Other>
        typedef BlockAllocator<_Other> other;
    };

    pointer address(reference _Val) const noexcept
    {	// return address of mutable _Val
        return (_STD addressof(_Val));
    }

    const_pointer address(const_reference _Val) const noexcept
    {	// return address of nonmutable _Val
        return (_STD addressof(_Val));
    }

    BlockAllocator()
    {	// construct default MyAllocator (do nothing)
    }

    BlockAllocator(const BlockAllocator<_Ty>&)
    {	// construct by copying (do nothing)
    }

    template<class _Other>
    BlockAllocator(const BlockAllocator<_Other>&)
    {	// construct from a related MyAllocator (do nothing)
    }

    template<class _Other>
    BlockAllocator<_Ty>& operator=(const BlockAllocator<_Other>&)
    {	// assign from a related MyAllocator (do nothing)
        return (*this);
    }

    ~BlockAllocator()
    {
        for (auto&& block : _blocks)
        {
            free((void*)block.base);
        }
    }

    void deallocate(pointer _Ptr, size_type)
    {	// deallocate object at _Ptr, ignore size
    }

    pointer allocate(size_type _Count)
    {	// allocate array of _Count elements
        size_t bytesRequired = sizeof(_Ty) * _Count;

        if (_last == nullptr || _last->used + bytesRequired >= _last->space)
        {
            size_t blockSize = BLOCK_SIZE;
            if (bytesRequired > BLOCK_SIZE)
                blockSize = bytesRequired;

            Block block;
            block.base = (uintptr_t)malloc(blockSize);
            block.space = blockSize;
            block.used = 0;

            _blocks.push_back(block);
            _last = &_blocks[_blocks.size() - 1];
        }

        pointer p = (pointer)(_last->base + _last->used);
        _last->used += bytesRequired;

        return p;
    }

    pointer allocate(size_type _Count, const void *)
    {	// allocate array of _Count elements, ignore hint
        return (allocate(_Count));
    }

    void construct(_Ty *_Ptr)
    {	// default construct object at _Ptr
        ::new ((void *)_Ptr) _Ty();
    }

    void construct(_Ty *_Ptr, const _Ty& _Val)
    {	// construct object at _Ptr with value _Val
        ::new ((void *)_Ptr) _Ty(_Val);
    }

    template<class _Objty,
        class... _Types>
        void construct(_Objty *_Ptr, _Types&&... _Args)
    {	// construct _Objty(_Types...) at _Ptr
        ::new ((void *)_Ptr) _Objty(_STD forward<_Types>(_Args)...);
    }


    template<class _Uty>
    void destroy(_Uty *_Ptr)
    {	// destroy object at _Ptr
        _Ptr->~_Uty();
    }

    size_t max_size() const
    {	// estimate maximum array size
        return ((size_t)(-1) / sizeof(_Ty));
    }
};
