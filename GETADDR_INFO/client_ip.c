#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>	/* for getaddrinfo */
#include<netinet/in.h>	/* for inet_ntop */

#define PORT 9000

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;	/* structure for getaddrinfo */
	int status, sfd, bnd, cnt, set = 1, recv_len;	
	char ip[INET_ADDRSTRLEN];		/* INET_ADDRSTRLEN = 16*/
	char buffer[1024];
	memset(&hints,'\0',sizeof(hints));
	hints.ai_family = AF_UNSPEC;		/*For both IPv4 and IPv6*/
	hints.ai_flags = AI_PASSIVE;		
	hints.ai_socktype = SOCK_STREAM;
	
	if (argc < 2) {
		printf("Insufficient Arguments\n");
		return -1;
	}
	/*
	 * getaddrinfo combined the functionality of both gethostbyname and getservicebyname
	 * interaddress returned can be used for both bind and connect.
	 * */
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
		/* To create a socket */
		sfd  = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if (sfd == -1) {
			perror("socket");
			return -1;
		}
		printf("Socket FD = %d\n",sfd);
		/* To connect to the server */
		cnt = connect(sfd,res->ai_addr,res->ai_addrlen);
		if (cnt == -1) {
			printf("Error:connect: %d is %s\n",cnt,strerror(errno));
		//	return -1;
		}

		/* To print in human readable form network to presentation */
		inet_ntop(p->ai_family,addr,ip,sizeof(ip));
		printf("%s : %s\n",ipver,ip);
	}
	if(recv_len = (recv(sfd, buffer, 1024, 0) == -1)) {
		perror("recv");
		return -1;
	}
	printf("CLIENT: Recv: %s\n",buffer);
	freeaddrinfo(res);	//Free the getaddrinfo structure	
	close(sfd);
return 0;
}
