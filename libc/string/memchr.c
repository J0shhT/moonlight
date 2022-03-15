#include <string.h>

void *memchr(const void *obj, int c, size_t n)
{
	const unsigned char *ptr = (const unsigned char*)obj;
	for (size_t i = 0; i < n; ++i)
	{
		if (ptr[i] == (unsigned char)c)
			return (void*)ptr;
	}
	return NULL;
}