#include "tui.h"
#include "rec.h"

void draw_rec(struct rectangle *rec)
{
	int width = rec->info->width, height = rec->info->height, b = 0, a = 0;  // initalize local variables
	Move_cursor(*term_row - height - 5,*term_col - width - 5);
	// height index (row index)
    int hi = 0;
	for (a = 0; a != height; a++) {
    	(Move_cursor)(*term_row - height - 2 + hi++,*term_col - width - 2);
		// fill the width
		for (b = 0; b != width; b++ ) {
			if ((a == 0) || (a == height - 1) || (b == width - 1) || (b == 0)){  // put all * printing condition in one place
				//also, conditions, (a == height-1) and (b == width-1) to be used
			    Write_char('*');
			}
			else   // if not to print *, print space
    			(Write_char)(' ');
		}
		(Move_nrow)();
	}
}
