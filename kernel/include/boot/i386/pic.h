/**
 * jOSh Kernel
 * @file arch/i386/pic.h
 * @brief x86 Programmable Interrupt Controller Driver
 * @author Josh Theriault
 * @date 1/02/2022
 */

#ifndef ARCH_I386_PIC_H
#define ARCH_I386_PIC_H

#include <types.h>

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0

#define PIC0_COMMAND PIC_MASTER
#define PIC1_COMMAND PIC_SLAVE
#define PIC0_DATA (PIC_MASTER+1)
#define PIC1_DATA (PIC_SLAVE+1)

#define PIC_EOI 0x20

#define PIC_ICW1_ICW4 0x01
#define PIC_ICW1_SINGLE 0x02
#define PIC_ICW1_INTERVAL4 0x04
#define PIC_ICW1_LEVEL 0x08
#define PIC_ICW1_INIT 0x10

#define PIC_ICW4_80X86 0x1
#define PIC_ICW4_AUTOEOI 0x2
#define PIC_ICW4_BUFSLAVE 0x08
#define PIC_ICW4_BUFMASTER 0x0C
#define PIC_ICW4_NESTED 0x10

#define PIC_OCW3_IRR 0x0A
#define PIC_OCW3_ISR 0x0B

void PicRemapOffsets(BYTE masterOffset, BYTE slaveOffset);
void PicSignalEnd(BYTE irq);
void PicSetMask(BYTE irq);
void PicClearMask(BYTE irq);
WORD PicGetIrr(void);
WORD PicGetIsr(void);
BOOL PicCheckSpuriousIrq(BYTE irq);

#endif