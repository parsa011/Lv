#include "lv.h"
#include <signal.h>

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

void at_exit()
{
	terminal_cooked_mode();
	tty_clear_screen();
}

void init_first_window()
{
	current_window = window_alloc();
	window_init(current_window);
	tty_move_cursor(current_window->cursor_pos);
	current_buffer = current_window->first_buffer;
}

/*
 *	update all buffers row count
 */
void terminal_size_changed()
{
	update_screen_size(&global_editor.term_row, &global_editor.term_col);
	buffer_text_update();
	update_screen();
}

/*
 *	handle signals , like window size change and ..
 */
void handle_signals()
{
	signal(SIGWINCH, terminal_size_changed);
}

void init_editor()
{
	getcwd(cwd, PATH_MAX);
	tty_clear_screen();
	terminal_raw_mode();
	global_editor.tty_in = STDIN_FILENO;

	init_first_window();

	setbuf(stdout, NULL);
	update_screen_size();
	atexit(at_exit);

	handle_signals();
}

void lv_loop()
{
	int c;
	do {
		update_screen();
		c = get_key();
		if (c == CTRL_KEY('n') || c == ARROW_DOWN) {
			next_line();
		} else if (c == CTRL_KEY('e')) {
			go_line_end();
		} else if (c == CTRL_KEY('v')) {
			page_down();
		} else if (c == META_KEY('v')) {
			page_up();
		} else if (c == CTRL_KEY('a')) {
			go_line_beginning();
		} else if (c == CTRL_KEY('p') || c == ARROW_DOWN) {
			prev_line();
		} else if (c == CTRL_KEY('f') || c == ARROW_RIGHT) {
			next_char();
		} else if (c == CTRL_KEY('b') || c == ARROW_LEFT) {
			prev_char();
		} else if (c == META_KEY('f') || c == CTRL_KEY(ARROW_RIGHT)) {
			next_word();
		} else if (c == META_KEY('b') || c == CTRL_KEY(ARROW_LEFT)) {
			prev_word();
		} else if (c == CTRL_KEY('/')) {
			do_undo();
		} else if (c == CTRL_KEY('x')) {
			c = get_key();
			if (c == ARROW_LEFT)
				window_prev();
			else if (c == ARROW_RIGHT)
				window_next();
			else if (c == CTRL_KEY('s')) {
				if (current_buffer->is_modified) {
					if (buffer_save(current_buffer))
						show_message(true, "File Saved : %s", current_buffer->file_path);
				} else {
					show_message(true, "(No changes need to be saved)");
				}
			} else if(c == CTRL_KEY('c')) {
				if (current_buffer->is_modified) {
					bool res = prompt_bool("Buffer Modified, Really Wanna Exit ?");
					if (res)
						exit(0);
					else
						clear_message();
				} else
					exit(0);
			} else if (c == CTRL_KEY('f')) {
				char *file = prompt_string(cwd, "Find File :");
				if (file == NULL)
					continue;
				window_open_new();
				buffer_init(current_buffer, file);
				buffer_open_file(current_buffer, file);
				tty_move_cursor(current_window->cursor_pos);
			} else if (c == 'k') {
				buffer_kill(current_buffer);
			}
		} else if (c == CTRL_KEY('d')) {
			if (next_char())
				line_delete_char();
		} else if (c == META_KEY('d')) {
			line_delete_word();
		} else if (c == TAB_KEY) {
			line_insert_char('\t', current_buffer->char_offset);
		} else if (c == CTRL_KEY('k')) {
			/* why storing line_count ? because we want to check if we deleted any line or no ?
			 * of any line deleted, cursor will going back to prev line, but we want to stay
			 * at current line like EMACS, so after remove we will check if line_count changed,
			 * if changed to we have to move cursor to next_line (prev position)
			 */
			int line_count = current_buffer->line_count;
			line_delete_after(current_buffer->char_offset);
			if (current_buffer->line_count != line_count)
				next_line();
		} else if (c == META_KEY('g')) {
			c = get_key();
			if (c == META_KEY('g')) {
				int line_nu;
				if (prompt_number(&line_nu, "GoTo Line : " )) {
					go_to_line(line_nu);
				}
			}
		} else if (c == CTRL_KEY('s')) {
			
		} else if (c == ESC) {
			c = get_key();
			if (c == '>')
				go_to_line(current_buffer->line_count - 1);
			else if (c == '<')
				go_to_line(0);
		} else if (!IS_CTRL_KEY(c)) {
			if (c == 13)
				line_insert_new();
			else if (c == BACKSPACE_KEY)
				line_delete_char();
			else {
				undo_insert_char(CHANGE_INSERT_CHAR, buffer_line_index(), current_buffer->char_offset, c);
				line_insert_char(c, current_buffer->char_offset);
			}
		 }
	} while (true);
}

int main(int argc, char *argv[])
{
	/* handy configs for now */
	global_editor.show_tabs = true;
	global_editor.tab_size = 8;

	init_editor();
	buffer_open_file(current_buffer, argv[1]);
	lv_loop();
	exit(0);
	return 0;
}
