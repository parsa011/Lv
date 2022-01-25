#ifndef _MOVE_H
# define _MOVE_H

/*
 *	Manager cursor and position of screen	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/* move directions */
#define MOVE_LEFT 	1
#define MOVE_RIGHT 	2
#define MOVE_UP 	3
#define MOVE_DOWN 	4

/* states of move */
#define OUTOFBUFFER 	0x001
#define TOPOFBUFFER		0x002
#define ENDOFBUFFER		0x003
#define EMPTYBUFFER		0x004

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
void move_cursor();

/*
 * after move between lines , it will convert cursor col to offset and offset ro cursor col
 */
void update_position();

/*
 *	check and control cursor to not be in unexpected regions like windowsbar and ...
 */
void check_cursor();

/*
 *	scroll page , into given direction , to specified times
 */
int scroll(int, int);

/*
 *	check if we we can scroll the page or no
 *	if cursor row - 1 goes to windowsbar section so we can sroll top
 *	else if cursor + 1 goes to statusbar section so we can scroll down :)
 */
bool can_scroll(int);

/*
 *	set next line to current line and increase cursor row
 */
int move_nextline(int, int);

int move_prevline(int, int);

/*
 * 	move cursor by 1 char to next
 */
int next_char(int, int);

int prev_char(int, int);

/*
 *	move cursor if current char in line is tab 
 */
bool jump_tab(int);

#endif
