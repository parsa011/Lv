/*
 *	Manager cursor and position of screen	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int move_cursor(int dir)
{
	if (curbp->lcount == 0)
		return false;
	if (dir == MOVE_RIGHT) {
		next_char();
	} else if (dir == MOVE_LEFT) {
			prev_char();
	} else if (dir == MOVE_UP) {
		/* check if we not going to windowsbar_start_offset region */
		if (cursor_row - 1 > windowsbar_start_offset) {
			move_prevline();
		} else if (cursor_row == windowsbar_start_offset + 1) {
			scroll(MOVE_UP,1);
		}
	} else if (dir == MOVE_DOWN) {
		if (curbp->clindex + 1 >= curbp->lcount)
			return false;
		if (cursor_row + 1 < statusbar_start_offset) {
			move_nextline();
		} else if (cursor_row + 1 == statusbar_start_offset) {
			scroll(MOVE_DOWN,1);
		}
	}
	check_cursor();
	TTmove(cursor_row,cursor_col);

	char temp[180];
	char c =lgetc(current_line,curbp->coffset);
	sprintf(temp,"%d | %d | %d | %c%s",curbp->coffset,cursor_col,current_line->len,c == '\t' ? ' ':c ,c == '\t' ? "\\t":"");
	set_window_title(temp);

	return TRUE;
}

void check_cursor()
{
	if (cursor_row == windowsbar_start_offset || cursor_row <= 0)
		cursor_row = buffers_start_offset;
	if (!cursor_col || !curbp->coffset) {
		cursor_col = 1;
		curbp->coffset = 0;
	}

	/* if cursor is out the line , set it to line length + 1 */
	if (curbp->coffset > current_line->len || cursor_col > line_length(current_line)) {
		cursor_col = line_length(current_line);
		curbp->coffset  = current_line->len;
	}
}

void update_position()
{
	curbp->coffset = convert_cursorcol_to_coffset(current_line->chars,cursor_col);
	cursor_col = convert_coffset_to_cursorcol(current_line->chars,curbp->coffset);
}

/*
 *	scroll down page , to specified times
 */
int scroll(int dir, int times)
{
	if (dir == MOVE_DOWN) {
		if (lnext(curbp->hline) != NULL) {
			curbp->hline = lnext(curbp->hline);
			move_nextline();
		}
	} else if (dir == MOVE_UP) {
		if (lprev(curbp->hline) != NULL) {
			curbp->hline = lprev(curbp->hline);
			move_prevline();
		}
	}
}

/*
 * move cursor to next line
 */
int move_nextline()
{
	if (lnext(current_line) == NULL)
		return false;
	curbp->clindex++;
	cursor_row++;
	current_line = lnext(current_line);
	update_position();
	return true;
}

int move_prevline()
{
	if (lprev(current_line) == NULL)
		return false;
	curbp->clindex--;
	cursor_row--;
	current_line = lprev(current_line);
	update_position();
	return true;
}

int next_char()
{
	if (curbp->coffset < current_line->len) {
		curbp->coffset++;
		cursor_col++;
	} else {
		/*
		 *	ok , at this point , we will check for next line , of it's NULL , so we are at the end of buffer , just return false
		 *	otherwise go to next line and set cursor col to 1
		 */
		if (lnext(current_line) == NULL)
			return false;
		cursor_col = 1;
		curbp->coffset = 0;
		move_cursor(MOVE_DOWN);
	}
	jump_tab(MOVE_RIGHT);
	update_position();
	return true;
}

int prev_char()
{
	jump_tab(MOVE_LEFT);
	if (curbp->coffset > 0) {
		cursor_col--;
		curbp->coffset--;
	} else {
		/*
		 * 	well , if prev line is NULL so we are at the top of buffer , we just have to return
		 * 	otherwise we will call move_cursor with move up argumant
		 * 	then set cursor col to next line (it's current line now) length
		 */
		if (lprev(current_line) == NULL)
			return false;
		move_cursor(MOVE_UP);
		cursor_col = line_length(current_line); 
		curbp->coffset = current_line->len;
	}
	return true;
}

bool jump_tab(int dir)
{
	if (lgetc(current_line,curbp->coffset) == '\t') {
		if (dir == MOVE_RIGHT) {
			cursor_col += tab_size - 1;
		} else if (dir == MOVE_LEFT) {
			cursor_col -= tab_size - 1;
		}
		return true;
	}
	return false;
}
