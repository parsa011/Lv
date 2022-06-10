#include "lv.h"

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

int main(int argc, char *argv[])
{
	
	usage(argv[0]);
	return 0;
}
