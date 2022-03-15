#include <stdio.h>
#include <stddef.h>

#if defined(__is_libk)
#include <string.h>
#include <drivers/video.h>
#ifndef NDEBUG
#include <drivers/i386/serial.h>
#endif
#endif
 
int puts(const char* str)
{
#if defined(__is_libk)
	size_t n = strlen(str);
#ifndef NDEBUG
	SePortWriteString(SE_PORT_COM1, str);
#endif
	VidDisplayString(str);
	putchar('\n');
	return n;
#else
	size_t n = 0;
	while (*s != '\0') ++n, putchar(*s++);
	putchar('\n');
	return n;
#endif
}