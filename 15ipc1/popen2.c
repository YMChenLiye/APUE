#include "apue.h"
#include <sys/wait.h>

#define PAGER "/bin/more"

int main(int argc,char** argv)
{
	char line[MAXLINE];
	FILE *fpin,*fpout;

	if(argc != 2){
		err_quit("usage: %s <pathname>",argv[0]);
	}
	if((fpin = fopen(argv[1],"r")) == NULL){
		err_sys("can't open %s",argv[1]);
	}
	if((fpout = popen(PAGER,"w")) == NULL){
		err_sys("popen eror");
	}

	//copy argv[1] to pager
	while(fgets(line,MAXLINE,fpin) != NULL){
		if(fputs(line,fpout) == EOF){
			printf("111\n");
			err_sys("fputs error to pipe");
		}
	}
	if(ferror(fpin)){
		err_sys("fgets error");
	}
	if(fclose(fpout) == -1){
		err_sys("pclose error");
	}

	exit(0);
}
