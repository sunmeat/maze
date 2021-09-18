#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>
using namespace std;

#pragma warning (disable:4996) 

// enumeration (перечисление - набор целочисленных констант)
// MazeObjects - пользовательский (кастомизированный) тип данных
enum MazeObjects { HALL, WALL, COIN, ENEMY, BORDER };
enum Colors { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4 };
enum KeyCodes { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

int main()
{

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font 
	cfi.dwFontSize.Y = 72;                  // Height 
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font 
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	//std::cout << "Font: Consolas, Size: 24\n";

	system("title Maze");
	MoveWindow(GetConsoleWindow(), 20, 60, 1850, 900, true);
	// 50 - отступ слева от левой границы рабочего стола до левой границы окна консоли (в пикселях!)
	// 60 - отступ сверху от верхней границы РС до верхней границы окна консоли
	// 1700 - ширина окна консоли в пикселях
	// 1000 - высота окна консоли

	srand(time(0));
	rand();

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	const int width = 40; // ширина лабиринта
	const int height = 10; // высота лабиринта

	int maze[height][width]{}; // maze - лабиринт по английски

	// алгоритм заполнения массива
	for (int y = 0; y < height; y++) // перебор строк
	{
		for (int x = 0; x < width; x++) // перебор столбцов
		{
			maze[y][x] = rand() % 4; // 4 типа объектов в игре



			if (maze[y][x] == MazeObjects::ENEMY) // если в лабиринте сгенерился враг
			{
				int probability = rand() % 10; // 0...4, если выпало 0 - враг останется 01234 останется только одна пятая часть врагов
				if (probability != 0) // убираем врага
				{
					maze[y][x] = MazeObjects::HALL; // на место врага ставим коридор
				}
			}

			if (maze[y][x] == MazeObjects::WALL) // если в лабиринте сгенерилась стена
			{
				int probability = rand() % 2; // 0...4, если выпало 0 - враг останется 01234 останется только одна пятая часть врагов
				if (probability != 0) // убираем стену
				{
					maze[y][x] = MazeObjects::HALL; // на место стены ставим коридор
				}
			}

			if (x == 0 || y == 0 || x == width - 1 || y == height - 1) maze[y][x] = MazeObjects::BORDER;

			if (x == 0 && y == 2 ||
				x == 1 && y == 2 ||
				x == 2 && y == 2) maze[y][x] = MazeObjects::HALL; // вход

			if (x == width - 1 && y == height - 3 ||
				x == width - 2 && y == height - 3 ||
				x == width - 3 && y == height - 3
				) maze[y][x] = MazeObjects::HALL; // выход

		}
	}

	// показ лабиринта
	for (int y = 0; y < height; y++) // перебор строк
	{
		for (int x = 0; x < width; x++) // перебор столбцов
		{
			switch (maze[y][x])
			{

			case MazeObjects::HALL: // hall - коридор
				cout << " ";
				break;

			case MazeObjects::WALL: // wall - стена
				SetConsoleTextAttribute(h, Colors::DARKGREEN);
				cout << (char)178;
				break;

			case MazeObjects::BORDER: // border - рамка
				SetConsoleTextAttribute(h, Colors::WHITE);
				cout << (char)178;
				break;

			case MazeObjects::COIN: // coin - монетка
				SetConsoleTextAttribute(h, Colors::YELLOW);
				cout << ".";
				break;

			case MazeObjects::ENEMY: // enemy - враг
				SetConsoleTextAttribute(h, Colors::RED);
				cout << (char)1;
				break;
			}
		}
		cout << "\n";
	}

	/////////////////////////////////////////////////////////////////////
	// скрытие мигающего курсора 
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false; // спрятать курсор
	cursor.dwSize = 1; // 1...100
	SetConsoleCursorInfo(h, &cursor);

	// размещение главного героя (ГГ)
	COORD position; // координаты нашего персонажа
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	//cout << "@";
	SetConsoleTextAttribute(h, Colors::BLUE);
	cout << (char)2;

	int coins = 0; // счётчик собранных монет
	int health = 100; // количество очков здоровья главного героя

	/////////////////////////////////////////////////////////////////////
	// информация по всем показателям
	COORD infobox;
	infobox.X = width + 1;
	infobox.Y = 1;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Colors::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Colors::YELLOW);
	cout << coins << "\n"; // 0

