/*
<?php

function calculate_attack_squares(&$board)
	{
	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	generate_moves($board, 'c');
	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	}
*/

// checkmate detect is - if in_check and no moves
// stalemate detect is - if not in_check and no moves

int in_check(int depth) // this function expect only one king
	{
	int other_side = !board.side;
	int x, y;
	int my_king = board.side == 0 ? 6 : 12;

	attacked_squares_only = 1;
	board.side = !board.side;
	generate_moves(1, depth);
	board.side = !board.side;
	attacked_squares_only = 0;

	for (y = 0; y < board.height; ++y)
	for (x = 0; x < board.width; ++x)
	if (board.cells[x][y] == my_king)
		{
		if (attacked_squares[depth][x][y])
			return 1;
		else
			return 0;
		}
	return 0; // checkmate? lol
	}

int evaluate()
	{
	int material = 0;
	int positional = 0;
//	int pieces_on_board = 0; // promote spawned pieces over hearding resource
	int x, y;

// note: checkmate is now done during move generation
//	int white_checkmated = 1; // assume checkmate until we find a king
//	int black_checkmated = 1;

	for (y = 0; y < board.height; ++y)
	for (x = 0; x < board.width; ++x)
		{
//		if (board.cells[x][y] == 6) white_checkmated = 0;
//		if (board.cells[x][y] == 12) black_checkmated = 0;

		material += material_value[board.cells[x][y]];
		if (board.cells[x][y] > 0 && board.cells[x][y] <= 5)
			{
			positional += positional_value[x][y];
//			++pieces_on_board;
			}
		if (board.cells[x][y] > 6 && board.cells[x][y] <= 11)
			{
			positional -= positional_value[x][y];
//			--pieces_on_board;
			}
		}

/*	if (white_checkmated && board.side == 0 || black_checkmated && board.side != 0) return -99999;
	if (white_checkmated && board.side != 0 || black_checkmated && board.side == 0) return 99999;
*/
	material += board.resource[0] - board.resource[1];

	if (board.side != 0)
		return -material - positional;// - pieces_on_board;

	return material + positional;// + pieces_on_board;
	}
/*
function evaluate(&$board)
	{
	$white_material = array('--' => 0, 'wp' => 100, 'wn' => 300, 'wb' => 300, 'wr' => 500, 'wq' => 900);
	$black_material = array('--' => 0, 'bp' => 100, 'bn' => 300, 'bb' => 300, 'br' => 500, 'bq' => 900);

	$ret = 0;
	$white = $board['wr'];
	$black = $board['br'];

	for ($y = 0; $y < $board['h']; ++$y)
	for ($x = 0; $x < $board['w']; ++$x)
		{
		$white += $white_material[$board[$x][$y]];
		$black += $black_material[$board[$x][$y]];
		}

	if ($board['s'] == 'w')
		$material = $white - $black;
	else
		$material = $black - $white;

	return $material;
	}

?>
*/
