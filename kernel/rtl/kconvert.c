#include <rtl/convert.h>

SIZE RtlIntegerToString(STR destStr, SIZE destSize, INT value, INT base)
{
	SIZE size = 0;
	STR pStr = destStr;
	CHAR *pStart, *pEnd;
	UINT uvalue = value;

	if (destSize == 1)
		*pStr = '\0';
	if (destSize <= 1)
		return 0;

	--destSize;
	if (value < 0)
	{
		*(pStr++) = '-';
		++size, --destSize;
		++destStr;
		uvalue = -value;
	}

	if (destSize == 0)
	{
		*pStr = '\0';
		return size;
	}

	do
	{
		INT remainder = uvalue % base;
		*(pStr++) = remainder < 10 ? '0'+remainder : 'a'+(remainder-10);
		++size, --destSize;
	}
	while ((uvalue /= base) && destSize > 0);
	*pStr = '\0';

	pStart = destStr, pEnd = pStr - 1;
	while (pStart < pEnd)
	{
		CHAR c = *pStart;
		*pStart = *pEnd;
		*pEnd = c;
		++pStart;
		--pEnd;
	}

	return size;
}

SIZE RtlUnsignedToString(STR destStr, SIZE destSize, UINT value, INT base)
{
	SIZE size = 0;
	STR pStr = destStr;
	CHAR *pStart, *pEnd;

	if (destSize == 1)
		*pStr = '\0';
	if (destSize <= 1)
		return 0;

	--destSize;
	if (destSize == 0)
	{
		*pStr = '\0';
		return size;
	}

	do
	{
		INT remainder = value % base;
		*(pStr++) = remainder < 10 ? '0'+remainder : 'a'+(remainder-10);
		++size, --destSize;
	}
	while ((value /= base) && destSize > 0);
	*pStr = '\0';

	pStart = destStr, pEnd = pStr - 1;
	while (pStart < pEnd)
	{
		CHAR c = *pStart;
		*pStart = *pEnd;
		*pEnd = c;
		++pStart;
		--pEnd;
	}

	return size;
}