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
 *	will open file and set 'fp' global FILE pointer
 */
int open_file(char *,char *);

/*
 *	will take a file path , and load it into given buffer
 *	if buffer is empty , it will load it into current buffer
 *	IMPORTANT : if we are loading into current buffer , it will clear first
 */
int load_file_into_buffer(buffer *,char *);

/*
 * save buffer and write lines into associated file
 */
int save_file(int,int);

/*
 *	append given line chars into last of global FILE
 */
int fputline(line *);

void close_file();

#endif
