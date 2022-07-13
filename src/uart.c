#include "uart.h"

pl011_T * const UART0 = (pl011_T *)0x101f1000;
pl011_T * const UART1 = (pl011_T *)0x101f2000;
pl011_T * const UART2 = (pl011_T *)0x101f3000;


void print_uart(pl011_T *uart, const char *s) {
    uart->DR = '\t';
    uart->DR = '\n';
    while(*s != '\0') { 
        uart->DR = (unsigned int)(*s); 
        s++;
    }
    uart->DR = '\t';
    uart->DR = '\n';
}


bool read_uart(pl011_T *uart, char *buf, int max) {
    int c = 1;
    if ((uart->FR & RXFE) == 0) {
        while(uart->FR & TXFF && c != max) {
            *buf = uart->DR;
            buf++;
            c++;
        }
        return 0;
    }
    return 1;
}