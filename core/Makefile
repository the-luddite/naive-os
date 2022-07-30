CROSS = aarch64-suse-linux

SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.s)
OBJS = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 

CFLAGS = -mcpu=cortex-a57 -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only

all: core.bin

%.o: %.c
	$(CROSS)-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CROSS)-gcc $(CFLAGS) -c $< -o $@


core.bin: $(OBJS)
	# $(CROSS)-ld -T linker.lt $(OBJS) -o core.bin
	$(CROSS)-ld  -T linker.lt $(OBJS) -o core.bin


clean:
	rm -r *.o *.bin

.PHONY: clean