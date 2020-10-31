#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include "COORDQueue.h"

#define true 1
#define false 0
#define bool char

// key define
#define keyUp 72
#define keyDown 80
#define keyLeft 75
#define keyRight 77
#define keyEnter 13
#define keySpace 32

// const
#define defaultRow 20
#define defaultColumn 20
#define maxFieldHeight 32
#define minFieldHeight 12
#define maxFieldWidth 32
#define minFieldWidth 12
#define maxGameSpeed 10
#define minGameSpeed 1
const COORD startPosition = { 6,7 };

// option variable
int gameFieldHeight = defaultRow - 2;
int gameFieldWidth = defaultColumn - 2;
int gameSpeed = 5;

// method claim
void* stage_Menu();
void stage_Game();
void stage_Description();
void stage_GameOption();
void stage_GameOver(int level);
void* keyDetect(void* argu);

// const string
char menuStage_TitleString[] = { "\n\n　　　－－－－貪食蛇小遊戲－－－－\n\n　　　　　(-_ - ;)　by Kevin" };
char menuStage_OptionString[] = { "\n\n\n　　　　　　　　開始遊戲　　\n\n　　　　　　　　遊戲說明　　　\n\n　　　　　　　　　選項　　　\n\n　　　　　　　　　結束　　　\n" };
char descriptionStage_DescriptionString[] = { "\n\n　　　－－－－　遊戲說明　－－－－\n\n　　透過上下左右操控一條不停前進的蛇\n　　，不斷撿取點數來獲得分數，並避免\n　　觸碰到自身及牆壁。每次貪食蛇吃掉\n　　一點，身體會便增長一點。\n\n\n\n　　　　　按　Ｅｎｔｅｒ　繼續　　" };

