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
    
	e = document.getElementById("contact");
	e.innerHTML = _("contact");
	
	e = document.getElementById("snmpversion");
	e.innerHTML = _("snmpversion");
	
	e = document.getElementById("User_Name");
	e.innerHTML = _("User_Name");
	
	e = document.getElementById("Security_Level");
	e.innerHTML = _("Security_Level");
	
	e = document.getElementById("snmp_user_ro_passwd_td");
	e.innerHTML = _("snmp_user_ro_passwd_td");
	
	e = document.getElementById("snmp_user_rw_passwd_td");
	e.innerHTML = _("snmp_user_rw_passwd_td");

	e = document.getElementById("community_read");
	e.innerHTML = _("community read");
	
	e = document.getElementById("community_write");
	e.innerHTML = _("community write");
		
	e = document.getElementById("trap_ip");
	e.innerHTML = _("trap ip");
	
	e = document.getElementById("snmpenable");
	e.innerHTML = _("snmp enable");
	
	e = document.getElementById("snmpdisable");
	e.innerHTML = _("snmp disable");
	
	e = document.getElementById("sshenable");
	e.innerHTML = _("ssh enable");
	
	e = document.getElementById("sshdisable");
	e.innerHTML = _("ssh disable");
	
	e = document.getElementById("telnetenable");
	e.innerHTML = _("telnet enable");
	
	e = document.getElementById("telnetdisable");
	e.innerHTML = _("telnet disable");
	
	e = document.getElementById("httpsenable");
	e.innerHTML = _("https enable");
	
	e = document.getElementById("httpsdisable");
	e.innerHTML = _("https disable");
	
	e = document.getElementById("sshserverport");
	e.innerHTML = _("ssh server port");
	
	e = document.getElementById("telnetserverport");
	e.innerHTML = _("telnet server port");
	
	e = document.getElementById("httpsserverport");
	e.innerHTML = _("https server port");
	
  	e = document.getElementById("serviceApply");
	e.value = _("service apply");
	
	e = document.getElementById("serviceReset");
	e.value = _("service reset");
}

//0<port<=0xffff
function isvalidport(str){
	var j=0;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) >= '0' && str.charAt(i) <= '9')
		{
			if(str.charAt(i) == '0')
				j++;
			continue;
		}
		return 0;
	}
	if(j==str.length)
		return 0;
	if(parseInt(str) > 0xffff)
		return 0;
	return 1;
}

function checkeMail(str)
{
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') ||
				(str.charAt(i) == '@') || (str.charAt(i) == '.'))
		     continue;
		else
	           return false;
	}
	return true;
}

function checkName(str)
{
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') ||
				(str.charAt(i) == '_') || (str.charAt(i) == '-'))
		     continue;
		else
	           return false;
	}
	return true;
}

function checkPwd(str)
{
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z'))
		     continue;
		else
	           return false;
	}
	return true;
}

