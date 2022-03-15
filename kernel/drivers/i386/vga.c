#include <drivers/i386/vga.h>
#include <arch/i386/intrinsic.h>

static DWORD_PTR VgapBase = 0;
static DWORD VgapWidth = 0;
static DWORD VgapHeight = 0;
static DWORD VgapDepth = 0;
static DWORD VgapPitch = 0;

void VgaInitialize(DWORD width, DWORD height)
{
	VgapBase = 0xb8000;
	VgapWidth = width, VgapHeight = height, VgapDepth = 2;
	VgapPitch = width * VgapDepth;

	// Disable hardware cursor
	__IRQ_SAVE();
	__outportb(0x3D4, 0x0A);
	__outportb(0x3D5, 0x20);
	__IRQ_RESTORE();
}

static inline DWORD VgapCalculateOffset(DWORD row, DWORD col)
{
	return VgapPitch * row + VgapDepth * col;
}

static inline volatile BYTE *VgapGetPointer(DWORD row, DWORD col, DWORD offset)
{
	volatile BYTE *vm = (volatile BYTE*)VgapBase;
	return &vm[VgapCalculateOffset(row, col)+offset];
}

void VgaWriteCharacter(DWORD row, DWORD col, BYTE character)
{
	*VgapGetPointer(row, col, 0) = character;
}

BYTE VgaReadCharacter(DWORD row, DWORD col)
{
	return *VgapGetPointer(row, col, 0);
}

void VgaWriteAttribute(DWORD row, DWORD col, BYTE attribute)
{
	*VgapGetPointer(row, col, 1) = attribute;
}

BYTE VgaReadAttribute(DWORD row, DWORD col)
{
	return *VgapGetPointer(row, col, 1);
}

void VgaWriteRow(DWORD row, BYTE character, BYTE attribute)
{
	volatile BYTE *rowPtr = VgapGetPointer(row, 0, 0);
	for (DWORD x = 0; x < VgapWidth; ++x)
	{
		*(rowPtr++) = character;
		*(rowPtr++) = attribute;
	}
}

void VgaWriteRowCharacters(DWORD row, BYTE character)
{
	volatile BYTE *rowPtr = VgapGetPointer(row, 0, 0);
	for (DWORD x = 0; x < VgapWidth; ++x)
	{
		*(rowPtr++) = character;
		++rowPtr;
	}
}

void VgaCopyRow(DWORD destRow, DWORD srcRow)
{
	volatile BYTE *destPtr = VgapGetPointer(destRow, 0, 0);
	volatile const BYTE *srcPtr = VgapGetPointer(srcRow, 0, 0);
	for (DWORD x = 0; x < VgapPitch; ++x)
	{
		*(destPtr++) = *(srcPtr++);
	}
}

void VgaSwapRow(DWORD row1, DWORD row2)
{
	volatile BYTE *row1Ptr = VgapGetPointer(row1, 0, 0);
	volatile BYTE *row2Ptr = VgapGetPointer(row2, 0, 0);
	for (DWORD x = 0; x < VgapPitch; ++x)
	{
		volatile BYTE temp = *row1Ptr;
		*(row1Ptr++) = *row2Ptr;
		*(row2Ptr++) = temp;
	}
}