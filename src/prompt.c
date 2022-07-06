#include "types.h"
#include <time.h>
#include <stdarg.h>
 
char user_message[USER_MSG_LEN];
int user_message_len;
time_t user_message_time;

int prompt_char_offset;

public void show_message(bool reset_pos, char *message, ...)
{
	user_message_time = time(NULL);
	va_list ap;
	va_start(ap, message);
	user_message_len = vsnprintf(user_message, USER_MSG_LEN, message, ap);
	va_end(ap);

	/* write message */
	cursor_position cur_pos;
	pos_copy(current_window->cursor_pos, cur_pos);
	tty_move_cursor(CURSOR_POS(global_editor.term_row, 1));
	tty_erase_end_of_line();
	printf("%s", user_message);
	if (reset_pos)
		tty_move_cursor(cur_pos);
}

public char *prompt_string(char *answer_prefix, char *message, ...)
{
	show_message(false, message);
	static char user_answer[USER_MSG_LEN];
	char *ptr = user_answer;
	*ptr = 0;
	if (answer_prefix) {
		int prefix_len = strlen(answer_prefix);
		memcpy(user_answer, answer_prefix, prefix_len);
		ptr += prefix_len;
		printf("%s", user_answer);
	}
	int c = get_key();
	while (c != 13) {
		if (c == CTRL_KEY('g') || c == ESC) {
			show_message(true, "(canceled)");
			*user_answer = 0;
			return NULL;
		}
		if (c == BACKSPACE_KEY || c == CTRL_KEY('g')) {
			if (ptr == user_answer)
				continue;
			*(--ptr) = 0;
		} else {
			*ptr++ = c;
		}
		show_message(false, "%s%s",message, user_answer);
		c = get_key();
	}
	*ptr = 0;
	return user_answer;
}

public bool prompt_bool(char *message, ...)
{
	show_message(false, message);
	printf(" (y/n) ");
	int c;
again :
	c = get_key();
	if (c != 'y' && c != 'n')
		goto again;
	show_message(true, "");
	return c == 'y';
}

public bool prompt_number(int *res, char *message, ...)
{
	char *str = prompt_string(NULL, message);
	if (!is_all_number(str)) {
		show_message(true, "Enter Valid Number");
		return false;
	}
	*res = atoi(str);
	return true;
}

public void clear_message()
{
	user_message[(user_message_len = 0)] = 0;
}
