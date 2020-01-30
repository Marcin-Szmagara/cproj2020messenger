#include "web.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int createSocket()
{
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockid == -1)
	{
		perror("Unable to create a socket\n");
		exit(-1);
	}
	return sockid;
}

void setNonBlocking(int sockid)
{
	int status = fcntl(sockid, F_SETFL, fcntl(sockid,
				F_GETFL, 0) | O_NONBLOCK);
	if (status == -1)
	{
		perror("Unable to setup socket\n");
		exit(-1);
	}
}

void closeSocket(int sockid)
{
	int x = close(sockid);
	if (x == -1)
	{
		perror("Unable to close socket\n");
		exit(-1);
	}
}

int sendBytes(int sockid, char* bytes, int nBytes)
{
	const long long timeout = 250;
	int nSent = 0;
	long long t1 = milliseconds();
	while (nSent < nBytes)
	{
		int stat = send(sockid, (void *) (bytes + nSent), nBytes - nSent, 0);
		if (stat == -1)
		{
			perror("send() failed\n");
			return 1;
		}
		else if (stat == 0)
		{
			if (milliseconds() - t1 > timeout)
			{
				printf("Connection timed out\n");
				return 1;
			}
		}
		else t1 = milliseconds();
		nSent += stat;
	}
	return 0;
}

int receiveBytes(int sockid, char* bytes, int nBytes) // 0 on success, 1 on timeout
{
	const long long timeout = 250;
	int nReceived = 0;
	long long t1 = milliseconds();
	while (nReceived < nBytes)
	{
		int stat = recv(sockid, (void*) (bytes + nReceived), nBytes - nReceived, 0);
		if (stat == -1)
		{
			perror("recv() failed\n");
			return 1;
		}
		else if (stat == 0)
		{
			if (milliseconds() - t1 > timeout)
			{
				printf("Connection timed out\n");
				return 1;
			}
		}
		else t1 = milliseconds();
		nReceived += stat;
	}
	//printf("Received %d bytes\n", nReceived);
	return 0;
}

int sendMessage(int sockid, char* message)
{
	char* codedMessage = malloc(4 + strlen(message) + 1);
	strcpy(codedMessage + 4, message);
	((int*)(codedMessage))[0] = strlen(message) + 1;
	if (sendBytes(sockid, codedMessage, 4 + strlen(message) + 1)) return 1;
	return 0;
}

int receiveMessage(int sockid, char* message)
{
	int nBytes;
	if (receiveBytes(sockid, (char*)(&nBytes), 4)) return 1;
	if (nBytes < 0) return nBytes;
	if (receiveBytes(sockid, message, nBytes)) return 1;
	return 0;
}
