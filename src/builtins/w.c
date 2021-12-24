/*
 *	write buffer command
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 *	write buffer into associated file
 *	possible args :
 *	- file path : to specify location of the file , if it's null
 *		we will save into buffer fname
 */
int write_cmd(int f, char **args)
{
	save_file();
	return true;
}
