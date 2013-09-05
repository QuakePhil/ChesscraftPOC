<?php

function calculate_attack_squares(&$board)
	{
	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	generate_moves($board, 'c');
	$board['s'] = ($board['s'] == 'w' ? 'b' : 'w');
	}

function in_check(&$board, $move = false)
	{
	$other_side = ($board['s'] == 'w' ? 'b' : 'w');


	$remember_board = $board;
	if ($move)
		{
	// c m x y x y 
		$board[$move[4]][$move[5]] = $board[$move[2]][$move[3]];
		if ($move[2] != -1)
			$board[$move[2]][$move[3]] = '--';
		}

//	if ($move) make_move($board, $move);
	calculate_attack_squares($board);

	for ($y = 0; $y < $board['h']; ++$y)
	for ($x = 0; $x < $board['w']; ++$x)
		{
		if ($board[$x][$y] == $board['s'].'k')
			{
			if ($board['asq'][$other_side][$x][$y])
				{
				//if ($move) unmake_move($board, $move);
				$board = $remember_board;
				return true;
				}
			}
		}

	//if ($move) unmake_move($board, $move);
	$board = $remember_board;
	return false;
	}

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
