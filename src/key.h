#ifndef _KEY_H
# define _KEY_H

/*
 * Convert a key to a control key
 */
#define CTRL_KEY(k) ((k) | CONTROL)

/*
 *	Checks if given key is a control key or not
 */
#define IS_CTRL_KEY(k) (k == CTRL_KEY(k))

#define CONTROL 0x10000
#define META 	0x20000
#define	SPEC	0x80000	/* special key (function keys)  */

enum keys {
	ESC = 27,
	ARROW_UP = SPEC | 'A',
	ARROW_RIGHT = SPEC | 'C',
	ARROW_DOWN = SPEC | 'B',
	ARROW_LEFT = SPEC | 'D',
	HOME_KEY = SPEC | '1',
	INS_KEY = SPEC | '2',
	DEL_KEY = SPEC | '3',
	END_KEY = SPEC | '4',
	PGUP_KEY = SPEC | '5',
	PGDOWN_KEY = SPEC | '6',
	F1_KEY = SPEC | 'P',
	F2_KEY = SPEC | 'Q',
	F3_KEY = SPEC | 'R',
	F4_KEY = SPEC | 'S'
};

/*
 *	Read char by 1 from global_editor.tty_in input and return it
 *	also check if we have any pending char in put_back_key or no, if this variable was not
 *	-1 we will return it
 */
int read_char_from_terminal();

/*
 *	read char from terminal and convert it to a key
 */
int get_key();

/*
 *	return human readable string for given key
 */
void get_key_str(int key, char *buffer);
#define print_key(key) do { \
		char key_str[16]; \
		get_key_str(key, key_str); \
		printf("%s\n", key_str); \
	} while (false);

#endif
