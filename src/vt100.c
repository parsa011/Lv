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
	ttputc("\x1b[Y");
	char buf[32];
	sprintf(buf,"%d%d",row + BIAS , col + BIAS);
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
