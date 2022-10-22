#include "uart.h"


static char *uart_buffer;
static u32 buffer_p = 0;

static pl011_T * const UART0 = (pl011_T *)QEMU_VIRT_UART_BASE;


void uart_init()
{
    uart_disable_tx_rx();
    uart_disable_irq();

    uart_buffer = (char*)kmalloc(sizeof(char) * UART_BUF_MAX);

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

u32 read_uart() 
{
    while( (UART0->FR & 0x10) != 0) continue;
    return (u32)UART0->DR;
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

void uart_irq_handler()
{
    u32 c = read_uart();

    if (buffer_p >= UART_BUF_MAX - 1) {
        uart_reset_buffer();
    } else {
        uart_buffer[buffer_p] = c;
        buffer_p++;
        uart_buffer[buffer_p] = '\0';
    }
}

void uart_reset_buffer()
{
    buffer_p = 0;
    uart_buffer[0] = '\0';
}

int32_t uart_read_buffer(char *buf)
{
    int c = 0;
    if (buffer_p == 0)
        return 0;
    while (c < buffer_p)
    {
        buf[c] = uart_buffer[c];
        c++;
    }
    uart_reset_buffer();
    return c;
}