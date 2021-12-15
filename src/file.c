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
	get_filename_of_path(curbp->bname,filepath);
	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline(&line_chars, &linecap, fp)) != EOF) { 
		/* remove extra 'new line' and 'line feed' characters of end of line */
		while (linelen > 0 && (line_chars[linelen - 1] == '\n' || line_chars[linelen - 1] == '\r'))
			linelen--;
		append_line(curbp,line_alloc(line_chars,linelen));
	}
	free(line_chars);
	fclose(fp);
}
