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
	TTputs(DEFAULT);
	close_file();
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
	
	append_window(init_window());
	append_buffer(curwp,init_buffer(NULL,0,0));
	curbp = curwp->fbuffer;

	/* debug window initialization */
	debug_win = init_window();
	debug_win->fbuffer = init_buffer(DEBUG_BUF_NAME,MDVIEW,0);

    L_LINK_INSERT(curwp,debug_win);

	set_terminal_title(TERMINAL_TITLE);

	generate_basic_macros();
	generate_basic_commands();
	generate_prompt_keys();
#if HAVE_LOG
	create_log_file();
#endif
}

void draw_rec()
{
	int width = 20, height = 20, b = 0, a = 0;  // initalize local variables
	term.t_move(term.t_mrow - height - 5,term.t_mcol - width - 5);
	// height index (row index)
    int hi = 0;
	for (a = 0; a != height; a++) {
    	term.t_move(term.t_mrow - height - 2 + hi++,term.t_mcol - width - 2);
		// fill the width
		for (b = 0; b != width; b++ ) {
			if ((a == 0) || (a == height-1) || (b == width-1) || (b == 0)){  // put all * printing condition in one place
				//also, conditions, (a == height-1) and (b == width-1) to be used
				TTputc('*');
			}
			else   // if not to print *, print space
				TTputc(' ');
		}
		term.t_mnl();
	}
	term.t_move(cursor_row,cursor_col);
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
    	draw_rec();
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
    		/* dont insert anything when buffer is readonly :) */
    		if (bmtest(curbp,MDVIEW))
        		continue;
			if (c != (c & SPEC) || c != (c & META)) {
				manage_insert_key(c);
				save_undo_by_key(c);
			}
		} else {
			if (macro != NULL) {
    			/* first save change , before applying it :) */
				save_undo_by_macro(macro);
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
