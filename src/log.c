/*
 *	Log informations into temp file
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

void lv_log(const char *msg, ...)
{
	char temp[512];
	va_list ap;
	va_start(ap, msg);
	vsnprintf(temp, sizeof(temp), msg, ap);
	buffer *old_buffer = curbp;
	curbp = debug_win->fbuffer;
    append_line(debug_win->fbuffer, line_alloc(temp, strlen(temp)));
    curbp = old_buffer;
	va_end(ap);
}
