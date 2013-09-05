void print_algebraic(MOVE move)
	{
	if (move.x == -1) printf("*");
	if (move.piece > 0 && move.piece != 1 && move.piece != 7) printf("%c", piece_algebraic(move.piece));
	if (move.x == -1)
		printf("%c%c", move.to_x + 97, board.height - move.to_y + 48);
	else
		printf("%c%c%c%c%c"
		, move.x + 97, board.height - move.y + 48
		, move.captured_piece == 0 ? '-' : 'x'
		, move.to_x + 97, board.height - move.to_y + 48);
	}

// fixme: todo: handle promotion
// fixme: handle resources (when spawning, when capturing)
void make_move(MOVE *move)
	{
	board.cells[move->to_x][move->to_y] = move->piece;
	if (move->captured_piece > 0)
		board.resource[board.side] += absolute_material_value[move->captured_piece];

	if (move->x == -1)
		board.resource[board.side] -= absolute_material_value[move->piece];
	else
		board.cells[move->x][move->y] = 0;
	board.side = !board.side;
	}

void unmake_move(MOVE *move)
	{
	board.side = !board.side;
	board.cells[move->to_x][move->to_y] = move->captured_piece;
	if (move->x == -1)
		board.resource[board.side] += absolute_material_value[move->piece];
	else
		board.cells[move->x][move->y] = move->piece;

	if (move->captured_piece > 0)
		board.resource[board.side] -= absolute_material_value[move->captured_piece];
	}

void record_a_move(int x, int y, int to_x, int to_y, int captures_only, int depth)
	{
//printf("record(%d,%d,%d,%d,%d,%d)\n",x,y,to_x,to_y,captures_only,depth);
	if (captures_only && board.cells[to_x][to_y] == 0)
		return;
	if (board.cells[x][y])
		++attacked_squares[depth][to_x][to_y];

	if (attacked_squares_only == 0)
		{
		int check;

		moves[depth][move_counter[depth]].x = x;
		moves[depth][move_counter[depth]].y = y;
		moves[depth][move_counter[depth]].piece = board.cells[x][y];
		moves[depth][move_counter[depth]].to_x = to_x;
		moves[depth][move_counter[depth]].to_y = to_y;
		moves[depth][move_counter[depth]].captured_piece = board.cells[to_x][to_y];

		make_move(&moves[depth][move_counter[depth]]);
		board.side = !board.side; // these switches are dumb
		check = in_check(depth);
		board.side = !board.side;
		unmake_move(&moves[depth][move_counter[depth]]);

		if (!check) ++move_counter[depth];
		}
	}

int attempt_a_move(int x, int y, int to_x, int to_y, int captures_only, int depth)
	{
//printf("attempt(%d,%d,%d,%d,%d,%d)\n",x,y,to_x,to_y,captures_only,depth);
	if (to_x < 0 || to_y < 0 || to_x >= board.width || to_y >= board.height)
		return 0;

	if (board.cells[to_x][to_y] == 0)
		{
		record_a_move(x, y, to_x, to_y, captures_only, depth);
		return 1;
		}
	if ((board.cells[to_x][to_y] <= 6 && board.side != 0)
		|| (board.cells[to_x][to_y] > 6 && board.side == 0))
		{
		record_a_move(x, y, to_x, to_y, captures_only, depth);
		return 0;
		}
	return 0;
	}

void attempt_a_ray_move(int x, int y, int ray_x_dir, int ray_y_dir, int captures_only, int depth)
	{
	int ray_x = ray_x_dir;
	int ray_y = ray_y_dir;
	while (attempt_a_move(x, y, x + ray_x, y + ray_y, captures_only, depth))
		{
		ray_x += ray_x_dir;
		ray_y += ray_y_dir;
		}
	}

