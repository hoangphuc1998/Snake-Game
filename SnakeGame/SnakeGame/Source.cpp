#include "Header.h"

void FixConsoleWindows()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void GoToXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool isValid(int x, int y)
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (Snake[i].x == x && Snake[i].y == y)
			return false;
	}
	return true;
}

void GenerateFood()
{
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
	{
		do
		{
			x = rand() % (WIDTH_CONSOLE - 2) + 2;
			y = rand() % (HEIGHT_CONSOLE - 2) + 2;
		} while (isValid(x, y) == false);
		Food[i] = { x, y ,'O'};
	}
}

void ResetData()
{
	CHAR_LOCK = 'A';
	MOVING = 'D';
	SPEED = 1;
	FOOD_INDEX = 0;
	WIDTH_CONSOLE = 70;
	HEIGHT_CONSOLE = 20;
	SIZE_SNAKE = 7;
	//Khoi tao gia tri mac dinh cho snake
	Snake[0] = { 10, 5, '1' }; Snake[1] = { 11, 5, '6' }; Snake[2] = { 12, 5, '1' };
	Snake[3] = { 13, 5, '2' }; Snake[4] = { 14, 5, '5' }; Snake[5] = { 15, 5, '2' };
	Snake[6] = { 16, 5, '1' };
	//Tao mang food
	GenerateFood();
	isPortal = false;
}

void DrawBoard(int width, int height, int curPosX, int curPosY)
{
	//Ve canh tren
	GoToXY(0,0);
	for (int i = 0; i <= width; i++) cout << 'X';
	//Ve canh duoi
	GoToXY(0, height);
	for (int i = 0; i <= width; i++) cout << 'X';
	//Ve 2 canh o 2 ben
	for (int i = 1; i < height; i++)
	{
		GoToXY(0,i); cout << 'X';
		GoToXY(width,i); cout << 'X';
	}
	GoToXY(curPosX, curPosY);
}

void StartGame()
{
	system("cls");
	ResetData();
	DrawBoard(WIDTH_CONSOLE, HEIGHT_CONSOLE);
	STATE = 1;
	Score = 0;
}

void ExitGame(HANDLE t)
{
	system("cls");
	TerminateThread(t, 0);
}

void PauseGame(HANDLE t)
{
	SuspendThread(t);
	GoToXY(0, HEIGHT_CONSOLE + 3);
	cout << "Press T to load game" << endl;
	cout << "Press L to save game" << endl;
	cout << "Press R to restart game" << endl;
	cout << "Press anykey to resume" << endl;
	int key = toupper(_getch());
	char s[100];
	switch (key)
	{
	case 'R':
		StartGame();
		DrawSnakeAndFood();
		GoToXY(WIDTH_CONSOLE + 5, 10);
		cout << "Score: " << Score;
		GoToXY(WIDTH_CONSOLE + 5, 12);
		cout << "Level: " << SPEED;
		break;
	case 'L':
		DrawScreen();
		cout << "Enter file name: ";
		_flushall();
		gets(s);
		s[strlen(s)] = '\0';
		if (SaveGame(s)) cout << "Save game successfully" << endl;
		else cout << "Error Saving"<<endl;
		system("pause");
		ContinueGame(t);
		break;
	case 'T':
		DrawScreen();
		cout << "Enter file name: ";
		_flushall();
		gets(s);
		s[strlen(s)] = '\0';
		if (LoadGame(s)) cout << "Load game successfully" << endl;
		else cout << "Error Loading. File is not existed" << endl;
		system("pause");
		ContinueGame(t);
		break;
	default:
		ContinueGame(t);
		break;
	}
}

void Eat()
{
	Snake[SIZE_SNAKE] = Food[FOOD_INDEX];
	Snake[SIZE_SNAKE].kiTu = Snake[SIZE_SNAKE%7].kiTu;
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = 0;
		SIZE_SNAKE++;
		isPortal = false;
		GenerateFood();
	}
	else
	{
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
	Score++;
}

