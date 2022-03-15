#include <drivers/video.h>
#include <kd/debug.h>

#include <drivers/i386/vga.h>

#include <assert.h>

#define VID_SCROLL_ENABLED

static const BYTE VidpTextColorTable[] = { 0x0, 0x0, 0xF, 0xC, 0xE, 0xA, 0xB, 0x9, 0xD };
static const BYTE VidpBackColorTable[] = { 0x0, 0x0, 0x7, 0x4, 0x6, 0x2, 0x3, 0x1, 0x5 };

static DWORD VidpWidth = 0;
static DWORD VidpHeight = 0;
static DWORD VidpCurrentX = 0;
static DWORD VidpCurrentY = 0;
static VID_COLOR VidpTextColor = 0;
static VID_COLOR VidpBackColor = 0;

void VidInitialize(void)
{
	VidpWidth = 80, VidpHeight = 25;
	VgaInitialize(VidpWidth, VidpHeight);
	VidResetDisplay();
	KLOG_INFO("Video driver has initialized\n");
}

void VidResetDisplay(void)
{
	VidpCurrentX = 0, VidpCurrentY = 0;
	VidSetTextColor(VID_COLOR_WHITE); // Default text color
	VidSetBackColor(VID_COLOR_TRANSPARENT); // Default back color
	VidFillRect(VID_COLOR_BLACK, 0, 0, VidpWidth, VidpHeight); // Clear screen to black
}

DWORD VidGetWidth(void)
{
	return VidpWidth;
}

DWORD VidGetHeight(void)
{
	return VidpHeight;
}



static BYTE VidpConvertColorToVgaAttribute(VID_COLOR textColor, VID_COLOR backColor)
{
	if (textColor >= VID_COLOR_COUNT_ || backColor >= VID_COLOR_COUNT_)
		return 0x00;
	return VidpTextColorTable[textColor] | (VidpBackColorTable[backColor] << 4);
}

static VID_COLOR VidpGetTextColorFromVgaAttribute(BYTE attribute)
{
	BYTE textColorAttr = attribute & 0xF;
	for (DWORD i = 0; i < sizeof(VidpTextColorTable); ++i)
	{
		if (VidpTextColorTable[i] == textColorAttr) return i;
	}
	return VID_COLOR_TRANSPARENT;
}

static VID_COLOR VidpGetBackColorFromVgaAttribute(BYTE attribute)
{
	BYTE backColorAttr = (attribute >> 4) & 0xF;
	for (DWORD i = 0; i < sizeof(VidpBackColorTable); ++i)
	{
		if (VidpBackColorTable[i] == backColorAttr) return i;
	}
	return VID_COLOR_TRANSPARENT;
}



void VidSetColorsAt(VID_COLOR textColor, VID_COLOR backColor, DWORD x, DWORD y)
{
	BYTE currentAttr = VgaReadAttribute(y, x);
	// VID_COLOR currentTextColor = VidpGetTextColorFromVgaAttribute(currentAttr);
	VID_COLOR currentBackColor = VidpGetBackColorFromVgaAttribute(currentAttr);
	if (textColor == VID_COLOR_TRANSPARENT)
		textColor = currentBackColor;
	if (backColor == VID_COLOR_TRANSPARENT)
		backColor = currentBackColor;
	BYTE newAttr = VidpConvertColorToVgaAttribute(textColor, backColor);
	if (newAttr != currentAttr)
	{
		VgaWriteAttribute(y, x, newAttr);
	}
}

void VidSetColors(VID_COLOR textColor, VID_COLOR backColor)
{
	VidpTextColor = textColor, VidpBackColor = backColor;
}

VID_COLOR VidSetTextColorAt(VID_COLOR color, DWORD x, DWORD y)
{
	BYTE currentAttr = VgaReadAttribute(y, x);
	VID_COLOR textColor = VidpGetTextColorFromVgaAttribute(currentAttr);
	VID_COLOR backColor = VidpGetBackColorFromVgaAttribute(currentAttr);
	if (color == VID_COLOR_TRANSPARENT)
		color = backColor;
	BYTE newAttr = VidpConvertColorToVgaAttribute(color, backColor);
	if (newAttr != currentAttr)
	{
		VgaWriteAttribute(y, x, newAttr);
	}
	return textColor;
}

VID_COLOR VidSetTextColor(VID_COLOR color)
{
	VID_COLOR oldColor = VidpTextColor;
	VidpTextColor = color;
	return oldColor;
}

VID_COLOR VidGetTextColor(void)
{
	return VidpTextColor;
}

VID_COLOR VidSetBackColorAt(VID_COLOR color, DWORD x, DWORD y)
{
	BYTE currentAttr = VgaReadAttribute(y, x);
	VID_COLOR textColor = VidpGetTextColorFromVgaAttribute(currentAttr);
	VID_COLOR backColor = VidpGetBackColorFromVgaAttribute(currentAttr);
	if (color == VID_COLOR_TRANSPARENT)
		color = backColor;
	BYTE newAttr = VidpConvertColorToVgaAttribute(textColor, color);
	if (newAttr != currentAttr)
	{
		VgaWriteAttribute(y, x, newAttr);
	}
	return backColor;
}

