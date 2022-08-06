#include "process.h"


void ret_from_fork(void);


int copy_process(uint64_t clone_flags, uint64_t fn, uint64_t arg, uint64_t stack)
{
	preempt_disable();
	struct job_s *p;

	p = (struct job_s *) kmalloc(sizeof(struct job_s));
	if (!p) {
		return -1;
	}

	struct pt_regs *childregs = task_pt_regs(p);
	memzero((uint64_t)childregs, sizeof(struct pt_regs));
	memzero((uint64_t)&p->cpu_context, sizeof(struct cpu_context));

	if (clone_flags & PF_KTHREAD) {
		p->cpu_context.x19 = fn;
		p->cpu_context.x20 = arg;
	} else {
		struct pt_regs * cur_regs = task_pt_regs(current);
		*childregs = *cur_regs;
		childregs->regs[0] = 0;
		childregs->sp = stack + PAGE_SIZE;
		p->stack = stack;
	}
	p->flags = clone_flags;
	p->priority = current->priority;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail

	p->cpu_context.pc = (uint64_t)ret_from_fork;
	p->cpu_context.sp = (uint64_t)childregs;
	int pid = nr_tasks++;
	task[pid] = p;
	preempt_enable();
	return pid;
}

uint8_t move_to_user_mode(uint64_t pc)
{
	struct pt_regs *regs = task_pt_regs(current);
	memzero((uint64_t)regs, sizeof(*regs));
	regs->pc = pc;
	regs->pstate = PSR_MODE_EL0t;
	uint64_t stack = kmalloc(sizeof(uint64_t)); //allocate new user stack
	if (!stack) {
		return -1;
	}
	regs->sp = stack + PAGE_SIZE;
	current->stack = stack;
	return 0;
}

static struct pt_regs * task_pt_regs(struct job_s *tsk)
{
	uint64_t p = (uint64_t)tsk + THREAD_SIZE - sizeof(struct pt_regs);
	return (struct pt_regs *)p;
}
