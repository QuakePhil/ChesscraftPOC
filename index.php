<!DOCTYPE html>
<html>
<head><title>Chesscraft</title>
<style>
img
	{
	width: 32px;
	height: 32px;
	}
</style>
<script type="text/javascript"
	src="https://ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js"></script>
<script type="text/javascript" src="js/interface.js"></script>
<script type="text/javascript" src="js/player.js"></script>
<script type="text/javascript" src="js/board.js"></script>
<script type="text/javascript" src="js/socket.js"></script>
<script type="text/javascript">
var game = <?php echo $_REQUEST['game'] * 1; ?>;
var fps = 10.0;
var checkmate = 0;
var dragging = 0;
var drag_src;
var drag_pos;

var online = 0;
var observing = 0;

var white = new player('wk');
var black = new player('bk');
var side = 0;
var my_side = -1;

var chatlog = [];

var socket = new socket_protocol();

$(document).ready(function(){
	draw_board(true);

//	for (var i = 0; i < 64; ++i)
//		board.pieces[i] = board.default_pieces[i];
//	update_board();

	/* this fucks with the on-demand in check checking (still_in_check)
	setInterval(function()
		{
		think();
		}, 1000.0/fps);
*/
	think();

	$("#chattext").keydown(function(e) {
		if (e.which == 13)
			chat();
		});

	$("#playername").keydown(function(e) {
		if (e.which == 13)
			play_as('');
		});

	socket.initialize();
});

</script>
</head>
<body>
<table border=0>
<tr>
<td valign=bottom><div id=chat style="overflow: auto; height: 300px"></div>
<input type=text id=chattext>
<input type=button value=chat onclick=chat()><br/>
<div id=playas>
<hr>
<input type=checkbox id=vs_ai><label for=vs_ai> play against AI</label><br/>
<input type=button style="display:none" id=play_as_white value='play as white' onclick=play_as('white')>
<input type=button style="display:none" id=play_as_black value='play as black' onclick=play_as('black')>
<input type=button id=play_as_observe value='continue' onclick=play_as('')>
<br/>
Name: <input type=text id=playername></div>
</td>
<td id=board></td>
</table>
</body>
</html>
