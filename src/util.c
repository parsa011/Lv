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
