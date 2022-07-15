#include "qemu.h"

#define UARTDR			(QEMU_VIRT_UART_BASE)
#define UARTFR			(QEMU_VIRT_UART_BASE + 0x018)


void print_uart(const char *s);
char read_uart();