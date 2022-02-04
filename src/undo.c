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
    undo_packet *packet = lv_malloc(sizeof(undo_packet));
    packet->link.next = packet->link.prev = 0;
    return packet;
}

/*
 *	it will store changes if given macro will modify buffer.
 */
void save_undo_by_macro(key_macro *m)
{
    undo_packet *buffer_db = get_change_db(curbp);
    undo_packet *packet = init_undo_packet();
    if (strcmp(m->key_str,"d") == 0) {
        packet->type = DELETE;
        packet->data[0] = lgetc(current_line,curbp->coffset);
        packet->offset = curbp->coffset;
        packet->lineno = curbp->clindex + 1;
        append_undo(packet);
    } else if (strcmp(m->key_str,"x-d") == 0) {
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
        /* if we have needed change db , we should delete first one */
        if (curbp->change_db_size < curbp->change_db->count) {
            undo_packet *pack = get_change_db(curbp);
            curbp->change_db->db = L_LINK_NEXT(get_change_db(curbp));
            L_LINK_SPREV(get_change_db(curbp),0);
            free(pack);
            curbp->change_db->count--;
        }
        int count = 0;
        for (undo_packet *p = get_change_db(curbp); L_LINK_NEXT(p) != 0; p = L_LINK_NEXT(p),count++);
        lv_log("change pack count is : %d",count);
        L_LINK_INSERT(get_last_packet(),packet);
    }
    curbp->change_db->count++;
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
    if (!get_current_change(curbp)) {
        showmsg(false,"no any change");
        return false;
    }
    apply_undo(get_current_change(curbp));
    return true;
}

/*
 *	actully this code is like to run a undo packet :)
 *	it will apply content of given buffer to buffer
 */
void apply_undo(undo_packet *packet)
{
    if (packet->type == DELETE_LINE) {
         goto_line(true,packet->lineno);
         line_new_up(true,1);
         // TODO : implement line append 
         current_line->chars = packet->ln->chars;
         current_line->len = packet->ln->len;
    } else if (packet->type == DELETE) {
         goto_line(true,packet->lineno);
         curbp->coffset = packet->offset;
         cursor_col = convert_coffset_to_cursorcol(current_line->chars,curbp->coffset);
         line_ins_char(packet->data[0]);
         prev_char(true,1);
    }
    if (get_current_change(curbp) != NULL)
        set_current_chagne(L_LINK_PREV(get_current_change(curbp)));
}
