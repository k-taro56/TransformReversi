//#undef _MSC_VER
#ifdef _MSC_VER
#include <Windows.h>
#else
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include "console.h"

char disc_str[] = "-ox +";

char toc(enum disc p)
{
	if (WHITE == p)
	{
		return 'W';
	}
	if (BLACK == p)
	{
		return 'B';
	}
	return ' ';
}

void display(const int discs[SIZE], const struct board* board)
{
	printf("   ");

	for (char c = 'A'; c < 'A' + BOARD_COLUMN; c++)
	{
		printf(" %c", c);
	}

	printf("\n  1");

	int line = 2;
	for (int i = COLUMN; i < SIZE - COLUMN - 1; i++)
	{
		printf(" %c", *&disc_str[discs[i]]);
		if (i % COLUMN == COLUMN - 1)
		{
			printf("\n%3d", line++);
		}
	}

	if (NULL == board)
	{
		printf("\n");
	}
	else
	{
		printf("\n%d B: %d - %d :W\n%c >>", board->move_count, board->black_count, board->white_count, toc(board->player));
	}
}

#ifdef _MSC_VER
HANDLE hStdin;
DWORD fdwSaveOldMode;

void ErrorExit(const LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

#define MOUVE_CURSOR_UP_HELPER(x) printf("\x1b[" #x "A");
#define MOUVE_CURSOR_UP(x) MOUVE_CURSOR_UP_HELPER(x)

// define ANSI escape code
// Swap foreground and background colors
#define CONSOLE_INVERT "\x1b[7m"
// All attributes off
#define CONSOLE_RESET "\x1b[0m"
// Erase in line
#define CONSOLE_ERASE "\x1b[K"

void advanced_display(const int discs[SIZE], const int x, const int y, const struct board* board)
{
	OutputDebugString(L"advanced_display\n");

	// Restore cursor position
	// First move cursor left end
	printf("\n");

#ifdef SQUARE
	MOUVE_CURSOR_UP(EDGE);
#else
	MOUVE_CURSOR_UP(BOARD_ROW);
#endif // SQUARE

	MOUVE_CURSOR_UP(3);

	printf("   ");

	for (char c = 'A'; c < 'A' + BOARD_COLUMN; c++)
	{
		if (x + 'A' - 1 == c)
		{
			printf(" " CONSOLE_INVERT "%c" CONSOLE_RESET, c);
		}
		else
		{
			printf(" %c", c);
		}
	}

	if (0 == y)
	{
		printf("\n  1");

		int line = 2;
		for (int i = COLUMN; i < SIZE - COLUMN - 1; i++)
		{
			if (i % COLUMN == x - 1)
			{
				printf(" " CONSOLE_INVERT "%c" CONSOLE_RESET, *&disc_str[discs[i]]);
			}
			else
			{
				printf(" %c", *&disc_str[discs[i]]);
			}

			if (i % COLUMN == COLUMN - 1)
			{
				printf("\n%3d", line++);
			}
		}
	}
	else
	{
		if (1 == y)
		{
			printf("\n  " CONSOLE_INVERT "1" CONSOLE_RESET);
		}
		else
		{
			printf("\n  1");
		}

		int line = 2;
		for (int i = COLUMN; i < SIZE - COLUMN - 1; i++)
		{
			if (xy_to_index(x, y) == i ||
				xy_to_index(x, y * 10 + 0) == i ||
				xy_to_index(x, y * 10 + 1) == i ||
				xy_to_index(x, y * 10 + 2) == i ||
				xy_to_index(x, y * 10 + 3) == i ||
				xy_to_index(x, y * 10 + 4) == i ||
				xy_to_index(x, y * 10 + 5) == i ||
				xy_to_index(x, y * 10 + 6) == i ||
				xy_to_index(x, y * 10 + 7) == i ||
				xy_to_index(x, y * 10 + 8) == i ||
				xy_to_index(x, y * 10 + 9) == i)
			{
				printf(" " CONSOLE_INVERT "%c" CONSOLE_RESET, *&disc_str[discs[i]]);
			}
			else
			{
				printf(" %c", *&disc_str[discs[i]]);
			}

			if (i % COLUMN == COLUMN - 1)
			{
				if (y != line)
				{
					if (y == line / 10)
					{
						printf("\n " CONSOLE_INVERT "%d" CONSOLE_RESET "%d", y, line++ % 10);
					}
					else
					{
						printf("\n%3d", line++);
					}
				}
				else if (line < 10)
				{
					printf("\n  " CONSOLE_INVERT "%d" CONSOLE_RESET, line++);
				}
				else
				{
					printf("\n " CONSOLE_INVERT "%d" CONSOLE_RESET, line++);
				}
			}
		}
	}

	if (NULL == board)
	{
		printf("\n");
	}
	else
	{
		printf("\n%d B: %d - %d :W\n%c >>", board->move_count, board->black_count, board->white_count, toc(board->player));
	}

	if (0 != y)
	{
		printf("%c%d" CONSOLE_ERASE, x + 'A' - 1, y);
	}
	else if (0 != x)
	{
		printf("%c" CONSOLE_ERASE, x + 'A' - 1);
	}
	else
	{
		printf(CONSOLE_ERASE);
	}
}
#endif

void input(const int discs[SIZE], int* index, const struct board* board)
{
	if (NULL == index)
	{
		// error!!
		return;
	}

#ifdef _MSC_VER
	// Get the standard input handle.
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (INVALID_HANDLE_VALUE == hStdin)
	{
		ErrorExit("GetStdHandle");
	}

	// Save the current input mode, to be restored on exit.
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
	{
		ErrorExit("GetConsoleMode");
	}

	// Enable the window and mouse input events.
	if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
	{
		ErrorExit("SetConsoleMode");
	}

	int x = 0;
	int y = 0;
	DWORD cNumRead;
	INPUT_RECORD irInBuf[128];
	CHAR c = 0;
	CHAR d = 0;
	BOOL b = TRUE;

	do
	{
		// Wait for the events.
		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead))  // number of records read
		{
			ErrorExit("ReadConsoleInput");
		}

		for (DWORD i = 0; i < cNumRead; i++)
		{
			if (KEY_EVENT == irInBuf[i].EventType && 0 != irInBuf[i].Event.KeyEvent.bKeyDown)
			{
				CHAR input = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;

				if (0 == x)
				{
					if ('a' <= input && input < 'a' + BOARD_COLUMN)
					{
						x = input - 'a' + 1;
						advanced_display(discs, x, y, board);
					}

					if ('A' <= input && input < 'A' + BOARD_COLUMN)
					{
						x = input - 'A' + 1;
						advanced_display(discs, x, y, board);
					}
				}
				// WHEN x is not 0, MUST accept '\b'(BackSpace Key)!
				else if ('\b' == input)
				{
					if (0 != d)
					{
						d = 0;
						y = c - '1' + 1;
					}
					else if (0 != c)
					{
						c = 0;
						y = 0;
					}
					else
					{
						x = 0;
					}

					advanced_display(discs, x, y, board);
				}
				else if (0 == c)
				{
					if ('1' <= input && input < '1' + BOARD_ROW && input <= '9')
					{
						c = input;
						y = input - '1' + 1;
						advanced_display(discs, x, y, board);
					}
				}
				else if ('\r' == input)
				{
					*index = xy_to_index(x, y);
					printf("\n\n");

					b = FALSE;
					break;
				}
				else if (9 < BOARD_ROW)
				{
					int tmp;

					if ('1' <= input && input < '1' + BOARD_ROW)
					{
						tmp = y * 10 + input - '1' + 1;
					}
					else if ('0' == input)
					{
						tmp = y * 10;
					}
					else
					{
						continue;
					}

					if (tmp <= BOARD_ROW)
					{
						y = tmp;
						d = input;
						advanced_display(discs, x, y, board);
					}
				}
			}
		}
	} while (b);

	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
#else
	char c = 0;
	int d = 0;

	scanf_s("%c", &c, 1);
	scanf_s("%d", &d);
	while (getchar() != '\n') {}

	if ('a' <= c && c < 'a' + BOARD_SIZE)
	{
		*index = xy_to_index(c - 'a' + 1, d);
		return;
	}

	if ('A' <= c && c < 'a' + BOARD_SIZE)
	{
		*index = xy_to_index(c - 'A' + 1, d);
		return;
	}

	// error!!
#endif
}
