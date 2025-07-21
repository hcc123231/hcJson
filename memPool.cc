#include"memPool.h"
#include<iostream>
#include<cstdio>
using namespace hcc;
MemPool::MemPool(size_t n):_blockSize{n},_pageSize{(n)*CACHE_BLOCK_COUNT},_availPtr{nullptr}{
    //std::cout<<"dsdsd:"<<_blockSize<<" "<<_pageSize<<'\n';
}
MemPool::~MemPool(){

}
void* MemPool::alloc_page(size_t size){
    return ::operator new(size,std::align_val_t{64});
}
void MemPool::dealloc_page(void* ptr){
    ::operator delete(ptr,std::align_val_t{64});
}
void* MemPool::alloc(){
    //如果可用块_availPtr指向nullptr
    [[unlikely]]if(!_availPtr){
        void* mem=alloc_page(_pageSize);
        char* p=(char*)mem;
        for(size_t i=0;i<CACHE_BLOCK_COUNT-1;i++){
            *(char**)p=p+_blockSize;
            p=p+_blockSize;
        }
        *(char**)p=nullptr;
        _availPtr=(char*)mem;
        p=_availPtr;
        _availPtr=*(char**)_availPtr;
        //printf("%p\n",(void*)p);
        return (void*)p;
    }    
    //如果_availPtr可以使用，那么返回_availPtr就完事儿了
    void* ptr=(void*)(_availPtr);
    _availPtr=*(char**)_availPtr;
    //printf("%p\n",(void*)ptr);
    return ptr;

}
void MemPool::dealloc(void* ptr){
    //printf("dealloc:%p\n",ptr);
    *(char**)ptr=_availPtr;
    _availPtr=(char*)ptr;
}
MemPool& hcc::gMemPool(){
    //这个64就是sizeof(JsonValue)
    static MemPool pool{64};
    return pool;
}