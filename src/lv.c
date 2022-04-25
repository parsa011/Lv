/*
 *	Main file of lv editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"
#include <signal.h>

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
	append_buffer(curwp, init_buffer(NULL, 0, 0));
	curbp = curwp->fbuffer;

	/* debug window initialization */
	debug_win = init_window();
	debug_win->fbuffer = init_buffer(DEBUG_BUF_NAME, MDVIEW, 0);

    L_LINK_INSERT(curwp, debug_win);

	set_terminal_title(TERMINAL_TITLE);

	inti_tui(term.t_mfw, term.t_mbw, term.t_mnl, term.t_mpl,
			term.t_move, term.t_putc, term.t_puts, 
			&term.t_mcol, &term.t_mrow);

	generate_basic_macros();
	generate_basic_commands();
	generate_prompt_keys();

    handle_signals();
}

/*
 *	update all buffers row count
 */
void terminal_size_changed()
{
    get_screen_size(&term.t_mrow, &term.t_mcol);
    for (window *win = firstwp; win != NULL; win = L_LINK_NEXT(win)) {
        for (buffer *bf = win->fbuffer; bf != NULL; bf = L_LINK_NEXT(bf)) {
        	bf->nrow = statusbar_start_offset - buffers_start_offset - 1;
        }
    }
    redisplay_buffer();
}

/*
 *	handle signals , like window size change and ..
 */
void handle_signals()
{
    signal(SIGWINCH, terminal_size_changed);
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
		update();
		c = get_cmd();
		// if given key is number and buffer is in lock mode , we will add given number to number stack
		if (bmtest(curbp, MDLOCK) && ISNUMBER(c)) {
			add_to_number_stack(c);
			continue;
		} else if (bmtest(curbp, MDCMMD)) { // if key we are in command mode , pass key to prompt key manager
			manage_prompt_key(c);
			continue;
		}
		// otherwise , first add key to stack , they find macro by stack
		if (bmtest(curbp, MDLOCK)) {
			add_to_macro_stack(c);
			macro = find_macro_str(macro_stack);
		} else {
    		// here we will find other modes macros , for example insert mode macros and ...
			macro = find_macro(c);
		}
		if (bmtest(curbp,MDINST) && macro == NULL) {
    		/* dont insert anything when buffer is readonly :) */
    		if (bmtest(curbp, MDVIEW))
        		continue;
			if (c != (c & SPEC) || c != (c & META)) {
				manage_insert_key(c);
				//save_undo_by_key(c);
			}
		} else {
			if (macro != NULL) {
    			/* first save change , before applying it :) */
				//save_undo_by_macro(macro);
				exec_macro(macro);
				refresh_lock_mode(true, 1);
				macro = NULL;
			}
		}
	} while (1);
}

/*
 *	search in command and find commands that their key
 *  is like macro_stack
 */
void print_avaiable_commands_to_info_box()
{
    /* show them in info_box if there was any macro like that */
	for (key_macro *macro = fmacro; macro != NULL; macro = L_LINK_NEXT(macro)) {
		if (strncmp(macro->key_str, macro_stack, macro_stack_p) == 0) {
			append_text_to_info_box(macro->key_str);
		}
	}
	print_info_box();
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
		load_file_into_buffer(curbp, argv[1]);
	}
	// this is for enabling mouse
	//TTputs("\033[?1000h");
	lv_loop();
	return 0;
 }
