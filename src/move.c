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
	if (curbp->cline == NULL)
		die("NULL");
	int llen = curbp->cline->len;
	if (dir == MOVE_RIGHT) {
		if (cursor_col < llen) {
			curbp->ccol++;
		} else {
			curbp->ccol = 1;
			move_cursor(MOVE_DOWN);
		}
	} else if (dir == MOVE_LEFT) {
			if (cursor_col > 0) {
				curbp->ccol--;
			} else
				move_cursor(MOVE_UP);
	} else if (dir == MOVE_UP) {
		if (cursor_row - 1 > windowsbar_start_offset) {
			curbp->crow--;
			curbp->cline = lprev(curbp->cline);
		}
	} else if (dir == MOVE_DOWN) {
		if (cursor_row + 1 < statusbar_start_offset && cursor_row - windowsbar_start_offset < curbp->lcount) {
			curbp->crow++;
			curbp->cline = lnext(curbp->cline);
		}
	}
	TTmove(cursor_row,cursor_col);
	return TRUE;
}
