/**
 * jOSh C Standard Library
 * @file <string.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 12/15/2021
 */

// Section 7.24 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);
	void *memmove(void *dest, const void *src, size_t n);
	char *strcpy(char *__restrict dest, const char *__restrict src);
	char *strncpy(char *__restrict dest, const char *__restrict src, size_t n);
	char *strcat(char *__restrict dest, const char *__restrict src);
	char *strncat(char *__restrict dest, const char *__restrict src, size_t n);

	int memcmp(const void *obj1, const void *obj2, size_t n);
	int strcmp(const char *str1, const char *str2);
	int strncmp(const char *str1, const char *str2, size_t n);
	// int strcoll(const char *str1, const char *str2);
	// size_t strxfrm(char *__restrict str1, const char *__restrict str2, size_t n);

	void *memchr(const void *obj, int c, size_t n);
	char *strchr(const char *str, int c);
	char *strrchr(const char *str, int c);
	// size_t strspn(const char *str, const char *span);
	// size_t strcspn(const char *str, const char *span);
	// char *strpbrk(const char *str, const char *search);
	// char *strstr(const char *str, const char *search);
	// char *strtok(char *__restrict str, const char *__restrict seperator);

	void *memset(void *dest, int c, size_t n);
	// char *strerror(int err);
	size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif