#include "types.h"

private char put_back_key = -1;

private void putback(char c)
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
