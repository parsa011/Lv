/*
 *	Main file of lv editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

int tab_size = 4;

void close_editor()
{
	TTeeop();
	TTmove(1,1);
	exit(0);
}

/*
 *	some basic initialization for editor
 *	like initialize first window and buffer
 *	open tty and clear screen then set cursor to 0,0
 *	and set terminal title :D
 */
void init_editor()
{
	curwp = init_window();
	firstwp = curwp;
	curbp = init_buffer("",NO_NAME_BUFFER,0,0);
	append_buffer(curbp);
	TTopen();
	if (system("clear") != 0)
		TTbeep();
	TTmove(0,0);
	set_window_title(TERMINAL_TITLE);
	/* update term global variable row and col */
	get_screen_size(&term.t_mrow,&term.t_mcol);
	generate_basic_macros();
}

int main(int argc,char *argv[])
{
	// initialize the terminal , and activate raw mode
	init_editor();

	if (argc > 1) {
		load_file_into_buffer(NULL,argv[1]);
	}

	int c;
	key_macro *macro = NULL;
	do {
		update();
		c = get_key();
		macro = find_macro(c);
		if (macro != NULL)
			macro->func();
	} while (1);
	return 0;
}
