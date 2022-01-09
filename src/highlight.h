#ifndef _HIGHLIGHT_H
# define _HIHGLIGHT_H

/*
 *	Highlight service for editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

struct syntax_field {
	char *keyword;
	char *color;
	char *file_type;
};

/*
 *	search in syntax files and read syntaxes from file
 */
void load_syntax(char *);

#endif
