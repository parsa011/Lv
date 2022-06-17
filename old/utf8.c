#include "types.h"

unicode_t utf8_to_unicode(char *line, unsigned index, unsigned len, unicode_t *res)
{
	unsigned value;
	unsigned char c = line[index];
	unsigned bytes, mask, i;

	*res = c;
	line += index;
	len -= index;

	/*
	 * 0xxxxxxx is valid utf8
	 * 10xxxxxx is invalid UTF-8, we assume it is Latin1
	 */
	if (c < 0xc0)
		return 1;

	/* ok, it's 11xxxxxx, do a stupid decode */
	mask = 0x20;
	bytes = 2;
	while (c & mask) {
		bytes++;
		mask >>= 1;
	}

	/* invalid? Do it as a single byte Latin1 */
	if (bytes > 6)
		return 1;
	if (bytes > len)
		return 1;

	value = c & (mask-1);

	/* ok, do the bytes */
	for (i = 1; i < bytes; i++) {
		c = line[i];
		if ((c & 0xc0) != 0x80)
			return 1;
		value = (value << 6) | (c & 0x3f);
	}
	*res = value;
	return bytes;
}

unicode_t unicode_to_utf8(unsigned int c, char *utf8)
{
	int bytes = 1;

	*utf8 = c;
	if (c > 0x7f) {
		int prefix = 0x40;
		char *p = utf8;
		do {
			*p++ = 0x80 + (c & 0x3f);
			bytes++;
			prefix >>= 1;
			c >>= 6;
		} while (c > prefix);
		*p = c - 2*prefix;
		reverse_string(utf8, p);
	}
	return bytes;
}

void reverse_string(char *begin, char *end)
{
	do {
		char a = *begin, b = *end;
		*end = a; *begin = b;
		begin++; end--;
	} while (begin < end);
}
