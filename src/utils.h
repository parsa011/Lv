#ifndef _UTILS_H
# define _UTILS_H

/* Versions of 'read' and 'write' which automatically retry when interrupted */
ssize_t lv_read(int fd, void *buf, size_t siz);
ssize_t lv_write(int fd, void *buf, size_t siz);
int lv_ioctl(int fd, int req, void *ptr);

// keys stuff
void convert_key_to_string();
int convert_string_to_key(char);

#endif
