#ifndef _WORD_H
# define _WORD_H

/*
 * 	This file deal with tasks that are related to word
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	check if cursor is in a word or not
 */
int in_word();
/*
 *	move a word , forward 
 */
int forwword();
/*
 *	move a word , backward
 */
int backword();
/*
 *	count of words in specified region or buffer
 */
int word_count();

/* got to end of line */
int gotoeol(int,int);
/* got to start of line */
int gotosol(int,int);
int goto_line(int,int);
int goto_start_of_buffer(int,int);
int goto_end_of_buffer(int,int);
int move_nextpage(int,int);
int move_prevpage(int,int);

int find_sibling(int,int);

#endif
