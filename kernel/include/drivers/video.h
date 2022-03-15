/**
 * jOSh Kernel
 * @file video.h
 * @brief Kernel video driver
 * @author Josh Theriault
 * @date 1/02/2022
 */

#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <types.h>

typedef enum _vid_color
{
	VID_COLOR_TRANSPARENT,
	VID_COLOR_BLACK,
	VID_COLOR_WHITE,
	VID_COLOR_RED,
	VID_COLOR_YELLOW,
	VID_COLOR_GREEN,
	VID_COLOR_CYAN,
	VID_COLOR_BLUE,
	VID_COLOR_PURPLE,
	VID_COLOR_COUNT_
} VID_COLOR;

void VidInitialize(void);
void VidResetDisplay(void);

DWORD VidGetWidth(void);
DWORD VidGetHeight(void);
VID_COLOR VidGetBackColor(void);
VID_COLOR VidGetTextColor(void);

void VidSetColorsAt(VID_COLOR textColor, VID_COLOR backColor, DWORD x, DWORD y);
void VidSetColors(VID_COLOR textColor, VID_COLOR backColor);
VID_COLOR VidSetTextColorAt(VID_COLOR color, DWORD x, DWORD y);
VID_COLOR VidSetTextColor(VID_COLOR color);
VID_COLOR VidSetBackColorAt(VID_COLOR color, DWORD x, DWORD y);
VID_COLOR VidSetBackColor(VID_COLOR color);

void VidDisplayCharAtEx(char c, DWORD x, DWORD y, VID_COLOR textColor, VID_COLOR backColor);
void VidDisplayCharAt(char c, DWORD x, DWORD y);
void VidDisplayCharEx(char c, VID_COLOR textColor, VID_COLOR backColor);
void VidDisplayChar(char c);

void VidDisplayStringAtEx(CSTR str, DWORD x, DWORD y, VID_COLOR textColor, VID_COLOR backColor);
void VidDisplayStringAt(CSTR str, DWORD x, DWORD y);
void VidDisplayStringEx(CSTR str, VID_COLOR textColor, VID_COLOR backColor);
void VidDisplayString(CSTR str);

void VidFillRect(VID_COLOR color, DWORD x, DWORD y, DWORD w, DWORD h);
void VidFillBack(VID_COLOR color, DWORD x, DWORD y, DWORD w, DWORD h);

void VidCopyLine(DWORD destY, DWORD srcY);
void VidSwapLine(DWORD y1, DWORD y2);
void VidPopLine(DWORD y);
void VidPushLine(DWORD y);

#endif