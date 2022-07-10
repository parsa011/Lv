#ifndef _BUFFER_H
# define _BUFFER_H

/*
 *	possible modes for a buffer, modes are like VIM modes
 */
typedef enum {
	MODE_LOCK = 1,
	MODE_INSERT = 2,
	MODE_VISUAL = 4,
	MODE_SEARCH = 8
} buffer_mode;

#define BUFFER_NAME_MAX_LENGTH 256

/*
 *	buffer contains our file to edit and informations about that
 *  buffer have their optinos like tab size , line number enables and ...
 *  that means we can have line number on in one buffer and off in another one
 *
 *  Args :
 *		line_offset : this is header lines index (line that is in top of screen), or we can say
 *					  it hold that how many lines we passed from first line
 *		char_offset : how far we are from beginning of line
 *		auto_pari : auto close for twin characters , like '(' or '{' and ...
 *		need_text_update : this is different from is_modified, because we everytime that we
 *						refresh the screen, we will turn this off, but we need to have is_modified
 */
struct buffer_t {
	L_LINK(buffer) link;
	line *first_line;
	line *current_line;
	uint64_t line_count;
	uint64_t line_offset;
	uint8_t char_offset;
	char *file_name;
	char *file_path;
	uint8_t mode;
	change_pack changes;

	/* Options for buffer */
	bool is_modified;
	bool need_text_update;
	bool line_number;
	bool auto_indent;
	bool auto_pair;
	bool syntax_highlight;
};

/*
 *	Just alloc new buffer and return pointer
 */
public buffer *buffer_alloc();

/*
 *	Initialize basic informations for buffer, and open file into buffer if file_name
 *	isn't nullb
 */
public void buffer_init(buffer *buf, char *file_name);

/*
 *	kill current buffer and free it, if current buffer is last buffer of window we will
 *	remove widow too :))
 */
public void buffer_kill();

/*
 *	append given buffer to list of window buffers 
 */
public void buffer_append(buffer *buf);

/*
 *	Mostly used to initialize buffer, it will set buffer options that are related to filename
 *  for example path, basename, file_type and ...
 */
public void buffer_set_file(buffer *buf, char *path);

/*
 *	it's similar to set_file, but it will load file into buffer, not just set name for buffer
 *	any other even that could happened after opening file in buffer , should be called here
 */
public void buffer_open_file(buffer *buf, char *path);

/*
 *	load lines of file into buffer and update options of buffer that are related to
 *	lines , like lines_count and ...
 */
public void buffer_load_file(buffer *buf, char *path);

/*
 *	write out strings of buffer to its file
 */
public bool buffer_save(buffer *buf);

/*
 *	Concat all of given buffer lines into one heap-allocated string
 */
public char *buffer_lines_to_string(buffer *buf, int *len);

/*
 *	Append given line to end of buffer lines
 */
public void buffer_line_append(buffer *buf, line *new_line);

/*
 *	this means text of current buffer modified
 */
public void buffer_modified();

/*
 *	append line after dest line
 *	buf it first line of given buffer is null, it will set it to first line
 *	of first line and 'dest' will be unused in this function
 */
public void buffer_line_append_after(buffer *buf, line *dest, line *ln);

/*
 *	return line of give buffer with given index
 */
public line *buffer_get_line_by_index(buffer *buf, uint64_t index);

/*
 *	return index of current line in current buffer
 */
public uint64_t buffer_line_index();

/*
 *	return pointer to current line of given buffer
 */
public line *buffer_current_line();

/*
 *	call this function everywhere that we need to update text section of current buffer
 *	it's better to separate this from is_modified
 */
public void buffer_text_update();

/*
 *	move between buffers in current window
 */
public void buffer_next();
public void buffer_prev();

#endif
