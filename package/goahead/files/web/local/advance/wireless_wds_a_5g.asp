<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/channel_sel.js"></script>
<title>.::Welcome to ::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/channel_sel.js"></script>

<style>
.on {display:block}
.off {display:none}
</style>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var opmode  = <% getCfgZero(1, "OP_Mode"); %>;
var board_type = "<% getCfgZero(1, "board_model"); %>";
var wirelessmode  = <% getCfgGeneral(1, "wireless_mode"); %>;

var wdsswitch  = "<% getCfgGeneral(1, "wds_mode_5g"); %>";

var bssid_1 = "<% getWDSLocalMac(1, 0); %>";
var bssid_2 = "<% getWDSLocalMac(1, 1); %>";
var bssid_3 = "<% getWDSLocalMac(1, 2); %>";
var bssid_4 = "<% getWDSLocalMac(1, 3); %>";


var mode_1="<% getCfgGeneral(1, "wds_sec_mode_5g_1"); %>"; 
var mode_2="<% getCfgGeneral(1, "wds_sec_mode_5g_2"); %>"; 
var mode_3="<% getCfgGeneral(1, "wds_sec_mode_5g_3"); %>"; 
var mode_4="<% getCfgGeneral(1, "wds_sec_mode_5g_4"); %>"; 
var wds_wpa_crypto_5g_1="<% getCfgGeneral(1, "wds_wpa_crypto_5g_1"); %>";
var wds_wpa_crypto_5g_2="<% getCfgGeneral(1, "wds_wpa_crypto_5g_2"); %>";
var wds_wpa_crypto_5g_3="<% getCfgGeneral(1, "wds_wpa_crypto_5g_3"); %>";
var wds_wpa_crypto_5g_4="<% getCfgGeneral(1, "wds_wpa_crypto_5g_4"); %>";
var wds_mac1="<% getCfgGeneral(1, "wds_5g_1"); %>";
var wds_mac2="<% getCfgGeneral(1, "wds_5g_2"); %>";
var wds_mac3="<% getCfgGeneral(1, "wds_5g_3"); %>";
var wds_mac4="<% getCfgGeneral(1, "wds_5g_4"); %>";

function  allTrim(ui){  
    var  notValid=/\s/;
	  
    while(notValid.test(ui)){  
        ui=ui.replace(notValid,"");
	} 
	 
    return ui;
} // \s means blank character, each blank would be replace with void

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
	return true;
}

function isASCII(str)
{
	for( var i = 0; i < str.length; i++)
	{
		 var chr = str.charCodeAt(i);
		 if( chr < 0 || chr > 255 )
		 {
		    return false;
		 }
	}
	
	return true;
}

function style_display_on()
{
	if (window.ActiveXObject){//IE
		return "block";
	}else if (window.XMLHttpRequest){//Mozilla, Safari,...
		return "table-row";
	}
}

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function SecurityModeSwitchInit()
{
	var sec_mode_select;
	var psk_input;
	for( i=1;i<=4;i++) {
		document.getElementById("wds_wpapsk_"+i).disabled = true;

		show_div(false, "div_wpa_psk_"+i);

		sec_mode_select = document.getElementById("wds_sec_mode_"+i);
		if(sec_mode_select.options.selectedIndex == 0) {
			show_div(false, "div_wpa_psk_"+i);	
		}
		else {
			show_div(true, "div_wpa_psk_"+i);
			sec_mode_select.disabled = false;

			psk_input = document.getElementById("wds_wpapsk_"+i);
			if(1==parent.radio_switch) {
				psk_input.disabled = false;
			}
		}

	}
	parent.adjustMyFrameHeight();
}

function SecurityModeSwitch(index)
{
	show_div(false, "div_wpa_psk_"+index);

	var psk_input = document.getElementById("wds_wpapsk_"+index);
	psk_input.disabled = true;

	var mode = document.getElementById("wds_sec_mode_"+index);
	var mode_index = mode.options.selectedIndex;
	
	if (mode_index == 0) {
		show_div(false, "div_wpa_psk_"+index);	
	}
	else{
		show_div(true, "div_wpa_psk_"+index);			
		psk_input.disabled = false;
	}
	parent.adjustMyFrameHeight();
}

