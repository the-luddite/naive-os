CROSS = aarch64-suse-linux
CC = $(CROSS)-gcc

SUBDIRS = syscalls 
TOPTARGETS := all clean

__C := $(wildcard *.c)
__S := $(wildcard *.S)

__C += $(wildcard syscalls/*.c)
__S += $(wildcard syscalls/*.S)

__C += $(wildcard aarch64/*.c)
__S += $(wildcard aarch64/*.S)

OBJECTS = $(__C:.c=.o) $(__S:.S=.o)

__CFLAGS = -Iinclude -g -mcpu=cortex-a57 -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only $(CFLAGS) $(INCLUDE)


all: core.bin

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)


$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS) CFLAGS="$(__CFLAGS)" CC=$(CC)

%.o: %.c
	$(CC) $(__CFLAGS) -c $< -o $@ 

%.o: %.S
	$(CC) $(__CFLAGS) -c $< -o $@

core.bin: $(OBJECTS)
	@echo $(OBJECTS)
	$(CROSS)-ld  -T linker.ld $(OBJECTS) -o core.bin

clean:
	rm -r *.o *.bin

.PHONY: clean