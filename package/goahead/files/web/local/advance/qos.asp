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

var wirelessmode  = <% getCfgGeneral(1, "wireless_mode"); %>;
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
	initTranslation();

	var WMMEnable  = "<% getCfgGeneral(1, "wmm_enable"); %>";
	var NoAck = "<% getCfgGeneral(1, "ack_enable"); %>";
	var AC_BK = "<% getCfgGeneral(1, "ac_bk"); %>";
	var AC_BE = "<% getCfgGeneral(1, "ac_be"); %>";
	var AC_VI = "<% getCfgGeneral(1, "ac_vi"); %>";
	var AC_VO = "<% getCfgGeneral(1, "ac_vo"); %>";
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
		document.wireless_advanced.wmm_enable[0].checked = true;
	}
	else
	{
		document.wireless_advanced.wmm_enable[1].checked = true;
	}
	wmmcheck();
	if (NoAck == 1)
	{
		document.wireless_advanced.ack_enable.checked = true;
	}
	else
	{
		document.wireless_advanced.ack_enable.checked = false;
	}
	if (AC_BK == 1)
	{
		document.wireless_advanced.ac_bk.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_bk.checked = false;
	}

	if (AC_BE == 1)
	{
		document.wireless_advanced.ac_be.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_be.checked = false;
	}	
	if (AC_VI == 1)
	{
		document.wireless_advanced.ac_vi.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_vi.checked = false;
	}

	if (AC_VO == 1)
	{
		document.wireless_advanced.ac_vo.checked = true;
	}
	else
	{
		document.wireless_advanced.ac_vo.checked = false;
	}
}

