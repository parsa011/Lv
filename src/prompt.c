#include "types.h"
#include <time.h>
#include <stdarg.h>
 
char user_message[USER_MSG_LEN];
int user_message_len;
time_t user_message_time;

int prompt_char_offset;

public void show_message(char *message, ...)
{
	user_message_time = time(NULL);
	va_list ap;
	va_start(ap, message);
	user_message_len = prompt_char_offset = vsnprintf(user_message, USER_MSG_LEN, message, ap);
	va_end(ap);
	update_command_bar();
}

public char *prompt_string(char *message, ...)
{
	show_message(message);
	tty_move_cursor(CURSOR_POS(global_editor.term_row, 1 + ++prompt_char_offset));
	int c = get_key();
	static char user_answer[USER_MSG_LEN];
	char *ptr = user_answer;
	*ptr = 0;
	while (c != 13) {
		*ptr++ = c;
		printf("%c", c);
		tty_move_cursor(CURSOR_POS(global_editor.term_row, 1 + ++prompt_char_offset));
		c = get_key();
	}
	*ptr = 0;
	return user_answer;
}

public bool prompt_bool(char *message, ...)
{
	show_message(message);
	tty_move_cursor(CURSOR_POS(global_editor.term_row, 1 + ++prompt_char_offset));
	return get_key() == 'y';
}

public void clear_message()
{
	user_message[(user_message_len = 0)] = 0;
}
