<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to  <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgGeneral(1, "wireless_mode_5g"); %>;
function submit_apply()
{
	if(!rangeCheck())
	{
		return false;
	}
	showWebMessage(2, "");
	return true;

}

function initTranslation()
{
	var e = document.getElementById("WMMTitle");
	e.innerHTML = _("WMMTitle");

	e = document.getElementById("wmmenable");
	e.innerHTML = _("wmmenable");
	
	e = document.getElementById("wmmdisable");
	e.innerHTML = _("wmmdisable");	
	
	e = document.getElementById("EnableWMM");
	e.innerHTML = _("Enable WMM");

	e = document.getElementById("NoAckEnable");
	e.innerHTML = _("NoAckEnable");
	
	e = document.getElementById("ac_type");
	e.innerHTML = _("ac_type");	
		
	e = document.getElementById("cwmin");
	e.innerHTML = _("cwmin");

	e = document.getElementById("cwmax");
	e.innerHTML = _("cwmax");
	
	e = document.getElementById("aifs");
	e.innerHTML = _("aifs");		
	
	e = document.getElementById("txop");
	e.innerHTML = _("txop");	
		
	e = document.getElementById("acm");
	e.innerHTML = _("acm");
		
	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");

	e = document.getElementById("GeneralRestore");
	e.value = _("restore");
}

function initValue()
{
	var flag = <% getOnloadDFSState(); %>;
	if(0 == flag)
	{
		window.location.href= '/local/advance/radar_waiting.asp';
	}
	initTranslation();

	var WMMEnable  = "<% getCfgGeneral(1, "wmm_enable_5g"); %>";
	var NoAck = "<% getCfgGeneral(1, "ack_enable_5g"); %>";
	var AC_BK = "<% getCfgGeneral(1, "ac_bk_5g"); %>";
	var AC_BE = "<% getCfgGeneral(1, "ac_be_5g"); %>";
	var AC_VI = "<% getCfgGeneral(1, "ac_vi_5g"); %>";
	var AC_VO = "<% getCfgGeneral(1, "ac_vo_5g"); %>";
	if(wirelessmode == "1")
	{
		document.getElementById("table1").style.display="none";
	}
	else
	{
		document.getElementById("table1").style.display="";
	}
	if (WMMEnable == 1)
	{
		document.wireless_advanced.wmm_enable_5g[0].checked = true;
	}
	else
	{
		document.wireless_advanced.wmm_enable_5g[1].checked = true;
	}
	wmmcheck();
	if (NoAck == 1)
	{
		document.wireless_advanced.ack_enable_5g.checked = true;
	}
	else
	{
		document.wireless_advanced.ack_enable_5g.checked = false;
	}
	if (AC_BK == 1)
	{
		document.wireless_advanced.ac_bk_5g.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_bk_5g.checked = false;
	}

	if (AC_BE == 1)
	{
		document.wireless_advanced.ac_be_5g.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_be_5g.checked = false;
	}	
	if (AC_VI == 1)
	{
		document.wireless_advanced.ac_vi_5g.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_vi_5g.checked = false;
	}

	if (AC_VO == 1)
	{
		document.wireless_advanced.ac_vo_5g.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_vo_5g.checked = false;
	}
}

