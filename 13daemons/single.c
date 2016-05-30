#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int lockfile(int fd)
{
	struct flock fk;  
	fk.l_type = F_WRLCK;  
	fk.l_start = 0;  
	fk.l_whence = SEEK_SET;  
	fk.l_len = 0;  
	return fcntl(fd,F_SETLK,&fk);  
}

int already_running(void)
{
	int fd;
	char buf[16];

	fd = open(LOCKFILE,O_RDWR|O_CREAT, LOCKMODE);
	if(fd < 0){
		syslog(LOG_ERR,"can't open %s: %s",LOCKFILE,strerror(errno));
		exit(1);
	}
	if(lockfile(fd) < 0){
		if(errno == EACCES || errno == EAGAIN){
			close(fd);
			return 1;
		}
		syslog(LOG_ERR,"can't lock %s: %s",LOCKFILE,strerror(errno));
		exit(1);
	}
	ftruncate(fd,0);
	sprintf(buf,"%ld",(long)getpid());
	write(fd,buf,strlen(buf)+1);
	return 0;
}
