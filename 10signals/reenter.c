//#include "apue.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>

static void my_alarm(int signo)
{
	struct passwd* rootptr;
	printf("in signal handler\n");
	if((rootptr = getpwnam("root")) == NULL){
//		err_sys("getpwnam(root) error");
	}
	alarm(2);
}

int main(void)
{
	struct passwd* ptr;
	signal(SIGALRM,my_alarm);
	alarm(1);
	signal(SIGALRM,my_alarm);
	for(;;){
		if((ptr = getpwnam("cly")) == NULL){
//			err_sys("getpwnam error");
		}
		if(strcmp(ptr->pw_name,"cly") != 0){
			printf("return value corrupted!,pw_name = %s\n",ptr->pw_name);
		}
	}
}
