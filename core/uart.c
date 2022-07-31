#include "uart.h"


#define UART_BUF_MAX 100

static uint32_t uart_buffer[UART_BUF_MAX];
static uint32_t buffer_p = 0;

pl011_T * const UART0 = (pl011_T *)QEMU_VIRT_UART_BASE;


void uart_init()
{
    uart_disable_tx_rx();
    uart_disable_irq();

    // set baudrate = 115200
    UART0->IBRD = 26;
    UART0->FBRD = 3;
    
    UART0->LCR_H = (3 << 5) | (1 << 4);
    uart_enable_tx_rx();
    uart_enable_irq();
}

void uart_disable_irq()
{
    UART0->IMSC = 0;
}

void uart_disable_tx_rx()
{
    UART0->CR = 0;
}

void uart_enable_irq()
{
    UART0->IMSC = 1<<4;
}

void uart_enable_tx_rx()
{
    UART0->CR = (1 << 9) | (1 << 8) | (1 << 0);
}

void print_uart(const char *s) 
{
    while(*s != '\0') 
    { 
        put_uart((char)*s); 
        s++;
    }
}

void put_uart(char c)
{
    while( (UART0->FR & 0x20) != 0) continue;
    UART0->DR = c;
}

// wrapper required by printf
void putc_uart(void* p, char c)
{
	put_uart(c);
}

uint32_t read_uart() 
{
    while( (UART0->FR & 0x10) != 0) continue;
    return (uint32_t)UART0->DR;
}

static char *__str_reverse_in_place(char *str, int len) 
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

    if (digit == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        print_uart(buf);
        return;
    }

    while (digit > 0)
    {
        unsigned int m = digit % 10;
        buf[c] = digits[m];
        digit /= 10;
        c++;
    }
    __str_reverse_in_place(buf, c);
    buf[c+1] = '\0';
    print_uart(buf);
}

void handle_uart_irq()
{
    uint32_t c = read_uart();

    if (buffer_p >= UART_BUF_MAX - 1
            || c == 13)
    {
        print_uart("debug\n");
        print_uart(uart_buffer);
        // for (uint32_t i = 0; i <= uart_buffer.p; i++)
        //     put_uart(uart_buffer.buffer[i]);
        put_uart('\n');
        buffer_p = 0;
    } else if (buffer_p < UART_BUF_MAX - 1)
    {
        uart_buffer[buffer_p] = c;
        uart_buffer[buffer_p + 1] = '\0';
        buffer_p++;
    }
    
}