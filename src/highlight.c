/*
 *	Highlight service for editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int syntax_count = 0;
language_syntax *syntax_db;

/*
 *	with given lang name , we will search in syntax folder
 *	select associated file , read and add its values to db
 */
void load_syntax(char *lang_name)
{
	char file_path[128];
	sprintf(file_path,"./syntax/%s.lvs",lang_name);
	FILE *fp = fopen(file_path,"r");	
	if (!fp) 
		die("language syntax file not founded");
	language_syntax *lang = malloc(sizeof(language_syntax *));
	lang->lang = strdup(lang_name);
	char *line = NULL;
	size_t len;
	ssize_t read;
	while ((read = getline(&line,&len,fp)) != -1) {
		if (line[0] == '#')
			continue;
		syntax_group *grp = parse_syntax_line(line);
		if (grp == NULL)
			continue;
		add_group_for_language(lang,grp);
	}
	add_language_syntax(lang);
}

syntax_group *parse_syntax_line(char *line)
{
	char **args = tokenize_string(line," ");
	char **p = args;
	while (*p++);
	if ((p - args) - 1);
		return NULL;
	int index = 0;
	syntax_group *grp = malloc(sizeof(syntax_group *));
	char key[128];
	int keyp = 0;
	while (*args) {
		if (index == 0) {
			grp->group_name = strdup(*args);
		} else if (*args[0] == '[') {
			char *str = *args;
			bool in = false;
			while (*str) {
				if (*str == '"') {
					if (!in) {
						in = true;
						goto next;
					}
					else {
						key[keyp] = '\0';
						add_key_to_syntax_group(grp,key);
						keyp = 0;
					}
				}
				if (in) {
					key[keyp++] = *str;
				}
next:
				str++;
			}
		}
		index++;
		args++;
	}
	return grp;
}

/*
 *	NOTE : dont use strdup for key , we will get a duplicate of key in function :)
 */
void add_key_to_syntax_group(syntax_group *grp,char *key)
{
	syntax_field *sn = malloc(sizeof(syntax_field));
	sn->keyword = strdup(key);
	if (grp->keywords == NULL)
		grp->keywords = sn;
	else {
		grp->lkeyword->next = sn;
	}
	grp->lkeyword = sn;
}

void add_language_syntax(language_syntax *lang)
{
	if (syntax_db == NULL)
		syntax_db = lang;
	// ADD LAST
}

void add_group_for_language(language_syntax *lang,syntax_group *grp)
{
	if (lang->syntax_groups == NULL)
		lang->syntax_groups = grp;
	else {
		grp->next = lang->syntax_groups;
		lang->syntax_groups = grp;
	}
}