function wmmcheck()
{
	var form=document.wireless_advanced;
	if(form.wmm_enable_5g[0].checked == true) 
	{
		form.ack_enable_5g.disabled=false;
		form.cwmin_bk_5g.disabled=false;
		form.cwmin_be_5g.disabled=false;
		form.cwmin_vi_5g.disabled=false;
		form.cwmin_vo_5g.disabled=false;
		form.cwmax_bk_5g.disabled=false;
		form.cwmax_be_5g.disabled=false;
		form.cwmax_vi_5g.disabled=false;
		form.cwmax_vo_5g.disabled=false;
		form.aifs_bk_5g.disabled=false;
		form.aifs_be_5g.disabled=false;
		form.aifs_vi_5g.disabled=false;
		form.aifs_vo_5g.disabled=false;
		form.txop_bk_5g.disabled=false;
		form.txop_be_5g.disabled=false;
		form.txop_vi_5g.disabled=false;
		form.txop_vo_5g.disabled=false;
		form.ac_bk_5g.disabled=false;
		form.ac_be_5g.disabled=false;			
		form.ac_vi_5g.disabled=false;
		form.ac_vo_5g.disabled=false;		
	}
	else 
	{
		form.ack_enable_5g.disabled=true;
		form.cwmin_bk_5g.disabled=true;
		form.cwmin_be_5g.disabled=true;
		form.cwmin_vi_5g.disabled=true;
		form.cwmin_vo_5g.disabled=true;
		form.cwmax_bk_5g.disabled=true;
		form.cwmax_be_5g.disabled=true;
		form.cwmax_vi_5g.disabled=true;
		form.cwmax_vo_5g.disabled=true;
		form.aifs_bk_5g.disabled=true;
		form.aifs_be_5g.disabled=true;
		form.aifs_vi_5g.disabled=true;
		form.aifs_vo_5g.disabled=true;
		form.txop_bk_5g.disabled=true;
		form.txop_be_5g.disabled=true;
		form.txop_vi_5g.disabled=true;
		form.txop_vo_5g.disabled=true;
		form.ac_bk_5g.disabled=true;
		form.ac_be_5g.disabled=true;			
		form.ac_vi_5g.disabled=true;
		form.ac_vo_5g.disabled=true;		
	}
}
function check_len(obj, min, max) {
if((obj.value.length>=min) && (obj.value.length<=max))
return true;
else {
obj.focus();
obj.select();
_alert("Valid string length is ["+min+","+max+"]");
return false;
}
} 

function RestoreValue()
{
	var form=document.wireless_advanced;
	form.wmm_enable_5g[0].checked == true;
	form.ack_enable_5g.checked = false;
	form.ac_bk_5g.checked = false;
	form.ac_be_5g.checked = false;
	form.ac_vi_5g.checked = false;
	form.ac_vo_5g.checked = false;

	form.cwmin_bk_5g.value="4";
	form.cwmax_bk_5g.value="6";
	form.cwmin_be_5g.value="4";
	form.cwmax_be_5g.value="10";
	form.cwmin_vi_5g.value="3";
	form.cwmax_vi_5g.value="4";
	form.cwmin_vo_5g.value="2";
	form.cwmax_vo_5g.value="3";
	form.aifs_bk_5g.value="3";
	form.txop_bk_5g.value="0";
	form.aifs_be_5g.value="7";
	form.txop_be_5g.value="0";
	form.aifs_vi_5g.value="1";
	form.txop_vi_5g.value="3008";
	form.aifs_vo_5g.value="1";
	form.txop_vo_5g.value="1504";
	return false;
}

