/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/include/api.h
 * PURPOSE:		Header for public kernel APIs
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/05/2022
 */

#ifndef KERNEL_API_H
#define KERNEL_API_H

#include <types.h>
#include <status.h>

#define _No_Return __attribute__((__noreturn__))
#define _Force_Inline __attribute__((__always_inline__)) inline

#define VA_LENGTH(...) __VA_LENGTH(0, ## __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __VA_LENGTH(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N


#endif