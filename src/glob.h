#ifndef _GLOB_H
# define _GLOB_H

#define TERMINAL_TITLE "LV - Parsa mahmoudy" /* default title for terminal */
#define NO_NAME_BUFFER "[NO NAME]" 	/* default name for buffers that don't have name */

extern window *firstwp;		/* first window		*/
extern window *curwp;		/* current window   */
extern buffer *curbp;		/* current buffer   */

#define current_line 	curbp->cline
#define cursor_row 		curwp->crow
#define cursor_col 		curwp->ccol

extern int metac;		/* current meta character */
extern int ctlxc;		/* current control X prefix char */

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
