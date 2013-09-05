//           0     1     2     3     4     5     6     7     8     9     10    11    12
var piece_name = ['',
'white pawn', 'white knight', 'white bishop', 'white rook', 'white queen', 'white king',
'black pawn', 'black knight', 'black bishop', 'black rook', 'black queen', 'black king'];
var enemy_piece = {wk: [7,8,9,10,11], bk: [1,2,3,4,5]};
var piece = ['--', 'wp', 'wn', 'wb', 'wr', 'wq', 'wk', 'bp', 'bn', 'bb', 'br', 'bq', 'bk'];
var piece_num = {"--": 0, wp: 1, wn: 2, wb: 3, wr: 4, wq: 5, wk: 6, bp: 7, bn: 8, bb: 9, br: 10, bq: 11, bk: 12};
var cost =  [0,   100,  300,  300,  500,  900,  0,    100,  300,  300,  500,  900,  0];
var board = {
	width: 8,
	height: 8,
	default_pieces: [10,8,9,11,12,9,8,10,
                  7,7,7,7,7,7,7,7,
                  0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,
                  1,1,1,1,1,1,1,1,
                  4,2,3,5,6,3,2,4],
	pieces: [],
	encode: function()
		{
		var ret = '';
		ret += board.width.toString().length > 1 ? board.width : '0' + board.width;
		ret += board.height.toString().length > 1 ? board.height : '0' + board.height;
		for (var i = 0; i < board.width * board.height; ++i)
			ret += piece[board.pieces[i]];
		ret += side;
		return ret;
		},
	decode: function(coded)
		{
		board.width = coded.substring(0, 2) * 1;
		board.height = coded.substring(2, 4) * 1;
		for (var i = 4, j = 0; i < coded.length; i += 2, ++j)
			board.pieces[j] = piece_num[coded.substring(i, i+2)];
		side = coded.substring(coded.length-1, coded.length) * 1;
		update_board();
		}
};

var coordinate = [];

function update_board(create)
	{
	for (var i = 0; i < board.width * board.height; ++i)
		$("[alt="+i+"]").attr("src", "img/"+piece[board.pieces[i]]+".png");
	think();
	}

var move_countdown = 0;
var current_countdown;
function countdown(reset)
	{
	if (reset > 1)
		{
		move_countdown = reset;
		clearTimeout(current_countdown);
		}
	else
		--move_countdown;
	$('#countdown').html(move_countdown + ' seconds left to move');
	if (move_countdown == 0)
		{
		white.resource += 50;
		black.resource += 50;
		side = (side == 0? 1:0);
		socket.h.send('move|'+game+'|'
			+(side*1)+'|'+board.encode()+'|'+white.resource+'|'+black.resource);
		}
	else
		{
		current_countdown = setTimeout('countdown(0)', 1000);
		}
	}

