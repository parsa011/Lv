/* Global header file to Lv */

#include "basics.h"

/* Common header files */

#include <stdio.h>
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

#include "lv.h"
#include "macro.h"
