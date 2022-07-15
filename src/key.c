#include "types.h"

private int put_back_key = -1;

private void putback(int c)
{
	put_back_key = c;
}

int read_char_from_terminal(void)
{
	if (put_back_key != -1) {
		char res = put_back_key;
		put_back_key = -1;
		return res;
	}
	unsigned char buf[1];
	ssize_t read_return;
	read_return = read(global_editor.tty_in, buf, 1);
	if (read_return == -1 && errno == EIO)
		exit(0);
	return buf[0];
}

int get_key()
{
	int masks = 0;
	int c;
#define GET_C() do {											\
		c = read_char_from_terminal();							\
		if (c == ESC)											\
			masks |= META;										\
		else if (iscntrl(c) && c != ('m' & 0x1f) && c != 127) { \
			if (c == 31) {										\
				c = '/';										\
			} else												\
				c = (c + 96);									\
			masks |= CONTROL;									\
		}														\
	} while (false);

	GET_C();
	if (c == ESC) {
		if (!ttycheck())
			return c;
		GET_C();
		if (c == 'O' || c == '[') {
			GET_C();
			int final_char = c;
			masks = 0;
			if (ttycheck()) {
				GET_C();
				GET_C();
				if (c == '3')
					masks |= META;
				else if (c == '5')
					masks |= CONTROL;
				if (ttycheck())
					GET_C();
				final_char = c;
				while (ttycheck())
					GET_C();
			}
			if (final_char >= 'A' && final_char <= 'D' || /* Arrow keys */
				final_char >= '1' && final_char <= '6' || /* home, end and ... */
				final_char >= 'P' && final_char <= 'S'    /* F1 ... F4 KEYS */
				)
				return SPEC | final_char | masks;
		}
		return c | masks;
	}
	return c | masks;

#undef GET_C
}

void get_key_str(int c, char *buffer)
{
	char *ptr = buffer;
	bool is_control = c & CONTROL;
	if (is_control) {
		*ptr++ = 'C';
		*ptr++ = '-';
		c &= ~CONTROL;
	}
	if (c & META) {
		*ptr++ = 'M';
		*ptr++ = '-';
		c &= ~META;
	}
	*ptr = 0;
	switch (c) {
		case ESC : 
			strcat(buffer, "ESC");
			return;
		case ARROW_UP :
			strcat(buffer, "ARROW_UP");
			return;
		case ARROW_DOWN :
			strcat(buffer, "ARROW_DOWN");
			return;
		case ARROW_RIGHT :
			strcat(buffer, "ARROW_RIGHT");
			return;
		case ARROW_LEFT :
			strcat(buffer, "ARROW_LEFT");
			return;
		case HOME_KEY :
			strcat(buffer, "HOME_KEY");
			return;
		case INS_KEY :
			strcat(buffer, "INS_KEY");
			return;
		case DEL_KEY :
			strcat(buffer, "DEL_KEY");
			return;
		case END_KEY :
			strcat(buffer ,"END_KEY");
			return;
		case PGUP_KEY :
			strcat(buffer, "PGUP_KEY");
			return;
		case PGDOWN_KEY :
			strcat(buffer, "PGDOWN_KEY");
			return;
		case F1_KEY :
			strcat(buffer, "F1");
			return;
		case F2_KEY :
			strcat(buffer, "F2");
			return;
		case F3_KEY :
			strcat(buffer, "F3");
			return;
		case F4_KEY :
			strcat(buffer, "F4");
			return;
	}
	if (is_control)
		*ptr++ = (c & 0x1f) + '@';
	else
		*ptr++ = c & 255;	/* strip the prefixes */
	*ptr = 0;		/* terminate the string */
}
