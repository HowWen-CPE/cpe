<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="style/style.css" rel="stylesheet" type="text/css" />
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>td {white-space: nowrap;}</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="images/common.js"></script>
<script type="text/javascript" src="/local/advance/table.js"></script>
<script type="text/javascript" src="/local/js/validate.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>
<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("firewall");
var wan0_proto= '<% nv_get("wan0_proto"); %>';
var F; 
var max_mtu = 1444;
var min_mtu = 1280;
//wan0_proto=dhcp
var autodns_objects =
[ { name: 'pptpd_dns' } ];
 
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

function CheckMtuValue()
{ 
	var mtu = parseInt(E('byteMTU').value, 10);
	var errorString = "";

	if(document.vpn_pptpd_general.pptpd_mtu.value==""){
		errorString = 'MTU value is empty!';
	}

	if (isDigit(document.vpn_pptpd_general.pptpd_mtu.value) == 0) {
		errorString = 'MTU should be a number.';
	}
	if (mtu > max_mtu || mtu < min_mtu) {
		errorString = 'Invalid MTU! Valid MTU value is [1280,1444].';
	} 

	if (errorString == "") {
		return true;
	} else {
		alertbottom (errorString);
		document.vpn_pptpd_general.pptpd_mtu.value = document.vpn_pptpd_general.pptpd_mtu.defaultValue;
		document.vpn_pptpd_general.pptpd_mtu.focus();
		return false;
	}
}

function CheckVpnClientValue()
{ 
	var err ="";
	var ptn = /\D/;
	var result = ptn.exec(E('pptpd_remoteip_range').value);

	if (result != null) {
		err = 'Invalid VPN Clients range, The clients number must be a digit.';
	}

	var d4 = atoi(E('pptpdRemoteip').value, 4);

	var d7 = parseInt(E('pptpd_remoteip_range').value, 10);
	var d5 = d4 + d7;

	if (32 < d7 || 0 >= d7) {
		err = 'Invalid VPN Clients range, the min clients number is 1, and the max clients number is 32.';
	}

	if (255 < d5) {
		var d6 = 255 - d4;
		err = 'Invalid VPN Clients range.';
	}

	if (err == "") {
		return true;
	} else {
		alertbottom (err);
		document.vpn_pptpd_general.pptpd_remoteip_range.value = document.vpn_pptpd_general.pptpd_remoteip_range.defaultValue;
		document.vpn_pptpd_general.pptpd_remoteip_range.focus();
		return false;
	}
}
function CheckChapValue()
{
	var errorString = "";
	var mppe = (document.getElementsByName("pptpd_mppe128_enable")[0].checked == true) ? 1 : 0;
	if (mppe == 1){
		if (document.getElementsByName("pptpd_mschap_enable")[0].checked == true || 
			document.getElementsByName("pptpd_mschapv2_enable")[0].checked == true)
			return true;
		else {
			errorString = 'Authentication error.In the case that MPPE enable, authentication way must include MSCHAP or MSCHAPV2.';
			alertbottom (errorString);
			return false;
		}
	}
	else if (mppe == 0){
		if(document.getElementsByName("pptpd_mschap_enable")[0].checked == true || 
			document.getElementsByName("pptpd_mschapv2_enable")[0].checked == true ||
			document.getElementsByName("pptpd_chap_enable")[0].checked == true 
			)
			return true;
		else {
			errorString = 'Authentication error.In the case that MPPE disable, you must enable anyone authentication way.';
			alertbottom (errorString);
			return false;
		}
	}
}

function checkIP(E) {

	var IPvalue = E.value;
	var errorString = ""; 
	var thisSegment = "";
	var theName = "IPaddress";

	var ipPattern=/^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;

	var ipArray = IPvalue.match(ipPattern);

	//alert(IPvalue);

	if (IPvalue == "0.0.0.0")
		errorString = errorString + theName + ': '+IPvalue+' is a special IP address and cannot be used here.';
	else if (IPvalue == "255.255.255.255")
		errorString = errorString + theName + ': '+IPvalue+' is a special IP address and cannot be used here.';
	else if (ipArray == null)
		errorString = errorString + theName + ': '+IPvalue+' is not a valid IP address.';
	else {
		for (i = 0; i < 4; i++) {
			thisSegment = ipArray[i];
			if (thisSegment > 255) {
				errorString = errorString + theName + ': '+IPvalue+' is not a valid IP address.';
				i = 4;
			}
			if ((i == 0) && (thisSegment > 255)) {
				errorString = errorString + theName + ': '+IPvalue+' is a special IP address and cannot be used here.';
				i = 4;
			}
		}
	}

	var patrn=/^(127)\.(0{1,3})\.(0{1,3})\.(1)$/;
	if (patrn.exec(IPvalue)) {
		errorString = errorString + theName + ': '+IPvalue+' is a special IP address and cannot be used here.';
	}

	if (errorString == "") {
		return true;
	} else {
		alertbottom (errorString);
		E.value = E.defaultValue;
		E.focus();
		return false;
	}
}