function draw_board(create)
	{
	var i;

	var html = "<div id=status></div>";
	html += "<div id=top_status></div>";
	for (i = 7; i < 12; ++i)
		html += "<img class=spawn src=img/" + piece[i] + ".png title='Cost: " + cost[i] + "' alt=" + cost[i] + ">";
	
	html += "<table border=1>";

	for (var x = 0, i = 0; x < board.height; ++x)
		{
		html += "<tr>";
		for (var y = 0; y < board.width; ++y, ++i)
			{
			if (create)
				{
				coordinate[i] = [x, y];
				board.pieces[i] = 0;
				if (x == 0 && y == Math.floor(board.width / 2))
					{
					black.x = x;
					black.y = y;
					black.i = i;
					board.pieces[i] = 12;
					}
				if (x == board.height - 1 && y == Math.floor(board.width / 2))
					{
					white.x = x;
					white.y = y;
					white.i = i;
					board.pieces[i] = 6;
					}
				}
			if (((x * (board.width % 2 + 1)) + i) % 2 == 0)
				html += "<td bgcolor=white>";
			else
				html += "<td bgcolor=lightgrey>";
			html += "<img src=img/" + piece[board.pieces[i]] + ".png alt=" + i + " />";
			html += "</td>";
			}
		html += "</tr>";
		}
	html += "</table>";
	for (i = 1; i < 6; ++i)
		html += "<img class=spawn src=img/" + piece[i] + ".png title='Cost: " + cost[i] + "' alt=" + cost[i] + ">";
	html += "<div id=bottom_status></div>";
	html += "<div id=action>&nbsp;</div>";
	html += "<div id=countdown>&nbsp;</div>";
	$("#board").html(html);

	$("td").mouseup(function()
		{
		if (dragging != 0)
			{
			var destination = this.childNodes[0].src.replace(/^.*\/|\.[^.]*$/g, '');
			var dest_pos = this.childNodes[0].alt;
			var dest_x = coordinate[dest_pos][0];
			var dest_y = coordinate[dest_pos][1];
			var drag_ok = true;
			var casualty = this.childNodes[0].src.replace(/^.*\/|\.[^.]*$/g, '');

			if ((side == 0 && dragging.substring(0,1) != 'w') ||
			    (side != 0 && dragging.substring(0,1) != 'b'))
				{
				$('#action').html('Not your turn');
				drag_ok = false;
				}
			else if (drag_pos == -1) // spawned piece
				{
				if (casualty != '--')
					drag_ok = false;

				if (dragging == 'bp' && dest_x != 1)
					drag_ok = false;
				else if (dragging.substring(0,1) == 'b' && dragging != 'bp' && dest_x != 0)
					drag_ok = false;

				if (dragging == 'wp' && dest_x != board.height - 2)
					drag_ok = false;
				else if (dragging.substring(0,1) == 'w' && dragging != 'wp' && dest_x != board.height - 1)
					drag_ok = false;
				}
			else
				{
				if (dest_pos == drag_pos)
					drag_ok = false;
				if (destination.substring(0,1) == dragging.substring(0,1))
					drag_ok = false; // same color

				var drag_p = dragging.substring(2,1)
				var drag_x = coordinate[drag_pos][0];
				var drag_y = coordinate[drag_pos][1];

				var line_move = false;
				var diagonal_move = false;
				var dir_x = 0;
				var dir_y = 0;

				if (drag_x == dest_x || drag_y == dest_y)
					{
					dir_x = dest_x - drag_x;
					dir_y = dest_y - drag_y;
					line_move = true;
					}
				if (Math.abs(drag_x - dest_x) == Math.abs(drag_y - dest_y))
					{
					dir_x = dest_x - drag_x;
					dir_y = dest_y - drag_y;
					diagonal_move = true;
					}
				if (dir_x > 0) dir_x = 1;
				if (dir_y > 0) dir_y = 1;
				if (dir_x < 0) dir_x = -1;
				if (dir_y < 0) dir_y = -1;

				if (drag_p == 'r' && !line_move)
					drag_ok = false;
				if (drag_p == 'b' && !diagonal_move)
					drag_ok = false;
				if (drag_p == 'q' && !diagonal_move && !line_move)
					drag_ok = false;
				if (drag_p == 'k' && Math.max(Math.abs(drag_x - dest_x),
					     Math.abs(drag_y - dest_y)) > 1)
					drag_ok = false;
				if (drag_p == 'n')
					{
					if ((Math.abs(drag_x - dest_x) == 1 &&
					     Math.abs(drag_y - dest_y) == 2)
					  ||(Math.abs(drag_x - dest_x) == 2 &&
					     Math.abs(drag_y - dest_y) == 1))
						{}
					else
						drag_ok = false;
					}

				if (line_move || diagonal_move) // disallow jump-overs
					{
					for (var check_x = drag_x + dir_x, check_y = drag_y + dir_y;
						check_x != dest_x || check_y != dest_y;
						check_x += dir_x, check_y += dir_y)
						{
						var check_this = $("[alt="+(board.width * check_x + check_y)+"]");
						if (check_this[0].src.replace(/^.*\/|\.[^.]*$/g, '') != '--')
							drag_ok = false;
						}
					}

				if (drag_p== 'p')
					{
					if (dragging == 'bp' && (drag_x > dest_x || drag_x - dest_x < -2))
						drag_ok = false;
					if (dragging == 'wp' && (drag_x < dest_x || drag_x - dest_x > 2))
						drag_ok = false;
					if (dragging == 'bp' && drag_x - dest_x == -2 && drag_x != 1)
						drag_ok = false;
					if (dragging == 'wp' && drag_x - dest_x == 2 && drag_x != board.height - 2)
						drag_ok = false;
					if (drag_y == dest_y)
						{
						if (casualty != '--')
							drag_ok = false;
						}
					else if (Math.abs(drag_y - dest_y) == 1)
						{
						if (dragging == 'bp' && (drag_x > dest_x || drag_x - dest_x < -1))
							drag_ok = false;
						if (dragging == 'wp' && (drag_x < dest_x || drag_x - dest_x > 1))
							drag_ok = false;
						if (casualty == '--')
							drag_ok = false;
						}
					else
						drag_ok = false;
					}

				if (casualty == 'bk' || casualty == 'wk')
					drag_ok = false;

				}

			// are we still in check?
			if (side == 1 && black.still_in_check(dragging, drag_pos, dest_x, dest_y, dest_pos))
				{
				drag_ok = false;
				$('#action').html('Must move out of check');
				}
			if (side == 0 && white.still_in_check(dragging, drag_pos, dest_x, dest_y, dest_pos))
				{
				drag_ok = false;
				$('#action').html('Must move out of check');
				}
				
			if (drag_ok)
				{
				white.resource += 50;
				black.resource += 50;

				side = (side == 0? 1:0);
				countdown(10);
				var casualty_cost = cost[piece_num[casualty]];
				if (casualty_cost && dragging.substring(0,1) == 'w')
					white.resource += casualty_cost;
				if (casualty_cost && dragging.substring(0,1) == 'b')
					black.resource += casualty_cost;

				if (dragging == 'wp' && dest_x == 0)
					dragging = 'wq';
				else if (dragging == 'bp' && dest_x == board.height - 1)
					dragging = 'bq'; //this.childNodes[0].src = "img/bq.png";
				this.childNodes[0].src = "img/" + dragging + ".png";

				board.pieces[dest_pos] = piece_num[dragging];
				if (dragging == 'wk')
					{
					white.x = dest_x;
					white.y = dest_y;
					white.i = dest_pos;
					}
				if (dragging == 'bk')
					{
					black.x = dest_x;
					black.y = dest_y;
					black.i = dest_pos;
					}

				if (drag_pos != -1)
					{
					board.pieces[drag_pos] = 0;
					drag_src.src = "img/--.png";
					$('#action').html('Moved a ' + piece_name[piece_num[dragging]]);
					}
				else if (dragging)
					{
					if (dragging.substring(0,1) == 'b')
						black.resource -= drag_src.alt;
					if (dragging.substring(0,1) == 'w')
						white.resource -= drag_src.alt;
					$('#action').html('Spawned a ' + piece_name[piece_num[dragging]]);
					}
				update_board();

				socket.h.send('move|'+game+'|'
					+(side*1)+'|'+board.encode()+'|'+white.resource+'|'+black.resource);

				if (white.checkmated() || black.checkmated())
					checkmate = 1;
				}
			dragging = 0;
			}
		return false;
		});

	$("body").mouseup(function()
		{
		dragging = 0;
		return false;
		});

	$("td img,img.spawn").mousedown(function()
		{
		if (checkmate == 1)
			return false;
		if (my_side != side)
			{
			$('#action').html('Not your turn');
			return false;
			}

		if (dragging == 0)
			{
			dragging = this.src.replace(/^.*\/|\.[^.]*$/g, '');
			drag_src = this;
			if ($(this).hasClass("spawn"))
				{
				drag_pos = -1;
				if (dragging.substring(0,1) == 'w' && white.resource < this.alt)
					{
					$('#action').html('Can\'t afford a ' + piece_name[piece_num[dragging]] + ' for ' + this.alt);
					dragging = 0;
					}
				else if (dragging.substring(0,1) == 'b' && black.resource < this.alt)
					{
					$('#action').html('Can\'t afford a ' + piece_name[piece_num[dragging]] + ' for ' + this.alt);
					dragging = 0;
					}
				}
			else
				drag_pos = this.alt;

			if (dragging == '--')
				dragging = 0;
			else
				{
				}

			if (dragging != 0 && drag_pos == -1)
				$('#action').html('Spawning a ' + piece_name[piece_num[dragging]] + ' for ' + this.alt);
			else if (dragging != 0)
				$('#action').html('Moving a ' + piece_name[piece_num[dragging]]);
			}
		else
			{
			dragging = 0;
			alert("Booboo while dragging");
			}
		return false;
		});

	}
