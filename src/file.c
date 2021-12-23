/*
 *	Deal with systemfiles :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

FILE *fp;

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

/*
 *	save buffer lines into associated file
 */
int save_file(int f,int n)
{
	if (bmtest(curbp,MDVIEW)) {
		return read_only();
	}
	if (curbp->fname[0] == 0) {
		showmsg(false,"(buffer should have a name to save)");
		return false;
	}
	showmsg(true,"writing file ...");
	fp = fopen(curbp->fname,"w");
	for (line *ln = curbp->fline;ln != NULL;ln = lnext(ln)) {
		if (fputline(ln) == FALSE)
			return false;
	}
	showmsg(true,"file wroted in %s",curbp->fname);
	return true;
}

int fputline(line *ln)
{
	for (int i = 0; i < ln->len; ++i)
		fputc(ln->chars[i] & 0xFF, fp);
	fputc('\n', fp);

	if (ferror(fp)) {
		showmsg(true,"Write I/O error");
		return false;
	}
	return true;
}
