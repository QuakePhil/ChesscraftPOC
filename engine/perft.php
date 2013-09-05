<?php

function perft(&$board, $depth, $initial_depth = false)
	{
	if ($depth == 0) return 1;

	$nodes = 0;

	$moves = generate_moves($board);
	if ($moves) foreach ($moves as $move)
		{
		if ($initial_depth) echo algebraic($move).": ";
		make_move($board, $move);
		$this_nodes = perft($board, $depth - 1, false);
		$nodes += $this_nodes;
		unmake_move($board, $move);
		if ($initial_depth) echo "$this_nodes\n";
		}

	return $nodes;
	}

function perft_results(&$board)
	{
	for ($depth = 1; $depth < 4; ++$depth)
		{
		echo "Nodes at depth $depth: ".perft($board, $depth, true)."\n";
		show_board($board);
		}
	}

?>
