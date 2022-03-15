/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/drivers/i386/serial.c
 * PURPOSE:		Serial port kernel driver for x86
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/06/2022
 */

#include <drivers/i386/serial.h>
#include <arch/i386/intrinsic.h>
#include <boot/i386/interrupt.h>

#include <ke/crash.h>
#include <kd/debug.h>

#include <assert.h>

// Initialization status of COM ports
static volatile BOOL SiPortStatus[] = { FALSE, FALSE, FALSE, FALSE };

static inline void SiPortSetAvailable(SE_PORT port, BOOL status)
{
	switch (port)
	{
		case SE_PORT_COM1:
			SiPortStatus[0] = status;
			break;
		case SE_PORT_COM2:
			SiPortStatus[1] = status;
			break;
		case SE_PORT_COM3:
			SiPortStatus[2] = status;
			break;
		case SE_PORT_COM4:
			SiPortStatus[3] = status;
			break;
		default:
			assert(FALSE);
			break;
	}
}

KSTATUS SeInitializePort(SE_PORT port)
{
	/** 
	 * Configure the serial controller chip for the specified COM port
	 */
	__IRQ_SAVE(); // Disable interrupts while writing to I/O ports
	__outportb(port+1, 0x00); // Disable port interrupts

	// Set baud rate to 38400 (115200/3)
	__outportb(port+3, 0x80); // Enable DLAB (Divisor Latch Access Bit)
	__outportb(port+0, 0x03); // Set divisor to 3 (byte low)
	__outportb(port+1, 0x00); // Set divisor to 3 (byte high)

	// Set control settings
	__outportb(port+3, 0x03); // Set to 8-bit data, 1 stop bit, no parity
	__outportb(port+2, 0xC7); // Enable 14-byte FIFO buffering and clear FIFO RX/TX
	__outportb(port+4, 0x0B); // Set DSR (Data Terminal Ready), RTS (Request to Send), and enable IRQs

	// Test if the serial controller chip is working
	__outportb(port+4, 0x1E); // Switch to loopback mode
	__outportb(port+0, 0xAE); // Send byte 0xAE to the transmit buffer
	if (__inportb(port+0) != 0xAE)
	{
		// Did not recieve same data byte on the loopback
		SiPortSetAvailable(port, FALSE); // Mark port as unavailable
		__IRQ_RESTORE(); // Restore interrupts
		return STATUS_ERROR; // Serial controller chip is faulty?
	}

	// Serial port is working correctly
	__outportb(port+4, 0x0B); // Set to normal operation mode
	SiPortSetAvailable(port, TRUE); // Mark port as available

	KLOGF_INFO("Serial port 0x%x has initialized\n", port);
	
	__IRQ_RESTORE(); // Restore interrupts
	return STATUS_SUCCESS;
}

BOOL SePortAvailable(SE_PORT port)
{
	switch (port)
	{
		case SE_PORT_COM1:
			return SiPortStatus[0] == TRUE;
		case SE_PORT_COM2:
			return SiPortStatus[1] == TRUE;
		case SE_PORT_COM3:
			return SiPortStatus[2] == TRUE;
		case SE_PORT_COM4:
			return SiPortStatus[3] == TRUE;
		default:
			return FALSE;
	}
}

BOOL SePortDataReady(SE_PORT port)
{
	if (!SePortAvailable(port)) return FALSE;
	return __inportb(port+5) & 0x01;
}

BYTE SePortRead(SE_PORT port)
{
	if (!SePortAvailable(port))
	{
		// arg1 is the unavailable port, arg2 is the operation (0 = read)
		KeSystemCrashEx(STATUS_PORT_UNAVAILABLE, (DWORD_PTR)port, 0);
		return 0;
	}
	while (SePortDataReady(port) == FALSE) {}
	return __inportb(port);
}

BOOL SePortTransmitReady(SE_PORT port)
{
	if (!SePortAvailable(port)) return FALSE;
	return __inportb(port+5) & 0x20;
}

void SePortWrite(SE_PORT port, BYTE data)
{
	if (!SePortAvailable(port))
	{
		// arg1 is the unavailable port, arg2 is the operation (1 = write)
		KeSystemCrashEx(STATUS_PORT_UNAVAILABLE, (DWORD_PTR)port, 1);
		return;
	}
	while (SePortTransmitReady(port) == FALSE) {}
	__outportb(port, data);
}

void SePortWriteBuffer(SE_PORT port, const BYTE *buffer, DWORD size)
{
	while (size > 0)
	{
		SePortWrite(port, *(buffer++));
		--size;
	}
}

DWORD SePortWriteString(SE_PORT port, CSTR str)
{
	DWORD count = 0;
	while (*str != '\0')
	{
		SePortWrite(port, *(str++));
		++count;
	}
	return count;
}