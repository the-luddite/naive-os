#include "memory.h"

#define FREE (0)

static uint64_t current_desc;
static uint8_t mem_map [ PAGING_PAGES ] = {0,};


static uintptr_t __allocate_page()
{
    for (int i = 0; i < PAGING_PAGES; i++)
    {
        if (mem_map[i] == FREE)
        {
            mem_map[i] = current_desc;
            return 0x40000000 + (LOW_MEMORY + i * PAGE_SIZE);
        }
    }
    return 0;
}

static void __free_page(uintptr_t p)
{
    mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = FREE;
}

uintptr_t kmalloc(uint64_t size)
{
    uintptr_t p;
    uint64_t pages = (uint64_t)(size / PAGE_SIZE);

    current_desc++;

    if (size > pages * PAGE_SIZE)
        pages++;
    
    p = __allocate_page();

    if (pages > 1)
    {
        for (int i = 0; i < pages - 1; i++)
        {
            if (!__allocate_page())
                goto fail;
        }
    }    
    return p;
fail:
    return 0;
}