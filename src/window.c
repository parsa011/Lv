#include "types.h"

public window *window_alloc()
{
	window *win = malloc(sizeof(window));
	if (!win) {
		show_message("Out Of Memory!");
		return NULL;
	}
	return win;
}

public void window_init(window *win)
{
	win->buffer_count = win->current_buffer_index = 0;
	win->first_buffer = buffer_alloc();
	buffer_init(win->first_buffer, NULL);
	reset_pos(win->cursor_pos);
}

public void window_append(window *win)
{
	window *lw = current_window;
	for (; L_LINK_NEXT(lw); lw = L_LINK_NEXT(lw))
		;
	L_LINK_INSERT(lw, win);
}

public buffer *window_get_last_buffer(window *win)
{
	buffer *fb = win->first_buffer;
	for (; L_LINK_NEXT(fb); fb = L_LINK_NEXT(fb))
		;
	return fb;
}

public void window_append_buffer(window *win, buffer *buf)
{
	L_LINK_INSERT(window_get_last_buffer(win), buf);
	win->buffer_count++;
}

public void window_open_new()
{
	window *win = window_alloc();
	window_init(win);
	L_LINK_INSERT(current_window, win);
	current_window = win;
	current_buffer = win->first_buffer;
}

public void window_next()
{
	if (!L_LINK_NEXT(current_window)) {
		show_message("Last Window");
		return;
	}
	current_window = L_LINK_NEXT(current_window);
	current_buffer = current_window->first_buffer;
	tty_move_cursor(current_window->cursor_pos);
	buffer_text_update();
}

public void window_prev()
{
	if (!L_LINK_PREV(current_window)) {
		show_message("First Window");
		return;
	}
	current_window = L_LINK_PREV(current_window);
	current_buffer = current_window->first_buffer;
	tty_move_cursor(current_window->cursor_pos);
	buffer_text_update();
}
