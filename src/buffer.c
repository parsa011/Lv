/*
 *	Manager buffers 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

buffer *init_buffer(char *filename, char *buffername,char modes)
{
	buffer *bf;
	if (!(bf = malloc(sizeof(buffer))))
		die("malloc buffer");

	if (strlen(buffername) < NBUFN) { 
		lv_strncpy(bf->bname,buffername,NBUFN);
	}
	if (strlen(filename) < NFILEN)
		lv_strncpy(bf->fname,filename,NFILEN);
	bf->mode = modes;
	return bf;
}

/*
 *	add given buffer into active window last buffers next
 */
void append_buffer(buffer *bf)
{
	if (curwp == NULL)
		die("NO any window");
	/* if currwp buffers null , so it's first one , otherwise add to last buffers next */
	if (curwp->buffers == NULL)
		curwp->buffers = bf;
	else {
		buffer *lbuffer = get_last_buffer(NULL);
		if (lbuffer == NULL)
			die("Something went wrong ;/");
		sbnext(lbuffer,bf);
	}
	curwp->bcount++;
}

/*
 *	return last line of given buffer 
 *	if given buffer is NULL we will get last line of curbp
 */
line *get_last_line(buffer *bf)
{
	/* set to curwp if wp is null */
	buffer *buff = bf != NULL ? bf : curbp;
	line *l = buff->fline;
	for (; l != NULL;l = lnext(l));
	return l;
}
