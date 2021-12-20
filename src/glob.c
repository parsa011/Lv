#include "types.h"

window *firstwp;	/* first window		*/
window *curwp;		/* Current window   */
buffer *curbp;		/* Current buffer   */

int metac = SPEC | 'P';	/* current meta character 		 */
int ctlxc = CONTROL | 'X';	/* current control X prefix char */

int generate_basic_macros()
{
	//change buffer mode functions
	append_macro(init_macro('i',set_insert_mode,MDLOCK,"insert mode"));
	append_macro(init_macro(CONTROL | '[',set_lock_mode,MDINST,"lock mode"));
	append_macro(init_macro('v',set_visual_mode,MDLOCK,"visual mode"));

	append_macro(init_macro(CTRL_KEY('q'),close_editor,(ALLMODES),"close editor"));

	//move keys
	append_macro(init_macro('j',move_nextline,(MDLOCK | MDVISL | MDVIEW),"go to next line"));
	append_macro(init_macro('k',move_prevline,(MDLOCK | MDVISL | MDVIEW),"go to prev line"));
	append_macro(init_macro('l',next_char,(MDLOCK | MDVISL | MDVIEW),"go to next char"));
	append_macro(init_macro('h',prev_char,(MDLOCK | MDVISL | MDVIEW),"go to prev char"));

	// move with arrow keys
	append_macro(init_macro(SPEC | 'B',move_nextline,ALLMODES,"go to next line"));
	append_macro(init_macro(SPEC | 'A',move_prevline,ALLMODES,"go to prev line"));
	append_macro(init_macro(SPEC | 'C',next_char,ALLMODES,"go to next char"));
	append_macro(init_macro(SPEC | 'D',prev_char,ALLMODES,"go to prev char"));


	append_macro(init_macro('w',forwword,(MDLOCK | MDVISL | MDVIEW),"move to next word"));
	append_macro(init_macro('b',backword,(MDLOCK | MDVISL | MDVIEW),"move to back word"));
	append_macro(init_macro('$',gotoeol,(MDLOCK | MDVISL | MDVIEW),"move to end of line"));
	append_macro(init_macro('^',gotosol,(MDLOCK | MDVISL | MDVIEW),"move to start of line"));

	append_macro(init_macro('o',line_new_down,(MDLOCK),"new line down"));
	append_macro(init_macro('O',line_new_up,(MDLOCK),"new line top"));

	return macros_count;
}

/*
 *	after user press key in insert mode , we will check for c
 *	and we do the work related to the entered character
 *	for example if given key is new line , so we will call add
 * 	new line function , or it was backslash , we will delete char and ...
 */
int manage_insert_key(int c)
{
	/* enter */
	if (c == CTRL_KEY('m'))
		line_new(false);
	else if (c == 127)
		line_del_char();
	else if (c == CTRL_KEY('i')) /* tab */
		line_ins_char('\t');
	else if (c == (SPEC | '3'))
		line_del_next();
	else 
		line_ins_char(c);
	return true;
}

/*
 * kill the program
 */
void die(const char *s) {
	ttclose();
	fprintf(stderr, "%s\n", s);
	exit(2);
}
