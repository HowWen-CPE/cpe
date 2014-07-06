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
td {white-space: nowrap;}
.scan {font-weight: bold;color: #385f95;font-size: 24px;text-align: center}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var APNUM_MAX = 256;

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

function rogueAPHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
	parent.adjustMyFrameHeight();
}

function parseAllData(str)
{
	var i = 0;
	
	var APList = new Array();
	APList = str.split("\n");
	var apNum = APList.length - 1;

	document.getElementById("waitingtable").style.visibility="hidden";
	document.getElementById("waitingtable").style.display="none";
	document.getElementById("roguetable").style.visibility="";
	document.getElementById("roguetable").style.display="";
	
	if (apNum > 256) 
	{
		alert("Rogue AP Num is more than 256");
		return false;
	}

	for(i=0; i<apNum; i++)
	{
		var APDes = new Array();
		//APDes = APList[i].split("\r");
		if (APList[i].split("\r").length != 9)
		{
			//Bug 5420: IE Charset
			if(APList[i].split("\r").length != 8)
			{
				continue;
			}
			else
			{
				APDes[0] = APList[i].split("\r")[0].slice(0,-18);
				APDes[1] = APList[i].split("\r")[0].slice(-17);
				APDes[2] = APList[i].split("\r")[1];
				APDes[3] = APList[i].split("\r")[2];
				APDes[4] = APList[i].split("\r")[3];
				APDes[5] = APList[i].split("\r")[4];
				APDes[6] = APList[i].split("\r")[5];
				APDes[7] = APList[i].split("\r")[6];
				APDes[8] = APList[i].split("\r")[7];
			}
			//alert("AP Description is less than 9");
			//return false;
		}
		else
		{
			APDes = APList[i].split("\r");
		}
		document.getElementById("RogueAP_"+i).style.visibility = "";
		document.getElementById("RogueAP_"+i).style.display = "";
		if(APDes[0].indexOf(" ") != -1){
			var k = 0;
			var tmp_ssid = new Array();
			for(k = 0; k < APDes[0].length; k++ )
			{
				if(APDes[0].charCodeAt(k) == 32)
					tmp_ssid[k] = "&nbsp;";
				else
					tmp_ssid[k] = APDes[0].charAt(k);
			}
							
			document.getElementById("SSID_"+i).innerHTML = tmp_ssid.join("");
		} else {
			document.getElementById("SSID_"+i).innerHTML = APDes[0];
		}
		
		document.getElementById("BSSID_"+i).innerHTML = APDes[1];
		document.getElementById("RSSI_"+i).innerHTML = APDes[2];
		document.getElementById("WMODE_"+i).innerHTML = APDes[3];
		document.getElementById("CHANNEL_"+i).innerHTML = APDes[4];
		document.getElementById("ENCRYP_"+i).innerHTML = APDes[5];
		document.getElementById("AUTH_"+i).innerHTML = APDes[6];
		document.getElementById("MODE_"+i).innerHTML = APDes[7];
		document.getElementById("CAUSE_"+i).innerHTML = APDes[8];
	}
}

function DFSHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			DFSparseAllData(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
	parent.adjustMyFrameHeight();
}

function DFSparseAllData(value)
{
	if(0==value)
	{
		alert('radar detect');
	}
	else
	{
		open_profile_page(false);
	}
}

function DFSRequest()
{
	makeRequest("/goform/getDFSState", "n/a", DFSHandler);
}

function countTime()
{
	//var connectstatus = '<!--#include ssi=getStaConnectionStatus() -->';

	//if (connectstatus == 1)  // 0 is NdisMediaStateConnected, 1 is NdisMediaStateDisconnected
		setTimeout("window.location.reload();", 1000*4);
}

function ssid(ssid)
{
	var i = 0;
	var s;
	var s1;
	var tmp_ssid = new Array();
	for(i = 0; i < ssid.length; i++ )
	{
		s = ssid.charAt(i);
		s1 = ssid.charCodeAt(i);
		if(s1 == 32)
			tmp_ssid[i] = "&nbsp;";
		else
			tmp_ssid[i] = s;
	}
				
	 document.write(tmp_ssid.join(""));
}

function open_connection_page()
{
	cwin = window.open("site_survey_connection.asp","sta_site_survey_connection","toolbar=no, location=yes, scrollbars=yes, resizable=no, width=660, height=600");
}

function open_profile_page(lockap)
{
	//parent.onChangeHeadMenu(48, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('Rogue AP Detection')+' > '+_('Rogue AP Detection'));
	if (true == lockap) {
		window.location.href= '/local/advance/rap_detection.asp';
	} else {
		window.location.reload();
	}
}

