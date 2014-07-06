<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>Ralink Wireless Security Settings</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
</style>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var changed = 0;
var old_MBSSID;
var defaultShownMBSSID = 0;
var TmpIp;
/************************/
var SSID = new Array();
var AuthMode = new Array();

/***********WEP**********/
/*
var WEP_Encmode = new Array();
var WEP_DefaultKeyID = new Array();
var WEP_KeyType = new Array();
var WEP_Key1Str = new Array();
var WEP_Key2Str = new Array();
var WEP_Key3Str = new Array();
var WEP_Key4Str = new Array();
*/

/***********WPA**********/
/*
var WPA_Crypto = new Array();
var WPA_Key = new Array();
var WPA_RekeyTI = new Array();
var WPA_SessionTI = new Array();

var WPA_RADIUS_Server = new Array();
var WPA_RADIUS_Port = new Array();
var WPA_RADIUS_Key = new Array();
*/

/***********WPA2**********/
var WPA2_Crypto = new Array();
var WPA2_Key = new Array();
var WPA2_RekeyTI = new Array();
var WPA2_SessionTI = new Array();

var WPA2_RADIUS_Server = new Array();
var WPA2_RADIUS_Port = new Array();
var WPA2_RADIUS_Key = new Array();

var WPA2_PMKPeriod = new Array();
var WPA2_PreAuth = new Array();
/************************/

var IEEE8021X = new Array();

function  allTrim(ui){  
    var  notValid=/\s/;
	  
    while(notValid.test(ui)){  
        ui=ui.replace(notValid,"");
	} 
	 
    return ui;
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

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function clickAutoChannel()
{
	if (document.security_form.Auto_Channel.checked == true)
		document.security_form.sz11gChannel.disabled = true;
	else
		document.security_form.sz11gChannel.disabled = false;	
}


function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if (    (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function checkRange(str, num, min, max)
{
        var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
             return false;
	}
	d = atoi(str,num);
	if(d > max || d < min)
		return false;
	return true;
}
function check_IP(field)    
{  var ip = field.value;
   var re=/^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;   
   if(re.test(ip))   
   {   
       if( RegExp.$1<224 && RegExp.$2<255 && RegExp.$3<255 && RegExp.$4<255) 
		{
		   if( RegExp.$1==127 || RegExp.$1==0) 
		   	{
		   		alert("IP adress format error.");
				field.value = TmpIp;
				field.focus();				
		   		return false;
		   	}
			return true;   
		}
   }   
   alert("IP adress format error.");
	field.value = TmpIp;
	field.focus();
   return false;    
}
function checkIpAddr(field)
{
	if(field.value == "")
	return false;
	var ip = field.value;
       ip=ip.split(".");
	if (parseInt(parseFloat(ip[0])) == 127 || (parseInt(parseFloat(ip[0])) >= 224 && parseInt(parseFloat(ip[0])) <= 239)){
             alert("ip value="+ip[0]);
		return false;
	}
	if ( checkAllNum(field.value) == 0)
		return false;

	if( (!checkRange(field.value,1,0,255)) ||
		(!checkRange(field.value,2,0,255)) ||
		(!checkRange(field.value,3,0,255)) ||
		(!checkRange(field.value,4,1,254)) ){
			return false;
	}
	return true;
}

function atoi(str, num)
{
	i=1;
	if(num != 1 ){
		while (i != num && str.length != 0){
			if(str.charAt(0) == '.'){
			i++;
		}
		str = str.substring(1);
	}
	
        if(i != num )
		return -1;
	}

	for(i=0; i<str.length; i++){
		if(str.charAt(i) == '.'){
			str = str.substring(0, i);
			break;
		}
	}
	
	if(str.length == 0)
		return -1;

	return parseInt(str, 10);
}

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
function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' 
			|| str.charAt(i) == '\n' ){
			return false;

		}
		else {
	        continue;
	    }
	}
	return true;
}

function checkStrictInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (  str.charAt(i) == ',' 
			||str.charAt(i) == '\r' 
			|| str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
	return true;
}

function checkAllNum(str)
{
	for (var i=0; i<str.length; i++){
		if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return false;
	}
	return true;
}

