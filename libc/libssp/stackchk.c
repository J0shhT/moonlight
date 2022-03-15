#include <stdint.h>
#include <stdlib.h>

//TODO: these should be cryptographically secure
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

#ifdef __is_libk
#include <kd/debug.h>
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
#ifdef __is_libk
	KLOG_FATAL("Stack smashing detected!");
#endif
	abort();
}