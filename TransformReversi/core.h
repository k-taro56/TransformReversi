#pragma once
// Implements the basic rules of othello. It also provides simple functions for them.

#define SQUARE
#ifdef SQUARE
#define EDGE 40
#define BOARD_COLUMN (EDGE)
#define BOARD_ROW  (EDGE)
#else
#define BOARD_COLUMN 8
#define BOARD_ROW 8
#endif

#define BOARD_SIZE (BOARD_COLUMN) * (BOARD_ROW)

#define TRANSFORM_1 0
#define TRANSFORM_2 0

#ifdef SQUARE
#if EDGE < 4 
#error EDGE must be 4 or more
#endif
#if EDGE % 2
#error EDGE must be even number
#endif
#else
#if BOARD_COLUMN < 4
#error BOARD_COLUMN must be 4 or more
#endif
#if BOARD_ROW < 4 
#error BOARD_ROW must be 4 or more
#endif
#if BOARD_COLUMN % 2
#error BOARD_COLUMN must be even number
#endif
#if BOARD_ROW % 2
#error BOARD_ROW must be even number
#endif
#endif

#if BOARD_COLUMN < 2 * TRANSFORM_1 + 2 && BOARD_ROW < 2 * TRANSFORM_1+ 2
#error TRANSFORM_1 is too large
#endif
#if BOARD_COLUMN < 2 * TRANSFORM_2 + 2 && BOARD_ROW < 2 * TRANSFORM_2+ 2
#error TRANSFORM_2 is too large
#endif

#define COLUMN ((BOARD_COLUMN) + 1)
#define ROW ((BOARD_ROW) + 2)
#define SIZE ((COLUMN) * (ROW))

enum disc
{
	EMPTY,
	WHITE,
	BLACK,
	SENTINEL,
	CHOICE
	//Stable
};

inline int index_to_row_int(const int index)
{
	return index / COLUMN;
}

inline char index_to_column_char(const int index)
{
	return 'A' + index % COLUMN;
}

inline int xy_to_index(const int x, const int y)
{
	return x + y * COLUMN - 1;
}

inline int opponent(int player)
{
	return 3 - player;
}

inline void copy(const int source[SIZE], int destination[SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		destination[i] = source[i];
	}
}

void initialize(int[SIZE]);

int set_choice(int[SIZE], const int /*player*/);

int can_move(int[SIZE], const int /*player*/);

int flip(int[SIZE], const int /*player*/, const int /*index*/);

int flip_with_counter(int[SIZE], const int /*player*/, const int /*index*/, int* /*count*/);

void get_flip_count(const int[SIZE], const int /*player*/, const int /*index*/, int* /*count*/);
