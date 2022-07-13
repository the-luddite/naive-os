#include "system.h"


static char horrible_buffer[4096];
static uint32_t bp;


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


void print_uint(unsigned int digit) {
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
    print_uart(UART0, buf);
}


void echo_console(void) {
    /* do echo */
    char c = UART0->DR;
    UART0->DR = c;

    /* why doesn't it react on '\n'? */
    if (c == 'q' || bp == (4096 - 2)) {
        print_uart(UART0, horrible_buffer);
        bp = 0;
    } else {
        horrible_buffer[bp] = c;
        horrible_buffer[bp + 1] = '\0';
        bp++;
    }
}

void __attribute__((interrupt)) uart0_irq_handler() {
    char buf[100];

    echo_console();
    
    unsigned int time = get_timer_masked();
    print_uint(time);
    // UART0->DR = time;

    VIC_INTCLEAR = 0;
}

void __attribute__((interrupt)) undef_handler(void) { for(;;); }
void __attribute__((interrupt)) swi_handler(void) { for(;;); }
void __attribute__((interrupt)) prefetch_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) data_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) fiq_handler(void) { for(;;); }
 

void copy_vectors(void) {
    extern uint32_t vectors_start;
    extern uint32_t vectors_end;
    uint32_t *vectors_src = &vectors_start;
    uint32_t *vectors_dst = (uint32_t *)0;

while(vectors_src < &vectors_end)
    *vectors_dst++ = *vectors_src++;
}


void core_main() {

    print_uart(UART0, "We have started\n");
    current_state = initializing;

    VIC_INTENABLE = 1<<12;
    UART0->IMSC = 1<<4;

    current_state = running;
    
    timer_init();

    print_uart(UART0, "Initialized\n");
    for(;;);
}

