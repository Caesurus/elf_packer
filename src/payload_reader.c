#include "payload_reader.h"
#include "payload.h"
#include "z_syscalls.h"
#include "z_utils.h"
#include "aes.h"

static off_t payload_pos = 0;
const char internal_payload_name[] = "internal_payload";

static unsigned char decrypted_payload[sizeof(payload_enc)];
static int payload_decrypted = 0;

void pl_decrypt(char **env)
{
	if (payload_decrypted)
		return;

	const char *key_hex = z_getenv_from(env, "AES_KEY");
	const char *iv_hex = z_getenv_from(env, "AES_IV");

	if (!key_hex || !iv_hex)
		z_errx(1, "AES_KEY and AES_IV must be set");

	if (z_strlen(key_hex) != 32 || z_strlen(iv_hex) != 32)
		z_errx(1, "AES_KEY and AES_IV must be 32 hex characters each");

	unsigned char key[16], iv[16];
	for (int i = 0; i < 16; i++)
	{
		char byte_k[3] = {key_hex[i * 2], key_hex[i * 2 + 1], 0};
		char byte_iv[3] = {iv_hex[i * 2], iv_hex[i * 2 + 1], 0};
		key[i] = (unsigned char)z_strtoul(byte_k, NULL, 16);
		iv[i] = (unsigned char)z_strtoul(byte_iv, NULL, 16);
	}

	z_memcpy(decrypted_payload, payload_enc, payload_enc_len);

	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);

	for (size_t i = 0; i + 16 <= payload_enc_len; i += 16)
		AES_CBC_decrypt_buffer(&ctx, decrypted_payload + i, 16);

	payload_decrypted = 1;
}


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
		if (!payload_decrypted)
		{
			z_printf("payload needs to be decrypted before access\n");
			z_exit(2);
		}

		if ((size_t)payload_pos >= payload_enc_len)
			return 0;
		size_t left = payload_enc_len - payload_pos;
		size_t to_read = count < left ? count : left;
		z_memcpy(buf, decrypted_payload + payload_pos, to_read);
		payload_pos += to_read;
		return to_read;
	}
	return z_read(fd, buf, count);
}

off_t pl_lseek(int fd, off_t offset, int whence)
{
	if (fd == INTERNAL_PAYLOAD_FD)
	{
		if (!payload_decrypted)
		{
			z_printf("payload needs to be decrypted before access\n");
			z_exit(2);
		}

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
			new_pos = payload_enc_len + offset;
			break;
		}
		if (new_pos < 0 || (size_t)new_pos > payload_enc_len)
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
