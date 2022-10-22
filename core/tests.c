#include "aarch64/system.h"
#include "scheduler.h"


void user_process1(char *array)
{
	char buf[2] = {0};
	while (1){
		for (int i = 0; i < 5; i++){
			buf[0] = array[i];
			asm_sys_write(buf);
			delay(100000);
		}
	}
}

void user_process(){
	char buf[30] = {0};
	tfp_sprintf(buf, "User process started\n\r");
	asm_sys_write(buf);
	// printf("User process EL %d\r\n", get_current_el());
	unsigned long stack = asm_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	int err = asm_sys_clone((unsigned long)&user_process1, (unsigned long)"12345", stack);
	if (err < 0){
		printf("Error while clonning process 1\n\r");
		return;
	} 
	stack = asm_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	err = asm_sys_clone((unsigned long)&user_process1, (unsigned long)"abcd", stack);
	if (err < 0){
		printf("Error while clonning process 2\n\r");
		return;
	} 
	asm_sys_exit();
}


void kernel_process(){
	printf("Kernel process started. EL %d\r\n", get_current_el());
	int err = move_to_user_mode((unsigned long)&user_process);
	if (err < 0){
		printf("Error while moving process to user mode\n\r");
	} 
}


void test_sched_core_thread()
{
    printf("test_sched_core_thread test started\n");

    int res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0, 0);
	if (res < 0) {
		printf("error while starting kernel process");
		return;
	}
	return;
}