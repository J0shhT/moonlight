#include <rtl/string.h>

SIZE RtlStringLength(CSTR str)
{
	SIZE len = 0;
	while (*(str++) != 0) ++len;
	return len;
}

SIZE RtlStringLengthEx(CSTR str, SIZE max)
{
	SIZE len = 0;
	while (max > 0 && *(str++) != 0) ++len, --max;
	return len;
}