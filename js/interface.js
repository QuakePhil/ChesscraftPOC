function can_play(which, can)
	{
	if (can == 0)
		$('#play_as_' + which).css('display', 'inline-block');
	else
		$('#play_as_' + which).css('display', 'none');
	}

function play_as(which)
	{
	if ($.trim($("#playername").val()) == "")
		{
		alert('Enter a name');
		return;
		}

	if ($("#vs_ai").is(':checked'))
		socket.h.send('playai|'+game+'|'+which
			+'|'+$('#playername').val());
	else
		socket.h.send('playas|'+game+'|'+which
			+'|'+$('#playername').val());

	if (which == '')
		$('#playername').attr('disabled', 'disabled');
	}

function chat()
	{
	if ($.trim($("#chattext").val()) == "")
		return;

	socket.h.send('chat|'+game+'|'+$('#playername').val()+'|'+$('#chattext').val());

	$("#chattext").val("");
	}
