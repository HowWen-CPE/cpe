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


function initTranslation()
{
	e = document.getElementById("survey_waiting");
	e.innerHTML = _("survey waiting");
}
function updateState()
{
	  initTranslation();
	  setTimeout("window.location.href= '/local/advance/rap_scanning.asp'",12000); 
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
<td><li class="scan" id="survey_waiting">Scanning AP, please wait ......,</li></td>
</tr>
</table>
</li>
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
