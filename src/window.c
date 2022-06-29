#include "types.h"

void window_init(window *win)
{
	win->buffer_count = win->current_buffer_index = 0;
	reset_pos(win->cursor_pos);
}

void window_append(window *win)
{
	window *lw = &current_window;
	for (; L_LINK_NEXT(lw); lw = L_LINK_NEXT(lw))
		;
	L_LINK_INSERT(lw, win);
}

buffer *window_get_last_buffer(window *win)
{
	buffer *fb = &win->first_buffer;
	for (; L_LINK_NEXT(fb); fb = L_LINK_NEXT(fb))
		;
	return fb;
}

void window_append_buffer(window *win, buffer *buf)
{
	L_LINK_INSERT(window_get_last_buffer(win), buf);
	win->buffer_count++;
}

void window_open_new()
{
}
