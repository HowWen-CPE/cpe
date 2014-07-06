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

var apcli_include = '<% getWlanApcliBuilt(); %>';
var channelAN = <% Channellist5g(); %>;
var channelANHT80 = <% Channellist5gHT80(); %>;

var nv_channel = <% getCfgZero(1, "Channel_5G"); %>;
var nv_ext = <% getCfgZero(1, "Extcha_5G"); %>;

var MainIntra_BSS = "<% getCfgZero(1, "MainIntra_BSS_5G"); %>";
var PhyMode  = '<% getCfgZero(1, "Wireless5GMode"); %>';
var ht_bw = '<% getCfgZero(1, "HT_5G_BW"); %>';

function isASCII(str)
{
	for( var i = 0; i < str.length; i++) {
		 var chr = str.charCodeAt(i);
		 if( chr < 0 || chr > 255 ) {
		    return false;
		 }
	}
	
	return true;
}

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n'){
			return false;

		}else
	        continue;
	}
	return true;
}

function style_display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Safari,...
		return "table-row";
	}
}

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
		alert('unavailable radar channel');
	}
	else
	{
		submit_apply();
	}
}

function radarRequest()
{
	var chanNum = document.wireless_basic.sz11gChannel.value;
	var extChanNum = document.wireless_basic.n_extcha.value;
	makeRequest("/goform/chanIsRadar", chanNum + "," +extChanNum, RadarHandler);
}

function Channel_BandWidth_onChange()
{
	if (document.wireless_basic.n_bandwidth.options.selectedIndex == 1) {
		document.wireless_basic.n_extcha.disabled = false;
	}
	else {
		document.wireless_basic.n_extcha.disabled = true;
	}
	parent.adjustMyFrameHeight();
}

function wirelessModeChange()
{
	show_div(false, "div_ht_phy_1_3");
	show_div(false, "div_networkmode_id");
	show_div(true, "div_extension_channel");
	show_div(true, "div_ht_phy_1_3");
	show_div(true, "div_networkmode_id");
	parent.adjustMyFrameHeight();
	if (document.wireless_basic.wirelessmode.options.selectedIndex == 1) {
		removeAllFromTheList(document.wireless_basic.n_bandwidth );
		addValueTextToList(document.wireless_basic.n_bandwidth, 0, "20MHz", 1*ht_bw == 0 );
		addValueTextToList(document.wireless_basic.n_bandwidth, 1, "40MHz", 1*ht_bw == 1 );
		addValueTextToList(document.wireless_basic.n_bandwidth, 2, "80MHz", 1*ht_bw == 2 );
	}
	else {
		removeAllFromTheList(document.wireless_basic.n_bandwidth );
		addValueTextToList(document.wireless_basic.n_bandwidth, 0, "20MHz", 1*ht_bw == 0 );
		addValueTextToList(document.wireless_basic.n_bandwidth, 1, "20MHz/40MHz", 1*ht_bw == 1 );
	}
}

function check_same_ssid()
{
	for (var i = 1; i < 8; i++){
		if (eval("document.wireless_basic.mssid_"+i).value != ""){
			if (i == 1){
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_1).value ){
					return false;	
				}
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_2).value ){
					return false;	
				}
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}	
			}
			else if (i == 2){
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_2).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}			
			}
			else if (i == 3){
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 4){
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 5){
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 6){
				if (eval(document.wireless_basic.mssid_5).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_5).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 7){
				if (eval(document.wireless_basic.mssid_6).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
		}
	}
	return true;
}

