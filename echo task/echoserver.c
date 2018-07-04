#include ".h"
#define INADDR_ANY_INIT {{ 0,0,0,0 }}

int main(int argc, char const *argv[])
{
	int sockfd;
	struct sockaddr_in addr;
	const struct in_addr in_addr_any = INADDR_ANY_INIT;
	
	memset(&addr, 0, sizeof(sturct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(in_addr_any);
	addr.sin_port = htons(22000); //addr.sin_port = 0;

	sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	bind(sockfd,(struct sockaddr_in *)&addr,sizeof(addr));
	return 0;
}

//Чтобы передавать, нужно endpoint зафиксировать.
//перед bind нужно sockaddr_in заполнить
//intel - little indian (!big indian)
//htons host to net - часто приводить 