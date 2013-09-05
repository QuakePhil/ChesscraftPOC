int quiesce(int alpha, int beta, int depth)
	{
	int stand_pat = evaluate();
	if (stand_pat >= beta)
		return beta;
	if (alpha < stand_pat)
		alpha = stand_pat;

	int i, score;
	generate_moves(1, depth);

	for (i = 0; i < move_counter[depth]; ++i)
		{
		make_move(&moves[depth][i]);
		score = -1 * quiesce(-beta, -alpha, depth + 1);
		unmake_move(&moves[depth][i]);

		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
		}
	return alpha;
	}

// note pv[0][...] and pv[...][0] is not being used since there are no best moves at return evaluate;
int negamax(int alpha, int beta, int depth, int initial_depth, double * ttl)
	{
	++node_counter;

	if (node_counter % 1000 == 0)
		{
		if (timer() > *ttl)
			{
			if (*ttl != 0.0)
				printf("Search timeout\n");
			*ttl = 0.0;
			return quiesce(alpha, beta, initial_depth + 1);
			}
		}

	if (depth == 0)
		return quiesce(alpha, beta, initial_depth + 1);

	int i, j, score;
	generate_moves(0, depth);

	if (move_counter[depth] == 0)
		{
		if (in_check(depth)) return -109999;
		return 0;
		}

	for (i = 0; i < move_counter[depth] && *ttl != 0.0; ++i)
		{
		make_move(&moves[depth][i]);
		score = -1 * negamax(-beta, -alpha, depth - 1, initial_depth, ttl);
//if (depth == initial_depth)
//{printf("Depth %d move ", depth);print_algebraic(moves[depth][i]);printf(" score %d max %d\n", score, alpha);}
		unmake_move(&moves[depth][i]);

		if (score >= beta)
			{
//printf("(beta cutoff %d >= %d, return %d)", score, beta, beta);
			return beta; // fail hard beta cutoff
			}

		if (score > alpha)
			{
//if (depth == initial_depth)
//{			printf("Depth %d *new* best move ", depth);
//			print_algebraic(moves[depth][i]);printf(" score %d max %d\n", score, alpha);}

			move_copy(&pv[depth][depth], &moves[depth][i]);
			for (j = depth - 1; j > 0; --j)
				move_copy(&pv[depth][j], &pv[depth-1][j]);

			alpha = score;
			}
		}
	if (alpha <= -99999) // double as mate counter
		++alpha;
	if (alpha >= 99999)
		--alpha;

	return alpha;
	}

int print_pv(int value, int depth, double start_time)
	{
	int mate = 0, i;
	if (value <= -99999)
		mate = -109999 - value;
	if (value >= 99999)
		mate = 109999 - value;

	double elapsed = timer() - start_time;
	double nps = (double)node_counter / elapsed;
	if (elapsed < 1.0) nps = (double)node_counter;

	printf("%2d %6.2lf %11d %11.2f ", depth, elapsed, node_counter, nps);
	if (mate > 0)
		{
		printf("  W%03d ", mate);
		value = depth - mate;
		}
	else if (mate < 0)
		{
		printf("  L%03d ", mate);
		value = depth - -mate;
		}
	else
		{
		printf("%6.2f ", (float)value / 100.0);
		value = 0;
		}

	if (value < 0) value = 0; // value now stores how much of the PV to print out

	for (i = depth; i > value; --i)
		{
		printf(" ");
		print_algebraic(pv[depth][i]);
		}
	printf("\n");

	if (mate != 0)
		return 0;
	return 1;
	}

int think(double time_limit)
	{
	int depth, i, value;
	double ttl = timer() + time_limit;
	double start_time;
	MOVE best;
	printf("Ply  Time       Nodes         NPS  Value  PV\n");
	for (depth = 1; depth <= 20; ++depth)
		{
		node_counter = 0;
		start_time = timer();
		value = negamax(-119999, 119999, depth, depth, &ttl);
		if (ttl == 0.0)
			{
			move_copy(&pv[depth-1][depth-1], &best);
			return depth - 1;
			}
		if (!print_pv(value, depth, start_time))
			return depth;
		move_copy(&best, &pv[depth][depth]);
		}
	return depth - 1;
	}

MOVE bestmove()
	{
	int depth;
	double ttl = timer() + 1.0;
	MOVE best;
	for (depth = 1; depth <= 5; ++depth)
		{
		negamax(-119999, 119999, depth, depth, &ttl);
		move_copy(&best, &pv[depth][depth]);
		}
	return best;
	}
