#pragma once

#include "memory.h"
#include "scheduler.h"


/*
 * PSR bits
 */
#define PSR_MODE_EL0t	0x00000000
#define PSR_MODE_EL1t	0x00000004
#define PSR_MODE_EL1h	0x00000005
#define PSR_MODE_EL2t	0x00000008
#define PSR_MODE_EL2h	0x00000009
#define PSR_MODE_EL3t	0x0000000c
#define PSR_MODE_EL3h	0x0000000d


struct pt_regs {
	unsigned long regs[31];
	unsigned long sp;
	unsigned long pc;
	unsigned long pstate;
};


int copy_process(u64 clone_flags, u64 fn, u64 arg, u64 stack);
u8 move_to_user_mode(u64 pc);
static struct pt_regs * task_pt_regs(struct job_s *tsk);