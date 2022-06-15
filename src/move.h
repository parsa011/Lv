#ifndef _MOVE_H
# define _MOVE_H

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

#endif
