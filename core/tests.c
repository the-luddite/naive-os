#include "system.h"
#include "scheduler.h"


// void process(char *array)
// {
// 	while (1){
// 		for (volatile int i = 0; i < 5; i++){
// 			put_uart(array[i]);
// 			delay(10000000);
// 		}
// 	}
// }

// void kernel_process(){
// 	printf("Kernel process started. EL %d\r\n", get_current_el());
// 	int err = move_to_user_mode((unsigned long)&process);
// 	if (err < 0){
// 		printf("Error while moving process to user mode\n\r");
// 	} 
// }


// void test_sched_core_thread()
// {
//     printf("test_sched_core_thread test started\n");

//     int res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, (unsigned long)"12345", 0);
// 	if (res < 0) {
// 		printf("error while starting process 1\n");
// 		return;
// 	}
// 	res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, (unsigned long)"abcde", 0);
// 	if (res < 0) {
// 		printf("error while starting process 2\n");
// 		return;
//     }
// 	return;
// }