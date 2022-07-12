#include "types.h"

public char *get_filename_of_path(char *path)
{
	if (path == NULL)
		return NULL;
	char *ptr = strrchr(path, '/');
	if (!ptr)
		return path;
	return (ptr + 1);
}

public bool file_exists(char *path)
{
	return access(path, F_OK) == 0;
}

// do shift write for buf string , start from end and end in start_index 
void shift_right(char *buf, int buflen, int start_index)
{
	for (int i = buflen - 1; i > start_index; i--) {
		buf[i] = buf[i - 1];
		buf[i - 1] = ' ';
	}
}

void shift_left(char *buf, int buflen, int start_index)
{
	for (; start_index < buflen - 1; start_index++) {
		buf[start_index] = buf[start_index + 1];
		buf[start_index + 1] = ' ';
	}
}

public char *create_rgb_color(color bg, color fg)
{
	static char output[100];
	sprintf(output, "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", fg.red,
			fg.green, fg.blue, bg.red, bg.green, bg.blue);
	return output;
}

public char *create_bg_color(color bg)
{
	static char output[100];
	sprintf(output, "\033[48;2;%d;%d;%dm", bg.red, bg.green, bg.blue);
	return output;
}

public char *create_fg_color(color fg)
{
	static char output[100];
	sprintf(output, "\033[38;2;%d;%d;%dm", fg.red, fg.green, fg.blue);
	return output;
}

public void reset_color()
{
	printf("%s", COLORDEFAULT);
}

public void change_color(char *color)
{
	printf("%s", color);
}

public void set_bold()
{
	printf("\033[1m");
}

public void unset_bold()
{
	printf("\033[22m");
}

public void set_underline()
{
	printf("\033[4m");
}

public void unset_underline()
{
	printf("\033[24m");
}

public bool is_all_number(char *str)
{
	char *ptr = str;
	while (*ptr) {
		if (!isdigit(*ptr++))
			return false;
	}
	return true;
}
