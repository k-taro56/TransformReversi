#pragma once

#include "core.h"

#define INFINITY 1 + (BOARD_SIZE)
#define NEGATIVE_INFINITY -1 * (INFINITY)

int evaluate(const int discs[SIZE])
{
	int count = 0;
	for (int i = COLUMN; i < SIZE - COLUMN; i++)
	{
		if (BLACK == discs[i])
		{
			count++;
		}
	}
	return count;
}

int negamax(const int discs[SIZE], const int player, const int depth, const int pass)
{
	if (0 == depth)
	{
		return evaluate(discs);
	}

	int best = NEGATIVE_INFINITY;
	int next[SIZE] = { 0 };
	copy(discs, next);

	for (int i = COLUMN; i < SIZE - COLUMN; i++)
	{
		if (0 != flip(next, player, i))
		{
			int tmp = -1 * negamax(next, opponent(player), depth - 1, 0);
			copy(discs, next);

			if (best < tmp)
			{
				best = tmp;
			}
		}
	}

	if (NEGATIVE_INFINITY == best)
	{
		if (1 == pass)
		{
			return evaluate(discs);
		}

		return -1 * negamax(discs, opponent(player), depth, 1);
	}

	return best;
}

int select_negamax_move(const int discs[SIZE], const int player)
{
	int index = 0;
	int eval = NEGATIVE_INFINITY;
	int next[SIZE] = { 0 };
	copy(discs, next);

	for (int i = COLUMN; i < SIZE - COLUMN; i++)
	{
		if (0 != flip(next, player, i))
		{
			int tmp = -1 * negamax(next, opponent(player), 2, 0);
			copy(discs, next);

			if (eval < tmp)
			{
				index = i;
				eval = tmp;
			}
		}
	}
	return index;
}
