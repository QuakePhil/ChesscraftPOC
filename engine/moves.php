<?php

// moves array is ['x', 'yz', x, y, tox, toy, 'vw']
// x - c for capture, or m for move
// yz - y is color, z is piece (wn is white knight)
// vw - v is color, w is piece - optional captured piece

// fixme: these make_moves should adjust resources accordingly
function make_move(&$board, $move)
	{
	$board[$move[4]][$move[5]] = $move[1];
	if ($move[2] != -1)
		$board[$move[2]][$move[3]] = '--';

	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	}

function unmake_move(&$board, $move)
	{
	$board[$move[4]][$move[5]] = $move[6] ? $move[6] : '--';
	if ($move[2] != -1)
		$board[$move[2]][$move[3]] = $move[1];

	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	}

function record_a_move(&$moves, &$board, $move, $type, $restrict_type)
	{
	if ($type == 'c')
		$board['asq'][$board['s']][$move[4]][$move[5]] = true;
	if (!$restrict_type || $restrict_type == $type)
		{
		if ($restrict_type == 'c') // we do not do in check detection for 'c' only generate_moves()
				// as that is used by in check detection itself
			$moves[] = $move; //array('m', $board[$x][$y], $x, $y, $to_x, $to_y, $board[$to_x][$to_y]);
		else
			{
			if (!in_check($board, $move))
				$moves[] = $move; //array('m', $board[$x][$y], $x, $y, $to_x, $to_y);
			}
		}
	}

// this function returns true only if the move is made on an empty space
// for purposes of ray moves
function attempt_a_move(&$moves, &$board, $x, $y, $to_x, $to_y, $restrict_type)
	{
	if ($to_x < 0 || $to_y < 0 || $to_x >= $board['w'] || $to_y >= $board['h'])
		return false;

	if ($board[$to_x][$to_y] == '--') // && ($restrict_type == 'm' || $restrict_type == false))
		{
		record_a_move($moves, $board
			,array('m', $board[$x][$y], $x, $y, $to_x, $to_y)
			, 'm', $restrict_type);
		return true;
		}
	else if ($board[$to_x][$to_y]{0} != $board[$x][$y]{0}) // && ($restrict_type == 'c' || $restrict_type == false))
		{
		record_a_move($moves, $board
			,array('c', $board[$x][$y], $x, $y, $to_x, $to_y)
			, 'c', $restrict_type);
		return false;
		}
	return false;
	}

function attempt_a_ray_move(&$moves, &$board, $x, $y, $ray_x_dir, $ray_y_dir, $restrict_type)
	{
	$ray_x = $ray_x_dir;
	$ray_y = $ray_y_dir;
	while (attempt_a_move($moves, $board, $x, $y, $x + $ray_x, $y + $ray_y, $restrict_type))
		{
		$ray_x += $ray_x_dir;
		$ray_y += $ray_y_dir;
		}
	}

