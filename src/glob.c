/*
 *	global functions
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

window *firstwp;	/* first window		*/
window *lastwp;		/* last window 		*/
window *debug_win;	/* debug window 	*/
window *curwp;		/* Current window   */
int wcount;			/* window count		*/
buffer *curbp;		/* Current buffer   */
msg_bag msgbag;		/* global msg bag	*/

int number_stack = 0;

int metac = SPEC | 'P';		/* current meta character 		 */
int ctlxc = CONTROL | 'X';	/* current control X prefix char */

char macro_stack[MAX_MACRO_STACK];
int macro_stack_p = 0;

int generate_basic_macros()
{
	//change buffer mode functions
	append_macro(init_macro('i',"i",set_insert_mode,MDLOCK,"insert mode"));
	append_macro(init_macro(ESCAPE_KEY,"^[",set_lock_mode,MDINST,"lock mode"));
	append_macro(init_macro(ESCAPE_KEY,"^[",refresh_lock_mode,(MDLOCK | MDVIEW),"clear all stacks"));
	append_macro(init_macro('v',"v",set_visual_mode,MDLOCK,"visual mode"));
	append_macro(init_macro(' '," ",set_command_mode,(MDLOCK | MDVIEW),"command mode"));

	append_macro(init_macro(CTRL_KEY('n'),"^N",toggle_linenumber,(ALLMODES),"toggle line number"));
	append_macro(init_macro(CTRL_KEY('q'),"^Q",close_editor,(ALLMODES),"close editor"));

	//move keys
	append_macro(init_macro('j',"j",move_nextline,(MDLOCK | MDVISL | MDVIEW),"go to next line"));
	append_macro(init_macro('k',"k",move_prevline,(MDLOCK | MDVISL | MDVIEW),"go to prev line"));
	append_macro(init_macro('l',"l",next_char,(MDLOCK | MDVISL | MDVIEW),"go to next char"));
	append_macro(init_macro('h',"h",prev_char,(MDLOCK | MDVISL | MDVIEW),"go to prev char"));

	// move with arrow keys
	append_macro(init_macro(SPEC | 'B',"FNB",move_nextline,ALLMODES,"go to next line"));
	append_macro(init_macro(SPEC | 'A',"FNA",move_prevline,ALLMODES,"go to prev line"));
	append_macro(init_macro(SPEC | 'C',"FNC",next_char,ALLMODES,"go to next char"));
	append_macro(init_macro(SPEC | 'D',"FND",prev_char,ALLMODES,"go to prev char"));

	append_macro(init_macro('x',"x",delete_current_char,(MDLOCK),"delete char under cursor"));
	append_macro(init_macro('G',"G",goto_line,(MDLOCK | MDVISL | MDVIEW),"goto to line"));
	append_macro(init_macro('w',"w",forwword,(MDLOCK | MDVISL | MDVIEW),"move to next word"));
	append_macro(init_macro('b',"b",backword,(MDLOCK | MDVISL | MDVIEW),"move to back word"));
	append_macro(init_macro(('g' | 'l'),"g-l",gotoeol,(MDLOCK | MDVISL | MDVIEW),"move to end of line"));
	append_macro(init_macro(('g' | 'h'),"g-h",gotosol,(MDLOCK | MDVISL | MDVIEW),"move to start of line"));
	append_macro(init_macro('m',"m",find_sibling,(MDLOCK | MDVISL | MDVIEW),"find current char sibling"));
	append_macro(init_macro(SPEC | '6',"FN6",move_nextpage,(ALLMODES),"move to next page"));
	append_macro(init_macro(SPEC | '5',"FN5",move_prevpage,(ALLMODES),"move to next page"));
	// g | g dosnt mean , we have to inhance it
	append_macro(init_macro(('g' | 'g'),"g-g",goto_start_of_buffer,(MDLOCK | MDVISL | MDVIEW),"move to beginnig page"));
	append_macro(init_macro(('g' | 'G'),"g-G",goto_end_of_buffer,(MDLOCK | MDVISL | MDVIEW),"move to end page"));
	append_macro(init_macro(('d' | 'd'),"d-d",delete_current_line,(MDLOCK | MDVIEW),"remove current line"));
	append_macro(init_macro((CTLX | 'N'),"^XN",next_window,(ALLMODES),"go to next window"));
	append_macro(init_macro((CTLX | 'P'),"^XP",prev_window,(ALLMODES),"go to prev window"));

	append_macro(init_macro((CTRL_KEY('w') | 'v'),"^W-v",window_vertinal_split,(MDLOCK | MDVIEW),"vertical window split"));
	append_macro(init_macro((CTRL_KEY('w') | 'j'),"^W-j",next_buffer_in_window,(MDLOCK | MDVIEW),"go to next buffer"));
	append_macro(init_macro((CTRL_KEY('w') | 'k'),"^W-k",prev_buffer_in_window,(MDLOCK | MDVIEW),"go to prev buffer"));

	append_macro(init_macro('o',"o",line_new_down,(MDLOCK),"new line down"));
	append_macro(init_macro('O',"O",line_new_up,(MDLOCK),"new line top"));

	return macros_count;
}

int generate_basic_commands()
{
	append_command(init_command("q",quit,0));
	append_command(init_command("w",write_command,0));
	append_command(init_command("o",open_command,0));
	append_command(init_command("buffer",buffer_command,0));
	append_command(init_command("set",set_command,0));
	return commands_count;
}

void generate_prompt_keys()
{
	change_prompt_key(ESCAPE_KEY,leave_prompt_mode);
	change_prompt_key(ENTER_KEY,prompt_enter_key);
	change_prompt_key(TAB_KEY,prompt_tab_key);
}

void clear_macro_stack()
{
	repeat_char(macro_stack,0,MAX_MACRO_STACK);
	macro_stack_p = 0;

}

int refresh_lock_mode(int f,int n)
{
	number_stack = 0;
	clear_macro_stack();
	clear_prompt();
}

/*
 *	add given key into macro stack to find multi key macros
 */
void add_to_macro_stack(int c)
{
	char key[6];
	cmdstr(c,key);
	int i = 0;
	if (macro_stack_p > 0)
		macro_stack[macro_stack_p++] = '-';
	while (*(key + i)) {
		if (macro_stack_p >= MAX_MACRO_STACK)
			goto no_space;
		macro_stack[macro_stack_p++] = *(key + i++);
	}
	// here we want to check if we have some macro like this or no
	if (compare_macro_name(macro_stack,macro_stack_p) == false) 
		goto no_space;
	if (macro_stack_p >= MAX_MACRO_STACK - 1) {
no_space:
		showmsg(true,"Macro not found : %s",macro_stack);
		clear_macro_stack();
		return;
	}
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
	if (c == ENTER_KEY)
		line_new(false);
	else if (c == 127)
		line_del_char();
	else if (c == TAB_KEY) /* tab */
		line_ins_char('\t');
	else if (c == (SPEC | '3'))
		line_del_next();
	else
		line_ins_char(c);
	return true;
}

void add_to_number_stack(char c)
{
	number_stack = number_stack * 10 + (c - '0');
}

/*
 * kill the program
 */
void die(const char *msg,...) {
	char buf[256];
	va_list ap;
	va_start(ap, msg);
	vsprintf(buf, msg, ap);
	va_end(ap);
	ttclose();
	fprintf(stderr, "%s\n", buf);
	exit(2);
}
