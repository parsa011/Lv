/*
 *	Main file of lv editor :)	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

int main(int argc,char *argv[])
{
	// initialize the terminal , and activate raw mode
	TTopen();
	TTeeop();
	TTmove(0,0);
	int c;
	while (1) {
		c = ttgetc();
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;
	}

	return 0;
}
