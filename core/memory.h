#pragma once

#include "qemu.h"
#include "stdint.h"
#include "debug.h"

/*
Src: https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson04/rpi-os.md
*/

#define PAGE_SHIFT      12
#define TABLE_SHIFT     9
#define SECTION_SHIFT   (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE       (1 << PAGE_SHIFT)	// 4096
#define SECTION_SIZE    (1 << SECTION_SHIFT)	

#define RAM_SHIFT       (0x40000000)
#define LOW_MEMORY      (2 * SECTION_SIZE + RAM_SHIFT)
#define HIGH_MEMORY     (QEMU_VIRT_PCIE_ECAM + RAM_SHIFT)

#define PAGING_MEMORY   (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES    (PAGING_MEMORY/PAGE_SIZE)


// #ifndef __ASSEMBLER__

u64 get_free_page();
void free_page(u64 p);
void memzero(u64 src, u64 n);
uintptr_t kmalloc(u64 size);

// #endif
