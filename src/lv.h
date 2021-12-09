#ifndef _LV_H
# define _LV_H

#define EDITOR_VERSION "0.0.1"
#define EDITOR_TITLE "PMS " EDITOR_VERSION

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
