#include "timer.h"


// static u32 current_cnt;
static u64 interval;


void timer_init(void) 
{
	static u64 current_cnt;
	interval = raw_read_cntfrq_el0() / 2;

    disable_cntv();

	current_cnt = raw_read_cntv_cval_el0();
	current_cnt += interval;
	raw_write_cntv_cval_el0(current_cnt);
	enable_cntv();
}

void timer_irq_handler()
{
	static u64 current_cnt;
	disable_cntv();
	// current_cnt = (get_cntvct() + interval);
	// printf("current: %u, next: %u\n", get_cntvct(), current_cnt);
	// put_cntv_cval(current_cnt);
	current_cnt = raw_read_cntv_cval_el0() + interval;
	raw_write_cntv_cval_el0(current_cnt);
	enable_cntv();
	timer_tick();	
}

// void timer_test()
// {
//     u32 val;
// 	u64 ticks, current_cnt;

//     print_uart("CurrentEL = ");
// 	get_current_el(&val);
// 	print_uint(val);

// 	// // Disable the timer
// 	disable_cntv();
//     print_uart("\nDisable the timer, CNTV_CTL_EL0 = ");
// 	get_cntv_ctl(&val);
// 	print_uint(val);
//     print_uart("\nSystem Frequency: CNTFRQ_EL0 = ");
// 	cntfrq = get_cntfrq();
// 	print_uint(cntfrq);

// 	// Next timer IRQ is after n sec(s).
// 	ticks = 1 * cntfrq;
// 	// Get value of the current timer
// 	current_cnt = get_cntvct();
//     print_uart("\nCurrent counter: CNTVCT_EL0 = ");
// 	print_uint(current_cnt);
// 	// Set the interrupt in Current Time + TimerTick
// 	put_cntv_cval(current_cnt + ticks);
//     print_uart("\nAssert Timer IRQ after 1 sec: CNTV_CVAL_EL0 = ");

// 	// Enable the timer
// 	enable_cntv();
//     print_uart("\nEnable the timer, CNTV_CTL_EL0 = ");
// 	get_cntv_ctl(&val);
// 	print_uint(val);

// 	// // Enable IRQ 
// 	enable_irq();
//     print_uart("\nEnable IRQ, DAIF = ");
// 	get_daif(&val);
// 	print_uint(val);
//     print_uart("\n");
// }