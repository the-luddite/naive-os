#pragma once

#define __NR_syscalls	    4


#ifndef __ASSEMBLER__
void call_sys_write(char * buf);
int call_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack);
unsigned long call_sys_malloc();
void call_sys_exit();
#endif