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

/*
 *	flush the output and get the next character from the tty
 * 	
 *	fflush();
 *
 *	read the next input character
 */
int ttgetc(void);
//int ttcheck(void);

/* 
 * 	write a character to the output buffer.  If it becomes
 * 	full, call ttflsh()
 */
extern int obufp; 			/* output buffer index */
extern int obufsiz; 		/* output buffer size */
extern unsigned char *obuf; /* output buffer */

/* 	flush the output buffer and check for typeahead.
 *
 *  write() any characters in the output buffer to the tty and then sleep
 *  for the amount of time it should take for the written characters to get
 *  to the tty.  This is so that any buffering between the editor and the
 *  tty is defeated.  If this is not done, the screen update will not be
 *  able to defer for typeahead.
 *
 *  the best way to do the sleep (possible only on systems with the
 *  setitimer call) is to set a timer for the necessary amount, write the
 *  characters to the tty, and then sleep until the timer expires.
 *
 *  if this can't be done, it's usually ok to 'write' and then to sleep for
 *  the necessary amount of time.  However, you will notice delays in the
 *  screen update if the 'write' actually takes any significant amount of
 *  time to execute (it usually takes none since all it usually does is
 *  write to an operating system output buffer).
 *
 * 	the way we check for typeahead is to put the TTY in nonblocking mode
 *  and attempt to read a character.  If one could be read, the global
 *  variable 'have' is set to indicate that there is typeahead pending and
 *  the character is stored in a single character buffer until ttgetc
 *  is called.  If the global variable 'leave' is set, the check for
 *  typeahead is disabled.  This is so that once the program knows that it's
 *  about to exit, it doesn't eat the first character of your typeahead if
 *  ttflsh gets called.  'leave' should also be set before shell escapes and
 *  suspends.
 */
int ttflsh(void);

extern int have; 			/* set if we have typeahead */
extern unsigned char havec; /* typeahead character */
extern int leave; 			/* set if we're exiting (so don't check for typeahead) */

/*
 * kill the program
 *
 * set the old term by calling ttclose()
 *
 * write error message into screen
 *
 * exit();
 */
void die(const char *);

#endif
