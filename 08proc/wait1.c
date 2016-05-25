#include "apue.h"
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status;
	if((pid = fork()) < 0){
		err_sys("fork error");
	}else if(pid == 0){		//child
		exit(7);
	}

	if(wait(&status) != pid){	//wait for child
		err_sys("wait error");
	}
	pr_exit(status);			//and print its status

	if((pid = fork()) < 0){
		err_sys("fork error");
	}else if(pid == 0){			//child
		abort();				//generates SIGABRT
	}

	if(wait(&status) != pid){
		err_sys("wait error");
	}
	pr_exit(status);

	if((pid = fork()) < 0){
		err_sys("fork error");
	}else if(pid == 0){		//child
		status /= 0;			//divide by 0 generaltes SIGFPE
	}

	if(wait(&status) != pid){
		err_sys("wait error");
	}
	pr_exit(status);
	
	exit(0);
}
