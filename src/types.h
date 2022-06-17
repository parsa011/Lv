#include "basic.h"
#include <assert.h>

typedef struct line_t line;
typedef struct editor_t editor;
typedef struct window_t window;
typedef struct buffer_t buffer;

/*
 * can be used as field in structs or anywhere else to hold cursor position info
 */
typedef struct {
	uint8_t row;
	uint8_t col;
} cursor_position;

#define pos_copy(from, to) { \
	to.row = from.row; \
	to.col = from.col; \
}
#define CURSOR_POS(x, y) ((cursor_position) {x, y})
#define print_pos(pos) (printf("%hhd:%hhd", pos.row, pos.col))
#define reset_pos(pos) {pos.row = 1 + global_editor.show_tabs;pos.col = 1;}

/*
 *	struct to hold rgb colors
 */
typedef struct {
	int red, green, blue;
} color;
#define color_new(red, green, blue) ((color) {red, green, blue})

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
#include "util.h"
#include "glob.h"
#include "display.h"
#include "tty.h"
#include "move.h"
#include "key.h"
#include "line.h"
#include "buffer.h"
#include "window.h"
