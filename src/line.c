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
