/*
 *	Main file of lv editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int tab_size = 4;
int msgbar_msg_time = 4;

/*
 *	close editor command , if force is 1 , so we will close editor without
 *	prompting dirty buffers . and code is for exit() function arg
 */
int close_editor(int focre, int code)
{
	if (!system("clear"))
		TTeeop();
	TTmove(1,1);
	close_file();
	TTputs(DEFAULT);
#if HAVE_LOG
	remove_log_file();
#endif
	TTputs(DEFAULT);
	TTflush();
	exit(code);
	return true;
}

/*
 *	some basic initialization for editor
 *	like initialize first window and buffer
 *	open tty and clear screen then set cursor to 0,0
 *	and set terminal title :D
 */
void init_editor()
{
	init_term();
	init_display();
	curwp = init_window();
	firstwp = curwp;
	lastwp = curwp;
	curbp = init_buffer("",NO_NAME_BUFFER,0,0);
	append_buffer(curwp,curbp);
	set_terminal_title(TERMINAL_TITLE);
	generate_basic_macros();
	generate_basic_commands();
	generate_prompt_keys();
#if HAVE_LOG
	create_log_file();
#endif
}

/*
 *	main loop of editor
 *	get key and run it if was command , or insert it into buffer
 */
void lv_loop()
{
	int c;
	key_macro *macro = NULL;
	do {
#if HAVE_LOG
#endif
		update();
		c = get_cmd();
		if (bmtest(curbp,MDLOCK) && ISNUMBER(c)) {
			add_to_number_stack(c);
			continue;
		} else if (bmtest(curbp,MDCMMD)) {
			manage_prompt_key(c);
			continue;
		}
		if (bmtest(curbp,MDLOCK)) {
			add_to_macro_stack(c);
			macro = find_macro_str(macro_stack);
		} else {
			macro = find_macro(c);
		}
		if (bmtest(curbp,MDINST) && macro == NULL) {
			if (c != (c & SPEC) || c != (c & META)) {
				manage_insert_key(c);
			}
		} else {
			if (macro != NULL) {
				exec_macro(macro);
				refresh_lock_mode(true,1);
				macro = NULL;
			}
		}
	} while (1);
}

int topof_buffer()
{
	showmsg(true,"top of buffer");
	return TOPOFBUFFER;
}

int endof_buffer()
{
	showmsg(true,"end of buffer");
	return ENDOFBUFFER;
}

int empty_buffer()
{
	showmsg(true,"buffer is empty");
	return EMPTYBUFFER;
}

int file_notfound()
{
	showmsg(true,"file not found !!");
	return FNOTF;
}

int read_only()
{
	showmsg(true,"buffer is read only");
	return false;
}

int main(int argc,char *argv[])
{
	// initialize the terminal , and activate raw mode
	init_editor();

	if (argc > 1) {
		load_file_into_buffer(curbp,argv[1]);
	}
	lv_loop();
	return 0;
}
