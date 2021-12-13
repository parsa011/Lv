/*
 * 	Manager lines 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

line *line_alloc(char *content,int len)
{
	line *ln = malloc(sizeof(line));
	if (!ln)
		die("Out ou memory i think :)");
	ln->chars = strdup(content);
	ln->len = len;
	return ln;
}

int line_length(line *ln)
{
	int tabs_count = count_tabs(ln->chars,ln->len);
	int res = strlen(ln->chars) + tabs_count * tab_size - tabs_count;
	return res;
}
