#include <iostream>
#include <iomanip>
#include <string.h>

#define COLS 9
#define ROWS 9
#define BLOCK_COLS 3
#define BLOCK_ROWS 3

using namespace std;

void PrintMatrix(int* matrix)
{
	cout << endl << setw(2) << " ";
	for (int i = 0; i < COLS; i++)
	{
		cout << setw(2) << i + 1;
	}
	cout << endl;

	for (int y = 0; y < ROWS; y++)
	{
		cout << setw(2) << (char)(y + 'A');
		for (int x = 0; x < COLS; x++)
		{
			char ch = *(matrix + COLS * y + x) + '0';
			cout << setw(2) << (ch == '0' ? '-' : ch);
		}
		cout << endl;
	}
}

bool TryNext(int* matrix, int offset = 0)
{
	for (int i = offset; i < COLS * ROWS; i++)
	{
		if (*(matrix + i) == 0)
		{
			// 逐个尝试所有可能的数字。
			for (int n = 1; n <= COLS; n++)
			{
				bool exists = false;

				// 检查所在行是否已经填过该数字了。
				for (int x = i / COLS * COLS; !exists && x < (i / COLS + 1) * COLS; x++)
				{
					if (*(matrix + x) == n)
					{
						exists = true;
					}
				}

				// 检查所在列是否已经填过该数字了。
				for (int y = i % COLS; !exists && y < COLS * ROWS; y += COLS)
				{
					if (*(matrix + y) == n)
					{
						exists = true;
					}
				}

				// 计算当前位置所在九宫格左上角的偏移量。
				int delta = i / (COLS * BLOCK_ROWS) * (COLS * BLOCK_ROWS) + i % COLS / BLOCK_COLS * BLOCK_COLS;
				// 检查所在九宫格是否已经填过该数字了。
				for (int y = 0; !exists && y < BLOCK_ROWS; y++)
				{
					for (int x = 0; !exists && x < BLOCK_COLS; x++)
					{
						if (*(matrix + delta + COLS * y + x) == n)
						{
							exists = true;
						}
					}
				}

				if (!exists)
				{
					*(matrix + i) = n;
					// 已经填了最后一个数，或者后续调用找到解，则返回成功标志。
					if (i == COLS * ROWS - 1 || TryNext(matrix, i + 1))
					{
						return true;
					}
					*(matrix + i) = 0;
				}
			}
			return false;
		}
	}
	return false;
}

void DisplayMenu()
{
	cout << "[0]Blank, [P]rint Matrix, [C]lear Line, [R]eset Matrix, [H]elp, [Q]uit" << endl;
}

int main(int argc, char* argv[])
{
	int matrix[COLS * ROWS] = { 0 };

	cout << "Sudoku Solver" << endl << "Version: 1.0.0, DfSoft, 2019-03-02" << endl;
	cout << endl << "Please enter sudoku puzzle numbers:" << endl;
	DisplayMenu();

	char buffer[COLS * ROWS + 1];
	for (int i = 0; i < COLS * ROWS && !cin.eof();)
	{
		cout << (char)(i / COLS + 'A') << i % COLS + 1 << ">";
		cin.getline(buffer, sizeof(buffer));
		for (int n = 0; n < sizeof(buffer) && buffer[n] != '\0'; n++)
		{
			if (buffer[n] >= '0' && buffer[n] <= '9')
			{
				if (i < COLS * ROWS)
				{
					matrix[i++] = buffer[n] - '0';
				}
			}
			else if (buffer[n] == 'p' || buffer[n] == 'P')
			{
				PrintMatrix(matrix);
			}
			else if (buffer[n] == 'c' || buffer[n] == 'C')
			{
				if (i < COLS * ROWS)
				{
					memset(matrix + (i / COLS * COLS), 0, sizeof(int) * COLS);
					i = i / COLS * COLS;
				}
			}
			else if (buffer[n] == 'r' || buffer[n] == 'R')
			{
				memset(matrix, 0, sizeof(matrix));
				i = 0;
			}
			else if (buffer[n] == 'h' || buffer[n] == 'H')
			{
				DisplayMenu();
			}
			else if (buffer[n] == 'q' || buffer[n] == 'Q')
			{
				return 0;
			}
		}

		if (i == COLS * ROWS)
		{
			if (TryNext(matrix))
			{
				PrintMatrix(matrix);
			}
			else
			{
				cout << "No solution!" << endl;
			}

			memset(matrix, 0, sizeof(matrix));
			i = 0;
		}
	}

	return 0;
}