function submit_apply()
{
	var submit_ssid_num;
			
	if (document.wireless_basic.ssid.value == "")
	{
		alert("Please enter SSID!");
		document.wireless_basic.ssid.focus();
		document.wireless_basic.ssid.select();
		return false;
	}

	if ((document.wireless_basic.enablessid1.checked == true) 
		&& (document.wireless_basic.mssid_1.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_1.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid2.checked == true) 
		&& (document.wireless_basic.mssid_2.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_2.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid3.checked == true) 
		&& (document.wireless_basic.mssid_3.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_3.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid4.checked == true) 
		&& (document.wireless_basic.mssid_4.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_4.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid5.checked == true) 
		&& (document.wireless_basic.mssid_5.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_5.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid6.checked == true) 
		&& (document.wireless_basic.mssid_6.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_6.focus();
		return false;
	}
	if ((document.wireless_basic.enablessid7.checked == true) 
		&& (document.wireless_basic.mssid_7.value == ""))
	{
		alert("Please enter SSID!");
		document.wireless_basic.mssid_7.focus();
		return false;
	}
/*
	if(checkInjection(document.wireless_basic.ssid.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.ssid.focus();
		document.wireless_basic.ssid.select();
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_1.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_1.focus();
		document.wireless_basic.mssid_1.select();
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_2.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_2.focus();
		document.wireless_basic.mssid_2.select();
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_3.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_3.focus();
		document.wireless_basic.mssid_3.select();
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_4.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_4.focus();
		document.wireless_basic.mssid_4.select();
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_5.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_5.focus();
		document.wireless_basic.mssid_5.select();
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_6.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_6.focus();
		document.wireless_basic.mssid_6.select();
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_7.value) == false){
		alert('System do not support specific \, character.');
		document.wireless_basic.mssid_7.focus();
		document.wireless_basic.mssid_7.select();
		return false;
	}	
*/

	if(isASCII(document.wireless_basic.ssid.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.ssid.focus();
		document.wireless_basic.ssid.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_1.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_1.focus();
		document.wireless_basic.mssid_1.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_2.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_2.focus();
		document.wireless_basic.mssid_2.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_3.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_3.focus();
		document.wireless_basic.mssid_3.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_4.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_4.focus();
		document.wireless_basic.mssid_4.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_5.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_5.focus();
		document.wireless_basic.mssid_5.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_6.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_6.focus();
		document.wireless_basic.mssid_6.select();
		return false;
	}	
	if(isASCII(document.wireless_basic.mssid_7.value) == false){
		alert('System only support ASCII characters.');
		document.wireless_basic.mssid_7.focus();
		document.wireless_basic.mssid_7.select();
		return false;
	}	


	if(check_same_ssid() == false){
		alert("Duplicate ssid names!");	
             return false;
	}
	submit_ssid_num = 1;

	for (i = 1; i < 8; i++) {
		if (eval("document.wireless_basic.mssid_"+i).value != "") {
			if (i == 8) {
				if (1*apcli_include == 0)
					submit_ssid_num++;
			}
			else
				submit_ssid_num++;
		}
	}

	document.wireless_basic.bssid_num.value = submit_ssid_num;		


	if(document.wireless_basic.mssid_1.value == "")
		document.wireless_basic.bssid_num1.value = "0";
	else
		document.wireless_basic.bssid_num1.value = "1";
	
	if(document.wireless_basic.mssid_2.value == "")
		document.wireless_basic.bssid_num2.value = "0";
	else
		document.wireless_basic.bssid_num2.value = "1";

	if(document.wireless_basic.mssid_3.value == "")
		document.wireless_basic.bssid_num3.value = "0";
	else
		document.wireless_basic.bssid_num3.value = "1";	

	if(document.wireless_basic.mssid_4.value == "")
		document.wireless_basic.bssid_num4.value = "0";
	else
		document.wireless_basic.bssid_num4.value = "1";	

	if(document.wireless_basic.mssid_5.value == "")
		document.wireless_basic.bssid_num5.value = "0";
	else
		document.wireless_basic.bssid_num5.value = "1";	

	if(document.wireless_basic.mssid_6.value == "")
		document.wireless_basic.bssid_num6.value = "0";
	else
		document.wireless_basic.bssid_num6.value = "1";	

	if(document.wireless_basic.mssid_7.value == "")
		document.wireless_basic.bssid_num7.value = "0";
	else
		document.wireless_basic.bssid_num7.value = "1";	

	showWebMessage(2, "");
	document.wireless_basic.submit();
}

function initTranslation()
{

	var e = document.getElementById("GeneralWirelessSetup");
	e.innerHTML = _("general wireless setup");

	e = document.getElementById("GeneralWirelessLAN");
	e.innerHTML = _("general wireless lan");
	
	e = document.getElementById("GeneralWirelessSSID");
	e.innerHTML = _("general wireless ssid");
	
	e = document.getElementById("GeneralHideSSID1");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralmSSIDHide1");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide2");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide3");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide4");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide5");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide6");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide7");
	e.innerHTML = _("general mssid hide");

	e = document.getElementById("AdvanceNetMode");
	e.innerHTML = _("advance net mode");
	e = document.getElementById("AdvanceHTChannelBW");
	e.innerHTML = _("advance ht channelbw");
	e = document.getElementById("GeneralChannelSelect");
	e.innerHTML = _("general channel select");
	e = document.getElementById("AdvanceHTExtChannel");
	e.innerHTML = _("advance ht extchannel");

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");

	e = document.getElementById("IntraBSS_0");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_1");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_2");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_3");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_4");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_5");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_6");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_7");
	e.innerHTML = _("advance intra bss");	

	e = document.getElementById("Main_IntraBSS");
	e.innerHTML = _("main advance intra bss");		
	e = document.getElementById("GeneralWirelessSSID1");
	e.innerHTML = _("general wireless ssid1");
	e = document.getElementById("GeneralWirelessSSID2");
	e.innerHTML = _("general wireless ssid2");
	e = document.getElementById("GeneralWirelessSSID3");
	e.innerHTML = _("general wireless ssid3");	
	e = document.getElementById("GeneralWirelessSSID4");
	e.innerHTML = _("general wireless ssid4");	
	e = document.getElementById("GeneralWirelessSSID5");
	e.innerHTML = _("general wireless ssid5");	
	e = document.getElementById("GeneralWirelessSSID6");
	e.innerHTML = _("general wireless ssid6");	
	e = document.getElementById("GeneralWirelessSSID7");
	e.innerHTML = _("general wireless ssid7");	
	
	e = document.getElementById("GeneralWirelessEnable1");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable2");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable3");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable4");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable5");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable6");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable7");
	e.innerHTML = _("wireless enable");
}

