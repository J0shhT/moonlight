#include <string.h>

char *strrchr(const char *str, int c)
{
	char *match = NULL;
	do
	{
		if (*str == (char)c)
			match = (char*)str;
	}
	while (*(str++) != '\0');
	return match;
}