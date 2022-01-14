#ifndef _PROMPT_H
# define _PROMPT_H

/*
 *	Get prompt of user
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

// prompt from message bar statuses
#define PROMPT_MAX_LENGTH 512 
extern int msgbar_cursor_col;
extern char msgbar_prompt[PROMPT_MAX_LENGTH];
extern char msgbar_prompt_mask[PROMPT_MAX_LENGTH];
extern int msgbar_prompt_mask_len;
extern int msgbar_prompt_p;

extern prompt_key *fprompt_key;
extern prompt_key *lprompt_key;

struct prompt_key_t {
	L_LINK(prompt_key) link;
	int key;
	int (*func)(int,int); /* function to run */
};

/*
 *	set mask for prompt
 */
void set_prompt_mask(char *);

/*
 *	manager user inputs when we are getting prompt
 */
int manage_prompt_key(int);

/*
 *	will change prompt mode special key event
 *	but it will create new event if given key
 *	dosnt exist
 */
int change_prompt_key(int,int (*)(int,int));

/*
 *	find prompt key
 */
prompt_key *get_prompt_key(int);

/*
 *	clear prompt and exit of prompt mode
 */
int leave_prompt_mode(int,int);

/*
 *	default enter key event for prompt
 */
int prompt_enter_key(int,int);

/*
 *	default tab key event for prompt
 */
int prompt_tab_key(int,int);

/*
 *	search in command to set it keys event
 */
void find_and_set_command_keys();

void clear_prompt();

#endif