function generate_move(&$moves, &$board, $x, $y, $piece, $restrict_type)
	{
	$piece_type = $piece{1};

	if ($piece_type == 'p') // pawn (or groan)
		{
		$pawn_dir = 1;
		$start_row = 1;
		if ($piece{0} == 'w')
			{
			$pawn_dir = -1;
			$start_row = $board['h'] - 2;
			}

		if ($restrict_type == 'm' || $restrict_type == false)
			{
			attempt_a_move($moves, $board, $x, $y, $x, $y + $pawn_dir, 'm');
			if ($y == $start_row)
				attempt_a_move($moves, $board, $x, $y, $x, $y + $pawn_dir + $pawn_dir, 'm');
			}

		if ($restrict_type == 'c' || $restrict_type == false)
			{
			attempt_a_move($moves, $board, $x, $y, $x + 1, $y + $pawn_dir, 'c');
			attempt_a_move($moves, $board, $x, $y, $x - 1, $y + $pawn_dir, 'c');
			}
		}
	if ($piece_type == 'r' || $piece_type == 'q') // rook (or queen)
		{
		attempt_a_ray_move($moves, $board, $x, $y, 1, 0, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, -1, 0, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, 0, 1, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, 0, -1, $restrict_type);
		}
	if ($piece_type == 'b' || $piece_type == 'q') // bishop (or queen)
		{
		attempt_a_ray_move($moves, $board, $x, $y, 1, 1, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, -1, 1, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, 1, -1, $restrict_type);
		attempt_a_ray_move($moves, $board, $x, $y, -1, -1, $restrict_type);
		}
	if ($piece_type == 'n') // knight
		{
		attempt_a_move($moves, $board, $x, $y, $x+1, $y+2, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-1, $y+2, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x+1, $y-2, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-1, $y-2, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x+2, $y+1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-2, $y+1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x+2, $y-1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-2, $y-1, $restrict_type);
		}
	if ($piece_type == 'k') // king
		{
		attempt_a_move($moves, $board, $x, $y, $x+1, $y+1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x+1, $y, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x+1, $y-1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-1, $y+1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-1, $y, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x-1, $y-1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x, $y+1, $restrict_type);
		attempt_a_move($moves, $board, $x, $y, $x, $y-1, $restrict_type);
		}
	}

function record_a_spawn(&$moves, &$board, $move, $type, $restrict_type)
	{
	if (!in_check($board, $move))
		$moves[] = $move;
	}

function generate_moves(&$board, $restrict_type = false)
	{
	$board['asq'][$board['s']] = array();

	$moves = array();

	// spawn moves
	if ($restrict_type != 'c' && $board[$board['s'].'r'] >= 100)
		{
		$pawn_dir = 1;
		$start_row = 1;
		if ($board['s'] == 'w')
			{
			$pawn_dir = -1;
			$start_row = $board['h'] - 2;
			}

		for($i = 0; $i < $board['w']; ++$i) if ($board[$i][$start_row] == '--')
			record_a_spawn($moves, $board, array('m', $board['s'].'p', -1, -1, $i, $start_row)
				, 'm', $restrict_type);

		if ($board[$board['s'].'r'] >= 300)
		for($i = 0; $i < $board['w']; ++$i) if ($board[$i][$start_row - $pawn_dir] == '--')
			{
			record_a_spawn($moves, $board, array('m', $board['s'].'b', -1, -1, $i, $start_row - $pawn_dir)
				, 'm', $restrict_type);
			record_a_spawn($moves, $board, array('m', $board['s'].'n', -1, -1, $i, $start_row - $pawn_dir)
				, 'm', $restrict_type);
			if ($board[$board['s'].'r'] >= 500)
				record_a_spawn($moves, $board, array('m', $board['s'].'r', -1, -1, $i, $start_row - $pawn_dir)
				, 'm', $restrict_type);
			if ($board[$board['s'].'r'] >= 900)
				record_a_spawn($moves, $board, array('m', $board['s'].'q', -1, -1, $i, $start_row - $pawn_dir)
				, 'm', $restrict_type);
			}
		}

	// board moves
	for ($y = 0; $y < $board['h']; ++$y)
	for ($x = 0; $x < $board['w']; ++$x)
		{
		$piece = $board[$x][$y];
		if ($piece{0} == $board['s'])
			{
			generate_move($moves, $board, $x, $y, $piece, $restrict_type);
			}
		}
	return $moves;
	}

function algebraic($move)
	{
	if ($move[2] == -1) // spawn move
	return ($move[1]{1} == 'p' ? '' : strtoupper($move[1]{1}))
		.'*'
		.chr($move[4]+97).(8-$move[5]);
	return ($move[1]{1} == 'p' ? '' : strtoupper($move[1]{1}))
		.chr($move[2]+97).(8-$move[3])
		.($move[0]=='c'?'x':'-')
		.chr($move[4]+97).(8-$move[5]);
	}

?>
