#include "board.h"

void start_new_game(struct board* board)
{
	initialize(board->discs);
	board->player = BLACK;
	board->pass = 0;
	board->move = 0;
	board->move_count = 0;
	board->black_count = 2;
	board->white_count = 2;
}

void move(struct board* board, const int index)
{
	if (1 == board->pass)
	{
		board->player = opponent(board->player);
		board->pass = 0;
		board->move = 0;
		board->move_count++;
	}
	else
	{
		int count = 0;
		if (0 == flip_with_counter(board->discs, board->player, index, &count))
		{
			// error
			return;
		}

		if (BLACK == board->player)
		{
			board->black_count += count;
			board->white_count += -1 * count + 1;
		}
		else
		{
			board->black_count += -1 * count + 1;
			board->white_count += count;
		}

		board->player = opponent(board->player);
		board->pass = 0;
		board->move = index;
		board->move_count++;
	}

	// cehck pass
	if (0 == set_choice(board->discs, board->player))
	{
		if (0 == can_move(board->discs, opponent(board->player)))
		{
			board->pass = 2;

			int i = board->black_count + board->white_count;

#if 0 < TRANSFORM_1 || 0 < TRANSFORM_2
			for (int i = COLUMN; i < SIZE - COLUMN; i++)
			{
				if (SENTINEL == game->discs[i] && i % COLUMN != COLUMN - 1)
				{
					i++;
				}
			}
#endif
			if (i < BOARD_SIZE)
			{
				if (board->black_count < board->white_count)
				{
					board->white_count += BOARD_SIZE - i;
				}
				else if (board->white_count < board->black_count)
				{
					board->black_count += BOARD_SIZE - i;
				}
			}
		}
		else
		{
			board->pass = 1;
		}
	}
}

void clone(const struct board* source, struct board* destination)
{
	copy(source->discs, destination->discs);
	destination->player = source->player;
	destination->pass = source->pass;
	destination->move = source->move;
	destination->move_count = source->move_count;
	destination->black_count = source->black_count;
	destination->white_count = source->white_count;
}
