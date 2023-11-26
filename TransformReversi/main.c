#include "console.h"
#include <stdio.h>
#include "board.h"
#include "computer.h"

int main()
{
	struct board board = { 0 };
	int index = 0;
	start_new_game(&board);
	display(board.discs, &board);

	do
	{
		if (BLACK == board.player)
		{
			do
			{
				input(board.discs, &index, &board);

				if (CHOICE == board.discs[index])
				{
					move(&board, index);
					break;
				}
				else
				{
					printf("illegal move\n\n");
					display(board.discs, &board);
				}
			} while (1);
		}
		else
		{
			move(&board, select_negamax_move(board.discs, board.player));
			printf(" %c%d\n\n", index_to_column_char(board.move), index_to_row_int(board.move));
		}

		display(board.discs, &board);

		if (1 == board.pass)
		{
			move(&board, 0);
			printf(" pass\n\n");
			display(board.discs, &board);
		}

	} while (2 != board.pass);

	printf("\n\x1b[1A\x1b[K");

	if (board.black_count < board.white_count)
	{
		printf("WHITE WIN!\n");
	}
	if (board.black_count == board.white_count)
	{
		printf("DRAW!\n");
	}
	if (board.white_count < board.black_count)
	{
		printf("BLACK WIN!\n");
	}
}