function service_check_len(obj, min, max) {
if((obj.value.length>=min) && (obj.value.length<=max))
return true;
else {
obj.focus();
obj.select();
alert("Valid string length is ["+min+","+max+"]");
return false;
}
}
function check_IP()    
{  var ip = document.getElementById('snmp_trap_ip').value;
   var re=/^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;   
   if(re.test(ip))   
   {   
       if( RegExp.$1<224 && RegExp.$2<255 && RegExp.$3<255 && RegExp.$4<255) 
		{
		   if( RegExp.$1==127) 
		   	{
		   		alert("IP adress format error.");
				document.service.snmp_trap_ip.value = document.service.snmp_trap_ip.defaultValue;
				document.service.snmp_trap_ip.focus();				
		   		return false;
		   	}
			if( RegExp.$1==0 && (RegExp.$2>0 || RegExp.$3>0 || RegExp.$4>0)) 
		   	{
		   		alert("IP adress format error.");
				document.service.snmp_trap_ip.value = document.service.snmp_trap_ip.defaultValue;
				document.service.snmp_trap_ip.focus();
		   		return false;
		   	}
			return true;   
		}
   }   
   alert("IP adress format error.");
	document.service.snmp_trap_ip.value = document.service.snmp_trap_ip.defaultValue;
	document.service.snmp_trap_ip.focus();
   return false;    
}
function myformCheck(){
var form = document.service;
if(form.snmp_enable[0].checked)
{
	document.service.snmp_version_1.value= document.service.snmp_version_1.checked ? "1" : "0";
	document.service.snmp_version_2c.value = document.service.snmp_version_2c.checked ? "1": "0";
	document.service.snmp_version_usm.value=document.service.snmp_version_usm.checked ?"1":"0";

	var obj=document.getElementById("snmpd_sel");
	var selvalue=obj.options[obj.selectedIndex].value;
	var e = document.getElementById("snmpd_sel_index");
	e.value = selvalue;
	
	if(form.snmp_enable[0].checked && !form.snmp_version_1.checked &&
	!form.snmp_version_2c.checked && !form.snmp_version_usm.checked) 
	{
		alert("You must choose one SNMP version");
		return false;
	}
	if(!service_check_len(form.snmp_contact,0,32))
	{
		return false;
	}
	if(form.snmp_version_1.checked || form.snmp_version_2c.checked) 
	{ 
		if(!service_check_len(form.snmp_comm_ro,1,32))
		{
			return false;
		}
		if(!service_check_len(form.snmp_comm_rw,1,32))
		{
			return false;
		}
		if(!check_IP())
		{
			return false;
		}
		if(form.snmp_comm_ro.value == form.snmp_comm_rw.value)
		{
			alert("ro community name should diffrent from rw community");
			return false;
		}
		if(checkeMail(form.snmp_contact.value) == false)
		{
			alert("Characters supported are letters, numbers, and  \@\.");	
			form.snmp_contact.value=form.snmp_contact.defaultValue;
			form.snmp_contact.focus();
			return false;
	    }

		if(checkName(form.snmp_comm_rw.value) == false)
		{
			alert("Characters supported are letters, numbers, and  \-\_");	
			form.snmp_comm_rw.value=form.snmp_comm_rw.defaultValue;
			form.snmp_comm_rw.focus();
			return false;
	    }
		if(checkName(form.snmp_comm_ro.value) == false)
		{
			alert("Characters supported are letters, numbers, and  \-\_");	
			form.snmp_comm_ro.value=form.snmp_comm_ro.defaultValue;
			form.snmp_comm_ro.focus();
			return false;
	    }
	}
	if(form.snmp_version_usm.checked) 
	{
		if(!service_check_len(form.snmp_user_ro_passwd,8,15))
		{
			return false;
		}
		if(!service_check_len(form.snmp_user_rw,1,32))
		{
			return false;
		}
		if(checkName(form.snmp_user_rw.value) == false)
		{
			alert("Characters supported are letters, numbers, and  \-\_");	
			form.snmp_user_rw.value=form.snmp_user_rw.defaultValue;
			form.snmp_user_rw.focus();
			return false;
	    }
		var usmusr=form.snmp_user_rw.value;
		if(usmusr.charAt(0)=='-')
		{
			alert("User Name cannot start with '-'");
			form.snmp_user_rw.value=form.snmp_user_rw.defaultValue;
			form.snmp_user_rw.focus();
			return false;
		}
		if(checkPwd(form.snmp_user_ro_passwd.value) == false)
		{
			alert("Characters supported are letters and numbers");	
			form.snmp_user_ro_passwd.value=form.snmp_user_ro_passwd.defaultValue;
			form.snmp_user_ro_passwd.focus();
			return false;
	    }		
		if(selvalue == 1)
		{
			if(!service_check_len(form.snmp_user_rw_passwd,8,15))
			{
				return false;
			}
			if(checkPwd(form.snmp_user_rw_passwd.value) == false)
			{
				alert("Characters supported are letters and numbers");	
				form.snmp_user_rw_passwd.value=form.snmp_user_rw_passwd.defaultValue;
				form.snmp_user_rw_passwd.focus();
				return false;
			}	
		}
	}
}
if(form.ssh_enable[0].checked)
{
	if(!isvalidport(form.ssh_port.value))
	{
		alert("invalid ssh port number!");
		return false;
	}
}
if(form.telnet_enable[0].checked)
{
	if(!isvalidport(form.telnet_port.value))
	{
		alert("invalid telnet port number!");
		return false;
	}
}
if(form.https_enable[0].checked)
{
	if(!isvalidport(form.https_port.value))
	{
		alert("invalid https port number!");
		return false;
	}
}
if((form.https_port.value) == (form.telnet_port.value))
{
	alert("Port has been occupied");
	form.telnet_port.value=form.telnet_port.defaultValue;
	form.https_port.value=form.https_port.defaultValue;		
	form.telnet_port.focus();
	return false;
}
showWebMessage(2, "");
return true;
}

