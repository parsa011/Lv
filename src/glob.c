#include "types.h"

window *firstwp;	/* first window		*/
window *lastwp;		/* last window 		*/
window *curwp;		/* Current window   */
buffer *curbp;		/* Current buffer   */
msg_bag msgbag;		/* global msg bag	*/

int number_stack = 0;

prompt_key *fprompt_key;
prompt_key *lprompt_key;

// prompt from message bar statuses
int msgbar_cursor_col = 1;
char msgbar_prompt[PROMPT_MAX_LENGTH];
int msgbar_prompt_p = 0;

int metac = SPEC | 'P';	/* current meta character 		 */
int ctlxc = CONTROL | 'X';	/* current control X prefix char */

char macro_stack[MAX_MACRO_STACK];
int macro_stack_p = 0;

int generate_basic_macros()
{
	//change buffer mode functions
	append_macro(init_macro('i',set_insert_mode,MDLOCK,"insert mode"));
	append_macro(init_macro(ESCAPE_KEY,set_lock_mode,MDINST,"lock mode"));
	append_macro(init_macro(ESCAPE_KEY,refresh_lock_mode,(MDLOCK | MDVIEW),"clear all stacks"));
	append_macro(init_macro('v',set_visual_mode,MDLOCK,"visual mode"));
	append_macro(init_macro(':',set_command_mode,(MDLOCK | MDVIEW),"command mode"));

	append_macro(init_macro(CTRL_KEY('n'),toggle_linenumber,(ALLMODES),"toggle line number"));
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


	append_macro(init_macro('x',delete_current_char,(MDLOCK),"delete char under cursor"));
	append_macro(init_macro('w',forwword,(MDLOCK | MDVISL | MDVIEW),"move to next word"));
	append_macro(init_macro('b',backword,(MDLOCK | MDVISL | MDVIEW),"move to back word"));
	append_macro(init_macro('$',gotoeol,(MDLOCK | MDVISL | MDVIEW),"move to end of line"));
	append_macro(init_macro('%',find_sibling,(MDLOCK | MDVISL | MDVIEW),"find current char sibling"));
	append_macro(init_macro('^',gotosol,(MDLOCK | MDVISL | MDVIEW),"move to start of line"));
	append_macro(init_macro(SPEC | '6',move_nextpage,(ALLMODES),"move to next page"));
	append_macro(init_macro(SPEC | '5',move_prevpage,(ALLMODES),"move to next page"));

	append_macro(init_macro((CTLX | 'N'),next_window,(ALLMODES),"go to next window"));
	append_macro(init_macro((CTLX | 'P'),prev_window,(ALLMODES),"go to prev window"));

	append_macro(init_macro('o',line_new_down,(MDLOCK),"new line down"));
	append_macro(init_macro('O',line_new_up,(MDLOCK),"new line top"));

	return macros_count;
}

