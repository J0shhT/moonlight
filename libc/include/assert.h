/**
 * jOSh C Standard Library
 * @file <assert.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 1/02/2022
 */

// Section 7.2 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

#ifndef _ASSERT_H
#define _ASSERT_H 1

#include <sys/cdefs.h>

#define static_assert _Static_assert

#ifdef __cplusplus
	#undef static_assert
#endif

#ifdef NDEBUG
	#define assert(expr) ((void)(expr))
#else
	extern __attribute__((__noreturn__)) void __doassert(const char*, const char*, int);
	#define assert(expr) ((expr) ? (void)0 : __doassert(#expr, __FILE__, __LINE__))
#endif

#endif