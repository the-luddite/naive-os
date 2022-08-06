#pragma once

#define __NR_syscalls	    4


#ifndef __ASSEMBLER__
void sys_write(char * buf);
int sys_clone(unsigned long stack);
unsigned long sys_malloc();
void sys_exit();
#endif