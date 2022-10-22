#include "ush.h"
#include "../printf.h"
#include "../stdint.h"


void read_uart_dev(char *comm, int *pos)
{
    char buf[300];

    int32_t read = do_sys_read(buf);
    if (!read)
        return;
    for (int i = 0; i < read; i++) {
        comm[*pos] = buf[i];
        (*pos)++;
    }
}

void ush_exec(char *comm)
{
    printf("the command: %s\n", comm);
}


void ush_start()
{
    int p = 0;
    char command[1000];

    while (1) {
        read_uart_dev(command, &p);
        ush_exec(command);
    }
}