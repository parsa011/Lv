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

int append_line(buffer *buf,line *ln)
{
	buf = buf == NULL ? curbp : buf;
	/* 
	 * if buffer don't have any line , so this is first line
	 * in other hand , we have to append this line to next of last line
	 * and then set last line to this new line
	 */
	if (buf->fline == NULL && buf->lcount == 0) {
		buf->fline = ln;
		buf->cline = ln;
		buf->hline = ln;
	} else {
		slnext(buf->lline,ln);
	}
	/* I think this line don't need any comment , but to be sure : increase total count of buffer lines */
	buf->lline = ln;
	slprev(buf->lline,ln);
	buf->lcount++;
}

int line_length(line *ln)
{
	int tabs_count = count_tabs(ln->chars,ln->len);
	int res = strlen(ln->chars) + tabs_count * tab_size - tabs_count;
	return res;
}
