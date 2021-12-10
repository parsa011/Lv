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
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	struct line_t *last_line = NULL;
	struct line *ln = NULL;
	while ((linelen = getline(&line, &linecap, fp)) != EOF) { 
		/* remove extra 'new line' and 'line feed' characters of end of line */
		while (linelen > 0 && (line[linelen - 1] == '\n' ||	line[linelen - 1] == '\r'))
			linelen--;
		/* 
		 * if buffer dont have any line , so this is first line 
		 * in other hand , we have to append this line to next of last line
		 * and then set last line to this new line
		 */
		ln = line_alloc(line,linelen);
		if (buf->fline == NULL) {
			buf->fline = ln;
			buf->cline = ln;
		} else {
			slnext(buf->lline,ln);
		}
		/* i think this line dont need any comment , but to be sure : increase total count of buffer lines */	
		buf->lline = ln;
			slprev(buf->lline,last_line);
		buf->lcount++;
		last_line = ln;
	}
	free(line);
	fclose(fp);
}
