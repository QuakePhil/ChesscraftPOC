<?php
include('engine/board.php');
include('engine/evaluation.php');
include('engine/think.php');
include('engine/perft.php');
include('engine/moves.php');

$game = file('games/1.txt');

$board = $game[count($game)-1];

$board = 
'0808'.
'----------------'.
'------wn--------'.
'----wr----bp----'.
'----------bk----'.
'--------bp------'.
'----------------'.
'----wp--wpwk--bq'.
'--------------br'.'0,900,250';

$board = 
'0808'.
'brbnbbbqbkbbbnbr'.
'bpbpbpbpbpbpbpbp'.
'----------------'.
'----------------'.
'----------------'.
'----------------'.
'wpwpwpwpwpwpwpwp'.
'wrwnwbwqwkwbwnwr'.'0,12,34';

$board = decode_board($board);

//function initialize(&$board)
//	{
//	calculate_attack_squares($board);
//	}
//
//initialize($board);

show_board($board);

perft_results($board);

?>
