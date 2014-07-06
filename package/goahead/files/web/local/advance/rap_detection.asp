<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<meta http-equiv="Pragma" content="no-cache">
<style>
td {white-space: nowrap;}
</style>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
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
		formCheck();
	}
}

function DFSRequest()
{
	makeRequest("/goform/getDFSState", "n/a", DFSHandler);
}
function initTranslation(){
	var e = document.getElementById("SecurityFilter");
	e.innerHTML = _("rap security filter");
	e = document.getElementById("LegalAPFilter");
	e.innerHTML = _("rap legal ap filter");	
	e = document.getElementById("LegalAP");
	e.innerHTML = _("rap legal ap");		
	e = document.getElementById("MACAddress");
	e.innerHTML = _("rap mac address");
	e = document.getElementById("OUIAddress");
	e.innerHTML = _("rap oui address");	
	e = document.getElementById("LegalAPList");
	e.innerHTML = _("rap legal ap list");
	e = document.getElementById("BSSIDAdd");
	e.value = _("rap add");
	e = document.getElementById("OUIAdd");
	e.value = _("rap add");
	e = document.getElementById("LegalAPDelete");
	e.value = _("rap delete");
	e = document.getElementById("LegalAPClear");
	e.value = _("rap clear all");
	e = document.getElementById("rapDetection");
	e.value = _("rap detection");
	e = document.getElementById("rapReset");
	e.value = _("rap reset");
	
	e = document.getElementById("NOSecurity");
	e.innerHTML = _("rap no security");
}    
function updateState(){
	initTranslation();
	if (document.RAPDetection.rapSecurityNO.value == "1")
		document.RAPDetection.rapSecurityNO.checked = true;		
	if (document.RAPDetection.legalBSSID_Enable.value == "1")
		document.RAPDetection.legalBSSID_Enable.checked = true;	
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

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddr(field, ismask)
{
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}
	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkInput(str)
{
	for(var i=0; i<str.length; i++) {
		if(str.charAt(i) == '\r' || str.charAt(i) == '\n' || 
		(str.charAt(i) == "'" )  || 
		(str.charAt(i) == '"' )  || (str.charAt(i) == ' ' ))
		return 1;
	}
	return 0;
}

function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;
	if(str=="00:00:00:00:00:00")   
        return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function checkOUI(str){
	var len = str.length;
	if(len!=8)
		return false;
	
	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function MACFocus(index){
	
	switch(index){
		case 1:
		 		if(document.RAPDetection.macAddress_Domain1.value.length==2)
					document.RAPDetection.macAddress_Domain2.select();	
				break;
		case 2:
		 		if(document.RAPDetection.macAddress_Domain2.value.length==2)
					document.RAPDetection.macAddress_Domain3.select();	
		 		break;
		case 3:
		 		if(document.RAPDetection.macAddress_Domain3.value.length==2)
					document.RAPDetection.macAddress_Domain4.select();	
		 		break;
		case 4:
		 		if(document.RAPDetection.macAddress_Domain4.value.length==2)
					document.RAPDetection.macAddress_Domain5.select();	
		 		break;
		case 5:
		 		if(document.RAPDetection.macAddress_Domain5.value.length==2)
					document.RAPDetection.macAddress_Domain6.select();	
		 		break;
		default:
		 		break;
	}
}

function OUIFocus(index){
	
	switch(index){
		case 1:
		 		if(document.RAPDetection.ouiAddress_Domain1.value.length==2)
					document.RAPDetection.ouiAddress_Domain2.select();	
				break;
		case 2:
		 		if(document.RAPDetection.ouiAddress_Domain2.value.length==2)
					document.RAPDetection.ouiAddress_Domain3.select();	
		 		break;
		default:
		 		break;
	}
}

function checkSubnet(ip, mask, client){
  if(client == "0.0.0.0")
    return true;
    
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  client_d = atoi(client, 1);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  client_d = atoi(client, 2);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  client_d = atoi(client, 3);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  client_d = atoi(client, 4);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  return true;
}

function formCheck(){   
	document.RAPDetection.rapSecurityNO.value = document.RAPDetection.rapSecurityNO.checked ? "1": "0";
	document.RAPDetection.legalBSSID_Enable.value = document.RAPDetection.legalBSSID_Enable.checked ? "1": "0";	
	
	var j=0;
	var bssid_set = new Array;
	var k=0;
	var oui_set = new Array;
	for(var i=0; i < document.RAPDetection.bssidMAC_DomainList.options.length; i++){
		if(17 == document.RAPDetection.bssidMAC_DomainList.options[i].text.length){
			bssid_set[j++] = document.RAPDetection.bssidMAC_DomainList.options[i].text;
		}
		else if(8 == document.RAPDetection.bssidMAC_DomainList.options[i].text.length){
			oui_set[k++] = document.RAPDetection.bssidMAC_DomainList.options[i].text;
		}
	}

	document.RAPDetection.bssid_str.value=bssid_set.join("|");
	document.RAPDetection.oui_str.value=oui_set.join("|");
	showWebMessage(2, "");
	document.RAPDetection.submit();
	return true;
}
function checkValue(str){
   var flag=0;
	for(i=0;i < document.RAPDetection.bssidMAC_DomainList.options.length;i++){
		if(document.RAPDetection.bssidMAC_DomainList.options[i].text.toUpperCase()== str.toUpperCase()){
		   flag=1;
		   break;	   
		}		  
	}
	if(flag == 1)
	   	return 1;
	else
    	return 0;
}
/*check number of the tables list-> 16*/
function checkTableNum(listNum)
{
	
	var num = parseInt(listNum,10);	
	var max = 16;
	max = parseInt(max,10);
	if(num >= max){
	   return false;
	}
	else
	   return true; 
}
function legalBSSID_AddKeyword_click(select){
	var D_str;
	if(select == 0) {
		var mac_set = new Array;
		mac_set[0] = document.RAPDetection.macAddress_Domain1.value.toUpperCase();
		mac_set[1] = document.RAPDetection.macAddress_Domain2.value.toUpperCase();
		mac_set[2] = document.RAPDetection.macAddress_Domain3.value.toUpperCase();
		mac_set[3] = document.RAPDetection.macAddress_Domain4.value.toUpperCase();
		mac_set[4] = document.RAPDetection.macAddress_Domain5.value.toUpperCase();
		mac_set[5] = document.RAPDetection.macAddress_Domain6.value.toUpperCase();
		D_str=mac_set.join(":");

		if(!checkMac(D_str)){
			alert('The input number scope: 0~9, a~f, A~F.');
		}
		else {
	   		if(D_str!=""){	
				var list_num=document.RAPDetection.bssidMAC_DomainList.length;
				if(checkTableNum(list_num)){ 
		    		if(checkValue(D_str)==0){
						document.RAPDetection.bssidMAC_DomainList.options[list_num]=new Option(D_str,D_str);
						
						document.RAPDetection.macAddress_Domain1.value="";
						document.RAPDetection.macAddress_Domain2.value="";
						document.RAPDetection.macAddress_Domain3.value="";
						document.RAPDetection.macAddress_Domain4.value="";
						document.RAPDetection.macAddress_Domain5.value="";
						document.RAPDetection.macAddress_Domain6.value="";
					}
					else
					{	    
						alert('Address is exist.');
					}
				}
				else{
		   			alert('List Full.');
				}
	    	}
		} 
	}
	else {
		var oui_set = new Array;
		oui_set[0] = document.RAPDetection.ouiAddress_Domain1.value.toUpperCase();
		oui_set[1] = document.RAPDetection.ouiAddress_Domain2.value.toUpperCase();
		oui_set[2] = document.RAPDetection.ouiAddress_Domain3.value.toUpperCase();
		D_str=oui_set.join(":");
		
		if(!checkOUI(D_str)){
			alert('The input number scope: 0~9, a~f, A~F.');
		}
		else {
	   		if(D_str!=""){	
				var list_num=document.RAPDetection.bssidMAC_DomainList.length;
				if(checkTableNum(list_num)){ 
		    		if(checkValue(D_str)==0){
						document.RAPDetection.bssidMAC_DomainList.options[list_num]=new Option(D_str,D_str);
						
						document.RAPDetection.ouiAddress_Domain1.value="";
						document.RAPDetection.ouiAddress_Domain2.value="";
						document.RAPDetection.ouiAddress_Domain3.value="";
					}
					else
					{	    
						alert('Address is exist.');
					}
				}
				else{
		   			alert('List Full.');
				}
	    	}
		} 
	}
	
}
function legalBSSID_ClearList_click(){
	document.RAPDetection.bssidMAC_DomainList.options.length=0;		    
}
function legalBSSID_DeleteKeyword_click(){
    
	//for IE & firefox
	if(document.RAPDetection.bssidMAC_DomainList.selectedIndex >= 0){
		document.RAPDetection.bssidMAC_DomainList.remove(document.RAPDetection.bssidMAC_DomainList.selectedIndex);
		legalBSSID_DeleteKeyword_click();
	}
} 
</script>
</head>
<body onload="updateState()">
<form method="post" name="RAPDetection" action="/goform/RogueAPDetection">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="SecurityFilter">Security Filter</td>
                   </tr></table>
</li>
<li class="w_text2">
<table width="95%" border="0" align=center  cellspacing="0">
<tr>
<td width="3%"><input name="rapSecurityNO" type="checkbox" value="<% getCfgZero(1, "websRogueAPSecurity"); %>" ></td>
<td width="22%" id="NOSecurity">NO Security    </td>
<td>&nbsp;</td>
</tr>
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="LegalAPFilter">Legal AP Filter</td>
                   </tr></table>
</li>
<li class="w_text2">
<table width="95%" border="0" align=center cellspacing="0">
<tr><td width="3%"><input type="checkbox" name="legalBSSID_Enable" value="<% getCfgZero(1, "websRogueAPLegalAP"); %>" ></td>
<td width="97%" id="LegalAP">Legal AP</td></tr>
<tr><td>&nbsp;</td></tr>
</table>
<table width="95%" border="0" align=center cellspacing="0">		   
<tr><td  width="10%" id="MACAddress" nowrap>MAC Address</td>
<td  width="2%">&nbsp;</td>
<td  width="20%">
<input type="text" name="macAddress_Domain1" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(1)">:
<input type="text" name="macAddress_Domain2" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(2)">:
<input type="text" name="macAddress_Domain3" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(3)">:
<input type="text" name="macAddress_Domain4" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(4)">:
<input type="text" name="macAddress_Domain5" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(5)">:
<input type="text" name="macAddress_Domain6" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="MACFocus(6)">
</td>
<td  width="2%">&nbsp;</td>
<td  width="66%"><input type="button" name="legalBSSID_AddKeyword"  id="BSSIDAdd" value="Add" style="width:60pt;" onClick="legalBSSID_AddKeyword_click(0)" ></td>
</tr>
<tr><td>&nbsp;</td></tr>
</table>
<table width="95%" border="0" align=center cellspacing="0">		   
<tr><td  width="10%" id="OUIAddress" nowrap>OUI Address</td>
<td  width="2%">&nbsp;</td>
<td  width="20%">
<input type="text" name="ouiAddress_Domain1" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="OUIFocus(1)">:
<input type="text" name="ouiAddress_Domain2" style="width:20px; text-align: center;" maxlength="2" value="" onKeyUp="OUIFocus(2)">:
<input type="text" name="ouiAddress_Domain3" style="width:20px; text-align: center;" maxlength="2" value="">:
<input type="text" name="ouiAddress_Domain4" style="width:20px; text-align: center;" maxlength="2" value="**" disabled="">:
<input type="text" name="ouiAddress_Domain5" style="width:20px; text-align: center;" maxlength="2" value="**" disabled="">:
<input type="text" name="ouiAddress_Domain6" style="width:20px; text-align: center;" maxlength="2" value="**" disabled="">
</td>
<td  width="2%">&nbsp;</td>
<td  width="66%"><input type="button" name="legalOUI_AddKeyword"  id="OUIAdd" value="Add" style="width:60pt;" onClick="legalBSSID_AddKeyword_click(1)" ></td>
</tr>
<tr><td>&nbsp;</td></tr>
</table>
<table width="95%" border="0" align=center cellspacing="0">	   
<tr><td id="LegalAPList">Legal AP List                  </td></tr>
<tr><td>
<select name="bssidMAC_DomainList" size="6" style="width:150px" multiple="true">                     
<script language="JavaScript" type="text/javascript">
var i;
var entries = new Array();
var all_str = '<% getLegalAPList(); %>';
if(all_str.length){
entries = all_str.split("|");
for(i=0; i<entries.length-1; i++){                       
document.write(" <option >");
document.write(entries[i]);
document.write("</option >");
}	
}
</script>
</select>
</td></tr>
<tr><td height="5"></td></tr>
<tr><td>
<input type="button" name="legalBSSID_DeleteKeyword"  id="LegalAPDelete" value="Delete" style="width:60pt;"  onClick="legalBSSID_DeleteKeyword_click()" />&nbsp;
<input type="button" name="legalBSSID_ClearList" id="LegalAPClear" value="Clear All" style="width:60pt;" onClick="legalBSSID_ClearList_click()" />
</td></tr>
<tr><td>&nbsp;</td></tr>
</table>
</li>
</ul>
</div>
</li>

<center>
<li class="table_button">
<input type="button" value="Detection" id="rapDetection" name="add" onClick="DFSRequest()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="rapReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/rap_detection.asp" name="rapdetection_url">
<input type=hidden value="" name="bssid_str">
<input type=hidden value="" name="oui_str">
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
