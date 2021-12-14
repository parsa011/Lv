#ifndef _KEYS_H
# define _KEYS_H

/*
 *	Definitions of keys	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define CONTROL 0x1000000
#define META 	0x2000000

// convert a normal key into its ^k
#define CTRL_KEY(k) (CONTROL | k)

#endif
