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
};

/*
 * Global editor struct to use anywhere :))) :)))
 */
public editor global_editor;

#endif
