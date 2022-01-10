#ifndef _HIGHLIGHT_H
# define _HIHGLIGHT_H

/*
 *	Highlight service for editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

struct language_syntax_t {
	language_syntax *next;
	char *lang;						/* language name 						*/
	syntax_field *syntax_groups; 	/* groups of syntaxes for this language */
};

struct syntax_group_t {
	syntax_group *next;
	char *group_name;		/* name of this group , will be specified in the file */
	syntax_field *keywords;	/* keyword as string for compare  					  */
	char *props[5];			/* properties for keyword like color and bold and ... */
};

struct syntax_field_t {
	syntax_field *next;
	char *keyword;
};

extern int syntax_count;
extern language_syntax *syntax_db;

/*
 *	search in syntax files and read syntaxes from file
 */
void load_syntax(char *);

/*
 *	line is a lins of syntax file , first we will tokenize it and make a loop on it
 *	then we will find keywords and colors in line
 */
void parse_syntax_line(char *line);

/*
 *	add new syntax to filed
 */
void add_synatx(syntax_field);

#endif
