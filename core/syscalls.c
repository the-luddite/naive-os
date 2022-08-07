#include "syscalls.h"
#include "system.h"


void do_sys_write(char * buf)
{
	printf(buf);
}

int do_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack)
{
	return copy_process(0, 0, 0, stack);
}

unsigned long do_sys_malloc(uint64_t size)
{
	unsigned long addr = kmalloc(size);
	if (!addr) {
		return -1;
	}
	return addr;
}

void do_sys_exit()
{
	exit_process();
    printf("exit process called\n");
}

void * const sys_call_table[] = {do_sys_write, do_sys_malloc, do_sys_clone, do_sys_exit};
