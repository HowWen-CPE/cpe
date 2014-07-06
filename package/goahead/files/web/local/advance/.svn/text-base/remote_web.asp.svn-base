<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
var lanIP = "<%getLanIp(); %>"; 
var lanMask="<% getLanNetmask(); %>";
var sys_port_list="<% get_sys_sock_port_list(); %>";

function find_www_port(port){
	var port_array = new Array();
	
	port_array = sys_port_list.split(":");
	
	for (i = 0; i < port_array.length; i ++) {
			if(port_array[i] == port)
					return true;	
	}
	
	return false;
}

function initTranslation(){
	var e = document.getElementById("RemoteManagementServerPort");
	e.innerHTML = _("remote management server port");
	e = document.getElementById("RemoteManagementServerAccess");
	e.innerHTML = _("remote management server access");
	e = document.getElementById("RemoteManagementSecuredIP");
	e.innerHTML = _("remote management secured ip");
	e = document.getElementById("RemoteManagementSecuredIPAll");
	e.innerHTML = _("remote management secured ip all");
	e = document.getElementById("RemoteManagementSecuredIPSel");
	e.innerHTML = _("remote management secured ip sel");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
	
	e = document.getElementById("remoteManagementWWW");
	e.innerHTML = _("remote management www");
}
function atoi(str, num){
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
function checkRange(str, num, min, max){
        var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
             return false;
	}
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}
function isAllNum(str){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkSubnet(ip, mask, client){
	if(client == "0.0.0.0")
		return false;

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

function checkIpAddr(field, ismask){
	if (field.value == "") {
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}	
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}

	if ( checkSubnet(lanIP, lanMask, field.value)) {
		alert('Invalid server ip address!\nIt should be located in different subnet of current LAN IP address.');
		field.value = field.defaultValue;
		field.focus();
		return false;
    }
	
	return true;
}
function formCheck(){
   var port_value = document.REMOTE.remoteManagementPort.value;
   var num = parseInt(port_value);
   if(port_value == ""){
      alert("The value is invalid");
      return false;
   }else{       
        if(( num > 65535 ) || (num < 1)){
            alert("The value is invalid");  
   			return false;
   		} else if ( ( num < 1024) && (num != 80 )) {
   				alert("User-defined port must be greater than 1024");	
   				return false;
   		}
   		
   		if(find_www_port(port_value)) {
   			alert("The port already in use");
   			return false;	
   		}
   }
   if (document.REMOTE.btnwww[1].checked == true){    
   		if (checkIpAddr(document.REMOTE.txtwwwip, false) == false)
   		{
			return false;
		}
	}
	showWebMessage(2, "");
    return true;
}
function rmsChange(){
	 if (document.REMOTE.remoteManagementSel.options.selectedIndex == 1){
	 	document.getElementById("div_rmsip").className  = "on" ;
	 }else {
	 	document.getElementById("div_rmsip").className  = "off" ;
	 }
	 parent.adjustMyFrameHeight();
}
function updateState(){
	initTranslation();
	var rm = "<% getCfgZero(1, "RemoteManagementSel"); %>";
	if(rm == "1") {
		document.REMOTE.remoteManagementSel.options.selectedIndex = 1;
		document.getElementById("div_rmsip").className  = "on" ;
	}else {
		document.REMOTE.remoteManagementSel.options.selectedIndex = 0;
		document.getElementById("div_rmsip").className  = "off" ;
	}
	
	var nv_rm_sel = <% getCfgZero(1, "RemoteManagementSecuredSel"); %>;	
	if(nv_rm_sel == "0"){
		document.REMOTE.btnwww[0].checked = true;
		document.REMOTE.txtwwwip.disabled = true;
	}else if(nv_rm_sel == "1"){
		document.REMOTE.btnwww[1].checked = true;
		document.REMOTE.txtwwwip.disabled = false;
	}else{
		document.REMOTE.btnwww[0].checked = true;
		document.REMOTE.txtwwwip.disabled = true;
	}
}
function radioChange(){
	if (document.REMOTE.btnwww[0].checked == true)
      	document.REMOTE.txtwwwip.disabled = true;
    else
    	document.REMOTE.txtwwwip.disabled = false;
} 
</script>
</head>
<body onload="updateState()">
<form method="post" name="REMOTE" action="/goform/remote_web" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="remoteManagementWWW">WWW</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">Server Port : </td>
<td><span class="cfig_word">
<input name="remoteManagementPort" size="5" maxlength="5" value="<% getCfgGeneral(1, "RemoteManagementPort"); %>" type="text" />
</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerAccess">Server Access : </td>
<td><span class="cfig_word">
<select name="remoteManagementSel" size="1" onChange="rmsChange()">
<option value="0">Disable</option>
<option value="1">WAN</option>
</select>
</span></td>
</tr>
</table>
</li>
<span id="div_rmsip" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementSecuredIP">Secured Client IP Address   : </td>
<td><input name="btnwww" value="0"  onclick="radioChange()" type="radio" /></td>
<td id="RemoteManagementSecuredIPAll"> All </td>                  
<td><input name="btnwww" value="1" onclick="radioChange()" type="radio" /></td>
<td id="RemoteManagementSecuredIPSel"> Selected </td>
<td><input name="txtwwwip" size="20" maxlength="15" value="<% getCfgGeneral(1, 'RemoteManagementSecuredIP'); %>" type="text" /></td>
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
<input type="submit" value="Apply" id="natApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/remote_web.asp" name="remote_url">
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
