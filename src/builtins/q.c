/*
 *	editor quite command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 *	quite buffer or close editor sometimes
 *	possible argumants for quit :
 *	- a : close all windows
 *	- ! : dont save changed buffer , and force close
 *	NOTE : args should be separated by space in prompt :))
 */
int quit(int f, char **args)
{
	if (args[1] == NULL) {
		if (remove_window(curwp) == ALONEWINDOW)
			close_editor(true,EXIT_SUCCESS);
	} else if (strcmp(args[1],"a") == 0) {
		while (remove_window(curwp) != ALONEWINDOW);
		close_editor(true,EXIT_SUCCESS);
	 }
	return true;
}
