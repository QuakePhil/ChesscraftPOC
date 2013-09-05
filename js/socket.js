function socket_protocol()
	{
	this.h = false;
	}

socket_protocol.prototype.initialize = function()
	{
	this.h = new WebSocket("ws://qw.quakephil.com:43188/chesscraft/server.php");
	this.h.onopen = this.onopen;
	this.h.onclose = this.onclose;
	this.h.onmessage = this.onmessage;
	}

socket_protocol.prototype.onopen = function()
	{
	console.log('Socket has opened');
	// why socket.h. and not this.h. ?
	socket.h.send('ohai|'+game+'|'+board.encode());
	}

socket_protocol.prototype.onclose = function()
	{
	console.log('Socket has closed');
	online = 0;
	}

socket_protocol.prototype.onmessage = function(m)
	{
	console.log(m);
	if (!m) return;
	if (!m.data) return;

	var parts = m.data.split('|');
	if (parts[0] == 'sup son')
		my_side = -1;

	if (parts[0] == 'sup son' || (parts[0] == 'ok' && online > 0) || (parts[0] == 'countdown' && online > 0))
		{
		if (parts.length == 8) // contains resource information
			{
			white.resource = parts[6] * 1;
			black.resource = parts[7] * 1;
			}

		online = parts[1];
		console.log('Joined game ' + online);
		can_play('white', parts[2]);
		can_play('black', parts[3]);

		// name == '' is reserved for AI
		if ($('#playername').val() != '')
			{
			if ($('#playername').val() == parts[2] || $('#playername').val() == parts[3] || observing)
				{
				if ($('#playername').val() == parts[2])
					my_side = 0;
				else if ($('#playername').val() == parts[3])
					my_side = 1;
				$('#playas').css('display', 'none');
				}
			}

		side = parts[4];
		board.decode(parts[5]);
		if (parts[0] == 'countdown')
			countdown(100000);
		}
	else if (parts[0] == 'chat')
		{
		$("#chat").append(parts[2] + "<br>").scrollTop(9999);
		}
	else if (parts[0] == 'oops')
		{
		alert(parts[2]);
		}
	else if (parts[0] == 'note')
		{
		alert(parts[2]);
		}
	}
