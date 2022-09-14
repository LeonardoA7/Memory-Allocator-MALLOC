#include "mem.h"                      
extern BLOCK_HEADER* first_header;

void setFree(BLOCK_HEADER* ptr) { 
    ptr->size_alloc = ptr->size_alloc & 0xFFFFFFFE;
}
void setAlloc(BLOCK_HEADER* ptr) { 
    ptr->size_alloc = ptr->size_alloc | 0x1;
}
int getAlloc(BLOCK_HEADER* ptr) { 
    int alloc = ptr->size_alloc & 1;
    return alloc;
}
void* getUserPointer(BLOCK_HEADER* ptr) { 
    BLOCK_HEADER* userPointer = (void *)((unsigned long)ptr + 8);
    return userPointer;
}
int getSize(BLOCK_HEADER* ptr) { 
    int size = ptr->size_alloc & 0xFFFFFFFE;
    return size;
}
BLOCK_HEADER* split(BLOCK_HEADER* ptr, int sizeNeeded, int size, int remainder) { 
    // WE HAVE TO SPLIT
    // SPLIT BLOCK
    BLOCK_HEADER* splitHeader = (BLOCK_HEADER *)((unsigned long)ptr + sizeNeeded);
    splitHeader->size_alloc = remainder;
    splitHeader->payload = remainder - 8;
    setFree(splitHeader);
    return splitHeader;
}

void coalesce(BLOCK_HEADER* current, BLOCK_HEADER* previousHeader) { 
    BLOCK_HEADER* nextHeader = (BLOCK_HEADER *)((unsigned long)current + getSize(current));
    if (nextHeader->size_alloc == 1)
        return;
    // FRONT BLOCK IS FREE: COALESCE
    if (!getAlloc(nextHeader)) {
        int nextSize = getSize(nextHeader);
        current->size_alloc = current->size_alloc + nextSize;
        current->payload = current->size_alloc - 8;
        setFree(nextHeader);
    }
    // PREVIOUS BLOCK IS IN HEAP
    if (current != first_header && (previousHeader != NULL)) {
        if (!getAlloc(previousHeader)) {
            previousHeader->size_alloc = previousHeader->size_alloc + getSize(current);
            previousHeader->payload = previousHeader->size_alloc - 8;
            setFree(current);
            current = previousHeader;
            return;
        }
    }
    return;
}
// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size){
    BLOCK_HEADER* current = first_header;
    // find a free block that's big enough
    while (1) { 
        // NOTHING WAS FOUND SO RETURN NULL
        if (current->size_alloc == 1) { 
            return NULL;
        }

        int allocatedBit = getAlloc(current);
        int block_size = getSize(current);
        int sizeNeeded = size;
        sizeNeeded += 8;
        while (sizeNeeded%16) sizeNeeded++;

        // CURRENT BLOCK IS NOT ALLOCATED
        if (!allocatedBit) { 
            // BLOCK IS BIG ENOUGH
            if (block_size >= sizeNeeded) { 
                // CHECK IF SPLIT IS NEEDED
                int remainder = block_size - sizeNeeded;
                if (remainder >= 16) { // SPLIT
                    current->payload = size;
                    current->size_alloc = sizeNeeded;
                    setAlloc(current);
                    BLOCK_HEADER* splitHeader = split(current, sizeNeeded, size, remainder);
                   // printf("USER POINTER IN MEMALLOC BEFORE ADDITION:%p\n", current);
                    BLOCK_HEADER* userPointer = (BLOCK_HEADER *)((unsigned long)current + 8);
                   // printf("USER POINTER IN MEMALLOC%p\n", userPointer);
                    return userPointer;
                } else { // WE DO NOT HAVE TO SPLIT
                    current->payload = size;
                    current->size_alloc = sizeNeeded;
                    setAlloc(current);
                   // printf("USER POINTER IN MEMALLOC BEFORE ADDITION:%p\n", current);
                    BLOCK_HEADER* userPointer = (BLOCK_HEADER *)((unsigned long)current + 8);
                   // printf("USER POINTER IN MEMALLOC%p\n", userPointer);
                    return userPointer;
                }
            } else { // BLOCK IS TOO SMALL
                current = (BLOCK_HEADER *)((unsigned long)current + block_size);
                continue; 
            }
        }
        // FIND THE NEXT UNALLOCATED BIT
        if (allocatedBit) { 
            current = (BLOCK_HEADER *)((unsigned long)current + block_size);
            continue;
        }
    }
    //BLOCK_HEADER *current = first_header;
    
    // return NULL if we didn't find a block

    // allocate the block

    // split if necessary (if padding size is greater than or equal to 16 split the block)

    return NULL;
}


// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr){
    if (ptr == NULL)
        return -1;
    // traverse the list and check all pointers to find the correct block 
    // if you reach the end of the list without finding it return -1
    BLOCK_HEADER* ptrHeader = (BLOCK_HEADER *)((unsigned long)ptr - 8);
    if (!getAlloc(ptrHeader))
        return -1;
   // printf("USER POINTER: %p\n", ptr);
   // printf("USER HEAD POINTER: %p\n", ptrHeader);
    // check to see if the block is already freed
    BLOCK_HEADER* current = first_header;
    BLOCK_HEADER* previousHeader = NULL;
    
    while (1) { 
        // NOTHING WAS FOUND SO RETURN NULL
        if (current->size_alloc == 1) { 
            return -1;
        }

        int allocatedBit = getAlloc(current);
        int block_size = getSize(current);
        // CURRENT BLOCK IS ALLOCATED
        if (allocatedBit) { 
            // CHECK ADDRESS OF POINTER
            if (current == ptrHeader) { 
              //  printf("CURRENT POINTER: %p\n", current);
              //  printf("USER POINTER TO BE FREED %p\n", ptrHeader);
                // SAME ADDRESS
                // SET ALLOCATED BIT TO ZERO
                setFree(ptrHeader);
                setFree(current);
                // CHECK TO SEE IF WE HAVE TO COALESCE
                coalesce(current, previousHeader);
               // printf("SHOULD BE FREED");
                return 0;

            } else { // DIFFERENT ADDRESS - FIND NEXT BLOCK
                previousHeader = current;
                current = (BLOCK_HEADER *)((unsigned long)current + block_size);
                continue; 
            }
        }
        // FIND THE NEXT ALLOCATED BIT
        if (!allocatedBit) { 
            previousHeader = current;
            current = (BLOCK_HEADER *)((unsigned long)current + block_size);
            continue;
        }
    }

    // free the block 

    // coalesce adjacent free blocks

    return -1;
}

