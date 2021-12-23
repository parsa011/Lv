#include "types.h"

#include <sys/ioctl.h> 
#include <assert.h>

/* Versions of 'read' and 'write' which automatically retry when interrupted */
ssize_t lv_read(int fd, void *buf, size_t size)
{
	ssize_t rt;

	do {
		rt = read(fd, buf, size);
	} while (rt < 0 && errno == EINTR);
	return rt;
}

ssize_t lv_write(int fd, void *buf, size_t size)
{
	ssize_t rt;

	do {
		rt = write(fd, buf, size);
	} while (rt < 0 && errno == EINTR);
	return rt;
}

int lv_ioctl(int fd, int req, void *ptr)
{
	int rt;
	do {
		rt = ioctl(fd, req, ptr);
	} while (rt == -1 && errno == EINTR);
	return rt;
}

/*
 *	original strncpy does not add \0 , so 
 *	ours add :)
 */
void lv_strncpy(char *dst, char *src, int len)
{
	strncpy(dst,src,len);
	if (dst[len] != '\0')
		dst[len] = '\0';
}

int count_tabs(char *string,int len)
{
	int count = 0;
	for (int i = 0;i < len;i++)
		if (string[i] == '\t')
			count++;
	return count;
}

int detab(char *string)
{
	int len = strlen(string) + count_tabs(string,strlen(string)) * tab_size;
	char *res = (char *)malloc(len);			
	int index = 0;
	for (int i = 0;i < strlen(string);i++) {
		if (string[i] == '\t') {
			for (int j = 0;j < tab_size;j++)
				res[index++] = ' ';
		} else 
			res[index++] = string[i];
	}
	string = strdup(res);
	return len;
}

/*
 *	this function is like this question :
 *		if we have x string and y col position in string , so how much is coffset
 */
int convert_cursorcol_to_coffset(char *string, int col)
{
	int coffset = 0;
	for (;col > 1;col--) {
		if (string[coffset] == '\t')
			col -= tab_size - 1;
		coffset++;
	}
	return coffset;
}

int convert_coffset_to_cursorcol(char *string, int offset)
{
	int col = 0;
	for (int i = 0;offset >= i;i++) {
		if (string[i] == '\t' && i != offset) /* if it's last tab , dont add tabsize to col */
			col += tab_size;
		else
			col++;
	}
	return col;
}

void strreverse(char *str)
{
	int n = strlen(str);
	for (int i = 0; i < n / 2; i++) {
		char temp = str[i];
		str[i] = str[n - i - 1];
		str[n - i - 1] = temp;
	}
}

int get_filename_of_path(char *buf,char *path)
{
	char *p = path;
	char *ch;
	ch = strtok(p, "/");
	while (ch != NULL) {
		strcpy(buf, ch);
		ch = strtok(NULL, "/");
	}
}

// do shift write for buf string , start from end and end in start_index 
void shift_right(char *buf,int buflen,int start_index)
{
	for (int i = buflen - 1;i > start_index;i--) {
		buf[i] = buf[i - 1];
		buf[i - 1] = ' ';
	}
}

void shift_left(char *buf,int buflen,int start_index)
{
	for (;start_index < buflen - 1;start_index++) {
		buf[start_index] = buf[start_index + 1];
		buf[start_index + 1] = ' ';
	}
}

/*
 * change a key command to a string we can print out
 *
 * int c;		sequence to translate
 * char *seq;		destination string for sequence
 */
void cmdstr(int c, char *seq)
{
	char *ptr;		/* pointer into current position in sequence */

	ptr = seq;

	/* apply meta sequence if needed */
	if (c & META) {
		*ptr++ = 'M';
		*ptr++ = '-';
	}

	/* apply ^X sequence if needed */
	if (c & CTLX) {
		*ptr++ = '^';
		*ptr++ = 'X';
	}

	/* apply SPEC sequence if needed */
	if (c & SPEC) {
		*ptr++ = 'F';
		*ptr++ = 'N';
	}

	/* apply control sequence if needed */
	if (c & CONTROL) {
		*ptr++ = '^';
	}

	/* and output the final sequence */

	*ptr++ = c & 255;	/* strip the prefixes */

	*ptr = 0;		/* terminate the string */
}

char twin_chars[] = {
	'"','"',
	'\'','\'',
	'(',')',
	'[',']',
	'{','}'
};

int get_twin_char_index(int c)
{
	for (int i = 0;i < ARRAY_LENGTH(twin_chars);i++)
		if (twin_chars[i] == c)
			return i;
	return -1;
}

char **tokenize_string(char *string, const char c)
{
	char **result = 0;
	size_t count = 0;
	char *tmp = string;
	char *last_comma = 0;
	// convert to char array to user in strtok
	char delim[2];
	delim[0] = c;
	delim[1] = 0;
	/* count how many elements will be extracted. */
	while (*tmp) {
		if (c == *tmp) {
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* add space for trailing token. */
	count += last_comma < (string + strlen(string) - 1);

	/* add space for terminating null string so caller
	   knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result) {
		size_t idx  = 0;
		char* token = strtok(string, delim);
		while (token) {
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}
	return result;
}
