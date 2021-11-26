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

/* Output buffer, index and size */

unsigned char *obuf = NULL;
int obufp = 0;
int obufsiz;

/*
 *  open terminal
 */
void ttopen(void)
{
	struct termios newterm;

	if (!termin) {
		if (idleout ? (!(termin = stdin) || !(termout = stdout)) : (!(termin = fopen("/dev/tty", "r")) || !(termout = fopen("/dev/tty", "w")))) {
			fputs(_("Couldn\'t open /dev/tty\n"), stderr);
			exit(1);
		}
	}

	if (ttymode)
		return;
	ttymode = 1;
	fflush(termout);
	
	tcgetattr(fileno(termin),&oldterm);
	newterm.c_cflag = 0;
	if (noxon)
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR | IXON | IXOFF);
	else
		newterm.c_iflag &= ~(ICRNL | IGNCR | INLCR);
	newterm.c_oflag = 0;
	newterm.c_cc[VMIN] = 0;
	newterm.c_cc[VTIME] = 1;
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

int ttgetc(void)
{
	return 0;
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
		lv_write(fileno(termout), obuf, obufp);
		// set zero to start again of first
		obufp = 0;
	}

	// check for typeahead or next packet 
	// ttcheck();
	return 0;
}

/*
 * kill the program
 */
void die(const char *s) {
	ttclose();
	fprintf(stderr, "%s\n", s);
	exit(2);
}
