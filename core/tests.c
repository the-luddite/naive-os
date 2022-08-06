#include "system.h"
#include "scheduler.h"


void process(char *array)
{
	while (1){
		for (volatile int i = 0; i < 5; i++){
			put_uart(array[i]);
			delay(10000000);
		}
	}
}


void test_sched_core_thread()
{
    printf("test_sched_core_thread test started\n");

    int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res) {
		printf("error while starting process 1\n");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde");
	if (res) {
		printf("error while starting process 2\n");
		return;
    }
}