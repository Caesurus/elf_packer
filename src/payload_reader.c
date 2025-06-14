#include "payload_reader.h"
#include "payload.h"
#include "z_syscalls.h"
#include "z_utils.h"

static off_t payload_pos = 0;
const char internal_payload_name[] = "internal_payload";

int pl_open(const char *path, int flags)
{
	size_t len = sizeof(internal_payload_name) - 1;

	if (z_memcmp(path, internal_payload_name, len) == 0 && path[len] == '\0')
	{
		payload_pos = 0;
		return INTERNAL_PAYLOAD_FD;
	}

	return z_open(path, flags);
}

ssize_t pl_read(int fd, void *buf, size_t count)
{
	if (fd == INTERNAL_PAYLOAD_FD)
	{
		if ((size_t)payload_pos >= payload_len)
			return 0;
		size_t left = payload_len - payload_pos;
		size_t to_read = count < left ? count : left;
		z_memcpy(buf, payload + payload_pos, to_read);
		payload_pos += to_read;
		return to_read;
	}
	return z_read(fd, buf, count);
}

off_t pl_lseek(int fd, off_t offset, int whence)
{
	if (fd == INTERNAL_PAYLOAD_FD)
	{
		off_t new_pos = -1;
		switch (whence)
		{
		case SEEK_SET:
			new_pos = offset;
			break;
		case SEEK_CUR:
			new_pos = payload_pos + offset;
			break;
		case SEEK_END:
			new_pos = payload_len + offset;
			break;
		}
		if (new_pos < 0 || (size_t)new_pos > payload_len)
			return -1;
		payload_pos = new_pos;
		return new_pos;
	}
	return z_lseek(fd, offset, whence);
}

int pl_close(int fd)
{
	if (fd == INTERNAL_PAYLOAD_FD)
		return 0;
	return z_close(fd);
}
