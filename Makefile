SUBDIRS = core
TOPTARGETS := all clean

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS) CFLAGS=$(CFLAGS)

qemu:
	qemu-system-aarch64  -m 1024 -machine virt -cpu cortex-a57 -kernel core/core.bin -nographic -display none -serial mon:stdio

.PHONY: $(TOPTARGETS) $(SUBDIRS)