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
	window *wp = init_window();
	append_window(wp);
	return true;
}
