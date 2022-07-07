#include "system.h"


static char horrible_buffer[4096];
static uint32_t bp;


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
    echo_console();
    
    UART0->DR = readl(CONFIG_SYS_TIMER_COUNTER);

    VIC_INTCLEAR = 0;
}
 
/* all other handlers are infinite loops */
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


// int timer_init (void)
// {
// 	unsigned long tmr_ctrl_val;

// 	/* 1st disable the Timer */
// 	tmr_ctrl_val = *(volatile unsigned long *)(CONFIG_SYS_TIMERBASE + 8);
// 	tmr_ctrl_val &= ~TIMER_ENABLE;
// 	*(volatile unsigned long *)(CONFIG_SYS_TIMERBASE + 8) = tmr_ctrl_val;

// 	/*
// 	 * The Timer Control Register has one Undefined/Shouldn't Use Bit
// 	 * So we should do read/modify/write Operation
// 	 */

// 	/*
// 	 * Timer Mode : Free Running
// 	 * Interrupt : Disabled
// 	 * Prescale : 8 Stage, Clk/256
// 	 * Tmr Siz : 16 Bit Counter
// 	 * Tmr in Wrapping Mode
// 	 */
// 	tmr_ctrl_val = *(volatile unsigned long *)(CONFIG_SYS_TIMERBASE + 8);
// 	tmr_ctrl_val &= ~(TIMER_MODE_MSK | TIMER_INT_EN | TIMER_PRS_MSK | TIMER_SIZE_MSK | TIMER_ONE_SHT );
// 	tmr_ctrl_val |= (TIMER_ENABLE | TIMER_PRS_8S);

// 	*(volatile unsigned long *)(CONFIG_SYS_TIMERBASE + 8) = tmr_ctrl_val;

//     *(volatile unsigned int *)(VERSATILE_SCTL_BASE) |=
// 	  ((VERSATILE_TIMCLK << VERSATILE_TIMER1_EnSel) | (VERSATILE_TIMCLK << VERSATILE_TIMER2_EnSel) |
// 	   (VERSATILE_TIMCLK << VERSATILE_TIMER3_EnSel) | (VERSATILE_TIMCLK << VERSATILE_TIMER4_EnSel));

// 	return 0;
// }

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

