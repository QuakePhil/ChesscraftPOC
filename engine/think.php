<?php

function echox($what, $depth)
	{
	for ($i = 0; $i < 10-$depth; ++$i)
		echo "  ";
	echo $what;
	}

function quiesce(&$board)
	{
	$max = -99999;

	$moves = generate_moves($board, 'c');
	if ($moves) foreach ($moves as $move)
		{
		make_move($board, $move);
		$score = -1 * quiesce($board);
		unmake_move($board, $move);

		if ($score > $max)
			$max = $score;
		}
	else
		return evaluate($board);

	return $max;
	}

function negamax(&$board, $depth)
	{
//	echox("negamax($depth)", $depth);
	if ($depth == 0)
		{
//		echo("(zero ".evaluate($board).")\n");
		return quiesce($board);
		}

	$max = -99999;
	
	$moves = generate_moves($board);
	if ($moves) foreach ($moves as $move)
		{
		make_move($board, $move);
//		echox($depth.": ".algebraic($move)."\n", $depth);
		$score = -1 * negamax($board, $depth - 1);
		unmake_move($board, $move);
		//echo("(curmax: $max, score ret: $score)");
		if ($score > $max)
			{
			$board['pv'][$depth] = $move;
			// do we need to update/delete other pvs here?
			$max = $score;
//			echo("(updmax[$depth])".algebraic($move)."\n");
			}
		}

//	echo("($depth max is $max)\n");
	return $max;
	}

function think(&$board)
	{
	$depth = 4;
	negamax($board, $depth);
	echo "PV:";
	for ($i = $depth; $i > 0; --$i)
		echo " ".algebraic($board['pv'][$i]);
	echo "\n";
	}

?>
