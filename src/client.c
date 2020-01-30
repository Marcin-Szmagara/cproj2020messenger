// client code
#include "web.h"
#include "chat.h"

void connectToServer(int sockid, const char* ipv4, int port)
{
	struct sockaddr_in servAddr;
	servAddr.sin_family= AF_INET; 
	servAddr.sin_addr.s_addr= inet_addr(ipv4);
	servAddr.sin_port= htons(port);
	printf("Connecting to the server...\n");
	if (connect(sockid, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		perror("connect() failed\n");
		exit(-1);
	}
	printf("Connected to the server\n");
}

int main(int argc, char *argv[])
{
	int server = createSocket();
	char ipv4[100] = "127.0.0.1";
	int port = 27094;
	if (argc == 3)
	{
		printf("Overriding default address\n");
		strcpy(ipv4, argv[1]);
		port = atoi(argv[2]);
	}
	else if (argc == 2)
	{
		printf("Overriding default address\n");
		port = atoi(argv[1]);
	}

	connectToServer(server, ipv4, port);

	chat(server);
	
	closeSocket(server);
}
