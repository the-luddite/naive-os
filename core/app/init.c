#include "../process.h"
#include "../debug.h"
#include "../include/syscall.h"
#include "ush.h"


static void __init()
{
    ush_start();
}

static void move_to_userspace()
{
    int err = move_to_user_mode((u64)&__init);
}

void userspace_init()
{
    int res = copy_process(PF_KTHREAD, (u64)&move_to_userspace, 0, 0);
	if (res < 0) {
		printf("error while starting kernel process");
		return;
	}
}


