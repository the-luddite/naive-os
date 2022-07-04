#pragma once

#include "uart.h"


enum system_state {
    none = 0,
    initializing,
    running,
    panic
};

unsigned int current_state;