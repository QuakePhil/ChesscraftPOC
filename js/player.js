function player(king)
	{
	this.king = king;
	this.x = 0;
	this.y = 0;
	this.i = 0;
	this.resource = 500;
	}

function pieces_between(x1, x2, y1, y2, dir_x, dir_y)
	{
	for (var check_x = x1 + dir_x, check_y = y1 + dir_y;
		check_x != x2 || check_y != y2;
		check_x += dir_x, check_y += dir_y)
		{
		if (board.pieces[board.width * check_x + check_y] != 0)
			return true;
		}
	return false;
	}

player.prototype.checkmated = function() // fixme todo
	{
	return false;
	}

player.prototype.check_in_check = function() // clever function name is clever
	{
	var in_check = false;
	for (var x = 0, i = 0; x < board.height; ++x)
	for (var y = 0; y < board.width; ++y, ++i)
		{
		if (jQuery.inArray(board.pieces[i], enemy_piece[this.king]) != -1)
			{
			var dir_x = 0, dir_y = 0;
			dir_x = x - this.x;
			dir_y = y - this.y;
			if (dir_x > 0) dir_x = 1;
			if (dir_y > 0) dir_y = 1;
			if (dir_x < 0) dir_x = -1;
			if (dir_y < 0) dir_y = -1;

			// check non-ray pieces first
			if (board.pieces[i] == 6 || board.pieces[i] == 12) // kings can check each other?
				{
				if (Math.max(Math.abs(this.x - x), Math.abs(this.y - y)) < 2)
					return true;
				}

			if (board.pieces[i] == 1) // pawns
				{
				if (Math.abs(this.y-y) == 1 && this.x-x==-1)
					return true;
				}
			if (board.pieces[i] == 7) // pawns
				{
				if (Math.abs(this.y-y) == 1 && this.x-x==1)
					return true;
				}

			if (board.pieces[i] == 2 || board.pieces[i] == 8) // knights
				{
				if (Math.abs(this.x-x) == 1 && Math.abs(this.y-y) == 2
				  ||Math.abs(this.x-x) == 2 && Math.abs(this.y-y) == 1)
					return true;
				}

			// check ray pieces now
			if (pieces_between(this.x, x, this.y, y, dir_x, dir_y))
				continue;

			if (board.pieces[i] == 4 || board.pieces[i] == 10) // rooks
				{
				if (this.x == x || this.y == y)
					return true;
				}
			if (board.pieces[i] == 5 || board.pieces[i] == 11) // queens
				{
				if (this.x == x || this.y == y || 
					Math.abs(this.x-x) == Math.abs(this.y-y))
					return true;
				}
			if (board.pieces[i] == 3 || board.pieces[i] == 9) // bishops
				{
				if (Math.abs(this.x-x) == Math.abs(this.y-y))
					return true;
				}
			}
		}
	}

player.prototype.still_in_check = function(dragging, drag_pos, dest_x, dest_y, dest_pos)
	{
	var backup_drag = board.pieces[drag_pos],
		backup_dest = board.pieces[dest_pos]
		backup_x = this.x,
		backup_y = this.y,
		backup_i = this.i;
	board.pieces[dest_pos] = piece_num[dragging];
	if (drag_pos != -1) board.pieces[drag_pos] = 0;
	if (dragging == this.king)
		{
		this.x = dest_x;
		this.y = dest_y;
		this.i = dest_pos;
		}
	if (this.check_in_check())
		{
		if (drag_pos != -1) board.pieces[drag_pos] = backup_drag;
		board.pieces[dest_pos] = backup_dest;
		if (dragging == this.king)
			{
			this.x = backup_x;
			this.y = backup_y;
			this.i = backup_i;
			}
		return true;
		}
	if (drag_pos != -1) board.pieces[drag_pos] = backup_drag;
	board.pieces[dest_pos] = backup_dest;
	if (dragging == this.king)
		{
		this.x = backup_x;
		this.y = backup_y;
		this.i = backup_i;
		}
	return false;
	}

player.prototype.think = function()
	{
	if (!this.check_in_check())
		{
		this.resource += 0;
		return 'Resources: ' + this.resource;
		}
	return '<b>In check (resources: ' + this.resource + ')</b>';
	}

function think()
	{
	$('#top_status').html(black.think());
	$('#bottom_status').html(white.think());
	$('#status').html(side==0?'White to move':'Black to move');
	}
