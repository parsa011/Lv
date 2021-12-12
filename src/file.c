/*
 *	Deal with systemfiles :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

/*
 *	load file of given path and write it into buffer
 */
int load_file_into_buffer(buffer *buf,char *filepath)
{
	FILE *fp = fopen(filepath, "r");
	/* if file not found , return NOT FOUND */
	if (!fp) 
		return FNOTF;
	/* if given buffer is NULL using curbp instead */
	buf = buf != NULL ? buf : curbp;
	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	line *last_line = NULL;
	line *ln = NULL;
	while ((linelen = getline(&line_chars, &linecap, fp)) != EOF) { 
		/* remove extra 'new line' and 'line feed' characters of end of line */
		while (linelen > 0 && (line_chars[linelen - 1] == '\n' || line_chars[linelen - 1] == '\r'))
			linelen--;
		/* 
		 * if buffer dont have any line , so this is first line 
		 * in other hand , we have to append this line to next of last line
		 * and then set last line to this new line
		 */
		ln = line_alloc(line_chars,linelen);
		if (buf->fline == NULL && buf->lcount == 0) {
			buf->fline = ln;
			buf->cline = ln;
			buf->hline = ln;
		} else {
			slnext(buf->lline,ln);
		}
		/* i think this line dont need any comment , but to be sure : increase total count of buffer lines */	
		buf->lline = ln;
		slprev(buf->lline,last_line);
		buf->lcount++;
		last_line = ln;
	}
	free(line_chars);
	fclose(fp);
}
