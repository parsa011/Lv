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
	static char buffer[32];
	static int pending;
	unicode_t c;
	int count, bytes = 1, expected;

	count = pending;
	if (!count) {
		count = read(0, buffer, sizeof(buffer));
		if (count <= 0)
			return 0;
		pending = count;
	}

	c = (unsigned char) buffer[0];
	if (c >= 32 && c < 128)
		goto done;

	/*
	 * lazy. We don't bother calculating the exact
	 * expected length. We want at least two characters
	 * for the special character case (ESC+[) and for
	 * the normal short UTF8 sequence that starts with
	 * the 110xxxxx pattern.
	 *
	 * but if we have any of the other patterns, just
	 * try to get more characters. At worst, that will
	 * just result in a barely perceptible 0.1 second
	 * delay for some *very* unusual utf8 character
	 * input.
	 */
	expected = 2;
	if ((c & 0xe0) == 0xe0)
		expected = 6;

	/* special character - try to fill buffer */
	if (count < expected) {
		int n;
		newterm.c_cc[VMIN] = 0;
		newterm.c_cc[VTIME] = 1;		/* A .1 second lag */
		tcsetattr(0, TCSANOW, &newterm);

		n = read(0, buffer + count, sizeof(buffer) - count);

		/* undo timeout */
		newterm.c_cc[VMIN] = 1;
		newterm.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &newterm);

		if (n > 0)
			pending += n;
	}
	if (pending > 1) {
		unsigned char second = buffer[1];

		/* turn ESC+'[' into CSI */
		if (c == 27 && second == '[') {
			bytes = 2;
			c = 128 + 27;
			goto done;
		}
	}
	bytes = utf8_to_unicode(buffer, 0, pending, &c);

	/* hackety hack! Turn no-break space into regular space */
	if (c == 0xa0)
		c = ' ';
done:
	pending -= bytes;
	memmove(buffer, buffer+bytes, pending);
	return c;

}

//int ttcheck(void)
//{
//	/* Check for typeahead or next packet */
//	if (!have && !leave) {
//		if (ackkbd != -1) {
//			fcntl(mpxfd, F_SETFL, O_NDELAY);
//			if (read(mpxfd, &pack, sizeof(struct packet) - 1024) > 0) {
//				fcntl(mpxfd, F_SETFL, 0);
//				lv_read(mpxfd, pack.data, pack.size);
//				have = 1;
//			} else
//				fcntl(mpxfd, F_SETFL, 0);
//		} else {
//			/* Set terminal input to non-blocking */
//			fcntl(fileno(termin), F_SETFL, O_NDELAY);
//
//			/* Try to read */
//			if (read(fileno(termin), &havec, 1) == 1)
//				have = 1;
//
//			/* Set terminal back to blocking */
//			fcntl(fileno(termin), F_SETFL, 0);
//		}
//	}
//	return have;
//}

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
