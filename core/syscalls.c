#include "syscalls.h"
#include "system.h"


void call_sys_write(char * buf)
{
	printf(buf);
}

int call_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack)
{
	return copy_process(0, 0, 0, stack);
}

unsigned long call_sys_malloc(uint64_t size)
{
	unsigned long addr = kmalloc(size);
	if (!addr) {
		return -1;
	}
	return addr;
}

void call_sys_exit()
{
	// exit_process();
    printf("exit process called\n");
}

void * const sys_call_table[] = {call_sys_write, call_sys_malloc, call_sys_clone, call_sys_exit};
