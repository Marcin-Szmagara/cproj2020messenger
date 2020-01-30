#include "Gui.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* messages[100];
int authors[100];
int nMessages = 0;

char currentMessage[10000];
char oldMessage[10000];
int messageLength = 0;

void initGui()
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
}

void killGui()
{
	endwin();
}

void addMessage(char* message, int author) // 0 us, 1 the other guy
{
	if (!messages[nMessages])
		messages[nMessages] = malloc(10000 * sizeof(char));
	strcpy(messages[nMessages], message);
	authors[nMessages] = author;
	nMessages = (nMessages + 1) % 100;
}

char* readInput()
{
	char x;
	while ((x = getch()) != ERR)
	{
		if (x == 27 || x == 4)
		{
			killGui();
			exit(0);
		}
		else if (x == 13 || x == 10)
		{
			if (messageLength)
			{
				strcpy(oldMessage, currentMessage);
				messageLength = 0;
				currentMessage[0] = '\0';
				return oldMessage;
			}
		}
		else if (x == 8 || x == 7)
		{
			if (messageLength)
			{
				messageLength--;
				currentMessage[messageLength] = '\0';
			}
		}
		else
		{
			if (messageLength < 10000 - 1)
			{
				currentMessage[messageLength] = x;
				currentMessage[messageLength + 1] = '\0';
				messageLength++;
			}
		}
	}
	return 0;
}

/*
void printScreen()
{
	erase();
	bool b = 1;
	int i = (nMessages + 1) % 100;
	while (b || i != (nMessages + 1) % 100) 
	{
		if (i == nMessages) b = 0;
		if (messages[i])
			printw("%d: %s\n", authors[i], messages[i]);
		i = (i + 1) % 100;
	}
	refresh();
}*/

/*
int printTextBox(char* text, int sx, int sy, int width, int align)
{
	int i = -1;
	int x = 0, y = 0;
	int c = 0;
	do
	{
		i++;
		if (!text[i] || isspace(text[i]))
		{
			if (c < width && x + c > width)
				y++, x = 0;
			for (int j = 0; j < c; j++)
			{
				mvaddch(sy + y, sx + x,
					text[i - c + j]);
				x++;
				if (x == width)
					x = 0, y++;
			}
			if (text[i] != '\0' && x)
			{
				mvaddch(sy + y, sx + x,
						' ');
				x++;
			}
			if (x == width)
				x = 0, y++;
			c = 0;
		}
		else c++;
	} while (text[i] != '\0');
	return y - (x == 0) + 1;
}*/

void printLine(char* line, int sx, int sy, int width,
		int align)
{
	if (align == 0)
	{
		for (int i = 0; i < width; i++)
			mvaddch(sy, sx + i, line[i]);
	}
	else
	{
		int j = 0;
		while (j < width
				&& line[width - j - 1] == ' ') j++;
		for (int i = 0; i < width - j; i++)
			mvaddch(sy, sx + i + j, line[i]);
	}
}

int printTextBox(char* text, int sx, int sy, int width, int align)
{
	int i = -1;
	int x = 0, y = 0;
	int c = 0;
	char line[10000];
	for (int i = 0; i < width; i++) line[i] = ' ';
	line[width] = '\0';
	do
	{
		i++;
		if (!text[i] || isspace(text[i]))
		{
			if (c < width && x + c > width)
			{
				printLine(line, sx, sy + y,
						width, align);
				for (int i = 0; i < width; i++) line[i] = ' ';
				line[width] = '\0';

				y++, x = 0;
			}
			for (int j = 0; j < c; j++)
			{
				line[x] = text[i - c + j];
				x++;
				if (x == width)
				{
					printLine(line, sx, sy + y,
						width, align);
					for (int i = 0; i < width; i++) line[i] = ' ';
						line[width] = '\0';

					x = 0, y++;
				}
			}
			if (text[i] != '\0' && x)
			{
				line[x] = ' ';
				x++;
				if (x == width)
				{
					printLine(line, sx, sy + y,
							width, align);
					for (int i = 0; i < width; i++) line[i] = ' ';
						line[width] = '\0';

					x = 0, y++;
				}
			}
			if (x != 0 && text[i] == '\0')
			{
				printLine(line, sx, sy + y,
							width, align);
					for (int i = 0; i < width; i++) line[i] = ' ';
					line[width] = '\0';

				
			}
			c = 0;
		}
		else c++;
	} while (text[i] != '\0');
	return y - (x == 0) + 1;
}

void printScreen()
{
	erase();
	
	int X, Y;
	getmaxyx(stdscr, Y, X);

	if (Y < 3 || X < 9)
	{
		printw("res not supp\n");
		return;
	}
	int hCurrent = printTextBox(currentMessage, 1e9, 1e9, X, 0);
	if (hCurrent == 0) hCurrent = 1;
	printTextBox(currentMessage, 0, Y - hCurrent, X, 0);
	char dashLine[1000];
	for (int i = 0; i < X; i++) dashLine[i] = '-';
	dashLine[X] = '\0';
	printTextBox(dashLine, 0, Y - hCurrent - 1, X, 0);

	int hl = Y - hCurrent - 2;
	int cm = nMessages;
	int padding = 4;
	if (X / 4 > padding) padding = X / 4;
	while (hl >= 0 && cm != (nMessages + 1) % 100)
	{
		if (messages[cm])
		{
			int hcm = printTextBox(messages[cm], 1e9, 1e9, X - padding, 1);
			int sx = 0;
			int align = 0;
			if (authors[cm] == 0)
			{
				sx += padding;
				align = 1;
			}
			printTextBox(messages[cm], sx, hl - hcm + 1,
					X - padding, align);
			hl = hl - hcm;
		}
		cm = (cm - 1 + 100) % 100;
	}

	refresh();
}
