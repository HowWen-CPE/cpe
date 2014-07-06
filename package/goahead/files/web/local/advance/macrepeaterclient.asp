<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

function initTranslation()
{
	var e = document.getElementById("MacRepeaterTitle");
	e.innerHTML = _("Ethernet Client Title");

	var e = document.getElementById("MacRepeaterTitle1");
	e.innerHTML = _("Ethernet Client Title 1");

	var e = document.getElementById("MacRepeaterNumber");
	e.innerHTML = _("Client");

	e = document.getElementById("MacRepeaterIpAddr");
	e.innerHTML = _("IP Address");
		
	e = document.getElementById("MacRepeaterMacAddr");
	e.innerHTML = _("MAC Address");

	e = document.getElementById("MacRepeaterRefresh");
	e.value = _("mac repeater refresh");
}

function PageInit()
{
	initTranslation();
}

</script>
</head>
<body onLoad="PageInit()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="MacRepeaterTitle">Ethernet Client</li>
<li></li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="6"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="MacRepeaterTitle1">Ethernet Client<span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr >
<td width="10%"><center>
<span class="top_left" id="MacRepeaterNumber">#</span>
</center>
</td>
<td width="45%">
<center>
<span class="top_font" id="MacRepeaterMacAddr">MAC Address</span>
</center>
</td>
<td width="45%">
<center>
<span class="top_font" id="MacRepeaterIpAddr">IP Address</span>
</center>
</td>
</tr>
</table>
</li>
<% getmacrepeaterStaInfo(); %>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="5" colspan="6"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table>
</td>
</tr>
</table>
</li>

<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=reset value=Refresh id="MacRepeaterRefresh" onClick="window.location.reload()">
</li>
</center>
</form>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>