void ProcessDead()
{
	cout << "\a\a";
	DrawSnakeAndFood();
	GoToXY(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y);
	cout << "X";
	Sleep(1000);
	DrawSnakeAndFood();
	Sleep(1000);
	DrawSnakeAndFood();
	GoToXY(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y);
	cout << "X";
	
	STATE = 0;
	DeletePortal();
	GoToXY(0, HEIGHT_CONSOLE + 2);
	cout << "You are dead. Press Y to continue or anykey to exit";
}

void DrawSnakeAndFood()
{
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		if (!isPortal)
		{
			GeneratePortal();
			DrawPortal();
			isPortal = true;
		}
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GoToXY(Snake[i].x, Snake[i].y);
			cout << Snake[i].kiTu;
		}
		return;
	}
	GoToXY(Food[FOOD_INDEX].x, Food[FOOD_INDEX].y);
	cout << Food[FOOD_INDEX].kiTu;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		GoToXY(Snake[i].x, Snake[i].y);
		cout << Snake[i].kiTu;
	}
}

void DeleteSnakeAndFood()
{
	GoToXY(Food[FOOD_INDEX].x, Food[FOOD_INDEX].y);
	cout << " ";
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		GoToXY(Snake[i].x, Snake[i].y);
		cout << " ";
	}
}

void MoveRight()
{
	bool dead = false;
	if (Snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE) dead = true;
	else if (PortalDead(Snake[SIZE_SNAKE - 1].x + 1, Snake[SIZE_SNAKE - 1].y)) dead = true;
	else
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			if (Snake[SIZE_SNAKE - 1].x + 1 == Snake[i].x && Snake[SIZE_SNAKE - 1].y == Snake[i].y)
			{
				dead = true;
				break;
			}
		}
	}
	if (isComing(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y) && por.EntranceMove == 'D')
	{
		GetIntoPortal();
		return;
	}
	if (dead)
	{
		ProcessDead();
	}
	else
	{
		if (Snake[SIZE_SNAKE - 1].x + 1 == Food[FOOD_INDEX].x && Snake[SIZE_SNAKE - 1].y == Food[FOOD_INDEX].y && isPortal==false)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE-1; i++)
		{
			Snake[i].x = Snake[i + 1].x;
			Snake[i].y = Snake[i + 1].y;
		}
		Snake[SIZE_SNAKE - 1].x++;
	}
}

void MoveLeft()
{
	bool dead = false;
	if (Snake[SIZE_SNAKE - 1].x - 1 == 0) dead = true;
	else if (PortalDead(Snake[SIZE_SNAKE - 1].x - 1, Snake[SIZE_SNAKE - 1].y))
		dead = true;
	else
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			if (Snake[SIZE_SNAKE - 1].x - 1 == Snake[i].x && Snake[SIZE_SNAKE - 1].y == Snake[i].y)
			{
				dead = true;
				break;
			}
		}
	}
	if (isComing(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y) && por.EntranceMove == 'A')
	{
		GetIntoPortal();
		return;
	}
	if (dead)
	{
		ProcessDead();
	}
	else
	{
		if (Snake[SIZE_SNAKE - 1].x - 1 == Food[FOOD_INDEX].x && Snake[SIZE_SNAKE - 1].y == Food[FOOD_INDEX].y && isPortal == false)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			Snake[i].x = Snake[i + 1].x;
			Snake[i].y = Snake[i + 1].y;
		}
		Snake[SIZE_SNAKE - 1].x--;
	}
}

void MoveDown()
{
	bool dead = false;
	if (Snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE) dead = true;
	else if (PortalDead(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y + 1))
		dead = true;
	else
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			if (Snake[SIZE_SNAKE - 1].x  == Snake[i].x && Snake[SIZE_SNAKE - 1].y+1 == Snake[i].y)
			{
				dead = true;
				break;
			}
		}
	}
	if (isComing(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y) && por.EntranceMove == 'W')
	{
		GetIntoPortal();
		return;
	}
	if (dead)
	{
		ProcessDead();
	}
	else
	{
		if (Snake[SIZE_SNAKE - 1].x == Food[FOOD_INDEX].x && Snake[SIZE_SNAKE - 1].y + 1 == Food[FOOD_INDEX].y && isPortal == false)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			Snake[i].x = Snake[i + 1].x;
			Snake[i].y = Snake[i + 1].y;
		}
		Snake[SIZE_SNAKE - 1].y++;
	}
}