function wmmcheck()
{
	var form=document.wireless_advanced;
	if(form.wmm_enable[0].checked == true) 
	{
		form.ack_enable.disabled=false;
		form.cwmin_bk.disabled=false;
		form.cwmin_be.disabled=false;
		form.cwmin_vi.disabled=false;
		form.cwmin_vo.disabled=false;
		form.cwmax_bk.disabled=false;
		form.cwmax_be.disabled=false;
		form.cwmax_vi.disabled=false;
		form.cwmax_vo.disabled=false;
		form.aifs_bk.disabled=false;
		form.aifs_be.disabled=false;
		form.aifs_vi.disabled=false;
		form.aifs_vo.disabled=false;
		form.txop_bk.disabled=false;
		form.txop_be.disabled=false;
		form.txop_vi.disabled=false;
		form.txop_vo.disabled=false;
		form.ac_bk.disabled=false;
		form.ac_be.disabled=false;			
		form.ac_vi.disabled=false;
		form.ac_vo.disabled=false;		
	}
	else 
	{
		form.ack_enable.disabled=true;
		form.cwmin_bk.disabled=true;
		form.cwmin_be.disabled=true;
		form.cwmin_vi.disabled=true;
		form.cwmin_vo.disabled=true;
		form.cwmax_bk.disabled=true;
		form.cwmax_be.disabled=true;
		form.cwmax_vi.disabled=true;
		form.cwmax_vo.disabled=true;
		form.aifs_bk.disabled=true;
		form.aifs_be.disabled=true;
		form.aifs_vi.disabled=true;
		form.aifs_vo.disabled=true;
		form.txop_bk.disabled=true;
		form.txop_be.disabled=true;
		form.txop_vi.disabled=true;
		form.txop_vo.disabled=true;
		form.ac_bk.disabled=true;
		form.ac_be.disabled=true;			
		form.ac_vi.disabled=true;
		form.ac_vo.disabled=true;		
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
	form.wmm_enable[0].checked == true;
	form.ack_enable.checked = false;
	form.ac_bk.checked = false;
	form.ac_be.checked = false;
	form.ac_vi.checked = false;
	form.ac_vo.checked = false;

	form.cwmin_bk.value="4";
	form.cwmax_bk.value="6";
	form.cwmin_be.value="4";
	form.cwmax_be.value="10";
	form.cwmin_vi.value="3";
	form.cwmax_vi.value="4";
	form.cwmin_vo.value="2";
	form.cwmax_vo.value="3";
	form.aifs_bk.value="3";
	form.txop_bk.value="0";
	form.aifs_be.value="7";
	form.txop_be.value="0";
	form.aifs_vi.value="1";
	form.txop_vi.value="3008";
	form.aifs_vo.value="1";
	form.txop_vo.value="1504";
	return false;
}

function rangeCheck()
{
	var form=document.wireless_advanced;
	if(!check_len(form.cwmin_bk,1,2) || !check_len(form.cwmin_be,1,2) 
	|| !check_len(form.cwmin_vi,1,2) || !check_len(form.cwmin_vo,1,2)
	|| !check_len(form.cwmax_bk,1,2) || !check_len(form.cwmax_be,1,2)
	|| !check_len(form.cwmax_vi,1,2) || !check_len(form.cwmax_vo,1,2) 
	|| !check_len(form.aifs_bk,1,2) || !check_len(form.aifs_be,1,2)
	|| !check_len(form.aifs_vi,1,2) || !check_len(form.aifs_vo,1,2))
	{
		return false;
	}
	if(!check_len(form.txop_bk,1,4) || !check_len(form.txop_be,1,4) 
	|| !check_len(form.txop_vi,1,4) || !check_len(form.txop_vo,1,4))
	{
		return false;
	}
	if(parseInt(form.cwmin_bk.value) >  parseInt(form.cwmax_bk.value))
	{
		_alert("ac_bk's cwmin");
		form.cwmin_bk.value = form.cwmin_bk.defaultValue;
		form.cwmax_bk.value = form.cwmax_bk.defaultValue;
		form.cwmin_bk.focus();
		return false;
	}
	if(parseInt(form.cwmin_be.value) >  parseInt(form.cwmax_be.value))
	{
		_alert("ac_be's cwmin");
		form.cwmin_be.value = form.cwmin_be.defaultValue;
		form.cwmax_be.value = form.cwmax_be.defaultValue;
		form.cwmax_be.focus();		
		return false;
	}
	if(parseInt(form.cwmin_vi.value) > parseInt(form.cwmax_vi.value))
	{
		_alert("ac_vi's cwmin");
		form.cwmin_vi.value = form.cwmin_vi.defaultValue;
		form.cwmax_vi.value = form.cwmax_vi.defaultValue;
		form.cwmax_vi.focus();		
		return false;
	}
	if(parseInt(form.cwmin_vo.value) > parseInt(form.cwmax_vo.value))
	{
		_alert("ac_vo's cwmin");
		form.cwmin_vo.value = form.cwmin_vo.defaultValue;
		form.cwmax_vo.value = form.cwmax_vo.defaultValue;
		form.cwmax_vo.focus();		
		return false;
	}
	if(parseInt(form.cwmin_bk.value)>15 || parseInt(form.cwmax_bk.value)>15||
	parseInt(form.cwmin_be.value)>15||parseInt(form.cwmax_be.value)>15||
	parseInt(form.cwmin_vi.value)>15||parseInt(form.cwmax_vi.value)>15||
	parseInt(form.cwmin_vo.value)>15||parseInt(form.cwmax_vo.value)>15||
	parseInt(form.aifs_bk.value)>15|| parseInt(form.aifs_be.value)>15||
	parseInt(form.aifs_vi.value)>15||parseInt(form.aifs_vo.value)>15)
	{
		form.cwmin_bk.value = form.cwmin_bk.defaultValue;
		form.cwmax_bk.value = form.cwmax_bk.defaultValue;
		form.cwmin_be.value = form.cwmin_be.defaultValue;
		form.cwmax_be.value = form.cwmax_be.defaultValue;
		form.cwmin_vi.value = form.cwmin_vi.defaultValue;
		form.cwmax_vi.value = form.cwmax_vi.defaultValue;
		form.cwmin_vo.value = form.cwmin_vo.defaultValue;
		form.cwmax_vo.value = form.cwmax_vo.defaultValue;
		form.aifs_bk.value = form.aifs_bk.defaultValue;
		form.aifs_be.value = form.aifs_be.defaultValue;
		form.aifs_vi.value = form.aifs_vi.defaultValue;
		form.aifs_vo.value = form.aifs_vo.defaultValue;								
		_alert("wmm range limit");
		return false;
	}
	if(parseInt(form.txop_bk.value)>8192 || parseInt(form.txop_be.value)>8192||
	parseInt(form.txop_vi.value)>8192||parseInt(form.txop_vo.value)>8192)
	{
		form.txop_bk.value = form.txop_bk.defaultValue;
		form.txop_be.value = form.txop_be.defaultValue;
		form.txop_vi.value = form.txop_vi.defaultValue;
		form.txop_vo.value = form.txop_vo.defaultValue;		
		form.txop_vo.focus();	
		_alert("txop limit");
		return false;
	}
	return true;
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/WMM_QoS" onSubmit="return submit_apply()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="WMMTitle">2.4G WMM Configuration </li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="20%" id="EnableWMM">Enable WMM : </td>
<td><input name="wmm_enable" onclick="wmmcheck()" value="1" type="radio" /></td>
<td id="wmmenable">Enable</td>
<td><input name="wmm_enable" onclick="wmmcheck()" value="0" type="radio" /></td>
<td id="wmmdisable">Disable</td>
<td width="60%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="ack_enable" type="checkbox" value="1" />
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
	<input name="cwmin_bk" id="cwmin_bk" value="<% getCfgGeneral(1, "cwmin_bk"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')"  type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_bk" id="cwmax_bk" value="<% getCfgGeneral(1, "cwmax_bk"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_bk" id="aifs_bk" value="<% getCfgGeneral(1, "aifs_bk"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_bk" id="txop_bk"  value="<% getCfgGeneral(1, "txop_bk"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_bk" id="ac_bk" type="checkbox" value="1" />
	</td>	
	</tr>
	
	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">AC_BE
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmin_be" id="cwmin_be" value="<% getCfgGeneral(1, "cwmin_be"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_be" id="cwmax_be" value="<% getCfgGeneral(1, "cwmax_be"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_be" id="aifs_be" value="<% getCfgGeneral(1, "aifs_be"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_be" id="txop_be" value="<% getCfgGeneral(1, "txop_be"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_be" id="ac_be" type="checkbox" value="1" />
	</td>
	</tr>
	
	<tr>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">AC_VI
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmin_vi" id="cwmin_vi" value="<% getCfgGeneral(1, "cwmin_vi"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="cwmax_vi" id="cwmax_vi" value="<% getCfgGeneral(1, "cwmax_vi"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="aifs_vi" id="aifs_vi" value="<% getCfgGeneral(1, "aifs_vi"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="txop_vi" id="txop_vi"  value="<% getCfgGeneral(1, "txop_vi"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_vi" id="ac_vi" type="checkbox" value="1" />
	</td>
	</tr>
	
	<tr>
	<td width="10%" align="center" border="1" cellpadding="0" cellspacing="0">AC_VO
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="cwmin_vo" id="cwmin_vo" value="<% getCfgGeneral(1, "cwmin_vo"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="cwmax_vo" id="cwmax_vo"  value="<% getCfgGeneral(1, "cwmax_vo"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="aifs_vo" id="aifs_vo" value="<% getCfgGeneral(1, "aifs_vo"); %>" maxlength="2" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="20%" align="center" border="1" cellpadding="0" cellspacing="0">
	<input name="txop_vo" id="txop_vo" value="<% getCfgGeneral(1, "txop_vo"); %>" maxlength="4" onkeyup="value=value.replace(/[^\d]/g,'')" type="text" />
	</td>
	<td width="10%" align="center" border="0" cellpadding="0" cellspacing="0">
	<input name="ac_vo" id="ac_vo" type="checkbox" value="1" />
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
