#ifndef _UNDO_H
# define _UNDO_H

/*
 *	Undo implemention
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	this is our main undo object , we store each moves in this struct.
 *	every buffer have a property with this type , to store undo packets , 
 *	they have a link like any other struct (buffer , window, line and etc).
 *	with type we have specify that event point , for example if that is 
 *	delete event , we will append the 'data' array to line
 */
struct undo_packet_t {
    L_LINK(undo_packet) link;
    short type;		/* to specify what kind of action that was    */
    int lineno;		/* line number that this action happend       */
    int offset;		/* offset from start of line for action       */
    char data[32];	/* removed data of appended data in line      */
    int data_index;	/* like length, but starts from 0 O_o		  */
    line *ln;		/* this is usefull when we delete a full line */
};

/*
 *	we use of this function as a propr for buffer , because we need to have
 *	all changes with current active change.
 */
struct change_db_t {
    undo_packet *db;			 /* store all changes here as a likned list */
    undo_packet *current_change; /* current active change   				*/
    int count;
};

/*
 *	some usefull macro for return db and current change
 */ 
#define get_change_db(b) 	  ((b)->change_db->db)
#define get_current_change(b) ((b)->change_db->current_change)
#define set_current_chagne(c) ((curbp)->change_db->current_change = c)

/*
 *	possible states of a change
 */
enum {
    DELETE,
    DELETE_LINE,
    INSERT
};

/*
 *	alloc and return new undo_packet
 */
undo_packet *init_undo_packet();

void save_undo_by_macro(key_macro *);

/*
 *	this function is used to do a undo move
 */
int do_undo(int,int);

/*
 *	return last packet of current buffer database
 */
undo_packet *get_last_packet();

/*
 *	append packet into db
 */ 
void append_undo(undo_packet *packet);

/*
 *	apply a undo into buffer
 */
void apply_undo(undo_packet *packet);

#endif