function updateState(){
initTranslation();
var enable = "<% getCfgGeneral(1, "SNMP_enable"); %>";
var snmp_version_v1 = "<% getCfgGeneral(1,"SNMP_version_v1");%>";
var snmp_version_v2c = "<% getCfgGeneral(1,"SNMP_version_v2c");%>";
var snmp_version_usm = "<% getCfgGeneral(1,"SNMP_version_usm");%>";
var sshenable = "<% getCfgGeneral(1, "ssh_enable"); %>";
var telnetenable = "<% getCfgGeneral(1, "telnet_enable"); %>";
var httpsenable = "<% getCfgGeneral(1, "https_enable"); %>";
var snmpd_sel_index = "<% getCfgGeneral(1, "snmpd_sel_id"); %>";
if(enable == 1) {
document.service.snmp_enable[0].checked = true;
}else
document.service.snmp_enable[1].checked = true;
if(snmp_version_v1 == 1)
document.service.snmp_version_1.checked = true;
if(snmp_version_v2c == 1)
document.service.snmp_version_2c.checked = true;
if(snmp_version_usm == 1)
document.service.snmp_version_usm.checked = true;
if(sshenable == 1) {
document.service.ssh_enable[0].checked = true;
}else
document.service.ssh_enable[1].checked = true;
if(telnetenable == 1) {
document.service.telnet_enable[0].checked = true;
}else
document.service.telnet_enable[1].checked = true;
if(httpsenable == 1) {
document.service.https_enable[0].checked = true;
}else
document.service.https_enable[1].checked = true;
if(snmpd_sel_index==0)
{
document.service.snmpd_sel.options[0].selected = true;
}
else if(snmpd_sel_index == 1)
{
document.service.snmpd_sel.options[1].selected = true;
}
if(document.service.snmpd_sel.options[0].selected == true)
{
	document.getElementById("snmp_user_rw_passwd").style.display="none";
	document.getElementById("snmp_user_rw_passwd_td").style.display="none";		
}
radioChange();
sshchange();
telnetchange();
httpschange();
return ;
}

