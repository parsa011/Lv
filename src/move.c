/*
 *  Manager cursor and position of screen	
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

	int llen = current_line->len;
	if (dir == MOVE_RIGHT) {
		if (cursor_col < llen) {
			curbp->ccol++;
		} else {
			/*
			 *	ok , at this point , we will check for next line , of it's NULL , so we are at the end of buffer , just return false
			 *	otherwise go to next line and set cursor col to 1
			 */
			if (lnext(current_line) == NULL)
				return false;
			curbp->ccol = 1;
			move_cursor(MOVE_DOWN);
		}
	} else if (dir == MOVE_LEFT) {
			if (cursor_col > 1) {
				curbp->ccol--;
			} else {
				/*
				 * 	well , if prev line is NULL so we are at the top of buffer , we just have to return
				 * 	otherwise we will call move_cursor with move up argumant
				 * 	then set cursor col to next line (it's current line now) length
				 */
				if (lprev(current_line) == NULL)
					return false;
				move_cursor(MOVE_UP);
				curbp->ccol = current_line->len;
			}
	} else if (dir == MOVE_UP) {
		/* check if we not going to windowsbar_start_offset region */
		if (cursor_row - 1 > windowsbar_start_offset) {
			curbp->crow--;
			curbp->clindex--;
			current_line = lprev(current_line);
		}
	} else if (dir == MOVE_DOWN) {
		if (cursor_row + 1 < statusbar_start_offset && curbp->clindex + 1 < curbp->lcount) {
			curbp->crow++;
			curbp->clindex++;
			current_line = lnext(current_line);
		}
	}
	/* if cursor is out the line , set it to line length + 1 */
	if (cursor_col > current_line->len)
		cursor_col = current_line->len + 1;
	TTmove(cursor_row,cursor_col);
	return TRUE;
}

int scroll(int dir, int times)
{

}
