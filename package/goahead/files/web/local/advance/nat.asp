<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="images/dtree.js"></script>
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>    
<script type="text/javascript" src="/lang/b28n.js"></script>

<script language="javascript" type="text/javascript">
//check Enter key submit form
document.onkeydown = function(event) { 
   var target,code,tag; 
   if (!event) {  
        event = window.event;
        target = event.srcElement;  //for IE^M
        code = event.keyCode; 
        if (code == 13) { 
             tag = target.tagName; 
             if (tag == "TEXTAREA") { return true; } 
             else { return false; }  
        }  
   }  
   else{  
            target = event.target; //for w3c stander eg:firefox
         code = event.keyCode; 
            if (code == 13) { 
                tag = target.tagName; 
                if (tag == "INPUT") { return false; }
                else { return true; }  
          } 
      }  
}; 
</script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");  
var lanIP = "<%getLanIp(); %>"; 

function initTranslation()
{
    var e = document.getElementById("natSetup");
    e.innerHTML = _("nat setup");
    e = document.getElementById("natTranslation");
	e.innerHTML = _("nat translation");	
	e = document.getElementById("natDefaultServerSetup");
	e.innerHTML = _("nat default server setup");
	e = document.getElementById("natServerIP");
	e.innerHTML = _("nat server ip");
	e = document.getElementById("dmzNote");
	e.innerHTML = _("nat dmz note");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
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
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
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


function check127net(str, num)
{
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
	if (d ==127 )
	{
	       alert("Error. IP address is loopback address.");
		return true;
	}
      
	return false;
}

function checkmultinet(str, num)
{
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
	if (d >223 && d <240 )
	{
	      alert("Error. IP address is multicast address.");
		return true;
	}      
	return false;
}

function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		//alert("Error. IP address is empty.");
		//field.value = field.defaultValue;
		field.value = "0.0.0.0";
        field.focus();
		return true;
	}
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		//field.value = field.defaultValue;
		field.focus();
		return false;
	}	

      if (check127net(field.value,1) == true) {
		field.focus();
		return false;
	}

	if (checkmultinet(field.value,1) == true) {
		field.focus();
		return false;
	}
	
	if (field.value == "0.0.0.0") {
		return true;
	}	
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			//field.value = field.defaultValue;
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
			//field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
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

function natClick(){
	if (checkIpAddr(document.NAT.NATServerIPAddress, false) == false)
		return false;	
		
	if ( !checkSubnet(document.NAT.lanIP.value, document.NAT.lanMask.value, document.NAT.NATServerIPAddress.value)) {
        		alert('Invalid server ip address!It should be located in the same subnet of current LAN IP address.');
        		//document.NAT.NATServerIPAddress.value = document.NAT.NATServerIPAddress.defaultValue;
        		document.NAT.NATServerIPAddress.focus();
        		return false;
    }
		
	document.NAT.NATEnabled.value = document.NAT.NATEnabled.checked ? "1": "0";	
	return true;
}
function formCheck()
{
	if(document.NAT.NATServerIPAddress.value == lanIP ){
		alert("Server ip couldn't be lan ip");
		return false;
	  }
    if(!natClick())
	   return false;
	showWebMessage(2, "");   
	return true;	   
}
function updateState()
{
	initTranslation();	
	if (document.NAT.NATEnabled.value == "1")
		document.NAT.NATEnabled.checked = true;	
}  
</script>
</head>
<body onload="updateState()">
<form method="post" name="NAT" action="/goform/DMZ_NAT" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="natSetup">NAT Setup_Steve</td>
                   </tr></table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="3%"><input type=checkbox name=NATEnabled  value="<% getCfgZero(1, "natEnabled"); %>"></td>
<td width="97%" id="natTranslation" >Enable Network Address Translation </td>
</tr>    
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="natDefaultServerSetup">Default Server Setup</td>
                   </tr></table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="natServerIP">Server IP Address : </td>
<td>
<input type="text" size="15" maxlength=15 name="NATServerIPAddress" value=<% showDMZIPAddressASP(); %> >
</td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id ="dmzNote">Note:When NAT mode disable, the DMZ setting is invalid.</span></td>
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
<input type="submit" value="Apply" id="natApply" name="addNAT" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/nat.asp" name="nat_url">
<input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
<input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>">
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
