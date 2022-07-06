#pragma once

#include "uart.h"

#define VIC_BASE_ADDR 0x10140000
#define VIC_INTENABLE (*((volatile uint32_t *)(VIC_BASE_ADDR + 0x010)))


enum system_state {
    none = 0,
    initializing,
    running,
    panic
};

unsigned int current_state;