#include "types.h"

private change_pack_entry *change_pack_new(change_type type, uint64_t line_number, uint8_t char_offset)
{
	change_pack_entry *new_one = malloc(sizeof(change_pack_entry));
	memset(new_one, 0, sizeof(change_pack_entry));
	if (!new_one) {
		exit(0);
	}
	new_one->bufp = 0;
	new_one->type = type;
	new_one->line_number = line_number;
	new_one->char_offset = char_offset;
	return new_one;
}

public void undo_entry_add(change_pack_entry *entry)
{
	if (current_buffer->changes.count == 0) {
		current_buffer->changes.entries = entry;
	} else {
		change_pack_entry *last = current_buffer->changes.current;
		if (last == NULL) {
			last = current_buffer->changes.entries = entry;
		} else {
			change_pack_entry *next = L_LINK_NEXT(entry);
			L_LINK_INSERT(last, entry);
		}
	}
	current_buffer->changes.current = entry;
	current_buffer->changes.count++;
}

public void do_undo()
{
	change_pack_entry *current = current_buffer->changes.current;
	if (current_buffer->changes.count == 0 || current == NULL) {
		show_message(true, "No further undo information");
		return;
	}
	switch (current->type) {
	case CHANGE_INSERT_CHAR :
		if (buffer_line_index() != current->line_number)
			go_to_line(current->line_number);
		go_to_col(current->char_offset);
		next_char();
		line_delete_char();
		break;
	}
	current_buffer->changes.current = L_LINK_PREV(current);
}

public void undo_insert_char(change_type type, uint64_t line_number, uint8_t char_offset , char ch)
{
	change_pack_entry *entry = change_pack_new(type, line_number, char_offset);
	entry->buffer[entry->bufp++] = ch;
	undo_entry_add(entry);
}
