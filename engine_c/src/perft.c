int perft(int depth, int initial_depth)
	{
	if (depth == 0) return 1;

	int nodes = 0;
	int this_nodes = 0;
	int i;

	generate_moves(0, depth);
	if (move_counter[depth] > 0) for (i = 0; i < move_counter[depth]; ++i)
//		if ((moves[depth][i].to_x == 3 && moves[depth][i].to_y == 5) || initial_depth != 2)
		{

		if (initial_depth > 0)
			{
			print_algebraic(moves[depth][i]);
			printf(": ");
			}
		make_move(&moves[depth][i]);
		this_nodes = perft(depth - 1, initial_depth - 1);
		nodes += this_nodes;
		unmake_move(&moves[depth][i]);
		if (initial_depth > 0) printf ("%d\n", this_nodes);
		}

	return nodes;
	}

void perft_results(int final_depth)
	{
	int depth;
	for (depth = 2; depth <= final_depth; ++depth)
		{
		printf ("Nodes at depth %d: %d\n", depth, perft(depth, 1));
		show_board();
		}
	printf("At depth 4, we expect 197,281 nodes\n");
	}

void check_perft_regular_board()
	{
        char initial_board[1000] =
        "0808"
        "brbnbbbqbkbbbnbr"
        "bpbpbpbpbpbpbpbp"
        "----------------"
        "----------------"
        "----------------"
        "----------------"
        "wpwpwpwpwpwpwpwp"
        "wrwnwbwqwkwbwnwr0,12,34";
        decode_board(initial_board);
        perft_results(4);
	}
