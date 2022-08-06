#include "syscalls.h"
#include "system.h"


void sys_write(char * buf)
{
	printf(buf);
}

int sys_clone(unsigned long stack)
{
	return copy_process(0, 0, 0, stack);
}

unsigned long sys_malloc(uint64_t size)
{
	unsigned long addr = kmalloc(size);
	if (!addr) {
		return -1;
	}
	return addr;
}

void sys_exit()
{
	// exit_process();
    printf("exit process called\n");
}

void * const sys_call_table[] = {sys_write, sys_malloc, sys_clone, sys_exit};
