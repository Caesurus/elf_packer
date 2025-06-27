#include <sys/types.h>
#include <stdarg.h>

#include "z_syscalls.h"

static int lastfd = -1;
#define OUTBUFSIZE 128
static char outbuf[OUTBUFSIZE];
static char *outptr;

static void kprintn(int, unsigned long, int);
static void kdoprnt(int, const char *, va_list);
static void z_flushbuf(void);

static void putcharfd(int, int);

static void
putcharfd(int c, int fd)
{
	char b = c;
	int len;

	if (fd != lastfd)
	{
		z_flushbuf();
		lastfd = fd;
	}
	*outptr++ = b;
	len = outptr - outbuf;
	if ((len >= OUTBUFSIZE) || (b == '\n') || (b == '\r'))
	{
		z_flushbuf();
	}
}

static void
z_flushbuf()
{
	int len = outptr - outbuf;
	if (len != 0)
	{
		if (lastfd != -1)
			z_write(lastfd, outbuf, len);
		outptr = outbuf;
	}
}

void z_printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	kdoprnt(2, fmt, ap);
	va_end(ap);
}

void z_vprintf(const char *fmt, va_list ap)
{
	kdoprnt(2, fmt, ap);
}

void z_fdprintf(int fd, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	kdoprnt(fd, fmt, ap);
	va_end(ap);
}

void z_vfdprintf(int fd, const char *fmt, va_list ap)
{
	kdoprnt(fd, fmt, ap);
}

static void
kdoprnt(int fd, const char *fmt, va_list ap)
{
	unsigned long ul;
	int lflag, ch;
	char *p;
	static int init;

	if (!init)
	{
		outptr = outbuf;
		init = 1;
	}

	for (;;)
	{
		while ((ch = *fmt++) != '%')
		{
			if (ch == '\0')
				return;
			putcharfd(ch, fd);
		}
		lflag = 0;
	reswitch:
		switch (ch = *fmt++)
		{
		case 'l':
			lflag = 1;
			goto reswitch;
		case 'c':
			ch = va_arg(ap, int);
			putcharfd(ch & 0x7f, fd);
			break;
		case 's':
			p = va_arg(ap, char *);
			while ((ch = *p++))
				putcharfd(ch, fd);
			break;
		case 'd':
			ul = lflag ? va_arg(ap, long) : va_arg(ap, int);
			if ((long)ul < 0)
			{
				putcharfd('-', fd);
				ul = -(long)ul;
			}
			kprintn(fd, ul, 10);
			break;
		case 'o':
			ul = lflag ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
			kprintn(fd, ul, 8);
			break;
		case 'u':
			ul = lflag ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
			kprintn(fd, ul, 10);
			break;
		case 'p':
			putcharfd('0', fd);
			putcharfd('x', fd);
			lflag += sizeof(void *) == sizeof(unsigned long) ? 1 : 0;
			/* FALLTHRU */
		case 'x':
			ul = lflag ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
			kprintn(fd, ul, 16);
			break;
		case 'X':
		{
			int l;

			ul = lflag ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
			if (lflag)
				l = (sizeof(unsigned long) * 8) - 4;
			else
				l = (sizeof(unsigned int) * 8) - 4;
			while (l >= 0)
			{
				putcharfd("0123456789abcdef"[(ul >> l) & 0xf], fd);
				l -= 4;
			}
			break;
		}
		default:
			putcharfd('%', fd);
			if (lflag)
				putcharfd('l', fd);
			putcharfd(ch, fd);
		}
	}
	z_flushbuf();
}

static void
kprintn(int fd, unsigned long ul, int base)
{
	// rewrote to avoid div
	char buf[(sizeof(long) * 8 / 3) + 1], *p = buf;
	const char *digits = "0123456789abcdef";
	if (ul == 0)
	{
		*p++ = '0';
	}
	else
	{
		while (ul)
		{
			unsigned long q = 0, x = ul, d = base, shift = 1;
			// scale divisor up
			while ((d << 1) > d && (d << 1) <= x)
			{
				d <<= 1;
				shift <<= 1;
			}
			// long division by shifts/subtractions
			while (shift)
			{
				if (x >= d)
				{
					x -= d;
					q += shift;
				}
				d >>= 1;
				shift >>= 1;
			}
			*p++ = digits[x]; // x is remainder
			ul = q;
		}
	}

	do
	{
		putcharfd(*--p, fd);
	} while (p > buf);
}
