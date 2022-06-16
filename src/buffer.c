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
	buf->is_modified = true;
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

	line *ln, *last_line;
	/* Init First line by hand, to set last_line
	 * we did it because of speed, we always save last line because in large
	 * files , we need to get last line in every loop and its lose of speed
	 * assume what gonna happen when file have more than 50,000 lines :)
	 */
	if ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		ln = line_init(line_chars, line_length);
		buf->first_line = ln;
		last_line = ln;
	}
	/* read other lines and add them to buffer */  
	while ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		ln = line_init(line_chars, line_length);
		buffer_line_append_after(buf, last_line, ln);
		last_line = ln;
	}
	free(line_chars);
	fclose(fp);
}

public void buffer_line_append(buffer *buf, line *ln)
{
	line *last_line = buf->first_line;
	for (; L_LINK_NEXT(last_line); last_line = L_LINK_NEXT(last_line))
		;
	buffer_line_append_after(buf, last_line, ln);	
}

public void buffer_line_append_after(buffer *buf, line *dest, line *new)
{
	if (!buf->first_line) {
		buf->first_line = new;
	} else {
		L_LINK_INSERT(dest, new);
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

/*
 *	it can be slow too, but we cant do anything else, because we use of linked list
 *	we have to go throw the list to get our destination
 *	Or we can save current line of buffer in buffer struct but it's hard to track
 *	it every time that we change line (line remove , move and ...), s easiest way it
 *	this but a little bit time wasting in large file, NP :))) we dont have very large files
 *	and today computers are fast enough to handle it :O
 */
public line *buffer_current_line()
{
	return buffer_get_line_by_index(current_buffer, buffer_line_index());
}
