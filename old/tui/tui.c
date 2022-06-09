#include "tui.h"

void (*Move_ncol)(),(*Move_pcol)(),(*Move_nrow)(),(*Move_prow)();
void (*Write_string)(char *);
void (*Write_char)(char);
void (*Move_cursor)(int, int);

/*
 * as a pointer to destination program terminal size variables
 */
int *term_col, *term_row;

void inti_tui(
    void (*ncol)(),void (*pcol)(),void (*nrow)(),void (*prow)(),
    void (*cursor)(int, int),
    void (*wchar)(char), void (*wstring)(char *),
    int *col,int *row
)
{
    Move_ncol = ncol;
    Move_pcol = pcol;
    Move_nrow = nrow;
    Move_prow = prow;
    Move_cursor = cursor;
    Write_string = wstring;
    Write_char = Write_char;
    term_col = col;
    term_row = row;
}

struct tui_info *init_info_struct()
{
    struct tui_info *info = malloc(sizeof(struct tui_info));
    assert(info);
    return info;
}
