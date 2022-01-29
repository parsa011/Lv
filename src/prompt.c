/*
 *	Get prompt of user
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

// prompt from message bar statuses
int msgbar_cursor_col = 1;
char msgbar_prompt[PROMPT_MAX_LENGTH];
char msgbar_prompt_mask[PROMPT_MAX_LENGTH];
int msgbar_prompt_mask_len = 0;
int msgbar_prompt_p = 0;

prompt_key *fprompt_key;
prompt_key *lprompt_key;

/*
 * this is key press event for prompt mode , this will happen each time that
 * we press any key in prompt mode , it's usefull in find mode and ...
 * becareful to make it null after each press enter key (exiting of prompt mode)
 */
static int (*prompt_key_press_event)(int, int) = NULL;

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
		if (msgbar_cursor_col > msgbar_prompt_mask_len)
			msgbar_cursor_col--;
	} else {
		if (msgbar_prompt_p >= PROMPT_MAX_LENGTH - 1) {
			showmsg(true,"Max length of promp");
			return false;
		}
		if (c == ' ' && msgbar_prompt_p == 0)
			return false;
		msgbar_prompt[msgbar_prompt_p++] = c;
		msgbar_cursor_col++;
	}
	msgbar_prompt[msgbar_prompt_p] = 0;
	find_and_set_command_keys();
	if (prompt_key_press_event)
    	(*prompt_key_press_event)(1,true);
	return true;
}

/*
 *	set mask for prompt , also set prompt default col
 */
void set_prompt_mask(char *str)
{
	msgbar_prompt_mask_len = strlen(str) + 1;
	memcpy(msgbar_prompt_mask,str,msgbar_prompt_mask_len);
	msgbar_cursor_col = msgbar_prompt_mask_len;
}

/*
 *	when user cancelec insertin char in prompt mode (or command mode or ..)
 *	will change buffer mode to lock
 */
int leave_prompt_mode(int f, int n)
{
	clear_prompt();
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
		char **args = tokenize_string(msgbar_prompt," ");
		command *cmd = find_command(args[0]);
		if (cmd == NULL) {
			showmsg(false,"(command not found)");
		} else {
			leave_prompt_mode(true,1);
			cmd->func(true,args);
		}
		prompt_key_press_event = NULL;
		free(args);
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
 *	it will set command key
 *  TODO : enhance this code ;/ really dirty (;/ * 2)
 */
void find_and_set_command_keys()
{
	if (msgbar_prompt_p == 0) {
		generate_prompt_keys();
		return;
	}
	char *temp = strdup(msgbar_prompt);
	char **args = tokenize_string(temp," ");
	if (strcmp(args[0],"o") == 0) {
		change_prompt_key(CTRL_KEY('i'),open_command_tab);
	} else if (strcmp(args[0],"w") == 0) {
		change_prompt_key(CTRL_KEY('i'),write_command_tab);
	} else if (strcmp(args[0],"q") == 0) {
		change_prompt_key(CTRL_KEY('i'),quite_command_tab);
	} else if (strcmp(args[0],"buffer") == 0) {
		change_prompt_key(CTRL_KEY('i'),buffer_command_tab);
	} else if (strcmp(args[0],"set") == 0)
    	change_prompt_key(CTRL_KEY('i'),set_command_tab);
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
	prompt_key *pk = (prompt_key *)lv_malloc(sizeof(prompt_key));
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

void clear_prompt()
{
	msgbar_prompt_p = 0;
	repeat_char(msgbar_prompt,0,PROMPT_MAX_LENGTH);
}
