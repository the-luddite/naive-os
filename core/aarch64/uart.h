#pragma once

#include "aarch64/qemu.h"
#include "memory.h"
#include "stdint.h"

#define UARTDR			(QEMU_VIRT_UART_BASE)
#define UARTFR			(QEMU_VIRT_UART_BASE + 0x018)
#define UART_BUF_MAX    (300)


typedef volatile struct {
 u32 DR;
 u32 RSR_ECR;
 u8 reserved1[0x10];
 const u32 FR;
 u8 reserved2[0x4];
 u32 LPR;
 u32 IBRD;
 u32 FBRD;
 u32 LCR_H;
 u32 CR;
 u32 IFLS;
 u32 IMSC;
 const u32 RIS;
 const u32 MIS;
 u32 ICR;
 u32 DMACR;
} pl011_T;

enum {
 RXFE = 0x10,
 TXFF = 0x20,
};


void print_uart(const char *s);
u32 read_uart();
void put_uart(char c);
void putc_uart( void* p, char c);