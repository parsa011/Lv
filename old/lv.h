#ifndef _LV_H
# define _LV_H

#define HAVE_WINDOWS_BAR 0

#define EDITOR_VERSION "0.0.1"
#define EDITOR_TITLE "PMS " EDITOR_VERSION
#define COMMAND_MODE_MASK ":"

#define DEBUG_BUF_NAME "**debug**"

extern int tab_size;
extern int msgbar_msg_time;

/*
 *	main loop of program , get key and insert it into buffer
 *	or if thats a macro , so execute it
 */
void lv_loop();

/*
 * 	do some basic initialization for editor
 */
void init_editor();

void print_avaiable_commands_to_info_box();

/*
 *	close editor
 *	but before closing , check for edited buffer to save them or prevent
 *	of closing
 */
int close_editor(int,int);

void handle_signals();

void terminal_size_changed();

int empty_buffer();
int endof_buffer();
int topof_buffer();
int file_notfound();
int read_only();

#endif
