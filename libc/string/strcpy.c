#include <string.h>

char *strcpy(char *restrict dest, const char *restrict src)
{
	char *destPtr = dest;
	for (; (*dest = *src) != '\0'; ++dest, ++src);
	return destPtr;
}