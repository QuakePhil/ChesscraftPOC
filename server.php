<?php
define('CB_SERVER_BIND_HOST', '0.0.0.0');
define('CB_SERVER_BIND_PORT', 43188);
define('CB_MAX_USERNAME_LENGTH', 18);

set_time_limit(0);
require 'ws.php';

$db = array();

// when a client sends data to the server
function wsOnMessage($clientID, $message, $messageLength, $binary)
	{
	global $db;

	if ($messageLength == 0)
		{
		wsClose($clientID);
		return;
		}

	// live code editing for the victory	
	include 'server_messages.php';
	return;
	}

function wsOnOpen($clientID)
	{
	echo "$clientID: WebSocket opened\n";
	}
function wsOnClose($clientID, $status)
	{
	global $db;

	echo "$clientID: WebSocket closed $status\n";
	$message = 'close';
	include 'server_messages.php';
	}

// start the server
echo "Listening: ".CB_SERVER_BIND_HOST.":".CB_SERVER_BIND_PORT."\n";
wsStartServer(CB_SERVER_BIND_HOST, CB_SERVER_BIND_PORT);

?>
