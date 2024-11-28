#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>  
#include <conio.h> 
#include <windows.h> 
#include <time.h> 
#include <string> 
#include <iostream> 

using namespace std;

// GLOBALS //////////////////////////////////////////////// 

CONSOLE_SCREEN_BUFFER_INFO con_info;   // holds screen info 
HANDLE hconsole;         // handle to console 
int    game_running = 1; // state of game, 0=done, 1=run 

int spindle = 0;
int savedSpindle1 = 0;
int savedSpindle2 = 0;

int columnsStopped = 0;

bool stopped = false;

string ruletteCharacters[3][5] = {


	{"@", "$", "X", "8", "*"},
	{"*", "@", "$", "X", "8"},
	{"8", "*", "@", "$", "X"}

};

void init_Graphics() {

	// this function initializes the console graphics engine 
	COORD console_size = { 110, 30 }; // size of console 

	// seed the random number generator with time 
	srand((unsigned)time(NULL));

	// open i/o channel to console screen 
	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,

		FILE_SHARE_READ | FILE_SHARE_WRITE,

		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);

	SetConsoleScreenBufferSize(hconsole, console_size);

	GetConsoleScreenBufferInfo(hconsole, &con_info);

}
void Set_Color(int fcolor, int bcolor = 0)

{

	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) | fcolor));


}
void Draw_String(int x, int y, string s)

{
	// this function draws a string at the given x,y 
	COORD cursor_pos; // used to pass coords 
	// set printing position 
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);
	std::cout << s;

}
void Clear_Screen(void)

{

	// this function clears the screen 



	// set color to white on black 

	Set_Color(15, 0);



	// clear the screen 

	for (int index = 0; index <= 30; index++)

		Draw_String(0, 29, "\n");



} // end Clear_Screen 




void DrawRulette(int spindle, int savedSpindle1, int savedSpindle2, int columnsStopped) {

	Draw_String(10, 2, "spindle: " + std::to_string(spindle));

	//Set_Color(spindle + rand() % 6, 0);

	if (columnsStopped == 0) {
		if (spindle < 4 && spindle >= 1) {
			for (int x = 0; x < 3; x++) {
				Draw_String(0, x, ruletteCharacters[0][spindle + (x - 1)]);
				Draw_String(2, x, ruletteCharacters[1][spindle + (x - 1)]);
				Draw_String(4, x, ruletteCharacters[2][spindle + (x - 1)]);
			}
		}
		else if (spindle == 0) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 0) {
					adder = 4;
				}
				else {
					adder -= 1;
				}
				Draw_String(0, x, ruletteCharacters[0][spindle + adder]);
				Draw_String(2, x, ruletteCharacters[1][spindle + adder]);
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}
		else if (spindle == 4) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 2) {
					adder = -4;
				}
				else {
					adder -= 1;
				}
				Draw_String(0, x, ruletteCharacters[0][spindle + adder]);
				Draw_String(2, x, ruletteCharacters[1][spindle + adder]);
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}

	}
	else if (columnsStopped == 1) {

		if (spindle < 4 && spindle >= 1) {
			for (int x = 0; x < 3; x++) {
				Draw_String(2, x, ruletteCharacters[1][spindle + (x - 1)]);
				Draw_String(4, x, ruletteCharacters[2][spindle + (x - 1)]);
			}
		}
		else if (spindle == 0) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 0) {
					adder = 4;
				}
				else {
					adder -= 1;
				}
				Draw_String(2, x, ruletteCharacters[1][spindle + adder]);
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}
		else if (spindle == 4) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 2) {
					adder = -4;
				}
				else {
					adder -= 1;
				}
				Draw_String(2, x, ruletteCharacters[1][spindle + adder]);
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}

	}

	else if (columnsStopped == 2) {

		if (spindle < 4 && spindle >= 1) {
			for (int x = 0; x < 3; x++) {
				Draw_String(4, x, ruletteCharacters[2][spindle + (x - 1)]);
			}
		}
		else if (spindle == 0) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 0) {
					adder = 4;
				}
				else {
					adder -= 1;
				}
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}
		else if (spindle == 4) {
			for (int x = 0; x < 3; x++) {
				int adder = x;
				if (x == 2) {
					adder = -4;
				}
				else {
					adder -= 1;
				}
				Draw_String(4, x, ruletteCharacters[2][spindle + adder]);

			}

		}

	}
}











///////////////////////////////////////// MAIN GAME LOOP ///////////////////////////////////////// 

int main()

{

	char key; // player input data 

	init_Graphics();
	Clear_Screen();

	while (game_running) {

		////// KEYBOARD INPUT ///////
		if (_kbhit()) {

			// get keyboard data, and filter it 
			std::pair<char, int> keyP;

			keyP.first = 0;
			keyP.second = int(_getch());

			if (keyP.second == 0x1B) {

				game_running = false;
			}

			if (keyP.second == 's') {

				stopped = true;
				Draw_String(4, 20, "stopped");

			}
		}

		////// LOGIC ///////
		Set_Color(15, 0);


		if (stopped && columnsStopped <= 2) {
			stopped = false;
			columnsStopped += 1;
			savedSpindle1 = spindle;
			savedSpindle2 = spindle;
		}

		if (columnsStopped > 2) {
			Draw_String(100, 100, "WSSTOPPPPPP");
		}

		spindle += 1;

		DrawRulette(spindle, savedSpindle1, savedSpindle2, columnsStopped);

		if (spindle == 4) {
			spindle = -1;
		}


		// SECTION: synchronize to a constant frame rate 

		Sleep(100);

	}


	Clear_Screen();
	std::cout << "\nG A M E  O V E R \n\n";
	return 0;

}