void MoveUp()
{
	bool dead = false;
	if (Snake[SIZE_SNAKE - 1].y - 1 == 0) dead = true;
	else if (PortalDead(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y - 1)) dead = true;
	else
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			if (Snake[SIZE_SNAKE - 1].x  == Snake[i].x && Snake[SIZE_SNAKE - 1].y -1== Snake[i].y)
			{
				dead = true;
				break;
			}
		}
	}
	if (isComing(Snake[SIZE_SNAKE - 1].x, Snake[SIZE_SNAKE - 1].y) && por.EntranceMove == 'S')
	{
		GetIntoPortal();
		return;
	}
	if (dead)
	{
		ProcessDead();
	}
	else
	{
		if (Snake[SIZE_SNAKE - 1].x == Food[FOOD_INDEX].x && Snake[SIZE_SNAKE - 1].y - 1 == Food[FOOD_INDEX].y && isPortal == false)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			Snake[i].x = Snake[i + 1].x;
			Snake[i].y = Snake[i + 1].y;
		}
		Snake[SIZE_SNAKE - 1].y--;
	}
}

void ThreadFunc()
{
	while (1)
	{
		if (STATE == 1)
		{
			DeleteSnakeAndFood();
			switch (MOVING)
			{
			case 'A':
				MoveLeft();
				break;
			case 'S':
				MoveDown();
				break;
			case 'W':
				MoveUp();
				break;
			case 'D':
				MoveRight();
				break;
			}
			DrawSnakeAndFood();
			GoToXY(WIDTH_CONSOLE + 5, 10);
			cout << "Score: " << Score;
			GoToXY(WIDTH_CONSOLE + 5, 12);
			cout << "Level: " << SPEED;
			GoToXY(WIDTH_CONSOLE + 5, 14);
			cout << "Press P to pause game";
			Sleep(200 / SPEED);
		}
	}
}

