#ifndef _UNDO_H
# define _UNDO_H

typedef enum {
	CHANGE_DELETE_LINE,
	CHANGE_DELETE_CHAR,
	CHANGE_DELETE_WORD,
	CHANGE_INSERT_CHAR,
	CHANGE_INSERT_STRING
} change_type;

/*
 *	every buffer have one of this structs as field, it hold our linked list of 
 *	buffer changes and current buffer that we have to do when user presed undo/redo
 *	buttons
 */
struct change_pack_t {
	int count;
	change_pack_entry *entries;
	change_pack_entry *current;
};

#define CHANGE_ENTRY_BUF_LEN 128
/*
 *	with every change in lines, a record will be inserted to change_pack of that buffer in this 
 *	struct, we hold type of change, content and line number and char offset, so we can undo that
 *	change with this informations
 *	
 *	For Example :
 *
 *		32 | hello  --------->  32 | hello parsa
 *
 *		type        : CHANGE_INSERT_STRING
 *		line_number : 32
 *		buffer      : parsa
 *		char_offset : strlen(hello) - 1 :D
 *
 *	So when we want to do undo , we will remove buffer from that position
 */
struct change_pack_entry_t {
	L_LINK(change_pack_entry) link;
	change_type type;
	uint64_t line_number;
	uint8_t char_offset;
	char buffer[CHANGE_ENTRY_BUF_LEN];
	int bufp;
};

/*
 *	do an undo for buffer
 */
public void do_undo();

/*
 *	Insert new undo pack to buffer changes pack, it's for when we insert new char into
 *	current line
 */
public void undo_insert_char(change_type type, uint64_t line_number, uint8_t char_offset , char ch);

#endif