function WdsModeOnChange()
{
	if (document.wireless_wds.wds_mode.options.selectedIndex == 0)
	{
		for (i=1;i<=4;i++) {
			show_div(false, "div_wpa_psk_"+i);
			show_div(false, "div_wds_mac_addr_"+i);
			document.getElementById("addprofSecureMode_"+i).style.visibility="hidden";
			document.getElementById("wds_sec_mode_"+i).style.visibility="hidden";
		}
	}
	else
	{ 
		for (i=1;i<=4;i++) {
		document.getElementById("addprofSecureMode_"+i).style.visibility="visible";
		document.getElementById("wds_sec_mode_"+i).style.visibility="visible";

	    show_div(true, "div_wds_mac_addr_"+i);
		updateSecModeFor(i);
		SecurityModeSwitch(i);	
		}
	}
	parent.adjustMyFrameHeight();
}

function initTranslation()
{
	var e = document.getElementById("WDSTitle");
	e.innerHTML = _("WDS Setup");

	e = document.getElementById("WDSModeDisable");
	e.innerHTML = _("general off");
	
	e = document.getElementById("APClientSSID_1");
	e.innerHTML = _("Local MAC Address")+" 1";
	e = document.getElementById("APClientSSID_2");
	e.innerHTML = _("Local MAC Address")+" 2";
	e = document.getElementById("APClientSSID_3");
	e.innerHTML = _("Local MAC Address")+" 3";
	e = document.getElementById("APClientSSID_4");
	e.innerHTML = _("Local MAC Address")+" 4";
	
	
	e = document.getElementById("addprofSecureMode_1");
	e.innerHTML = _("addprof security mode");
	e = document.getElementById("addprofSecureMode_2");
	e.innerHTML = _("addprof security mode");
	e = document.getElementById("addprofSecureMode_3");
	e.innerHTML = _("addprof security mode");
	e = document.getElementById("addprofSecureMode_4");
	e.innerHTML = _("addprof security mode");

	e = document.getElementById("GeneralEncrypOpen_1");
	e.innerHTML = _("general nosecurity");	
	e = document.getElementById("GeneralEncrypOpen_2");
	e.innerHTML = _("general nosecurity");	
	e = document.getElementById("GeneralEncrypOpen_3");
	e.innerHTML = _("general nosecurity");	
	e = document.getElementById("GeneralEncrypOpen_4");
	e.innerHTML = _("general nosecurity");	

	e = document.getElementById("wpapsk_Pass_Phrase_1");
	e.innerHTML = _("general wpapsk presharedkey");
	e = document.getElementById("wpapsk_Pass_Phrase_2");
	e.innerHTML = _("general wpapsk presharedkey");
	e = document.getElementById("wpapsk_Pass_Phrase_3");
	e.innerHTML = _("general wpapsk presharedkey");
	e = document.getElementById("wpapsk_Pass_Phrase_4");
	e.innerHTML = _("general wpapsk presharedkey");

	e = document.getElementById("WDSAPMacAddr1");
	e.innerHTML = _("WDSAPMacAddr") + " 1";
	e = document.getElementById("WDSAPMacAddr2");
	e.innerHTML = _("WDSAPMacAddr") + " 2";
	e = document.getElementById("WDSAPMacAddr3");
	e.innerHTML = _("WDSAPMacAddr") + " 3";
	e = document.getElementById("WDSAPMacAddr4");
	e.innerHTML = _("WDSAPMacAddr") + " 4";	

	e = document.getElementById("WDSModeTitle");
	e.innerHTML = _("WDSMode");	

  	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}	

