#include "tui.h"
#include "rec.h"
#include <string.h>

/*
 *	alloc for new rectangle struct
 */
struct rectangle *init_rec(int width, int height,char *title)
{
    struct rectangle *rec = malloc(sizeof(struct rectangle));
    rec->info = init_info_struct();
    assert(rec);
    rec->info->width = width;
    rec->info->height = height;
    rec->info->title = title;
    return rec;
}

/*
 *	print rectangle into screen
 */
void draw_rec(struct rectangle *rec)
{
	int width = rec->info->width, height = rec->info->height, b = 0, a = 0;  // initalize local variables
	(Move_cursor)(*term_row - height - 5,*term_col - width - 5);
	int title_len = 0;
	if (rec->info->title != NULL)
        title_len = strlen(rec->info->title);
	if (width < title_len) {
        width += title_len - width + 4;
	}
	// height index (row index)
    int hi = 0;
	for (a = 0; a != height; a++) {
    	(Move_cursor)(*term_row - height - 2 + hi++,*term_col - width - 2);
		// fill the width
		for (b = 0; b < width; b++ ) {
			if ((a == 0) || (a == height - 1) || (b == width - 1) || (b == 0)) {  // put all * printing condition in one place
				// also, conditions, (a == height-1) and (b == width-1) to be used
			    if (b == (width - title_len) / 2 && title_len != 0 && a == 0) {
    			    (Write_string)(rec->info->title);
    			    b += title_len - 1;
    			    continue;
			    }
			    if (a == 0 && b == 0) {
                    (Write_string)("\u256d");
			    } else if (a == 0 && b == width - 1) {
                    (Write_string)("\u256e");
			    } else if (a > 0 && a < height - 1) {
                    (Write_string)("\u2502");
    			} else if (a == height - 1 && b == 0) {
                    (Write_string)("\u2570");
        	    } else if (a == height - 1 && b == width - 1) { 
            	    (Write_string)("\u256f");
            	} else
    			    (Write_string)("\u2500");
			}
			else   // if not to print *, print space
    			(Write_string)(" ");
		}
		(Move_nrow)();
	}
}
