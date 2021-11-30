#include "types.h"

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
