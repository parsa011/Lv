#ifndef _KEY_H
# define _KEY_H

/*
 * Convert a key to a control key
 */
#define CTRL_KEY(k) ((k) & 0x1f)

/*
 *	Checks if given key is a control key or not
 */
#define IS_CTRL_KEY(k) (k == CTRL_KEY(k))

enum keys {
	ESC = 27,
};

/*
 *	Read char by 1 from global_editor.tty_in input and return it
 *	also check if we have any pending char in put_back_key or no, if this variable was not
 *	-1 we will return it
 */
int read_char_from_terminal();

#endif
