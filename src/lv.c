/*
 *	Main file of lv editor :)	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

void close_editor()
{
	
}

/*
 *	some basic initilization for editor
 *	like initilize first window and buffer
 *	open tty and clear screen then set cursor to 0,0
 *	and set terminal title :D
 */
void init_editor()
{
	curwp = init_window();
	firstwp = curwp;
	curbp = init_buffer("",NO_NAME_BUFFER,0);
	append_buffer(curbp);
	TTopen();
	if (system("clear") != 0)
		TTbeep();
	TTmove(0,0);
	set_window_title(TERMINAL_TITLE);
	/* update term global vairbale row and col */
	get_screen_size(&term.t_mrow,&term.t_mcol);
}

int main(int argc,char *argv[])
{
	// initialize the terminal , and activate raw mode
	init_editor();
	
	if (argc > 1) {
		load_file_into_buffer(NULL,argv[1]);
	}

		update();
	int c;
	while (1) {
		c = ttgetc();
		if (c == 'j')
			TTmnl();
		else if (c == 'k')
			TTmpl();
		else if (c == 'l')
			TTmfw();
		else if (c == 'h')
			TTmbw();
		if (c == 'q') break;
	}
	return 0;
}
