CROSS=arm-none-eabi

all:
	$(CROSS)-as -mcpu=arm926ej-s -g src/startup.s -o obj/startup.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/test.c -o obj/test.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/uart.c -o obj/uart.o
	pushd obj && $(CROSS)-ld -T ../src/test.ld startup.o test.o uart.o -o ../bin/test.elf && popd
	$(CROSS)-objcopy -O binary bin/test.elf bin/test.bin

bin: 

qemu:
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel bin/test.bin

.PHONY: clean
clean:
	rm -r obj/* bin/*