#ifndef _MOVE_H
# define _MOVE_H

/*
 *	calculate that what is cursor column in given line, until that offset
 *	col consist of tab_size for tabs and 1 for other chars
 *	example (tab_size is 4) : 
 *		chars = "ab\t as"
 *		offset  = 3;
 *	output should be :
 *		8
 */
public int offset_to_col(char *chars, int offset);

/*
 *	When we move to another line , it will control cursur column to be in right place
 *	because lines have different sizes and structures
 */
public void control_offset();

/*
 *	go to next line if line is avaiable
 */
public bool next_line();

/*
 *	go to prev line
 */
public bool prev_line();

/*
 *	Move cursor to next char in current line
 */
public bool next_char();

/*
 *	Move cursor to prev char
 */
public bool prev_char();

/*
 *	Go to end of currnet line
 */
public bool go_line_end();

/*
 *	Go to start of current line
 */
public bool go_line_beginning();

/*
 *	scroll page down
 */
public bool page_down();

/*
 *	scroll up on page
 */
public bool page_up();

/*
 *	go to next work
 */
public bool next_word();

/*
 *	move to prev work();
 */
public bool prev_word();

/*
 *	Go To specifit col in current line
 */
public void go_to_col(int col);

#endif
