#ifndef _UTILS_H
# define _UTILS_H

/* Versions of 'read' and 'write' which automatically retry when interrupted */
ssize_t lv_read(int fd, void *buf, size_t siz);
ssize_t lv_write(int fd, void *buf, size_t siz);
int lv_ioctl(int fd, int req, void *ptr);
void *lv_malloc(size_t);
void *lv_realloc(void *, size_t);
void *lv_calloc(size_t, size_t);

void lv_strncpy(char *, char *, int);

int count_tabs(char *, int);
/*
 *	convert tabs to spaces and return new length of string
 */
int detab(char *);

int convert_cursorcol_to_coffset(char *, int);
int convert_coffset_to_cursorcol(char *, int);

void strreverse(char *);
int get_filename_of_path(char *, char *);

// keys stuff
void convert_key_to_string();
int convert_string_to_key(char);

void shift_right(char*, int, int);
void shift_left(char*, int, int);

/*
 *	convert a key to printable char
 */
void cmdstr(int,char *);

extern char twin_chars[];
int get_twin_char_index(int);

/*
 *	split char by given delim
 */
char **tokenize_string(char *, char *);

/*
 *	split char by alpha letters 
 */
char **tokenize_string_byalpha(char *);

bool string_endwith(char *, char *);

/*
 *	return given number len (numbers of digits in number :D)
 */
int number_len(int);

/*
 *	fill given string with given char
 */
void repeat_char(char *, char, int);

#endif
