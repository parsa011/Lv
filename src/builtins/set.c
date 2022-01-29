/*
 *	editor set command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

static const char *options[] = {
    "nu", 			/* activate line number for current buffer 	*/
    "syntax"		/* toggle syntax highlight for buffer 		*/
};

static int (*options_db[])(int,int) = {
    toggle_linenumber,
    toggle_highligth
};

int set_command_tab(int n, int f)
{
    return true;
}

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
    (*options_db[i])(1,true);
    return true;
}
