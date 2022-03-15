/**
 * jOSh Kernel
 * @file kernel/rtl/string.h
 * @brief Kernel RTL string functions
 * @author Josh Theriault
 * @date 12/31/2021
 */

#ifndef KERNEL_RTL_STRING_H
#define KERNEL_RTL_STRING_H

#include <types.h>

SIZE RtlStringLength(CSTR str);
SIZE RtlStringLengthEx(CSTR str, SIZE max);

#endif