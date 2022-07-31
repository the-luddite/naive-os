#pragma once

#include "system.h"


#define MAX_PROCESS_PAGES   (16)
#define PF_KTHREAD          (2)
#define PRIO_BGND           (15)
#define PRIO_TOP            (0)
#define NR_TASKS			(64)
#define TASK_RUNNING		(0)
#define THREAD_SIZE			(4096)


extern struct job_s *current;
extern struct job_s *task[NR_TASKS];
extern int nr_tasks;


struct cpu_context {
	uint64_t x19;
	uint64_t x20;
	uint64_t x21;
	uint64_t x22;
	uint64_t x23;
	uint64_t x24;
	uint64_t x25;
	uint64_t x26;
	uint64_t x27;
	uint64_t x28;
	uint64_t fp;
	uint64_t sp;
	uint64_t pc;
};

struct job_s {
	struct cpu_context cpu_context;
	long state;	
	long counter;
	long priority;
	long preempt_count;
};

#define INIT_TASK \
/*cpu_context*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,1, 0 \
}
