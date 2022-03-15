#include <stdarg.h>
#include <stdio.h>

int printf(const char *restrict format, ...)
{
	va_list ap;
	va_start(ap, format);
	int count = vprintf(format, ap);
	va_end(ap);
	return count;
}