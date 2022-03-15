#include <assert.h>

#include <stddef.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <ke/crash.h>
#endif

noreturn void __doassert(const char *expr, const char *file, int line)
{
#ifdef __is_libk
	// We are in kernel mode so crash the system
	KeSystemCrashEx(STATUS_ASSERTION_FAILED, (DWORD_PTR)expr, (DWORD_PTR)file, (DWORD_PTR)line);
#else
	// We are in user mode
	printf("Assertion failed: %s, %s:%d\n", expr, file, line);
	abort();
#endif
}