/*
 *	editor set command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 *	available option that we have to set command for now :)
 */
static const char *options[] = {
    "nu", 			/* activate line number for current buffer 	*/
    "syntax",		/* toggle syntax highlight for buffer 		*/
    "tabstop"		/* set tab size for editor 					*/
};

/*
 *	instead of check given option name , to run associated function
 * 	we store functions here , then we can find them by their associated option name
 * 	in options array (they must be in same order :D)
 */
static int (*options_db[])(int,int) = {
    toggle_linenumber,
    toggle_highligth
};

/*
 * showing available options
 */
int set_command_tab(int n, int f)
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
 *	will search in options array , if we found any option that equals to given s
 *  we returns it index , otherwise return -1 that isn't valid index :) so we can find that
 * 	given 's' is not valid option name
 */
int get_option_index(char *s)
{
    for (int i = 0; i < ARRAY_LENGTH(options); i++) {
        if (strcmp(options[i],s) == 0)
            return i;
    }
    return -1;
}

int set_command(int n, char **args)
{
    args++; // skip command name
    if (!*args) {
        showmsg(false,"enter option name");
        return false;
    }
    int i = get_option_index(*args);
    if (i == -1) {
        showmsg(false,"'%s' is not valid option",*args);
        return false;
    }
    if (strcmp(*args++,"tabstop") == 0) {
        if (**args++ != '=' && !*(args)) {
            showmsg(false,"invalid set command");
            return false;
        }
        // we have to create a function for set tab size
        int tab = atoi(*args);
        tab_size = tab;
        redisplay_buffer();
    } else
        (*options_db[i])(1,true);
    return true;
}