function updateSecModeFor(index)
{
	var sec_mode;
	switch(index) {
		case 1:
			sec_mode=mode_1;
			break;
		case 2:
			sec_mode=mode_2;
			break;
		case 3:
			sec_mode=mode_3;
			break;
		case 4:
			sec_mode=mode_4;
			break;
		default:
			break;
	}

	var sec_mode_select = document.getElementById("wds_sec_mode_"+index);

	if (sec_mode == "none")
		sec_mode_select.options.selectedIndex = 0;
	else if (sec_mode == "aes")
		sec_mode_select.options.selectedIndex = 1;
	else if (sec_mode == "tkip")
		sec_mode_select.options.selectedIndex = 2;
	else
		sec_mode_select.options.selectedIndex = 0;
}

function disableAll()
{
	document.wireless_wds.wds_mode.disabled = true;
	document.wireless_wds.GeneralApply.disabled = true;
	document.wireless_wds.GeneralReset.disabled = true;
	for( i=1;i<=4;i++) {
		document.getElementById("wds_mac_list_"+i).disabled = true;
		document.getElementById("wds_"+i).disabled = true;
		document.getElementById("wds_sec_mode_"+i).disabled = true;
	}
}

function initValue()
{
	var flag = <% getOnloadDFSState(); %>;
	if(0 == flag)
	{
		window.location.href= '/local/advance/radar_waiting.asp';
	}

	initTranslation();
	if(wdsswitch == "0") {
		document.wireless_wds.wds_mode.options[0].selected = true;
	}
	else if(wdsswitch == "1") {
		document.wireless_wds.wds_mode.options[1].selected = true;
	}
	else {
		alert("wds enable mode switch error");
		document.wireless_wds.wds_mode.options[0].selected = true;
	}

	updateSecModeFor(1);
	updateSecModeFor(2);
	updateSecModeFor(3);
	updateSecModeFor(4);


	if (document.wireless_wds.wds_mode.options.selectedIndex == 0){
		for (i=1;i<=4;i++) {
			show_div(false, "div_wpa_psk_"+i);
			show_div(false, "div_wds_mac_addr_"+i);
			document.getElementById("addprofSecureMode_"+i).style.visibility="hidden";
			document.getElementById("wds_sec_mode_"+i).style.visibility="hidden";
			document.getElementById("wds_sec_mode_"+i).options.selectedIndex = 0;
		}
	}
	else { 
		for (i=1;i<=4;i++) {
			document.getElementById("addprofSecureMode_"+i).style.visibility="visible";
			document.getElementById("wds_sec_mode_"+i).style.visibility="visible";
			show_div(true, "div_wds_mac_addr_"+i);
			SecurityModeSwitch(i);	
		}
	}

	document.getElementById("apcli_bssid_1").innerHTML=bssid_1;
	document.getElementById("apcli_bssid_2").innerHTML=bssid_2;
	document.getElementById("apcli_bssid_3").innerHTML=bssid_3;
	document.getElementById("apcli_bssid_4").innerHTML=bssid_4;

	SecurityModeSwitchInit();
	if(0==parent.radio_switch) {
		disableAll();
	}

	parent.adjustMyFrameHeight();	
}

function checkInjection(str)
{
	var len = str.length;
	for (var i = 0; i < str.length; i++) {
		//if (str.charAt(i) == ';' || str.charAt(i) == ',' ||
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n') {
			return false;
		}
		else
			continue;
	}
	return true;
}
function checkStrInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' ){
			return false;

		}else
	        continue;
	}
	return true;
}

