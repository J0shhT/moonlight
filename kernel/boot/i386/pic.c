#include <boot/i386/pic.h>
#include <arch/i386/intrinsic.h>

#include <kd/debug.h>

void PicRemapOffsets(BYTE masterOffset, BYTE slaveOffset)
{
	__IRQ_SAVE();

	// Save masks
	BYTE old0 = __inportb(PIC0_DATA);
	BYTE old1 = __inportb(PIC1_DATA);

	// Start initialization sequence (ICW1)
	__outportb(PIC0_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	__iowait();
	__outportb(PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	__iowait();

	// Set IRQ offsets (ICW2)
	__outportb(PIC0_DATA, masterOffset);
	__iowait();
	__outportb(PIC1_DATA, slaveOffset);
	__iowait();

	// Set IRQ communication lines (ICW3)
	__outportb(PIC0_DATA, 0x4);
	__iowait();
	__outportb(PIC1_DATA, 0x2);
	__iowait();

	// Set operation mode (ICW4)
	__outportb(PIC0_DATA, PIC_ICW4_80X86);
	__iowait();
	__outportb(PIC1_DATA, PIC_ICW4_80X86);
	__iowait();

	// Restore masks
	__outportb(PIC0_DATA, old0);
	__outportb(PIC1_DATA, old1);

	__IRQ_RESTORE();
}

void PicSignalEnd(BYTE irq)
{
	if (PicCheckSpuriousIrq(irq))
	{
		// A spurious IRQ occured!
		// Send an EOI to the master if IRQ occured on slave,
		// otherwise, do not send any EOI as it is fake
		if (irq > 7) __outportb(PIC0_COMMAND, PIC_EOI);
	}
	else
	{
		// A real IRQ occured, send the EOI(s)
		if(irq > 7) __outportb(PIC1_COMMAND, PIC_EOI);
		__outportb(PIC0_COMMAND, PIC_EOI);
	}
}

void PicSetMask(BYTE irq)
{
	WORD port;
	BYTE value;
	if(irq < 8)
	{
		port = PIC0_DATA;
	}
	else
	{
		port = PIC1_DATA;
		irq -= 8;
	}
	value = __inportb(port) | (1 << irq);
	__outportb(port, value);
}

void PicClearMask(BYTE irq)
{
	WORD port;
	BYTE value;
	if(irq < 8)
	{
		port = PIC0_DATA;
	}
	else
	{
		port = PIC1_DATA;
		irq -= 8;
	}
	value = __inportb(port) & ~(1 << irq);
	__outportb(port, value);
}

static inline WORD PicpGetIrqRegister(BYTE ocw3)
{
	__outportb(PIC0_COMMAND, ocw3);
    __outportb(PIC1_COMMAND, ocw3);
	__iowait();
    return (__inportb(PIC1_COMMAND) << 8) | __inportb(PIC0_COMMAND);
}

WORD PicGetIrr(void)
{
	return PicpGetIrqRegister(PIC_OCW3_IRR);
}

WORD PicGetIsr(void)
{
	return PicpGetIrqRegister(PIC_OCW3_ISR);
}

static volatile DWORD PicpSpuriousIrqCount = 0;

BOOL PicCheckSpuriousIrq(BYTE irq)
{
	WORD isr = PicGetIsr();
	if ((isr & (1 << irq)) == 0)
	{
		++PicpSpuriousIrqCount;
		KLOGF_DEBUG("Spurious IRQ %u\n", irq);
		return TRUE;
	}
	return FALSE;
}