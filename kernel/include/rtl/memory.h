/**
 * jOSh Kernel
 * @file kernel/rtl/memory.h
 * @brief Kernel RTL memory functions
 * @author Josh Theriault
 * @date 12/31/2021
 */

#ifndef KERNEL_RTL_MEMORY_H
#define KERNEL_RTL_MEMORY_H

#include <types.h>

void RtlZeroMemory(PVOID buffer, SIZE size);
BOOL RtlIsZeroMemory(PCVOID buffer, SIZE size);

#endif