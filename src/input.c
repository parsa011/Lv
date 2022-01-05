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
	if (c == 128 + 27) /* CSI */
		goto handle_CSI;
	/* process META prefix */
	if (c == '[') {
		//c = get_key();
		if (c == '[' || c == 'O') { /* CSI P.K. */
handle_CSI:
			c = get_key();
			if (c >= 'A' && c <= 'D')
				return SPEC | c | cmask;
			if (c >= 'E' && c <= 'z' && c != 'i' && c != 'c')
				return SPEC | c | cmask;
			d = get_key();
			if (d == '~') /* ESC [ n ~   P.K. */
				return SPEC | c | cmask;
			switch (c) { /* ESC [ n n ~ P.K. */
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
			if (d != '~') /* eat tilde P.K. */
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
		if (islower(c)) /* Force to upper */
			c ^= DIFCASE;
		if (c >= 0x00 && c <= 0x1F) /* control key */
			c = CONTROL | (c + '@');
		return META | c;
	}
	else if (c == metac) {
		c = get_key();
		if (c == metac) {
			cmask = META;
			goto proc_metac;
		}
		if (islower(c)) /* Force to upper */
			c ^= DIFCASE;
		if (c >= 0x00 && c <= 0x1F) /* control key */
			c = CONTROL | (c + '@');
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
		if (c >= 'a' && c <= 'z') /* Force to upper */
			c -= 0x20;
		if (c >= 0x00 && c <= 0x1F) /* control key */
			c = CONTROL | (c + '@');
		return CTLX | c;
	}

	/* otherwise, just return it */
	return c;
}
