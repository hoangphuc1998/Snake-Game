
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <conio.h>
#include <stdlib.h>
#pragma pack(1)
using namespace std;
#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 4
//Khai bao vi tri ve menu
#define MENU_WIDTH 35
#define MENU_HEIGHT 20
//Khai bao cac struct
struct Point
{
	int x;
	int y;
	char kiTu;
};
struct Portal{
	Point start;
	Point middle;
	Point end;
	Point startMiddle;
	Point endMiddle;
	Point Entrance;
	int EntranceMove;
};

extern Point Snake[23];
extern Point Food[4];
extern int CHAR_LOCK;//Bien dinh huong khong the di chuyen
extern int MOVING;//Bien dih huong ran co the di chuyen
extern int SPEED;
extern int HEIGHT_CONSOLE, WIDTH_CONSOLE;
extern int FOOD_INDEX;//Chi so food hien hanh tren man hinh
extern int SIZE_SNAKE;
extern int STATE;//Trang thai cua ran (chet hay con song)
extern Portal por;
extern bool isPortal;
extern int Score;

//Khai bao cac ham trong chuong trinh
void FixConsoleWindows();
void GoToXY(int x, int y);
//Ham Kiem tra tinh hop le cua 1 point
bool isValid(int x,int y);
//Ham tao Food
void GenerateFood();
//Ham khoi tao lai cac bien toan cuc
void ResetData();
//Ham ve man hinh game
void DrawBoard(int width, int height, int curPosX = 0, int curPosY = 0);
//Start Game
void StartGame();
//Exit Game
void ExitGame(HANDLE t);
//Pause Game
void PauseGame(HANDLE t);
//Ham xu li khi ran an moi
void Eat();
//Ham xu ly chet
void ProcessDead();
//Ham ve ran va food
void DrawSnakeAndFood();
//Ham xoa snake va food
void DeleteSnakeAndFood();
//Nhom ham di chuyen
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
//Thu tuc Thread
void ThreadFunc();
//Ham tao Portal
void GeneratePortal();
//Ham ve cong
void DrawPortal();
//Ham xoa cong
void DeletePortal();
//Ham kiem tra ran da vao cong chua
bool isComing(int x, int y);
//Ham kiem tra xem ran co va vao cong khong
bool PortalDead(int x, int y);
//Ham cho ran vao cong
void GetIntoPortal();
//Ham luu file
bool SaveGame(char* s);
//Ham load game
bool LoadGame(char* s);
//Ham xoa man hinh khi xu li luu va tai game
void DrawScreen();
//Ham de game duoc tiep tuc
void ContinueGame(HANDLE t);
//Ham ve menu cua game
void DrawMenu();