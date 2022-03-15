/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/boot/kmain.c
 * PURPOSE:		Main entry point for the kernel
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/05/2022
 */

#include <assert.h>

#include <drivers/video.h>
#include <boot/timer.h>
#include <kd/debug.h>
#include <ke/crash.h>

#include <boot/i386/gdt.h>
#include <boot/i386/idt.h>
#include <boot/i386/interrupt.h>
#include <arch/i386/intrinsic.h>

void kernel_trap(KTRAP_FRAME *ctx)
{
	UNUSED(ctx);
	volatile BYTE scan = __inportb(0x60);
	if (!(scan & 0x80))
	{
		//KLOGF_DEBUG("%u\n", (unsigned int)scan);
		if (scan == 2) // 1 key
			KeSystemCrash(0xDEAD);
		else if (scan == 3) // 2 key
			__builtin_trap();
		else if (scan == 11) // 0 key
			assert(1 == 2);
	}
}

#include <drivers/i386/serial.h>


/**
 * This is called during early system startup!
 * The current state of the system is as follows:
 * - The stack is initialized for 16KiB.
 * - Interrupts are disabled.
 * - Global C/C++ constructors have been called.
 */
void kernel_main()
{
	KdPrint("\n ");
	KdTitle("Project Moonlight");
	KdPrintLine("\n Build " __DATE__ " " __TIME__);
	KdPrintLine(" Created by JoshT\n");

	KiSetInterruptRoutine(1, kernel_trap);
	KiEnableClock();

	__sti();

	// KLOG_DEBUG("Testing");

	// VidSetBackColor(VID_COLOR_WHITE);
	// VidSetTextColor(VID_COLOR_BLACK);
	// puts(" Global Descriptor Table \n");
	// VidSetBackColor(VID_COLOR_TRANSPARENT);
	// VidSetTextColor(VID_COLOR_WHITE);

	// KdDumpGdtEntry(0x00);
	// KdDumpGdtEntry(0x08);
	// KdDumpGdtEntry(0x10);

	// VidSetBackColor(VID_COLOR_WHITE);
	// VidSetTextColor(VID_COLOR_BLACK);
	// puts(" Interrupt Descriptor Table \n");
	// VidSetBackColor(VID_COLOR_TRANSPARENT);
	// VidSetTextColor(VID_COLOR_WHITE);

	// KdDumpIdtEntry(0x0);
	// KdDumpIdtEntry(0x6);
	// KdDumpIdtEntry(0xD);
}