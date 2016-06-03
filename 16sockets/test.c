#include "apue.h"

int main(int argc,char** argv)
{
	char* host = (char*)malloc(256);
	gethostname(host,256);

	printf("%s\n",host);

	exit(0);
}
