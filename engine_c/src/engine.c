#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "data.h"
#include "misc.c"
#include "board.c"
#include "evaluate.c"
#include "think.c"
#include "perft.c"
#include "moves.c"

int main(int argc, char *argv[])
	{
	precalculate();
//	check_perft_regular_board();

	double time_limit = 1.0;

	if (argc == 2)
		{
		decode_board(argv[1]);
		}
	else if (argc == 3)
		{
		time_limit = (double)atof(argv[2]);
		decode_board(argv[1]);
		}
	else
		{
		char initial_board[1000] =
		"0808"
		"bq------bk----bb"
		"--bpbp--bp------"
		"------bpbp------"
		"----bp----bp----"
		"----wp--wp--wp--"
		"----wpwpwpwpwp--"
		"------wp------wp"
		"--wq----wk----wb"
		"0,500,500";
		decode_board(initial_board);
set_classic_board();
time_limit = 60.0;
		}

	show_board();
	int depth;
	depth = think(time_limit);
	printf("Best: ", depth);
	print_algebraic(pv[depth][depth]);
	printf("\n");
	print_encoded_board();
	printf("\n");
	make_move(&pv[depth][depth]);
	print_encoded_board();
	printf("\n");

//	perft_results(2);
/*
if(2==2){
	generate_moves(0, 0);show_board();
	int x;
	for (x = 0; x < move_counter[0]; ++x)
		{
		printf("Move %d) ", x + 1);
		print_algebraic(moves[0][x]);
		make_move(&moves[0][x]);
		generate_moves(0, 1);
		int z;
		for (z = 0; z < move_counter[1]; ++z)
			{
			printf("\tMove %d) ", z + 1);
			print_algebraic(moves[1][z]);
			make_move(&moves[1][z]);
			printf(" value %d", evaluate());
			unmake_move(&moves[1][z]);
			printf("\n");
			}

		printf(" value %d", evaluate());
		unmake_move(&moves[0][x]);
		printf("\n");
		}
show_board();
}*/

	return 0;
	}