function CheckWpa(index)
{
	var wpa_psk=document.getElementById("wds_wpapsk_"+index);
	if (wpa_psk.value.length < 8) {
		alert("Pass Phrase length should be larger than 8!");
		wpa_psk.focus();
		return false;
	} else {
		var tmp_wds_wpapsk = wpa_psk.value;
		tmp_wds_wpapsk = allTrim(tmp_wds_wpapsk);
		if (! isASCII(tmp_wds_wpapsk)) {
				alert("System only support ASCII characters.");
				wpa_psk.focus();
				return false;
		}
	}

	if (checkHex(wpa_psk.value) == true){ // Hex
		if ((wpa_psk.value.length < 8) || (wpa_psk.value.length > 64)){
			alert('Passphrase is invalid.');
			return false;
		}
	}else{ // ASCII
		if ((wpa_psk.length < 8) || (wpa_psk.value.length > 63)){
			alert('Passphrase is invalid.');
			return false;
		}
	}
	
	return true;
}
function checkmac(mac)
{
   if(mac.value != "00:00:00:00:00:00")
   {
       var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
       if (!re.test(mac.value)) {
       alert('Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !');
       mac.focus();
       return false;
       }
       var address_value=mac.value;
       var value=address_value.substr(0,2);
       var val=parseInt(value,16);
       if(val & 0x01 == 1){
       alert('Broadcast or Multicast MAC Address not support!');
       mac.focus();
       return false;				
       }
       return true;
   }else {
       mac.focus();
       alert('Invalid MAC Address!');
       return false;
   }
}
/*
   if(mac.value.length==0)
   {
   		return false;
   }
*/
function submit_apply()
{

	if(parent.radio_switch == 0)
	{
		alert("current basic page's radio switch is disabled, WDS is forbidden");
		return false;
	}

	if(parent.channel_index==0 && document.wireless_wds.wds_mode.options[0].selected == false)
	{
		alert("You should set a AP's fixed channel first before set wds");
		return false;
	}
		
	var mac1=document.wireless_wds.wds_1;
	var mac2=document.wireless_wds.wds_2;
	var mac3=document.wireless_wds.wds_3;
	var mac4=document.wireless_wds.wds_4;
	if(mac1.value.length >0  && !checkmac(mac1))
	{
		return false;
	}
	if(mac2.value.length >0  && !checkmac(mac2))
	{
		return false;
	}
	if(mac3.value.length >0  && !checkmac(mac3))
	{
		return false;
	}
	if(mac4.value.length >0  && !checkmac(mac4))
	{
		return false;
	}	

	if(bssid_1==mac1.value.toUpperCase() || bssid_1==mac2.value.toUpperCase() || bssid_1==mac3.value.toUpperCase() || bssid_1==mac4.value.toUpperCase())
	{
		alert("mac address confliction");
		return false;
	}
	if(bssid_2==mac1.value.toUpperCase() || bssid_2==mac2.value.toUpperCase() || bssid_2==mac3.value.toUpperCase() || bssid_2==mac4.value.toUpperCase())
	{
		alert("mac address confliction");
		return false;
	}
	if(bssid_3==mac1.value.toUpperCase() || bssid_3==mac2.value.toUpperCase() || bssid_3==mac3.value.toUpperCase() || bssid_3==mac4.value.toUpperCase())
	{
		alert("mac address confliction");
		return false;
	}
	if(bssid_4==mac1.value.toUpperCase() || bssid_4==mac2.value.toUpperCase() || bssid_4==mac3.value.toUpperCase() || bssid_4==mac4.value.toUpperCase())
	{
		alert("mac address confliction");
		return false;
	}
	if(mac1.value.toUpperCase().length>0 && (mac1.value.toUpperCase()==mac2.value.toUpperCase() || mac1.value.toUpperCase()==mac3.value.toUpperCase() || mac1.value.toUpperCase()==mac4.value.toUpperCase()))
	{
		alert("mac address confliction");
		return false;		
	}
	if(mac2.value.toUpperCase().length>0 && (mac1.value.toUpperCase()==mac2.value.toUpperCase() || mac2.value.toUpperCase()==mac3.value.toUpperCase() || mac2.value.toUpperCase()==mac4.value.toUpperCase()))
	{
		alert("mac address confliction");
		return false;		
	}
	if(mac3.value.toUpperCase().length>0 && (mac1.value.toUpperCase()==mac3.value.toUpperCase() || mac2.value.toUpperCase()==mac3.value.toUpperCase() || mac3.value.toUpperCase()==mac4.value.toUpperCase()))
	{
		alert("mac address confliction");
		return false;		
	}
	if(mac4.value.toUpperCase().length>0 && (mac1.value.toUpperCase()==mac4.value.toUpperCase() || mac2.value.toUpperCase()==mac4.value.toUpperCase() || mac3.value.toUpperCase()==mac4.value.toUpperCase()))
	{
		alert("mac address confliction");
		return false;		
	}	
	mac1.value = mac1.value.toUpperCase();
	mac2.value = mac2.value.toUpperCase();
	mac3.value = mac3.value.toUpperCase();
	mac4.value = mac4.value.toUpperCase();

	showWebMessage(2, "");
	return true;	
}

