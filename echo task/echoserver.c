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
	const char *ip = "127.0.0.1";

	struct sockaddr_in servaddr, cliaddr;
	char recievebuf[100];

	////////////////////////////////////////////////
	int servaddr_len = sizeof(servaddr);
	int cliaddr_len  = sizeof(cliaddr);
	int recievebuf_len = 5;
	int sockadrr_in_sz = sizeof(struct sockaddr_in);
	////////////////////////////////////////////////
	
	sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(22000); 

	bind(sockfd,(struct sockaddr *)&servaddr,servaddr_len);

	/* Получить сообщение от клиента */
	while(1){
		if(recvfrom(sockfd, recievebuf, recievebuf_len, 0,	(struct sockaddr *)&cliaddr, &cliaddr_len)==-1)
			break;
		if(!strcmp(recievebuf,"exit"))
			break;
		printf("%s\n",recievebuf);
	/*	Изменю и отправлю клиенту полученное сообщение */
		recievebuf[1]='Z';
		sendto(sockfd,recievebuf,recievebuf_len, 0, (struct sockaddr *)&cliaddr, cliaddr_len);	
	}

	close(sockfd);
	return 0;
}

//Чтобы передавать, нужно endpoint зафиксировать.
//перед bind нужно sockaddr_in заполнить
//intel - little indian (!big indian)
//htons host to net - часто приводить 