#ifndef _KEYS_H
# define _KEYS_H

/*
 *	Definitions of keys	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define CONTROL 0x01
#define META 	0x02

// conver a normal key into its ^k
#define CTRL_KEY(k) (k & 0x1F)

#endif
