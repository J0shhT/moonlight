#include <rtl/memory.h>

void RtlZeroMemory(PVOID buffer, SIZE size)
{
	if (size == 0) return;
	if (size == sizeof(BYTE))
	{
		*(PBYTE)(buffer) = (BYTE)0;
	}
	else if (size == sizeof(WORD))
	{
		*(PWORD)(buffer) = (WORD)0;
	}
	else if (size == sizeof(DWORD))
	{
		*(PDWORD)(buffer) = (DWORD)0;
	}
	else
	{
		{ // operate in 4 byte increments
			PDWORD ptr = (PDWORD)buffer;
			for (SIZE i = size/sizeof(DWORD); i > 0; --i)
			{
				*(ptr++) = 0;
				size -= sizeof(DWORD);
			}
			buffer = (PVOID)ptr;
		}
		{ // operate in 2 byte increments
			PWORD ptr = (PWORD)buffer;
			for (SIZE i = size/sizeof(WORD); i > 0; --i)
			{
				*(ptr++) = 0;
				size -= sizeof(WORD);
			}
			buffer = (PVOID)ptr;
		}
		{ // operate in 1 byte increments
			PBYTE ptr = (PBYTE)buffer;
			for (SIZE i = 0; i < size; ++i)
			{
				*(ptr++) = 0;
			}
			buffer = (PVOID)ptr;
		}
	}
}

BOOL RtlIsZeroMemory(PCVOID buffer, SIZE size)
{
	if (size == 0) return TRUE;
	if (size == sizeof(BYTE))
	{
		return *(PCBYTE)(buffer) == 0;
	}
	else if (size == sizeof(WORD))
	{
		return *(PCWORD)(buffer) == 0;
	}
	else if (size == sizeof(DWORD))
	{
		return *(PCDWORD)(buffer) == 0;
	}
	else
	{
		{ // operate in 4 byte increments
			PCDWORD ptr = (PCDWORD)buffer;
			for (SIZE i = size/sizeof(DWORD); i > 0; --i)
			{
				if (*(ptr++) != 0) return FALSE;
				size -= sizeof(DWORD);
			}
			buffer = (PCVOID)ptr;
		}
		{ // operate in 2 byte increments
			PCWORD ptr = (PCWORD)buffer;
			for (SIZE i = size/sizeof(WORD); i > 0; --i)
			{
				if (*(ptr++) != 0) return FALSE;
				size -= sizeof(WORD);
			}
			buffer = (PCVOID)ptr;
		}
		{ // operate in 1 byte increments
			PCBYTE ptr = (PCBYTE)buffer;
			for (SIZE i = 0; i < size; ++i)
			{
				if (*(ptr++) != 0) return FALSE;
			}
			buffer = (PCVOID)ptr;
		}
		return TRUE;
	}
}