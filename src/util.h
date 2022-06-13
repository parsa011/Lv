#ifndef _UTIL_H
# define _UTIL_H

/*
 *	return base name of given path
 *	return path if given path contains just basename
 */
public char *get_filename_of_path(char *path);

/*
 *	return true if file exists
 */
public bool file_exists(char *path);

#endif
