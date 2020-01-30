#ifndef GUI
#define GUI

void initGui();
void killGui();
void addMessage(char* message, int autor);
char* readInput();
void printLine(char* line, int sx, int sy, int width,
		int align);
int printTextBox(char* text, int sx, int sy, int width,
		int align);
void printScreen();

#endif
