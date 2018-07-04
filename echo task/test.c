#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#define INADDR_ANY_INIT {{ 0,0,0,0 }}

int main(int argc, char const *argv[])
{	
	struct sockaddr_in addr;
	int msg[5];
	printf("%lu\n",sizeof(msg));
	return 0;
}

//Чтобы передавать, нужно endpoint зафиксировать.
//перед bind нужно sockaddr_in заполнить
//intel - little indian (!big indian)
//htons host to net - часто приводить 