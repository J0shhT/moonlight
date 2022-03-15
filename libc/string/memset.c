#include <string.h>
 
void *memset(void *dest, int c, size_t n)
{
	if (n == 0) return dest;
	for (size_t i = 0; i < n; ++i)
		((unsigned char*)dest)[i] = (unsigned char)c;
	return dest;
}