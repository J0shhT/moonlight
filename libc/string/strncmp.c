#include <string.h>

int strncmp(const char *str1, const char *str2, size_t n)
{
	char c1, c2;
	while (n > 0 && (c1 = *(str1++), c2 = *(str2++), (c1 != '\0' || c2 != '\0')))
	{
		int diff = (unsigned char)c1 - (unsigned char)c2;
		if (diff != 0)
			return diff;
		--n;
	}
	return 0;
}