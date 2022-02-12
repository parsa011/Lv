#ifndef _REC_H
# define _REC_H

#include "tui.h"

struct rectangle {
    struct tui_info *info;
};

/*
 * print given rec into screen
 */
void draw_rec(struct rectangle *);

#endif
