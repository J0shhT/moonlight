#include <string.h>
 
void *memmove(void *dest, const void *src, size_t n)
{
	if (n == 0) return dest;
	// TODO: Is temp needed? Maybe it can just copy from src to dest?
	// ...as there is no restrict specifier optimization?
	unsigned char temp[n];
	size_t i;
	for (i = 0; i < n; ++i)
		temp[i] = ((const unsigned char*)src)[i];
	for (i = 0; i < n; ++i)
		((unsigned char*)dest)[i] = temp[i];
	return dest;
}