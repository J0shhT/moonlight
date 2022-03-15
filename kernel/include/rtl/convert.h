/**
 * jOSh Kernel
 * @file kernel/rtl/convert.h
 * @brief Kernel RTL conversion functions
 * @author Josh Theriault
 * @date 12/31/2021
 */

#ifndef KERNEL_RTL_STRING_H
#define KERNEL_RTL_STRING_H

#include <types.h>
#include <status.h>

SIZE RtlIntegerToString(STR destStr, SIZE destSize, INT value, INT base);
SIZE RtlUnsignedToString(STR destStr, SIZE destSize, UINT value, INT base);

#endif