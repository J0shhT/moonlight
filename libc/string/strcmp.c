#include <string.h>

int strcmp(const char *str1, const char *str2)
{
	char c1, c2;
	while ((c1 = *(str1++), c2 = *(str2++), (c1 != '\0' || c2 != '\0')))
	{
		int diff = (unsigned char)c1 - (unsigned char)c2;
		if (diff != 0)
			return diff;
	}
	return 0;
}