function initAll()
{
	var flag = <% getOnloadDFSState(); %>;
	if(0 == flag)
	{
		window.location.href= '/local/advance/radar_waiting.asp';
	}
	initTranslation();
	
	var Enable_SSID  = '<% getCfgZero(1, "EnableSSID_5G" ); %>';
	var Enable_SSID1 = '<% getCfgZero(1, "EnableSSID1_5G"); %>';
	var Enable_SSID2 = '<% getCfgZero(1, "EnableSSID2_5G"); %>';	
	var Enable_SSID3 = '<% getCfgZero(1, "EnableSSID3_5G"); %>';	
	var Enable_SSID4 = '<% getCfgZero(1, "EnableSSID4_5G"); %>';	
	var Enable_SSID5 = '<% getCfgZero(1, "EnableSSID5_5G"); %>';	
	var Enable_SSID6 = '<% getCfgZero(1, "EnableSSID6_5G"); %>';	
	var Enable_SSID7 = '<% getCfgZero(1, "EnableSSID7_5G"); %>';	
	var Intra_BSS  = '<% getCfgZero(1, "NoForwarding_5G"); %>';
	var Intra_BSS1 = '<% getCfgZero(1, "NoForwarding1_5G"); %>';
	var Intra_BSS2 = '<% getCfgZero(1, "NoForwarding2_5G"); %>';	
	var Intra_BSS3 = '<% getCfgZero(1, "NoForwarding3_5G"); %>';
	var Intra_BSS4 = '<% getCfgZero(1, "NoForwarding4_5G"); %>';
	var Intra_BSS5 = '<% getCfgZero(1, "NoForwarding5_5G"); %>';
	var Intra_BSS6 = '<% getCfgZero(1, "NoForwarding6_5G"); %>';
	var Intra_BSS7 = '<% getCfgZero(1, "NoForwarding7_5G"); %>';
	var broadcastssidEnable   = '<% getCfgZero(1, "HideSSID_5G"); %>';
	var broadcastssidEnable1  = '<% getCfgZero(1, "HideSSID1_5G"); %>';
	var broadcastssidEnable2  = '<% getCfgZero(1, "HideSSID2_5G"); %>';
	var broadcastssidEnable3  = '<% getCfgZero(1, "HideSSID3_5G"); %>';
	var broadcastssidEnable4  = '<% getCfgZero(1, "HideSSID4_5G"); %>';
	var broadcastssidEnable5  = '<% getCfgZero(1, "HideSSID5_5G"); %>';
	var broadcastssidEnable6  = '<% getCfgZero(1, "HideSSID6_5G"); %>';
	var broadcastssidEnable7  = '<% getCfgZero(1, "HideSSID7_5G"); %>';

	/* HT20 / HT20_40 */
	if (1*ht_bw == 0)
	{
		document.wireless_basic.n_bandwidth.options.selectedIndex = 0;
		document.wireless_basic.n_extcha.disabled = true;
	}
	else
	{
		document.wireless_basic.n_bandwidth.options.selectedIndex = 1;
		document.wireless_basic.n_extcha.disabled = false;
	}

	/* 11bgn / 11an */
	PhyMode = 1*PhyMode;
	if (PhyMode == 8){
		document.wireless_basic.wirelessmode.options.selectedIndex = 0;
	}
	else if (PhyMode == 14) {
		document.wireless_basic.wirelessmode.options.selectedIndex = 1;
	}

	if (Enable_SSID1 == "1"){
		document.wireless_basic.enablessid1.checked = true;
	}else{
		document.wireless_basic.enablessid1.checked = false;
	}
	if (Enable_SSID2 == "1"){
		document.wireless_basic.enablessid2.checked = true;
	}else{
		document.wireless_basic.enablessid2.checked = false;
	}
	if (Enable_SSID3 == "1"){
		document.wireless_basic.enablessid3.checked = true;
	}else{
		document.wireless_basic.enablessid3.checked = false;
	}
	if (Enable_SSID4 == "1"){
		document.wireless_basic.enablessid4.checked = true;
	}else{
		document.wireless_basic.enablessid4.checked = false;
	}
	if (Enable_SSID5 == "1"){
		document.wireless_basic.enablessid5.checked = true;
	}else{
		document.wireless_basic.enablessid5.checked = false;
	}
	if (Enable_SSID6 == "1"){
		document.wireless_basic.enablessid6.checked = true;
	}else{
		document.wireless_basic.enablessid6.checked = false;
	}
	if (Enable_SSID7 == "1"){
		document.wireless_basic.enablessid7.checked = true;
	}else{
		document.wireless_basic.enablessid7.checked = false;
	}
		
	if (broadcastssidEnable == "1"){
		document.wireless_basic.hidessid.checked = true;
	}else{
		document.wireless_basic.hidessid.checked = false;
	}
	if (broadcastssidEnable1 == "1"){
		document.wireless_basic.hidemssid_1.checked = true;
	}else{
		document.wireless_basic.hidemssid_1.checked = false;
	}
	if (broadcastssidEnable2 == "1"){
		document.wireless_basic.hidemssid_2.checked = true;
	}else{
		document.wireless_basic.hidemssid_2.checked = false;
	}
	if (broadcastssidEnable3 == "1"){
		document.wireless_basic.hidemssid_3.checked = true;
	}else{
		document.wireless_basic.hidemssid_3.checked = false;
	}
	if (broadcastssidEnable4 == "1"){
		document.wireless_basic.hidemssid_4.checked = true;
	}else{
		document.wireless_basic.hidemssid_4.checked = false;
	}
	if (broadcastssidEnable5 == "1"){
		document.wireless_basic.hidemssid_5.checked = true;
	}else{
		document.wireless_basic.hidemssid_5.checked = false;
	}
	if (broadcastssidEnable6 == "1"){
		document.wireless_basic.hidemssid_6.checked = true;
	}else{
		document.wireless_basic.hidemssid_6.checked = false;
	}
	if (broadcastssidEnable7 == "1"){
		document.wireless_basic.hidemssid_7.checked = true;
	}else{
		document.wireless_basic.hidemssid_7.checked = false;
	}
	
	if (Intra_BSS == "0"){
		document.wireless_basic.IntraBSS.checked = true;
	}else{
		document.wireless_basic.IntraBSS.checked = false;
	}
	
	if (Intra_BSS1 == "0"){
		document.wireless_basic.IntraBSS1.checked = true;
	}else{
		document.wireless_basic.IntraBSS1.checked = false;
	}
	if (Intra_BSS2 == "0"){
		document.wireless_basic.IntraBSS2.checked = true;
	}else{
		document.wireless_basic.IntraBSS2.checked = false;
	}
	if (Intra_BSS3 == "0"){
		document.wireless_basic.IntraBSS3.checked = true;
	}else{
		document.wireless_basic.IntraBSS3.checked = false;
	}
	if (Intra_BSS4 == "0"){
		document.wireless_basic.IntraBSS4.checked = true;
	}else{
		document.wireless_basic.IntraBSS4.checked = false;
	}
	if (Intra_BSS5 == "0"){
		document.wireless_basic.IntraBSS5.checked = true;
	}else{
		document.wireless_basic.IntraBSS5.checked = false;
	}
	if (Intra_BSS6 == "0"){
		document.wireless_basic.IntraBSS6.checked = true;
	}else{
		document.wireless_basic.IntraBSS6.checked = false;
	}
	if (Intra_BSS7 == "0"){
		document.wireless_basic.IntraBSS7.checked = true;
	}else{
		document.wireless_basic.IntraBSS7.checked = false;
	}

	if (MainIntra_BSS == "0"){
		document.wireless_basic.MainIntraBSS.checked = true;
	}else{
		document.wireless_basic.MainIntraBSS.checked = false;
	}



		
	wirelessModeChange();
	updateChannel();
	SetChannelIdx(nv_channel);
	refreshExtChannel();
	SetExtIdx(nv_ext);

	document.wireless_basic.generalRadioSwitch.options[eval(Enable_SSID)].selected=true;
	onGeneralRadioSwitch();
	
	parent.adjustMyFrameHeight();
}

