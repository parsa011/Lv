#ifndef _HIGHLIGHT_H
# define _HIHGLIGHT_H

/*
 *	Highlight service for editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

struct syntax_field_t {
	syntax_field *next;
	char *keyword;		/* keyword as string for compare  					  */
	char *prop[5];		/* properties for keyword like color and bold and ... */
	char *file_type;	/* to specify , for what type (lang) this keyword is  */
};

extern int syntax_count;
extern syntax_field *syntax_db;

/*
 *	search in syntax files and read syntaxes from file
 */
void load_syntax(char *);

/*
 *	add new syntax to filed
 */
void add_synatx(syntax_field);

#endif
