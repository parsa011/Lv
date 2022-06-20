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

	/* Options for buffer */
	bool is_modified;
	bool line_number;
	bool auto_indent;
	bool auto_pair;
	bool syntax_highlight;
};

/*
 *	Initialize basic informations for buffer, and open file into buffer if file_name
 *	isn't nullb
 */
public void buffer_init(buffer *buf, char *file_name);

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
 *	just set modified true for current buffer to redisplay
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
 *
 *	it can be slow too, but we cant do anything else, because we use of linked list
 *	we have to go throw the list to get our destination
 *	Or we can save current line of buffer in buffer struct but it's hard to track
 *	it every time that we change line (line remove , move and ...), s easiest way it
 *	this but a little bit time wasting in large file, NP :))) we dont have very large files
 *	and today computers are fast enough to handle it :O
 */
public line *buffer_current_line();

#endif
