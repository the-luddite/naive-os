.global _Reset
_Reset:
 LDR sp, =stack_top
 BL core_main
 B .
