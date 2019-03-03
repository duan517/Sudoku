#include <iostream>
#include <iomanip>
#include <string.h>

#define COLS 9
#define ROWS 9
#define BLOCK_COLS 3
#define BLOCK_ROWS 3

using namespace std;

void PrintMatrix(int *matrix)
{
	cout << setw(2) << " ";
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

inline bool IsComplete(int *matrix)
{
	for (int i = 0; i < COLS * ROWS; i++)
	{
		if (*(matrix + i) == 0)
		{
			return false;
		}
	}
	return true;
}

bool TryNext(int *matrix, int offset = 0)
{
	for (int i = offset; i < COLS * ROWS; i++)
	{
		if (*(matrix + i) == 0)
		{
			for (int n = 1; n <= COLS; n++)
			{
				bool exists = false;

				for (int x = i / COLS * COLS; !exists && x < (i / COLS + 1) * COLS; x++)
				{
					if (*(matrix + x) == n)
					{
						exists = true;
					}
				}

				for (int y = i % COLS; !exists && y < COLS * ROWS; y += COLS)
				{
					if (*(matrix + y) == n)
					{
						exists = true;
					}
				}

				int delta = i / (COLS * BLOCK_ROWS) * (COLS * BLOCK_ROWS) + i % COLS / BLOCK_COLS * BLOCK_COLS;
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
					if (!TryNext(matrix, i + 1))
					{
						if (IsComplete(matrix))
						{
							return true;
						}
					}
					else
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

int main(int argc, char *argv[])
{
	int matrix[COLS * ROWS] = { 0 };

	cout << "Sudoku Solver" << endl << "Version: 1.0.0, DfSoft, 2019-03-02" << endl;
	cout << endl << "Please enter sudoku puzzle numbers:" << endl;
	DisplayMenu();

	char buffer[COLS * ROWS + 1];
	for (int i = 0; i < COLS * ROWS;)
	{
		cout << (char)(i / COLS + 'A') << i % COLS + 1 << ">";
		cin.getline(buffer, sizeof(buffer));
		for (int n = 0; n < sizeof(buffer) && buffer[n] != '\0'; n++)
		{
			if (buffer[n] >= '0' && buffer[n] <= '9')
			{
				matrix[i++] = buffer[n] - '0';
			}
			else if (buffer[n] == 'p' || buffer[n] == 'P')
			{
				PrintMatrix(matrix);
			}
			else if (buffer[n] == 'c' || buffer[n] == 'C')
			{
				memset(matrix + (i / COLS * COLS), 0, sizeof(int) * COLS);
				i = i / COLS * COLS;
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
	}

	if (TryNext(matrix))
	{
		cout << endl << "Solution: " << endl;
		PrintMatrix(matrix);
	}
	else
	{
		cout << "No solution found!" << endl;
		return 1;
	}

	return 0;
}