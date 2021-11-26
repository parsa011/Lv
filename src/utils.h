#ifndef _UTILS_H
# define _UTILS_H

/* Versions of 'read' and 'write' which automatically retry when interrupted */
ssize_t lv_read(int fd, void *buf, size_t siz);
ssize_t lv_write(int fd, void *buf, size_t siz);
int lv_ioctl(int fd, int req, void *ptr);

#endif
