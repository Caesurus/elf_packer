#include <stdlib.h>
#include "z_utils.h"

size_t z_strlen(const char *s) __attribute__((noinline));
size_t z_strlen(const char *s)
{
	size_t len = 0;
	while (s[len])
		len++;
	return len;
}

void *z_memset(void *s, int c, size_t n)
{
	unsigned char *p = s, *e = p + n;
	while (p < e)
		*p++ = c;
	return s;
}

void *z_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *p = src, *e = p + n;
	while (p < e)
		*d++ = *p++;
	return dest;
}

int z_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = s1, *p2 = s2;
	while (n--)
	{
		if (*p1 != *p2)
			return *p1 - *p2;
		p1++;
		p2++;
	}
	return 0;
}

void z_print_env(char **env)
{
	while (*env)
	{
		z_printf("%s\n", *env);
		env++;
	}
}

const char *z_getenv_from(char **env, const char *key)
{
	if (!env || !key)
		return NULL;

	size_t keylen = z_strlen(key);
	for (; *env != NULL; env++)
	{
		if (z_memcmp(*env, key, keylen) == 0 && (*env)[keylen] == '=')
			return *env + keylen + 1;
	}
	return NULL;
}

unsigned long z_strtoul(const char *nptr, char **endptr, int base)
{
	unsigned long result = 0;
	const char *p = nptr;

	while (*p == ' ' || *p == '\t')
		p++;

	int digit;
	while (1)
	{
		if (*p >= '0' && *p <= '9')
			digit = *p - '0';
		else if (*p >= 'a' && *p <= 'f')
			digit = *p - 'a' + 10;
		else if (*p >= 'A' && *p <= 'F')
			digit = *p - 'A' + 10;
		else
			break;

		if (digit >= base)
			break;

		result = result * base + digit;
		p++;
	}

	if (endptr)
		*endptr = (char *)p;

	return result;
}
