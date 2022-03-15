#ifndef _KERNEL_TYPES_H
#define _KERNEL_TYPES_H

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef unsigned long LONG_PTR;
typedef unsigned short SHORT_PTR;
typedef LONG_PTR DWORD_PTR;

typedef void *PVOID;
typedef const void *PCVOID;
typedef BYTE *PBYTE;
typedef const BYTE *PCBYTE;
typedef WORD *PWORD;
typedef const WORD *PCWORD;
typedef DWORD *PDWORD;
typedef const DWORD *PCDWORD;

typedef DWORD SIZE;

typedef char CHAR;
typedef CHAR* STR;
typedef const CHAR* CSTR;

typedef unsigned char UCHAR;
typedef UCHAR* USTR;
typedef const UCHAR* UCSTR;

typedef signed int INT;
typedef unsigned int UINT;

typedef signed short SHORT;
typedef unsigned short USHORT;

typedef signed long LONG;
typedef unsigned long ULONG;

#ifdef __bool_true_false_are_defined
	typedef bool BOOL;
#else
	typedef _Bool BOOL;
	#define true 1
	#define false 0
#endif

#ifndef TRUE
	#define TRUE true
#endif
#ifndef FALSE
	#define FALSE false
#endif

#ifndef NULL
	#define NULL ((void*)0)
#endif

#define UNUSED(x) (void)(x)

#endif