#ifndef _FILE_H
# define _FILE_H

/*
 *	Deal with systemfiles :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define FNOTF	0x001	/* file not found */
#define FCRTD 	0x002	/* file created   */
#define FADND	0x004	/* access denied  */

/*
 *	will take a file path , and load it into given buffer
 *	if buffer is empty , it will load it into current buffer
 *	IMPORTANT : if we are loading into current buffer , it will clear first
 */
int load_file_into_buffer(buffer *,char *);

#endif
