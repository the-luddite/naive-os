CROSS=arm-none-eabi

all:
	$(CROSS)-gcc -mcpu=arm926ej-s -c src/startup.s -o obj/startup.o
	$(CROSS)-gcc -mcpu=arm926ej-s -c src/idivmod.s -o obj/idivmod.o
	$(CROSS)-gcc -mcpu=arm926ej-s -c src/crt.s -o obj/crt.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/test.c -o obj/test.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/uart.c -o obj/uart.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/timer.c -o obj/timer.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/div.c -o obj/div.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/console.c -o obj/console.o
	$(CROSS)-gcc -I.. -c -mcpu=arm926ej-s -g src/interrupts.c -o obj/interrupts.o
	pushd obj && $(CROSS)-ld -T ../src/test.ld startup.o test.o uart.o crt.o idivmod.o div.o timer.o interrupts.o console.o -o ../bin/test.elf && popd
	$(CROSS)-objcopy -O binary bin/test.elf bin/test.bin

qemu:
	qemu-system-arm -M versatilepb -m 128M -nographic -display none -serial mon:stdio -kernel bin/test.bin

.PHONY: clean
clean:
	rm -r obj/* bin/*