// server program for udp connection
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <netdb.h>

#define PORT 5001
#define MAXLINE 1000

// Driver code

int Create_Socket(bool UDP){
    if(UDP) return socket(AF_INET, SOCK_DGRAM, 0);
	else return socket(AF_INET, SOCK_STREAM, 0);
}
void Init_ServAddr(struct sockaddr_in* servaddr){
    bzero(servaddr, sizeof(servaddr));
    servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(PORT);
    servaddr->sin_family = AF_INET;
}

void ServerInfo(){
	char ac[80];
	gethostname(ac, sizeof(ac));
	printf("Host name: %s\n", ac);
	struct hostent* p_he = gethostbyname(ac);
	 if (p_he != 0){
		for (int i=0; p_he->h_addr_list[i] != 0; ++i)
		{
			struct in_addr addr;
			memcpy(&addr, p_he->h_addr_list[i], sizeof(struct in_addr));
			printf("IP address %d: %s\n", i, inet_ntoa(addr));
		}
	}
}

int main()
{
	char buffer[100];

	int listenfd = 0;
	int len = 0;
	bool UDP = true;
	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));

	// Create a UDP Socket
	listenfd = Create_Socket(UDP);		
	ServerInfo();
	Init_ServAddr(&servaddr);

	// bind server address to socket descriptor
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
    // receive the datagram
	len = sizeof(cliaddr);
	printf("Waiting for message...\n\n\n");
	int n = recvfrom(listenfd, buffer, sizeof(buffer),
			0, (struct sockaddr*)&cliaddr,&len); //receive message from server
	buffer[n] = '\0';
	printf("###### [ Message received from client ] ######\n");
	puts(buffer);
		
	// send the response
	sendto(listenfd, buffer, sizeof(buffer), 0,
		(struct sockaddr*)&cliaddr, sizeof(cliaddr));
}

