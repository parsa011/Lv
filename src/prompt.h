#ifndef _PROMPT_H
# define _PROMPT_H

#define USER_MSG_LEN 256

public char user_message[USER_MSG_LEN];
public int user_message_len;
public time_t user_message_time;

/*
 *	every time that we want to prompt to user and get a value,
 *	we hold cursor col in command bar with this  
 */
public int prompt_char_offset;

/*
 *	Just show a message to user in command_bar section 
 */
public void show_message(char *message, ...);

/*
 *	hide and clear message from command-bar and show buffer informations again
 */
public void clear_message();

/*
 *	get a string of user with given mask , if 'answer_prefix' is not null, it gonna 
 *	to be appended into user answer, for example we can use it when we want to get a 
 *	path from user
 */
public char *prompt_string(char *answer_prefix, char *message, ...);

/*
 *	Get Boolean value from user, for example it can be used for y/n questions
 */
public bool prompt_bool(char *message, ...);

/*
 *	return number value from prompt
 *	it returs bool because maybe user enterd invalid number,
 *	so we cant return -1 as return value, we return false, and we will
 *	store that number in given 'res' variable
 */
public bool prompt_number(int *res, char *message, ...);

#endif
