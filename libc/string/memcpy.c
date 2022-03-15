#include <string.h>
 
void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	if (n == 0) return dest;
	for (size_t i = 0; i < n; ++i)
		((unsigned char *restrict)dest)[i] = ((const unsigned char *restrict)src)[i];
	return dest;
}