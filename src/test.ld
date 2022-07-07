ENTRY(vectors_start)
SECTIONS
{
 . = 0x10000;
 .text : {
 startup.o
 *(.text .rodata)
 }
 .data : { *(.data) }
 .bss : { *(.bss) }
 . = ALIGN(8);
 . = . + 0x4000; /* 16kB of stack memory */
 stack_top = .;
 . = . + 0x4000; /* 16kB of irq stack memory */
 irq_stack_top = .;
}