function onGeneralRadioSwitch()
{
	var index = document.wireless_basic.generalRadioSwitch.options.selectedIndex;
	if ( 1 == index) {
		document.wireless_basic.IntraBSS.disabled = false;
		document.wireless_basic.IntraBSS1.disabled = false;
		document.wireless_basic.IntraBSS2.disabled = false;
		document.wireless_basic.IntraBSS3.disabled = false;
		document.wireless_basic.IntraBSS4.disabled = false;
		document.wireless_basic.IntraBSS5.disabled = false;
		document.wireless_basic.IntraBSS6.disabled = false;
		document.wireless_basic.IntraBSS7.disabled = false;

		document.wireless_basic.ssid.disabled = false;
		document.wireless_basic.mssid_1.disabled = false;
		document.wireless_basic.mssid_2.disabled = false;
		document.wireless_basic.mssid_3.disabled = false;
		document.wireless_basic.mssid_4.disabled = false;
		document.wireless_basic.mssid_5.disabled = false;
		document.wireless_basic.mssid_6.disabled = false;
		document.wireless_basic.mssid_7.disabled = false;

		document.wireless_basic.hidessid.disabled = false;
		document.wireless_basic.hidemssid_1.disabled = false;
		document.wireless_basic.hidemssid_2.disabled = false;
		document.wireless_basic.hidemssid_3.disabled = false;
		document.wireless_basic.hidemssid_4.disabled = false;
		document.wireless_basic.hidemssid_5.disabled = false;
		document.wireless_basic.hidemssid_6.disabled = false;
		document.wireless_basic.hidemssid_7.disabled = false;

		document.wireless_basic.enablessid1.disabled = false;
		document.wireless_basic.enablessid2.disabled = false;
		document.wireless_basic.enablessid3.disabled = false;
		document.wireless_basic.enablessid4.disabled = false;
		document.wireless_basic.enablessid5.disabled = false;
		document.wireless_basic.enablessid6.disabled = false;
		document.wireless_basic.enablessid7.disabled = false;

		document.wireless_basic.wirelessmode.disabled = false;
		document.wireless_basic.n_bandwidth.disabled = false;
		document.wireless_basic.sz11gChannel.disabled = false;
		document.wireless_basic.n_extcha.disabled = false;
		document.wireless_basic.MainIntraBSS.disabled = false;

		wirelessModeChange();
		Channel_BandWidth_onChange();
		updateChannel();
		SetChannelIdx(nv_channel);
		refreshExtChannel();

		toggleEnableSSID1();
		toggleEnableSSID2();
		toggleEnableSSID3();
		toggleEnableSSID4();
		toggleEnableSSID5();
		toggleEnableSSID6();
		toggleEnableSSID7();
	}
	else {
		document.wireless_basic.IntraBSS.disabled = true;
		document.wireless_basic.IntraBSS1.disabled = true;
		document.wireless_basic.IntraBSS2.disabled = true;
		document.wireless_basic.IntraBSS3.disabled = true;
		document.wireless_basic.IntraBSS4.disabled = true;
		document.wireless_basic.IntraBSS5.disabled = true;
		document.wireless_basic.IntraBSS6.disabled = true;
		document.wireless_basic.IntraBSS7.disabled = true;

		document.wireless_basic.ssid.disabled = true;
		document.wireless_basic.mssid_1.disabled = true;
		document.wireless_basic.mssid_2.disabled = true;
		document.wireless_basic.mssid_3.disabled = true;
		document.wireless_basic.mssid_4.disabled = true;
		document.wireless_basic.mssid_5.disabled = true;
		document.wireless_basic.mssid_6.disabled = true;
		document.wireless_basic.mssid_7.disabled = true;

		document.wireless_basic.hidessid.disabled = true;
		document.wireless_basic.hidemssid_1.disabled = true;
		document.wireless_basic.hidemssid_2.disabled = true;
		document.wireless_basic.hidemssid_3.disabled = true;
		document.wireless_basic.hidemssid_4.disabled = true;
		document.wireless_basic.hidemssid_5.disabled = true;
		document.wireless_basic.hidemssid_6.disabled = true;
		document.wireless_basic.hidemssid_7.disabled = true;

		document.wireless_basic.enablessid1.disabled = true;
		document.wireless_basic.enablessid2.disabled = true;
		document.wireless_basic.enablessid3.disabled = true;
		document.wireless_basic.enablessid4.disabled = true;
		document.wireless_basic.enablessid5.disabled = true;
		document.wireless_basic.enablessid6.disabled = true;
		document.wireless_basic.enablessid7.disabled = true;

		document.wireless_basic.wirelessmode.disabled = true;
		document.wireless_basic.n_bandwidth.disabled = true;
		document.wireless_basic.sz11gChannel.disabled = true;
		document.wireless_basic.n_extcha.disabled = true;
		document.wireless_basic.MainIntraBSS.disabled = true;
	}

}


