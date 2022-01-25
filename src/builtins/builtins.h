#ifndef _BUILTINS_H
# define _BUILTINS_H

/*
 *	definition of builtin commands and functions of editor
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	NOTE : for every command we can have its own prompt key
 *	event , we have to implement them in their file and
 *	when we are typing in prompt , we will check typed command
 *	name , and find it prompt key events in its file and assign them
 *	ez pz :)))
 */

//commands

/*
 *	editor quit commands that will process many input args
 */
int quit(int,char **);
int quite_command_tab(int,int);

/*
 *	write buffer into file command
 */
int write_command(int,char **);
int write_command_tab(int,int);

/*
 *	open file command
 */
int open_command(int,char **);
int open_command_tab(int,int);

/*
 *	this command will show all buffers from all windows
 *	then user can open a buffer with writing it name (bname)
 */
int buffer_command(int,char **);
int buffer_command_tab(int,int);

#endif
