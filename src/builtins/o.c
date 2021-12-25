/*
 *	open file command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

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
	window *wp = init_window();
	append_window(wp);
	append_buffer(init_buffer(args[1],"",0,0));
	// TODO : check if we got '.' or not , if we got , we should load file into current buffer
	return load_file_into_buffer(curbp,args[1]);
}