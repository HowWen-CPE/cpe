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
</head>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/JavaScript">
Butterlate.setTextDomain("main");


function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
	       if(str.charAt(i) == ' ')
		    return false;
		else
	        continue;
	}
	return true;
}
function checkDomainName(str)
{
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') ||	(str.charAt(i) == '.') || (str.charAt(i) == '-'))
		     continue;
		else
	           return false;
	}
	return true;
}

function checkSystemName(str)
{
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') ||	(str.charAt(i) == '_') || (str.charAt(i) == '-'))
		     continue;
		else
	           return false;
	}
	return true;
}

function check_confirm()
{
	document.forms[0].admin_inactivity_timer.value = 
	parseInt(document.forms[0].admin_inactivity_timer.value+"");

	if (document.forms[0].system_name.value=="")
	{
		alert("System Name cannot be empty.");
		document.forms[0].system_name.focus();
		document.forms[0].system_name.select();
		return false;
	}
	else
   if(checkSystemName(document.forms[0].system_name.value) == false){
	alert("The only valid characters for a system name are letters numbers and a hyphen \-\_");	
	document.forms[0].domain_name.value=document.forms[0].domain_name.defaultValue;
	document.forms[0].domain_name.focus();
	document.forms[0].system_name.select();
	return false;
   }
	else
	if (document.forms[0].domain_name.value=="")
	{
		alert("Domain Name cannot be empty.");
		document.forms[0].domain_name.focus();
		document.forms[0].domain_name.select();
		return false;
	}
	else
       if(checkDomainName(document.forms[0].domain_name.value) == false){
		alert("The only valid characters for a domain name are letters numbers and a hyphen \-\.");
		document.forms[0].domain_name.focus();
		document.forms[0].domain_name.select();
		return false;
       }
	else
	if (document.forms[0].admin_inactivity_timer.value == "" )
	{
		alert('Please specify Administrator Inactivity Timer.');
		document.forms[0].admin_inactivity_timer.focus();
		document.forms[0].admin_inactivity_timer.select();
		return false;
	}
	else
	if (checkInjection(document.forms[0].admin_inactivity_timer.value) == false)
	{
		alert('Blank is invalid value.');
		document.forms[0].admin_inactivity_timer.focus();
		document.forms[0].admin_inactivity_timer.select();
		return false;
	}
	else
	if (isNaN(document.forms[0].admin_inactivity_timer.value) || document.forms[0].admin_inactivity_timer.value < 0)
	{
		alert('Invalid Administrator Inactivity Timer');
		document.forms[0].admin_inactivity_timer.focus();
		document.forms[0].admin_inactivity_timer.select();
		return false;
	}
	else
	{
		showWebMessage(2, "");
		document.forms[0].submit();
		return true;
	}
}
function initTranslation()
{
	var e;

	e = document.getElementById("maintainTitle");
	e.innerHTML = _("maintain title");
	e = document.getElementById("maintainSysName");
	e.innerHTML = _("maintain sys name");
	e = document.getElementById("maintainDomName");
	e.innerHTML = _("maintain dom name");
	e = document.getElementById("maintainAdmInTimer");
	e.innerHTML = _("maintain adm in timer");
	e = document.getElementById("maintainMinutes");
	e.innerHTML = _("maintain minutes");
	e = document.getElementById("maintainApply");
	e.value = _("maintain apply");
	e = document.getElementById("maintainReset");
	e.value = _("maintain reset");
}
</script>
<body onload="initTranslation();">
<form method="post" name="maintenance_general" action="/goform/maintenance_general">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="maintainTitle">System Setup </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="maintainSysName">System Name</font> : </td>
                      <td><input name="system_name" size="30" maxlength="30" value="<% getCfgGeneral(1, 'SystemName'); %>" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="maintainDomName">Domain Name</font>  : </td>
                      <td><input name="domain_name" size="30" maxlength="38" value="<% getCfgGeneral(1, 'DomainName'); %>" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="maintainAdmInTimer">Administrator Inactivity Timer</font> : </td>
                      <td><input name="admin_inactivity_timer" size="10" maxlength="4" 
                      value="<% getCfgGeneral(1, 'AdminInactivityTimer'); %>" type="text" 
						 onKeyUp="value=value.replace(/[^\d]/g,'');" />
                      (<font id="maintainMinutes">minutes, 0 means no timeout</font> )</td>
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
        <input type="submit" value="Apply" id="maintainApply" onclick="return check_confirm();">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" value="Reset" id="maintainReset" onClick="window.location.reload()">
        </li>
              </center>
</body>
</form>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
