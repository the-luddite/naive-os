#pragma once

#include "aarch64/system.h"
#include "include/types.h"


#define MAX_PROCESS_PAGES   (16)
#define PF_KTHREAD          (2)
#define PRIO_BGND           (15)
#define PRIO_TOP            (0)
#define NR_TASKS			(64)
#define THREAD_SIZE			(4096)
#define NR_OPEN				(256)

#define TASK_RUNNING		(0)
#define TASK_ZOMBIE			(1)


extern struct job_s *current;
extern struct job_s *task[NR_TASKS];
extern int nr_tasks;


struct cpu_context {
	u64 x19;
	u64 x20;
	u64 x21;
	u64 x22;
	u64 x23;
	u64 x24;
	u64 x25;
	u64 x26;
	u64 x27;
	u64 x28;
	u64 fp;
	u64 sp;
	u64 pc;
};

struct job_s {
	struct cpu_context cpu_context;
	long state;	
	long counter;
	long priority;
	long preempt_count;
	unsigned long stack;
	unsigned long flags;
	struct file * filp[NR_OPEN];
};

#define INIT_TASK \
/*cpu_context*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,1, 0, 0, PF_KTHREAD \
}

struct file {
	mode_t f_mode;
	dev_t  f_rdev;
	off_t  f_pos;            // each opened file has its own offset
	unsigned short f_flags;
	unsigned short f_count;
	unsigned short f_reada;
	struct inode * f_inode;
	struct file_operations * f_op;
};

struct inode {
	dev_t           i_dev;
	unsigned long   i_ino;
	umode_t         i_mode;
	nlink_t         i_nlink;
	uid_t           i_uid;
	gid_t           i_gid;
	dev_t           i_rdev;
	off_t           i_size;  // each disk file has one size
	unsigned long   i_blksize;
	unsigned long   i_blocks;
	struct inode_operations * i_op;
	struct super_block * i_sb;
	// ...
};

void preempt_disable(void);
void preempt_enable(void);
void schedule(void);
void cpu_switch_to(struct job_s *, struct job_s *);
void timer_tick();
void schedule_tail(void);
static void switch_to(struct job_s *, int);
static void _schedule(void);
