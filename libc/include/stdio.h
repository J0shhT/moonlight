/**
 * jOSh C Standard Library
 * @file <stdio.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 12/15/2021
 */

// Section 7.21 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

	int printf(const char *__restrict format, ...);
	int putchar(int c);
	int puts(const char *str);

	typedef char *va_list;
	int vprintf(const char *__restrict format, va_list args);

#ifdef __cplusplus
}
#endif

#endif