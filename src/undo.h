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
    line *ln;		/* this is usefull when we delete a full line */
};

int undo(int,int);

int apply_undo();

#endif
