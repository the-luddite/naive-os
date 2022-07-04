#include "uart.h"

pl011_T * const UART0 = (pl011_T *)0x101f1000;
pl011_T * const UART1 = (pl011_T *)0x101f2000;
pl011_T * const UART2 = (pl011_T *)0x101f3000;


void print_uart(pl011_T *uart, const char *s) {
    while(*s != '\0') { 
        uart->DR = (unsigned int)(*s); 
        s++;
    }
}