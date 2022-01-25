#ifndef _UTF8_H
# define _UTF8_H

typedef unsigned int unicode_t;

/*
 * utf8_to_unicode()
 *
 * convert a UTF-8 sequence to its unicode value, and return the length of
 * the sequence in bytes.
 *
 * NOTE! Invalid UTF-8 will be converted to a one-byte sequence, so you can
 * either use it as-is (ie as Latin1) or you can check for invalid UTF-8
 * by checking for a length of 1 and a result > 127.
 *
 * NOTE 2! This does *not* verify things like minimality. So overlong forms
 * are happily accepted and decoded, as are the various "invalid values".
 */
unicode_t utf8_to_unicode(char *line, unsigned index, unsigned len, unicode_t *res);

/*
 * unicode_to_utf8()
 *
 * convert a unicode value to its canonical utf-8 sequence.
 *
 * NOTE! This does not check for - or care about - the "invalid" unicode
 * values.  Also, converting a utf-8 sequence to unicode and back does
 * *not* guarantee the same sequence, since this generates the shortest
 * possible sequence, while utf8_to_unicode() accepts both Latin1 and
 * overlong utf-8 sequences.
 */
unicode_t unicode_to_utf8(unsigned int c, char *utf8);

void reverse_string(char *,char *);	

static inline int is_beginning_utf8(unsigned char c)
{
	return (c & 0xc0) != 0x80;
}

#endif 