int generate_basic_commands()
{
	append_command(init_command("q",quit,0));
	append_command(init_command("w",write_cmd,0));
	append_command(init_command("o",open_command,0));
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

void clear_number_stack()
{
	msgbar_prompt_p = 0;
	repeat_char(msgbar_prompt,0,PROMPT_MAX_LENGTH);
}


int refresh_lock_mode(int f,int n)
{
	clear_macro_stack();
	clear_number_stack();
}

void add_to_macro_stack(char c)
{
	char key[6];
	cmdstr(c,key);
	int i = 0;
	while (*(key + i)) {
		if (macro_stack_p >= MAX_MACRO_STACK)
			goto no_space;
		macro_stack[macro_stack_p++] = *(key + i++);
	}
	if (macro_stack_p >= MAX_MACRO_STACK - 1) {
no_space:
		showmsg(true,"Macro not found : %s",macro_stack);
		clear_macro_stack();
		return;
	}
	macro_stack[macro_stack_p++] = '-';
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
 *	when user cancelec insertin char in prompt mode (or command mode or ..)
 *	will change buffer mode to lock 
 */
int leave_prompt_mode(int f, int n)
{
	clear_number_stack();
	msgbar_cursor_col = 1;
	set_mode_for_buffer(MDLOCK);
	write_buffer();
	// set prompt keys to default
	generate_prompt_keys();
}

/*
 *	default propmt enter key event
 *	i dont think we need this anywhere by we can
 *	implement it again if needed :)
 */
int prompt_enter_key(int f,int n)
{
	if (bmtest(curbp,MDCMMD)) {
		if (msgbar_prompt_p == 0)
			return false;
		char **args = tokenize_string(msgbar_prompt,' ');
		command *cmd = find_command(args[0]);
		if (cmd == NULL) {
			showmsg(false,"(command not found)");
		} else {
			leave_prompt_mode(true,1);
			cmd->func(true,args);
		}
	}
}

/*
 *	default propmt tab event 
 *	it will write all command into pop up screen
 *	if user wroted something it will find by name
 *	then print them
 */
int prompt_tab_key(int f,int n) 
{
	TTmove(buffers_start_offset,1);
	command *cmd = fcommand;
	for (int i = 0;i < statusbar_start_offset - windowsbar_start_offset - 1;i++) {
		TTeeol();
		if (cmd != NULL) {
			TTputs(cmd->name);
			cmd = cmd->link.next;
		}
		TTputs("\r\n");
	}
}

/*
 *	when buffer is in command mode or function mode or prompt mode
 *	this function will handle user inputs , store inputs and move
 *	cursor in message bar
 */
int manage_prompt_key(int c)
{
	prompt_key *pk = get_prompt_key(c);
	if (pk != NULL) {
		pk->func(true,1);
		return true;
	}
	if (c == 127) {
		if (msgbar_prompt_p > 0)
			msgbar_prompt_p--;
		if (msgbar_cursor_col > 2)
			msgbar_cursor_col--;
	} else {
		if (msgbar_prompt_p >= PROMPT_MAX_LENGTH - 1) {
			showmsg(true,"Max length of promp");
			return false;
		}
		msgbar_prompt[msgbar_prompt_p++] = c;
		msgbar_cursor_col++;
	}
	msgbar_prompt[msgbar_prompt_p] = 0;
	//find_and_set_command_keys();
	return true;
}

void find_and_set_command_keys()
{
	if (msgbar_prompt_p == 0) {
		generate_prompt_keys();
		return;
	}
	char *temp = msgbar_prompt;
	char **args = tokenize_string(temp,' ');
	if (strcmp(args[0],"o") == 0) {
		change_prompt_key(CTRL_KEY('i'),open_command_tab);
	}
}

/*
 *	excuse me SRP :)
 *	this function will do tree task : 
 *	if key exists it will change its function
 *	if dosnt exist , it will create new one , ane
 *	append it into our prompt key list
 */
int change_prompt_key(int key,int (*func)(int,int)) 
{
	for (prompt_key *pk = fprompt_key;pk != NULL;pk = pk->link.next) {
		if (pk->key == key) {
			pk->func = func;
			return true;
		}
	}
	prompt_key *pk = (prompt_key *)malloc(sizeof(prompt_key));
	pk->func = func;
	pk->key = key;
	if (fprompt_key == NULL) {
		fprompt_key = pk;
	} else {
		lprompt_key->link.next = pk;
	}
	lprompt_key = pk;
	return true;
}

/*
 *	find in prompt keys by given key
 */
prompt_key *get_prompt_key(int key)
{
	for (prompt_key *pk = fprompt_key;pk != NULL;pk = pk->link.next) {
		if (pk->key == key)
			return pk;
	}
	return NULL;
}

/*
 * kill the program
 */
void die(const char *s) {
	ttclose();
	fprintf(stderr, "%s\n", s);
	exit(2);
}
