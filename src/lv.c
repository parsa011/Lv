#include "types.h"

int main(int argc,char *argv[])
{
	ttopen();
	int c;
	char ch;
	while (1) {
		c = read(STDIN_FILENO,&ch,1);
		printf("%c\n",ch);
		if (ch == 'q')
			break;
	}
	printl("WORKS !!!!!");
	return 0;
}
