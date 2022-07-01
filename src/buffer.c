#include "types.h"
#include <fcntl.h>
#include <errno.h>

public buffer *buffer_alloc()
{
	buffer *buf = malloc(sizeof(buffer));
	if (!buf) {
		show_message(true, "Out Of Memory!");
		return NULL;
	}
	return buf;
}

public void buffer_init(buffer *buf, char *file_name)
{
	L_LINK_RESET(buf);
	buf->first_line = buf->current_line = NULL;
	buf->line_count = buf->line_offset = buf->char_offset = 0;
	buf->is_modified = false;
	buffer_set_file(buf, file_name);
	buf->need_text_update = true;
}

public void buffer_kill(buffer *buf)
{
	buffer *new_buffer = L_LINK_PREV(buf) ? L_LINK_PREV(buf) :
		L_LINK_NEXT(buf) ? L_LINK_NEXT(buf) : NULL;
	
	/* for (line *ln = buf->first_line; L_LINK_NEXT(ln); ln = L_LINK_NEXT(ln)) */
		/* line_free(ln); */

	/*	yeap, we assume that give buffer is from current_window, it should be 
	 *	how we can delete buffer from another window ?
	 */
	current_window->buffer_count--;
	
	/* if (current_buffer == buf) */
		/* buffer_activate(new_buffer); */
	if (new_buffer) {
		current_buffer = new_buffer;
		if (buf == current_window->first_buffer)
			current_window->first_buffer = new_buffer;
		L_LINK_REMOVE(buf);
		free(buf->file_path);
		free(buf);
	} else {
		window_kill();
	}
}

public void buffer_append(buffer *buf)
{
	if (current_window->first_buffer) {
		L_LINK_INSERT(current_buffer, buf);
	} else
		current_window->first_buffer = buf;
	current_window->buffer_count++;
}

public void buffer_open_file(buffer *buf, char *file_name)
{
	buffer_set_file(buf, file_name);
	if (file_exists(file_name)) {
		buffer_load_file(buf, file_name);
	}
	buffer_text_update();
}

public void buffer_set_file(buffer *buf, char *path)
{
	if (path == NULL) {
		buf->file_name = "[ NO NAME]";
		return;
	}
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
	/* Init First line handy, to set last_line
	 * we did it because of speed, we always save last line because in large
	 * files , we need to get last line in every loop and its lose of speed
	 * assume what gonna happen when file have more than 50,000 lines :)
	 */
	if ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		ln = line_init(line_chars, line_length);
		buf->first_line = buf->current_line = ln;
		last_line = ln;
		buf->line_count++;
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

// TODO : show messages in command bar
public bool buffer_save(buffer *buf)
{
	if (!buf->file_path) {
		char *buffer_file = prompt_string(cwd, "Enter File Name For Buffer :");
		if (buffer_file)
			buffer_set_file(current_buffer,  buffer_file);
		else
			return false;
	}
	
	FILE *fp = fopen(buf->file_path, "w+");
	if (!fp) {
		goto writeerr;
	}
	line *ln = buf->first_line;
	for (int i = 0; i < buf->line_count; i++) {
		fprintf(fp, "%s\n", ln->chars);
		ln = L_LINK_NEXT(ln);
	}
	fclose(fp);
	current_buffer->is_modified = false;
	return true;

writeerr:
	show_message(true, "%s", strerror(errno));
    if (fp)
		fclose(fp);
    return false;
}

public char *buffer_lines_to_string(buffer *buf, int *len)
{
    char *string = NULL, *p;
    int totlen = 0;
    int j;

    /* Compute count of bytes */
	line *ln = buf->first_line;
    for (j = 0; j < buf->line_count; j++) {
        totlen += ln->len; /* +1 is for "\n" at end of every row */
		ln = L_LINK_NEXT(ln);
	}
    p = string = malloc((*len = totlen) + 1);
	ln = buf->first_line;
    for (j = 0; j < buf->line_count; j++) {
        memcpy(p, ln->chars, ln->len);
        p += ln->len;
        *p++ = '\n';
		ln = L_LINK_NEXT(ln);
    }
    *p = 0;
    return string;
}

public void buffer_line_append(buffer *buf, line *ln)
{
	line *last_line = buf->first_line;
	if (last_line)
		for (; L_LINK_NEXT(last_line); last_line = L_LINK_NEXT(last_line))
			;
	buffer_line_append_after(buf, last_line, ln);	
}

public void buffer_line_append_after(buffer *buf, line *dest, line *new)
{
	if (!buf->first_line || !dest) {
		buf->first_line = buf->current_line = new;
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


public line *buffer_current_line()
{
	return current_buffer->current_line;
}

public void buffer_modified()
{
	current_buffer->is_modified = true;
	buffer_text_update();
}

public void buffer_text_update()
{
	current_buffer->need_text_update = true;
}


public void buffer_next()
{
	if (!L_LINK_NEXT(current_buffer)) {
		show_message(true, "Last buffer");
		return;
	}
	current_buffer = L_LINK_NEXT(current_buffer);
	buffer_text_update();
}

public void buffer_prev()
{
	if (!L_LINK_PREV(current_buffer)) {
		show_message(true, "First Buffer");
		return;
	}
	current_buffer = L_LINK_PREV(current_buffer);
	buffer_text_update();
}
