#include "core.h"

void initialize(int discs[SIZE])
{
	int i;

	for (i = 0; i < COLUMN; i++)
	{
		discs[i] = SENTINEL;
	}

	for (; i < SIZE - COLUMN; i++)
	{
		discs[i] = EMPTY;
	}

	discs[xy_to_index(BOARD_COLUMN / 2, BOARD_ROW / 2)] =
		discs[xy_to_index(BOARD_COLUMN / 2 + 1, BOARD_ROW / 2 + 1)] = WHITE;

	discs[xy_to_index(BOARD_COLUMN / 2, BOARD_ROW / 2 + 1)] =
		discs[xy_to_index(BOARD_COLUMN / 2 + 1, BOARD_ROW / 2)] = BLACK;

	discs[xy_to_index(BOARD_COLUMN / 2, BOARD_ROW / 2 - 1)] =
		discs[xy_to_index(BOARD_COLUMN / 2 - 1, BOARD_ROW / 2)] =
		discs[xy_to_index(BOARD_COLUMN / 2 + 2, BOARD_ROW / 2 + 1)] =
		discs[xy_to_index(BOARD_COLUMN / 2 + 1, BOARD_ROW / 2 + 2)] = CHOICE;

	for (; i < SIZE; i++)
	{
		discs[i] = SENTINEL;
	}

	for (i = 2 * COLUMN - 1; i < SIZE; i += COLUMN)
	{
		discs[i] = SENTINEL;
	}

#if 0 < TRANSFORM_1
	for (i = 0; i < TRANSFORM_1; i++)
	{
		for (int j = 0; j < TRANSFORM_1 - i; j++)
		{
			board[i * COLUMN + j + COLUMN] = SENTINEL;
			board[-1 * i * COLUMN + j + SIZE - 2 * COLUMN] = SENTINEL;

			int k = TRANSFORM_1 - j - i;

			board[i * COLUMN + BOARD_COLUMN + COLUMN - k] = SENTINEL;
			board[-1 * i * COLUMN + BOARD_COLUMN + SIZE - k - 2 * COLUMN] = SENTINEL;
		}
	}
#endif
#if 0 < TRANSFORM_2
	for (int i = 0; i < TRANSFORM_2; i++)
	{
		board[i * (COLUMN + 1) + COLUMN] = SENTINEL;
		board[i * (COLUMN - 1) + COLUMN + BOARD_COLUMN - 1] = SENTINEL;
		board[-i * (COLUMN - 1) + SIZE - 2 * COLUMN] = SENTINEL;
		board[-i * (COLUMN + 1) + SIZE - 2 * COLUMN + BOARD_COLUMN - 1] = SENTINEL;
	}
#endif
}

// { Left, Rhight, Up, Down,
// UpperLeft, UpperRhight, LowerLeft, LowerRhight }
int directions[] = { -1, 1, -1 * COLUMN, COLUMN,
-1 * COLUMN - 1, -1 * COLUMN + 1, COLUMN - 1 , COLUMN + 1 };
const int directions_count = 8;

enum disc can_flip(const int discs[SIZE], const int player, const int index)
{
	const int opp = opponent(player);

	for (int i = 0; i < directions_count; i++)
	{
		int diff = directions[i];

		int j;
		for (j = index + diff; opp == discs[j]; j += diff)
		{
		}

		if (index + diff != j && discs[j] == player)
		{
			return CHOICE;
		}
	}
	return EMPTY;
}

int set_choice(int discs[SIZE], const int player)
{
	int ret = 0;
	if (player != WHITE && player != BLACK)
	{
		// error!!
		return ret;
	}

	for (int i = COLUMN; i < SIZE - COLUMN; i++)
	{
		int disc = discs[i];

		if (CHOICE == disc)
		{
			discs[i] = EMPTY;
		}

		if (BLACK == disc || WHITE == disc || SENTINEL == disc)
		{
			continue;
		}

		discs[i] = can_flip(discs, player, i);

		if (CHOICE == discs[i])
		{
			ret = 1;
		}
	}
	return ret;
}

int can_move(int discs[SIZE], const int player)
{
	int ret = 0;
	if (player != WHITE && player != BLACK)
	{
		// error!!
		return ret;
	}

	for (int i = COLUMN; i < SIZE - COLUMN; i++)
	{
		int disc = discs[i];

		if (CHOICE == disc)
		{
			discs[i] = EMPTY;
		}

		if (BLACK == disc || WHITE == disc || SENTINEL == disc)
		{
			continue;
		}

		if (CHOICE == can_flip(discs, player, i))
		{
			return 1;
		}
	}
	return ret;
}

int flip(int discs[SIZE], const int player, const int index)
{
	int ret = 0;
	if (player != WHITE && player != BLACK)
	{
		// error!!
		return ret;
	}

	if ((discs[index] != CHOICE) && (discs[index] != EMPTY))
	{
		// error!!
		return ret;
	}

	const int opp = opponent(player);

	for (int i = 0; i < directions_count; i++)
	{
		int diff = directions[i];

		int j;
		int k = 0;
		for (j = index + diff; opp == discs[j]; j += diff)
		{
			k = 1;
		}

		if (1 == k && player == discs[j])
		{
			ret = 1;
			for (j = index + diff; player != discs[j]; j += diff)
			{
				discs[j] = player;
			}
		}
	}

	if (1 == ret)
	{
		discs[index] = player;
	}

	return ret;
}

int flip_with_counter(int discs[SIZE], const int player, const int index, int* count)
{
	int ret = 0;
	if (player != WHITE && player != BLACK)
	{
		// error!!
		return ret;
	}

	if ((discs[index] != CHOICE) && (discs[index] != EMPTY))
	{
		// error!!
		return ret;
	}

	const int opp = opponent(player);
	int tmp_count = 0;

	for (int i = 0; i < directions_count; i++)
	{
		int diff = directions[i];

		int j;
		int k = 0;
		for (j = index + diff; opp == discs[j]; j += diff)
		{
			k++;
		}

		if (0 != k && player == discs[j])
		{
			tmp_count += k;
			for (j = index + diff; player != discs[j]; j += diff)
			{
				discs[j] = player;
			}
		}
	}

	if (0 != tmp_count)
	{
		ret = 1;
		discs[index] = player;
		*count += tmp_count + 1;
	}
	return ret;
}

void get_flip_count(const int discs[SIZE], const int player, const int index, int* count)
{
	const int opp = opponent(player);
	int tmp_count = 0;

	for (int i = 0; i < directions_count; i++)
	{
		int diff = directions[i];

		int j;
		int k = 0;
		for (j = index + diff; opp == discs[j]; j += diff)
		{
			k++;
		}

		if (0 != k && player == discs[j])
		{
			tmp_count += k;
		}
	}

	if (0 != tmp_count)
	{
		*count += tmp_count + 1;
	}
}
