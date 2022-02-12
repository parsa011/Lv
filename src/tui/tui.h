#ifndef _TUI_H
#define _TUI_H

#include "rec.h"

/*
 *	these function pointer are used to move cursor (next , prev) col and row .
 *	we use of these so that this library does not depend on a specific program .
 *  these will set on function call , as a pointer of associated functions , 
 *  in destination program .
 */
extern void (*Move_ncol)(),(*Move_pcol)(),(*Move_nrow)(),(*Move_prow)();
extern void (*Move_cursor)(int, int);
extern void (*Write_string)(char *);
extern void (*Write_char)(char);

/*
 * as a pointer to destination program terminal size variables
 */
extern int *term_col, *term_row;

/*
 *	this is base struct that all components will have this
 */
struct tui_info {
    int width;
    int height;
    int margin_top;
    int margin_left;
};

/*
 * init and set base variables
 */
void inti_tui(
    void (*ncol)(),void (*pcol)(),void (*nrow)(),void (*prow)(),
    void (*cursor)(int, int),
    void (*wchar)(char), void (*wstring)(char *),
    int *col,int *row
);

#endif // src/tui/tui_h_INCLUDED

