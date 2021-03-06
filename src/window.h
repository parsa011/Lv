#ifndef _WINDOW_H
# define  _WINDOW_H

/*
 *	window (also we can call it tab but i like 'window' more) is main structure to
 *	hold informatino in a window, like buffers, cursor position and ...
 *
 *  +----------------------------------------------------------------------------+
 *  |  window (1) | window (2)                                                   |
 *  |----------------------------------------------------------------------------|
 *  | Texts are here                       | Second buffer                       |
 *  | More text                            | Splited Window                      |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |--------------------------------------|                                     |
 *  |1 This buffer Have Line number ON     |                                     |
 *  |2 some code around here               |                                     |
 *  |3                                     |                                     |
 *  |4 line number 3 is empty line         |                                     |
 *  |                                      |                                     |
 *  |                                      |                                     |
 *  |----------------------------------------------------------------------------|
 *  | main.c | 1231 Line | 24 KB           | common.h | 248 Line | 4 KB          |
 *  +----------------------------------------------------------------------------+
 *	
 *  Args : 
 *		link : Double-linked list to have all windows
 *      first_buffer : it's obvious what it is , points to first buffer to access to its link
 *		current_buffer_index : index of buffer that we focused on in this window
 *		cursor_ps : holds cursor position of window ;/
 */
struct window_t {
	L_LINK(window) link;
	buffer *first_buffer;
	uint8_t buffer_count;
	uint8_t current_buffer_index;
	cursor_position cursor_pos;
};

/*
 *	allocate and return pointer of new window, it will not set anything for window,
 *	for configuring window we need to call 'window_init()'
 */
public window *window_alloc();

/*
 * give a pointer of a window and initialize it's fileds, we will not use of
 * malloc to register new window then return it pointer, we used of stack
 * so we don't need to be worry about freeing stuff anymore.
 */
public void window_init(window *win);

/*
 *	remove current window from list and set another window (next or prev) as current
 *	window and free it
 */
public void window_kill();

/*
 *	return first window by going backward of current_window link
 */
public window *window_first();

/*
 *	append given window to window list
 */
public void window_append(window *win);

/*
 * return pointer of last buffer in given window
 */
public buffer *window_get_last_buffer(window *win);

/*
 * append given buffer to given window with specified type
 * it can be in middle, beginning or end
 */
public void window_append_buffer(window *win, buffer *buf);

/*
 *	create and initialize new window, then set it as current window 
 */
public void window_open_new();

/*
 *	activate next window of current_window
 */
public void window_next();

/*
 *	activate prev window of current_window
 */
public void window_prev();

#endif
