#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>

using namespace std;

#pragma region COLOR DEFINE
#define COLOR_BLACK      0
#define COLOR_DARK_BLUE  1
#define COLOR_DARK_GREEN 2
#define COLOR_LIGHT_BLUE 3
#define COLOR_DARK_RED   4
#define COLOR_MAGENTA    5
#define COLOR_ORANGE     6
#define COLOR_LIGHT_GRAY 7
#define COLOR_GRAY       8
#define COLOR_BLUE       9
#define COLOR_GREEN     10
#define COLOR_CYAN      11
#define COLOR_RED       12
#define COLOR_PINK      13
#define COLOR_YELLOW    14
#define COLOR_WHITE     15
#pragma endregion 

struct Cell
{
	bool isOpened = false;

	bool isMine = false;
	bool isFlagged = false;
	bool isUndefined = false;
};

struct Vector2
{
	int x, y;
};

HANDLE _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Vector2 _mapSize;

Cell** _map;

Vector2 _cursorPosition = {0,0};

//Sets up map size and returns mines count
int MainMenu()
{
	printf("--> New game");
	system("pause > null");

	bool wrongInputFlag = true;

	do
	{
		system("cls");
		printf("Input map size: ");

		scanf_s("%i%i", &_mapSize.x, &_mapSize.y);
		if (_mapSize.x >= 10 && _mapSize.x <= 85 && _mapSize.y >= 10 && _mapSize.y <= 25) {
			wrongInputFlag = false;
		} else
		{
			printf("Wrong map size! ");

			if (_mapSize.x < 10 || _mapSize.y < 10)
				printf("Map must be at least 10x10\n");
			else
				printf("Map must be smaller than 85x25\n");

			system("pause > null");
		}
	} while (wrongInputFlag);

	wrongInputFlag = true;
	int mineCount = 0;

	do
	{
		system("cls");
		printf("Input mines count: ");

		scanf_s("%i", &mineCount);

		if (mineCount >= 10 && mineCount <= (_mapSize.x * _mapSize.y * 0.25)) 
		{
			system("cls");
			return mineCount;
		} else
		{
			if (mineCount < 10)
				printf("Think bigger!\n");
			else
				printf("Oh you think you will survive that? No way! Go easy on yourself!\n");
			system("pause > null");
		}

	} while (wrongInputFlag);

	return 0;
}

//Generates map and puts mines in random places
void GenerateMap(int mineCount)
{
	//Create map array full of cells 
	_map = new Cell * [_mapSize.x];
	for (int i = 0; i < _mapSize.x; i++)
		_map[i] = new Cell[_mapSize.y];

	//Generate mines
	int generatedMines = 0;
	do
	{
		Vector2 pos = { rand() % _mapSize.x, rand() % _mapSize.y };
		if (!_map[pos.x][pos.y].isMine && (_map[pos.x][pos.y].isMine = true))
			generatedMines++;

	} while (generatedMines < mineCount);
}

//Calculates adjascent mines around given cell
int CalculateAdjascentMines(Vector2 pos)
{
	int adjascentMines = 0;
	if (pos.x > 0 && _map[pos.x - 1][pos.y].isMine) adjascentMines++;
	if (pos.x > 0 && pos.y > 0 && _map[pos.x - 1][pos.y - 1].isMine) adjascentMines++;
	if (pos.x > 0 && pos.y < _mapSize.y - 1 && _map[pos.x - 1][pos.y + 1].isMine) adjascentMines++;
	if (pos.y > 0 && _map[pos.x][pos.y - 1].isMine) adjascentMines++;
	if (pos.x < _mapSize.x - 1 && _map[pos.x + 1][pos.y].isMine) adjascentMines++;
	if (pos.y < _mapSize.y - 1 && _map[pos.x][pos.y + 1].isMine) adjascentMines++;
	if (pos.x < _mapSize.x - 1 && pos.y < _mapSize.y - 1 && _map[pos.x + 1][pos.y + 1].isMine) adjascentMines++;
	if (pos.x < _mapSize.x - 1 && pos.y > 0 && _map[pos.x + 1][pos.y - 1].isMine) adjascentMines++;

	return adjascentMines;
} 