function formCheck()
{
	if (CheckMtuValue() == false)
		return false;

	//if (checkIP(E('pptpd_remoteip')) == false) {
		//return false;
	//}
	if (!checkIpAddr(E('pptpdRemoteip'), false))
		return false;
	if (!checkIpAddr(E('pptpd_localip'), false))
		return false;
	if (CheckVpnClientValue() == false)
		return false;
	if (CheckChapValue() == false)
		return false;

	var checkDns = (document.getElementsByName("pptpd_dns_auto_enable")[0].checked == true) ? 1 : 0;
    	if (0 == checkDns){
    		if (!checkIpAddr(E('pptpd_dns'), false))
        		return false;
    	}

	showWebMessage(2, "");
	return true;
}
function initTranslation(){
	var e = document.getElementById("applicationAddApp");
	e.innerHTML = _("application AddApp");
	e = document.getElementById("pptpServerRule");
	e.innerHTML = _("pptp Server Rule");
	e = document.getElementById("pptpMTU");
	e.innerHTML = _("pptp MTU");
	e = document.getElementById("pptpLocalIP");
	e.innerHTML = _("pptp LocalIP");
	e = document.getElementById("startIP");
	e.innerHTML = _("start IP");
	e = document.getElementById("maxNum");
	e.innerHTML = _("max Num");
	
	e = document.getElementById("autoDns");
	e.innerHTML = _("auto Dns");
	e = document.getElementById("pptpDns");
	e.innerHTML = _("pptp Dns");
	e = document.getElementById("pptpChap");
	e.innerHTML = _("chap Enable");
	e = document.getElementById("pptpMscharp");
	e.innerHTML = _("mscharp Enable");
	e = document.getElementById("mscharpV2");
	e.innerHTML = _("mscharp V2");
	e = document.getElementById("mppe128");
	e.innerHTML = _("mppe 128");
	//e = document.getElementById("proxyARP");
	//e.innerHTML = _("proxy ARP");
	e = document.getElementById("pptpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("dnsEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("charpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("mscharpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("mscharpv2Enable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("mppeEnable");
	e.innerHTML = _("pptp Enable");
	//e = document.getElementById("arpEnable");
	//e.innerHTML = _("pptp Enable");
	
	e = document.getElementById("pptpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("dnsDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("charpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("mscharpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("mscharpv2Disable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("mppeDisable");
	e.innerHTML = _("pptp Disable");
	//e = document.getElementById("arpDisable");
	//e.innerHTML = _("pptp Disable");
	e = document.getElementById("VPNApply");
	e.value = _("VPN Apply");
	e = document.getElementById("VPNReset");
	e.value = _("VPN Reset");
			
} 
function init() {
	initTranslation();
   	 F = document.getElementById("vpn_pptpd_general");

	if (wan0_proto == "pptp" || wan0_proto == "l2tp")
	{
		document.getElementsByName("pptpd_enable")[0].disabled = "ture";
		document.getElementsByName("pptpd_enable")[1].disabled = "ture";
	}

    var autodns_enable = (document.getElementsByName("pptpd_dns_auto_enable")[0].checked == true) ? 1 : 0;
    sel_change(autodns_enable, 0, F, autodns_objects);

}
</script>
</head>
  <body onload="init();">

  <form id="vpn_pptpd_general" name="vpn_pptpd_general" action="/goform/vpn_pptpd_general" method="post">

  <div id="table">
    <ul>
      <li class="table_content">
        <div class="data">
          <ul>
	          <script type="text/javascript">do_hidden_elements('pptpd_rule')</script>	
              <input type="hidden" name="pptpd_name" 
                    value='<%nvg_attr_get("pptpd_name", "pptpd_rule","0","name");%>' />
              <input type="hidden" name="pptpd_redialperiod" 
                    value='<%nvg_attr_get("pptpd_redialperiod", "pptpd_rule","0","redialperiod");%>' />
              <input type="hidden" name="pptpd_eap_enable" 
                    value='<%nvg_attr_get("pptpd_eap_enable", "pptpd_rule","0","eap_enable");%>' />
              <input type="hidden" name="pptpd_pap_enable" 
                    value='<%nvg_attr_get("pptpd_pap_enable", "pptpd_rule","0","pap_enable");%>' />
              <input type="hidden" name="pptpd_nat_enable" 
                    value='<%nvg_attr_get("pptpd_nat_enable", "pptpd_rule","0","nat_enable");%>' />
              <input type="hidden" name="pptpd_proxyarp_enable" 
                    value='<%nvg_attr_get("pptpd_proxyarp_enable", "pptpd_rule","0","proxyarp_enable");%>' />

              <% tbl_get("pptpd_user_rule"); %>

              <fieldset>
			  <li class="title" >

			  <table border="0" cellpadding="0" cellspacing="0">
			  <tr>
			  <td nowrap id="applicationAddApp">PPTP Server</td>
			  </tr>
			  </table>

              </li>

              <span class='on'>
                <li class="w_text">
                <td>
                  <input type="radio" value="1"  name="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","1","checked");%>>
                        <font id="pptpEnable">Enable</font>
                  </td>
                  <td>
                  <input type="radio" value="0"  name="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","0","checked");%>>
                  <font id="pptpDisable">Disable</font>
                  </td>

		</li>
              </span>

              <li class="space3">
              </li>

			  <li class="title" id="pptpServerRule">PPTP Server Rule </li> 
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="pptpMTU">MTU: </td>
						<td><input id="byteMTU" name="pptpd_mtu" size="4" maxLength="4" value='<%nvg_attr_get("pptpd_mtu", "pptpd_rule", "0", "mtu");%>'/> 
						</td>
						</tr>
					</table>
				</li>
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="pptpLocalIP">VPN Local IP Address: </td>
						<td><input id="pptpd_localip" name="pptpd_localip" size="20" maxLength="15" value='<%nvg_attr_get("pptpd_localip", "pptpd_rule", "0", "localip");%>'/> 
						
						</td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="startIP">VPN Start IP Address </td>
						<td><input id="pptpdRemoteip" name="pptpd_remoteip" size="20" maxLength="15" value='<%nvg_attr_get("pptpd_remoteip", "pptpd_rule", "0", "remoteip");%>'  /></td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="maxNum">Max VPN Clients </td>
						<td><input size="3" maxLength="3" name="pptpd_remoteip_range" id="pptpd_remoteip_range"
                          value='<%nvg_attr_get("pptpd_remoteip_range", "pptpd_rule", "$0", "remoteip_range");%>'/>
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="autoDns">Auto DNS </td>
						<td><input type="radio" value="1" name="pptpd_dns_auto_enable" onclick="init()"
						<%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","1","checked");%>>
						<font id="dnsEnable">Enable</font>
						
						</td>
						<td>
						<input type="radio" value="0" name="pptpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","0","checked");%>>
                       			 <font id="dnsDisable">Disable</font>
						
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="pptpDns">DNS</td>
						<td><input type="text" maxLength="15" size="20" name="pptpd_dns" id="pptpd_dns"
						value='<%nvg_attr_get("pptpd_dns", "pptpd_rule", "$0", "dns");%>'/>
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="pptpChap">CHAP Enable</td>
                  <td><input type="radio" value="1" name="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","1","checked");%>>
                        <font id="charpEnable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0" name="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","0","checked");%>>
                        <font id="charpDisable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="pptpMscharp">MSCHAP Enable </td>
                  <td><input type="radio" value="1" name="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","1","checked");%>>
                        <font id="mscharpEnable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0" name="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","0","checked");%>>
                        <font id="mscharpDisable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="mscharpV2">MSCHAP v2 Enable </td>
                  <td><input type="radio" value="1" name="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","1","checked");%>>
                       <font id="mscharpv2Enable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0" name="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","0","checked");%>>
                        <font id="mscharpv2Disable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="mppe128">MPPE128 Enable</td>
                  <td><input type="radio" value="1" name="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","1","checked");%>>
                        <font id="mppeEnable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0" name="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","0","checked");%>>
                        <font id="mppeDisable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>
				<!--
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="proxyARP">Proxy ARP Enable</td>
                  <td><input type="radio" value="1" name="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","1","checked");%>>
                        <font id="arpEnable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0"  name="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","0","checked");%>>
                        <font id="arpDisable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>
				-->
                <!--
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="natEnable">NAT Enable </td>
                  <td><input type="radio" value="1" name="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","1","checked");%>>
                    <font id="natActEnable">Enable</font>
                  
				  </td>
				  <td>
                  <input type="radio" value="0" name="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","0","checked");%>>
                        <font id="natActDisable">Disable</font>
                  
						  </td>
						</tr>
					</table>
				</li>
				-->
              </fieldset><br />

            </ul>
          </div>
          </li>
    <center>
      <li class="table_button">
		<input type="submit" value="Apply" id="VPNApply" name="apply" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" value="Cancel" id="VPNReset" name="reset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/vpn_pptpd_general.asp" name="pptpd_gen_url">
      </li>
    </center>
	</ul>
	</div>
  </form>

</body>
<script>
function alertbottom(str)
{
  showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>

