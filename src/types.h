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
typedef struct command_t command;
typedef struct msg_bag_t msg_bag;
typedef struct terminal_t terminal;
typedef struct key_macro_t key_macro;
typedef struct prompt_key_t prompt_key;
typedef struct syntax_field_t syntax_field;
typedef struct syntax_group_t syntax_group;
typedef struct language_syntax_t language_syntax;

#include "builtins/builtins.h"
#include "color.h"
#include "macro.h"
#include "tty.h"
#include "utils.h"
#include "glob.h"
#include "utf8.h"
#include "keys.h"
#include "input.h"
#include "window.h"
#include "line.h"
#include "buffer.h"
#include "move.h"
#include "vt100.h"
#include "display.h"
#include "file.h"
#include "word.h"
#include "commands.h"
#include "prompt.h"
#include "lv.h"
#include "highlight.h"
#include "../libs/llist.h"
#include "log.h"
