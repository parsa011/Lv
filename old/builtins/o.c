/*
 *	open file command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "../types.h"

#include <dirent.h>

int open_command_tab(int f,int n)
{
	return true;
}

/*
 *	open file command 
 *	structure of open command is like this :
 *	: o filename [.]
 *	if user add '.' to end of line , we will open that file
 *	into current buffer
 */
int open_command(int f, char **args)
{
	if (args[1] == NULL) {
		showmsg(false,"You must enter file name");
		return false;
	}
	if (args[2] != NULL && strcmp(args[2],".") == 0) {
		curbp->flags |= FREDRW;
		load_file_into_buffer(curbp,args[1]);
	}
	else {
    	// first we should initialize new window , after append that window we have to init new buffer for it 
    	// also after inserint that buffer into window , we should load file into new buffer :)
		window *wp = init_window();
		append_window(wp);
		append_buffer(wp,init_buffer(NULL,0,0));
		activate_window(wp);
		load_file_into_buffer(wp->fbuffer,args[1]);
	}
	return true;
}