function toggleEnableSSID1()
{
	if (document.wireless_basic.enablessid1.checked == true ) {
		document.wireless_basic.mssid_1.disabled = false;
		document.wireless_basic.hidemssid_1.disabled = false;
		document.wireless_basic.IntraBSS1.disabled = false;
	}
	else {
		document.wireless_basic.mssid_1.disabled = true;
		document.wireless_basic.hidemssid_1.disabled = true;
		document.wireless_basic.IntraBSS1.disabled = true;
	}
}
function toggleEnableSSID2()
{
	if (document.wireless_basic.enablessid2.checked == true ) {
		document.wireless_basic.mssid_2.disabled = false;
		document.wireless_basic.hidemssid_2.disabled = false;
		document.wireless_basic.IntraBSS2.disabled = false;
	}
	else {
		document.wireless_basic.mssid_2.disabled = true;
		document.wireless_basic.hidemssid_2.disabled = true;
		document.wireless_basic.IntraBSS2.disabled = true;
	}
}
function toggleEnableSSID3()
{
	if (document.wireless_basic.enablessid3.checked == true ) {
		document.wireless_basic.mssid_3.disabled = false;
		document.wireless_basic.hidemssid_3.disabled = false;
		document.wireless_basic.IntraBSS3.disabled = false;
	}
	else {
		document.wireless_basic.mssid_3.disabled = true;
		document.wireless_basic.hidemssid_3.disabled = true;
		document.wireless_basic.IntraBSS3.disabled = true;
	}
}
function toggleEnableSSID4()
{
	if (document.wireless_basic.enablessid4.checked == true ) {
		document.wireless_basic.mssid_4.disabled = false;
		document.wireless_basic.hidemssid_4.disabled = false;
		document.wireless_basic.IntraBSS4.disabled = false;
	}
	else {
		document.wireless_basic.mssid_4.disabled = true;
		document.wireless_basic.hidemssid_4.disabled = true;
		document.wireless_basic.IntraBSS4.disabled = true;
	}
}
function toggleEnableSSID5()
{
	if (document.wireless_basic.enablessid5.checked == true ) {
		document.wireless_basic.mssid_5.disabled = false;
		document.wireless_basic.hidemssid_5.disabled = false;
		document.wireless_basic.IntraBSS5.disabled = false;
	}
	else {
		document.wireless_basic.mssid_5.disabled = true;
		document.wireless_basic.hidemssid_5.disabled = true;
		document.wireless_basic.IntraBSS5.disabled = true;
	}
}
function toggleEnableSSID6()
{
	if (document.wireless_basic.enablessid6.checked == true ) {
		document.wireless_basic.mssid_6.disabled = false;
		document.wireless_basic.hidemssid_6.disabled = false;
		document.wireless_basic.IntraBSS6.disabled = false;
	}
	else {
		document.wireless_basic.mssid_6.disabled = true;
		document.wireless_basic.hidemssid_6.disabled = true;
		document.wireless_basic.IntraBSS6.disabled = true;
	}
}
function toggleEnableSSID7()
{
	if (document.wireless_basic.enablessid7.checked == true ) {
		document.wireless_basic.mssid_7.disabled = false;
		document.wireless_basic.hidemssid_7.disabled = false;
		document.wireless_basic.IntraBSS7.disabled = false;
	}
	else {
		document.wireless_basic.mssid_7.disabled = true;
		document.wireless_basic.hidemssid_7.disabled = true;
		document.wireless_basic.IntraBSS7.disabled = true;
	}
}

