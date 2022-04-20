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
	vsnprintf(temp,sizeof(temp),msg,ap);
    append_line(debug_win->fbuffer, line_alloc(temp,strlen(temp)));
	va_end(ap);
}
