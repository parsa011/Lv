/*
 *	Undo implemention
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 *	just alloc for new undo_packet :))
 */
undo_packet *init_undo_packet()
{
    return lv_malloc(sizeof(undo_packet));
}

void save_undo_by_macro(key_macro *m)
{
    int type;
    char data;
    undo_packet *buffer_db = get_change_db(curbp);
    undo_packet *packet = buffer_db ? buffer_db : init_undo_packet();
    if (strcmp(m->key_str,"x") == 0) {
        type = DELETE;
    } else if (strcmp(m->key_str,"d-d") == 0) {
        if (!curbp->cline)
            return;
        packet->type = DELETE_LINE;
        packet->ln = curbp->cline;
        packet->lineno = curbp->clindex + 1;
        append_undo(packet);
    } else {
        return;
    }
}

/*
 *	add given packet into current buffer change_db.
 *  TODO : limit them to specific number (LIFO)
 */
void append_undo(undo_packet *packet)
{
    if (!get_change_db(curbp))
        curbp->change_db->db = packet;
    else {
        L_LINK_INSERT(get_last_packet(),packet);
    }
    set_current_chagne(packet);
}

/*
 *	return last added packet of current buffer
 */
undo_packet *get_last_packet()
{
    undo_packet *up = get_change_db(curbp);
    for (; L_LINK_NEXT(up) != NULL; up = L_LINK_NEXT(up));
    return up;
}

/*
 *	this macro is used for apply a undo packet.
 * 	first we will check that if we have any packet or no
 * 	if we have  , so we will apply it by calling apply_undo function
 */
int do_undo(int f,int n)
{
    if (!get_change_db(curbp)) {
        showmsg(false,"no any change");
        return false;
    }
    apply_undo(get_current_change(curbp));
    return true;
}

void apply_undo(undo_packet *packet)
{
    if (packet->type == DELETE_LINE) {
         goto_line(true,packet->lineno);
         line_new_up(true,1);
         // TODO : implement line append 
         current_line->chars = packet->ln->chars;
         current_line->len = packet->ln->len;
    }
    if (get_current_change(curbp) != NULL)
        set_current_chagne(L_LINK_PREV(get_current_change(curbp)));
}
