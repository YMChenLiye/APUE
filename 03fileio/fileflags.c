#include "apue.h"
#include "fcntl.h"

int main(int argc,char* argv[])
{
	int val;

	if(argc != 2){
		err_quit("usage: %s <descriptor#>",argv[0]);
	}
	if((val = fcntl(atoi(argv[1]),F_GETFL,0)) < 0){
		err_sys("fcntl error for fd %d",atoi(argv[1]));
	}
	printf("O_ACCMODE:%x\n",O_ACCMODE);
	printf("O_RDONLY:%x,O_WRONLY:%x,O_RDWR:%x\n",O_RDONLY,O_WRONLY,O_RDWR);
	switch(val & O_ACCMODE){
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;

		default:
			err_dump("unknown access mode");
	}
	printf("\nval:%x,O_APPEND:%x,O_NONBLOCK:%x,O_SYNC:%x\n",val,O_APPEND,O_NONBLOCK,O_SYNC);
	if(val & O_APPEND){
		printf(", append");
	}
	if(val & O_NONBLOCK){
		printf(", noblocking");
	}
	if(val & O_SYNC){
		printf(", synchronous writes");
	}

#if ! defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
	if(val & O_FSYNC){
		printf(", synchronous writes");
	}
#endif

	putchar('\n');
	exit(0);
}
