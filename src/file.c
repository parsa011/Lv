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
	memcpy(curbp->fname,filepath,strlen(filepath));
	get_filename_of_path(curbp->bname,filepath);
	if (!fp) 
		return file_notfound();
	/* if given buffer is NULL using curbp instead */
	buf = buf != NULL ? buf : curbp;
	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline(&line_chars, &linecap, fp)) != EOF) { 
		append_line(curbp,line_alloc(line_chars,linelen));
	}
	free(line_chars);
	fclose(fp);
}
