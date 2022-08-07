#pragma once

#include "system.h"


#define MAX_PROCESS_PAGES   (16)
#define PF_KTHREAD          (2)
#define PRIO_BGND           (15)
#define PRIO_TOP            (0)
#define NR_TASKS			(64)
#define THREAD_SIZE			(4096)

#define TASK_RUNNING		(0)
#define TASK_ZOMBIE			(1)


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
	unsigned long stack;
	unsigned long flags;
};

#define INIT_TASK \
/*cpu_context*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,1, 0, 0, PF_KTHREAD \
}

void preempt_disable(void);
void preempt_enable(void);
void schedule(void);
void cpu_switch_to(struct job_s *, struct job_s *);
void timer_tick();
void schedule_tail(void);
static void switch_to(struct job_s *, int);
static void _schedule(void);
