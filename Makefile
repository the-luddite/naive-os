SUBDIRS = core 
TOPTARGETS := all clean
QEMU_ARGS = -m 1024 -machine virt -cpu cortex-a57 -kernel core/core.bin -nographic -display none -serial mon:stdio

INCLUDE := -I$(shell pwd)
INCLUDE += -I$(shell pwd)/core

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS) CFLAGS="$(CFLAGS)" INCLUDE="$(INCLUDE)"

qemu:
	qemu-system-aarch64 $(QEMU_ARGS)

qemu_debug:
	qemu-system-aarch64 $(QEMU_ARGS) -s -S

.PHONY: $(TOPTARGETS) $(SUBDIRS)