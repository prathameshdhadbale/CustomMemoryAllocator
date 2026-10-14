#include <iostream>
#include "allocation.h"

using namespace std;

int main() {
    allocation alloc;

    // Allocate memory
    int* p1 = static_cast<int*>(alloc.memalloc(sizeof(int)));
    int* p2 = static_cast<int*>(alloc.memalloc(sizeof(int)));

    if (p1 == nullptr || p2 == nullptr) {
        cout << "Memory allocation failed\n";
        if (p1) alloc.memfree(p1);
        if (p2) alloc.memfree(p2);
        return 1;
    }

    // Use allocated memory
    *p1 = 10;
    *p2 = 20;

    cout << "p1 value: " << *p1 << '\n';
    cout << "p2 value: " << *p2 << '\n';

    // Deallocate memory
    alloc.memfree(p1);
    alloc.memfree(p2);

    cout << "Memory deallocated successfully\n";

    return 0;
}