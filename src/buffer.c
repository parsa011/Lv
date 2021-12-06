#include "types.h"

buffer *init_buffer(char *filename, char *buffername,char modes)
{
	buffer *bf;
	if (!(bf = malloc(sizeof(buffer))))
		die("malloc buffer");

	if (strlen(buffername) < NBUFN)
		memcpy(bf->bname,buffername,strlen(buffername));
	if (strlen(filename) < NFILEN)
		memcpy(bf->fname,filename,strlen(filename));
	bf->mode = modes;
}
