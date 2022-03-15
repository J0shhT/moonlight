#include <stdio.h>
 
#ifdef __is_libk
#include <drivers/video.h>
#ifndef NDEBUG
#include <drivers/i386/serial.h>
#endif
#endif
 
int putchar(int c)
{
#ifdef __is_libk
	c = (char)c;
#ifndef NDEBUG
	SePortWrite(SE_PORT_COM1, (char)c);
#endif
	VidDisplayChar(c);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return c;
}