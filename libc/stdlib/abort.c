#include <stdio.h>
#include <stdlib.h>

#ifdef __is_libk
#include <arch/i386/intrinsic.h>
#endif
 
__attribute__((__noreturn__))
void abort(void)
{
#ifdef __is_libk
	// TODO: Add proper kernel panic.
	// printf("kernel: panic: abort()\n");
	__hang_processor();
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	// printf("abort()\n");
	__builtin_trap();
#endif
	while (1) { }
	__builtin_unreachable();
}