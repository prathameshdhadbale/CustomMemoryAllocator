
#include <windows.h>
#include <iostream>
#include <cstddef>
#include <limits>
#include <new>
#include <cstdint>

using namespace std;

struct block {
    SIZE_T size;
    bool free = true;
    block* next;
};

class allocation {
private:
    block* memory = nullptr;

    // Alignment promised for returned payloads.
    static constexpr SIZE_T ALIGNMENT = alignof(max_align_t);

    // Header span includes padding before the payload.
    static constexpr SIZE_T HEADER_SIZE =
        ((sizeof(block) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    bool alignSize(SIZE_T requested, SIZE_T& aligned) {
        const SIZE_T remainder = requested % ALIGNMENT;
        const SIZE_T padding =
            remainder == 0 ? 0 : ALIGNMENT - remainder;

        if (requested > numeric_limits<SIZE_T>::max() - padding) {
            return false;
        }

        aligned = requested + padding;
        return true;
    }

    block* initialMemory() {
        const SIZE_T regionSize = 1024;

        void* region = VirtualAlloc(
            nullptr,
            regionSize,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );

        if (region == nullptr) {
            cerr << "Memory allocation failed\n";
            return nullptr;
        }

        // VirtualAlloc returns suitably aligned memory.
        block* initial = ::new (region) block{};

        initial->free = true;
        initial->size = regionSize - HEADER_SIZE;
        initial->next = nullptr;

        cout << "Memory allocated at " << region << '\n';

        return initial;
    }

public:
    void* memalloc(int size_of_alloc) {
        if (size_of_alloc <= 0) {
            return nullptr;
        }

        SIZE_T aligned_size;

        if (!alignSize(
                static_cast<SIZE_T>(size_of_alloc),
                aligned_size)) {
            return nullptr;
        }

        if (memory == nullptr) {
            memory = initialMemory();
        }

        if (memory == nullptr) {
            return nullptr;
        }

        // Best-fit search.
        block* p = memory;
        block* best = nullptr;

        while (p != nullptr) {
            if (p->free && p->size >= aligned_size) {
                if (best == nullptr || p->size < best->size) {
                    best = p;
                }
            }

            p = p->next;
        }

        if (best == nullptr) {
            return nullptr;
        }

        // Split only if a new header and positive aligned payload fit.
        const SIZE_T remaining = best->size - aligned_size;

        if (remaining >= HEADER_SIZE + ALIGNMENT) {
            char* start = reinterpret_cast<char*>(best);

            char* address = start + HEADER_SIZE + aligned_size;

            block* split = ::new (address) block{};

            split->size = remaining - HEADER_SIZE;
            split->free = true;
            split->next = best->next;

            best->size = aligned_size;
            best->free = false;
            best->next = split;
        } else {
            // Consume the entire block when splitting is not viable.
            best->free = false;
        }

        char* payload =
            reinterpret_cast<char*>(best) + HEADER_SIZE;

        return payload;
    }


    ////////////////////////////////////////////


    void memfree(void* ptr){
        if(ptr == nullptr) return ;

        block* p = memory ;
        bool ptr_present = false ;

        while(p != nullptr){
            char* payload = reinterpret_cast<char*>(p) + HEADER_SIZE;
            if(payload == ptr){
                ptr_present = true ;;
                if(p->free) ptr_present = false ;
                break ;
            }
            p = p->next ;
        }

        if(!ptr_present) return ;

        char* start = reinterpret_cast<char*>(ptr) - HEADER_SIZE ;
        block* best = reinterpret_cast<block*>(start);
        best->free = true ;
        if(best->next != nullptr && best->next->free == true){
            best->size = best->size + best->next->size + HEADER_SIZE ;
            best->next = best->next->next ;
        }
        
        block* q = memory ;
        while(q != nullptr){
            if(q->free){
                if(q->next != nullptr && q->next->free == true){
                    q->size = q->size + q->next->size + HEADER_SIZE ;
                    q->next = q->next->next ;
                }else q = q->next ;
            }
            else q = q->next ;
        }

        return ;
    }



    ///////////////////////////////
void printBlocks() {
    cout << "\n========== ALLOCATOR BLOCKS ==========\n";

    block* p = memory;
    int index = 0;

    while (p != nullptr) {
        char* header =
            reinterpret_cast<char*>(p);

        char* payload = header + HEADER_SIZE;

        cout << "\nBlock #" << index << '\n';
        cout << "Header address : " << static_cast<void*>(header) << '\n';
        cout << "Payload address: " << static_cast<void*>(payload) << '\n';
        cout << "Payload size   : " << p->size << " bytes\n";
        cout << "Free           : " << boolalpha << p->free << '\n';
        cout << "Next block     : " << p->next << '\n';

        cout << "Header aligned : "
             << (reinterpret_cast<uintptr_t>(header)
                 % alignof(block) == 0) << '\n';

        cout << "Payload aligned: "
             << (reinterpret_cast<uintptr_t>(payload)
                 % ALIGNMENT == 0) << '\n';

        p = p->next;
        index++;
    }

    cout << "\n======================================\n";
}

};
