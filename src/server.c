// server code
#include "web.h"
#include "chat.h"

int bindSocket(int sockid) // binds a socket to an address, returns port number
{
	struct sockaddr_in addrport;
	
	int retCode = 0, currPort = 27093;
	do
	{
		currPort++;
		addrport.sin_port = htons(currPort);
		addrport.sin_family = AF_INET;
		addrport.sin_addr.s_addr = htonl(INADDR_ANY);
		retCode = bind(sockid, (struct sockaddr*) &addrport, sizeof(addrport));
	} while (currPort < 40220 && retCode);
	if (retCode)
	{
		perror("Unable to bind socket\n");
		exit(-1);
		return -1;
	}
	return currPort;
}

// puts sockid to listen state and returns the socket used to talk
int waitForConnection(int sockid) // blocking
{
	int x = listen(sockid, 1);
	if (x)
	{
		perror("listen() failed\n");
		exit(-1);
	}
	printf("Waiting for connections...\n");
	struct sockaddr addrport;
	socklen_t sz = sizeof(addrport);
	int socktalk = -1;
	socktalk = accept(sockid, &addrport, &sz);
	if (socktalk < 0)
	{
		perror("accept() failed\n");
		exit(-1);
	}
	printf("Connected to a client\n");
	return socktalk;
}

int main(int argc, char *argv[])
{
	int waitHandle = createSocket();
	int port = bindSocket(waitHandle);
	printf("Server set up on port %d\n", port);

	int client = waitForConnection(waitHandle);

	chat(client);
	
	closeSocket(client);
	
	closeSocket(waitHandle);
}
