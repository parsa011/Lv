#include "types.h"

/*
 * Dispatch table.
 * All the hard fields just point into the terminal I/O code.
 */
struct terminal term = {
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
void vt100beep()
{
	//ttputc(BEL);
	ttputc("\x07");
}

void vt100move(int row,int col)
{
	//ttputc(ESC);
	//ttputc("\x1b[Y");
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
	ttputc(buf);
}

void vt100eeol()
{
	ttputc("\x1b[2K");
}

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