function checkIsNum(str)
{
	for (var i=0; i<str.length; i++){
		if((str.charAt(i) >= '0' && str.charAt(i) <= '9'))
			continue;
		return false;
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

function securityHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
			UpdateMBSSIDList();
			LoadFields(defaultShownMBSSID);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function deleteAccessPolicyListHandler()
{
	window.location.reload(false);
}


function parseAllData(str)
{
	var all_str = new Array();
	all_str = str.split("\n");

	if (parent.current_control_ssid){
		defaultShownMBSSID = parent.current_control_ssid;
	}
	else {
		defaultShownMBSSID = parseInt(all_str[0]);
	}

	for (var i=0; i<all_str.length-2; i++) {
		var fields_str = new Array();
		fields_str = all_str[i+1].split("\r");

		/*********************/
		SSID[i] = fields_str[0];
		AuthMode[i] = fields_str[1];

		/***********WEP**********/
		/*
		WEP_Encmode[i] = fields_str[2];
		WEP_DefaultKeyID[i] = fields_str[3];
		WEP_KeyType[i] = fields_str[4];
		WEP_Key1Str[i] = fields_str[5];
		WEP_Key2Str[i] = fields_str[6];
		WEP_Key3Str[i] = fields_str[7];
		WEP_Key4Str[i] = fields_str[8];
		*/

		/***********WPA**********/
		/*
		WPA_Crypto[i] = fields_str[9];
		WPA_Key[i] = fields_str[10];
		WPA_RekeyTI[i] = fields_str[11];
		WPA_SessionTI[i] = fields_str[12];

		WPA_RADIUS_Server[i] = fields_str[13];
		WPA_RADIUS_Port[i] = fields_str[14];
		WPA_RADIUS_Key[i] = fields_str[15];
		*/

		/***********WPA2**********/
		WPA2_Crypto[i] = fields_str[2];
		WPA2_Key[i] = fields_str[3];
		WPA2_RekeyTI[i] = fields_str[4];
		WPA2_SessionTI[i] = fields_str[5];

		WPA2_RADIUS_Server[i] = fields_str[6];
		WPA2_RADIUS_Port[i] = fields_str[7];
		WPA2_RADIUS_Key[i] = fields_str[8];

		WPA2_PMKPeriod[i] = fields_str[9];
		WPA2_PreAuth[i] = fields_str[10];
		/*********************/

		IEEE8021X[i] = 0;

		/*********************/
		/* !!!! IMPORTANT !!!!*/
		if(IEEE8021X[i] == "1")
			AuthMode[i] = "IEEE8021X";

		if(AuthMode[i] == "OPEN" 
			&& WEP_Encmode[i] == "NONE" 
			&& IEEE8021X[i] == "0") {
			AuthMode[i] = "Disable";
		}
	}
}

function checkIntervalValue(str,min,max){
      	d = parseInt(str, 10);
	if(d > max || d < min)
		return false;
	return true;
}

function checkData()
{
	var securitymode;
	securitymode = document.security_form.security_mode.value;

	if (securitymode == "OPEN" || securitymode == "SHARED" ||securitymode == "WEPAUTO")
	{
		if(! check_Wep(securitymode) )
			return false;
	}
	else if (securitymode == "WPAPSK" || securitymode == "WPA2PSK" || securitymode == "WPAPSKWPA2PSK" ){
		var keyvalue = document.security_form.passphrase.value;

		if (keyvalue.length == 0){
			alert('Please input passphrase!');
			document.security_form.passphrase.focus();
			return false;
		} else {
			tmp_ap_wpapsk = keyvalue;
			tmp_ap_wpapsk = allTrim(tmp_ap_wpapsk);
			if (! isASCII(tmp_ap_wpapsk)) {
				alert("invalid passphrase: only support printable ASCII except '\,'!");
				document.security_form.passphrase.focus();
				return false;
			}	
		}

		if (keyvalue.length < 8){
			alert('invalid passphrase: Please input at least 8 characters for PSK key!');
			document.security_form.passphrase.focus();
			return false;
		}
/*
		if(checkInjection(document.security_form.passphrase.value) == false){
			alert("invalid passphrase: only support printable ASCII except '\,'!");
			document.security_form.passphrase.focus();
			return false;
		}
*/

		if(check_wpa() == false)
			return false;

		// Hex
		if (checkHex(document.security_form.passphrase.value) == true){ 
			if ((document.security_form.passphrase.value.length < 8) 
				|| (document.security_form.passphrase.value.length > 64)){
				alert('Passphrase is invalid.');
				return false;
			}
		}
		// ASCII
		else{ 
			if ((document.security_form.passphrase.value.length < 8) 
				|| (document.security_form.passphrase.value.length > 63)){
				alert('Passphrase is invalid.');
				return false;
			}
		}
	}
	//802.1x
	else if (securitymode == "IEEE8021X") {
		if( document.security_form.ieee8021x_wep[0].checked == false &&
				document.security_form.ieee8021x_wep[1].checked == false){
			alert('Please choose the 802.1x WEP option.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}
	//     WPA 
	else if (securitymode == "WPA" ){
		if(check_wpa() == false)
			return false;
		if(check_radius() == false)
			return false;
	}
	// WPA2 or WPA1WP2 mixed mode
	else if (securitymode == "WPA2" || securitymode == "WPA1WPA2") 
	{
		if(check_wpa() == false)
			return false;
		if( document.security_form.PreAuthentication[0].checked == false &&
				document.security_form.PreAuthentication[1].checked == false){
			alert('Please choose the Pre-Authentication options.');
			return false;
		}

		if(!document.security_form.PMKCachePeriod.value.length){
			alert('Please input the PMK Cache Period.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}
	var timervalue = document.security_form.keyRenewalInterval.value;

	if (timervalue.length == 0){
		alert('Please input timer value!');
		return false;
	}
	if(checkIsNum(document.security_form.keyRenewalInterval.value) == false){
		alert('Please input a valid key renewal interval');
		return false;
	}
	if(checkIntervalValue(document.security_form.keyRenewalInterval.value,10,3600) == false){
		alert('Input update timer value range is 10~3600!');
		return false;
	}
	if(document.security_form.keyRenewalInterval.value < 60){
		alert('Warning: A short key renewal interval.');
	}
	return true;
}

function check_wpa()
{
	/*
	if(checkInjection(document.security_form.RadiusServerSecret.value)== false){
		alert('System do not support specific , character.');
		document.security_form.RadiusServerSecret.focus();
		return false;
	}	
	*/
	if(checkIsNum(document.security_form.keyRenewalInterval.value) == false){
		alert('Please input a valid key renewal interval');
		return false;
	}
	if(checkIntervalValue(document.security_form.keyRenewalInterval.value,10,3600) == false){
		alert('Input update timer value range is 10~3600!');
		return false;
	}
	return true;
}

function check_radius()
{
	if(!document.security_form.RadiusServerIP.value.length){
		alert('Please input the radius server ip address.');
		document.security_form.RadiusServerIP.focus();
		return false;		
	}
	else {
		tmp_RadiusServerIP = document.security_form.RadiusServerIP.value;
		tmp_RadiusServerIP = allTrim(tmp_RadiusServerIP);
		if (! isASCII(tmp_RadiusServerIP)) {
			alert("invalid Radius Server IP: Only ASCII characters are supported!");
			document.security_form.RadiusServerIP.focus();
			return false;
		}
	}
	document.security_form.RadiusServerPort.value = 
		parseInt(document.security_form.RadiusServerPort.value+"");
	if(!document.security_form.RadiusServerPort.value.length){
		alert('Please input the radius server port number.');
		document.security_form.RadiusServerPort.focus();
		return false;		
	} 
	else {
		tmp_RadiusServerPort = document.security_form.RadiusServerPort.value;
		tmp_RadiusServerPort = allTrim(tmp_RadiusServerPort);
		if (! isASCII(tmp_RadiusServerPort)) {
			alert("invalid Radius Server Port: Only ASCII characters are supported!");
			document.security_form.RadiusServerPort.focus();
			return false;
		}
	}

	if(!document.security_form.RadiusServerSecret.value.length){
		alert('Please input the radius server shared secret.');
		document.security_form.RadiusServerSecret.focus();
		return false;		
	}
	else {
		tmp_RadiusServerSecret = document.security_form.RadiusServerSecret.value;
		tmp_RadiusServerSecret = allTrim(tmp_RadiusServerSecret);
		if (! isASCII(tmp_RadiusServerSecret)) {
			alert("invalid Radius Server Secret: Only ASCII characters are supported!");
			document.security_form.RadiusServerSecret.focus();
			return false;
		}
	}

	if(check_IP(document.security_form.RadiusServerIP) == false){
		return false;		
	}
	if( (checkRange(document.security_form.RadiusServerPort.value, 1, 1, 65535)==false) ||
			(checkAllNum(document.security_form.RadiusServerPort.value)==false)){
		alert('Please input a valid radius server port number.');
		document.security_form.RadiusServerPort.focus();
		return false;		
	}
	if(checkStrictInjection(document.security_form.RadiusServerSecret.value)==false){
		alert("invalid passphrase: only support printable ASCII except '\,'!");
		document.security_form.RadiusServerSecret.focus();
		return false;		
	}
	if( document.security_form.RadiusServerSecret.value.length < 6 ){
		alert("Please input at least 6 characters for RADIUS password!");
		document.security_form.RadiusServerSecret.focus();
		return false;		
	}

	return true;
}

function securityMode(c_f)
{
	var security_mode;

	changed = c_f;

	hideWep();

	show_div(false, "div_wpapsk_compatible");
	show_div(false, "div_wpa_compatible");	
	
	show_div(false, "wpa_passphrase");
	show_div(false, "wpa_key_renewal_interval");
	show_div(false, "wpa_PMK_Cache_Period");
	show_div(false, "wpa_preAuthentication");
	show_div(false, "wpa_Crypto");
	
	document.security_form.passphrase.disabled = true;
	document.security_form.keyRenewalInterval.disabled = true;
	document.security_form.PMKCachePeriod.disabled = true;
	document.security_form.PreAuthentication.disabled = true;

	// 802.1x
	show_div(false, "div_radius_server");
	show_div(false, "div_8021x_wep");
	document.security_form.ieee8021x_wep.disable = true;
	document.security_form.RadiusServerIP.disable = true;
	document.security_form.RadiusServerPort.disable = true;
	document.security_form.RadiusServerSecret.disable = true;	

	security_mode = document.security_form.security_mode.value;
	/*WEP*/
	if (security_mode == "OPEN" || security_mode == "SHARED" ||security_mode == "WEPAUTO"){
		showWep(security_mode);
	}
	/*PSK*/
	else if (security_mode == "WPAPSK" 
		|| security_mode == "WPA2PSK" 
		|| security_mode == "WPAPSKWPA2PSK"){
		
		show_div(true, "wpa_Crypto");
		show_div(true, "wpa_passphrase");
		document.security_form.passphrase.disabled = false;
	}
	/*802.1x*/
	else if (security_mode == "WPA" 
		|| security_mode == "WPA2" 
		|| security_mode == "WPA1WPA2") {
	
		show_div(true, "wpa_Crypto");
		show_div(true, "div_radius_server");
		document.security_form.RadiusServerIP.disable = false;
		document.security_form.RadiusServerPort.disable = false;
		document.security_form.RadiusServerSecret.disable = false;	

		if(security_mode == "WPA2" || security_mode == "WPA1WPA2"){
			show_div(true, "wpa_preAuthentication");
			document.security_form.PreAuthentication.disabled = false;
			show_div(false, "wpa_PMK_Cache_Period");
			document.security_form.PMKCachePeriod.disabled = false;
		}
	}
	// 802.1X-WEP
	else if (security_mode == "IEEE8021X") { 
		show_div(true, "div_8021x_wep");
		show_div(true, "div_radius_server");
		document.security_form.ieee8021x_wep.disable = false;
		document.security_form.RadiusServerIP.disable = false;
		document.security_form.RadiusServerPort.disable = false;
		document.security_form.RadiusServerSecret.disable = false;	
	}
	
	show_div(false, "div_note2_id");
	parent.adjustMyFrameHeight();
}


function hideWep()
{
	show_div(false, "div_wep");
}

function showWep(mode)
{
	show_div(true, "div_wep");
}


function check_Wep(securitymode)
{
	var defaultid, i;
	for (i=0; i<=3; i++){	
		if (document.security_form.DefWEPKey[i].checked == true){
			defaultid = i;
		}
	}	
	defaultid = defaultid+1;

	if ( defaultid == 1 )
		var keyvalue = document.security_form.wep_key_1.value;
	else if (defaultid == 2)
		var keyvalue = document.security_form.wep_key_2.value;
	else if (defaultid == 3)
		var keyvalue = document.security_form.wep_key_3.value;
	else if (defaultid == 4)
		var keyvalue = document.security_form.wep_key_4.value;

	tmp_keyvalue = keyvalue;
	tmp_keyvalue = allTrim(tmp_keyvalue);
	if (! isASCII(tmp_keyvalue)) {
			alert("invalid key: cann't include chinese!");
			if (defaultid == 1)
				document.security_form.wep_key_1.focus();
			else if (defaultid == 2)
				document.security_form.wep_key_1.focus();
			else if (defaultid == 3)
				document.security_form.wep_key_1.focus();
			else if (defaultid == 4)
				document.security_form.wep_key_1.focus();
					
			return false;
	}

	if (keyvalue.length == 0 &&  (securitymode == "SHARED" || securitymode == "OPEN")){ // shared wep  || md5
		alert(_('Please input wep key')+defaultid+'!');
		return false;
	}

	var keylength = document.security_form.wep_key_1.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key1 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_1.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key1 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_1.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.security_form.wep_key_2.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key2 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_2.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key2 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_2.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.security_form.wep_key_3.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key3 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_3.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key3 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_3.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}							
			}		
		
		}
	}	

	var keylength = document.security_form.wep_key_4.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key4 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_4.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key4 !');
					return false;
				}
				/*
				if(checkInjection(document.security_form.wep_key_4.value)== false){
					alert('System do not support specific , character.');
					return false;
				}
				*/
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
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

	submit_ssid_num = 1;

	var securitymode = document.security_form.security_mode.value;
	var tmp_mode;
	var wpsenable = <% getWPSModeASP(); %>;
	if (securitymode == "SHARED" || securitymode == "WPAPSK"){ 
		if(securitymode == "SHARED"){
			tmp_mode="-Static WEP"
		}else if(securitymode == "WPAPSK"){
			tmp_mode="-WPA-PSK"
		}
		if(wpsenable == 1){
			if (!confirm(_(tmp_mode+" security mode will disable WPS function."+"\n"+"-Do you wish to continue with this process?")))
				return false;
		}
	}	
	if (checkData() == true){
		changed = 0;
		showWebMessage(2, "");
		document.security_form.submit();
	}
}

function RadiusServerStatus()
{
	var security_mode = document.security_form.security_mode.value;
	var MBSSID = document.security_form.ssidIndex.options.selectedIndex;

	if(security_mode == "WPA" || security_mode == "WPA2"){
               document.security_form.RadiusServerIP.disabled = false;
		  document.security_form.RadiusServerPort.disabled = false;
		  document.security_form.RadiusServerSecret.disabled = false;	
	}
}

/*disabled|wep|psk|psk2|psk2mixed|wpa|wpa2|wpa2mixed*/

/*
Disable
WEPAUTO|SHARED|OPEN
WPA
WPA1WPA2|WPA2
WPAPSK
WPAPSKWPA2PSK|WPA2PSK
*/

function LoadFields(MBSSID)
{
	var result;
	sp_select = document.getElementById("security_mode");
	sp_select.options.length = 0;
	var PhyMode  = '<% getCfgZero(1, "WirelessMode"); %>';

	sp_select.options[sp_select.length] = new Option(_("general nosecurity"),"Disable",	false, AuthMode[MBSSID] == "Disable");       
	/*
	sp_select.options[sp_select.length] = new Option(_("general static wep"),"SHARED", 	false, AuthMode[MBSSID] == "SHARED");
	sp_select.options[sp_select.length] = new Option(_("general wpapsk"), "WPAPSK",	false, AuthMode[MBSSID] == "WPAPSK");
	sp_select.options[sp_select.length] = new Option(_("general wpa"),	"WPA",		false, AuthMode[MBSSID] == "WPA");        
	*/
	sp_select.options[sp_select.length] = new Option(_("general wpa2psk"),"WPA2PSK",	false, AuthMode[MBSSID] == "WPA2PSK");
	sp_select.options[sp_select.length] = new Option(_("general wpa2psk-mixed"),"WPAPSKWPA2PSK",	false, AuthMode[MBSSID] == "WPAPSKWPA2PSK");
	sp_select.options[sp_select.length] = new Option(_("general wpa2"),	"WPA2",		false, AuthMode[MBSSID] == "WPA2");
	sp_select.options[sp_select.length] = new Option(_("general wpa2-mixed"),	"WPA1WPA2",		false, AuthMode[MBSSID] == "WPA1WPA2");

	/***********************WEP************************/
	/*
	document.getElementById("WEP1").value = WEP_Key1Str[MBSSID];
	document.getElementById("WEP2").value = WEP_Key2Str[MBSSID];
	document.getElementById("WEP3").value = WEP_Key3Str[MBSSID];
	document.getElementById("WEP4").value = WEP_Key4Str[MBSSID];

	// Now, 4 Keys type are same, so only get the first key to display HEX or ASCII. It's OK
	if ( (WEP_Key1Str[MBSSID] == "") 
		&& (WEP_Key2Str[MBSSID] == "") 
		&& (WEP_Key3Str[MBSSID] == "") 
		&& (WEP_Key4Str[MBSSID] == "") ){
		//Set ASCII default
		document.security_form.WEPKey_Code[0].checked = true; //ASCII	
	}
	else if (WEP_KeyType[MBSSID] == "0"){
		document.security_form.WEPKey_Code[1].checked = true; //hex

		if (WEP_Key1Str[MBSSID] != ""){
			if (WEP_Key1Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}
		else if (WEP_Key2Str[MBSSID] != ""){
			if (WEP_Key2Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}
		else if (WEP_Key3Str[MBSSID] != ""){
			if (WEP_Key3Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}
			else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}
		else if (WEP_Key4Str[MBSSID] != ""){
			if (WEP_Key4Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}
			else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}		
		}
	}
	else{
		document.security_form.WEPKey_Code[0].checked = true; //ASCII

		if (WEP_Key1Str[MBSSID] != ""){
			if (WEP_Key1Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (WEP_Key2Str[MBSSID] != ""){
			if (WEP_Key2Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (WEP_Key3Str[MBSSID] != ""){
			if (WEP_Key3Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (WEP_Key4Str[MBSSID] != ""){
			if (WEP_Key4Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}		
		}
	}

	document.security_form.DefWEPKey[0].checked= false;
	document.security_form.DefWEPKey[1].checked= false;
	document.security_form.DefWEPKey[2].checked= false;
	document.security_form.DefWEPKey[3].checked= false;

	var defkey_index = parseInt(WEP_DefaultKeyID[MBSSID]) - 1;

	if ( (defkey_index >= 0) && (defkey_index <= 3) ){
		document.security_form.DefWEPKey[defkey_index].checked= true;
	}
	else {
		document.security_form.DefWEPKey[0].checked= true;	
	}     

	// Static WEP 	

	// AUTO
	if(AuthMode[MBSSID] == "WEPAUTO"){
		document.getElementById("security_mode").selectedIndex = 1;
		document.getElementById("auth_method").selectedIndex = 0; 
	}
	else if (AuthMode[MBSSID] == "OPEN"){
		document.getElementById("security_mode").selectedIndex = 1;
		document.getElementById("auth_method").selectedIndex = 1; // Open
	}
	else if (AuthMode[MBSSID] == "SHARED"){
		document.getElementById("security_mode").selectedIndex = 1;
		document.getElementById("auth_method").selectedIndex = 2; // Shared Key
	}
	*/

	/***********************WPA************************/
/*
0	Disable
1	WEP(WEPAUTO|OPEN|SHARED)
2	WPAPSK
3	WPA	
4	WPAPSKWPA2PSK|WPA2PSK
5	WPA1WPA2|WPA2
*/
	/*
	//2	WPAPSK
	if(AuthMode[MBSSID] == "WPAPSK") {
		document.getElementById("security_mode").selectedIndex = 2;
	}

	//3	WPA	
	if(AuthMode[MBSSID] == "WPA") {
		document.getElementById("security_mode").selectedIndex = 3;
	}
	*/

	//1	WPA2PSK
	if( AuthMode[MBSSID]=="WPA2PSK" ) {
		document.getElementById("security_mode").selectedIndex = 1;
		if(AuthMode[MBSSID] == "WPAPSKWPA2PSK") 
			document.security_form.wpapsk_compatible.checked = true;	   	
	}

	//2	WPA2PSK
	if( AuthMode[MBSSID] == "WPAPSKWPA2PSK") {
		document.getElementById("security_mode").selectedIndex = 2;
		if(AuthMode[MBSSID] == "WPAPSKWPA2PSK") 
			document.security_form.wpapsk_compatible.checked = true;	   	
	}

	//3	WPA2
	if( AuthMode[MBSSID] == "WPA2" ){
		document.getElementById("security_mode").selectedIndex = 3;		
		if(AuthMode[MBSSID] == "WPA1WPA2") 
		document.security_form.wpa_compatible.checked = true;	
	}

	//4	WPA1WPA2
	if( AuthMode[MBSSID] == "WPA1WPA2" ){
		document.getElementById("security_mode").selectedIndex = 4;		
		if(AuthMode[MBSSID] == "WPA1WPA2") {
			document.security_form.wpa_compatible.checked = true;	
		}
	}
	
	if (WPA2_RekeyTI[MBSSID] != '') {
		document.getElementById("keyRenewalInterval").value = WPA2_RekeyTI[MBSSID];
	}
	document.getElementById("PMKCachePeriod").value = WPA2_PMKPeriod[MBSSID];
	
	//PreAuthentication
	if(WPA2_PreAuth[MBSSID] == "0")
		document.security_form.PreAuthentication[0].checked = true;
	else
		document.security_form.PreAuthentication[1].checked = true;

	//Crypto
	if(WPA2_Crypto[MBSSID] == "tkip")
		document.security_form.Crypto[0].checked = true;
	else if(WPA2_Crypto[MBSSID] == "aes")
		document.security_form.Crypto[1].checked = true;
	else if(WPA2_Crypto[MBSSID] == "mixed")
		document.security_form.Crypto[2].checked = true;



	//802.1x wep
	if(IEEE8021X[MBSSID] == "1"){
		if(WEP_Encmode[MBSSID] == "WEP")
			document.security_form.ieee8021x_wep[1].checked = true;
		else
			document.security_form.ieee8021x_wep[0].checked = true;
	}

	/*WPA;WPA1WPA2|WPA2*/

	document.getElementById("passphrase").value =WPA2_Key[MBSSID];
	document.getElementById("RadiusServerIP").value = WPA2_RADIUS_Server[MBSSID];
	TmpIp = document.getElementById("RadiusServerIP").value;
	document.getElementById("RadiusServerPort").value = WPA2_RADIUS_Port[MBSSID];
	document.getElementById("RadiusServerSecret").value = WPA2_RADIUS_Key[MBSSID];			

	securityMode(0);
}


function selectMBSSIDChanged()
{

	// check if any security settings changed
	if(changed){
		ret = confirm("Are you sure to ignore changed?");
		if(!ret){
			document.security_form.ssidIndex.options.selectedIndex = old_MBSSID;
			return false;
		}
		else
			changed = 0;
	}

	var selected = document.security_form.ssidIndex.options.selectedIndex;

	parent.current_control_ssid=selected;

	// backup for user cancel action
	old_MBSSID = selected;

	MBSSIDChange(selected);
}

/*
 * When user select the different SSID, this function would be called.
 */ 
function MBSSIDChange(selected)
{

	// load wep/wpa/802.1x table for MBSSID[selected]
	LoadFields(selected);

	// radio button special case
	//WPAAlgorithms = WEP_Encmode[selected];
	IEEE8021XWEP = IEEE8021X[selected];
	PreAuthentication = WPA2_PreAuth[selected];

	return true;
}



function initTranslation()
{
	var e = document.getElementById("GeneralSSIDChoice");
	e.innerHTML = _("station ssid");

	e = document.getElementById("GeneralSecurityTitle");
	e.innerHTML = _("general security title");
	
	e = document.getElementById("GeneralSecureMode");
	e.innerHTML = _("general security mode");

	e = document.getElementById("GeneralWEPPassPhrase");
	e.innerHTML = _("general wep passphrase");

	e = document.getElementById("GeneralWEPEncryp");
	e.innerHTML = _("general wep encryp");

	e = document.getElementById("GeneralAuthMethod");
	e.innerHTML = _("general auth method");

	e = document.getElementById("GeneralWEPGenerate");
	e.value = _("general wep generate");

	e = document.getElementById("GeneralWEPEncryp64");
	e.innerHTML = _("general wep encryp64");
	
	e = document.getElementById("GeneralWEPEncryp128");
	e.innerHTML = _("general wep encryp128");

	e = document.getElementById("GeneralEncrypSharedKey");
	e.innerHTML = _("general encryp sharedkey");

	e = document.getElementById("GeneralEncrypAuto");
	e.innerHTML = _("general encryp auto");

	e = document.getElementById("GeneralNote1_1");
	e.innerHTML = _("general note1_1");
	
	e = document.getElementById("GeneralNote1_2");
	e.innerHTML = _("general note1_2");
	
	e = document.getElementById("GeneralNote1_3");
	e.innerHTML = _("general note1_3");
	
	e = document.getElementById("GeneralNote1_4");
	e.innerHTML = _("general note1_4");

	e = document.getElementById("GeneralWEPASCII");
	e.innerHTML = _("general wep ascii");
	
	e = document.getElementById("GeneralWEPHex");
	e.innerHTML = _("general wep hex");

	e = document.getElementById("GeneralWEPKEY1");
	e.innerHTML = _("general wep key1");

	e = document.getElementById("GeneralWEPKEY2");
	e.innerHTML = _("general wep key2");
	
	e = document.getElementById("GeneralWEPKEY3");
	e.innerHTML = _("general wep key3");
		
	e = document.getElementById("GeneralWEPKEY4");
	e.innerHTML = _("general wep key4");

	e = document.getElementById("GeneralWPAPSKCompatible");
	e.innerHTML = _("general wpapsk compatible");

	e = document.getElementById("GeneralWPACompatible");
	e.innerHTML = _("general wpa compatible");

	e = document.getElementById("GeneralWPACrypto");
	e.innerHTML = _("general crypto");

	e = document.getElementById("GeneralWPAPSKPreSharedKey");
	e.innerHTML = _("general wpapsk presharedkey");	

	e = document.getElementById("GeneralWPAKeyRenewInterval");
	e.innerHTML = _("general wpa keyrenewinterval");
	
	e = document.getElementById("GeneralWPAPMKCachePeriod");
	e.innerHTML = _("general wpa pmkcacheperiod");
	
	e = document.getElementById("GeneralWPAkeyRenewalIntervalUnit");
	e.innerHTML = _("general wpa keyrenewalintervalunit");

	e = document.getElementById("GeneralWPAPMKCachePeriodUnit");
	e.innerHTML = _("general wpa pmkcacheperiodunit");

	e = document.getElementById("GeneralWPAPreAuth");
	e.innerHTML = _("general wpa preauth");

	e = document.getElementById("GeneralWPAPreAuthDisable");
	e.innerHTML = _("general wpa preauthdisable");

	e = document.getElementById("GeneralWPAPreAuthEnable");
	e.innerHTML = _("general wpa preauthenable");

	e = document.getElementById("GeneralIEEE8021xSettings");
	e.innerHTML = _("general ieee8021x settings");

	e = document.getElementById("General1XWEP");
	e.innerHTML = _("general ieee8021x wep");

	e = document.getElementById("General1XWEPDisable");
	e.innerHTML = _("general ieee8021x wepdisable");

	e = document.getElementById("General1XWEPEnable");
	e.innerHTML = _("general ieee8021x wepenable");

	e = document.getElementById("GeneralAuthServer");
	e.innerHTML = _("general auth server");

	e = document.getElementById("GeneralRadiusIPAddr");
	e.innerHTML = _("general radius ipaddr");

	e = document.getElementById("GeneralRadiusPort");
	e.innerHTML = _("general radius port");

	e = document.getElementById("GeneralRadiusSharedSecret");
	e.innerHTML = _("general radius sharedsecret");

	e = document.getElementById("GeneralNote2");
	e.innerHTML = _("general note_2");	

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initAll()
{	
	initTranslation();
	show_div(true, "div_ssidchoice");	
	show_div(false, "div_note2_id");
	
	makeRequest("/goform/wirelessGetSecurity", "n/a", securityHandler);
	parent.adjustMyFrameHeight();
}

function UpdateMBSSIDList()
{
	document.security_form.ssidIndex.length = 0;
	
	for(var i=0; i<SSID.length; i++){
		var j = document.security_form.ssidIndex.options.length;
		if(SSID[i] != ""){
			document.security_form.ssidIndex.options[j] = new Option(SSID[i], i, false, false);
			// Added By Andy Yu in 20140303: Consecutive Space Question for SSID Display
			if(SSID[i].indexOf(" ") != -1){
				var k = 0;
				var tmp_ssid = new Array();
				for(k = 0; k < SSID[i].length; k++ )
				{
					if(SSID[i].charCodeAt(k) == 32)
						tmp_ssid[k] = "&nbsp;";
					else
						tmp_ssid[k] = SSID[i].charAt(k);
				}
							
				document.security_form.ssidIndex.options[j].innerHTML = tmp_ssid.join("");
			}
		}
	}
	
	document.security_form.ssidIndex.options.selectedIndex = defaultShownMBSSID;
	old_MBSSID = defaultShownMBSSID;
}

function setChange(c){
	changed = c;
}

var WPAAlgorithms;
function onWPAAlgorithmsClick(type)
{
	if(type == 0 && WPAAlgorithms == "TKIP") return;
	if(type == 1 && WPAAlgorithms == "AES") return;
	if(type == 2 && WPAAlgorithms == "TKIPAES") return;
	setChange(1);
}

var IEEE8021XWEP;
function onIEEE8021XWEPClick(type)
{
	if(type == 0 && IEEE8021XWEP == false) return;
	if(type == 1 && IEEE8021XWEP == true) return;
	setChange(1);
}

var PreAuthentication;
function onPreAuthenticationClick(type)
{
	if(type == 0 && PreAuthentication == false) return;
	if(type == 1 && PreAuthentication == true) return;
	setChange(1);
}

function parse40WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.security_form.wep_key_1.value = fields_str[0];
	document.security_form.wep_key_2.value = fields_str[1];
	document.security_form.wep_key_3.value = fields_str[2];
	document.security_form.wep_key_4.value = fields_str[3];
}

function parse128WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.security_form.wep_key_1.value = fields_str[0];
	document.security_form.wep_key_2.value = fields_str[0];
	document.security_form.wep_key_3.value = fields_str[0];
	document.security_form.wep_key_4.value = fields_str[0];
}

function get40wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse40WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function get128wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse128WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
	    }
	} 
}