void GeneratePortal()
{
	srand(time(NULL));
	do{
		int x, y;
		do
		{
			x = rand() % (WIDTH_CONSOLE - 5) + 3;
			y = rand() % (HEIGHT_CONSOLE - 5) + 3;
		} while (isValid(x, y) == false);
		por.start = { x, y, '0' };
		int k = rand() % 8;
		//Tao cong theo 8 huong ung voi diem dau cua cong
		switch (k)
		{
		case 0:
			por.end = { x - 2, y, '0' };
			por.middle = { x - 1, y + 1, '0' };
			por.EntranceMove = 'W';
			por.startMiddle = { por.start.x, por.middle.y, '0' };
			por.endMiddle = { por.end.x, por.middle.y, '0' };
			por.Entrance = { por.middle.x, por.start.y, ' ' };
			break;
		case 1:
			por.end = { x - 2, y, '0' };
			por.middle = { x - 1, y - 1, '0' };
			por.EntranceMove = 'S';
			por.startMiddle = { por.start.x, por.middle.y, '0' };
			por.endMiddle = { por.end.x, por.middle.y, '0' };
			por.Entrance = { por.middle.x, por.start.y, ' ' };
			break;
		case 2:
			por.end = { x, y + 2, '0' };
			por.middle = { x - 1, y + 1, '0' };
			por.EntranceMove = 'A';
			por.startMiddle = { por.middle.x, por.start.y, '0' };
			por.endMiddle = { por.middle.x, por.end.y, '0' };
			por.Entrance = { por.start.x, por.middle.y, ' ' };
			break;
		case 3:
			por.end = { x, y + 2, '0' };
			por.middle = { x + 1, y + 1, '0' };
			por.EntranceMove = 'D';
			por.startMiddle = { por.middle.x, por.start.y, '0' };
			por.endMiddle = { por.middle.x, por.end.y, '0' };
			por.Entrance = { por.start.x, por.middle.y, ' ' };
			break;
		case 4:
			por.end = { x + 2, y, '0' };
			por.middle = { x + 1, y + 1, '0' };
			por.EntranceMove = 'W';
			por.startMiddle = { por.start.x, por.middle.y, '0' };
			por.endMiddle = { por.end.x, por.middle.y, '0' };
			por.Entrance = { por.middle.x, por.start.y, ' ' };
			break;
		case 5:
			por.end = { x + 2, y, '0' };
			por.middle = { x + 1, y - 1, '0' };
			por.EntranceMove = 'S';
			por.startMiddle = { por.start.x, por.middle.y, '0' };
			por.endMiddle = { por.end.x, por.middle.y, '0' };
			por.Entrance = { por.middle.x, por.start.y, ' ' };
			break;
		case 6:
			por.end = { x, y - 2, '0' };
			por.middle = { x - 1, y - 1, '0' };
			por.EntranceMove = 'A';
			por.startMiddle = { por.middle.x, por.start.y, '0' };
			por.endMiddle = { por.middle.x, por.end.y, '0' };
			por.Entrance = { por.start.x, por.middle.y, ' ' };
			break;
		case 7:
			por.end = { x, y - 2, '0' };
			por.middle = { x + 1, y - 1, '0' };
			por.EntranceMove = 'D';
			por.startMiddle = { por.middle.x, por.start.y, '0' };
			por.endMiddle = { por.middle.x, por.end.y, '0' };
			por.Entrance = { por.start.x, por.middle.y, ' ' };
			break;
		}
	} while (!isValid(por.end.x, por.end.y) || !isValid(por.middle.x, por.middle.y) || !isValid(por.startMiddle.x, por.startMiddle.y)
		|| !isValid(por.endMiddle.x, por.endMiddle.y));
}

void DrawPortal()
{
	GoToXY(por.start.x, por.start.y);
	cout << por.start.kiTu;
	GoToXY(por.end.x, por.end.y);
	cout << por.end.kiTu;
	GoToXY(por.middle.x, por.middle.y);
	cout << por.middle.kiTu;
	GoToXY(por.startMiddle.x, por.startMiddle.y);
	cout << por.startMiddle.kiTu;
	GoToXY(por.endMiddle.x, por.endMiddle.y);
	cout << por.endMiddle.kiTu;
}

void DeletePortal()
{
	GoToXY(por.start.x, por.start.y);
	cout << " ";
	GoToXY(por.end.x, por.end.y);
	cout << " ";
	GoToXY(por.middle.x, por.middle.y);
	cout << " ";
	GoToXY(por.startMiddle.x, por.startMiddle.y);
	cout << " ";
	GoToXY(por.endMiddle.x, por.endMiddle.y);
	cout << " ";
	por.start = por.end = por.startMiddle = por.endMiddle = por.middle=por.Entrance= { 1000, 1000, '0' };
}

bool isComing(int x, int y)
{
	if (x == por.Entrance.x && y == por.Entrance.y)
		return true;
	return false;
}

bool PortalDead(int x, int y)
{
	if ((x == por.start.x && y == por.start.y) || (x==por.end.x && y==por.end.y) || (x==por.middle.x && y==por.middle.y)
		|| (x == por.startMiddle.x && y == por.startMiddle.y) || (x == por.endMiddle.x && y == por.endMiddle.y))
	{
		return true;
	}
	return false;
}

