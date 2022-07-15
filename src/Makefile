CROSS = aarch64-suse-linux

SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.s)
OBJS = $(SRC_C:.c=.o) $(SRC_S:.s=.o) 

all: core.bin

%.o: %.c
	$(CROSS)-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CROSS)-gcc $(CFLAGS) -c $< -o $@


core.bin: $(OBJS)
	$(CROSS)-ld -T linker.lt $(OBJS) -o core.bin

qemu:
	qemu-system-aarch64 -machine virt -cpu cortex-a57 -nographic \
		-kernel core.bin

clean:
	rm -r *.o *.bin

.PHONY: clean