</script>
</head>
<body onload="initAll()">
<form method="post" name="wireless_basic" action="/goform/wifiAPGeneral5G">

<input type="hidden" name="bssid_num" value="1">
<input type="hidden" name="bssid_num1" value="">
<input type="hidden" name="bssid_num2" value="">
<input type="hidden" name="bssid_num3" value="">
<input type="hidden" name="bssid_num4" value="">
<input type="hidden" name="bssid_num5" value="">
<input type="hidden" name="bssid_num6" value="">
<input type="hidden" name="bssid_num7" value="">

<div id="table">
<ul>
  <li class="table_content">
  <div class="data">
<ul>
<li class="title" id="GeneralWirelessSetup">Wireless Setup</li>
<li class="w_text2">
<table width=95% border="0" align="center" cellpadding="0" cellspacing="0">
  <tr height=25>
    <td width=50>&nbsp;</td>
	<td width=100 id="GeneralWirelessLAN">Wireless LAN :</td>
	<td width=150 id=GeneralRadioStatus>
	  <select name="generalRadioSwitch" id="generalRadioSwitch" size="1" onChange="onGeneralRadioSwitch();" style="width:140px">
		<option value=0>Off</option>       
		<option value=1>On</option>       
	  </select>
	</td>
	<td width=50>&nbsp;</td>
	<td width=*>&nbsp;</td>
  </tr>

  <tr height=25>
    <td>&nbsp;</td>
    <td>
      <font id="GeneralWirelessSSID">Network Name(SSID) :</font>
    </td>
    <td>
      <input type=text name=ssid size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G"); %>" />
    </td>        
    <td ><input type="checkbox" name=hidessid value="1" />
	  <font id ="GeneralHideSSID1"> Hide</font>
    </td>
    <td>
      <input name="IntraBSS" type="checkbox" value="0" />
      <font id ="IntraBSS_0"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>


  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid1 value="1" onclick="toggleEnableSSID1()"/>
      <font id="GeneralWirelessEnable1">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID1">Name(SSID1) :</font>
    </td>
    <td>
      <input type=text name=mssid_1 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G1"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_1 value="1" />
      <font id ="GeneralmSSIDHide1">Hide</font>
    </td>
    <td>
      <input name="IntraBSS1" type="checkbox" value="0" />
      <font id ="IntraBSS_1"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid2 value="1" onclick="toggleEnableSSID2()"/>
      <font id="GeneralWirelessEnable2">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID2">Name(SSID2) :</font>
    </td>
    <td>
      <input type=text name=mssid_2 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G2"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_2 value="1" />
      <font id ="GeneralmSSIDHide2">Hide</font>
    </td>
    <td>
      <input name="IntraBSS2" type="checkbox" value="0" />
      <font id ="IntraBSS_2"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid3 value="1" onclick="toggleEnableSSID3()"/>
      <font id="GeneralWirelessEnable3">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID3">Name(SSID3) :</font>
    </td>
    <td>
      <input type=text name=mssid_3 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G3"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_3 value="1" />
      <font id ="GeneralmSSIDHide3">Hide</font>
    </td>
    <td>
      <input name="IntraBSS3" type="checkbox" value="0" />
      <font id ="IntraBSS_3"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid4 value="1" onclick="toggleEnableSSID4()"/>
      <font id="GeneralWirelessEnable4">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID4">Name(SSID4) :</font>
    </td>
    <td>
      <input type=text name=mssid_4 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G4"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_4 value="1" />
      <font id ="GeneralmSSIDHide4">Hide</font>
    </td>
    <td>
      <input name="IntraBSS4" type="checkbox" value="0" />
      <font id ="IntraBSS_4"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid5 value="1" onclick="toggleEnableSSID5()"/>
      <font id="GeneralWirelessEnable5">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID5">Name(SSID5) :</font>
    </td>
    <td>
      <input type=text name=mssid_5 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G5"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_5 value="1" />
      <font id ="GeneralmSSIDHide5">Hide</font>
    </td>
    <td>
      <input name="IntraBSS5" type="checkbox" value="0" />
      <font id ="IntraBSS_5"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid6 value="1" onclick="toggleEnableSSID6()"/>
      <font id="GeneralWirelessEnable6">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID6">Name(SSID6) :</font>
    </td>
    <td>
      <input type=text name=mssid_6 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G6"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_6 value="1" />
      <font id ="GeneralmSSIDHide6">Hide</font>
    </td>
    <td>
      <input name="IntraBSS6" type="checkbox" value="0" />
      <font id ="IntraBSS_6"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

  <tr height=25>
    <td>
      <input type="checkbox" name=enablessid7 value="1" onclick="toggleEnableSSID7()"/>
      <font id="GeneralWirelessEnable7">Enable </font>
    </td>
    <td>
      <font id="GeneralWirelessSSID7">Name(SSID7) :</font>
    </td>
    <td>
      <input type=text name=mssid_7 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5G7"); %>" />
    </td>
    <td>
      <input type="checkbox" name=hidemssid_7 value="1" />
      <font id ="GeneralmSSIDHide7">Hide</font>
    </td>
    <td>
      <input name="IntraBSS7" type="checkbox" value="0" />
      <font id ="IntraBSS_7"> Enable Intra-BSS Traffic</font> 
    </td>
  </tr>

