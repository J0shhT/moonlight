/**
 * jOSh Kernel
 * @file intrinsic.h
 * @brief x86 Intrinsic CPU Instructions
 * @author Josh Theriault
 * @date 1/02/2022
 */

#ifndef ARCH_I386_INTRINSIC_H
#define ARCH_I386_INTRINSIC_H

#include <types.h>

#define __FORCEINLINE static __attribute__((__always_inline__)) inline
#define __NORETURN __attribute__((__noreturn__))
#define __ASSEMBLY(...) __asm__ __volatile__ ( __VA_ARGS__ )

void __lgdt(DWORD_PTR gdt); // Load global descriptor table -- defined in gdti.asm
void __lidt(DWORD_PTR idt); // Load interrupt descriptor table -- defined in idti.asm

__FORCEINLINE void __cli(void) { __ASSEMBLY ( "cli" : : : "memory" ); } // Disable interrupts
__FORCEINLINE void __sti(void) { __ASSEMBLY ( "sti" : : : "memory" ); } // Enable interrupts
__FORCEINLINE void __hlt(void) { __ASSEMBLY ( "hlt" : : : "memory" ); } // Wait for next interrupt

__NORETURN void __hang_processor(void); // Infinite hlt+jmp loop with cli -- defined in boot.asm

__FORCEINLINE BOOL __irq_enabled(void) // Are interrupts enabled?
{
    DWORD flags;
    __ASSEMBLY( "pushf" "\n\t" "pop %0"
                : "=g"(flags) );
    return (BOOL)(flags & (1 << 9));
}

typedef DWORD __IRQ_STATE;
__FORCEINLINE __IRQ_STATE __irq_save(void) // Save IRQ state and disable interrupts
{
    __IRQ_STATE flags;
    __ASSEMBLY( "pushf" "\n\t" "cli" "\n\t" "pop %0"
                : "=r"(flags)
                :
                : "memory" );
    return flags;
}
 
__FORCEINLINE void __irq_restore(__IRQ_STATE flags) // Restore IRQ state from previous __irq_save()
{
    __ASSEMBLY( "push %0" "\n\t" "popf"
                :
                : "rm"(flags) 
                : "memory", "cc" );
}

#define __IRQ_SAVE() const volatile __IRQ_STATE __SAVED_IRQ = __irq_save()
#define __IRQ_RESTORE() __irq_restore(__SAVED_IRQ)

__FORCEINLINE void __outportb(WORD port, BYTE val) // Write to I/O port
{
	__ASSEMBLY( "outb %0, %1"
                :
                : "a"(val), "Nd"(port) );
}

__FORCEINLINE BYTE __inportb(WORD port) // Read from I/O port
{
    BYTE val;
    __ASSEMBLY( "inb %1, %0"
                : "=a"(val)
                : "Nd"(port) );
    return val;
}

__FORCEINLINE void __iowait(void) // Wait a tiny amount of time (1 to 4 microseconds)
{
    __outportb(0x80, 0);
}

#endif