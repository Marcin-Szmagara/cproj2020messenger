#define _DEFAULT_SOURCE
#include "chat.h"
#include "web.h"
#include "Gui.h"
#include <unistd.h>


void chat(int sockid)
{
	initGui();
	int sync = -2;
	while (1)
	{
		char receivedMessage[10000];

		sendBytes(sockid, &sync, 4);
		receiveMessage(sockid, receivedMessage);
		
		char* inputMessage;
		while (inputMessage = readInput())
		{
			addMessage(inputMessage, 0);

			if (sendMessage(sockid, inputMessage) == 1)
			{
				killGui();
				exit(1);
			}
		}

		sendBytes(sockid, &sync, 4);

		while (!receiveMessage(sockid, receivedMessage))
		{
			addMessage(receivedMessage, 1);
		}

		printScreen();
		usleep(10 * 1000);
	}
	killGui();
}