void generate_move(int x, int y, int piece, int captures_only, int depth)
	{
	int pawn_dir = -1;
	int start_row = board.height - 2;
	int promotion_row = 0;

	if (piece > 6)
		{
		piece -= 6; // side detection is done one level above
		pawn_dir = start_row = 1;
		promotion_row = board.height - 1;
		}

	if (piece == 2 || piece == 5) // bishop
		{
		attempt_a_ray_move(x, y, 1, 1, captures_only, depth);
		attempt_a_ray_move(x, y, 1, -1, captures_only, depth);
		attempt_a_ray_move(x, y, -1, 1, captures_only, depth);
		attempt_a_ray_move(x, y, -1, -1, captures_only, depth);
		}
	if (piece == 4 || piece == 5) // rook
		{
		attempt_a_ray_move(x, y, 1, 0, captures_only, depth);
		attempt_a_ray_move(x, y, 0, 1, captures_only, depth);
		attempt_a_ray_move(x, y, -1, 0, captures_only, depth);
		attempt_a_ray_move(x, y, 0, -1, captures_only, depth);
		}

	if (piece == 1 && y != promotion_row) // pawn
		{
// fixme: todo: handle promotion in make_move/unmake_move
		if (board.cells[x+1][y+pawn_dir] != 0)
			attempt_a_move(x, y, x + 1, y + pawn_dir, captures_only, depth);
		if (board.cells[x-1][y+pawn_dir] != 0)
			attempt_a_move(x, y, x - 1, y + pawn_dir, captures_only, depth);
		if (board.cells[x][y+pawn_dir] == 0)
			attempt_a_move(x, y, x, y + pawn_dir, captures_only, depth);
		if (board.cells[x][y+pawn_dir+pawn_dir] == 0 && start_row == y && board.cells[x][y+pawn_dir] == 0)
			attempt_a_move(x, y, x, y + pawn_dir + pawn_dir, captures_only, depth);
		}
	else if (piece == 3) // knight
		{
		attempt_a_move(x, y, x+2, y+1, captures_only, depth);
		attempt_a_move(x, y, x-2, y+1, captures_only, depth);
		attempt_a_move(x, y, x+2, y-1, captures_only, depth);
		attempt_a_move(x, y, x-2, y-1, captures_only, depth);
		attempt_a_move(x, y, x+1, y+2, captures_only, depth);
		attempt_a_move(x, y, x-1, y+2, captures_only, depth);
		attempt_a_move(x, y, x+1, y-2, captures_only, depth);
		attempt_a_move(x, y, x-1, y-2, captures_only, depth);
		}
	else if (piece == 6) // king
		{
		attempt_a_move(x, y, x+1, y+1, captures_only, depth);
		attempt_a_move(x, y, x, y+1, captures_only, depth);
		attempt_a_move(x, y, x-1, y+1, captures_only, depth);
		attempt_a_move(x, y, x+1, y, captures_only, depth);
		attempt_a_move(x, y, x-1, y, captures_only, depth);
		attempt_a_move(x, y, x+1, y-1, captures_only, depth);
		attempt_a_move(x, y, x, y-1, captures_only, depth);
		attempt_a_move(x, y, x-1, y-1, captures_only, depth);
		}
	}

// captures_only is used 1) for check detection (in conjunction with not making moves[]) and 2) quiescent search

void record_a_spawn(piece, to_x, to_y, depth)
	{
	int check;
	moves[depth][move_counter[depth]].x = -1;
	moves[depth][move_counter[depth]].y = -1;
	moves[depth][move_counter[depth]].piece = piece;
	moves[depth][move_counter[depth]].to_x = to_x;
	moves[depth][move_counter[depth]].to_y = to_y;
	moves[depth][move_counter[depth]].captured_piece = 0;

	make_move(&moves[depth][move_counter[depth]]);
	board.side = !board.side; // these switches are dumb
	check = in_check(depth);
	board.side = !board.side;
	unmake_move(&moves[depth][move_counter[depth]]);

	if (!check) ++move_counter[depth];
	}

void generate_moves(int captures_only, int depth)
	{
	int x, y, piece;

	if (attacked_squares_only)
		{
		for (y = 0; y < board.height; ++y)
		for (x = 0; x < board.width; ++x)
			attacked_squares[depth][x][y] = 0;
		}
	else
		move_counter[depth] = 0;

	// generate spawns
	if (!captures_only && board.resource[board.side] >= 100)
		{
		int piece_row = board.height - 1;
		int start_row = board.height - 2;
		int piece_color = 0;
		if (board.side)
			{
			start_row = 1;
			piece_row = 0;
			piece_color = 6;
			}
		for (x = 0; x < board.width; ++x) if (board.cells[x][start_row] == 0)
			record_a_spawn(1 + piece_color, x, start_row, depth);
		if (board.resource[board.side] >= 300)
		for (x = 0; x < board.width; ++x) if (board.cells[x][piece_row] == 0)
			{
			record_a_spawn(2 + piece_color, x, piece_row, depth);
			record_a_spawn(3 + piece_color, x, piece_row, depth);
			if (board.resource[board.side] >= 500)
				record_a_spawn(4 + piece_color, x, piece_row, depth);
			if (board.resource[board.side] >= 900)
				record_a_spawn(5 + piece_color, x, piece_row, depth);
			}
		}

	// generate moves
	for (y = 0; y < board.height; ++y)
	for (x = 0; x < board.width; ++x)
		{
		piece = board.cells[x][y];
		if ((board.side == 0 && piece > 0 && piece < 7) || (board.side != 0 && piece > 6))
			{
			generate_move(x, y, piece, captures_only, depth);
			}
		}

/*	for (x = 0; x < move_counter[depth]; ++x)
		{
		printf("Move %d: ", x + 1);
		print_algebraic(moves[depth][x]);
		printf("\n");
		}*/
	}
