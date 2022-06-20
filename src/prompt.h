#ifndef _PROMPT_H
# define _PROMPT_H

#define USER_MSG_LEN 256

public char user_message[USER_MSG_LEN];
public int user_message_len;
public time_t user_message_time;

/*
 *	Just show a message to user in command_bar section 
 */
public void show_message(char *message, ...);

#endif
