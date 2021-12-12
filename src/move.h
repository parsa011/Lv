#ifndef _MOVE_H
# define _MOVE_H

/* move directions */
#define MOVE_LEFT 	1
#define MOVE_RIGHT 	2
#define MOVE_UP 	3
#define MOVE_DOWN 	4

/*
 *  Manager cursor and position of screen	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 * 	it will move curosr in the given direction
 * 	and update buffer current line
 * 	and buffer cursor row and col
 */
int move_cursor(int);

/*
 *	scroll page , into given direction , to specified times
 */
int scroll(int, int);

/*
 *	set next line to current line and increase cursor row
 */
int move_nextline();

int move_prevline();

#endif
