#include "syscalls.h"
#include "aarch64/system.h"


void do_sys_write(char * buf)
{
	printf(buf);
}

int do_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack)
{
	return copy_process(0, 0, 0, stack);
}

unsigned long do_sys_malloc(unsigned long size)
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
}

void do_sys_open(char *path)
{
	exit_process();
}

int32_t do_sys_read(unsigned *buf)
{
	return uart_read_buffer(buf);
}

void do_sys_print(unsigned *buf)
{
	printf(buf);
}

void * const sys_call_table[] = {do_sys_write, do_sys_malloc, do_sys_clone, do_sys_exit, do_sys_open, do_sys_read, do_sys_print};
