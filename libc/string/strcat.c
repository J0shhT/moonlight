#include <string.h>

char *strcat(char *restrict dest, const char *restrict src)
{
	char *destPtr = dest;
	for (; *dest != '\0'; ++dest);
	for (; (*dest = *src) != '\0'; ++dest, ++src);
	return destPtr;
}