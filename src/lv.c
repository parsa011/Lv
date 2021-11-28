#include "types.h"

#define CTRL_KEY(k) ((k) & 0x1f)
#define CONTROL 0x1f

int main(int argc,char *argv[])
{
	ttopen();
	int c = 0;
	while (1) {
		c = ttgetc();
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;
	}
	return 0;
}