function CheckValue()
{
	if (submit_apply() == false)
	    return false;

	for(i=1;i<=4;i++) {
		if (document.getElementById("wds_sec_mode_"+i).options.selectedIndex == 0) {
			continue;
		}
		else if (document.getElementById("wds_sec_mode_"+i).options.selectedIndex == 1) {
			if(CheckWpa(i)) continue;
			else return false;
		}
		else {
			alert('Unknown Security Mode!');
			return false;
		}
	}
	return true;
}


</script>
</head>
<body onLoad="initValue()">
<form method=post name=wireless_wds action="/goform/wireless5g_wds" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>

<li class='title' id='WDSTitle'>WDS settings</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="40%" id="WDSModeTitle">Mode Switch</td>
<td>
<select name="wds_mode" id="wds_mode" size="1" style="width:202px" onchange="WdsModeOnChange()">
<option value=0 SELECTED id="WDSModeDisable">Disable</option>
<option value=1 id="WDSModeAPBridge">AP+Bridge</option>

</select>
</td>
</tr>
</table>
</li>    

<li class="space3"></li>

<!-- WDS MAC 1-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">   
<tr> 
<td width="40%" id="APClientSSID_1">Local SSID</td>
<td id="apcli_bssid_1"></td>
</tr>
</table>
</li>

<span id="div_wds_mac_addr_1" class="on">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="wds_mac_list_1" name="wds_mac_list_1">
<td width="40%" id="WDSAPMacAddr1">Remote MAC Address</td>
<td><input type=text name="wds_1" id="wds_1" size=20 maxlength=17  style="width:202px" value="<% getCfgGeneral(1, "wds_5g_1"); %>"></td>
</tr>
</table>
</li>
</span>

<!-- Security Mode Switch 1-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode_1">Security Mode</td>
<td>
<select name="wds_sec_mode_1" style="width:202px" id="wds_sec_mode_1" size="1" onchange="SecurityModeSwitch(1)">
<option value="NONE" id="GeneralEncrypOpen_1">No Security</option>
<option value="AES" >AES</option>
<!--<option value="TKIP" >TKIP</option>-->
</select>
</td>
</tr>
</table>
</li>  

<!-- WPA-PSK/ WPA2-PSK 1-->
<span id="div_wpa_psk_1" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr>
<td width="40%" id="wpapsk_Pass_Phrase_1">Pre-Shared Key</td>
<td><input id="wds_wpapsk_1" name="wds_wpapsk_1" size=20 maxlength=64 style="width:200px" value="<% getCfgGeneral(1, "wds_wpapsk_5g_1"); %>"></td>
</tr>
</table>
</li>
</span>

<li class="space3"></li>

<!-- WDS MAC 2-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">   
<tr> 
<td width="40%" id="APClientSSID_2">Local SSID</td>
<td id="apcli_bssid_2"></td>
</tr>
</table>
</li>
  
<span id="div_wds_mac_addr_2" class="on">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="wds_mac_list_2" name="wds_mac_list_2">
<td width="40%" id="WDSAPMacAddr2">Remote MAC Address</td>
<td><input type=text name="wds_2" id="wds_2" size=20 maxlength=17 style="width:202px" value="<% getCfgGeneral(1, "wds_5g_2"); %>"></td>
</tr>
</table>
</li>
</span>

