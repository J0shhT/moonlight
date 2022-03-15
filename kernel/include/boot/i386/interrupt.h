/**
 * jOSh Kernel
 * @file arch/i386/interrupt.h
 * @brief x86 Interrupt Service Routines
 * @author Josh Theriault
 * @date 1/02/2022
 */

#ifndef ARCH_I386_INTERRUPT_H
#define ARCH_I386_INTERRUPT_H

#include <types.h>

typedef struct _ktrap_frame
{
	DWORD gs, fs, es, ds;
	DWORD edi, esi, ebp, esp, ebx, edx, ecx, eax;
	DWORD ivec, error, eip, cs, eflags;
} KTRAP_FRAME;

typedef void (*KSERVICE_ROUTINE)(KTRAP_FRAME*);

void KiSetInterruptRoutine(DWORD irq, KSERVICE_ROUTINE handler);
#define KiClearInterruptRoutine(irq) KiSetInterruptRoutine((irq), (void*)0)
//void KiRemapPicInterrupts(void);

#define DECLARE_ISR(ivec) extern void __isr ## ivec()
#define DECLARE_IRQ(ivec) extern void __irq ## ivec()

#define TASK_GATE 0x5
#define INTERRUPT_GATE 0xE
#define TRAP_GATE 0xF

DECLARE_ISR(0);
DECLARE_ISR(1);
DECLARE_ISR(2);
DECLARE_ISR(3);
DECLARE_ISR(4);
DECLARE_ISR(5);
DECLARE_ISR(6);
DECLARE_ISR(7);
DECLARE_ISR(8);
DECLARE_ISR(9);
DECLARE_ISR(10);
DECLARE_ISR(11);
DECLARE_ISR(12);
DECLARE_ISR(13);
DECLARE_ISR(14);
DECLARE_ISR(15);
DECLARE_ISR(16);
DECLARE_ISR(17);
DECLARE_ISR(18);
DECLARE_ISR(19);
DECLARE_ISR(20);
DECLARE_ISR(21);
DECLARE_ISR(22);
DECLARE_ISR(23);
DECLARE_ISR(24);
DECLARE_ISR(25);
DECLARE_ISR(26);
DECLARE_ISR(27);
DECLARE_ISR(28);
DECLARE_ISR(29);
DECLARE_ISR(30);
DECLARE_ISR(31);

DECLARE_IRQ(0);
DECLARE_IRQ(1);
DECLARE_IRQ(2);
DECLARE_IRQ(3);
DECLARE_IRQ(4);
DECLARE_IRQ(5);
DECLARE_IRQ(6);
DECLARE_IRQ(7);
DECLARE_IRQ(8);
DECLARE_IRQ(9);
DECLARE_IRQ(10);
DECLARE_IRQ(11);
DECLARE_IRQ(12);
DECLARE_IRQ(13);
DECLARE_IRQ(14);
DECLARE_IRQ(15);

#endif