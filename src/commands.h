#ifndef _COMMDNDS_H
# define _COMMANDS_H

/*
 *	Manager editor internal commands
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

struct command_t {
	LINK(command) link;
	char *name;		/* command name 		  */
	int	flag;		/* execution flags 		  */
	int (*func)(int,int); /* function bound to name */
};

#define cnext(m) ((m)->link.next)
#define cprev(m) ((m)->link.prev)

extern int commands_count;	/* total count of commands */
extern command *fcommand;	/* first generated command */
extern command *lcommand;	/* last appended command   */

command *init_command(char *,int(*)(int,int),int);

/*
 * append command to our list
 */
void append_command(command *);

/*
 *	this routin will find a command by it's	name 
 */
command *find_command(char *);


#endif
