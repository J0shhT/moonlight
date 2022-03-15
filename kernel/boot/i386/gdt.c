#include <boot/i386/gdt.h>
#include <arch/i386/intrinsic.h>

#include <kd/debug.h>
#include <assert.h>

void KiSetGdtEntryBase(GdtEntry *entry, DWORD base)
{
	entry->BaseLow = base & 0x00FFFFFF;
	entry->BaseHigh = (base & 0xFF000000) >> 24;
}

void KiSetGdtEntryLimit(GdtEntry *entry, DWORD limit)
{
	if (limit < 0x100000)
	{
		entry->Granularity = 0;
	}
	else
	{
		entry->Granularity = 1;
		limit >>= 12;
	}
	entry->LimitLow = (limit & 0x0FFFF);
	entry->LimitHigh = ((limit & 0xF0000) >> 16);
}

void KiSetGdtEntryType(GdtEntry *entry, BYTE type)
{
	assert(type <= 0xF);
	entry->Type = ((type & 0x8) >> 3);
	entry->Executable = ((type & 0x4) >> 2);
	entry->Conforming = ((type & 0x2) >> 1);
	entry->ReadWrite = (type & 0x1);
}

void KiSetGdtEntryMode(GdtEntry *entry, BYTE mode) // 0 = 16-bit, 1 = 32-bit, 2 = 64-bit
{
	assert(mode <= 2);
	entry->LongMode = (mode == 2);
	entry->BigMode = (mode == 1);
	assert(!entry->LongMode || entry->Executable); // long mode implies code segment
}

void KiSetGdtEntryEx(GdtEntry *entry, DWORD base, DWORD limit, BYTE type, BYTE ring, BYTE mode, BYTE granularity)
{
	KiSetGdtEntryBase(entry, base);
	KiSetGdtEntryLimit(entry, limit);
	KiSetGdtEntryType(entry, type);
	entry->Ring = (ring & 0x3);
	KiSetGdtEntryMode(entry, mode);
	entry->Granularity |= !!(granularity);
	entry->Present = type != 0;
	entry->Accessed = 0;
}

void KiSetGdtEntry(GdtEntry *entry, DWORD base, DWORD limit, BYTE type, BYTE ring, BYTE mode)
{
	KiSetGdtEntryEx(entry, base, limit, type, ring, mode, 0);
}

GdtEntry KiGdt[3];
GdtPtr KiGdtr;

void KiLoadGlobalDescriptorTable()
{
	KiSetGdtEntryEx(&KiGdt[0], 0, 0, 0, 0, 0, 0); // NULL descriptor
	KiSetGdtEntry(&KiGdt[1], 0, 0xFFFFFFFF, 0b1101, 0, 1); // Kernel code descriptor (ring 0)
	KiSetGdtEntry(&KiGdt[2], 0, 0xFFFFFFFF, 0b1001, 0, 1); // Kernel data descriptor (ring 0)

	KiGdtr.Limit = (sizeof(KiGdt) - 1);
	KiGdtr.Base = (DWORD_PTR)&KiGdt;
	__lgdt((DWORD_PTR)&KiGdtr);

	KLOG_INFO("Global Descriptor Table has loaded\n");
}

#ifndef NDEBUG

void KdDumpGdtEntry(DWORD selector)
{
	DWORD offset = selector >> 3;
	GdtEntry entry = KiGdt[offset];
	KdPrintf("GDT ENTRY #%u (Selector=0x%x)\n"
		"\tBase=0x%08x\n"
		"\tLimit=0x%08x\n"
		"\tType=%s [%s%s%s] (0x%x)\n"
		"\tRing=%u\n"
		"\tMode=%u\n\n",
		offset, selector,
		(entry.BaseLow | (entry.BaseHigh << 24)),
		(entry.LimitLow | (entry.LimitHigh << 16)),
		(entry.Type ? entry.Executable ? "CODE" : "DATA" : "NULL"),
		(entry.ReadWrite ? entry.Executable ? "R" : "W" : "-"),
		(entry.Conforming ? entry.Executable ? "C" : "D" : "-"),
		(entry.Type ? "T" : "-"),
		(entry.ReadWrite | (entry.Conforming << 1) | (entry.Executable << 2) | (entry.Type << 3)),
		entry.Ring,
		(entry.LongMode | (entry.BigMode << 1) | (entry.Granularity << 2)));
}

#endif