void PrintMap()
{
	for (int i = 0; i < _mapSize.x; i++) {
		for (int j = 0; j < _mapSize.y; j++)
		{
			SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_WHITE);
			if (_cursorPosition.x == i && _cursorPosition.y == j)
				SetConsoleTextAttribute(_hConsole, COLOR_WHITE * 16 + COLOR_BLACK);

			if (_map[i][j].isOpened)
			{
				if (_map[i][j].isMine)
					printf("*");
				else
				{
					switch (CalculateAdjascentMines({i,j}))
					{
					case 1:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_GREEN);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_GREEN);
						printf("1");
						break;
					case 2:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_DARK_GREEN);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_DARK_GREEN);
						printf("2");
						break;
					case 3:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_LIGHT_BLUE);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_LIGHT_BLUE);
						printf("3");
						break;
					case 4:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_BLUE);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_BLUE);
						printf("4");
						break;
					case 5:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_DARK_BLUE);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_DARK_BLUE);
						printf("5");
						break;
					case 6:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_CYAN);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_CYAN);
						printf("6");
						break;
					case 7:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_RED);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_RED);
						printf("7");
						break;
					case 8:
						SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_DARK_RED);
						if (_cursorPosition.x == i && _cursorPosition.y == j)
							SetConsoleTextAttribute(_hConsole, COLOR_LIGHT_GRAY * 16 + COLOR_DARK_RED);
						printf("8");
						break;

					default:
						printf(" ");
					}
				}
				continue;
			}
			if (_map[i][j].isUndefined)
			{
				printf("?");
				continue;
			}
			if (_map[i][j].isFlagged)
			{
				printf("!");
				continue;
			}
			printf(".");
		}
		SetConsoleTextAttribute(_hConsole, COLOR_BLACK * 16 + COLOR_WHITE);
		printf("\n");
	}
}

//Opens all cells connected to given one
void OpenCells(Vector2 pos)
{
	if (_map[pos.x][pos.y].isMine)
		return;

	if (_map[pos.x][pos.y].isOpened)
		return;

	if (CalculateAdjascentMines({ pos.x, pos.y }) > 0)
	{
		_map[pos.x][pos.y].isOpened = true;
		return;
	}
	else
		_map[pos.x][pos.y].isOpened = true;

	if (pos.x > 0) OpenCells({pos.x - 1, pos.y});
	if (pos.x < _mapSize.x - 1) OpenCells({pos.x + 1, pos.y});
	if (pos.y > 0) OpenCells({pos.x, pos.y - 1});
	if (pos.y < _mapSize.y) OpenCells({pos.x, pos.y + 1});
	if (pos.x > 0 && pos.y > 0) OpenCells({ pos.x - 1, pos.y - 1 });
	if (pos.x > 0 && pos.y < _mapSize.y - 1) OpenCells({ pos.x - 1, pos.y + 1 });
	if (pos.x < _mapSize.x - 1 && pos.y > 0) OpenCells({ pos.x + 1, pos.y - 1 });
	if (pos.x < _mapSize.x - 1 && pos.y < _mapSize.y - 1) OpenCells({ pos.x + 1, pos.y + 1 });
}

