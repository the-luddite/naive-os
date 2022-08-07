#include "system.h"
#include "syscalls.h"
#include "process.h"


void user_process1(char *array)
{
	char buf[2] = {0};
	while (1){
		for (int i = 0; i < 5; i++){
			buf[0] = array[i];
			call_sys_write(buf);
			delay(100000);
		}
	}
}

void user_process(){
	char buf[30] = {0};
	tfp_sprintf(buf, "User process started\n\r");
	call_sys_write(buf);
	// printf("User process EL %d\r\n", get_current_el());
	unsigned long stack = call_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	int err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"12345", stack);
	if (err < 0){
		printf("Error while clonning process 1\n\r");
		return;
	} 
	stack = call_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"abcd", stack);
	if (err < 0){
		printf("Error while clonning process 2\n\r");
		return;
	} 
	call_sys_exit();
}

void kernel_process(){
	printf("Kernel process started. EL %d\r\n", get_current_el());
	int err = move_to_user_mode((unsigned long)&user_process);
	if (err < 0){
		printf("Error while moving process to user mode\n\r");
	} 
}


void core_main() 
{
    debug("entered core_main\n", DEBUG_TRACE);

	uart_init();

	init_printf(0, putc_uart);

	gic_v3_initialize();

    timer_init();

	enable_irq();

// #ifdef TEST_SCHED
// 	test_sched_core_thread();
// #endif
	int res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0, 0);
	if (res < 0) {
		printf("error while starting kernel process");
		return;
	}

	

	while(1) {
		// wfi();	/* Wait for Interrupt */
		schedule();
	}

}