VID_COLOR VidSetBackColor(VID_COLOR color)
{
	VID_COLOR oldColor = VidpBackColor;
	VidpBackColor = color;
	return oldColor;
}

VID_COLOR VidGetBackColor(void)
{
	return VidpBackColor;
}



void VidDisplayCharAtEx(char c, DWORD x, DWORD y, VID_COLOR textColor, VID_COLOR backColor)
{
	if (x >= VidpWidth)
	{
		x = 0;
		y += 1;
	}
#ifndef VID_SCROLL_ENABLED
	if (y >= VidpHeight)
	{
		y = 0;
	}
#else
	assert(y < VidpHeight);
#endif
	
	VidSetColorsAt(textColor, backColor, x, y);
	VgaWriteCharacter(y, x, c);
}

void VidDisplayCharAt(char c, DWORD x, DWORD y)
{
	VidDisplayCharAtEx(c, x, y, VidpTextColor, VidpBackColor);
}

void VidDisplayCharEx(char c, VID_COLOR textColor, VID_COLOR backColor)
{
#ifdef VID_SCROLL_ENABLED
	if (VidpCurrentY >= VidpHeight)
	{
		VidPopLine(0);
		VidpCurrentY = VidpHeight-1;
	}
#endif

	if (c == '\t')
	{
		VidpCurrentX += 4;
	}
	else if (c != '\n')
	{
		VidDisplayCharAtEx(c, VidpCurrentX, VidpCurrentY, textColor, backColor);
		++VidpCurrentX;
	}

	if (c == '\n' || VidpCurrentX >= VidpWidth)
	{
		VidpCurrentX = 0;
		++VidpCurrentY;
	}

#ifndef VID_SCROLL_ENABLED
	if (VidpCurrentY >= VidpHeight)
	{
		VidpCurrentY = 0;
	}
#endif
}

void VidDisplayChar(char c)
{
	VidDisplayCharEx(c, VidpTextColor, VidpBackColor);
}



void VidDisplayStringAtEx(CSTR str, DWORD x, DWORD y, VID_COLOR textColor, VID_COLOR backColor)
{
	// Save old position
	DWORD oldX = VidpCurrentX, oldY = VidpCurrentY;

	VidpCurrentX = x, VidpCurrentY = y;
	VidDisplayStringEx(str, textColor, backColor);

	// Restore old position
	VidpCurrentX = oldX, VidpCurrentY = oldY;
}

void VidDisplayStringAt(CSTR str, DWORD x, DWORD y)
{
	VidDisplayStringAtEx(str, x, y, VidpTextColor, VidpBackColor);
}

void VidDisplayStringEx(CSTR str, VID_COLOR textColor, VID_COLOR backColor)
{
	while (*str != '\0')
	{
		VidDisplayCharEx(*(str++), textColor, backColor);
	}
}

void VidDisplayString(CSTR str)
{
	VidDisplayStringEx(str, VidpTextColor, VidpBackColor);
}



void VidFillRect(VID_COLOR color, DWORD x, DWORD y, DWORD w, DWORD h)
{
	for (DWORD row = y; row < y+h; ++row)
	{
		if (row >= VidpHeight) break;
		for (DWORD col = x; col < x+w; ++col)
		{
			if (col >= VidpWidth) break;
			VgaWriteCharacter(row, col, ' ');
			VgaWriteAttribute(row, col, VidpConvertColorToVgaAttribute(VID_COLOR_TRANSPARENT, color));
		}
	}
}

void VidFillBack(VID_COLOR color, DWORD x, DWORD y, DWORD w, DWORD h)
{
	for (DWORD row = y; row < y+h; ++row)
	{
		if (row >= VidpHeight) break;
		for (DWORD col = x; col < x+w; ++col)
		{
			if (col >= VidpWidth) break;
			VidSetBackColorAt(color, col, row);
		}
	}
}



void VidCopyLine(DWORD destY, DWORD srcY)
{
	assert(destY != srcY);
	assert(destY < VidpHeight && srcY < VidpHeight);
	VgaCopyRow(destY, srcY);
}

void VidSwapLine(DWORD y1, DWORD y2)
{
	assert(y1 != y2);
	assert(y1 < VidpHeight && y2 < VidpHeight);
	VgaSwapRow(y1, y2);
}

void VidPopLine(DWORD y)
{
	BYTE attr = VidpConvertColorToVgaAttribute(VID_COLOR_TRANSPARENT, VID_COLOR_TRANSPARENT);
	assert(y < VidpHeight);
	VgaWriteRow(y, ' ', attr);
	for (DWORD row = y+1; row < VidpHeight; ++row)
	{
		VgaSwapRow(row, row-1);
	}
}

void VidPushLine(DWORD y)
{
	BYTE attr = VidpConvertColorToVgaAttribute(VID_COLOR_TRANSPARENT, VID_COLOR_TRANSPARENT);
	assert(y < VidpHeight);
	for (DWORD row = VidpHeight-1; row > y; --row)
	{
		VgaCopyRow(row, row-1);
	}
	VgaWriteRow(y, ' ', attr);
}