function generate_wep()
{
	var passphrase;
	passphrase = document.security_form.wep_passphrase.value;
	tmp_passphrase = passphrase;
	tmp_passphrase = allTrim(tmp_passphrase);
	if (! isASCII(tmp_passphrase)) {
			alert("invalid passphrase: cann't include chinese!");
			document.security_form.wep_passphrase.focus();
			return false;
	}

	document.security_form.WEPKey_Code[1].checked = true; //Hex	
	if (document.getElementById("wep_encry").selectedIndex == 1){ // get 128 bits WEP KEY
		makeRequest("/goform/wifiget128wepkey", passphrase, get128wepeyHandler);
	}else{ // get 40 bits WEP KEY
		makeRequest("/goform/wifiget40wepkey", passphrase, get40wepeyHandler);
	}
}

</script>
</head>
<body onload="initAll()">
<form method="post" name="security_form" action="/goform/wifiAPSecurity">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>



<li class="title" id="GeneralSecurityTitle">Security</li>
<span id="div_ssidchoice" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSSIDChoice">SSID</td>
<td><select name="ssidIndex" size="1" onchange="selectMBSSIDChanged()">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>
</span>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSecureMode">Security Mode</td>
<td>
<select name="security_mode" id="security_mode" size="1" onchange="securityMode(1)">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>

