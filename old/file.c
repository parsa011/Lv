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
 *	global open file for work with especial file , so we can have access to that file
 *	in other functions
 */
int open_file(char *filepath,char *mode)
{
	fp = fopen(filepath,mode);
	return true;
}

/*
 *	load file of given path and write it into buffer
 */
int load_file_into_buffer(buffer *buf,char *filepath)
{
	open_file(filepath,"r");
	/* if file not found , return NOT FOUND */
	set_buffer_name(filepath);
	if (!fp) 
		return file_notfound();
	/* if given buffer is NULL using curbp instead */
	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline(&line_chars, &linecap, fp)) != EOF) { 
		append_line(buf,line_alloc(line_chars,linelen));
	}
	curbp->change_db->count = 0;
	close_file();
	free(line_chars);
	return true;
}

/*
 *	save buffer lines into associated file
 */
int save_file()
{
	if (bmtest(curbp,MDVIEW)) {
		return read_only();
	}
	if (curbp->fname[0] == 0) {
		showmsg(false,"(buffer should have a name to save)");
		return false;
	}
	showmsg(true,"writing file ...");
	open_file(curbp->fname,"w+");
	for (line *ln = curbp->fline;ln != NULL;ln = L_LINK_NEXT(ln)) {
		if (fputline(ln) == FALSE)
			return false;
	}
	curbp->change_db->count = 0;
	showmsg(true,"file wroted in %s",curbp->fname);
	close_file();
	return true;
}

/*
 *	put line into file 
 */
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

/*
 *	close global file
 */
void close_file()
{
	if (fp) {
		fclose(fp);
		fp = NULL;
	}
}
