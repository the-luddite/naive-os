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

#define LOW_MEMORY      (2 * SECTION_SIZE)
#define HIGH_MEMORY     QEMU_VIRT_PCIE_ECAM

#define PAGING_MEMORY   (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES    (PAGING_MEMORY/PAGE_SIZE)


// #ifndef __ASSEMBLER__

uint64_t get_free_page();
void free_page(uint64_t p);
void memzero(uint64_t src, uint64_t n);
uintptr_t kmalloc(uint64_t size);

// #endif
