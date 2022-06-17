/*
 *	editor set command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "../types.h"

/*
 *	available option that we have to set command for now :)
 */
static const char *options[] = {
    "nu", 			/* activate line number for current buffer 	*/
    "syntax",		/* toggle syntax highlight for buffer 		*/
    "tabstop"		/* set tab size for editor 					*/
};

/*
 *	we have some options that will take arguments like tabstop and shiftwidth and ..
 * 	they will stored here , then we can distinguish them between those that do not
 * 	take any argument
 */
static const char *argy_options[] = {
    "tabstop"
};

/*
 *	instead of check given option name , to run associated function
 * 	we store functions here , then we can find them by their associated option name
 * 	in options array (they must be in same order :D)
 */
static int (*options_db[])(int,int) = {
    toggle_linenumber,
    toggle_highligth,
    set_tab_size
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

bool is_argy_option(char *s)
{
    for (int i = 0; i < ARRAY_LENGTH(argy_options); i++) {
        if (strcmp(argy_options[i],s) == 0)
            return true;
    }
    return false;
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
    if (is_argy_option(*args++)) {
        if (!*args || **args++ != '=' || !*(args)) {
            showmsg(false,"invalid set command");
            return false;
        }
        /* for now we just have int values */
        int value = atoi(*args);
        (*options_db[i])(true,value);
    } else 
        (*options_db[i])(true,1);

    return true;
}
