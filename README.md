Custom memory allocator

-- i am building a custom memory allocator in c++. 
- goal is to self implement the dynamic memory allocation functions and program
- i will implement following things
    * .h file for complete allocation program
    * in main.cpp will include the .h file and use allocatemem() and freemem() functions. 

- for phase one i will use windows api funnction - virtualAlloc() to get intial memory from the OS . 
- later in phase 2 for further i will bypass this and implement my own virtual Allocate funt. 

- currently studying three algos - best fit, segregate list, buddy system - out off this will implement better one - or based on situation will dynamically switch in phase 2 implementation. 

- for each data allocation i need to implement meta data which will have info about size and allocation status. 

- i can traverse through memory by - current mem address + size of mem block = next block address. 

- from current analysis - following is the project folder structure:
-- Memalloc
    |- memalloc.h 
    |- main.cpp
    |- readme.md

target is to complete before - date 16/10/2026 .
