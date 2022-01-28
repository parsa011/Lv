/*
 *	Log informations into temp file
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

#ifdef HAVE_LOG
void log_all()
{
	close_log_file();
	create_log_file();
	lv_log("cursor position is : %d / %d",cursor_row,cursor_col);
}
#endif

static FILE *fp;

void create_log_file()
{
	fp = fopen(LOG_FILE_NAME,"w+");
	if (!fp)
		die("Can not create log file");
}

void lv_log(const char *msg,...)
{
	char temp[512];
	va_list ap;
	va_start(ap, msg);
	vsnprintf(temp,sizeof(temp),msg,ap);
    append_line(debug_win->fbuffer, line_alloc(temp,strlen(temp)));
	va_end(ap);
}

void close_log_file()
{
	if (fp)
		fclose(fp);
}

void remove_log_file()
{
	remove(LOG_FILE_NAME);
}
