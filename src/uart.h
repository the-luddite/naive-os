#pragma once

#include "qemu.h"
#include "stdint.h"

#define UARTDR			(QEMU_VIRT_UART_BASE)
#define UARTFR			(QEMU_VIRT_UART_BASE + 0x018)


typedef volatile struct {
 uint32_t DR;
 uint32_t RSR_ECR;
 uint8_t reserved1[0x10];
 const uint32_t FR;
 uint8_t reserved2[0x4];
 uint32_t LPR;
 uint32_t IBRD;
 uint32_t FBRD;
 uint32_t LCR_H;
 uint32_t CR;
 uint32_t IFLS;
 uint32_t IMSC;
 const uint32_t RIS;
 const uint32_t MIS;
 uint32_t ICR;
 uint32_t DMACR;
} pl011_T;

enum {
 RXFE = 0x10,
 TXFF = 0x20,
};


void print_uart(const char *s);
uint32_t read_uart();