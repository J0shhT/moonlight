#include <string.h>
 
int memcmp(const void *obj1, const void *obj2, size_t n)
{
	for (size_t i = 0; i < n; ++i)
	{
		int diff = ((unsigned char*)obj1)[i] - ((unsigned char*)obj2)[i];
		if (diff != 0)
			return diff;
	}
	return 0;
}