</table>
</li></ul>

<ul>
<li></li>

<li class="w_text">
<span id="div_networkmode_id"  class='off'>
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr> 
<td id="AdvanceNetMode" width="180" nowrap>Network Mode :</td>
<td width="150">
<select name="wirelessmode" id="wirelessmode" size="1" onChange="wirelessModeChange();Channel_BandWidth_onChange();updateChannel();SetChannelIdx(nv_channel);refreshExtChannel();"  style="width:240px">
<option value=8>11a/n mixed mode</option>       
<option value=14>11ac mode</option>       
</select>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</span>
</li>

<li class="w_text">
<span id="div_ht_phy_1_3" class="off">  
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr>
<td id="AdvanceHTChannelBW" width="180" nowrap>Channel BandWidth :</td>
<td width="150">
<select name="n_bandwidth" id="n_bandwidth" size="1" onChange="Channel_BandWidth_onChange();updateChannel();SetChannelIdx(nv_channel);refreshExtChannel();" style="width:240px">
</select>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</span>  
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralChannelSelect" width="180" nowrap>Channel Selection :</td>
<td width="150">
	<select id="sz11gChannel" name="sz11gChannel" size="1" onChange="refreshExtChannel()"  style="width:240px">
	</select>                        
</td>
<td width="*">
&nbsp;
</td>
</tr>
</table>
</li>

<li class="w_text">
<span id="div_extension_channel" class="off">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr>
<td id="AdvanceHTExtChannel" width="180" nowrap>Extension Channel :</td>
<td width="150">
	<select id="n_extcha" name="n_extcha" size="1" style="width:240px">
	</select>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</span>
</li> 

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%"><input name="MainIntraBSS" type="checkbox" value=1 />
<font id ="Main_IntraBSS">Communication between wireless clients with different SSIDs</font> 
</td>
<td>&nbsp;</td>
</tr>
</table>
</li></ul>

<ul>
  <li></li>
  <li></li>
</ul>

  </div>
  </li>
</ul>

<ul><center>
  <li class="table_button">
  <input type=button value=Apply id="GeneralApply" onclick="radarRequest()" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <input type=reset value=Reset id="GeneralReset" onClick="window.location.reload()">
  </li></center>
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
