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

void *lv_malloc(size_t size) 
{
	void *p = malloc(size);
	if (!p)
		die("can allocate memory , size : %d\n",size);
	return p;
}

void *lv_realloc(void *ptr, size_t size) 
{
	void *p = realloc(ptr, size);
	if (!p)
		die("can realloc , size : %d\n",size);
	return p;
}

void *lv_calloc(size_t n, size_t size) 
{
	void *p = calloc(n, size);
	if (!p)
		die("can calloc , size : %d\n",size);
	return p;
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

int count_tabs(char *string, int len)
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

int get_filename_of_path(char *buf, char *path)
{
	char *p = path;
	char *ch;
	ch = strtok(p, "/");
	while (ch != NULL) {
		strcpy(buf, ch);
		ch = strtok(NULL, "/");
	}
	return strlen(buf);
}

// do shift write for buf string , start from end and end in start_index 
void shift_right(char *buf, int buflen, int start_index)
{
	for (int i = buflen - 1;i > start_index;i--) {
		buf[i] = buf[i - 1];
		buf[i - 1] = ' ';
	}
}

void shift_left(char *buf, int buflen, int start_index)
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

char **tokenize_string(char *string, char *delim)
{
#define BUFSIZE 5
#define ELEMSIZE 20
	int delim_len = strlen(delim);
	int buf_size = BUFSIZE;
	char **buf = (char **)calloc(buf_size, sizeof(char *));
	int bufp = 0;
	int elem_size = ELEMSIZE;
	char *elem = (char *)malloc(sizeof(char *));
	int elemp = 0;
	int str_len = strlen(string);
	for (int i = 0; i < str_len; i++) {
		if (bufp == buf_size - 1) {
			buf_size += BUFSIZE;
			buf = (char **) realloc(buf, buf_size * sizeof(char *));
		}
		if (elemp == elem_size - 1) {
			elem_size += ELEMSIZE;
			elem = (char *) realloc(elem, elem_size);
		}
		if (string[i] == delim[0]) {
			bool is_delim = true;
			int j, k;
			for (k = i, j = 0; j < delim_len; j++, k++) {
				if (string[k] == delim[j])
					is_delim = true;
				else {
					is_delim = false;
					break;
				}
			}
			if (is_delim) {
				if (elemp == 0) {
					goto go_next;
				}
append:
				elem[elemp] = 0;
				buf[bufp++] = strdup(elem);
				elemp = 0;
				elem_size = ELEMSIZE;
go_next:
				i += delim_len - 1;
				continue;
			}
		}
		*(elem + elemp++) = string[i];
		if (i == str_len - 1)
			goto append;
	}
	buf[bufp] = 0;
#undef BUFSIZE
#undef ELEMSIZE
    free(elem);
	return buf;
}

char **tokenize_string_byalpha(char *string)
{
#define BUFSIZE 5
#define ELEMSIZE 20
	int buf_size = BUFSIZE;
	char **buf = (char **) calloc(buf_size, sizeof(char *));
	int bufp = 0;
	int elem_size = ELEMSIZE;
	char *elem = (char *) malloc(sizeof(char *));
	int elemp = 0;
	int str_len = strlen(string);
	bool in_word = false;
	for (int i = 0; i < str_len; i++) {
		if (isalpha(string[i]))
			in_word = true;
		if (bufp == buf_size - 1) {
			buf_size += BUFSIZE;
			buf = (char **) realloc(buf, buf_size * sizeof(char *));
		}
		if (elemp == elem_size - 1) {
			elem_size += ELEMSIZE;
			elem = (char *) realloc(elem, elem_size);
		}
		if (!isalpha(string[i])) {
			if (in_word) {
				elem[elemp] = 0;
				buf[bufp++] = strdup(elem);
				in_word = false;
			}

			if (!isspace(string[i])) {
				elem[0] = string[i];
				elem[1] = 0;
				buf[bufp++] = strdup(elem);
			}

			elemp = 0;
			elem_size = ELEMSIZE;
			continue;
		}
		*(elem + elemp++) = string[i];
		if (i == str_len - 1) {
			elem[elemp] = 0;
			buf[bufp++] = strdup(elem);
		}
	}
	buf[bufp] = 0;
#undef BUFSIZE
#undef ELEMSIZE
	free(elem);
	return buf;
}

bool string_endwith(char *str, char *t)
{
	char *bs = str;
	char *bt = t;
	char *ptr = str;
	while (*ptr)
		ptr++;
	while (*t)
		t++;
	for (; *ptr == *t; ptr--,t--)
		if (t == bt || ptr == bs)
			break;
	if (*ptr == *t && *t == *bt && *ptr != '\0')
		return true;
	return false;
}

/*
 *	repeat specified char in s by n times
 */
void repeat_char(char *s,char c,int times)
{
	memset(s, c, times);
}

/*
 *	how many digit this number have ?
 */
int number_len(int n)
{
	if (n < 0)
		n = -n;
	int len = 1;
	while (n / 10) {
		n /= 10;
		len++;
	}
	return len;
}
