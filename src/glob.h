#ifndef _GLOB_H
# define _GLOB_h

/*
 *	Global Informations and options will be stored in this struct
 *	We will have just one instnace of it and no more.
 *  Every time that we change a global options or in every window size change
 *	this informations will be updated
 */
struct editor_t {
	int term_row, term_col;
	int tty_in;
	bool show_tabs;
};

/*
 * Global editor struct to use anywhere :))) :)))
 */
public editor global_editor;

public window current_window;
public buffer *current_buffer;

#define cursor_row (current_window.cursor_pos.row)
#define cursor_col (current_window.cursor_pos.col)

#endif
