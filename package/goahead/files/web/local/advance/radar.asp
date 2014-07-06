<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="images/radar.js"></script>
<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("wireless"); 

var nv_radio_off = "<% getRadioStatusASP5G(); %>";

function initTranslation()
{

}
function updateState()
{
  if (nv_radio_off == "ON"){
  	var radar_status = "<% getwifi5GRadarStatus(); %>";
	//alert("radar_status="+radar_status);
      if(radar_status == 2 || radar_status == 1)
	{
	   //showdiv("Radar detecting,please wait ...");
	   setTimeout('myrefresh()',3000); 
	}else{//TODO set page...Wireless Setup
          window.location.href="/local/advance/wlan_5g.asp";
	}
   }		
}
</script>
</head>
<body onload="updateState()">
<form method="post" name="RADAR" action="/goform/radar_detect" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="GeneralWirelessSetup"></li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="33%" id="radar_detect"><font size="2" color="#385f95"><b>Radar detecting, please wait ......</b></font></td>
<td width="67%">
</td>
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
     <input type=hidden value="/local/advance/radar.asp" name="radar_url">
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
