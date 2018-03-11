#include"header.h"
/*creating a socket queue of max size 10..*/
#define BACKLOG 10

int main(int argc, char* argv[])
{
	int fd,nme,lsn,acpt;
	int len,leng;
	int buffer,ret=1;
		/*structure for server...*/
	struct sockaddr_un skadr;
		/*structure for client...*/
	struct sockaddr_un skadr1;
	/*unlinking the socket path name...*/
	unlink("xyz");
	
	/*
	 * creating socket for end to end communications.
	 * Domain:AF_UNIX(which can be used by a socket based on signle comp....that is not networked.
	 * TYPE:SOCK_STREAM:tp provided connection oriented ,reliable communication.
	 * Protocol:0(specified as single protocol.
	 * on success returns socket file descriptor.
	 * */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd == -1) {
		//fprintf(stderr,"error encountered %d\n",errno);
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("socket created successfully\n");
	printf("%s:fd is %d",__FILE__,fd);
	
	/*givimg family and path...*/
	memset(&skadr,'\0',sizeof(struct sockaddr_un));
	skadr.sun_family=AF_UNIX;
	strncpy(&skadr.sun_path[1],"xyz",sizeof(skadr.sun_path)-2);
	len=sizeof(struct sockaddr_un);
	
	/*
	 * struct of server...
	 * bind()is used to name the socket so that it become visible across network.
	 * */
	nme = bind(fd, (struct sockaddr *)&skadr, len);
	if(nme == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	printf("bind() created successfully\n");
	/*to accepting the client request..*/
	lsn = listen(fd, BACKLOG);
	if(lsn == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("listen() created successfully\n");
	/*structure of client...*/
	skadr1.sun_family=AF_UNIX;
	strcpy(skadr1.sun_path,"xyz");
	leng=sizeof(struct sockaddr_un);
	
	/*accepting the client request...*/
	while(1) {
		acpt = accept(fd,(struct sockaddr *)&skadr1,&leng);
		if(acpt == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("accept() successful\n");
		//reading the data...
		while(read(acpt,&buffer,1)) {
			printf("%c",buffer);
		}
		printf("\n");
	}
	exit(EXIT_SUCCESS);
}  
