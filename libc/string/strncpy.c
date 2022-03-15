#include <string.h>

char *strncpy(char *restrict dest, const char *restrict src, size_t n)
{
	char *destPtr = dest;
	for (; n > 0 && (*dest = *src) != '\0'; ++dest, ++src, --n);
	while (n > 0) *(dest++) = '\0', --n;
	return destPtr;
}