#ifndef __mem_h__
#define __mem_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// **************************************
// **********   BLOCK_HEADER   **********
// **************************************
// block_size = header_size + payload_size + padding 
// block_size must be divisible by 16
// alloc_bit = 1 if allocated and 0 if free
// size_alloc = block_size + alloc_bit
// payload = size requested for payload by the user of an allocated block or available size of a free block (used for performance analysis)
// performance analysis is provided in Mem_Dump function
typedef struct{
    int size_alloc; 
    int payload;
} BLOCK_HEADER;

void Initialize_Memory_Allocator(int size); 
void Free_Memory_Allocator(); 
void Mem_Dump();
void* Mem_Alloc(int size);
int Mem_Free(void *ptr);

#endif 
