<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style type="text/css" media="all">
.scan {font-weight: bold;color: #385f95;font-size: 24px;text-align: center}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("wireless"); 

var http_request = false;
function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function RadarHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
	parent.adjustMyFrameHeight();
}

function parseAllData(value)
{
	if(1==value)
	{
		parent.onChangeHeadMenu(35, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Basic'));
		window.location.href= '/local/advance/wlan_5g.asp';
	}
	else
	{
		setTimeout('radarRequest()',10000);
	}
}

function radarRequest()
{
	makeRequest("/goform/getDFSState", "n/a", RadarHandler);
}

function initTranslation()
{
	e = document.getElementById("survey_waiting");
	e.innerHTML = _("radar detect");
}
function updateState()
{
	  initTranslation();
	  setTimeout('radarRequest()',8000);
}
</script>
</head>
<body onload="updateState()">
<form method="post" name="SURVEY_WAITING">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="GeneralWirelessScan"></li>
<li class="w_text">
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td ><center>
<img src="/local/advance/images/loading.gif" width="83" height="83" />
</center></td>
</tr>
<tr>
<td><Center><span style="font-weight: bold;color: #385f95;font-size: 24px;text-align: center;"><a id="survey_waiting">Radar detecting, please wait ......</a></span></cneter></td>
<!--
<td><li class="scan" id="survey_waiting">Scanning AP, please wait ......,</li></td>
-->
</tr>
</table>
</li>
<li></li>
<li></li>
<li></li>
<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
     <input type=hidden value="/local/advance/ur_site_waiting.asp" name="ur_site_waiting_url">
</li>
</center>
</ul>
</div>
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
