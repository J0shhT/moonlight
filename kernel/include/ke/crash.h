/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/include/ke/crash.h
 * PURPOSE:		Crash support functions for the kernel
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/05/2022
 */

#ifndef KERNEL_CRASH_H
#define KERNEL_CRASH_H

#include <api.h>

// Forward declarations
#ifndef ARCH_I386_INTERRUPT_H
typedef struct _ktrap_frame KTRAP_FRAME;
#endif

/*
typedef struct _ke_crash_extras {
	DWORD_PTR arg1;
	DWORD_PTR arg2;
	DWORD_PTR arg3;
} KCRASH_EXTRAS;
*/

typedef union _ke_crash_extras {
	struct {
		DWORD_PTR arg1;
		DWORD_PTR arg2;
		DWORD_PTR arg3;
	};
	DWORD_PTR args[3];
} KCRASH_EXTRAS;

#define MAX_CRASH_EXTRAS_COUNT (sizeof(KCRASH_EXTRAS) / sizeof(DWORD_PTR))

_Force_Inline void KeInitializeCrashExtras(KCRASH_EXTRAS *extras)
{
	(extras->arg1 = 0), (extras->arg2 = 0), (extras->arg3 = 0);
}

void KiInitializeSystemCrashMessages(void);
KSTATUS KeRegisterCrashMessage(SIZE messageId, CSTR messageStr);

_No_Return void KeSystemCrash(DWORD err);
_No_Return void KiSystemCrash(DWORD err, KCRASH_EXTRAS *extras, KTRAP_FRAME *ctx);
#define KeSystemCrashCtx(err, ctx) KiSystemCrash((err), NULL, (ctx))

_No_Return void KeSystemCrashVa(DWORD err, KTRAP_FRAME *ctx, SIZE argc, ...);
#define KeSystemCrashCtxEx(err, ctx, ...) KeSystemCrashVa((err), (ctx), VA_LENGTH(__VA_ARGS__), __VA_ARGS__)
#define KeSystemCrashEx(err, ...) KeSystemCrashVa((err), NULL, VA_LENGTH(__VA_ARGS__), __VA_ARGS__)

#endif