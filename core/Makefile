CROSS = aarch64-suse-linux

SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.S)
OBJS = $(SRC_C:.c=.o) $(SRC_S:.S=.o) 

__CFLAGS = -Iinclude -g -mcpu=cortex-a57 -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only $(CFLAGS)

all: core.bin

%.o: %.c
	$(CROSS)-gcc $(__CFLAGS) -c $< -o $@ 

%.o: %.S
	$(CROSS)-gcc -Iinclude -c $< -o $@


core.bin: $(OBJS)
	# $(CROSS)-ld -T linker.ld $(OBJS) -o core.bin
	$(CROSS)-ld  -T linker.ld $(OBJS) -o core.bin


clean:
	rm -r *.o *.bin

.PHONY: clean