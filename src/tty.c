/*
 *	UNIX tty and Process interface
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

#include <sys/ioctl.h>
#include <termios.h>

int idleout = 1;

/* The terminal */

FILE *termin = NULL;
FILE *termout = NULL;
struct termios oldterm;

/* TTY mode flag.  1 for open, 0 for closed */
static int ttymode = 0;

/* Global configuration variables */

int noxon = 0;			/* Set if ^S/^Q processing should be disabled */
int Baud = 0;			/* Baud rate from joerc, cmd line or environment */

/* Input buffer */

int have = 0;			/* Set if we have pending input */
unsigned char havec;	/* Character read in during pending input check */
int leave = 0;			/* When set, typeahead checking is disabled */


/*
 *  open terminal
 */
void ttopen()
{
	struct termios newterm;

	if (!termin) {
		if (idleout ? (!(termin = stdin) || !(termout = stdout)) : (!(termin = fopen("/dev/tty", "r")) || !(termout = fopen("/dev/tty", "w")))) {
			fputs(_("Couldn\'t open /dev/tty\n"), stderr);
			exit(1);
		}
	}
	fflush(termout);
	
	tcgetattr(fileno(termin),&oldterm);
	newterm.c_cflag = 0;
	if (noxon)
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR | IXON | IXOFF);
	else
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR);
	newterm.c_oflag = 0;
	newterm.c_cc[VMIN] = 1;
	newterm.c_cc[VTIME] = 0;
	tcsetattr(fileno(termin), TCSADRAIN, &newterm);
}

/*
 *  close terminal 
 */
void ttclose()
{
	int oleave;

	if (ttymode)
		ttymode = 0;
	else 
		return;
	oleave = leave;
	leave = 1;

	tcsetattr(fileno(termin), TCSADRAIN, &oldterm);

	leave = oleave;
}
