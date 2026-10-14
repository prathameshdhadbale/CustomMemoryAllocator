# Custom Memory Allocator in C++

A custom memory allocator implemented in C++ to understand the fundamentals of dynamic memory allocation, memory management, and how allocators manage memory internally.

## Overview

This project explores the internal mechanisms of dynamic memory allocation by implementing allocation and deallocation functions from scratch.

The objective is **to understand how memory allocators work, rather than to outperform existing allocators** such as `malloc` or `new`.

## Features

- **OS-Level Memory Acquisition:** Uses the Windows `VirtualAlloc()` API to obtain an initial memory region.
- **Best-Fit Allocation:** Searches available blocks and selects the smallest free block that satisfies the requested size.
- **Memory Metadata:** Maintains block size, allocation status, and a pointer to the next block.
- **Block Splitting:** Splits free blocks when sufficient space remains for a new block and its metadata.
- **Memory Deallocation:** Implements a custom `memfree()` function to make allocated blocks available for reuse.
- **Block Coalescing:** Merges adjacent free blocks to reduce external fragmentation.
- **Memory Alignment:** Aligns payload addresses and allocation sizes to meet alignment requirements.
- **Resource Cleanup:** Uses a destructor with `VirtualFree()` to release the allocated memory region.

## Project Structure

```text
Memalloc/
├── allocation.h  # Allocator implementation
├── main.cpp      # Usage and testing
└── README.md     # Project documentation
```

## Implementation

The allocator manages a memory region obtained from Windows and divides it into blocks. Each block contains metadata describing its size, allocation status, and the next block.

Allocation follows the Best-Fit strategy:

1. Obtain the initial memory region from the operating system using `VirtualAlloc()`.
2. Search the block list for the smallest suitable free block.
3. Split the selected block when enough space remains for a new header and payload.
4. Return an aligned pointer to the payload rather than the metadata.
5. On deallocation, mark the block as free and merge adjacent free blocks.

## Usage

Include `allocation.h` in `main.cpp` and use the allocator's allocation and deallocation methods.

```cpp
#include "allocation.h"
#include <iostream>

int main() {
    allocation alloc;

    void* ptr = alloc.memalloc(100);

    if (ptr != nullptr) {
        std::cout << "Memory allocated successfully\n";

        alloc.memfree(ptr);
        std::cout << "Memory deallocated successfully\n";
    }

    return 0;
}
```

The underlying memory region is released when the `allocation` object is destroyed.

## Key Learnings

Building this allocator provided practical experience with:

- Pointer arithmetic and memory layout.
- Managing metadata alongside user payloads.
- Returning payload pointers while preserving allocator metadata.
- Memory alignment and allocation-size calculations.
- Best-Fit searching, block splitting, and coalescing.
- Resource management and destructor-based cleanup.

The most challenging part was implementing allocation correctly, particularly managing pointers, calculating metadata and payload addresses, and ensuring proper alignment and sufficient space for both headers and payloads.

The approach was to reason backward from the allocation process: first understand where the allocator obtains memory, then determine how to track available blocks, select a suitable block, and return the correct payload pointer.


## Author

**Prathamesh**
