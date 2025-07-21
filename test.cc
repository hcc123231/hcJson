#include<iostream>
#include<string>
#include"memPool.h"
#include <cstdint>
#include <algorithm>

#include <cstdint>
inline std::uint64_t up_pow2(std::uint64_t x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return ++x;
}
class My{
private:
    int a;
    double b;
    bool c;
public:
    My()=default;

};
int main(){
    /*hcc::MemPool pool{15};
    void* ptr1=pool.alloc();
    void* ptr2=pool.alloc();
    void* ptr3=pool.alloc();
    pool.dealloc(ptr2);
    void* ptr4=pool.alloc();
    void* ptr5=pool.alloc();
    
    void* ptr6=pool.alloc();
    void* ptr7=pool.alloc();*/

    void* ptr=malloc(512);
    char* p=(char*)ptr+16;
    free(p);
    return 0;
}