BOOL setConsoleCursorPosition(int x, int y)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);  // handle of Get stdout
	if (hOutput == INVALID_HANDLE_VALUE) return FALSE; // Can't get
	COORD cc;
	cc.X = (SHORT)x;
	cc.Y = (SHORT)y;
	return SetConsoleCursorPosition(hOutput, cc);   // set cursor
}
void refresh()
{
	int h = gameFieldHeight + 2 < defaultRow ? defaultRow : gameFieldHeight + 2;
	int w = gameFieldWidth + 2 < defaultColumn ? defaultColumn : gameFieldWidth + 2;
	for (short i = 0, j; i < h; i++)
	{
		setConsoleCursorPosition(0, i);
		for (j = 0; j < w; j++)
		{
			printf("　");
		}
	}
	setConsoleCursorPosition(0, 0);
}
enum consoleColorStyleEnum
{
	WhiteOnBlack,
	RedOnBlack,
	GreenOnBlack,
	YellowOnBlack,
	BlueOnBlack,
	MagentaOnBlack,
	CyanOnBlack,
	BlackOnGray,
	BlackOnWhite,
	RedOnWhite,
	GreenOnWhite,
	YellowOnWhite,
	BlueOnWhite,
	MagentaOnWhite,
	CyanOnWhite,
	WhiteOnWhite,
};
typedef enum consoleColorStyleEnum consoleColorStyleEnum;
void setColorStyle(consoleColorStyleEnum color) // 直接先給預設
{
	switch (color)
	{
	case 0:    // White on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 1:    // Red on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case 2:    // Green on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case 3:    // Yellow on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);
		break;
	case 4:    // Blue on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		break;
	case 5:    // Magenta on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_BLUE);
		break;
	case 6:    // Cyan on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_GREEN |
			FOREGROUND_BLUE);
		break;
	case 7:    // Black on Gray
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | BACKGROUND_INTENSITY);
		break;
	case 8:    // Black on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE);
		break;
	case 9:    // Red on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE |
			FOREGROUND_RED);
		break;
	case 10:    // Green on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_GREEN);
		break;
	case 11:    // Yellow on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_RED |
			FOREGROUND_GREEN);
		break;
	case 12:    // Blue on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_BLUE);
		break;
	case 13:    // Magenta on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_RED |
			FOREGROUND_BLUE);
		break;
	case 14:    // Cyan on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_GREEN |
			FOREGROUND_BLUE);
		break;
	case 15:    // White on White
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | FOREGROUND_INTENSITY |
			BACKGROUND_RED | BACKGROUND_GREEN |
			BACKGROUND_BLUE | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:    // White on Black
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
}
void showConsoleCursor(BOOL showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void clearConsole()
{
	system("CLS");
}

enum stageEnum { menuStage, gameOptionStage, gameStage, descriprionStage };
typedef enum stageEnum stageEnum;

void changeOption(stageEnum stage, char option)
{
	switch (stage)
	{
	case menuStage:
		for (int i = 0; i < 4; i++)
		{
			setConsoleCursorPosition(12, i * 2 + 7);
			printf("　");
			setConsoleCursorPosition(26, i * 2 + 7);
			printf("　");
		}
		setConsoleCursorPosition(12, option * 2 + 7);
		printf("＞");
		setConsoleCursorPosition(26, option * 2 + 7);
		printf("＜");
		break;
	case gameOptionStage:
		for (int i = 0; i < 4; i++)
		{
			setConsoleCursorPosition(12, i * 3 + 5);
			printf("　");
			setConsoleCursorPosition(26, i * 3 + 5);
			printf("　");
		}
		setConsoleCursorPosition(12, option * 3 + 5);
		printf("＞");
		setConsoleCursorPosition(26, option * 3 + 5);
		printf("＜");
		break;
	default:
		break;
	}
}

BOOL setConsoleCursorPositionByXY(int x, int y)
{
	return setConsoleCursorPosition(x * 2 + 2, y + 1);
}

struct fiveParameters
{
	void* parameter1;
	void* parameter2;
	void* parameter3;
	void* parameter4;
	void* parameter5;
};
typedef struct fiveParameters fiveParameters;

enum pixelUnitEnum { wall, head, body, point, air, first };
typedef enum pixelUnitEnum pixelUnitEnum;

int main()
{
	// Console window setting
	showConsoleCursor(false);
	SetConsoleTitle("Snake Game");
	setColorStyle(GreenOnBlack);

	// Stage thread
	pthread_t threadGUI;
	void* isExit;
	do
	{

		pthread_create(&threadGUI, NULL, &stage_Menu, NULL);
		pthread_join(threadGUI, &isExit);
		setConsoleCursorPosition(0, 0);
	} while (!(int)isExit);
	return 0;
}

void* stage_Menu()
{
	enum menuOptionEnum { optionStart, optionDescription, optionGameOption, optionExit };
	typedef enum menuOptionEnum menuOptionEnum;

	menuOptionEnum option = 0;

stage_Menu_Start:
	refresh();
	MoveWindow(GetConsoleWindow(), 100, 100, 35 + defaultColumn * 20.3, 50 + defaultRow * 22, TRUE);
	printf(menuStage_TitleString);
	printf(menuStage_OptionString);
	changeOption(menuStage, option);

	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case keyUp:
				option = option == 0 ? 3 : option - 1;
				changeOption(menuStage, option);
				break;
			case keyDown:
				option = (option + 1) % 4;
				changeOption(menuStage, option);
				break;
			case keyEnter:
			case keySpace:
				switch (option)
				{
				case optionStart:
					stage_Game();
					pthread_exit((void*)false);
					break;
				case optionDescription:
					stage_Description();
					goto stage_Menu_Start;
					break;
				case optionGameOption:
					stage_GameOption();
					goto stage_Menu_Start;
					break;
				case optionExit:
					pthread_exit((void*)true);
					break;
				}
				break;
			}
		}
	}
	pthread_exit((void*)false);
}

