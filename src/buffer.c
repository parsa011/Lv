#include "types.h"

public void buffer_init(buffer *buf, char *file_name)
{
	buf->link.next = buf->link.prev = NULL;
	buf->first_line = NULL;
	buffer_set_file(buf, file_name);
}

public void buffer_open_file(buffer *buf, char *file_name)
{
	buffer_set_file(buf, file_name);
	if (file_exists(file_name)) {
		buffer_load_file(buf, file_name);
	} else {
		// TODO : get y/n to create file or no
	}
}

public void buffer_set_file(buffer *buf, char *path)
{
	if (path == NULL)
		path == "[ NO NAME]";
	buf->file_path = path;
	buf->file_name = get_filename_of_path(path);
}

public void buffer_load_file(buffer *buf, char *path)
{
	if (!file_exists(path))
		return;
	FILE *fp = fopen(path, "r");
	
	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t line_length;

	line *ln;
	while ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		ln = line_init(line_chars, line_length);
		buffer_line_append(buf, ln);
	}
	free(line_chars);
	fclose(fp);
}

public void buffer_line_append(buffer *buf, line *ln)
{
	if (!buf->first_line) {
		buf->first_line = ln;
	} else {
		line *last_line = buf->first_line;
		for (; L_LINK_NEXT(last_line); last_line = L_LINK_NEXT(last_line))
			;
		L_LINK_INSERT(last_line, ln);
	}
	buf->line_count++;
}

public line *buffer_get_line_by_index(buffer *buf, uint64_t index)
{
	int lines_passed = 0;
	line *ln = buf->first_line;
	for (; ln; ln = L_LINK_NEXT(ln)) {
		if (lines_passed++ == index)
			return ln;
	}
	return NULL;
}

public uint64_t buffer_line_index()
{
	return cursor_row + current_buffer->line_offset - global_editor.show_tabs - 1;
}
