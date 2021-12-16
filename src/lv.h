#ifndef _LV_H
# define _LV_H

#define EDITOR_VERSION "0.0.1"
#define EDITOR_TITLE "PMS " EDITOR_VERSION

extern int tab_size;

/*
 *	main loop of program , get key and insert it into buffer
 *	or if thats a macro , so execute it
 */
void lv_loop();

/*
 * 	do some basic initialization for editor
 */
void init_editor();

/*
 *	close editor 
 *	but before closing , check for edited buffer to save them or prevent
 *	of closing
 */
int close_editor(int,int);

#endif
