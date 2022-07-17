#include "uart.h"

// volatile unsigned int * const UART0DR = (unsigned int *) UARTDR;
// volatile unsigned int * const UART0FR = (unsigned int *) UARTFR;

pl011_T * const UART0 = (pl011_T *)QEMU_VIRT_UART_BASE;


void uart_init()
{
    // enable UART interrupts
    

    UART0->CR = 0;
    UART0->IMSC = 0;

    // set baudrate = 115200
    UART0->IBRD = 26;
    UART0->FBRD = 3;
    
    UART0->LCR_H = (3 << 5) | (1 << 4);
    UART0->CR = (1 << 9) | (1 << 8) | (1 << 0);
    UART0->IMSC = 1<<4;
}

void print_uart(const char *s) 
{
    while(*s != '\0') 
    { 
        UART0->DR = (unsigned int)(*s); 
        s++;
    }
}

char read_uart() 
{
    while( (UART0->FR & 0x10) != 0) continue;
    return UART0->FR;
}

char *str_reverse_in_place(char *str, int len) 
{
    char *p1 = str;
    char *p2 = str + len - 1;

    while (p1 < p2) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }

    return str;
}

void print_uint(unsigned int digit) 
{
    char digits[] = "0123456789";
    char buf[25];
    unsigned int c = 0;

    while (digit > 0)
    {
        unsigned int m = digit % 10;
        buf[c] = digits[m];
        digit /= 10;
        c++;
    }
    str_reverse_in_place(buf, c);
    buf[c+1] = '\0';
    print_uart(buf);
}