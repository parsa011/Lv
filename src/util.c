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

public char *create_rgb_color(color fg, color bg)
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
