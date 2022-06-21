#include "lv.h"

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

void at_exit()
{
	terminal_cooked_mode();
	tty_clear_screen();
}

void init_editor()
{
	getcwd(cwd, PATH_MAX);
	tty_clear_screen();
	terminal_raw_mode();
	global_editor.tty_in = STDIN_FILENO;

	reset_pos(current_window.cursor_pos);
	tty_move_cursor(current_window.cursor_pos);

	setbuf(stdout, NULL);
	update_screen_size();
	current_buffer = &current_window.first_buffer;
	atexit(at_exit);
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
		} else if (!IS_CTRL_KEY(c) && c != ESC) {
			if (c == 13)
				line_insert_new();
			else if (c == 127)
				line_delete_char();
			else
				line_insert_char(c, current_buffer->char_offset);
			
		} else if (c == CTRL_KEY('x')) {
			c = get_key();
			if (c == CTRL_KEY('s')) {
				if (current_buffer->is_modified) {
					if (buffer_save(current_buffer))
						show_message("File Saved : %s", current_buffer->file_path);
				} else {
					show_message("(No changes need to be saved)");
				}
			} else if(c == CTRL_KEY('c')) {
				if (current_buffer->is_modified) {
					bool res = prompt_bool("Buffer Modified, Really Wann Exit ?");
					if (res)
						exit(0);
					else
						clear_message();
				} else

					exit(0);
			}
		} else if (c == CTRL_KEY('d')) {
			if (next_char())
				line_delete_char();
		} else if (c == TAB_KEY) {
			line_insert_char('\t', current_buffer->char_offset);
		}

	} while (true);
}

int main(int argc, char *argv[])
{
	/* handy configs for now */
	global_editor.show_tabs = false;
	global_editor.tab_size = 4;

	init_editor();
	buffer_open_file(current_buffer, argv[1]);
	lv_loop();
	exit(0);
	return 0;
}
