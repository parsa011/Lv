#ifndef _GLOB_H
# define _GLOB_H

#define TERMINAL_TITLE "LV - Parsa mahmoudy" /* default title for terminal */
#define NO_NAME_BUFFER "[NO NAME]" 	/* default name for buffers that don't have name */
#define LINE_MASK	   "~" /* we line is empty , we will write this */

#define MESSAGE_MAX_LENGTH 120

extern window *firstwp;		/* first window		*/
extern window *curwp;		/* current window   */
extern buffer *curbp;		/* current buffer   */
extern msg_bag msgbag;

// prompt from message bar statuses
#define PROMPT_MAX_LENGTH 1024
extern int msgbar_cursor_col;
extern char msgbar_prompt[PROMPT_MAX_LENGTH];
extern int msgbar_prompt_p;

#define current_line 	curbp->cline
#define cursor_row 		curwp->crow
#define cursor_col 		curwp->ccol

extern int metac;		/* current meta character */
extern int ctlxc;		/* current control X prefix char */

/*
 *	global structs like message bar struct and ...
 */
struct msg_bag_t {
	char message[MESSAGE_MAX_LENGTH];
	time_t msg_time;
	int timer;
};


/*
 *	this function will initilize and append all of basic macros
 */
int generate_basic_macros();

/*
 *	take a key and check it
 *	for example if it was new line , so we will add new line
 */
int manage_insert_key(int);

/*
 *	manager user inputs when we are getting prompt
 */
int manage_prompt_key(int);

/*
 * kill the program
 *
 * set the old term by calling ttclose()
 *
 * write error message into screen
 *
 * exit();
 */
void die(const char *);

#endif