<!-- WEP -->
<span id="div_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPPassPhrase">PassPhrase </td>
<td><input name="wep_passphrase" id="wep_passphrase" maxlength="26" value="" onKeyUp="setChange(1)">
<input type=button value="Generate" id="GeneralWEPGenerate" onclick="generate_wep()" ></td>		      
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPEncryp">WEP Encryption</td>
<td>
<select name="wep_encry" id="wep_encry" size="1" onchange="securityMode(1)">
<option id="GeneralWEPEncryp64" value="0">64-bits </option>
<option id="GeneralWEPEncryp128" value="1">128-bits</option>
</select>
</td>
</tr>
</table>
</li>                

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthMethod">Authentication Method</td>
<td>
<select name="auth_method" id="auth_method" size="1" onchange="securityMode(1)">
<option id="GeneralEncrypAuto"      value="0">Auto      </option>
<option id="GeneralEncrypOpen"      value="1">Open      </option>
<option id="GeneralEncrypSharedKey" value="2">Shared Key</option>
</select>
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="GeneralNote1_1">Note:</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_2">64-bit WEP: Enter 5 ASCII characters or 10 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_3">128-bit WEP: Enter 13 ASCII characters or 26 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_4">(Select one WEP key as an active key to encrypt wireless data transmission.)</span></td>
</tr>
</table>
</li>                


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%"></td>
<td>
<input type="radio" name="WEPKey_Code" value="1" /><font id="GeneralWEPASCII">ASCII </font>
<input type="radio" name="WEPKey_Code" value="0" /><font id="GeneralWEPHex">Hex</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey1"><input type="radio" name="DefWEPKey" value="1"/><font id="GeneralWEPKEY1"> Key 1</font></td>
<td><input name="wep_key_1" id="WEP1" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey2"><input type="radio" name="DefWEPKey" value="2" /><font id="GeneralWEPKEY2"> Key 2</font></td>
<td><input name="wep_key_2" id="WEP2" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey3"><input type="radio" name="DefWEPKey" value="3" /><font id="GeneralWEPKEY3"> Key 3</font></td>
<td><input name="wep_key_3" id="WEP3" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey4"><input type="radio" name="DefWEPKey" value="4" /><font id="GeneralWEPKEY4"> Key 4</font></td>
<td><input name="wep_key_4" id="WEP4" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
</span>

