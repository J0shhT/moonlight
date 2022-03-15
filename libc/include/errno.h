/**
 * jOSh C Standard Library
 * @file <errno.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 12/15/2021
 */

// Section 7.5 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

#ifndef _ERRNO_H
#define _ERRNO_H 1

#include <sys/cdefs.h>

#define EDOM 1
#define EILSEQ 2
#define ERANGE 3

#ifdef __cplusplus
extern "C" {
#endif

	typedef volatile int __errno_t;
	__errno_t *__errno(void);

#ifdef __cplusplus
}
#endif

#define errno (*__errno())

#endif