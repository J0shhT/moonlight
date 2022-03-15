/**
 * jOSh C System Library
 * @file <sys/util.h>
 * @brief TODO
 * @author Josh Theriault
 * @date 12/18/2021
 */

#ifndef _SYS_UTIL_H
#define _SYS_UTIL_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	int itoa(int n, char *buffer, size_t size, int base);
	int utoa(unsigned int u, char *buffer, size_t size, int base);

#ifdef __cplusplus
}
#endif

#endif