#include "memory.h"

#define FREE (0)
#define ALLOCATED (1)


static unsigned short mem_map [ PAGING_PAGES ] = {0,};


unsigned long allocate_page()
{
    for (int i = 0; i < PAGING_PAGES; i++)
    {
        if (mem_map[i] == FREE)
        {
            mem_map[i] = ALLOCATED;
            return 0x40000000 + (LOW_MEMORY + i * PAGE_SIZE);
        }
    }
    return 0;
}

void free_page(unsigned long p)
{
    mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = FREE;
}