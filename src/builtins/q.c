/*
 *	editor quite command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "../types.h"

static bool quite_all = false;
static bool quite_force = false;

static const char *options[] = {
	"a (remove all buffer and close editor)",
	"! (force quite , ignoring chagnes"
};

int quite_command_tab(int f,int c)
{
	TTmove(buffers_start_offset,1);
	command *cmd = fcommand;
	int k = 0;
	for (int i = 0;i < statusbar_start_offset - windowsbar_start_offset - 1;i++) {
		TTeeol();
		if (k < ARRAY_LENGTH(options)) {
			TTputs(options[k++]);
		}
		TTputs("\n\r");
	}
	return true;
}

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
 *	possible arguments for quit :
 *	- a : close all windows
 *	- ! : dont save changed buffer , and force close
 *	NOTE : args should be separated by space in prompt :))
 */
int quit(int f, char **args)
{
	parse_args(args);
	if (buffer_change_count(curbp) && !quite_force) {
		showmsg(true,"buffer is dirty, use 'q !' to force quite");
		return false;
	}
	if (quite_all) {
		// TODO : right now we dont care about force quite in quite all mode
		while (remove_window(curwp) != ALONEWINDOW);
		close_editor(true,EXIT_SUCCESS);
	} else {
    	/* we cant close reserved buffers */
    	if (is_reserved_buffer_name(curbp->bname))
        	return false;
		if (buffer_change_count(curbp) && !quite_force) {
			showmsg(true,"buffer is dirty, use 'q !' to force quite");
			return false;
		}
		if (curwp->bcount == 1) {
    		if (remove_window(curwp) == ALONEWINDOW)
        		close_editor(true,1);
		} else
    		remove_buffer(curbp);
	}
	return true;
}
