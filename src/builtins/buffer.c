/*
 *	buffers command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

#include <dirent.h>

int buffer_command_tab(int f,int n)
{
	TTmove(buffers_start_offset,1);
	int i = 0;
	for (window *wp = firstwp;i < statusbar_start_offset - windowsbar_start_offset - 2 ;i++) {
		TTeeol();
		if (wp != NULL) {
			for (buffer *bf = wp->fbuffer; bf != NULL; bf = bnext(bf)) {
				TTputs(bf->bname);
				TTputs("\n\r");
			}
			TTputs("===============================================");
			wp = wnext(wp);
		}
		TTputs("\n\r");
	}
	return true;
}

/*
 *	open buffer command
 */
int buffer_command(int f, char **args)
{
}
