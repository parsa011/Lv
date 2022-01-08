#ifndef _LOG_H
# define _LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/*
 *	Log informations into temp file
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define LOG_FILE_NAME "lvlog.txt"

void create_log_file();

#ifdef HAVE_LOG
void log_all();
#endif

void lv_log(const char *,...);

void close_log_file();

void remove_log_file();

#endif
