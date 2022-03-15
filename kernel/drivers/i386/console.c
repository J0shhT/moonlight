#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/console.h>

typedef volatile void* io_udata;
typedef void(*io_ofunc)(io_udata, size_t, char, uint8_t);

//#include "vga.h"

struct io_terminal
{
	io_udata ud;
	io_ofunc uf;
	struct
	{
		uint8_t x;
		uint8_t y;
	} cursor;
	uint8_t width;
	uint8_t height;
	struct
	{
		enum vga_color foreground;
		enum vga_color background;
	} colors;
};

static struct io_terminal terminal;
void terminal_init()
{
	terminal.ud = (io_udata)VGAT_BUFFER_ADDR;
	terminal.uf = &vgat_ofunc;
	terminal.cursor.x = 0;
	terminal.cursor.y = 0;
	terminal.width = VGAT_BUFFER_COLS;
	terminal.height = VGAT_BUFFER_ROWS;
	terminal.colors.foreground = 0xF;
	terminal.colors.background = 0x0;
}

void terminal_clear()
{
	vgat_style style = VGAT_STYLE(terminal.colors.background, terminal.colors.background);
	for (size_t i = 0; i < terminal.width * terminal.height; ++i)
		(*terminal.uf)(terminal.ud, i, ' ', style);
	terminal.cursor.x = 0;
	terminal.cursor.y = 0;
}

void terminal_foreground(uint8_t color)
{
	terminal.colors.foreground = (enum vga_color)(color & 0x0F);
}

void terminal_background(uint8_t color)
{
	terminal.colors.background = (enum vga_color)(color & 0x0F);
}

static inline void _terminal_validate()
{
	if (terminal.cursor.x >= terminal.width)
	{
		++terminal.cursor.y;
		terminal.cursor.x = 0;
	}
	if (terminal.cursor.y >= terminal.height)
	{
		// TODO: scrolling support
		terminal.cursor.y = 0;
		terminal.cursor.x = 0;
	}
}

static inline void _terminal_newline()
{
	++terminal.cursor.y;
	terminal.cursor.x = 0;
	_terminal_validate();
}

static inline void _terminal_move_cursor(unsigned int n)
{
	terminal.cursor.x += n;
	_terminal_validate();
}

void terminal_putc(char c)
{
	if (c == '\n') return _terminal_newline();
	if (c == '\t') return _terminal_move_cursor(4);

	vgat_style style = VGAT_STYLE(terminal.colors.foreground, terminal.colors.background);
	size_t offset = terminal.cursor.y * terminal.width + terminal.cursor.x;
	(*terminal.uf)(terminal.ud, offset, c, style);

	_terminal_move_cursor(1);
}

void terminal_puts(const char *s)
{
	char c;
	while ((c = *s++)) terminal_putc(c);
	terminal_putc('\n');
}