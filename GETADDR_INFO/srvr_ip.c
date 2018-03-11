#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>

#define BACKLOG 10
//#define PORT 9000

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;	/*getaddrinfo structure*/
	struct sockaddr_in clnt_addr;	
	socklen_t clnt_size;
	int status, sfd, bnd, set = 1, lsn, new_fd, recv_len;
	char ip[INET_ADDRSTRLEN];		/*INET_ADDRSTRLEN = 16*/
	char buffer[1024];

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
		/* Create the socket */
		sfd  = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if (sfd == -1) {
			perror("socket");
			return -1;
		}
		printf("Socket FD = %d\n",sfd);
		int yes = 1;
		if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
			perror("setsockopt");
			return -1;
		}
		/* Bind the socket */
		bnd = bind(sfd,res->ai_addr,res->ai_addrlen);
		if(bnd == -1) {
			perror("bind");
			return -1;
		}
		printf("Bind successful\n");
		inet_ntop(p->ai_family,addr,ip,sizeof(ip));
		printf("%s : %s\n",ipver,ip);
	}

	lsn = listen(sfd,BACKLOG);
	if(lsn == -1) {
		perror("listen");
		return -1;
	}
	printf("listen successful\n");
	
	clnt_size = sizeof(struct sockaddr_storage);
	/*Accept from new connection socket*/
	while(1) {
		new_fd = accept(sfd,(struct sockaddr *)&clnt_addr,&clnt_size);
		if (new_fd == -1) {
			perror("accept");
		//	return -1;
		}
		printf("SERVER:Got Connection from %s: port %d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
		send(new_fd, "HELLO CLIENT", 13, 0);
		recv_len = recv(new_fd, &buffer, sizeof(buffer), 0);
		while(recv_len > 0) {
			printf("Recv %s: %d Bytes\n",buffer,recv_len);
			recv_len = recv(new_fd, &buffer, sizeof(buffer), 0);
			if(recv_len == 0) {
				printf("Client have close the connection\n");
				close(new_fd);
			}			
		}
		close(new_fd);
	}
	freeaddrinfo(res);	
	close(sfd);
return 0;
}
