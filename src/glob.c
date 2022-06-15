#include "types.h"

editor global_editor;
window current_window;
buffer *current_buffer;

public uint64_t current_line_index()
{
	return cursor_row + current_buffer->line_offset - global_editor.show_tabs - 1;
}
