/*
 *	Highlight service for editor :)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

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
	char *line = NULL;
	size_t len;
	ssize_t read;
	while ((read = getline(&line,&len,fp)) != -1) {
		if (line[0] == '#')
			continue;
		char **args = tokenize_string(line," "); 
		while (*args) {
			fprintf(stderr,"%s\n\r",*args++);
		}
	}
	die("SDF");
}
