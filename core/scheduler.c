#include "scheduler.h"


static struct job_s init_task = INIT_TASK;
struct job_s *current = &(init_task);
struct job_s *task[NR_TASKS] = {&(init_task), };
int nr_tasks = 1;


void preempt_disable(void)
{
	current->preempt_count++;
}

void preempt_enable(void)
{
	current->preempt_count--;
}

void schedule(void)
{
    current->counter = 0;
    _schedule();
}

// schedule_tail is called from asm
void schedule_tail(void)
{
    preempt_enable();
}

void timer_tick()
{
    --current->counter;
    if (current->counter>0 || current->preempt_count >0) {
        return;
    }
    current->counter=0;
    enable_irq();
    _schedule();
    disable_irq();
}

static void _schedule(void)
{
	// debug("scheduler init\n", DEBUG_TRACE);

	preempt_disable();
	long next,c;
	struct job_s * p;
	while (1) {
		c = -1;
		next = 0;
		for (int i = 0; i < NR_TASKS; i++){
			p = task[i];
			if (p && p->state == TASK_RUNNING && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}
		if (c) {
			break;
		}
		for (int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}
	switch_to(task[next], next);
	preempt_enable();
}

static void switch_to(struct job_s * next, int pid)
{
	// debug("reached c level switch_to\n", DEBUG_TRACE);
	if (current == next) 
		return;
	struct job_s * prev = current;
	current = next;

	// debug("body of c level switch_to\n", DEBUG_TRACE);
	printf("switch_to: %d\n", pid);

	cpu_switch_to(prev, next);
}

void exit_process(){
	preempt_disable();
	for (int i = 0; i < NR_TASKS; i++){
		if (task[i] == current) {
			task[i]->state = TASK_ZOMBIE;
			break;
		}
	}
	if (current->stack) {
		kfree(current->stack);
	}
	preempt_enable();
	schedule();
}