function rangeCheck()
{
	var form=document.wireless_advanced;
	if(!check_len(form.cwmin_bk_5g,1,2) || !check_len(form.cwmin_be_5g,1,2) 
	|| !check_len(form.cwmin_vi_5g,1,2) || !check_len(form.cwmin_vo_5g,1,2)
	|| !check_len(form.cwmax_bk_5g,1,2) || !check_len(form.cwmax_be_5g,1,2)
	|| !check_len(form.cwmax_vi_5g,1,2) || !check_len(form.cwmax_vo_5g,1,2) 
	|| !check_len(form.aifs_bk_5g,1,2) || !check_len(form.aifs_be_5g,1,2)
	|| !check_len(form.aifs_vi_5g,1,2) || !check_len(form.aifs_vo_5g,1,2))
	{
		return false;
	}
	if(!check_len(form.txop_bk_5g,1,4) || !check_len(form.txop_be_5g,1,4) 
	|| !check_len(form.txop_vi_5g,1,4) || !check_len(form.txop_vo_5g,1,4))
	{
		return false;
	}
	if(parseInt(form.cwmin_bk_5g.value) >  parseInt(form.cwmax_bk_5g.value))
	{
		_alert("ac_bk's cwmin");
		form.cwmin_bk_5g.value = form.cwmin_bk_5g.defaultValue;
		form.cwmax_bk_5g.value = form.cwmax_bk_5g.defaultValue;
		form.cwmin_bk_5g.focus();
		return false;
	}
	if(parseInt(form.cwmin_be_5g.value) >  parseInt(form.cwmax_be_5g.value))
	{
		_alert("ac_be's cwmin");
		form.cwmin_be_5g.value = form.cwmin_be_5g.defaultValue;
		form.cwmax_be_5g.value = form.cwmax_be_5g.defaultValue;
		form.cwmax_be_5g.focus();		
		return false;
	}
	if(parseInt(form.cwmin_vi_5g.value) > parseInt(form.cwmax_vi_5g.value))
	{
		_alert("ac_vi's cwmin");
		form.cwmin_vi_5g.value = form.cwmin_vi_5g.defaultValue;
		form.cwmax_vi_5g.value = form.cwmax_vi_5g.defaultValue;
		form.cwmax_vi_5g.focus();		
		return false;
	}
	if(parseInt(form.cwmin_vo_5g.value) > parseInt(form.cwmax_vo_5g.value))
	{
		_alert("ac_vo's cwmin");
		form.cwmin_vo_5g.value = form.cwmin_vo_5g.defaultValue;
		form.cwmax_vo_5g.value = form.cwmax_vo_5g.defaultValue;
		form.cwmax_vo_5g.focus();		
		return false;
	}
	if(parseInt(form.cwmin_bk_5g.value)>15 || parseInt(form.cwmax_bk_5g.value)>15||
	parseInt(form.cwmin_be_5g.value)>15||parseInt(form.cwmax_be_5g.value)>15||
	parseInt(form.cwmin_vi_5g.value)>15||parseInt(form.cwmax_vi_5g.value)>15||
	parseInt(form.cwmin_vo_5g.value)>15||parseInt(form.cwmax_vo_5g.value)>15||
	parseInt(form.aifs_bk_5g.value)>15|| parseInt(form.aifs_be_5g.value)>15||
	parseInt(form.aifs_vi_5g.value)>15||parseInt(form.aifs_vo_5g.value)>15)
	{
		form.cwmin_bk_5g.value = form.cwmin_bk_5g.defaultValue;
		form.cwmax_bk_5g.value = form.cwmax_bk_5g.defaultValue;
		form.cwmin_be_5g.value = form.cwmin_be_5g.defaultValue;
		form.cwmax_be_5g.value = form.cwmax_be_5g.defaultValue;
		form.cwmin_vi_5g.value = form.cwmin_vi_5g.defaultValue;
		form.cwmax_vi_5g.value = form.cwmax_vi_5g.defaultValue;
		form.cwmin_vo_5g.value = form.cwmin_vo_5g.defaultValue;
		form.cwmax_vo_5g.value = form.cwmax_vo_5g.defaultValue;
		form.aifs_bk_5g.value = form.aifs_bk_5g.defaultValue;
		form.aifs_be_5g.value = form.aifs_be_5g.defaultValue;
		form.aifs_vi_5g.value = form.aifs_vi_5g.defaultValue;
		form.aifs_vo_5g.value = form.aifs_vo_5g.defaultValue;	
		form.aifs_vo_5g.focus();							
		_alert("wmm range limit");
		return false;
	}
	if(parseInt(form.txop_bk_5g.value)>8192 || parseInt(form.txop_be_5g.value)>8192||
	parseInt(form.txop_vi_5g.value)>8192||parseInt(form.txop_vo_5g.value)>8192)
	{
		form.txop_bk_5g.value = form.txop_bk_5g.defaultValue;
		form.txop_be_5g.value = form.txop_be_5g.defaultValue;
		form.txop_vi_5g.value = form.txop_vi_5g.defaultValue;
		form.txop_vo_5g.value = form.txop_vo_5g.defaultValue;		
		form.txop_vo_5g.focus();	
		_alert("txop limit");
		return false;
	}
	return true;
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/WMM_QoS_5G" onSubmit="return submit_apply()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="WMMTitle">5G WMM Configuration </li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="20%" id="EnableWMM">Enable WMM : </td>
<td><input name="wmm_enable_5g" onclick="wmmcheck()" value="1" type="radio" /></td>
<td id="wmmenable">Enable</td>
<td><input name="wmm_enable_5g" onclick="wmmcheck()" value="0" type="radio" /></td>
<td id="wmmdisable">Disable</td>
<td width="60%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="ack_enable_5g" type="checkbox" value="1" />
<font id ="NoAckEnable">NoACK Policy</font>
</td>
</tr>
</table>
</li>               

</ul>
<ul>
<li class="w_text">
	<table width="100%" border="0" id="table1" align="center" cellpadding="0" cellspacing="0">
	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="ac_type">AC TYPE</font>
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="cwmin">CWMIN[0-15]</font>
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="cwmax">CWMAX[0-15]</font>
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="aifs">AIFS[0-15]</font>
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="txop">TXPOLIMIT[0-8192]us</font>
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0"><font id="acm">Admission Control</font>
	</td>
	</tr>

	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">AC_BK
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmin_bk_5g" id="cwmin_bk_5g" value="<% getCfgGeneral(1, "cwmin_bk_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')"  type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_bk_5g" id="cwmax_bk_5g" value="<% getCfgGeneral(1, "cwmax_bk_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_bk_5g" id="aifs_bk_5g" value="<% getCfgGeneral(1, "aifs_bk_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_bk_5g" id="txop_bk_5g"  value="<% getCfgGeneral(1, "txop_bk_5g"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_bk_5g" id="ac_bk_5g" type="checkbox" value="1" />
	</td>	
	</tr>
	
	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">AC_BE
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmin_be_5g" id="cwmin_be_5g" value="<% getCfgGeneral(1, "cwmin_be_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_be_5g" id="cwmax_be_5g" value="<% getCfgGeneral(1, "cwmax_be_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_be_5g" id="aifs_be_5g" value="<% getCfgGeneral(1, "aifs_be_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_be_5g" id="txop_be_5g" value="<% getCfgGeneral(1, "txop_be_5g"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_be_5g" id="ac_be_5g" type="checkbox" value="1" />
	</td>
	</tr>
	
	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">AC_VI
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmin_vi_5g" id="cwmin_vi_5g" value="<% getCfgGeneral(1, "cwmin_vi_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_vi_5g" id="cwmax_vi_5g" value="<% getCfgGeneral(1, "cwmax_vi_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_vi_5g" id="aifs_vi_5g" value="<% getCfgGeneral(1, "aifs_vi_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_vi_5g" id="txop_vi_5g"  value="<% getCfgGeneral(1, "txop_vi_5g"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_vi_5g" id="ac_vi_5g" type="checkbox" value="1" />
	</td>
	</tr>
	
	<tr>
	<td width="10%" align="center" border="1" cellpadding="0" cellspacing="0">AC_VO
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="cwmin_vo_5g" id="cwmin_vo_5g" value="<% getCfgGeneral(1, "cwmin_vo_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="cwmax_vo_5g" id="cwmax_vo_5g"  value="<% getCfgGeneral(1, "cwmax_vo_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="aifs_vo_5g" id="aifs_vo_5g" value="<% getCfgGeneral(1, "aifs_vo_5g"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="txop_vo_5g" id="txop_vo_5g" value="<% getCfgGeneral(1, "txop_vo_5g"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_vo_5g" id="ac_vo_5g" type="checkbox" value="1" />
	</td>
	</tr>
	</table>
</li> 
</ul>
<ul>
  <li></li>
  <li></li>
</ul>
<ul><li></li><li></li><li></li><li></li><li></li></ul>
</div>
</li>
</ul>

<ul><center>
<li class="table_button">
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=button   id="GeneralRestore" onClick="RestoreValue()">
</li></center>
</ul>
</div>

</form>
</body>
<script>
function _alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>