function rogue_ap_request()
{
	makeRequest("/goform/getRogueAPList", "n/a", rogueAPHandler);
}

function initTranslation()
{
	var e = document.getElementById("rapTitle");
	e.innerHTML = _("rap title");
	e = document.getElementById("rapTitle1");
	e.innerHTML = _("rap title");
	e = document.getElementById("rapSSID");
	e.innerHTML = _("rap ssid");
	e = document.getElementById("rapBSSID");
	e.innerHTML = _("rap bssid");
	e = document.getElementById("rapRSSI");
	e.innerHTML = _("rap rssi");
	e = document.getElementById("rapWMode");
	e.innerHTML = _("rap wmode");
	e = document.getElementById("rapChannel");
	e.innerHTML = _("rap channel");
	e = document.getElementById("rapEncryp");
	e.innerHTML = _("rap encryption");
	e = document.getElementById("rapAuth");
	e.innerHTML = _("rap authentication");
	e = document.getElementById("rapMode");
	e.innerHTML = _("rap mode");
	e = document.getElementById("rapCause");
	e.innerHTML = _("rap cause");
	e = document.getElementById("rapRescan");
	e.value = _("rap rescan");
	e = document.getElementById("rapGoBack");
	e.value = _("rap go back");
	e = document.getElementById("survey_waiting");
	e.innerHTML = _("survey waiting");
}

function PageInit()
{
	initTranslation();
	rogue_ap_request();
}

</script>
</head>
<body onload="PageInit()">
<form method=post name="sta_site_survey">

<div id="waitingtable">
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
<td><Center><span style="font-weight: bold;color: #385f95;font-size: 24px;text-align: center;"><a id="survey_waiting">Scanning AP, please wait ......</a></span></cneter></td>
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

<div id="roguetable" style="display:none;visibility:hidden;">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="rapTitle"></li>

<li class="w_text2">
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="9"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="rapTitle1"><span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>

<tr >
<!--
<td width="5%"><center><span class="top_left">#</span></center></td>
-->
<td width="24%">
<center>
<span class="top_left" id="rapSSID">SSID</span>
</center>
</td>


<td width="12%">
<center>
<span class="top_font" id="rapBSSID">BSSID</span>
</center>
</td>


<td width="7%">
<center>
<span class="top_font" id="rapRSSI">Signal Strength</span>
</center>
</td>

<td width="5%">
<center>
<span class="top_font" id="rapWMode">Type</span>
</center>
</td>

<td width="5%">
<center>
<span class="top_font" id="rapChannel">Channel</span>
</center>
</td>

<td width="8%">
<center>
<span class="top_font" id="rapEncryp">Encryption</span>
</center>
</td>

<td width="10%">
<center>
<span class="top_font" id="rapAuth">Authentication</span>
</center>
</td>

<td width="7%">
<center>
<span class="top_font" id="rapMode">Mode</span>
</center>
</td>

<td width="11%">
<center>
<span class="top_right" id="rapCause">Cause</span>
</center>
</td>
</tr>
<script language="JavaScript" type="text/javascript">
var num;
for(num=0; num<APNUM_MAX; num++)
{
	document.write("<tr id=RogueAP_"+num);
	document.write(" style='display:none;visibility:hidden;'>");
	
	document.write("<td width='24%'><center>");
	//document.write("<span class='table_left'>");
	document.write("<span id=SSID_"+num);
	document.write(" class='table_left'>");
	document.write("</span></center></td>");
	
	document.write("<td width='12%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=BSSID_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='7%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=RSSI_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='5%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=WMODE_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='5%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=CHANNEL_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='8%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=ENCRYP_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='10%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=AUTH_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='7%'><center>");
	//document.write("<span class='table_font'>");
	document.write("<span id=MODE_"+num);
	document.write(" class='table_font'>");
	document.write("</span></center></td>");

	document.write("<td width='11%'><center>");
	//document.write("<span class='table_right'>");
	document.write("<span id=CAUSE_"+num);
	document.write(" class='table_right'>");
	document.write("</span></center></td>");
	
	document.write(" </tr>");
}
</script>
<tr>
<td height="5" colspan="9"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
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
<li></li>
</ul>
</div>
</li>


<center>
<li class="table_button">
<input type=button value="Rescan" id="rapRescan" onClick="DFSRequest()">&nbsp;
<input type=button name="addProfileButton" value="Add Profile" id="rapGoBack" onClick="open_profile_page(true)">
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

