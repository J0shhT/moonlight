/**
 * jOSh Kernel
 * @file arch/i386/video.h
 * @brief x86 VGA text-mode driver
 * @author Josh Theriault
 * @date 1/02/2022
 */

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H
 
#include <types.h>

void VgaInitialize(DWORD width, DWORD height);

void VgaWriteCharacter(DWORD row, DWORD col, BYTE character);
BYTE VgaReadCharacter(DWORD row, DWORD col);
void VgaWriteAttribute(DWORD row, DWORD col, BYTE attribute);
BYTE VgaReadAttribute(DWORD row, DWORD col);

void VgaWriteRow(DWORD row, BYTE character, BYTE attribute);
void VgaWriteRowCharacters(DWORD row, BYTE character);

void VgaCopyRow(DWORD destRow, DWORD srcRow);
void VgaSwapRow(DWORD row1, DWORD row2);

#endif