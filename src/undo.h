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
struct change_pack_entry_t {
	L_LINK(change_pack_entry) link;
	change_type type;
	uint64_t line_number;
	char buffer[CHANGE_ENTRY_BUF_LEN];
	char bufp;
};

#endif
