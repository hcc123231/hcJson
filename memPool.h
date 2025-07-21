#pragma once
#include<stddef.h>
#include<vector>
#include<memory>
namespace hcc{
    #define CACHE_BLOCK_COUNT 3 
    class MemPool{
    private:
        char* _availPtr;
        size_t _pageSize;
        size_t _blockSize;
    public:
        MemPool()=delete;
        MemPool(size_t n);
        ~MemPool();
    public:
        void* alloc();
        void dealloc(void* ptr);
        void* alloc_page(size_t size);
        void dealloc_page(void* ptr);
    };
    MemPool& gMemPool();
}