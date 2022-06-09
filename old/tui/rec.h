#ifndef _REC_H
# define _REC_H

#include "tui.h"

struct rectangle {
    struct tui_info *info;
    char **texts;
    int text_count;
    int buf_size;
};

struct rectangle *init_rec(int, int, char *);

/*
 * print given rec into screen
 */
void draw_rec(struct rectangle *);

void append_string_to_rec(struct rectangle *, char *);

#endif
