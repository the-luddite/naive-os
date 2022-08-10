CROSS = aarch64-suse-linux

SRC_C = $(wildcard *.c)
OBJS = $(SRC_C:.c=.o)

__CFLAGS = -Iinclude -g -mcpu=cortex-a57 -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only $(CFLAGS)

$(OBJS):%.o: %.c
	$(CROSS)-gcc $(__CFLAGS) -c $< -o $@

clean:
	rm -r *.o

.PHONY: clean