void stage_Game()
{
	// claim variable and initial
	VectorData headVector = { startPosition.X,startPosition.Y,right };
	VectorData tailVector = { startPosition.X - 3,startPosition.Y,right };
	VectorData tempVector = { 0,0,right };
	bool isTurn = false;
	bool isOver = false;
	int level = (gameFieldHeight - 2) * (gameFieldWidth - 2) - 4;
	pixelUnitEnum area[gameFieldHeight][gameFieldWidth];
	int delayTime = 160 - gameSpeed * 12;	// milliseconds
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	// temp variable
	VectorData* tempCornerData;
	int randomNum;
	int row;
	int column;

	// create cornerQueue
	Queue* cornerQueue;
	cornerQueue = malloc(sizeof(Queue));
	QueueInitialize(cornerQueue);

	// initial the game field and "area"
	refresh();
	MoveWindow(GetConsoleWindow(), 100, 100, 75 + gameFieldWidth * 20.3, 116 + gameFieldHeight * 22, TRUE);
	for (int i = 0, j; i < gameFieldHeight; i++)
	{
		for (j = 0; j < gameFieldWidth; j++)
		{
			area[i][j] = air;
		}
	}
	setConsoleCursorPositionByXY(0, 0);
	for (int i = 0, j; i < gameFieldWidth; i++)
	{
		printf("※");
		area[0][i] = wall;
	}
	setConsoleCursorPositionByXY(0, gameFieldHeight - 1);
	for (int i = 0; i < gameFieldWidth; i++)
	{
		printf("※");
		area[gameFieldHeight - 1][i] = wall;
	}
	for (int i = 0, j; i < gameFieldWidth; i = i + gameFieldWidth - 1)
	{
		for (j = 0; j < gameFieldHeight; j++)
		{
			setConsoleCursorPositionByXY(i, j);
			printf("※");
			area[j][i] = wall;
		}
	}
	setConsoleCursorPositionByXY(tailVector.X, tailVector.Y);
	printf("■■■□");
	area[tailVector.Y][tailVector.X] = body;
	area[tailVector.Y][tailVector.X + 1] = body;
	area[tailVector.Y][tailVector.X + 2] = body;
	area[headVector.Y][headVector.X] = first;

	fiveParameters parameterStruct =
	{
		(void*)& headVector,
		(void*)& tempVector,
		(void*)& isTurn,
		(void*)& mutex,
		(void*)& isOver
	};
	pthread_t threadKeyDetect;
	pthread_create(&threadKeyDetect, NULL, &keyDetect, (void*)& parameterStruct);

	Sleep(500);
	while (1)
	{
		Sleep(delayTime);

		switch (area[headVector.Y][headVector.X])
		{
		case first:
			setConsoleCursorPositionByXY(tailVector.X, tailVector.Y);
			printf("　");
			area[tailVector.Y][tailVector.X] = air;
			tailVector.X++;
		case point:
			randomNum = rand() % level;
			row = randomNum / (gameFieldWidth - 2) + 1;
			column = randomNum % (gameFieldWidth - 2) + 1;
			while (area[row][column] != air)
			{
				if (column == gameFieldWidth - 2)
				{
					column = 0;
					if (row == gameFieldHeight - 2)
					{
						row = 0;
					}
					else
					{
						row++;
					}
				}
				else
				{
					column++;
				}
			}
			setConsoleCursorPositionByXY(column, row);
			printf("◎");
			area[row][column] = point;
			level--;
			break;
		case head:
		case air:
			setConsoleCursorPositionByXY(tailVector.X, tailVector.Y);
			printf("　");
			area[tailVector.Y][tailVector.X] = air;

			if (!QueueIsempty(cornerQueue))
			{
				tempCornerData = QueuePick(cornerQueue);
				if (tailVector.X == tempCornerData->X)
				{
					if (tailVector.Y == tempCornerData->Y)
					{
						tailVector.Direct = Dequeue(cornerQueue).Direct;
					}
				}
			}

			switch (tailVector.Direct)
			{
			case up:
				tailVector.Y--;
				break;
			case down:
				tailVector.Y++;
				break;
			case left:
				tailVector.X--;
				break;
			case right:
				tailVector.X++;
				break;
			}
			break;
		case wall:
		case body:
			isOver = true;
			pthread_join(threadKeyDetect, NULL);
			area[headVector.Y][headVector.X] = head;
			Sleep(400);
			for (int i = 0; i < 10; i++)
			{
				if (i % 2 == 0)
				{
					setColorStyle(RedOnWhite);
				}
				else
				{
					setColorStyle(GreenOnBlack);
				}
				refresh();
				for (row = 0; row < gameFieldHeight; row++)
				{
					setConsoleCursorPosition(2, row + 1);
					for (column = 0; column < gameFieldWidth; column++)
					{
						switch (area[row][column])
						{
						case point:
							printf("◎");
							break;
						case head:
							printf("□");
							break;
						case body:
							printf("■");
							break;
						case wall:
							printf("※");
							break;
						case air:
							printf("　");
							break;
						}
					}
				}
				Sleep(120);
			}
			stage_GameOver(level);
			pthread_exit(NULL);
			break;
		}

		setConsoleCursorPositionByXY(headVector.X, headVector.Y);
		printf("■");
		area[headVector.Y][headVector.X] = body;

		pthread_mutex_lock(&mutex);
		if (isTurn == true)
		{
			Enqueue(cornerQueue, tempVector);
			headVector = tempVector;
			isTurn = false;
		}
		pthread_mutex_unlock(&mutex);

		switch (headVector.Direct)
		{
		case up:
			headVector.Y--;
			break;
		case down:
			headVector.Y++;
			break;
		case left:
			headVector.X--;
			break;
		case right:
			headVector.X++;
			break;
		}
		setConsoleCursorPositionByXY(headVector.X, headVector.Y);
		printf("□");
	}
}

