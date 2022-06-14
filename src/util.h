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

/*
 *	Create and return pointer to rgb color sequence for terminal
 *	fg and bg should be like :
 *		"(r);(g);(b)" 8 bit code of colors
 *
 *	and it will return something like :
 *		ESC[38;2;fg ESC[45;2;bg
 */
public char *create_rgb_color(color fg, color *bg);

#endif
