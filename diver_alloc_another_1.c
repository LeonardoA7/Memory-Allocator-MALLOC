#include "mem.h"
#include <stdio.h>

int main() {
    Initialize_Memory_Allocator(2600);
    Mem_Dump();

    int *p1 = Mem_Alloc(202); //32
    int *p2 = Mem_Alloc(313); //48
    int *p3 = Mem_Alloc(412); //64
    int *p4 = Mem_Alloc(133); //32
    int *p5 = Mem_Alloc(116); //32
    int *p6 = Mem_Alloc(233); //32
    int *p7 = Mem_Alloc(146); //32
    int *p8 = Mem_Alloc(112); //32
    int *p9 = Mem_Alloc(245); //32
    int *p10 = Mem_Alloc(983); //Total allocated size should be 
    if (p10 == NULL) {
        printf("YO\n");
    }
    Mem_Dump();
    Mem_Free(p7);
    Mem_Dump();
    Mem_Free(p4);
    Mem_Dump();
    Mem_Free(p3);
    Mem_Dump();
    Mem_Free(p6);
    Mem_Dump();
    Mem_Free(p1);
    Mem_Dump();
    Mem_Free(p2);
    Mem_Dump();
    Mem_Free(p8);
    Mem_Dump();
    Mem_Free(p10);
    Mem_Dump();
    Mem_Free(p9);
    Mem_Dump();
    Mem_Free(p5);

    Mem_Dump();
    Free_Memory_Allocator();
    return 0;
}