void stage_Description()
{
	refresh();
	setConsoleCursorPosition(0, 0);
	printf(descriptionStage_DescriptionString);
	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case keyEnter:
			case keySpace:
				return;
				break;
			}
		}
	}
}

void stage_GameOption()
{
	enum optionEnum { optionGameSpeed, optionGameFieldWidth, optionGameFieldHeight, optionBack };
	typedef enum optionEnum optionEnum;

	char option = 0;
	refresh();
	setConsoleCursorPosition(0, 0);
	printf("\n\n　　　－－－－　　選項　　－－－－\n\n　　　　　　　　遊戲速度　　\n　　　　　　　　   %2d 　　　\n\n　　　　　　　　場地寬度　　　\n　　　　　　　　   %2d 　　　\n\n　　　　　　　　場地高度　　　\n　　　　　　　　   %2d 　　　\n\n\n　　　　　　　　回上一頁　　　\n"
		, gameSpeed, gameFieldWidth - 2, gameFieldHeight - 2);
	changeOption(gameOptionStage, option);

	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case keyUp:
				option = option == 0 ? 3 : option - 1;
				changeOption(gameOptionStage, option);
				break;
			case keyDown:
				option = (option + 1) % 4;
				changeOption(gameOptionStage, option);
				break;
			case keyLeft:
				switch (option)
				{
				case optionGameSpeed:
					gameSpeed = gameSpeed > minGameSpeed ? gameSpeed - 1 : minGameSpeed;
					setConsoleCursorPosition(19, 5);
					printf("%2d", gameSpeed);
					break;
				case optionGameFieldWidth:
					gameFieldWidth = gameFieldWidth > minFieldWidth ? gameFieldWidth - 1 : minFieldWidth;
					setConsoleCursorPosition(19, 8);
					printf("%2d", gameFieldWidth - 2);
					break;
				case optionGameFieldHeight:
					gameFieldHeight = gameFieldHeight > minFieldHeight ? gameFieldHeight - 1 : minFieldHeight;
					setConsoleCursorPosition(19, 11);
					printf("%2d", gameFieldHeight - 2);
					break;
				}
				break;
			case keyRight:
				switch (option)
				{
				case optionGameSpeed:
					gameSpeed = gameSpeed < maxGameSpeed ? gameSpeed + 1 : maxGameSpeed;
					setConsoleCursorPosition(19, 5);
					printf("%2d", gameSpeed);
					break;
				case optionGameFieldWidth:
					gameFieldWidth = gameFieldWidth < maxFieldWidth ? gameFieldWidth + 1 : maxFieldWidth;
					setConsoleCursorPosition(19, 8);
					printf("%2d", gameFieldWidth - 2);
					break;
				case optionGameFieldHeight:
					gameFieldHeight = gameFieldHeight < maxFieldHeight ? gameFieldHeight + 1 : maxFieldHeight;
					setConsoleCursorPosition(19, 11);
					printf("%2d", gameFieldHeight - 2);
					break;
				}
				break;
			case keyEnter:
			case keySpace:
				if (option == optionBack)
				{
					return;
				}
				break;
			}
		}
	}
}

