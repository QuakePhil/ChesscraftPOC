<?php

function show_board(&$board, $prefix = '')
	{
	echo $prefix. $board['s']." to move (wr: $board[wr]) (br: $board[br])";
	if (in_check($board)) echo " (in check)";
	echo "\n";
	print $prefix."- ";
	for ($x = 0; $x < $board['w']; ++$x)
		print chr($x + 97)." ";
	print "\n";

	for ($y = 0; $y < $board['h']; ++$y)
		{
		print $prefix.(8-$y)." ";
		for ($x = 0; $x < $board['w']; ++$x)
			{
			print $board[$x][$y];
			}
		print "\n";
		}
	}

function decode_board($board)
	{
	$ret['w'] = substr($board, 0, 2);
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
	}

?>
