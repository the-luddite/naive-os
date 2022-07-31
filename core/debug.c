#include "debug.h"



void debug(const char *message, unsigned level)
{
    unsigned enabled = 0;
    #ifdef DEBUG_LEVEL
    if (level >= DEBUG_LEVEL)
        enabled = 1;
    #endif
    if (enabled)
        print_uart(message);
}

void BUG(const char *message)
{
    printf("BUG: %s\n", message);
    for(;;);
}