#pragma once

#include "uart.h"
#include "versatile.h"
#include "timer.h"
#include "console.h"


#define readl(addr) (*(volatile unsigned int *) (addr))


enum system_state {
    none = 0,
    initializing,
    running,
    panic
};

unsigned int current_state;