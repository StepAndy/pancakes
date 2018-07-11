#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	const char *ip = "127.0.0.1";
	
	char *sendbuf = "I<3U"; // Отправляемое сообщ.
	char recievebuf[100];			//Буфер для приема сообщ.

	////////////////////////////////////////////////
	int servaddr_len = sizeof(servaddr);
	int cliaddr_len  = sizeof(cliaddr);
	int sendbuf_len    = 5;
	int recievebuf_len = sizeof(recievebuf);
	int sockadrr_in_sz = sizeof(struct sockaddr_in);
	////////////////////////////////////////////////

	sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(22000); 

	while(1){
		sendto(sockfd,sendbuf,sendbuf_len, 0, (struct sockaddr*)&servaddr, servaddr_len);
		if(strcmp(sendbuf,"exit"))
			break;
		if(recvfrom(sockfd, recievebuf, recievebuf_len, 0,(struct sockaddr*)&servaddr, &servaddr_len)==-1)
			break;
		printf("%s\n", recievebuf);	
	}

	close(sockfd);
	return 0;
}

//Чтобы передавать, нужно endpoint зафиксировать.
//перед bind нужно sockaddr_in заполнить
//intel - little indian (!big indian)
//htons host to net - часто приводить 