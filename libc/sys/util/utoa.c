#include <sys/util.h>
#include <assert.h>

#include <stdio.h>

int utoa(unsigned int u, char *buffer, size_t size, int base)
{
	int count = 0;
	char* ptr = buffer;
	char *pStart, *pEnd;
	if (size == 1)
		*ptr = '\0';
	if (size <= 1)
		return 0;

	--size;
	if (size == 0)
	{
		*ptr = '\0';
		return count;
	}

	do
	{
		int remainder = u % base;
		*(ptr++) = remainder < 10 ? '0'+remainder : 'a'+(remainder-10);
		++count, --size;
	}
	while ((u /= base) && size > 0);
	*ptr = '\0';

	pStart = buffer, pEnd = ptr - 1;
	while (pStart < pEnd)
	{
		char c = *pStart;
		*pStart = *pEnd;
		*pEnd = c;
		++pStart;
		--pEnd;
	}

	return count;
}