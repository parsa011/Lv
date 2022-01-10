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
	syntax_group *syntax_groups; 	/* groups of syntaxes for this language */
};

struct syntax_group_t {
	syntax_group *next;
	char *group_name;		/* name of this group , will be specified in the file */
	syntax_field *keywords;	/* keyword as string for compare  					  */
	syntax_field *lkeyword; /* last added keyword , it's usefull in append :)	  */
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
 *	add given lang to language database
 */
void add_language_syntax(language_syntax *);

/*
 *	append given group to end of language groups
 */
void add_group_for_language(language_syntax *,syntax_group *);

/*
 *	append given key to group
 */
void add_key_to_syntax_group(syntax_group *,char *);

/*
 *	line is a lins of syntax file , first we will tokenize it and make a loop on it
 *	then we will find keywords and colors in line
 */
syntax_group *parse_syntax_line(char *line);

/*
 *	add new syntax to filed
 */
void add_synatx(syntax_field);

#endif
