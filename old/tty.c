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

/* the terminal */

FILE *termin = NULL;
FILE *termout = NULL;

struct termios oldterm;		/* original terminal characteristics */
struct termios newterm;		/* characteristics to use inside */

/* TTY mode flag.  1 for open, 0 for closed */

static int ttymode = 0;

/* global configuration variables */

int noxon = 1;			/* Set if ^S/^Q processing should be disabled */

/* input buffer */

int have = 0;			/* Set if we have pending input */
unsigned char havec;	/* Character read in during pending input check */
int leave = 0;			/* When set, typeahead checking is disabled */

/* output buffer, index and size */

unsigned char obuf[OBUFSIZE];
int obufp = 0;

/*
 *  open terminal
 */
void ttopen(void)
{
	if (!termin) {
		if (idleout ? (!(termin = stdin) || !(termout = stdout)) : (!(termin = fopen("/dev/tty", "r")) || !(termout = fopen("/dev/tty", "w")))) {
			fputs("Couldn\'t open /dev/tty\n", stderr);
			exit(1);
		}
	}

	if (ttymode)
		return;
	ttymode = 1;
	fflush(termout);
	
	tcgetattr(fileno(termin),&oldterm);

	/*
	 * base new settings on old ones - don't change things
	 * we don't know about
	 */
	newterm = oldterm;

	if (noxon)
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR | IXON | IXOFF);
	else
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR);

	/* raw CR/NR etc output handling */
	newterm.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET);

	newterm.c_cflag |= (CS8);

	/* No signal handling, no echo etc */
	newterm.c_lflag &= ~(ISIG | ICANON | XCASE | ECHO | ECHOE | ECHOK
			| ECHONL | NOFLSH | TOSTOP | ECHOCTL |
			ECHOPRT | ECHOKE | FLUSHO | PENDIN | IEXTEN);

	newterm.c_cc[VMIN] = 1;
	newterm.c_cc[VTIME] = 0;
	// when client exited by any accident , set orig term :)
	atexit(ttclose);
	tcsetattr(fileno(termin), TCSANOW, &newterm);
}

/*
 *  close terminal 
 */
void ttclose(void)
{
	int oleave;

	if (ttymode)
		ttymode = 0;
	else 
		return;
	oleave = leave;
	leave = 1;

	tcsetattr(fileno(termin), TCSANOW, &oldterm);

	leave = oleave;
}

/*
 *	get key from user
 */
int ttgetc(void)
{
	char	c;
	ssize_t	ret;

	do {
		ret = read(STDIN_FILENO, &c, 1);
		if (ret == -1 && errno == EINTR) {
			//if (winch_flag) {
			//	redraw(0, 0);
			//	winch_flag = 0;
			//}
		} else if (ret == -1 && errno == EIO)
			die("lost stdin");
		else if (ret == 1)
			break;
	} while (1);
	return ((int) c) & 0xFF;
}

/*
 *	will check if there is any char for read
 */
int ttcheck(void)
{
	int	x;
	return ((ioctl(0, FIONREAD, &x) == -1) ? 0 : x);
}

/* 
 * flush output and check for type ahead 
 */
int ttflsh(void)
{
	// if output buffer pointer was biggger than zero , it will came into this if statement and write our data to output
	if (obufp) {
		if (lv_write(fileno(termout), obuf, obufp) == 0)
			die("something went wront when writing into termout");
		// set zero to start again of first
		obufp = 0;
	}
	// check for typeahead or next packet 
	// ttcheck();
	return TRUE;
}

/* 
 * write string to output buffer
 */
void ttputs(char *s)
{
	while (*s) {
		ttputc(*s++);
	}
}

void ttputc(char c)
{
	if (obufp == OBUFSIZE)
		ttflsh();
	obuf[obufp++] = c;
}
