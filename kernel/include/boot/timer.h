/**
 * jOSh Kernel
 * @file timer.h
 * @brief Kernel system clock & timer
 * @author Josh Theriault
 * @date 12/24/2021
 */

#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <types.h>

//void KeSetSystemTimerEnabled(BOOL enabled);
//BOOL KeIsSystemTimerEnabled(void);

//DWORD KeGetSystemTickCount(void);

//#define KeEnableSystemTimer() KeSetSystemTimerEnabled(true)
//#define KeDisableSystemTimer() KeSetSystemTimerEnabled(false)

void KiEnableClock();
void KiDisableClock();

#endif