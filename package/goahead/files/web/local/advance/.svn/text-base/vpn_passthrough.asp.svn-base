<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
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

function formCheck()
{
	showWebMessage(2, "");
	return true;
}

function initTranslation(){
	var e = document.getElementById("vpnPassSetup");
	e.innerHTML = _("vpn Pass Setup");

	e = document.getElementById("ipsecEnable");
	e.innerHTML = _("ipsec Enable");
	e = document.getElementById("ipsecDisable");
	e.innerHTML = _("ipsec Disable");
	e = document.getElementById("ipsecSet");
	e.innerHTML = _("ipsec Set");
	
	e = document.getElementById("pptpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("pptpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("pptpSet");
	e.innerHTML = _("pptp Set");
	
	e = document.getElementById("l2tpEnable");
	e.innerHTML = _("l2tp Enable");
	e = document.getElementById("l2tpDisable");
	e.innerHTML = _("l2tp Disable");
	e = document.getElementById("l2tpSet");
	e.innerHTML = _("l2tp Set");
	
	e = document.getElementById("pppoeEnable");
	e.innerHTML = _("pppoe Enable");
	e = document.getElementById("pppoeDisable");
	e.innerHTML = _("pppoe Disable");
	e = document.getElementById("pppoeSet");
	e.innerHTML = _("pppoe Set");

	e = document.getElementById("VPNApply");
	e.value = _("VPN Apply");
	
	e = document.getElementById("VPNReset");
	e.value = _("VPN Reset");

} 

</script>
</head>
<body onload="initTranslation();">
<form method="post" name="vpn_passthrough" action="/goform/vpn_passthrough" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="vpnPassSetup" style="white-space:nowrap" >VPN Passthrough Setup</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					<!--
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("IPsec Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="ipsec_enable" onclick="init()"
                        <%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="ipsec_enable" onclick="init()"
						<%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
					-->
					<tr>
					<td width="10%" id="ipsecSet" style="white-space:nowrap" >IPsec Passthrough</td>
					<td width="3%" >
					<input type="radio" value="0" name="ipsec_enable" <%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","0", "checked");%>>
					</td>
					<td width="5%" >
					<font id="ipsecEnable">Enable</font>
					</td>
					<td width="3%">
					<input type="radio" value="1" name="ipsec_enable" <%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","1", "checked");%>>
					</td>
					<td width="5%">
					<font id="ipsecDisable">Disable</font>
					</td>
					<td width="77%"></td>
					</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<!--
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("PPTP Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="pptp_enable" onclick="init()"
                        <%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="pptp_enable" onclick="init()"
						<%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
						-->
					<tr>
					<td width="10%" id="pptpSet" style="white-space:nowrap" >PPTP Passthrough</td>
					<td width="3%" >
					<input type="radio" value="0" name="pptp_enable" <%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","0", "checked");%>>
					</td>
					<td width="5%" >
					<font id="pptpEnable">Enable</font>
					</td>
					<td width="3%" >
					<input type="radio" value="1" name="pptp_enable" <%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","1", "checked");%>>
					</td>
					<td width="5%">
					<font id="pptpDisable">Disable</font>
					</td>
					<td width="77%"></td>
					</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<!--
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("L2TP Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="l2tp_enable" onclick="init()"
                        <%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="l2tp_enable" onclick="init()"
						<%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
						-->
					<tr>
					<td width="10%" id="l2tpSet" style="white-space:nowrap" >L2TP Passthrough</td>
					<td width="3%" >
					<input type="radio" value="0" name="l2tp_enable" <%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","0", "checked");%>>
					</td>
					<td width="5%" >
					<font id="l2tpEnable">Enable</font>
					</td>
					<td width="3%" >
					<input type="radio" value="1" name="l2tp_enable" <%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","1", "checked");%>>
					</td>
					<td width="5%" >
					<font id="l2tpDisable">Disable</font>
					</td>
					<td width="77%"></td>
					</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					<tr>
					<td width="10%" id="pppoeSet" style="white-space:nowrap" >PPPOE Passthrough</td>
					<td width="3%" >
					<input type="radio" value="0" name="pppoe_enable" <%nvg_attr_match("pppoe_enable", "smb_vpn_passthrough_rule","0","pppoe_enable","0", "checked");%>>
					</td>
					<td width="5%" >
					<font id="pppoeEnable">Enable</font>
					</td>
					<td width="3%" >
					<input type="radio" value="1" name="pppoe_enable" <%nvg_attr_match("pppoe_enable", "smb_vpn_passthrough_rule","0","pppoe_enable","1", "checked");%>>
					</td>
					<td width="5%" >
					<font id="pppoeDisable">Disable</font>
					</td>
					<td width="77%"></td>
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
<input type="submit" value="Apply" id="VPNApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="VPNReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/vpn_passthrough.asp" name="vpn_pass_url">
</li>
</center>
</ul>
</div>
</form>
<!--
<form method="post" name="ftp_passthrough" action="/goform/ftp_passthrough" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="FtpSetup" style="white-space:nowrap" >FTP Passthrough Setup</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("FTP Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="ftp_enable" onclick="init()" 
                                          <%nvg_attr_match("ftp_enable", "smb_ftp_passthrough_rule","0","ftp_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="ftp_enable" onclick="init()" 
						<%nvg_attr_match("ftp_enable", "smb_ftp_passthrough_rule","0","ftp_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
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
<input type="submit" value="Apply" id="ApplyFtp" name="" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="ResetFtp" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/vpn_passthrough.asp" name="ftp_pass_url">
</li>
</center>
</ul>
</div>
</form>
<form method="post" name="h323_passthrough" action="/goform/h323_passthrough" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="H323Setup" style="white-space:nowrap" >H.323 Passthrough Setup</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("H.323 Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="h323_enable" onclick="init()" 
                                          <%nvg_attr_match("h323_enable", "smb_h323_passthrough_rule","0","h323_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="h323_enable" onclick="init()" 
						<%nvg_attr_match("h323_enable", "smb_h323_passthrough_rule","0","h323_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
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
<input type="submit" value="Apply" id="ApplyH323" name="" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="ResetH323" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/vpn_passthrough.asp" name="h323_pass_url">
</li>
</center>
</ul>
</div>
</form>
-->
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
