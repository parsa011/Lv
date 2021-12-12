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
	NCOL,
	MARGIN,
	SCRSIZ,
	NPAUSE,
	&ttopen,
	&ttclose,
	&ttgetc,
	&ttputs,
	&ttflsh,
	&vt100move,
	&vt100eeol,
	&vt100eeop,
	&vt100beep,
	&vt100mnl,
	&vt100mpl,
	&vt100mfw,
	&vt100mbw,
	&vt100cursorhide,
	&vt100cursorshow,
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
	ttputs("\x07");
}

void vt100move(int row,int col)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
	ttputs(buf);
}

/*
 *	erase end of line
 */
void vt100eeol()
{
	ttputs("\x1b[2K");
}

/*
 *	erase end of page
 */
void vt100eeop()
{
	ttputs("\x1b[2J");
}

/*
 *	move cursor to next line
 *	(cursor down)
 */
void vt100mnl()
{
	ttputs("\x1b[B");
}

/*
 *	move cursor to prev line
 *	(cursor up)
 */
void vt100mpl()
{
	ttputs("\x1b[A");
}

/*
 *	move cursor to next char 
 *	(cursor right)
 */
void vt100mfw()
{
	ttputs("\x1b[C");
}

/*
 *	move cursor to prev char 
 *	(cursor left)
 */
void vt100mbw()
{
	ttputs("\x1b[D");
}

void vt100cursorhide()
{
	ttputs("\x1b[?25l");
}

void vt100cursorshow()
{
	ttputs("\x1b[?25h");
}
