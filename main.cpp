
#include <iostream>
#include "allocation.h"

using namespace std;

int main() {
    allocation allocator;

    void* a = allocator.memalloc(1000);
    cout << "Allocation A: " << a << '\n';
    allocator.printBlocks();
    
    allocator.memfree(a) ;
    allocator.printBlocks();

    // void* b = allocator.memalloc(40);
    // cout << "Allocation B: " << b << '\n';
    // allocator.printBlocks();

    // void* c = allocator.memalloc(80);
    // cout << "Allocation C: " << c << '\n';
    // allocator.printBlocks();

    return 0;
}
