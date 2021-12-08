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
	curbp = init_buffer("",NO_NAME_BUFFER,0);
	append_buffer(curbp);
	TTopen();
	if (system("clear") != 0)
		TTbeep();
	//TTeeop();
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

		FILE *fp = fopen(argv[1], "r");
		if (!fp) die("fopen");
		int lines = 0;
		char *line = NULL;
		size_t linecap = 0;
		ssize_t linelen;
		while ((linelen = getline(&line, &linecap, fp)) != -1 && lines < term.t_mrow - 1) {
			while (linelen > 0 && (line[linelen - 1] == '\n' ||	line[linelen - 1] == '\r'))
				linelen--;
			ttputc(line);
			ttputc("\r");
			lines++;
		}
		free(line);
		fclose(fp);
	}

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
