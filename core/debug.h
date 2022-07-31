#pragma once

#include "printf.h"


#define DEBUG_WARN  (3)
#define DEBUG_TRACE (2)
#define DEBUG_ALL (1)

void BUG(const char *message);
void debug(const char *message, unsigned level);