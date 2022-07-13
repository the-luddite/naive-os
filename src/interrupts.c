#include "system.h"


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