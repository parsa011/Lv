/*
 *	Deal with user to take key 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

const char *all_keys[] = {
	"f-1","\x1b[OP",
	"f-2","\x1b[OQ",
	"f-3","\x1b[OR",
	"f-4","\x1b[OS",
	"f-5","\x1b[[15~",
	"f-6","\x1b[[17~",
	"f-7","\x1b[[18~",
	"f-8","\x1b[[19~",
	"f-9","\x1b[[20~",
	"f-10","\x1b[[21~",
	"f-11","",
	"f-12","\x1b[[24~",
};

int get_key()
{
	int c = TTgetc();

	if (c >= 0x00 && c <= 0x1F)
		c = CONTROL | (c + '@');

	return c;
}
