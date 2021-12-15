#ifndef _KEYS_H
# define _KEYS_H

/*
 *	Definitions of keys	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define	DIFCASE	0x20

#define CONTROL 0x10000
#define META 	0x20000
#define CTLX    0x40000	/* ^X flag, or'ed in            */
#define	SPEC	0x80000	/* special key (function keys)  */

// convert a normal key into its ^k
// note that given k should uppercase , if itsn't upper ,  we will
// convert it :)
#define CTRL_KEY(k) (CONTROL | ((k >= 'A' && k <= 'Z') ? k : k - ('a' - 'A')))

#endif