<input id==wep_key128 name=wep_key128 size=26 type=hidden>
              

<span id="div_wpapsk_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpapsk_compatible" type="checkbox" value=1 />
<font id="GeneralWPAPSKCompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                

<span id="div_wpa_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpa_compatible" type="checkbox" value=1 />
<font id="GeneralWPACompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                
                
<span id="wpa_Crypto" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPACrypto">Crypto</td>
<td>
<input name="Crypto" id="Crypto" value="0" type="radio" onClick="onWPAAlgorithmsClick(0)"><font id="GeneralWPACryptoTKIP">TKIP</font>
<input name="Crypto" id="Crypto" value="1" type="radio" onClick="onWPAAlgorithmsClick(1)"><font id="GeneralWPACryptoAES">AES</font>
<input name="Crypto" id="Crypto" value="2" type="radio" onClick="onWPAAlgorithmsClick(2)"><font id="GeneralWPACryptoMIXED">TKIP/AES</font>
</td>
</tr>
</table>
</li>
</span>                              

<span id="wpa_passphrase" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPSKPreSharedKey">Pre-Shared Key</td>
<td>
<input  name="passphrase" id="passphrase" size="28" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
</span>
 
<span id="wpa_key_renewal_interval" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAKeyRenewInterval">Group Key Update Timer</td>
<td>
<input name="keyRenewalInterval" id="keyRenewalInterval" size="4" maxlength="4" value="3600" onKeyUp="setChange(1)"><font id="GeneralWPAkeyRenewalIntervalUnit"> seconds</font> 
</td>
</tr>
</table>
</li>
</span>


