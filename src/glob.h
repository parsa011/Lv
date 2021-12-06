#ifndef _GLOB_H
# define _GLOB_H

extern window *curwp;		/* Current window               */
extern buffer *curbp;		/* Current buffer               */

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
