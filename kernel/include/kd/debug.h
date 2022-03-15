/**
 * jOSh Kernel
 * @file debug.h
 * @brief Kernel debugging utilities
 * @author Josh Theriault
 * @date 12/31/2021
 */

#ifndef KERNEL_DEBUG_H
#define KERNEL_DEBUG_H

#include <types.h>

typedef enum _klog_level
{
	KLOG_LEVEL_TRACE,
	KLOG_LEVEL_DEBUG,
	KLOG_LEVEL_INFO,
	KLOG_LEVEL_WARN,
	KLOG_LEVEL_ERROR,
	KLOG_LEVEL_FATAL
} KLOG_LEVEL;

void KdPrint(CSTR str);
void KdTitle(CSTR str);
void KdPrintf(CSTR fmt, ...);
#define KdPrintLine(str) KdPrint(str"\n")

void KdLog(KLOG_LEVEL level, CSTR str);
void KdLogf(KLOG_LEVEL level, CSTR str, ...);

#define KLOG_TRACE(str) KdLog(KLOG_LEVEL_TRACE, (str))
#define KLOGF_TRACE(str, ...) KdLogf(KLOG_LEVEL_TRACE, (str), __VA_ARGS__)

#define KLOG_DEBUG(str) KdLog(KLOG_LEVEL_DEBUG, (str))
#define KLOGF_DEBUG(str, ...) KdLogf(KLOG_LEVEL_DEBUG, (str), __VA_ARGS__)

#define KLOG_INFO(str) KdLog(KLOG_LEVEL_INFO, (str))
#define KLOGF_INFO(str, ...) KdLogf(KLOG_LEVEL_INFO, (str), __VA_ARGS__)

#define KLOG_WARN(str) KdLog(KLOG_LEVEL_WARN, (str))
#define KLOGF_WARN(str, ...) KdLogf(KLOG_LEVEL_WARN, (str), __VA_ARGS__)

#define KLOG_ERROR(str) KdLog(KLOG_LEVEL_ERROR, (str))
#define KLOGF_ERROR(str, ...) KdLogf(KLOG_LEVEL_ERROR, (str), __VA_ARGS__)

#define KLOG_FATAL(str) KdLog(KLOG_LEVEL_FATAL, (str))
#define KLOGF_FATAL(str, ...) KdLogf(KLOG_LEVEL_FATAL, (str), __VA_ARGS__)

#endif