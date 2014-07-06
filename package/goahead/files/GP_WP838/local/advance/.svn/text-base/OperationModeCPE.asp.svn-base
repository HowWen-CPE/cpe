<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
    <link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
.i_note_a {
	color: #000000;
	display: block;
	font-size: 11px;
	font-weight: bold;
	line-height: 16px;
	text-indent: 25px;
}
</style>    
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var board_type = "<% getCfgZero(1, "board_model"); %>";
var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;
function show_div(show,id)
{
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
	}

function initTranslation()
{
	var e = document.getElementById("OPModeTitle");
	e.innerHTML = _("op mode title");

	e = document.getElementById("OPModeRouter");
	e.innerHTML = _("op mode router");

	e = document.getElementById("OPModeAP");
	e.innerHTML = _("op mode ap");
	
	e = document.getElementById("OPModeUR");
	e.innerHTML = _("op mode ur");
	
	e = document.getElementById("OPModeWISP");
	e.innerHTML = _("op mode wisp");

	e = document.getElementById("OPModeSTA");
	e.innerHTML = _("op mode sta0");

	e = document.getElementById("OPModeNote1_0");
	e.innerHTML = _("op mode note1_0");
	
	e = document.getElementById("OPModeNote1_1");
	e.innerHTML = _("op mode note1_1");
	
	e = document.getElementById("OPModeNote1_2");
	e.innerHTML = _("op mode note1_2");

	e = document.getElementById("OPModeNote1_3");
	e.innerHTML = _("op mode note1_3");
	e = document.getElementById("OPModeNote1_8");
	e.innerHTML = _("op mode note1_8");

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	initTranslation();
 
 	//router
	if (nv_OP_Mode == 0) {
		document.CfgOperationMode.OPMode[0].checked = true;
	}
	else if (nv_OP_Mode == 1) {
		document.CfgOperationMode.OPMode[1].checked = true;
	}
	else if (nv_OP_Mode == 2) {
		document.CfgOperationMode.OPMode[2].checked = true;
	}
	else if (nv_OP_Mode == 3) {
		document.CfgOperationMode.OPMode[3].checked = true;
	}
	else if (nv_OP_Mode == 4) {
		document.CfgOperationMode.OPMode[4].checked = true;
	}
	else if (nv_OP_Mode == 5) {
		document.CfgOperationMode.OPMode[5].checked = true;
	}
	//wisp0
	else if (nv_OP_Mode == 6) {
		document.CfgOperationMode.OPMode[6].checked = true;
	}
	//wisp1
	else if (nv_OP_Mode == 7) {
		document.CfgOperationMode.OPMode[7].checked = true;
	}
	//sta0
	else if (nv_OP_Mode == 8) {
		document.CfgOperationMode.OPMode[8].checked = true;
	}
	//sta1
	else if (nv_OP_Mode == 9) {
		document.CfgOperationMode.OPMode[9].checked = true;
	}

	show_div(false, "div_hidden_some_op_modes");
	show_div(false, "div_hidden_some_op_modes2");
	parent.adjustMyFrameHeight();
}


var lanip_router = "<% getCfgGeneral(1, "lan_ipaddr_router"); %>";
var lanip_ap = "<% getCfgGeneral(1, "lan_ipaddr_ap"); %>";
function changmod_msg_Router()
{
alert(_("- LAN DHCP server is configurable")+"\n"+_("- LAN IP is")+lanip_router+"\n"+_("- WAN IP is configurable"));      
}
function changmod_msg_WISP()
{
alert(_("- LAN DHCP server is configurable")+"\n"+_("- LAN IP is")+lanip_router+"\n"+_("- WAN IP is configurable"));      
}
function changmod_msg_WISP_UR()
{
alert(_("- LAN DHCP server is configurable")+"\n"+_("- LAN IP is")+lanip_router+"\n"+_("- WAN IP is configurable"));      
}
function changmod_msg_AP()
{
alert(_("- LAN DHCP server is disabled & unconfigurable")+"\n"+_("- LAN IPb is")+lanip_ap+"\n"+_("- WAN IP is unreachable"));      
}
function changmod_msg_STA()
{
alert(_("- LAN DHCP server is disabled & unconfigurable")+"\n"+_("- LAN IPb is")+lanip_ap+"\n"+_("- WAN IP is unreachable"));      
}
function changmod_msg_bridge()
{
alert(_("- LAN DHCP server is disabled & unconfigurable")+"\n"+_("- LAN IPb is")+lanip_ap+"\n"+_("- WAN IP is unreachable"));
}