//Scans and handles all input
int ManageInput()
{
	int input;
	input = _getch();

	if (input == 224) //Arrows
	{
		input = _getch();
		switch (input) {
		case 72: //UP
			if (_cursorPosition.x > 0) 
				_cursorPosition.x--; 
			break;
		case 80: //DOWN
			if (_cursorPosition.x < _mapSize.x - 1) 
				_cursorPosition.x++; 
			break;
		case 75: //RIGHT
			if (_cursorPosition.y > 0)
				_cursorPosition.y--;
			else if (_cursorPosition.x > 0)
			{
				_cursorPosition.y = _mapSize.y - 1;
				_cursorPosition.x--;
			}
			break;
		case 77: //LEFT 

			if (_cursorPosition.y < _mapSize.y - 1)
				_cursorPosition.y++;
			else if (_cursorPosition.x < _mapSize.x - 1)
			{
				_cursorPosition.y = 0;
				_cursorPosition.x++;
			}

			break;
		}
		return 0;
	}

	if (input == 8 || input == 46) {
		_map[_cursorPosition.x][_cursorPosition.y].isUndefined = false;
		_map[_cursorPosition.x][_cursorPosition.y].isFlagged = false;
		return 0;
	}

	if (input == 9) {
		if (_map[_cursorPosition.x][_cursorPosition.y].isUndefined) _map[_cursorPosition.x][_cursorPosition.y].isUndefined = false;
		_map[_cursorPosition.x][_cursorPosition.y].isFlagged = !_map[_cursorPosition.x][_cursorPosition.y].isFlagged;
		return 0;
	}

	if (input == 13 && !_map[_cursorPosition.x][_cursorPosition.y].isFlagged) {
		if (!_map[_cursorPosition.x][_cursorPosition.y].isMine && CalculateAdjascentMines(_cursorPosition) == 0)
			OpenCells(_cursorPosition);
		else
		{
			_map[_cursorPosition.x][_cursorPosition.y].isOpened = true;
			if (_map[_cursorPosition.x][_cursorPosition.y].isMine)
				return 2;
		}
		return 0;
	}

	if (input == 27) {
		return 1;
	}

	switch (char(input)) {
	case 'q': case 'Q': 
		if (_map[_cursorPosition.x][_cursorPosition.y].isFlagged) _map[_cursorPosition.x][_cursorPosition.y].isFlagged = false;
		_map[_cursorPosition.x][_cursorPosition.y].isUndefined = !_map[_cursorPosition.x][_cursorPosition.y].isUndefined; break;
	}
	return 0;
}

//Chacks game for a win and returns 1 if won
int CheckWin()
{
	for (int i = 0; i < _mapSize.x; i++)
		for (int j = 0; j < _mapSize.y; j++)
			if (!_map[i][j].isOpened && !_map[i][j].isMine)
				return 0;
	return 1;
}

void SaveGame(int mineCount)
{
	ofstream saveFile("save.bin", ios::out | ios::binary);
	if (!saveFile)
	{
		printf("Save file was not opened!\n");
		return;
	}

	saveFile.write((char*)&_mapSize, sizeof(Vector2));
	saveFile.write((char*)&mineCount, sizeof(int));
	for (int i = 0; i < _mapSize.x; i++)
		for (int j = 0; j < _mapSize.y; j++)
			saveFile.write((char*)&_map[i][j], sizeof(Cell));

	saveFile.close();

	if (!saveFile.good())
		printf("Error occured at writing to save file\n");
}

void LoadGame()
{
	ifstream saveFile("save.bin", ios::out | ios::binary);
	if (!saveFile)
	{
		printf("Save file was not opened!\n");
		return;
	}

	int mineCount = 0;

	saveFile.read((char*)&_mapSize, sizeof(Vector2));
	saveFile.read((char*)&mineCount, sizeof(int));
	
	//Generate map
	_map = new Cell * [_mapSize.x];
	for (int i = 0; i < _mapSize.x; i++)
		_map[i] = new Cell[_mapSize.y];
	
	//Read cells info
	for (int i = 0; i < _mapSize.x; i++)
		for (int j = 0; j < _mapSize.y; j++)
			saveFile.read((char*)&_map[i][j], sizeof(Cell));

	if (!saveFile.good())
		printf("Error occured at reading save file\n");
}

int main()
{
	bool isPlaying = true;
	int managerResult;

	//Program loop
	while (true)
	{
		int mineCount = MainMenu();
		GenerateMap(mineCount);

		isPlaying = true;
		//Game loop
		while (isPlaying)
		{
			system("cls");
			PrintMap();

			managerResult = ManageInput();

			switch (managerResult)
			{
			case 1:
				return 0;
			case 2:
				isPlaying = false;
				printf("You blew up! Be careful next time!\n");
				system("pause > null");
				break;
			}

			if (CheckWin())
			{
				isPlaying = false;
				printf("You won! Good job boss man!\n");
				system("pause > null");
			}
		}
	}
	return 0;
}