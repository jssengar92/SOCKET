#include"header.h"
int main(int argc, char* argv[])
{
	int fd,cnt,len,opn,erno;
	char buffer[100];
	struct sockaddr_un skdr;
	/*
	 * creating socket for end to end communications.
	 * Domain:AF_UNIX(which can be used by a socket based on signle comp.that is not networked.
	 * TYPE:SOCK_STREAM:tp provided connection oriented ,reliable communication.
	 * Protocol:0(specified as single protocol.
	 * on success returns socket fd.
	 * 
	 * */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("socket created successfully\n");
	printf("%s:fd is %d\n",__FILE__,fd);
	/*server structure..*/
	skdr.sun_family=AF_UNIX;
	//strcpy(skdr.sun_path,"SOCKET_NAME");
	strncpy(&skdr.sun_path[1],"xyz",sizeof(skdr.sun_path)-2);
	len = sizeof(struct sockaddr_un);
	/*client requesting the connections....*/
	cnt = connect(fd,(struct sockaddr *)&skdr,len);
	if(cnt == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("connect() created successfully\n");
	memset(buffer,'\0',sizeof(buffer));	

	printf("Enter data\n");
	gets(buffer);	
		//writing data..	
	write(fd,buffer,strlen(buffer));
	exit(EXIT_SUCCESS);
}
