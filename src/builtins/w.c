/*
 *	write buffer command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int write_command_tab(int f, int n)
{
	TTmove(buffers_start_offset,1);
	command *cmd = fcommand;
	int k = 0;
	for (int i = 0;i < statusbar_start_offset - windowsbar_start_offset - 1;i++) {
		TTeeol();
		if (k <= 0) {
			TTputs("w [filepath]");
			k++;
		}
		TTputs("\n\r");
	}
	return true;
}

/*
 *	write buffer into associated file
 *	possible args :
 *	- file path : to specify location of the file , if it's null
 *		we will save into buffer fname
 */
int write_command(int f, char **args)
{
	if (curbp->dirty == 0) {
		showmsg(false,"Buffer not changed");
		return false;
	}
	// get file name of arguments
	if (args[1] != NULL) {
		set_buffer_name(args[1]);
	}
	save_file();
	return true;
}
