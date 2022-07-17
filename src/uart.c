#include "uart.h"

volatile unsigned int * const UART0DR = (unsigned int *) UARTDR;
volatile unsigned int * const UART0FR = (unsigned int *) UARTFR;


void print_uart(const char *s) 
{
    *UART0DR = '\t';
    *UART0DR = '\n';
    while(*s != '\0') { 
        *UART0DR = (unsigned int)(*s); 
        s++;
    }
    *UART0DR = '\t';
    *UART0DR = '\n';
}


char read_uart() 
{
    while( (*UART0FR & 0x10) != 0) continue;
    return *UART0DR;
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