function radioChange(){
var form=document.service;
if(form.snmp_enable[0].checked) {
form.snmp_version_1.disabled=false;
form.snmp_version_2c.disabled=false;
form.snmp_version_usm.disabled=false;
form.snmp_contact.disabled=false;
form.snmp_comm_ro.disabled=false;
form.snmp_comm_rw.disabled=false;
form.snmp_user_ro_passwd.disabled=false;
form.snmp_user_rw.disabled=false;
form.snmp_user_rw_passwd.disabled=false;
form.snmp_trap_ip.disabled=false;
VersionCheck();
}else {
form.snmp_version_1.disabled=true;
form.snmp_version_2c.disabled=true;
form.snmp_version_usm.disabled=true;
form.snmp_contact.disabled=true;
form.snmp_comm_ro.disabled=true;
form.snmp_comm_rw.disabled=true;
form.snmp_user_ro_passwd.disabled=true;
form.snmp_user_rw.disabled=true;
form.snmp_user_rw_passwd.disabled=true;
form.snmp_trap_ip.disabled=true;
form.snmpd_sel.disabled=true;
}
return true;
}
function chg()
{
	var obj=document.getElementById("snmpd_sel");

	if(obj.selectedIndex == "0")
	{
		document.getElementById("snmp_user_ro_passwd").style.display="";
		document.getElementById("snmp_user_rw_passwd").style.display="none";
		document.getElementById("snmp_user_ro_passwd_td").style.display="";
		document.getElementById("snmp_user_rw_passwd_td").style.display="none";		
	}
	if(obj.selectedIndex == "1")
	{
		document.getElementById("snmp_user_ro_passwd").style.display="";
		document.getElementById("snmp_user_rw_passwd").style.display="";
		document.getElementById("snmp_user_ro_passwd_td").style.display="";
		document.getElementById("snmp_user_rw_passwd_td").style.display="";		
	}
}

function sshchange(){
	if(document.service.ssh_enable[0].checked)
		document.service.ssh_port.disabled=false;
	else
		document.service.ssh_port.disabled=true;
}

function telnetchange(){
	if(document.service.telnet_enable[0].checked)
		document.service.telnet_port.disabled=false;
	else
		document.service.telnet_port.disabled=true;
}

function httpschange(){
	if(document.service.https_enable[0].checked)
		document.service.https_port.disabled=false;
	else
		document.service.https_port.disabled=true;
}
function VersionCheck()
{
var form=document.service;
if(form.snmp_version_usm.checked == true) {
form.snmp_user_ro_passwd.disabled=false;
form.snmp_user_rw.disabled=false;
form.snmp_user_rw_passwd.disabled=false;
form.snmpd_sel.disabled=false;
}else {
form.snmp_user_ro_passwd.disabled=true;
form.snmp_user_rw.disabled=true;
form.snmp_user_rw_passwd.disabled=true;
form.snmpd_sel.disabled=true;
}
if(!form.snmp_version_1.checked && !form.snmp_version_2c.checked ) {
form.snmp_comm_ro.disabled=true;
form.snmp_comm_rw.disabled=true;
form.snmp_trap_ip.disabled=true;
}else {
form.snmp_comm_ro.disabled=false;
form.snmp_comm_rw.disabled=false;
form.snmp_trap_ip.disabled=false;
}
}
</script>
</head>
<body onload="updateState()">
<form method="post" name="service" action="/goform/service" onSubmit="return myformCheck();">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="remoteManagementWWW">SNMP</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="SNMP">SNMP : </td>
<td><input name="snmp_enable" onclick="radioChange()" value="1" type="radio" /></td>
<td id="snmpenable"></td>
<td><input name="snmp_enable" onclick="radioChange()" value="0" type="radio" /></td>
<td id="snmpdisable"></td>
<td width="43%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="snmpversion">SNMP Version Support : </td>
<td><input name="snmp_version_1" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v1 </td>

<td><input name="snmp_version_2c" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v2 </td>

<td><input name="snmp_version_usm" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v3</td>
<td width="35%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="contact"> Contact: </td>
<td><input name="snmp_contact" style="width:200px" maxlength="32" value="<% getCfgGeneral(1, "SNMP_contact");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="community_read"> Community Name(read only): </td>
<td><input name="snmp_comm_ro" style="width:200px" maxlength="32" value="<% getCfgGeneral(1, "SNMP_comm_ro");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="community_write"> Community Name(read/write): </td>
<td><input name="snmp_comm_rw" style="width:200px" maxlength="32" value="<% getCfgGeneral(1, "SNMP_comm_rw");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
  <td width="40%" id="trap_ip"> Trap IP: </td>
  <td><input name="snmp_trap_ip" id="snmp_trap_ip" style="width:200px" maxlength="32" onkeyup="value=value.replace(/[^\d\.]/g,'')" value="<% getCfgGeneral(1, "SNMP_trap_ip");%>" type="text" /></td>
