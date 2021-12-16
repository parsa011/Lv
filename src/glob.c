#include "types.h"

window *firstwp;	/* first window		*/
window *curwp;		/* Current window   */
buffer *curbp;		/* Current buffer   */

int metac = SPEC | 'P';	/* current meta character 		 */
int ctlxc = CONTROL | 'X';	/* current control X prefix char */

int generate_basic_macros()
{
	append_macro(init_macro(CTRL_KEY('q'),close_editor,(ALLMODES),"close editor"));

	//move keys
	append_macro(init_macro('j',move_nextline,(MDLOCK | MDVISL | MDVIEW),"go to next line"));
	append_macro(init_macro('k',move_prevline,(MDLOCK | MDVISL | MDVIEW),"go to prev line"));
	append_macro(init_macro('l',next_char,(MDLOCK | MDVISL | MDVIEW),"go to next char"));
	append_macro(init_macro('h',prev_char,(MDLOCK | MDVISL | MDVIEW),"go to prev char"));

	//change buffer mode functions 
	append_macro(init_macro('i',set_insert_mode,MDLOCK,"lock mode"));
	append_macro(init_macro(CONTROL | '[',set_lock_mode,MDINST,"lock mode"));
	append_macro(init_macro('v',set_lock_mode,MDINST,"lock mode"));

	return macros_count;
}

/*
 * kill the program
 */
void die(const char *s) {
	ttclose();
	fprintf(stderr, "%s\n", s);
	exit(2);
}
