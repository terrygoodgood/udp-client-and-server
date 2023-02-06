#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <stdbool.h> 
#define PORT 5001
#define MAXLINE 1000



void Connect_To_Server_And_Send_Message(int sockfd, struct sockaddr_in servaddr, \
                                         char* message, char* buffer, int MAX_RETRY, int wait_time, int multiplier){
    /// 1. connect to the server address
    /// --> if fails, exit(0)
    /// 2. send message to server ans receive from it
    /// --> if fails , retry for MAX_RETRY times.
    /// 3. if MAX_RETRY is reached, exit(1);

	// step 1
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
	
	// step 2
    int connect_status = 0;
	
    while (MAX_RETRY >= 0) {
        sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));

        connect_status = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)NULL, NULL);

        if (connect_status == -1 && MAX_RETRY >0) {
            MAX_RETRY--;

            printf("[ Warning ] SERVER NO RESPONSE. Resending %d times more.\n", MAX_RETRY);
            
			if(wait_time < 8*1000*1000) wait_time *= multiplier;

            usleep(wait_time); // wait_interval

        }else if( MAX_RETRY == 0){
			// step 3
            // reach MAX_RETRY
            printf("[  Error  ] MAX_RETRY reached. Connection aborted.\n");
            exit(1);
        }else{
            // Message received from server. (success)
            printf("######## [ Server Message ] ########\n");
            puts(buffer);
            // close the descriptor
            close(sockfd);
            return;
        }
    }
}

void Init_ServAddr(struct sockaddr_in* servaddr, char* ServerIP){
    bzero(servaddr, sizeof(servaddr));
    servaddr->sin_addr.s_addr = inet_addr(ServerIP);
    servaddr->sin_port = htons(PORT);
    servaddr->sin_family = AF_INET;
}



int Create_Socket(bool UDP){
	if(UDP) return socket(AF_INET, SOCK_DGRAM, 0);
	else return socket(AF_INET, SOCK_STREAM, 0);
}
// Driver code
void main(int argc,  char *argv[])
{

    char buffer[100];
    char *message     = argv[1];
    int sockfd        = 0;
    char* str         = argv[2];
    int MAX_RETRY;
    sscanf(str, "%d", &MAX_RETRY);
    bool UDP = true;
    char* ServerIP      = argv[3];

    /////////////////////////////////////////
    ///         Wait Interval Args        ///
    /////////////////////////////////////////

    int base          = 500;
    int multiplier    = 2;
    int wait_time     = base*1000;

    //////////////////////////////////////////////////////////////////
    ///         initialize server address and create socket        ///
    //////////////////////////////////////////////////////////////////

    struct sockaddr_in servaddr;
    Init_ServAddr(&servaddr, ServerIP);
	sockfd = Create_Socket(UDP); // true -> UDP ; false -> TCP

    //////////////////////////////////////////////////////////////////////////
    ///         Connect to server and retry sending message until:         ///
    ///                                                                    ///
    ///         1. receiving server message                                ///
    ///         2. reaching MAX_RETRY                                      ///
    //////////////////////////////////////////////////////////////////////////

    Connect_To_Server_And_Send_Message(sockfd, servaddr, message, buffer, MAX_RETRY, wait_time, multiplier);

    return;
}

