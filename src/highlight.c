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

char *props_info_table[] = {
	"RED",RED,
	"BLUE",BLUE,
	"YELLOW",YELLOW,
	"CYAN",CYAN,
	"INVERT",INVERT,
	"DEFAULT",DEFAULT,
	"GRAY",GRAY,
	"NORMAL",NORMAL,
	"GREEN",GREEN,
	"MAGENTA",MAGENTA,
	"BOLD",BOLD,
	"ITALIC",ITALIC,
	"BLINK",BLINK,
	"UNDERLINE",UNDLINE,
	"DIM",DIM,
	"OVERLINE",OVLINE
};

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
		return;
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

language_syntax *get_language_syntax_by_name(char *s)
{
	for (language_syntax *ls = syntax_db; ls != NULL; ls = ls->next)
		if (strcmp(ls->lang,s) == 0)
			return ls;
	return NULL;
}

syntax_group *parse_syntax_line(char *line)
{
	char **args = tokenize_string(line," ");
	char **p = args;
	int len = 0;
	while (*p++)
		len++;
	if (len < 4)
		return NULL;
	syntax_group *grp = malloc(sizeof(syntax_group *));
	char key[128];
	int index = 0,keyp = 0;
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
						in = false;
					}
				}
				if (in) {
					key[keyp++] = *str;
				}
next:
				str++;
			}
		} 
		if (index > 2) {
			grp->props[index - 3] = strdup(*args);
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
		sn->next = grp->keywords;
		grp->keywords = sn;
	}
}

void add_language_syntax(language_syntax *lang)
{
	if (syntax_db == NULL)
		syntax_db = lang;
	else {
		lang->next = syntax_db;
		syntax_db = lang;
	}
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

/*
 *	NOTE : default language is current buffer filetype 
 *	we will return syntax_groups props
 */
char **get_syntax_for_keyword(char *key)
{
	if (curbp->filetype == NULL)
		return NULL;
	language_syntax *ls = get_language_syntax_by_name(curbp->filetype);
	if (ls == NULL) {
		//showmsg(true,"syntax for %s lang not founded",curbp->filetype);
		return NULL;
	}
	for (syntax_group *sg = ls->syntax_groups; sg != NULL; sg = sg->next) {
		for (syntax_field *sf = sg->keywords; sf != NULL; sf = sf->next) 
			if (strcmp(sf->keyword,key) == 0)
				return sg->props;
	}
	return NULL;
}

/*
 *	return the sequence for given prop to write in output 
 *	it can be color or bold or italic or ...
 *	TODO : enhance this function
 */
char *prop_to_str(char *prop)
{
	for (int i = 0;i < ARRAY_LENGTH(props_info_table); i += 2) {
		if (strncmp(props_info_table[i],prop,strlen(prop) - 1) == 0) {
			return props_info_table[i + 1];
		}
	}
	return DEFAULT;
}
