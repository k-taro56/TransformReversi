#pragma once

#include "core.h"

struct board
{
	int discs[SIZE];
	int player;
	int pass;
	int move;
	int move_count;
	int black_count;
	int white_count;
};

void start_new_game(struct board*);

void move(struct board*, const int /*index*/);

void clone(const struct board* /*source*/, struct board* /*destination*/);
