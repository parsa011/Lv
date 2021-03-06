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
 *	just shit right ;// 
 */
public void shift_right(char *buf, int buflen, int start_index);
public void shift_left(char *buf, int buflen, int start_index);

/*
 *	Create and return pointer to rgb color sequence for terminal
 *	fg and bg should be like :
 *		"(r);(g);(b)" 8 bit code of colors
 *
 *	and it will return something like :
 *		ESC[38;2;fg ESC[45;2;bg
 */
public char *create_rgb_color(color fg, color bg);

/*
 *	create just background color string
 */
public char *create_bg_color(color bg);

/*
 *	like create_bg_color() but for foreground
 */
public char *create_fg_color(color fg);

public void reset_color();
public void change_color(char *color);
public void set_bold();
public void unset_bold();
public void set_underline();
public void unset_underline();

/*
 *	check if given string characters are all number characters
 */
public bool is_all_number(char *str);

#endif
