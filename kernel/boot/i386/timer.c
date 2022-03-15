#include <arch/i386/intrinsic.h>
#include <boot/i386/interrupt.h>
#include <drivers/video.h>

#include <stdio.h>

void KiSetClockRate(DWORD hz)
{
	DWORD divisor = 1193180 / hz;
	__outportb(0x43, 0x36);
    __outportb(0x40, divisor & 0xFF);
    __outportb(0x40, divisor >> 8);
}

static DWORD KiTickCounter = 0;

void KiHandleTick(KTRAP_FRAME *ctx)
{
	UNUSED(ctx);
	++KiTickCounter;
	VidDisplayCharEx(' ', VID_COLOR_TRANSPARENT, (KiTickCounter % (VID_COLOR_COUNT_-1))+1);
	// terminal_foreground(0xF - (KiTickCounter % 7));
	// putchar('*');
}

void KiEnableClock()
{
	//KiSetClockRate(1);
	KiSetInterruptRoutine(0, KiHandleTick);
}

void KiDisableClock()
{
	KiClearInterruptRoutine(0);
}