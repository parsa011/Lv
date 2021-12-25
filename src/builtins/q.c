/*
 *	editor quite command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

static bool quite_all = false;
static bool quite_force = false;

const char *quite_args[] = {
	"a",
	"!"
};

/*
 *	recieve given args from command , and find inserted
 *	args with a loop :))
 */
void parse_args(char **args)
{
	while (*args) {
		if (strcmp(*args,"!") == 0)
			quite_force = true;
		else if (strcmp(*args,"a") == 0)
			quite_all = true;
		*args++;
	}
}

/*
 *	quite buffer or close editor sometimes
 *	possible argumants for quit :
 *	- a : close all windows
 *	- ! : dont save changed buffer , and force close
 *	NOTE : args should be separated by space in prompt :))
 */
int quit(int f, char **args)
{
	parse_args(args);
	if (quite_all) {
		// TODO : right now we dont care about force quite in quite all mode
		while (remove_window(curwp) != ALONEWINDOW);
		close_editor(true,EXIT_SUCCESS);
	} else {
		if (curbp->dirty && !quite_force) {
			showmsg(true,"buffer is dity, use 'q !' to force quite");
			return false;
		}
		if (remove_window(curwp) == ALONEWINDOW)
			close_editor(true,EXIT_SUCCESS);
	}
	return true;
}
