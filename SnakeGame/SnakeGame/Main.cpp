#include "Header.h"
//Khai bao cac bien toan cuc
Point Snake[23];
Point Food[4];
int CHAR_LOCK;//Bien dinh huong khong the di chuyen
int MOVING;//Bien dih huong ran co the di chuyen
int SPEED;
int HEIGHT_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;//Chi so food hien hanh tren man hinh
int SIZE_SNAKE;
int STATE;//Trang thai cua ran (chet hay con song)
Portal por;
bool isPortal;//Xem cong da duoc ve hay chua
int Score;

void main()
{
	int temp;
	FixConsoleWindows();
	//Draw menu
	DrawMenu();
	temp = toupper(_getch());
	switch (temp)
	{
	case 'T':
		system("cls");
		GoToXY(MENU_WIDTH, MENU_HEIGHT);
		cout << "Enter file name: ";
		_flushall();
		char s[100];
		gets(s);
		s[strlen(s)] = '\0';
		GoToXY(MENU_WIDTH, MENU_HEIGHT+2);
		if (LoadGame(s)) cout << "Load game successfully" << endl;
		else
		{
			cout << "Error Loading. File is not existed" << endl;
			GoToXY(MENU_WIDTH, MENU_HEIGHT + 4);
			system("pause");
			StartGame();
			break;
		}
		GoToXY(MENU_WIDTH, MENU_HEIGHT+4);
		system("pause");
		system("cls");
		DrawBoard(WIDTH_CONSOLE, HEIGHT_CONSOLE);
		if (isPortal) DrawPortal();
		break;
	default:
		StartGame();
		break;
	}
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	while (1)
	{
		temp = toupper(_getch());
		if (STATE == 1)
		{
			if (temp == 'P')
			{
				PauseGame(handle_t1);
			}
			else if (temp == 27)
			{
				ExitGame(handle_t1);
				t1.join();
				return;
			}
			else
			{
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'A' || temp == 'S' || temp == 'D' || temp == 'W'))
				{
					if (temp == 'A') CHAR_LOCK = 'D';
					else if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else CHAR_LOCK = 'W';
					MOVING = temp;
				}
			}
		}
		else
		{
			if (temp == 'Y') StartGame();
			else
			{
				ExitGame(handle_t1);
				t1.join();
				return;
			}
		}
	}
	t1.join();
	return;
}

