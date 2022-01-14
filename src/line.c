/*
 * 	Manager lines
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 *	this routing used for allocate new line , and remove extra new line
 *	and line feed characters from end of line
 */
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

/*
 *	append given line to given buffer 
 *	if its first line , we will set basic configs for buffer and set all of
 *	buffer line pointer into ln
 *	otherwise we will add it into last of buffer
 */
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
		L_LINK_INSERT(buf->lline,ln);
	}
	/* I think this line don't need any comment , but to be sure : increase total count of buffer lines */
	buf->lline = ln;
	buf->lcount++;
	buffer_changed();
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
		L_LINK_INSERT(current_line,ln);
		//line *current_next = L_LINK_NEXT(current_line);
		//if (current_next != NULL) {
		//	L_LINK_PREV(current_next,ln);	
		//	L_LINK_NEXT(ln,current_next);
		//}
		//L_LINK_PREV(ln,current_line);
		//L_LINK_NEXT(current_line,ln);
		cursor_col = 1;
		curbp->coffset = 0;
		move_nextline(true,1);
		curbp->lcount++;
	}
	buffer_changed();
	return true;
}

/*
 *	create new line in bottom of current line , witout break it
 */
int line_new_down(int f, int n)
{
	while (n--) {
		if (gotoeol(1,1) == EMPTYBUFFER)
			return EMPTYBUFFER;
		gotoeol(1,1);
		line_new(true);
	}
	return true;
}

/*
 *	create new line in top of current line , without break it
 */
int line_new_up(int f, int n)
{
	while (n--) {
		if (gotosol(1,1) == EMPTYBUFFER)
			return EMPTYBUFFER;
		gotosol(1,1);
		line_new(true);
		move_prevline(true,1);
	}
	return true;
}

/*
 *	calculate line length with tabs (count every tab as tab_size value)
 */
int line_length(line *ln)
{
	int tabs_count = count_tabs(ln->chars,ln->len);
	int res = strlen(ln->chars) + tabs_count * tab_size - tabs_count;
	return res;
}

/*
 * 	insert char into current position of cursor in line
 *	also if it's the first line in buffer , we have to add one into it.
 *	or if we are out of buffer
 */
void line_ins_char(char c)
{
	if (current_line == NULL)
		line_new(true);
	current_line->len++;
	current_line->chars  = realloc(current_line->chars,current_line->len + 1);
	shift_right(current_line->chars,current_line->len,curbp->coffset);
	lputc(current_line,curbp->coffset,c);
	lputc(current_line,current_line->len,'\0');
	next_char(true,1);
	buffer_changed();
}

/*
 *	append string into end of give line
 */
void line_append(line *ln,char *s,int len)
{
	ln->chars = realloc(ln->chars,ln->len + len + 1);
	memcpy(&ln->chars[ln->len],s,len);
	ln->len += len;
	ln->chars[ln->len] = '\0';
	buffer_changed();
}

/*
 *	delete a character before cursro ( coffset - 1 )
 *	if we are at start of line we will cut current line and 
 *	paste it into the last of prev line , then delete current line
 *	from list
 */
void line_del_char()
{
	if (current_line == NULL)
		line_new(true);
	if (curbp->coffset == 0) {
		line *prev_line = L_LINK_PREV(current_line);
		if (prev_line == NULL)
			return;
		cursor_col = line_length(prev_line) + 1;
		curbp->coffset = prev_line->len;
		line_append(prev_line,current_line->chars,current_line->len);
		line_delete(curbp->clindex);
		buffer_changed();
		return;
	}
	int at = curbp->coffset - 1;
	/* check if we are deleting a tab , if it's a tab , so we decrease cursor col by tab size */
	prev_char(true,1);
	memmove(&current_line->chars[at], &current_line->chars[at + 1], current_line->len - at);
	current_line->len--;
	buffer_changed();
}

void line_del_next()
{
	if (next_char(true,1)) {
		line_del_char();
	}
}

int delete_current_char(int f,int n)
{
	while (n--) {
		if (next_char(true,1)) {
			line_del_char();
		}
	}
	return true;
}

/*
 *	return line by it's index (remember , index , means start from 0 )
 */
line *get_line_by_index(int index)
{
	if (curbp->lcount < index)
		return NULL;
	int i = 0;
	for (line *ln = curbp->fline;ln != NULL;ln = L_LINK_NEXT(ln),i++)
		if (i == index)
			return ln;
	return NULL;
}

/*
 *	remove a line from doubly linked list , set prev lines next to currnet next
 *	then set next lines prev to current prev
 *	and also check for last line or first line , if current line is last
 *	line of buffer , so we will set last line to prev line ,
 *	and also set next line for first line if current line is fline (first line in buffer)
 */
void line_delete(int index)
{
	line *ln = get_line_by_index(index);	
	if (ln == NULL) {
		empty_buffer();
		return;
	}
	line *L_LINK_NEXT = L_LINK_NEXT(ln);
	line *L_LINK_PREV = L_LINK_PREV(ln);
	if (index == 0) {
		if (L_LINK_NEXT == NULL) {
			cursor_col = 1;
			current_line = NULL;
			curbp->lline = NULL;curbp->fline = curbp->hline = current_line;
			goto ret;
		}
		current_line  = L_LINK_NEXT;
		L_LINK_SPREV(current_line,NULL);
		curbp->fline = curbp->hline = current_line;
		goto ret2;
	} else if (index == curbp->lcount - 1) {
		if (L_LINK_PREV == NULL)
			goto ret;
		current_line = L_LINK_PREV;
		if (!can_scroll(MOVE_UP))
			cursor_row--;
		L_LINK_SNEXT(current_line,NULL);
		curbp->lline = current_line;
		goto ret;
	}
	L_LINK_REMOVE(ln);
	current_line = L_LINK_PREV;
	/* if this is header line in buffer , we will set header to prev line */
	if (ln != curbp->hline)
		if (!can_scroll(MOVE_UP))
			cursor_row--;

ret:
	if (curbp->hline == ln) {
		curbp->hline = L_LINK_PREV;
		curbp->loffset--;
	}
	curbp->clindex--;
ret2:
	curbp->lcount--;
	free(ln);
	buffer_changed();
}

int delete_current_line(int f,int n) 
{
	int temp = curbp->clindex;
	line_delete(curbp->clindex);
	if (temp > 0)
		move_nextline(true,1);
	return true;
}
