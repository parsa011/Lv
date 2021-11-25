#ifndef _TTY_H
# define _TTY_H

/*
 *	TTY interface header file
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	open the tty for use in editor
 *
 *	fflush();
 *
 *	save current state of tty
 *
 *	disable CR/LF/NL input translation
 *	diaable all output processing
 *	disable echo 
 *	set time for tty input
 *
 *	set new tty
 */
void ttopen(void);

/*
 * restore tty to its original mode
 *
 * fflush();
 *
 * restore original tty 
 */
void ttclose(void);


#endif
