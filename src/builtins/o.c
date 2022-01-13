/*
 *	open file command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

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
	buffer *bf = NULL;
	if (args[2] != NULL && strcmp(args[2],".") == 0) {
		bf = curbp;
		curbp->flags |= FREDRW;
	}
	else {
		window *wp = init_window();
		append_window(wp);
		bf = init_buffer(args[1],"",0,0);
		append_buffer(wp,bf);
		//change_current_buffer(bf);
		activate_window(wp);
	}
	// TODO : check if we got '.' or not , if we got , we should load file into current buffer
	return load_file_into_buffer(bf,args[1]);
}
