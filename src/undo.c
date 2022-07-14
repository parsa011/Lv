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
		current_buffer->changes.entries =  entry;
	} else {
		change_pack_entry *last = current_buffer->changes.entries;
		for (; last && L_LINK_NEXT(last); last = L_LINK_NEXT(last))
			;
		L_LINK_INSERT(last, entry);
	}
	current_buffer->changes.current = entry;
	current_buffer->changes.count++;
}

public void do_undo()
{
	
}

public void undo_insert_char(change_type type, uint64_t line_number, uint8_t char_offset , char ch)
{
	change_pack_entry *entry = change_pack_new(type, line_number, char_offset);
	entry->buffer[entry->bufp++] = ch;
	undo_entry_add(entry);
}
