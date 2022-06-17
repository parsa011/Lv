 /*
 *	Deal with user to take key 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int get_key()
{
	int c = TTgetc();

	if (c >= 0x00 && c <= 0x1F)
		c = CONTROL | (c + '@');

	return c;
}

/*
 *	this part of is copied from uemacs :)
 *	get any key from user and convert it
 */
int get_cmd()
{
	int c; /* fetched keystroke */
	int d; /* second character P.K. */
	int cmask = 0;
	/* get initial character */
	c = get_key();
proc_metac:
	//if (c == 128 + 27) /* CSI */
	//	goto handle_CSI;
	/* process META prefix */
	if (c == (CONTROL | '[')) {
		if (!ttcheck())
			return c;
		c = get_key();
		if (c == '[' || c == 'O') { /* CSI P.K. */
//handle_CSI:
			c = get_key();
			if (c >= 'A' && c <= 'D') /* arrow keys */
				return SPEC | c | cmask;
			if (c >= 'E' && c <= 'z' && c != 'i' && c != 'c') /* home and end keys ... */
				return SPEC | c | cmask;
			d = get_key();
			if (d == '~') /* ins ,del ,page up and down*/
				return SPEC | c | cmask;
			switch (c) { /* ESC [ n n ~ P.K. (like fn keys) */
				case '1':
					c = d + 32;
					break;
				case '2':
					c = d + 48;
					break;
				case '3':
					c = d + 64;
					break;
				default:
					c = '?';
					break;
			}
			if (d != '~') 	/* control or shit special keys */
				get_key();
			if (c == 'i') { /* DO key    P.K. */
				c = ctlxc;
				goto proc_ctlxc;
			}
			else if (c == 'c') /* ESC key   P.K. */
				c = get_key();
			else
				return SPEC | c | cmask;
		}
		if (c == (SPEC | 'P')) { // f1 key
			cmask = META;
			goto proc_metac;
		}
		convert_to_control_key(&c);
		return META | c;
	}
	else if (c == metac) {
		c = get_key();
		if (c == (CONTROL | '[')) {
			cmask = META;
			goto proc_metac;
		}
		convert_to_control_key(&c);
		return META | c;
	}

proc_ctlxc:
	/* process CTLX prefix */
	if (c == ctlxc) {
		c = get_key();
		if (c == (CONTROL | '[')) {
			cmask = CTLX;
			goto proc_metac;
		}
		convert_to_control_key(&c);
		return CTLX | c;
	}

	/* otherwise, just return it */
	return c;
}

void convert_to_control_key(int *c)
{
	if (islower(*c)) /* Force to upper */
		*c ^= DIFCASE;
	if (*c >= 0x00 && *c <= 0x1F) /* control key */
		*c = CONTROL | (*c + '@');
}	