function HasChanged()
{
	if(document.CfgOperationMode.OPMode[nv_OP_Mode].checked == true) {
		return false;
	}
	return true;
}
</script>
</head>
<body onLoad="initValue()">
<form method=post name="CfgOperationMode" action="/goform/SetOperationMode" onSubmit="return HasChanged()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                 <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td id="OPModeTitle" nowrap>Configuration Mode</td>
                   </tr>
                  </table>
                 </li>


<li class="w_text" style="display:none">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="0" onClick="changmod_msg_Router()"/>
	<b><font id ="OPModeRouter">Router Mode</font></b>
</td>
</tr>
</table>
</li>

<!-- </span> hidden some OP Modes -->

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="1" onClick="changmod_msg_AP()"/>
	<b><font id ="OPModeAP">Access Point Mode</font></b>
</td>
</tr>
</table>
</li>

<!-- hidden some OP Modes -->
<span id="div_hidden_some_op_modes" class="on"> 
<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="2" />Client Mode</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="3" />Bridge Mode (WDS)</td>
</tr>
</table>
</li>

<li class="w_text" style="display:none">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="4" />AP + Repeater Mode (AP +WDS)</td>
</tr>
</table>
</li>

<li class="w_text" style="display:none">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td width="25%" nowrap><input type="radio" name="OPMode" value="5" onClick="changmod_msg_bridge()"/><font id ="OPModeUR">Workgroup Bridge Mode</font></td>
</tr>
</table>
</li>
</span>
<!-- hidden some OP Modes -->

<!-- Wireless Bridge Mode -->
<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="8" onClick="changmod_msg_STA()" />
	<b><font id ="OPModeSTA">Wireless Client Mode</font></b>
</td>
</tr>
</table>
</li>

<!-- open WISP0 Modes -->
<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="6" onClick="changmod_msg_WISP()" />
	<b><font id ="OPModeWISP">WISP Mode</font></b>
</td>
</tr>
</table>
</li>

<!-- open WISP1 Modes -->
<li class="w_text" style="display:none">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="7" onClick="changmod_msg_WISP()" />
	<b><font id ="OPModeWISP">WISP1 Mode</font></b>
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td width="25%" nowrap>
</td>
</tr>
</table>
</li>

<!-- hidden some OP Modes -->
<span id="div_hidden_some_op_modes2" class="on"> 
</span>
<!-- hidden some OP Modes -->

<li class="w_text">
<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="OPModeNote1_0">Note:</span></td>
</tr>
</table>
</li>


<li class="w_text2">
<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">

<tr style="display:none;">
  <td class="i_note_a" id="OPModeNote1_1">
    Router: The device is supported to connect to internet via ADSL/Cable Modem. PCs in LAN ports share the same IP to ISP through WAN Port.
  </td>
</tr>
<tr><td>&nbsp;</td></tr>

<tr>
  <td class="i_note_a" id="OPModeNote1_2">
    Access Point: The device allows the wireless-equipped computer can communicate with a wired network.
  </td>
</tr>
<tr><td>&nbsp;</td></tr>

<tr>
  <td class="i_note_a" id="OPModeNote1_8">
    Wireless Client Mode: The wireless device connects to an existing network via an access point as the wirless client, and be in the same LAN bridge as the ethernet port.
  </td>
</tr>
<tr><td>&nbsp;</td></tr>

<tr>
  <td class="i_note_a" id="OPModeNote1_3">
    WISP Mode: The device acts as a wireless client. It can connect to an existing network via an access point. Also router functions are added between the wireless WAN and the LAN.
  </td>
</tr>
<tr><td>&nbsp;</td></tr>
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
		<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">        
		</li></center>
	</ul>
    </div>
</form>        
</body>
</html>
