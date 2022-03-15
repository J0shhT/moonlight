#include <boot/i386/idt.h>
#include <arch/i386/intrinsic.h>
#include <boot/i386/interrupt.h>
#include <boot/i386/pic.h>

#include <kd/debug.h>

#include <string.h>
#include <assert.h>

#define ISR_ENTRY(vec, ...) KiSetIdtEntry(&KiIdt[vec], (DWORD_PTR)__isr ## vec, __VA_ARGS__)
#define IRQ_ENTRY(irq, ...) KiSetIdtEntry(&KiIdt[32+irq], (DWORD_PTR)__irq ## irq, __VA_ARGS__)

void KiSetIdtEntryBase(IdtEntry *entry, DWORD_PTR base)
{
	entry->BaseLow = (base & 0xFFFF);
	entry->BaseHigh = (base & 0xFFFF0000) >> 16;
}

void KiSetIdtEntrySelector(IdtEntry *entry, WORD selector)
{
	entry->Selector = selector;
}

void KiSetIdtEntryGate(IdtEntry *entry, BYTE gate)
{
	assert(gate <= 0xF);
	entry->Gate = (gate & 0xF);
}

void KiSetIdtEntry(IdtEntry *entry, DWORD_PTR base, WORD selector, BYTE gate, BYTE ring)
{
	KiSetIdtEntryBase(entry, base);
	KiSetIdtEntrySelector(entry, selector);
	KiSetIdtEntryGate(entry, gate);
	entry->Ring = (ring & 0x3);
	entry->Reserved = 0;
	entry->Unused = 0;
	entry->Present = 1;
}

IdtEntry KiIdt[48];
IdtPtr KiIdtr;

void KiLoadInterruptDescriptorTable()
{
	// Map the ISRs into the IDT
	ISR_ENTRY(0, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(1, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(2, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(3, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(4, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(5, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(6, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(7, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(8, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(9, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(10, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(11, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(12, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(13, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(14, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(15, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(16, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(17, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(18, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(19, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(20, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(21, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(22, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(23, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(24, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(25, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(26, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(27, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(28, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(29, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(30, 0x08, INTERRUPT_GATE, 0);
	ISR_ENTRY(31, 0x08, INTERRUPT_GATE, 0);

	// Map the IRQs into the IDT
	IRQ_ENTRY(0, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(1, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(2, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(3, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(4, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(5, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(6, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(7, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(8, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(9, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(10, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(11, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(12, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(13, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(14, 0x08, INTERRUPT_GATE, 0);
	IRQ_ENTRY(15, 0x08, INTERRUPT_GATE, 0);
	PicRemapOffsets(32, 40);
	//KiRemapPicInterrupts();

	// Load the IDT into the CPU
	KiIdtr.Limit = (sizeof(KiIdt) - 1);
	KiIdtr.Base = (DWORD_PTR)&KiIdt;
	__lidt((DWORD_PTR)&KiIdtr);

	KLOG_INFO("Interrupt Descriptor Table has loaded\n");
}

#ifndef NDEBUG

void KdDumpIdtEntry(BYTE ivector)
{
	IdtEntry entry = KiIdt[ivector];
	KdPrintf("IDT ENTRY #%u\n"
		"\tBase=0x%08x\n"
		"\tSelector=0x%02x\n"
		"\tGate=%s (0x%x)\n"
		"\tRing=%u\n\n",
		ivector,
		(entry.BaseLow | (entry.BaseHigh << 16)),
		entry.Selector,
		(entry.Gate == TASK_GATE ? "TASK" : entry.Gate == TRAP_GATE ? "TRAP" : "ISR"),
		entry.Gate,
		entry.Ring);
}

#endif