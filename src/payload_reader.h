#ifndef PAYLOAD_READER_H
#define PAYLOAD_READER_H

#include <stddef.h>
#include <sys/types.h>

#define INTERNAL_PAYLOAD_FD 999

extern const char internal_payload_name[];
void pl_decrypt(char **env);

int pl_open(const char *path, int flags);
ssize_t pl_read(int fd, void *buf, size_t count);
off_t pl_lseek(int fd, off_t offset, int whence);
int pl_close(int fd);

#endif