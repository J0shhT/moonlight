#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#include <sys/util.h> // for itoa()

int vprintf(const char *restrict format, va_list args)
{
	char c;
	while ((c = *(format++)) != '\0')
	{
		if (c != '%')
		{
			putchar(c);
			continue;
		}
		c = *(format++);

		bool zeroPad = false;
		int padding = 0;
		if (c == '0')
		{
			zeroPad = true;
			c = *(format++);
		}
		if (c >= '0' && c <= '9')
		{
			padding = c - '0';
			c = *(format++);
		}

		char buffer[34] = { 0 };
		const char *pStart = NULL;
		switch (c)
		{
			case 'd':
			{
				int n = va_arg(args, int);
				itoa(n, buffer, sizeof(buffer), 10);
				pStart = buffer;
				break;
			}
			case 'u':
			{
				unsigned int u = va_arg(args, unsigned int);
				utoa(u, buffer, sizeof(buffer), 10);
				pStart = buffer;
				break;
			}
			case 'x':
			{
				unsigned int u = va_arg(args, unsigned int);
				utoa(u, buffer, sizeof(buffer), 16);
				pStart = buffer;
				break;
			}
			case 's':
			{
				pStart = va_arg(args, const char*);
				assert(pStart != NULL);
				break;
			}
			default:
				putchar(c);
				break;
		}

		if (pStart != NULL)
		{
			const char *pEnd;
			for (pEnd = pStart; *pEnd != '\0'; ++pEnd);
			for (; pEnd < pStart + padding; ++pEnd)
                putchar(zeroPad ? '0' : ' ');
			while (*pStart != '\0')
				putchar(*(pStart++));
		}
	}
	return 0;
}