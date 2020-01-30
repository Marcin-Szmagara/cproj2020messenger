#ifndef WEB_H
#define WEB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "timef.h"

int createSocket();
void setNonBlocking(int sockid);
void closeSocket(int sockid);
int sendBytes(int sockid, char* bytes, int nBytes);
int receiveBytes(int sockid, char* bytes, int nBytes);
int sendMessage(int sockid, char* message);
int receiveMessage(int sockid, char* message);

#endif