	//infobox.X = width + 1;
	infobox.Y = 2;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Colors::DARKRED);
	cout << "HEALTH: ";
	SetConsoleTextAttribute(h, Colors::RED);
	cout << health << "\n";



	while (true)
	{

		if (_kbhit()) // если было нажатие на клавиши пользователем
		{
			int code = _getch(); // get character получение кода нажатой клавиши
			if (code == 224) { // если это стрелка
				code = _getch(); // получить конкретный код стрелки
			}

			// стриание персонажика в старой позиции
			SetConsoleCursorPosition(h, position);
			cout << " ";

			// cout << code << "\n"; // отладочный сиаут для других клавиш кроме перечисленных ниже

			if (code == KeyCodes::ENTER)
			{
				cout << "ENTER!\n";
				system("cls");
				exit(0);
			}
			else if (code == KeyCodes::ESCAPE)
			{
				cout << "ESCAPE!\n";
				system("cls");
				exit(0);
			}
			else if (code == KeyCodes::SPACE)
			{
				cout << "SPACE!\n";
				system("cls");
				exit(0);
			}
			else if (code == KeyCodes::LEFT
				&& maze[position.Y][position.X - 1] != MazeObjects::WALL
				&& maze[position.Y][position.X - 1] != MazeObjects::BORDER)
			{
				//cout << "LEFT!\n";
				position.X--;
			}
			else if (code == KeyCodes::RIGHT // если я собрался пойти направо
				&& maze[position.Y][position.X + 1] != MazeObjects::WALL
				&& maze[position.Y][position.X + 1] != MazeObjects::BORDER)
				// и при этом в лабиринте на той же строке (где смайлик) и
				// немного (на одну ячейку) правее на 1 столбец от смайлика
			{
				// cout << "RIGHT!\n";
				position.X++;
			}
			else if (code == KeyCodes::UP
				&& maze[position.Y - 1][position.X] != MazeObjects::WALL
				&& maze[position.Y - 1][position.X] != MazeObjects::BORDER)
			{
				//cout << "UP!\n";
				position.Y--;
			}
			else if (code == KeyCodes::DOWN
				&& maze[position.Y + 1][position.X] != MazeObjects::WALL
				&& maze[position.Y + 1][position.X] != MazeObjects::BORDER)
			{
				//cout << "DOWN!\n";
				position.Y++;
			}

			// показ персонажика в новой позиции
			SetConsoleCursorPosition(h, position);
			SetConsoleTextAttribute(h, Colors::BLUE);
			cout << (char)2;

			////////////////////////////////////////////////////////////////
			// пересечение с элементами массива
			if (position.Y == height - 3 &&
				position.X == width - 1)
			{
				MessageBoxA(0, "найден выход из лабиринта!", "ПОБЕДА!", 0);
				system("cls");
				exit(0);
				//main(); // для запуска сначала но на другой рандомной локации
			}

			// пересечение с монетками
			// если в лабиринте по позиции смайлика (под ним) находится монетка
			if (maze[position.Y][position.X] == MazeObjects::COIN)
			{
				coins++; // на одну монетку собрали больше
				//cout << coins << "\n"; // показ количества
				COORD info;
				info.X = width + 1;
				info.Y = 1;
				SetConsoleCursorPosition(h, info);
				SetConsoleTextAttribute(h, Colors::DARKYELLOW);
				cout << "COINS: ";
				SetConsoleTextAttribute(h, Colors::YELLOW);
				cout << coins << "\n";
				maze[position.Y][position.X] = MazeObjects::HALL; // убираем монетку из лабиринта
			}

			// пересечение с врагами
			// если в лабиринте по позиции смайлика (под ним) находится враг
			if (maze[position.Y][position.X] == MazeObjects::ENEMY)
			{
				health -= 20;
				COORD info;
				info.X = width + 1;
				info.Y = 2;
				SetConsoleCursorPosition(h, info);
				SetConsoleTextAttribute(h, Colors::DARKRED);
				cout << "HEALTH: ";
				SetConsoleTextAttribute(h, Colors::RED);
				cout << health << " \n";
				maze[position.Y][position.X] = MazeObjects::HALL; // убираем монетку из лабиринта

				if (health <= 0)
				{
					int answer = MessageBoxA(0, "здоровье закончилось!\n\nхотите начать сначала?", "ПОРАЖЕНИЕ!", MB_YESNO);
					system("cls");
					if (answer == IDYES)
					{
						main();
					}
					else
					{
						exit(0);
					}


				}
			}
		}
		else // нажатия не было, двигаем врагов
		{
			Sleep(15);
			///////////////////////////////////////////////////////////////
			// движение врагов
			COORD enemy_positions[100]; // массив который хранит координаты врагов
			int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ

			// перебор всех ячеек которые есть в лабиринте
			for (int y = 0; y < height; y++) // перебор строк
			{
				for (int x = 0; x < width; x++) // перебор столбцов
				{
					// если очередная ячейка - это враг
					if (maze[y][x] == MazeObjects::ENEMY)
					{
						enemy_positions[enemy_count].X = x;
						enemy_positions[enemy_count].Y = y;
						enemy_count++;
					}
				}
			}

			// cout << enemy_count << "\n";

			// перебор всех врагов
			for (int i = 0; i < enemy_count; i++)
			{
				int r = rand() % 100; // 0-left 1-right 2-up 3-down
				if (r == 0 &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObjects::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObjects::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObjects::ENEMY) // left
				{
					// стирание врага в старой позиции
					COORD temp;
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObjects::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X - 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Colors::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObjects::ENEMY;
				}
				else if (r == 1 && // right
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObjects::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObjects::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObjects::ENEMY)
				{
					// стирание врага в старой позиции
					COORD temp;
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObjects::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X + 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Colors::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObjects::ENEMY;
				}

				else if (r == 2 && // up
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObjects::WALL &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObjects::BORDER &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObjects::ENEMY)
				{
					// стирание врага в старой позиции
					COORD temp;
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObjects::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y - 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Colors::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObjects::ENEMY;
				}
				else if (r == 3 && // down
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObjects::WALL &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObjects::BORDER &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObjects::ENEMY)
				{
					// стирание врага в старой позиции
					COORD temp;
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObjects::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y + 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Colors::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObjects::ENEMY;
				}
			}
		}
	}

}
