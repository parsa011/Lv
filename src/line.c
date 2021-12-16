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
	/* remove extra 'new line' and 'line feed' characters of end of line */
	while (len > 0 && (content[len - 1] == '\n' || content[len - 1] == '\r'))
		len--;
	content[len] = '\0';

	if (!ln)
		die("Out out memory i think :)");
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
		slprev(ln,buf->lline);
	}
	/* I think this line don't need any comment , but to be sure : increase total count of buffer lines */
	buf->lline = ln;
	buf->lcount++;
}

int line_new()
{
	/* TODO : in future we have to show user a message : this macro is available in insert mode and .... */
	if ((curbp->modes & (MDINST)))
		return false;
	line *ln = line_alloc("",0);
	if (current_line == NULL) {
		append_line(curbp,ln);
		move_nextline(0,0);
		append_line(curbp,line_alloc("",0));
	} else {
		/* at this case we are at the first of line so we have to move current line to down
		 * and set new line prev and next */
		if (curbp->coffset == 0) {
			line *current_prev = lprev(current_line);
			// if prev line is null , so we are at the top of buffer (first line)
			if (current_prev == NULL) {
				curbp->fline = ln;
				curbp->hline = ln;
				slprev(current_line,ln);
				slprev(ln,NULL);
				slnext(ln,current_line);
			} else {
				// else if , we just have to change currnet line and it's next line
				slprev(ln,current_prev);
				slnext(ln,current_line);

				slprev(current_line,ln);
				slnext(current_prev,ln);
			}
			current_line = ln;
			move_nextline(0,0);
		} else {
			/* and at this case , we are away from start of string */

		}
		curbp->lcount++;
	}
}

int line_length(line *ln)
{
	int tabs_count = count_tabs(ln->chars,ln->len);
	int res = strlen(ln->chars) + tabs_count * tab_size - tabs_count;
	return res;
}
