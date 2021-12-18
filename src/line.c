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
	line *ln = calloc(1,sizeof(line));
	/* remove extra 'new line' and 'line feed' characters of end of line */
	while (len > 0 && ((content[len - 1] == '\n' || content[len - 1] == '\r')))
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

/*
 *	alloc and append new line 
 *	if force is true , so we have to add new line without considering curbp->mode
 */
int line_new(int force)
{
	/* TODO : in future we have to show user a message : this macro is available in insert mode and .... */
	if (!(curbp->modes & (MDINST)) && force != true)
		return false;
	line *ln = line_alloc("",0);
	if (current_line == NULL) {
		/* if buffer is null , so we have to append line to buffer , it's gonna be first line */
		append_line(curbp,line_alloc("",0));
	} else {
		/* otherwise , we will break current line into two part , before and after the cursor , 
		 * and move after the cursor part into next line */
		//lv_strncpy(ln->chars,&current_line->chars[curbp->coffset],current_line->len - curbp->coffset);
		ln->chars = strdup(&current_line->chars[curbp->coffset]);
		current_line->chars[curbp->coffset] = '\0';
		current_line->len = strlen(current_line->chars);
		ln->len = strlen(ln->chars);
		/* if next line is not null , set ln next to current_next 
		 * and prev of current_next to ln */
		line *current_next = lnext(current_line);
		if (current_next != NULL) {
			slprev(current_next,ln);	
			slnext(ln,current_next);
		}
		slprev(ln,current_line);
		slnext(current_line,ln);
		cursor_col = 1;
		curbp->coffset = 0;
		move_nextline(0,0);
		curbp->lcount++;
	}
	return true;
}

int line_new_down(int f, int n)
{
	if (gotoeol(1,1) == EMPTYBUFFER)
		return EMPTYBUFFER;
	gotoeol(1,1);
	line_new(true);
	return true;
}

int line_new_up(int f, int n)
{
	if (gotosol(1,1) == EMPTYBUFFER)
		return EMPTYBUFFER;
	gotosol(1,1);
	line_new(true);
	move_prevline(1,1);
	return true;
}

int line_length(line *ln)
{
	int tabs_count = count_tabs(ln->chars,ln->len);
	int res = strlen(ln->chars) + tabs_count * tab_size - tabs_count;
	return res;
}

void line_ins_char(char c)
{
	if (current_line == NULL)
		line_new(true);
	current_line->len++;
	curbp->coffset++;
}
