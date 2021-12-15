/*
 *	Manager buffers 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

buffer *init_buffer(char *filename, char *buffername,short modes,short flags)
{
	buffer *bf;
	if (!(bf = malloc(sizeof(buffer))))
		die("malloc buffer");

	if (strlen(buffername) < NBUFN) { 
		lv_strncpy(bf->bname,buffername,NBUFN);
	}
	if (strlen(filename) < NFILEN)
		lv_strncpy(bf->fname,filename,NFILEN);
	/* when a buffer initialized , we have set lock mode for it , if we don't want to be locked ,we must specify it at modes input */
	bf->modes |= MDLOCK;
	bf->modes |= modes;
	bf->flags = FFULLS;
	bf->flags = flags;
	bf->coffset = 0;
	bf->clindex = 0;
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
	if (curwp->fbuffer == NULL)
		curwp->fbuffer = bf;
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

/*
 *	set given mode for buffer
 *	we have to check given mode , for example if that was insert mode
 *	we have to turn lock and visual mode to off and ...
 */
void set_mode_for_buffer(int mode)
{
	if (mode == MDLOCK) {
		usmode(curbp,MDVISL);
		usmode(curbp,MDINST);
		stmode(curbp,MDLOCK);
	} else if (mode == MDVISL) {
		usmode(curbp,MDLOCK);
		usmode(curbp,MDINST);
		stmode(curbp,MDVISL);
	} else if (mode == MDINST) {
		usmode(curbp,MDVISL);
		usmode(curbp,MDLOCK);
		stmode(curbp,MDINST);
	} else {
		stmode(curbp,mode);
	}
}
