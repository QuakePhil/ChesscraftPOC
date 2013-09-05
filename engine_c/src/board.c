int piece_i(char *a)
	{
	int i = 0;
	switch (a[1])
		{
		case '-': i = 0; break;
		case 'p': i = 1; break;
		case 'b': i = 2; break;
		case 'n': i = 3; break;
		case 'r': i = 4; break;
		case 'q': i = 5; break;
		case 'k': i = 6; break;
		}
	if (a[0] == 'b')
		return i+6;
	return i;
	}

char piece_algebraic(int i)
	{
	switch (i)
		{
		case 0: return '-';
		case 1: return ' ';
		case 2: return 'B';
		case 3: return 'N';
		case 4: return 'R';
		case 5: return 'Q';
		case 6: return 'K';
		case 7: return ' ';
		case 8: return 'B';
		case 9: return 'N';
		case 10: return 'R';
		case 11: return 'Q';
		case 12: return 'K';
		}
	return '?';
	}

char *piece_a(int i)
	{
	switch (i)
		{
		case 0: return "--";
		case 1: return "wp";
		case 2: return "wb";
		case 3: return "wn";
		case 4: return "wr";
		case 5: return "wq";
		case 6: return "wk";
		case 7: return "bp";
		case 8: return "bb";
		case 9: return "bn";
		case 10: return "br";
		case 11: return "bq";
		case 12: return "bk";
		}
	return "??";
	}

void show_board()
	{
	int x, y;
	printf("v: %d, %c to move (wr: %d) (br: %d)%s\n", evaluate(), board.side == 0 ? 'w' : 'b'
		, board.resource[0], board.resource[1]
		, in_check(0) ? " (incheck)" : "");
	printf("- ");
	for (x = 0; x < board.width; ++x)
		printf("%c ", x + 97);
	printf("\n");
	for (y = 0; y < board.height; ++y)
		{
		printf("%d ", board.height - y);
		for (x = 0; x < board.width; ++x)
			printf("%s", piece_a(board.cells[x][y]));
		printf("\n");
		}
	}

void print_encoded_board()
	{
	printf("%02d%02d", board.width, board.height);
	int x, y;
	for (y = 0; y < board.height; ++y)
	for (x = 0; x < board.width; ++x)
		printf("%s", piece_a(board.cells[x][y]));
	printf("%d,%d,%d", board.side, board.resource[0], board.resource[1]);
	}

void decode_board(char *initial_board)
	{
	char buf[10];
	buf[0] = initial_board[0];
	buf[1] = initial_board[1];
	buf[2] = 0;
	board.width = atoi(buf);

	buf[0] = initial_board[2];
	buf[1] = initial_board[3];
	board.height = atoi(buf);

	int i = 4, x, y;
	for (y = 0; y < board.height; ++y)
	for (x = 0; x < board.width; ++x)
		{
		buf[0] = initial_board[i];
		buf[1] = initial_board[i+1];
		board.cells[x][y] = piece_i(buf);
		i += 2;
		}

	x = 0;
	y = 0;
	while (1)
		{
		if (initial_board[i] == ',' || initial_board[i] == 0)
			{
			buf[x] = 0;
			if (y == 0)
				board.side = atoi(buf);
			if (y == 1)
				board.resource[0] = atoi(buf);
			if (y == 2)
				board.resource[1] = atoi(buf);
			if (initial_board[i] == 0)
				break;
			x = -1;
			++y;
			}
		else
			buf[x] = initial_board[i];
		++i;
		++x;
//	board.side = initial_board[i] == '0' ? 0 : 1;
//	board.resource[0] = 0;
//	board.resource[1] = 0;
		}

// todo : side and resources
	/*board.width = substr($board, 0, 2);
	$ret['h'] = substr($board, 2, 2);
	$i = 4;
	for ($y = 0; $y < $ret['h']; ++$y)
		for ($x = 0; $x < $ret['w']; ++$x)
			{
			$ret[$x][$y] = substr($board, $i, 2);
			$i += 2;
			}
	list($ret['s'], $ret['wr'], $ret['br']) = explode(',', substr($board, $i));
	$ret['s'] = ($ret['s'] == 0 ? 'w' : 'b');

	return $ret;
*/	}

void set_classic_board()
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
        "wrwnwbwqwkwbwnwr0,0,0";
	decode_board(initial_board);
	}
