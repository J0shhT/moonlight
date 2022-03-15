/**
 * jOSh Kernel
 * @file idt.h
 * @brief x86 Interrupt Descriptor Table
 * @author Josh Theriault
 * @date 12/21/2021
 */

#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include <types.h>

typedef struct _idt_entry {
	DWORD BaseLow		: 16;	// Linear address of the ISR entry point (low).
	DWORD Selector		: 16;	// Selector value of a code segment for the ISR.
	DWORD Reserved		: 8;	// Reserved.
	DWORD Gate			: 4;	// Type of gate for this interrupt.
	DWORD Unused		: 1;	// Unused.
	DWORD Ring			: 2;	// Interrupt privilege level. Ring 0 (kernel mode) to Ring 3 (user mode).
	DWORD Present		: 1;	// 1 = Interrupt is valid.
	DWORD BaseHigh		: 16;	// Linear address of the ISR entry point (high).
} __attribute__((packed)) IdtEntry;

typedef struct _idt_ptr {
	DWORD Limit			: 16;	// Size of the IDT in bytes.
	DWORD Base			: 32;	// Linear address where the IDT begins.
} __attribute__((packed)) IdtPtr;

void KiLoadInterruptDescriptorTable();

#ifndef NDEBUG

void KdDumpIdtEntry(BYTE ivector);

#endif

#endif