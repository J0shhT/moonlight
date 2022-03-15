/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/include/drivers/i386/serial.h
 * PURPOSE:		Serial port kernel driver for x86
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/06/2022
 */

#ifndef ARCH_I386_SERIAL_H
#define ARCH_I386_SERIAL_H

#include <types.h>
#include <status.h>

typedef enum _se_port
{
	SE_PORT_COM1 = 0x3F8,
	SE_PORT_COM2 = 0x2F8,
	SE_PORT_COM3 = 0x3E8,
	SE_PORT_COM4 = 0x2E8
} SE_PORT;

KSTATUS SeInitializePort(SE_PORT port);
BOOL SePortAvailable(SE_PORT port);

BOOL SePortDataReady(SE_PORT port);
BYTE SePortRead(SE_PORT port);

BOOL SePortTransmitReady(SE_PORT port);
void SePortWrite(SE_PORT port, BYTE data);
void SePortWriteBuffer(SE_PORT port, const BYTE *buffer, DWORD size);
DWORD SePortWriteString(SE_PORT port, CSTR str);

#endif