</tr>
</table>
</li>

<li class="w_text" >
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="User_Name"> User Name: </td>
<td><input name="snmp_user_rw" id="snmp_user_rw" style="width:200px" maxlength="32" value="<% getCfgGeneral(1, "SNMP_user_rw");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="Security_Level"> Security Level: </td>
<td>
<select name="snmpd_sel" id="snmpd_sel" onChange="chg()" style="width:202px">
<option value="0" >authentication(MD5), no privacy &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option>
<option value="1" >authentication(MD5), privacy(DES)</option>
</select>
<input type="hidden" name="snmpd_sel_index" id="snmpd_sel_index" value="">
</td>
</tr>
</table>
</li>
<li class="w_text" >
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="snmp_user_ro_passwd_td" >Auth Password: </td>
<td><input name="snmp_user_ro_passwd" id="snmp_user_ro_passwd"  style="width:200px" maxlength="15" value="<% getCfgGeneral(1, "SNMP_user_ro_passwd");%>" type="password" /></td>
</tr>
</table>
</li>
<li class="w_text" >
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="snmp_user_rw_passwd_td"  >Privacy Password: </td>
<td><input name="snmp_user_rw_passwd" id="snmp_user_rw_passwd" style="width:200px" maxlength="15" value="<% getCfgGeneral(1, "SNMP_user_rw_passwd");%>" type="password" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>

<!--SSH start-->
<li class="space3"  style="visibility:hidden;display:none"></li>
<li class="table_content"  style="visibility:hidden;display:none">
<div class="data"  style="visibility:hidden;display:none">
<ul  style="visibility:hidden;display:none">
<li class="title" id="ManagementSSH">SSH</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="ManagementSSHFunc">SSH : </td>
<td><input name="ssh_enable" onclick="sshchange()" value="1" type="radio" /></td>
<td id="sshenable"></td>
<td><input name="ssh_enable" onclick="sshchange()" value="0" type="radio" /></td>
<td id="sshdisable"></td>
<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="sshserverport"> Server Port: </td>
<td><input name="ssh_port" size="5" maxlength="5" value="<% getCfgGeneral(1, "ssh_port");%>" type="text" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>
<!--SSH end-->

<!--TELNET start-->
<li class="space3"></li>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="ManagementTELNET">Telnet</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="ManagementTelnetFunc">Telnet : </td>
<td><input name="telnet_enable" onclick="telnetchange()" value="1" type="radio" /></td>
<td id="telnetenable"></td>
<td><input name="telnet_enable" onclick="telnetchange()" value="0" type="radio" /></td>
<td id="telnetdisable"></td>
<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="telnetserverport"> Server Port: </td>
<td><input name="telnet_port" size="5" maxlength="5" value="<% getCfgGeneral(1, "telnet_port");%>" type="text" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>
<!--TELNET end-->

<!--https start-->
<li class="space3"></li>
<li class="table_content">
<div class="data">
<ul >
<li class="title" id="Managementhttps">HTTPS</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="ManagementhttpsFunc">HTTPS : </td>
<td><input name="https_enable" onclick="httpschange()" value="1" type="radio" /></td>
<td id="httpsenable"></td>
<td><input name="https_enable" onclick="httpschange()" value="0" type="radio" /></td>
<td id="httpsdisable"></td>
<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="httpsserverport"> Port: </td>
<td><input name="https_port" size="5" maxlength="5" onkeyup="this.value=this.value.replace(/\D/g,'')" value="<% getCfgGeneral(1, "https_port");%>" type="text" /></td>
</tr>
</table>
</li>

<li></li>
</ul>
</div>
</li>
<!--https end-->

<center>
<li class="table_button">
<input type="submit" value="Apply" id="serviceApply" name="add" onClick="return myformCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Cancel" id="serviceReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/service.asp" name="service_url">
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
