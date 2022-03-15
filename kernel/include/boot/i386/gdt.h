/**
 * jOSh Kernel
 * @file gdt.h
 * @brief x86 Global Descriptor Table
 * @author Josh Theriault
 * @date 12/21/2021
 */

#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

#include <types.h>

typedef struct _gdt_entry {
	DWORD LimitLow		: 16;	// Maximum addressable unit (low).
	DWORD BaseLow		: 24;	// Linear address where the segment begins (low).
	DWORD Accessed		: 1;	// Set by the CPU if the segment is accessed.
	DWORD ReadWrite		: 1;	// Read access bit for code segments. Write access bit for data segments.
	DWORD Conforming	: 1;	// Privilege conformance bit for code segments. Direction bit for data segments.
	DWORD Executable	: 1;	// 0 = Data Segment, 1 = Code Segment.
	DWORD Type			: 1;	// 0 = System Segment, 1 = Normal Segment.
	DWORD Ring			: 2;	// Segment privilege level. Ring 0 (kernel mode) to Ring 3 (user mode).
	DWORD Present		: 1;	// 1 = Segment is valid.
	DWORD LimitHigh		: 4;	// Maximum addressable unit (high).
	DWORD Reserved		: 1;	// Reserved for software.
	DWORD LongMode		: 1;	// 1 = 64-bit Mode.
	DWORD BigMode		: 1;	// 0 = 16-bit Mode, 1 = 32-bit Mode.
	DWORD Granularity	: 1;	// 0 = Use byte addressing, 1 = Use page addressing (4KiB).
	DWORD BaseHigh		: 8;	// Linear address where the segment begins (high).
} __attribute__((packed)) GdtEntry;

typedef struct _gdt_ptr {
	DWORD Limit			: 16;	// Size of the GDT in bytes.
	DWORD Base			: 32;	// Linear address where the GDT begins.
} __attribute__((packed)) GdtPtr;

void KiLoadGlobalDescriptorTable();

#ifndef NDEBUG

void KdDumpGdtEntry(DWORD selector);

#endif

#endif