void GetIntoPortal()
{
	
	//Process get into portal eventually
	for (int i = SIZE_SNAKE - 1; i >= 0; i--)
	{

		for (int j = 0; j < SIZE_SNAKE; j++)
		{
			GoToXY(Snake[j].x, Snake[j].y);
			if (j < SIZE_SNAKE - i) cout << " ";
			else cout << Snake[(j-SIZE_SNAKE+i)%7].kiTu;
		}
		Sleep(250 / (SPEED));
	}
	//Set the speed for snake
	if (SPEED == MAX_SPEED)
	{
		SPEED = 1;
		SIZE_SNAKE = 7;
	}
	else SPEED++;
	//Set the snake when get through portal
	if (por.EntranceMove == 'A')
	{
		MOVING = 'A';
		CHAR_LOCK = 'D';
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GoToXY(Snake[i].x, Snake[i].y);
			cout << " ";
			Snake[i].x = WIDTH_CONSOLE -i - 10;
			Snake[i].y = 5;
		}
	}
	else
	{
		MOVING = 'D';
		CHAR_LOCK = 'A';
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GoToXY(Snake[i].x, Snake[i].y);
			cout << " ";
			Snake[i].x = i + 10;
			Snake[i].y = 5;
		}
	}
	DeletePortal();
	FOOD_INDEX = 0;
	isPortal = false;
	GenerateFood();
}

bool SaveGame(char* s)
{
	FILE *f = fopen(s, "wb");
	if (f == NULL) return false;
	fwrite(Snake, sizeof(Point), 23, f);
	fwrite(Food, sizeof(Point), 4, f);
	fwrite(&CHAR_LOCK, sizeof(int), 1, f);
	fwrite(&MOVING, sizeof(int), 1, f);
	fwrite(&SPEED, sizeof(int), 1, f);
	fwrite(&FOOD_INDEX, sizeof(int), 1, f);
	fwrite(&SIZE_SNAKE, sizeof(int), 1, f);
	fwrite(&STATE, sizeof(int), 1, f);
	fwrite(&por, sizeof(Portal), 1, f);
	fwrite(&isPortal, sizeof(bool), 1, f);
	fwrite(&Score, sizeof(int), 1, f);
	fwrite(&WIDTH_CONSOLE, sizeof(int), 1, f);
	fwrite(&HEIGHT_CONSOLE, sizeof(int), 1, f);
	fclose(f);
	return true;
}

void DrawScreen()
{
	system("cls");
	DrawBoard(WIDTH_CONSOLE, HEIGHT_CONSOLE);
	DrawSnakeAndFood();
	if (isPortal) DrawPortal();
	GoToXY(WIDTH_CONSOLE + 5, 10);
	cout << "Score: " << Score;
	GoToXY(WIDTH_CONSOLE + 5, 12);
	cout << "Level: " << SPEED;
	GoToXY(0, HEIGHT_CONSOLE + 3);
}

void ContinueGame(HANDLE t)
{
	system("cls");
	DrawBoard(WIDTH_CONSOLE, HEIGHT_CONSOLE);
	DrawSnakeAndFood();
	if (isPortal) DrawPortal();
	GoToXY(WIDTH_CONSOLE + 5, 10);
	cout << "Score: " << Score;
	GoToXY(WIDTH_CONSOLE + 5, 12);
	cout << "Level: " << SPEED;
	ResumeThread(t);
}

bool LoadGame(char* s)
{
	FILE *f = fopen(s, "rb");
	if (f == NULL) return false;
	fread(Snake, sizeof(Point), 23, f);
	fread(Food, sizeof(Point), 4, f);
	fread(&CHAR_LOCK, sizeof(int), 1, f);
	fread(&MOVING, sizeof(int), 1, f);
	fread(&SPEED, sizeof(int), 1, f);
	fread(&FOOD_INDEX, sizeof(int), 1, f);
	fread(&SIZE_SNAKE, sizeof(int), 1, f);
	fread(&STATE, sizeof(int), 1, f);
	fread(&por, sizeof(Portal), 1, f);
	fread(&isPortal, sizeof(bool), 1, f);
	fread(&Score, sizeof(int), 1, f);
	fread(&WIDTH_CONSOLE, sizeof(int), 1, f);
	fread(&HEIGHT_CONSOLE, sizeof(int), 1, f);
	fclose(f);
	return true;
}

void DrawMenu()
{
	GoToXY(MENU_WIDTH,MENU_HEIGHT);
	cout << "Press T to load game";
	GoToXY(MENU_WIDTH, MENU_HEIGHT + 4);
	cout << "Press any key to start game";
}