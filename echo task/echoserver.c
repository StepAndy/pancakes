#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>

#define INADDR_ANY_INIT {{ 0,0,0,0 }}

int main(int argc, char const *argv[])
{
	int sockfd;
	struct sockaddr_in addr;
	const struct in_addr in_addr_any = INADDR_ANY_INIT;
	const char *ip = "127.0.0.1";
	char recievebuf[100];
	
	memset(&addr, 0, sizeof(sturct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(ip);
	addr.sin_port = htons(22000); 

	sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	bind(sockfd,(struct sockaddr_in *)&addr,sizeof(addr));

	recvfrom(sockfd, recievebuf, sizoef(recievebuf), 0,
				addr, sizeof(addr));

	sendto(sockfd,recievebuf,sizoef(recievebuf), 0,			//[not filled]
				addr, sizeof(addr));
	close(sockfd);
	return 0;
}

//Чтобы передавать, нужно endpoint зафиксировать.
//перед bind нужно sockaddr_in заполнить
//intel - little indian (!big indian)
//htons host to net - часто приводить 