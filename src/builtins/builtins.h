#ifndef _BUILTINS_H
# define _BUILTINS_H

/*
 *	definition of builtin commands and functions of editor
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

//commands

/*
 *	editor quit commands that will process many input args
 */
int quit(int,char **);

/*
 *	write buffer into file command
 */
int write_cmd(int , char **);

#endif
