#include "memory.h"
#include "scheduler.h"


void ret_from_fork(void);


int copy_process(unsigned long fn, unsigned long arg)
{
	preempt_disable();
	struct job_s *p;

	p = (struct job_s *) kmalloc(sizeof(struct job_s));
	if (!p)
		return 1;
	p->priority = current->priority;
	// p->priority = 2;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail

	p->cpu_context.x19 = fn;
	p->cpu_context.x20 = arg;
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)p + THREAD_SIZE;
	int pid = nr_tasks++;
	task[pid] = p;

	preempt_enable();
	return 0;
}
