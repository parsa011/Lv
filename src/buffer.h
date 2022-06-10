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
	uint64_t line_count;
	uint64_t current_line_index;
	uint64_t line_offset;
	uint8_t char_offset;
	char file_name[BUFFER_NAME_MAX_LENGTH];
	char file_path[BUFFER_NAME_MAX_LENGTH];
	uint8_t mode;

	/* Options for buffer */
	bool line_number;
	bool auto_indent;
	bool auto_pair;
	bool syntax_highlight;
};

/*
 *	Initialize basic informations for buffer, and open file into buffer if file_name
 *	isn't nullb
 */
void buffer_init(buffer *buf, char *file_name);

#endif