<span id="wpa_PMK_Cache_Period" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPMKCachePeriod">PMK Cache Period</td>
<td>
<input name="PMKCachePeriod" id="PMKCachePeriod" size="4" maxlength="4" value="" onKeyUp="setChange(1)"><font id="GeneralWPAPMKCachePeriodUnit"> minute</font> 
</td>
</tr>
</table>
</li>
</span>


<span id="wpa_preAuthentication" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPreAuth">Pre-Authentication</td>
<td>
<input name="PreAuthentication" id="PreAuthentication" value="0" type="radio" onClick="onPreAuthenticationClick(0)"><font id="GeneralWPAPreAuthDisable">Disable </font>
<input name="PreAuthentication" id="PreAuthentication" value="1" type="radio" onClick="onPreAuthenticationClick(1)"><font id="GeneralWPAPreAuthEnable">Enable </font>
</td>
</tr>
</table>
</li>
</span>                              


<!-- IEEE 802.1x WEP  -->
<span id="div_8021x_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralIEEE8021xSettings">IEEE 802.1x Settings</td>
</tr>
</table>
</li>                
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="General1XWEP">WEP</td>
<td>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="0" type="radio" onClick="onIEEE8021XWEPClick(0)"><font id="General1XWEPDisable">Disable</font>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="1" type="radio" onClick="onIEEE8021XWEPClick(1)"><font id="General1XWEPEnable">Enable</font>
</td>
</tr>
</table>
</li>
</span>                

<!-- Radius Server  -->              
<span id="div_radius_server" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthServer">Authentication Server</td>
</tr>
</table>
</li>
                                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusIPAddr">IP Address </td>
<td>
<input name="RadiusServerIP" id="RadiusServerIP" size="16" maxlength="32" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusPort">Port Number</td>
<td>
<input name="RadiusServerPort" id="RadiusServerPort" size="5" maxlength="5" value="" onKeyUp="setChange(1);value=value.replace(/[^\d]/g,'');">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusSharedSecret">Shared Secret </td>
<td>
<input name="RadiusServerSecret" id="RadiusServerSecret" size="16" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
</span>

<span id="div_note2_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span id="GeneralNote2" class="i_note">Note: WPA and WPA2 can be configured when WPS disabled</span></td>
</tr>
</table>
</li>                
</span>

<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=button value=Apply id="GeneralApply" onclick="submit_apply()" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value=Reset id="GeneralReset" onClick="window.location.reload()">
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

