#pragma once


#define __NR_syscalls	    7

#define SYS_WRITE_NUMBER    0
#define SYS_MALLOC_NUMBER   1
#define SYS_CLONE_NUMBER    2
#define SYS_EXIT_NUMBER     3
#define SYS_OPEN_NUMBER     4
#define SYS_READ_NUMBER     5
#define SYS_PRINT_NUMBER    6

#ifndef __ASSEMBLER__
void asm_sys_write(char * buf);
int asm_sys_clone(unsigned long fn, unsigned long arg, unsigned long stack);
unsigned long asm_sys_malloc();
void asm_sys_exit();
void asm_sys_open(char *path);
void asm_sys_read(unsigned *buf);
void asm_sys_print(unsigned *buf);

#endif