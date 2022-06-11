#include "basic.h"

typedef struct line_t line;
typedef struct editor_t editor;
typedef struct window_t window;
typedef struct buffer_t buffer;

/*
 * can be used as field in structs or anywhere else to hold cursor position info
 */
typedef struct {
	int x;
	int y;
} cursor_position;

#define CURSOR_POS(x, y) ((cursor_position) {x, y})

/*
 * these types are useful when we are working with some functions that thier taks is to
 * add new element to a list. these can sepcify where we want to add that element in list  
 */
typedef enum {
	APPEND_FIRST,
	APPEND_MIDDLE,
	APPEND_END
} append_type;

#include "../libs/llist.h"
#include "glob.h"
#include "display.h"
#include "tty.h"
#include "key.h"
#include "line.h"
#include "buffer.h"
#include "window.h"
