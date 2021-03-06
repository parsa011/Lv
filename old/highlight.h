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
	char *props[5];			/* properties for keyword like color and bold and ... */
};

/*
 *	we store each keyword of group in syntax file here 
 */
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
 *	will return language syntax with for given lang
 */
language_syntax *get_language_syntax_by_name(char *);

/*
 *	append given group to end of language groups
 */
void add_group_for_language(language_syntax *,syntax_group *);

/*
 *	append given key to group
 */
void add_key_to_syntax_group(syntax_group *,char *);

/*
 *	return equivalent sentence with given prop
 */
char *prop_to_str(char *);

/*
 *	line is a lins of syntax file , first we will tokenize it and make a loop on it
 *	then we will find keywords and colors in line
 */
syntax_group *parse_syntax_line(char *line);

/*
 *	will return associated props with given syntax
 */
char **get_syntax_for_keyword(char *);

#endif
