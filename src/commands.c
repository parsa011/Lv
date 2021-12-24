/*
 *	Manager editor internal commands
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int commands_count = 0;	/* total count of commands */
command *fcommand;	/* first generated command */
command *lcommand;	/* last appended command   */

command *init_command(char *name,int (*func)(int,char **),int flags)
{
	command *cmd= calloc(1,sizeof(command));
	cmd->name = name;
	cmd->func = func;
	cmd->flag = flags;
	return cmd;
}

void append_command(command *cmd)
{
	if (fcommand == NULL)	{
		fcommand = cmd;
	} else {
		lcommand->link.next = cmd;
	}
	cmd->link.prev = lcommand;
	lcommand = cmd;
	commands_count++;
}

command *find_command(char *name)
{
	for (command *cmd = fcommand; cmd != NULL; cmd = cnext(cmd))
		if (strcmp(cmd->name,name) == 0)
			return cmd;
	return NULL;
}
