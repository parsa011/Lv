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
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have
 * funny get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */
struct terminal_t {
	short t_mrow;					/* max number of rows allowable  */
	short t_nrow;					/* current number of rows used   */
	short t_mcol;					/* max Number of columns.        */
	short t_ncol;					/* current Number of columns.    */
	short t_margin;					/* min margin for extended lines */
	short t_scrsiz;					/* size of scroll region "       */
	int t_pause;					/* # times thru update to pause  */
	void (*t_open)(void);			/* open terminal at the start.   */
	void (*t_close)(void);			/* close terminal at end.        */
	//void (*t_kopen)(void);		/* open keyboard                 */
	//void (*t_kclose)(void);		/* close keyboard                */
	int (*t_getchar)();				/* get character from keyboard.  */
	void (*t_putchar)(char *);		/* put character to display.     */
	int (*t_flush) (void);			/* flush output buffers.         */
	void (*t_move)(int, int);		/* move the cursor, origin 0.    */
	void (*t_eeol)(void);			/* erase to end of line.         */
	void (*t_eeop)(void);			/* erase to end of page.         */
	void (*t_beep)(void);			/* beep.                         */
	
	// cursor reposition functions
	void (*t_mnl)(void);			/* move next line 				 */
	void (*t_mpl)(void);			/* move prev line			     */
	void (*t_mfw)(void);			/* move forward					 */
	void (*t_mbw)(void);			/* move backward				 */

	void (*t_chide)(void);			/* hide cursor 					 */
	void (*t_cshow)(void);			/* show cursor					 */ 

	void (*t_rev)(int);				/* set reverse video state       */
	int (*t_rez)(char *);			/* change screen resolution      */
	int (*t_setfor) ();				/* set forground color           */
	int (*t_setback) ();			/* set background color          */
	void (*t_scroll)(int, int,int);	/* scroll a region of the screen */
};

/*	TEMPORARY macros for terminal I/O  */

#define	TTopen		(*term.t_open)
#define	TTclose		(*term.t_close)
#define	TTgetc		(*term.t_getchar)
#define	TTputc		(*term.t_putchar)
#define	TTflush		(*term.t_flush)
#define	TTmove		(*term.t_move)
#define	TTeeol		(*term.t_eeol)
#define	TTeeop		(*term.t_eeop)
#define	TTbeep		(*term.t_beep)
#define TTmnl		(*term.t_mnl)
#define TTmpl		(*term.t_mpl)
#define TTmfw		(*term.t_mfw)
#define TTmbw		(*term.t_mbw)
#define TTchide		(*term.t_chide)
#define TTcshow		(*term.t_cshow)

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
#define OBUFSIZE 128 
extern int obufp; 			/* output buffer index */
extern unsigned char obuf[OBUFSIZE]; /* output buffer */

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

/*	write a character to the output buffer.  If it becomes
 * 	full, call ttflsh()
 */
void ttputc(char *);

extern int have; 			/* set if we have typeahead */
extern unsigned char havec; /* typeahead character */
extern int leave; 			/* set if we're exiting (so don't check for typeahead) */

#endif
