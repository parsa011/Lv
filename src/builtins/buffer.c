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
			for (buffer *bf = wp->fbuffer; bf != NULL; bf = L_LINK_NEXT(bf)) {
				TTputs(bf->bname);
				TTputs("\r\n");
			}
			wp = L_LINK_NEXT(wp);
		}
	}
	for (int i = 0; i < term.t_mcol;i++)
		TTputs("\u2500"); // ─ char
	return true;
}

/*
 *	open buffer command
 */
int buffer_command(int f, char **args)
{
	if (args[1] == NULL) {
		showmsg(false,"insert buffer name");
		return false;
	}
	if (set_buffer_by_name(args[1]) == false) {
		showmsg(true,"buffer not founded");
		return false;
	}
	return true;
}
