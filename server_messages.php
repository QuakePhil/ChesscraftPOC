<?php
//function wsOnMessage($clientID, $message, $messageLength, $binary)

// messages higher than protocol
if ($message == 'close')
	{
	foreach ($db as $game => $db_game)
		{
		if ($db_game['white'] == $clientID)
			unset($db[$game]['white']);
		if ($db_game['black'] == $clientID)
			unset($db[$game]['black']);
		if ($db_game['clients']) foreach ($db_game['clients'] as $thisclientID => $thisname)
			if ($thisclientID != $clientID)

			wsSend($thisclientID, 'chat|'.$game.'|'.$db[$game]['clients'][$clientID].' disconnected');
		unset($db[$game]['clients'][$clientID]);
		}
	$message = false;
	}

// protocol messages
if ($message)
{
$parts = explode('|', $message);
echo "$clientID: $message\n";
$game = $parts[1]*1;

if ($game > 0) // load game
	{
	$game_file = 'games/'.$game.'.txt';
	$game_white = 0;
	$game_black = 0;
	$side = 0;
	$board = 0;
	if (file_exists($game_file))
		{
		$game_data = file($game_file);
		$game_white = trim($game_data[0]);
		$game_black = trim($game_data[1]);
		$side = trim($game_data[2]);
		$board = trim($game_data[count($game_data)-1]);
		list($board, $resources) = explode(",", $board, 2);
		// fixme: send resources too maybe theyre not 500 at start
		}
	else
		{
		$board = $parts[2];

		$h = fopen($game_file, 'w');
		fwrite($h, $game_white."\n");
		fwrite($h, $game_black."\n");
		fwrite($h, $side."\n");
		fwrite($h, $board.",500,500\n");
		fclose($h);
		}
	}

$reply = false;

switch ($parts[0])
	{
	case 'playai':
	case 'playas':
		$parts[3] = trim(strtolower($parts[3]));
		if ($game > 0 && $game_white == 0 && $parts[2] == 'white' && $parts[3])
			{
			$game_white = $parts[3];
			if ($parts[0]) $game_black = '';
			$db[$game]['white'] = $clientID;
			}
		if ($game > 0 && $game_black == 0 && $parts[2] == 'black' && $parts[3])
			{
			$game_black = $parts[3];
			if ($parts[0]) $game_white = '';
			$db[$game]['black'] = $clientID;
			}

		$game_data[0] = $game_white."\n";
		$game_data[1] = $game_black."\n";

		$already_exists = false;
		foreach ($db[$game]['clients'] as $thisclientID => $thisname)
			if ($thisname == $parts[3])
				{
				$already_exists = true;
				break;
				}

		if ($parts[2] == '' && !$already_exists)
			{
			if ($parts[3] == $game_white)
				{
				$reply_note = 'you are playing white';
				$db[$game]['white'] = $clientID;
				}
			else if ($parts[3] == $game_black)
				{
				$reply_note = 'you are playing black';
				$db[$game]['black'] = $clientID;
				}
			else
				$reply_note = 'you are observing';
			$db[$game]['clients'][$clientID] = $parts[3];
			$reply = array(
				'ok',
				$game,
				$game_white,
				$game_black,
				$side,
				$board);
			}
		else if ($game_data[0] != $game_data[1] && !$already_exists)
			{
			$db[$game]['clients'][$clientID] = $parts[3];

			$h = fopen($game_file, 'w');
			foreach ($game_data as $game_data_line)
				fwrite($h, $game_data_line);
			fclose($h);
			$reply = array(
				'ok',
				$game,
				$game_white,
				$game_black,
				$side,
				$board);
			}
		else
			$reply = array('oops', $game, 'that name is already taken');
	break;

	case 'chat':
		// fixme: check if this is a client in this game first?
		$reply_all = array(
		'chat',
		$game,
		trim($parts[2]) . ': ' . trim($parts[3])
		);
	break;
	case 'move':
		if (($parts[2] == 0 && $game_white == '') || ($parts[2] == 1 && $game_black == ''))
			{
//var_dump("ai ./engine_c/engine $parts[3],$parts[4],$parts[5] 1.0");
			$ai = explode("\n", `./engine_c/engine $parts[3],$parts[4],$parts[5] 1.0`);
//var_dump($ai);
			$ai_board = explode(",", $ai[count($ai)-2]);
//var_dump($ai_board);
			$reply_all = array(
				'countdown',
				$game,
				$game_white,
				$game_black,
				$parts[2] == 0 ? 1 : 0,
				$ai_board[0],
				$ai_board[1],
				$ai_board[2]);
			$h = fopen($game_file, 'a');
			fwrite($h, trim($ai_board[0]).",".($ai_board[1]*1).",".($ai_board[2]*1)."\n");
			fclose($h);
			}
		else
			{
			$reply_all = array(
				'countdown',
				$game,
				$game_white,
				$game_black,
				$parts[2],
				$parts[3],
				$parts[4],
				$parts[5]);
			$h = fopen($game_file, 'a');
			fwrite($h, trim($parts[3]).",".($parts[4]*1).",".($parts[5]*1)."\n");
			fclose($h);
			}
	break;
	case 'ohai':
		$reply = array(
			'sup son',
			$game,
			$game_white,
			$game_black,
			$side,
			$board);
	break;
	default:
	break;
	}

if ($reply_note)
	wsSend($clientID, implode('|', array('note', $game, $reply_note)));

if ($reply)
	wsSend($clientID, implode('|', $reply));

if ($reply_all)
	{
	foreach ($db[$game]['clients'] as $thisclientID => $thisname)
		wsSend($thisclientID, implode('|', $reply_all));
	}

var_dump($db);

}
?>

