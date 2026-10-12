#include<windows.h>
#include<iostream>
using namespace std ;

void* initialMemory(){
    SIZE_T size = 1024 ;

    void* memory = VirtualAlloc(
        nullptr ,
        size,
        MEM_RESERVE | MEM_COMMIT ,
        PAGE_READWRITE
    ) ;

    if(memory == nullptr){
        cerr<<"memory allocation failed"<<'\n' ;
        return nullptr ;
    }

    cout<<"memory allocated at "<<memory<<'\n' ;
    return memory ;
}


void* memalloc(int size_of_alloc){
    void* memory = initialMemory() ;
    //we got initial memory now and we need to now implement algorithm to find free space of required size and to allocate it.
}