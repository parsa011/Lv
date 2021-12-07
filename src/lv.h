#ifndef _LV_H
# define _LV_H

extern window *curwp;		/* Current window               */
extern buffer *curbp;		/* Current buffer               */

/*
 * 	do some basic initilization for editor
 */
void init_editor();

/*
 *	close editor 
 *	but before closing , check for edited buffer to save them or prevent
 *	of closing
 */
void clsoe_editor();

#endif
