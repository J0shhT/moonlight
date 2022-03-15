#include <errno.h>

static __errno_t __errno_val = 0;

__errno_t *__errno(void)
{
	return &__errno_val;
}