<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
function initTranslation(){

	<!-- var e = document.getElementById("RemoteManagementServerPort"); -->
	<!-- e.innerHTML = _("remote management server port"); -->
	<!-- e = document.getElementById("RemoteManagementServerAccess"); -->
	<!-- e.innerHTML = _("remote management server access"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIP"); -->
	<!-- e.innerHTML = _("remote management secured ip"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIPAll"); -->
	<!-- e.innerHTML = _("remote management secured ip all"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIPSel"); -->
	<!-- e.innerHTML = _("remote management secured ip sel"); -->
	<!-- e = document.getElementById("natApply"); -->
	<!-- e.value = _("nat apply"); -->
	<!-- e = document.getElementById("natReset"); -->
	<!-- e.value = _("nat reset"); -->
	
	<!-- e = document.getElementById("remoteManagementWWW"); -->
	<!-- e.innerHTML = _("remote management www"); -->
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
	      alert("Error. IP address is not valid.");
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
	return true;
}
function passwd_check_len(obj, min, max) {
    if((obj.value.length>=min) && (obj.value.length<=max))
        return true;
    else 
   {
       obj.focus();
       obj.select();
       alert("passwd valid string length is ["+min+","+max+"]");
       return false;
   }
}
function checkStrInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' || str.charAt(i) == '^' || str.charAt(i) == '|' || str.charAt(i) == '%' || str.charAt(i) == '$' || str.charAt(i) == ' ' || str.charAt(i) == ';' || str.charAt(i) == '"'|| str.charAt(i) == ',' || str.charAt(i) == '&'){
			return false;
		}else
	        continue;
	}
	return true;
}

function myformCheck(){
    if(document.opendns.opendns_service_enable[1].checked){
	showWebMessage(2, "");
	return true;
    }
    if(document.opendns.opendns_username.value == ""){
	alert('user name can not null!!');
	return false;
    }
    if(!passwd_check_len(document.opendns.opendns_password,6,20))
    	return false;
    if(document.opendns.opendns_lable.value == ""){
	alert('opendns lable can not null!!');
	return false;
    }
    if(checkStrInjection(document.opendns.opendns_password.value) == false){
	alert('System do not support \^\|\.\%\;\$ \,\&\" characters.');
	return false;
    }
    if(checkStrInjection(document.opendns.opendns_username.value) == false){
	alert('System do not support \^\|\.\%\;\$ \,\&\" characters.');
	return false;
    }
    if(checkStrInjection(document.opendns.opendns_lable.value) == false){
	alert('System do not support \^\|\.\%\;\$ \,\&\" characters.');
       return false;
   }       
    showWebMessage(2, "");
   return true;
}
function ServiceRadioChange(){
    var form=document.opendns;
    if(form.opendns_service_enable[0].checked) {
        form.opendns_username.disabled=false;
        form.opendns_password.disabled=false;
        form.opendns_redircet_enable[0].disabled=false;
        form.opendns_redircet_enable[1].disabled=false;
        form.opendns_lable.disabled=false;
    }else {
        form.opendns_username.disabled=true;
        form.opendns_password.disabled=true;
        form.opendns_redircet_enable[0].disabled=true;
        form.opendns_redircet_enable[1].disabled=true;
        form.opendns_lable.disabled=true;
    }
    return true;
}

function RedirectRadioChange(){

}

function updateState(){
    initTranslation();
    var service_enable = "<% getCfgGeneral(1, "opendns_service"); %>";
    var redirect_enable = "<% getCfgGeneral(1,"opendns_redircet");%>";
    if(service_enable == 1) {
       document.opendns.opendns_service_enable[0].checked = true;
    }else
       document.opendns.opendns_service_enable[1].checked = true;
    if(redirect_enable == 1)
       document.opendns.opendns_redircet_enable[0].checked = true;
    else
       document.opendns.opendns_redircet_enable[1].checked = true;
    ServiceRadioChange();
    return ;
}

</script>
</head>
<body onload="updateState()">
<form method="post" name="opendns" action="/goform/OPENDNS" onSubmit="return myformCheck();">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="remoteManagementWWW">OpenDNS</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">OpenDNS Service: </td>
<td><input name="opendns_service_enable" onclick="ServiceRadioChange()" value="1" type="radio" />&nbsp;&nbsp;Enable</td>

<td><input name="opendns_service_enable" onclick="ServiceRadioChange()" value="0" type="radio" />&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> OpenDNS Username: </td>
<td><input name="opendns_username" size="32" maxlength="32" value="<% getCfgGeneral(1, "opendns_username");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> OpenDNS Password: </td>
<td><input name="opendns_password" size="32" maxlength="32" value="<% getCfgGeneral(1, "opendns_password");%>" type="password" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">DNS Query Redirection to OpenDNS DNS Servers: </td>
<td><input name="opendns_redircet_enable" onclick="RedirectRadioChange()" value="1" type="radio" />&nbsp;&nbsp;Enable</td>

<td><input name="opendns_redircet_enable" onclick="RedirectRadioChange()" value="0" type="radio" />&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> OpenDNS Label: </td>
<td><input name="opendns_lable" size="32" maxlength="32" value="<% getCfgGeneral(1, "opendns_lable");%>" type="text" /></td>
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
<input type="submit" value="Apply" id="natApply" name="add"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Cancel" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/opendns.asp" name="opendns_url">
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
