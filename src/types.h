/*
 *	Global header file to Lv
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "basics.h"

/* Common header files */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* Strings needing translation are marked with this macro */
#define _(s) (s)

/* Doubly-linked list node */
#define LINK(type) struct { type *next; type *prev; }

/* Prefix to make string constants unsigned */
#define USTR (unsigned char *)

#ifndef EOF
#define EOF -1
#endif
#define NO_MORE_DATA EOF

#if defined(__TURBOC__)
#define MSDOS 1 /* MS/PC DOS 3.1-4.0 with Turbo C 2.0 */
#else
#define	MSDOS 0
#endif

#if defined(BSD) || defined(sun) || defined(ultrix) || (defined(vax) && defined(unix)) || defined(ultrix) || defined(__osf__)
#ifndef BSD
#define BSD 1 /* Berkeley UNIX */
#endif
#else
#define	BSD 0
#endif

typedef struct line_t line;
typedef struct buffer_t buffer;
typedef struct window_t window;

#include "lv.h"
#include "macro.h"
#include "tty.h"
#include "utils.h"
#include "glob.h"
#include "utf8.h"
#include "keys.h"
#include "input.h"
#include "line.h"
#include "buffer.h"
