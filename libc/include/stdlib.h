/**
 * jOSh C Standard Library
 * @file <stdlib.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 12/15/2021
 */

// Section 7.22 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

#ifndef _STDLIB_H
#define _STDLIB_H 1
 
#include <sys/cdefs.h>
#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 32767
 
#ifdef __cplusplus
extern "C" {
#endif
	
	// long int strtol(const char *__restrict nptr, char **__restrict endptr, int base);
	// long long int strtoll(const char *__restrict nptr, char **__restrict endptr, int base);
	// unsigned long int strtoul(const char *__restrict nptr, char **__restrict endptr, int base);
	// unsigned long long int strtoull(const char *__restrict nptr, char **__restrict endptr, int base);

	// void *aligned_alloc(size_t alignment, size_t size);
	// void *calloc(size_t nmemb, size_t size);
	// void free(void *ptr);
	// void *malloc(size_t size);
	// void *realloc(void *ptr, size_t size);

	// int rand(void);
	// void srand(unsigned int seed);

	__attribute__((__noreturn__)) void abort(void);
	// __attribute__((__noreturn__)) void exit(int status);
	// __attribute__((__noreturn__)) void quick_exit(int status);
	// __attribute__((__noreturn__)) void _Exit(int status);
	// int atexit(void (*func)(void));
	// int at_quick_exit(void (*func)(void));

	// char *getenv(const char *name);
	// int system(const char *string);

	// void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
	// void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

	// int abs(int j);
	// long int labs(long int j);
	// long long int llabs(long long int j);
	// div_t div(int numer, int denom);
	// ldiv_t ldiv(long int numer, long int denom);
	// lldiv_t lldiv(long long int numer, long long int denom);
 
#ifdef __cplusplus
}
#endif
 
#endif