<!-- Security Mode Switch 2-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode_2">Security Mode</td>
<td>
<select name="wds_sec_mode_2" style="width:202px" id="wds_sec_mode_2" size="1" onchange="SecurityModeSwitch(2)">
<option value="NONE" id="GeneralEncrypOpen_2">No Security</option>
<option value="AES" >AES</option>
<!--<option value="TKIP" >TKIP</option>-->
</select>
</td>
</tr>
</table>
</li>  


<!-- WPA-PSK/ WPA2-PSK 2-->
<span id="div_wpa_psk_2" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr>
<td width="40%" id="wpapsk_Pass_Phrase_2">Pre-Shared Key</td>
<td><input id="wds_wpapsk_2"  name="wds_wpapsk_2" size=20 maxlength=64 style="width:200px" value="<% getCfgGeneral(1, "wds_wpapsk_5g_2"); %>"></td>
</tr>
</table>
</li>
</span>

<li class="space3"></li>

<!-- WDS MAC 3-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">   
<tr> 
<td width="40%" id="APClientSSID_3">Local SSID</td>
<td id="apcli_bssid_3"></td>
</tr>
</table>
</li>

<span id="div_wds_mac_addr_3" class="on">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="wds_mac_list_3" name="wds_mac_list_3">
<td width="40%" id="WDSAPMacAddr3">Remote MAC Address</td>
<td><input type=text name="wds_3" id="wds_3" size=20 maxlength=17 style="width:202px" value="<% getCfgGeneral(1, "wds_5g_3"); %>"></td>
</tr>
</table>
</li>
</span>

<!-- Security Mode Switch 3-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode_3">Security Mode</td>
<td>
<select name="wds_sec_mode_3" style="width:202px" id="wds_sec_mode_3" size="1" onchange="SecurityModeSwitch(3)">
<option value="NONE" id="GeneralEncrypOpen_3">No Security</option>
<option value="AES" >AES</option>
<!--<option value="TKIP" >TKIP</option>-->
</select>
</td>
</tr>
</table>
</li>  

<!-- WPA-PSK/ WPA2-PSK 3-->
<span id="div_wpa_psk_3" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr>
<td width="40%" id="wpapsk_Pass_Phrase_3">Pre-Shared Key</td>
<td><input id="wds_wpapsk_3" name="wds_wpapsk_3" size=20 maxlength=64 style="width:200px" value="<% getCfgGeneral(1, "wds_wpapsk_5g_3"); %>"></td>
</tr>
</table>
</li>
</span>


<li class="space3"></li>

<!-- WDS MAC 4-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">   
<tr> 
<td width="40%" id="APClientSSID_4">Local SSID</td>
<td id="apcli_bssid_4"></td>
</tr>
</table>
</li>

<span id="div_wds_mac_addr_4" class="on">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="wds_mac_list_4" name="wds_mac_list_4">
<td width="40%" id="WDSAPMacAddr4">Remote MAC Address</td>
<td><input type=text name="wds_4" id="wds_4" size=20 maxlength=17 style="width:202px" value="<% getCfgGeneral(1, "wds_5g_4"); %>"></td>
</tr>
</table>
</li>
</span>
  

<!-- Security Mode Switch 4-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode_4">Security Mode</td>
<td>
<select name="wds_sec_mode_4" style="width:202px" id="wds_sec_mode_4" size="1" onchange="SecurityModeSwitch(4)">
<option value="NONE" id="GeneralEncrypOpen_4">No Security</option>
<option value="AES" >AES</option>
<!--<option value="TKIP" >TKIP</option>-->
</select>
</td>
</tr>
</table>
</li>  

<!-- WPA-PSK/ WPA2-PSK 4-->
<span id="div_wpa_psk_4" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr>
<td width="40%" id="wpapsk_Pass_Phrase_4">Pre-Shared Key</td>
<td><input id="wds_wpapsk_4" name="wds_wpapsk_4" size=20 maxlength=64 style="width:200px" value="<% getCfgGeneral(1, "wds_wpapsk_5g_4"); %>"></td>
</tr>
</table>
</li>
</span>


<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value="Reset" id="GeneralReset" onClick="window.location.reload()">
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
