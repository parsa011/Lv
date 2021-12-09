/*
 *	control vt100 screen 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 * Dispatch table.
 * All the hard fields just point into the terminal I/O code.
 */
terminal term = {
	NROW - 1,
	NROW - 1,
	NCOL,
	NCOL,
	MARGIN,
	SCRSIZ,
	NPAUSE,
	&ttopen,
	&ttclose,
	&ttgetc,
	&ttputc,
	&ttflsh,
	&vt100move,
	&vt100eeol,
	&vt100eeop,
	&vt100beep,
	&vt100mnl,
	&vt100mpl,
	&vt100mfw,
	&vt100mbw,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

/*
 *	vt100 screens interfaces
 */

/*
 * beep :)
 */
void vt100beep()
{
	ttputc("\x07");
}

void vt100move(int row,int col)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
	ttputc(buf);
}

/*
 *	erase end of line
 */
void vt100eeol()
{
	ttputc("\x1b[2K");
}

/*
 *	erase end of page
 */
void vt100eeop()
{
	ttputc("\x1b[2J");
}

/*
 *	move cursor to next line
 *	(cursor down)
 */
void vt100mnl()
{
	ttputc("\x1b[B");
}

/*
 *	move cursor to prev line
 *	(cursor up)
 */
void vt100mpl()
{
	ttputc("\x1b[A");
}

/*
 *	move cursor to next char 
 *	(cursor right)
 */
void vt100mfw()
{
	ttputc("\x1b[C");
}

/*
 *	move cursor to prev char 
 *	(cursor left)
 */
void vt100mbw()
{
	ttputc("\x1b[D");
}
