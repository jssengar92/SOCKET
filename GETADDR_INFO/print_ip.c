#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int status, set = 1;
	char ip[INET_ADDRSTRLEN];

	memset(&hints,'\0',sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	
	if (argc < 2) {
		printf("Insufficient Arguments\n");
		return -1;
	}

	if ((status = getaddrinfo(argv[1],NULL, &hints,&res)) != 0) {
		printf("Error:getaddrinfo %d is %s\n",status,gai_strerror(status));
		return -1;
	}
	printf("IP Address for %s\n",argv[1]);
	
	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;
		if (p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		inet_ntop(p->ai_family,addr,ip,sizeof(ip));
		printf("%s : %s\n",ipver,ip);
	}
	freeaddrinfo(res);	
return 0;
}
