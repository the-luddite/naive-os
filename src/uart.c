#include "uart.h"

volatile unsigned int * const UART0DR = (unsigned int *) UARTDR;
volatile unsigned int * const UART0FR = (unsigned int *) UARTFR;


void print_uart(const char *s) {
    *UART0DR = '\t';
    *UART0DR = '\n';
    while(*s != '\0') { 
        *UART0DR = (unsigned int)(*s); 
        s++;
    }
    *UART0DR = '\t';
    *UART0DR = '\n';
}


char read_uart() {
    while( (*UART0FR & 0x10) != 0) continue;
    return *UART0DR;
}