#include <kd/debug.h>
#include <rtl/convert.h>

#include <drivers/video.h>
#include <drivers/i386/serial.h>

#include <assert.h>
#include <stddef.h>
#include <stdarg.h>

typedef BOOL(*KDATA_HANDLER)(BYTE data);

static SIZE KdFormatData(KDATA_HANDLER handler, CSTR fmt, va_list args)
{
	SIZE size = 0;
	CHAR c;
	while ((c = *(fmt++)) != '\0')
	{
		if (c != '%')
		{
			if ((*handler)(c)) ++size;
			continue;
		}
		c = *(fmt++);

		BOOL zeroPad = FALSE;
		INT padding = 0;
		if (c == '0')
		{
			zeroPad = TRUE;
			c = *(fmt++);
		}
		if (c >= '0' && c <= '9')
		{
			padding = c - '0';
			c = *(fmt++);
		}

		CHAR buffer[34] = { 0 };
		CSTR pStart = NULL;
		switch (c)
		{
			case 'd': // signed integer
			{
				INT value = va_arg(args, INT);
				RtlIntegerToString(buffer, sizeof(buffer), value, 10);
				pStart = buffer;
				break;
			}
			case 'u': // unsigned integer
			{
				UINT value = va_arg(args, UINT);
				RtlUnsignedToString(buffer, sizeof(buffer), value, 10);
				pStart = buffer;
				break;
			}
			case 'x': // hexadecimal
			{
				UINT value = va_arg(args, UINT);
				RtlUnsignedToString(buffer, sizeof(buffer), value, 16);
				pStart = buffer;
				break;
			}
			case 's': // string
			{
				pStart = va_arg(args, CSTR);
				assert(pStart != NULL); //TODO: replace with a kernel assert
				break;
			}
			default:
				if ((*handler)(c)) ++size;
				break;
		}

		if (pStart != NULL)
		{
			CSTR pEnd;
			for (pEnd = pStart; *pEnd != '\0'; ++pEnd);
			for (; pEnd < pStart + padding; ++pEnd)
			{
                if ((*handler)(zeroPad ? '0' : ' ')) ++size;
			}
			while (*pStart != '\0')
			{
				if ((*handler)(*(pStart++))) ++size;
			}
		}
	}
	return size;
}



static BOOL KdPrintDataHandler(BYTE data)
{
#ifndef NDEBUG
	SePortWrite(SE_PORT_COM1, data);
#endif
	VidDisplayChar((char)data);
	return TRUE;
}

void KdPrintf(CSTR fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	KdFormatData(&KdPrintDataHandler, fmt, ap);
	va_end(ap);
}

void KdPrint(CSTR str)
{
	while (*str != '\0')
	{
		KdPrintDataHandler((BYTE)(*(str++)));
	}
}

void KdTitle(CSTR str)
{
	VID_COLOR backColor = VidSetBackColor(VID_COLOR_WHITE);
	VID_COLOR textColor = VidSetTextColor(VID_COLOR_BLACK);

	VidDisplayChar(' ');
	KdPrint(str);
	VidDisplayChar(' ');

	
	VidSetTextColor(textColor); // Restore previous text color
	VidSetBackColor(backColor); // Restore previous back color
	KdPrintDataHandler('\n');
}



static inline CSTR KdLogLevelLabel(KLOG_LEVEL level)
{
	static CSTR labels[] = {
		"[ TRACE ]", "[ DEBUG ]", "[ INFO  ]",
		"[ WARN  ]", "[ ERROR ]", "[ FATAL ]"
	};
	if (level >= KLOG_LEVEL_TRACE && level <= KLOG_LEVEL_FATAL)
		return labels[level];
	else
		return "[ ????? ]";
}

static BOOL KdLogDataHandler(BYTE data)
{
	SePortWrite(SE_PORT_COM1, data);
	return TRUE;
}

void KdLogf(KLOG_LEVEL level, CSTR str, ...)
{
	SePortWriteString(SE_PORT_COM1, KdLogLevelLabel(level));
	SePortWrite(SE_PORT_COM1, ' ');
	va_list ap;
	va_start(ap, str);
	KdFormatData(&KdLogDataHandler, str, ap);
	va_end(ap);
}

void KdLog(KLOG_LEVEL level, CSTR str)
{
	SePortWriteString(SE_PORT_COM1, KdLogLevelLabel(level));
	SePortWrite(SE_PORT_COM1, ' ');
	while (*str != '\0')
	{
		SePortWrite(SE_PORT_COM1, (BYTE)(*(str++)));
	}
}