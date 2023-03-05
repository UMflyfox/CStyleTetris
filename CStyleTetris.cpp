#include <iostream>
#include <iomanip>
#include <random>
#include <windows.h>
#include <chrono>
#include <thread>
#include <conio.h>

using namespace std;
constexpr auto SCREEN_X = 10;
constexpr auto SCREEN_Y = 20;

int screen[SCREEN_X][SCREEN_Y];
size_t score = 0;

int get_random(int min, int max)
{
	random_device rand;
	mt19937 seed(rand());

	uniform_int_distribution<> distrib(min, max);

	return (distrib(seed));
}

void display() 
{
	std::cout << "       TETRIS\n Score: " << setw(6) << score << endl;
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (screen[j][i] == 0)
				cout << "□";
			else
				cout << "■";
		}
		cout << endl;
	}
}

bool is_no_block() 
{
	for (size_t y = 0; y < SCREEN_Y; y++)
	{
		for (size_t x = 0; x < SCREEN_X; x++)
		{
			if (screen[x][y] == 2)
				return false;
		}
	}
	return true;
}

void new_block()
{
	int type = get_random(1, 3);

	switch (type)
	{
	default:
		break;
	case 1:
		screen[SCREEN_X / 2 - 1][0] = 2;
		screen[SCREEN_X / 2 - 1][1] = 2;
		screen[SCREEN_X / 2][0] = 2;
		screen[SCREEN_X / 2][1] = 2;
		break;
	case 2:
		screen[SCREEN_X / 2 - 2][0] = 2;
		screen[SCREEN_X / 2 - 1][0] = 2;
		screen[SCREEN_X / 2][0] = 2;
		screen[SCREEN_X / 2 + 1][0] = 2;
		break;
	case 3:
		screen[SCREEN_X / 2 - 2][0] = 2;
		screen[SCREEN_X / 2 - 1][0] = 2;
		screen[SCREEN_X / 2 - 1][1] = 2;
		screen[SCREEN_X / 2][1] = 2;
		break;
	}
}

bool is_overleap() 
{
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		if (screen[i][SCREEN_Y - 1] == 2)
			return true;
	}
	for (int y = SCREEN_Y - 1; y > 0; y--)
	{
		for (int x = SCREEN_X; x > 0; x--)
		{
			if (screen[x-1][y-1] == 2 && screen[x - 1][y] == 1) 
				return true;	
		}
	}
	return false;
}

void block_go_down()
{
	if (!is_overleap())
	{
		for (int y = SCREEN_Y - 1; y > 0; y--)
		{
			for (int x = SCREEN_X; x > 0; x--)
			{
				if (screen[x-1][y-1] == 2) {
					screen[x-1][y] = 2;
					screen[x-1][y-1] = 0;
				}
			}
		}
	}
	else 
	{
		for (int y = SCREEN_Y - 1; y > 0; y--)
		{
			for (int x = SCREEN_X; x > 0; x--)
			{
				if (screen[x-1][y-1] == 2) {
					screen[x-1][y-1] = 1;
				}
			}
		}
	}
}

bool check_by_line(int line)
{
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		if (screen[i][line] == 0)
			return false;
	}
	return true;
}

void clean_line(int line)
{
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		screen[i][line] = 0;
	}
	score += 1000;
}
/*
int go_down_to(int line)
{
	int min = SCREEN_Y;
	int tmp = line;
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		for (size_t j = line; j < SCREEN_Y; j++)
		{
			if (screen[i][j] > 0)
			{
				tmp = j;
				break;
			}
		}
		if (tmp < min)
			min = tmp;
	}
	return min;
}*/

int pixel_go_down(int x, int y) 
{
	for (size_t i = 0; i < SCREEN_Y - y; i++)
	{
		if (screen[x][y + i] == 1)
			return i;
	}
	return SCREEN_Y;
}

int go_down_to() 
{
	int temp = SCREEN_Y;
	int min = temp;
	for (size_t y = 0; y < SCREEN_Y; y++)
	{
		for (size_t x = 0; x < SCREEN_X; x++)
		{
			if (screen[x][y] == 2) {
				temp = pixel_go_down(x, y);
				if (temp < min)
					min = temp;
			}
		}
	}
	return (min - 1);
}

void down_one_by_line(int line)
{
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		screen[i][line] = screen[i][line - 1];
	}
}

void check()
{
	for (size_t i = 0; i < SCREEN_Y; i++)
	{
		if (check_by_line(i))
		{
			clean_line(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			//int where_stop = go_down_to(i);
			//for (size_t j = where_stop; j > 1; j--)
			for (int y = i; y > 1; y--)
			{
				down_one_by_line(y);
			}
		}
	}
}

bool left_boundary_check() {
	for (size_t i = 0; i < SCREEN_Y; i++)
	{
		if (screen[0][i] == 2)
			return true;
	}
	return false;
}

bool right_boundary_check() {
	for (size_t i = 0; i < SCREEN_Y; i++)
	{
		if (screen[SCREEN_X - 1][i] == 2)
			return true;
	}
	return false;
}

void handle_input()
{
	char input = _getch();
	int down;
	switch (input) {
	case 'a':
		if (!left_boundary_check()) 
		{
			for (size_t x = 1; x < SCREEN_X; x++)
			{
				for (size_t y = 0; y < SCREEN_Y; y++)
				{
					if (screen[x][y] == 2) {
						screen[x - 1][y] = 2;
						screen[x][y] = 0;
					}
				}
			}
		}
		break;
	case 'd':
		if (!right_boundary_check()) 
		{
			for (int x = SCREEN_X; x > 0; x--)
			{
				for (int y = SCREEN_Y; y > 0; y--)
				{
					if (screen[x - 1][y - 1] == 2) {
						screen[x][y - 1] = 2;
						screen[x - 1][y - 1] = 0;
					}
				}
			}
		}
		break;
	case 's':
		down = go_down_to();
		for (size_t y = 0; y < SCREEN_Y; y++)
		{
			for (size_t x = 0; x < SCREEN_X; x++)
			{
				if (screen[x][y] == 2) {
					screen[x][y + down] = 1;
					screen[x][y] = 0;
				}
			}
		}
		break;
	case 'p':
		score += 10000;
		break;
	case 'q':
		exit(0);
	}
}

void init()
{
	for (size_t i = 0; i < SCREEN_X; i++)
	{
		for (size_t j = 0; j < SCREEN_Y; j++)
		{
			screen[i][j] = 0;
		}
	}

	for (size_t i = 0; i < SCREEN_X; i++)
	{
		screen[i][19] = get_random(0, 1);
		screen[i][18] = get_random(0, 1);
		screen[i][17] = get_random(0, 1);
	}
}

bool is_game_over()
{
	for (size_t x = 0; x < SCREEN_X; x++)
	{
		if (screen[x][0] == 1)
			return true;
	}
	return false;
}

int main()
{
	string tmp;
	init();
	display();
	while (!is_game_over())
	{
		system("CLS");
		display();
		if (is_no_block())
			new_block();
		if (_kbhit())
		{
			handle_input();
		}
		block_go_down();
		check();
		this_thread::sleep_for(chrono::milliseconds(220));
		rewind(stdin);
	}
	cin >> tmp;
}