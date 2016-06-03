#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFLEN	128

extern int connect_retry(int,int,int,const struct sockaddr*,socklen_t);
void print_family(struct addrinfo* aip)
{
	printf(" family: ");
	switch(aip->ai_family){
		case AF_INET:
			printf("inet");
			break;
		case AF_INET6:
			printf("inet6");
			break;
		case AF_UNIX:
			printf("unix");
			break;
		case AF_UNSPEC:
			printf("unspecified");
			break;
		default:
			printf("unknown");
	}
	printf("\t");
}

void print_type(struct addrinfo* aip)
{
	printf(" type: ");
	switch(aip->ai_socktype){
		case SOCK_STREAM:
			printf("stream");
			break;
		case SOCK_DGRAM:
			printf("datagram");
			break;
		case SOCK_SEQPACKET:
			printf("seqpacket");
			break;
		case SOCK_RAW:
			printf("raw");
			break;
		default:
			printf("unknown (%d)",aip->ai_socktype);
	}
	printf("\t");
}

void print_protocol(struct addrinfo* aip)
{
	printf(" protocol: ");
	switch(aip->ai_protocol){
		case 0:
			printf("default");
			break;
		case IPPROTO_TCP:
			printf("TCP");
			break;
		case IPPROTO_UDP:
			printf("UDP");
			break;
		case IPPROTO_RAW:
			printf("raw");
			break;
		default:
			printf("unknown (%d)",aip->ai_protocol);
	}
	printf("\t");
}

void print_flags(struct addrinfo* aip)
{
	printf("flags: ");
	if(aip->ai_flags == 0){
		printf("0");
	}else {
		if (aip->ai_flags & AI_PASSIVE)
			printf(" passive");
		if (aip->ai_flags & AI_CANONNAME)
			printf(" canon");
		if (aip->ai_flags & AI_NUMERICHOST)
			printf(" numhost");
		if (aip->ai_flags & AI_NUMERICSERV)
			printf(" numserv");
		if (aip->ai_flags & AI_V4MAPPED)
			printf(" v4mapped");
		if (aip->ai_flags & AI_ALL)
			printf(" all");
	}
}

void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];
	while((n = recv(sockfd,buf,BUFLEN,0)) > 0){
		write(STDOUT_FILENO,buf,n);
	}
	if(n < 0){
		err_sys("revc error");
	}
}

int main(int argc,char** argv)
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;
	struct sockaddr_in *sinp;
	char abuf[1000];
	const char* addr;

	if(argc != 2){
		err_quit("usage: %s hostname",argv[0]);
	}
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if((err = getaddrinfo(argv[1],"ruptime",&hint,&ailist)) != 0){
		err_quit("getaddrinfo error: %s",gai_strerror(err));
	}
	for(aip = ailist; aip != NULL; aip = aip->ai_next){
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n\thost %s",aip->ai_canonname ? aip->ai_canonname : "-");
		if(aip->ai_family == AF_INET){
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET,&sinp->sin_addr,abuf,INET_ADDRSTRLEN);
			printf(" address %s",addr ? addr : "unknown");
			printf(" port %d",ntohs(sinp->sin_port));
		}
		printf("\n\n");
		printf("ready to connect\n");
		if((sockfd = connect_retry(aip->ai_family,SOCK_STREAM,0,aip->ai_addr,aip->ai_addrlen)) < 0){
			err = errno;
		}else{
			printf("connect success\n");
			print_uptime(sockfd);
			exit(0);
		}
	}
	err_exit(err,"can't connect to %s",argv[1]);
}


