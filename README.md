# udp-client-and-server

<h1> demo link : https://www.youtube.com/watch?v=_-FQ30OVD1o
<h2> client code (UDP-Client.c):
<h3> void Connect_To_Server_And_Send_Message(int sockfd, struct sockaddr_in servaddr, char* message, char* buffer, int MAX_RETRY, int wait_time, int multiplier) 
<h4> The function does :
<h5> 1. Connect to the server IP obtained from servaddr. For TCP, if it fails -> exit(0);
<h5> 2. Send message to server ans receive from it. If there is no reply, resend message for MAX_RETRY times and the wait interval follows the exponenetial backoff algorithm.
<h5> 3. Once reaching the MAX_RETRY , the connection will be given up and the program will be shut down by exit(1).

<h3> void Init_ServAddr(struct sockaddr_in* servaddr, char* ServerIP)
<h4> The function does :
<h5> 1. Initialize a socket address struct (struct sockaddr_in)

<h3> int Create_Socket(bool UDP)
<h4> The function does :
<h5> 1. Return a socket descriptor of UDP (SOCKET_DGRAM) or TCP(SOCKET_STREAM).

<h2> server code (UDP-Server.c)
<h3> void Init_ServAddr(struct sockaddr_in* servaddr)
<h4> Similar to the one in client code. No IP assigned.

<h3> int Create_Socket(bool UDP)
<h4> same as client code.

<h3> void ServerInfo()
<h4> the function shows the hostname and IP address binded.
