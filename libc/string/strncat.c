#include <string.h>

char *strncat(char *restrict dest, const char *restrict src, size_t n)
{
	char *destPtr = dest;
	for (; *dest != '\0'; ++dest);
	for (; n > 0 && (*dest = *src) != '\0'; ++dest, ++src, --n);
	if (*dest != '\0') *(++dest) = '\0';
	return destPtr;
}