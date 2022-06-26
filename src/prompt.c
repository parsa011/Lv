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

public char *prompt_string(char *answer_prefix, char *message, ...)
{
	show_message(message);
	tty_move_cursor(CURSOR_POS(global_editor.term_row, 1 + ++prompt_char_offset));
	static char user_answer[USER_MSG_LEN];
	char *ptr = user_answer;
	*ptr = 0;
	if (answer_prefix) {
		int prefix_len = strlen(answer_prefix);
		memcpy(user_answer, answer_prefix, prefix_len);
		ptr += prefix_len;
		prompt_char_offset += prefix_len;
		printf("%s", user_answer);
	}
	int c = get_key();
	while (c != 13) {
		if (c == BACKSPACE_KEY || c == CTRL_KEY('g')) {
			if (ptr == user_answer)
				continue;
			*(--ptr) = 0;
			prompt_char_offset -= 2;
		} else if (c == ESC) {
			*user_answer = 0;
			return NULL;
		} else {
			*ptr++ = c;
			printf("%c", c);
		}
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
	printf("(y/n) ");
	return get_key() == 'y';
}

public bool prompt_number(int *res, char *message, ...)
{
	char *str = prompt_string(NULL, message);
	if (!is_all_number(str)) {
		show_message("Enter Valid Number");
		return false;
	}
	*res = atoi(str);
	return true;
}

public void clear_message()
{
	user_message[(user_message_len = 0)] = 0;
}
