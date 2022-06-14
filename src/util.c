#include "types.h"

public char *get_filename_of_path(char *path)
{
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
	sprintf(output, "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", fg.red.
			fg.green, fg.blue, bg.red, bg.green, bg.blue);
	return output;
}