void stage_GameOver(int level)
{
	refresh();
	MoveWindow(GetConsoleWindow(), 100, 100, 34 + defaultColumn * 20, 50 + defaultRow * 22, TRUE);
	setConsoleCursorPosition(0, 0);
	printf("\n\n　　　　－－－－遊戲結束－－－－　\n\n　　　　　　遊戲速度：%2d　　\n\n　　　　　　場地寬度：%2d　　\n\n　　　　　　場地高度：%2d　　\n\n　　　　　　　　總分：%2d　　\n\n\n\n　　　　　按　Ｅｎｔｅｒ　繼續　　"
		, gameSpeed, gameFieldWidth - 2, gameFieldHeight - 2, (gameFieldHeight - 2) * (gameFieldWidth - 2) - 5 - level);
	while (1)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case keyEnter:
			case keySpace:
				return;
				break;
			}
		}
	}
}

void* keyDetect(void* parameter)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	fiveParameters* parameters = (fiveParameters*)parameter;
	VectorData* headVector = (VectorData*)parameters->parameter1;
	VectorData* tempVector = (VectorData*)parameters->parameter2;
	bool* isTurn = (bool*)parameters->parameter3;
	pthread_mutex_t* mutex = (pthread_mutex_t*)parameters->parameter4;
	bool* isOver = (bool*)parameters->parameter5;

	while (1)
	{
		if (*isOver == true)
			pthread_exit(NULL);
		if (_kbhit())
		{
			switch (_getch())
			{
			case keyUp:
				if (headVector->Direct != up && headVector->Direct != down)
				{
					*tempVector = *headVector;
					(*tempVector).Direct = up;
					pthread_mutex_lock(mutex);
					*isTurn = true;
					pthread_mutex_unlock(mutex);
				}
				break;
			case keyDown:
				if (headVector->Direct != up && headVector->Direct != down)
				{
					*tempVector = *headVector;
					(*tempVector).Direct = down;
					pthread_mutex_lock(mutex);
					*isTurn = true;
					pthread_mutex_unlock(mutex);
				}
				break;
			case keyLeft:
				if (headVector->Direct != left && headVector->Direct != right)
				{
					*tempVector = *headVector;
					(*tempVector).Direct = left;
					pthread_mutex_lock(mutex);
					*isTurn = true;
					pthread_mutex_unlock(mutex);
				}
				break;
			case keyRight:
				if (headVector->Direct != left && headVector->Direct != right)
				{
					*tempVector = *headVector;
					(*tempVector).Direct = right;
					pthread_mutex_lock(mutex);
					*isTurn = true;
					pthread_mutex_unlock(mutex);
				}
				break;
			}
		}
	}
}