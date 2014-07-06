<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/channel_sel.js"></script>
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

var dtim = '<% getCfgZero(1,"DTIM"); %>';
var txPower = '<% getCfgZero(1, "TxPower"); %>';
var beacon = '<% getCfgZero(1,"beacon"); %>';
var powerSelect = <% powerSelect(); %>;

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function CheckValue()
{
	/* Check beacon interval */
	if (document.wireless_advanced.bcn_int.value == "" )
	{
		alert('Please specify Beacon Interval');
		document.wireless_advanced.bcn_int.focus();
		document.wireless_advanced.bcn_int.select();
		return false;
	}
	document.wireless_advanced.bcn_int.value = 
		parseInt(document.wireless_advanced.bcn_int.value+"");
	if (isNaN(document.wireless_advanced.bcn_int.value) || 
		document.wireless_advanced.bcn_int.value < 100 || 
		document.wireless_advanced.bcn_int.value > 1000)
	{
		alert('Invalid Beacon Interval');
		document.wireless_advanced.bcn_int.focus();
		document.wireless_advanced.bcn_int.select();
		return false;
	}
	
	//Check DTIM
	if (document.wireless_advanced.dtim.value == "" )
	{
		alert('Please specify DTIM Length!');
		document.wireless_advanced.dtim.focus();
		document.wireless_advanced.dtim.select();
		return false;
	}
	document.wireless_advanced.dtim.value = 
		parseInt(document.wireless_advanced.dtim.value+"");
	if (isNaN(document.wireless_advanced.dtim.value) || document.wireless_advanced.dtim.value < 1 || document.wireless_advanced.dtim.value > 255)
	{
		alert('Invalid DTIM value !');
		document.wireless_advanced.dtim.focus();
		document.wireless_advanced.dtim.select();
		return false;
	}
	
	showWebMessage(2, "");
	return true;
}

function initTranslation()
{
	
	var e = document.getElementById("AdvanceTitle");
	e.innerHTML = _("advance title");

	e = document.getElementById("AdvanceTXPower");
	e.innerHTML = _("advance tx power");

	e = document.getElementById("AdvanceBeaconInterval");
	e.innerHTML = _("advance beacon interval");
	
	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function refreshPower(selection)
{
	removeAllFromTheList(document.wireless_advanced.tx_power);
	for(i=0;i<selection.length-1;i++) {
		addValueTextToList(document.wireless_advanced.tx_power,
			selection[i][0], selection[i][1], selection[i][0]==txPower);
	}
}
	

function initValue()
{
	initTranslation();
	document.wireless_advanced.dtim.value = dtim;
	document.wireless_advanced.bcn_int.value = beacon;
	
	refreshPower(powerSelect);
	/*
	if (txPower == 0){
		document.wireless_advanced.tx_power.options.selectedIndex = 0;
	}else if (txPower == 2){
		document.wireless_advanced.tx_power.options.selectedIndex = 1;	
	}else if (txPower == 4){
		document.wireless_advanced.tx_power.options.selectedIndex = 2;	
	}else if (txPower == 6){
		document.wireless_advanced.tx_power.options.selectedIndex = 3;	
	}else if (txPower == 8){
		document.wireless_advanced.tx_power.options.selectedIndex = 4;	
	}else if (txPower == 10){
		document.wireless_advanced.tx_power.options.selectedIndex = 5;	
	}else if (txPower == 12){
		document.wireless_advanced.tx_power.options.selectedIndex = 6;	
	}else if (txPower == 14){
		document.wireless_advanced.tx_power.options.selectedIndex = 7;	
	}else if (txPower == 16){
		document.wireless_advanced.tx_power.options.selectedIndex = 8;	
	}else if (txPower == 18){
		document.wireless_advanced.tx_power.options.selectedIndex = 9;	
	}
	*/


	show_div(true, "AdvanceDTIM");


	var Enable_SSID  = '<% getCfgZero(1, "EnableSSID" ); %>';
	if (Enable_SSID == "0"){
		document.wireless_advanced.bcn_int.disabled =true;
		document.wireless_advanced.dtim.disabled = true;
		document.wireless_advanced.tx_power.disabled = true;
		document.wireless_advanced.GeneralApply.disabled = true;
		document.wireless_advanced.GeneralReset.disabled = true;
	}
	else {
		document.wireless_advanced.bcn_int.disabled =false;
		document.wireless_advanced.dtim.disabled = false;
		document.wireless_advanced.tx_power.disabled = false;
		document.wireless_advanced.GeneralApply.disabled = false;
		document.wireless_advanced.GeneralReset.disabled = false;
	}
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/wifiAdvanced" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
              
<li class="title" id="AdvanceTitle">Wireless Advanced Setup </li>

<span id="AdvanceDTIM" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id="dtim">DTIM</td>
<td><input type=text maxlength="3" size="15" value="" name="dtim" onkeyup="value=value.replace(/[^\d]/g,'')"/> 
(1 ~ 255)</td>
</tr>  
</table>
</li> 
</span>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id ="AdvanceTXPower">Output Power</td>
<td>
<select name="tx_power" size="1">
<!--
<option value = 0>Full</option>
<option value = 2>-1 dB</option>
<option value = 4>-2 dB</option>
<option value = 6>-3 dB(Half)</option>
<option value = 8>-4 dB</option>
<option value = 10>-5 dB</option>
<option value = 12>-6 dB(Quarter)</option>
<option value = 14>-7 dB</option>
<option value = 16>-8 dB</option>
<option value = 18>-9 dB(Eighth)</option>
-->
</select>
</td>	
</tr>  
</table>
</li>  

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id ="AdvanceBeaconInterval">Beacon Interval</td>
<td>
	<input type=text maxlength="4" size="15" name="bcn_int" onkeyup="value=value.replace(/[^\d]/g,'')"/>(100-1000)ms
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
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">        
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
