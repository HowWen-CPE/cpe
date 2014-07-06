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
var pptp_status = '<% getCfgZero(1, "pptp_status"); %>';
var ipsec_status = '<% getCfgZero(1, "ipsec_status"); %>';
var F; 
var max_mtu = 1460;
var min_mtu = 1280;
var autodns_objects =
[ { name: 'l2tpd_dns' } ];

var psk_objects =
[ { name: 'l2tpd_psk' } ];
 
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

	if(document.vpn_l2tpd_general.l2tpd_mtu.value==""){
		errorString = 'MTU value is empty!';
	}

	if (isDigit(document.vpn_l2tpd_general.l2tpd_mtu.value) == 0) {
		errorString = 'MTU should be a number.';
	}
	if (mtu > max_mtu || mtu < min_mtu) {
		errorString = 'Invalid MTU! Valid MTU value is [1280,1460].';
	} 

	if (errorString == "") {
		return true;
	} else {
		alertbottom (errorString);
		document.vpn_l2tpd_general.l2tpd_mtu.value = document.vpn_l2tpd_general.l2tpd_mtu.defaultValue;
		document.vpn_l2tpd_general.l2tpd_mtu.focus();
		return false;
	}
}

//^[A-Za-z0-9]+$
//Enter letters or digitals. The min length of preshared key is 8, and the max length is 40.
function isPassword(s)
{
	var patrn=/^[A-Za-z0-9_]+$/;
	if (!patrn.exec(s)) 
		return false
	else
		return true
}


function CheckVpnClientValue()
{ 
	var err ="";
	var ptn = /\D/;
	var result = ptn.exec(E('l2tpd_remoteip_range').value);

	if(document.vpn_l2tpd_general.l2tpd_remoteip_range.value==""){
		alertbottom ("Max VPN Client number should not be empty.");
		return false;
	}
	
	if (result != null) {
		err = 'Invalid VPN Clients range, The clients number must be a digit.';
	}

	var d4 = atoi(E('l2tpRemoteIP').value, 4);

	var d7 = parseInt(E('l2tpd_remoteip_range').value, 10);
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
		document.vpn_l2tpd_general.l2tpd_remoteip_range.value = document.vpn_l2tpd_general.l2tpd_remoteip_range.defaultValue;
		document.vpn_l2tpd_general.l2tpd_remoteip_range.focus();
		return false;
	}
}

function formCheck()
{

	if (pptp_status == 1){
		alertbottom('Please disable PPTP Server function.');
		return false;
	}
	if (CheckMtuValue() == false)
		return false;
	if (!checkIpAddr(E('l2tpRemoteIP'), false))
		return false;
	if (!checkIpAddr(E('l2tpd_localip'), false))
		return false;
	if (CheckVpnClientValue() == false)
		return false;

	var checkDns = (document.getElementsByName("l2tpd_dns_auto_enable")[0].checked == true) ? 1 : 0;
   	if (0 == checkDns){
   		if (!checkIpAddr(E('l2tpd_dns'), false))
    		return false;
   	}
    
    var checkChap =  (document.getElementsByName("l2tpd_chap_enable")[0].checked == true) ? 1 : 0;
    var checkMsChap =  (document.getElementsByName("l2tpd_mschap_enable")[0].checked == true) ? 1 : 0;
    var checkMsV2Chap =  (document.getElementsByName("l2tpd_mschapv2_enable")[0].checked == true) ? 1 : 0;
    
    if (checkChap == 0 && checkMsChap == 0 && checkMsV2Chap == 0)
    {
    	alertbottom('Could not change l2tp server settings,at least one authentication protocol should be specified.');
       	return false;
    }

	var checkPsk = (document.getElementsByName("l2tpd_l2tpOIPsec_enable")[0].checked == true) ? 1 : 0;
	var checkL2tpServer = (document.getElementsByName("l2tpd_enable")[0].checked == true) ? 1 : 0;
	if (checkL2tpServer == 1){
	     if(1 == checkPsk){
		   if (ipsec_status == 1){
			alertbottom('Please disable IPsec function.');
			return false;
		   }
	     }
	}
      if (1 == checkPsk){
		if (isPassword(E('l2tpd_psk').value) == false) {
			alertbottom('Enter letters digits or underline. Preshared Key length should be from 8 to 40!');
			return false;
		}
		var len = E('l2tpd_psk').value.length;
		if (len < 8 || len > 40) {
			alertbottom('Preshared Key length should be from 8 to 40 digits letters or underline.');
			return false;
		}
    	}
	
	showWebMessage(2, "");
	return true;
}
function initTranslation(){
	var e = document.getElementById("applicationAddApp");
	e.innerHTML = _("l2tp AddApp");
	e = document.getElementById("l2tpServerRule");
	e.innerHTML = _("l2tp Server Rule");
	e = document.getElementById("l2tpMTU");
	e.innerHTML = _("pptp MTU");
	e = document.getElementById("l2tpLocalIP");
	e.innerHTML = _("pptp LocalIP");
	e = document.getElementById("startIP");
	e.innerHTML = _("start IP");
	e = document.getElementById("maxNum");
	e.innerHTML = _("max Num");
	e = document.getElementById("preshareKey");
	e.innerHTML = _("PSK Rule");
	
	e = document.getElementById("autoDns");
	e.innerHTML = _("auto Dns");
	e = document.getElementById("l2tpDns");
	e.innerHTML = _("pptp Dns");
	e = document.getElementById("l2tpChap");
	e.innerHTML = _("chap Enable");
	e = document.getElementById("l2tpMscharp");
	e.innerHTML = _("mscharp Enable");
	e = document.getElementById("mscharpV2");
	e.innerHTML = _("mscharp V2");
	e = document.getElementById("l2tpOIPsec");
	e.innerHTML = _("L2tpOIPsec Rule");
	/*e = document.getElementById("proxyARP");
	e.innerHTML = _("proxy ARP");*/
	e = document.getElementById("l2tpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("dnsEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("charpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("mscharpEnable");
	e.innerHTML = _("pptp Enable");
	e = document.getElementById("mscharpv2Enable");
	e.innerHTML = _("pptp Enable");
	/*e = document.getElementById("arpEnable");
	e.innerHTML = _("pptp Enable");*/
	e = document.getElementById("l2tpOIPsecEnable");
	e.innerHTML = _("pptp Enable");
	
	e = document.getElementById("l2tpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("dnsDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("charpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("mscharpDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("mscharpv2Disable");
	e.innerHTML = _("pptp Disable");
	/*e = document.getElementById("arpDisable");
	e.innerHTML = _("pptp Disable");*/
	e = document.getElementById("l2tpOIPsecDisable");
	e.innerHTML = _("pptp Disable");
	e = document.getElementById("VPNApply");
	e.value = _("VPN Apply");
	e = document.getElementById("VPNReset");
	e.value = _("VPN Reset");
			
} 
function init() {
	initTranslation();
   	 F = document.getElementById("vpn_l2tpd_general");

	if (wan0_proto == "pptp" || wan0_proto == "l2tp")
	{
		document.getElementsByName("l2tpd_enable")[0].disabled = "ture";
		document.getElementsByName("l2tpd_enable")[1].disabled = "ture";
	}

    var autodns_enable = (document.getElementsByName("l2tpd_dns_auto_enable")[0].checked == true) ? 1 : 0;
    sel_change(autodns_enable, 0, F, autodns_objects);
	
    var ipsec_enable = (document.getElementsByName("l2tpd_l2tpOIPsec_enable")[0].checked == true) ? 1 : 0;
    sel_change(ipsec_enable, 1, F, psk_objects);

}
</script>
</head>
  <body onload="init();">

  <form id="vpn_l2tpd_general" name="vpn_l2tpd_general" action="/goform/vpn_l2tpd_general" method="post">

  <div id="table">
    <ul>
      <li class="table_content">
        <div class="data">
          <ul>
	       <script type="text/javascript">do_hidden_elements('l2tpd_rule')</script>	
              <input type="hidden" name="l2tpd_name" 
                    value='<%nvg_attr_get("l2tpd_name", "l2tpd_rule","0","name");%>' />
              <input type="hidden" name="l2tpd_redialperiod" 
                    value='<%nvg_attr_get("l2tpd_redialperiod", "l2tpd_rule","0","redialperiod");%>' />
              <input type="hidden" name="l2tpd_eap_enable" 
                    value='<%nvg_attr_get("l2tpd_eap_enable", "l2tpd_rule","0","eap_enable");%>' />
              <input type="hidden" name="l2tpd_pap_enable" 
                    value='<%nvg_attr_get("l2tpd_pap_enable", "l2tpd_rule","0","pap_enable");%>' />
              <input type="hidden" name="l2tpd_nat_enable" 
                    value='<%nvg_attr_get("l2tpd_nat_enable", "l2tpd_rule","0","nat_enable");%>' />
              <input type="hidden" name="l2tpd_proxyarp_enable" 
                    value='<%nvg_attr_get("l2tpd_proxyarp_enable", "l2tpd_rule","0","proxyarp_enable");%>' />

              <% tbl_get("l2tpd_user_rule"); %>

              <fieldset>
		<li class="title" >
			<table border="0" cellpadding="0" cellspacing="0">
			  <tr><td nowrap id="applicationAddApp">L2TP Server</td></tr>
			</table>
              </li>
              
              <span class='on'>
                <li class="w_text">
                <td>
                  <input type="radio" value="1"  name="l2tpd_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_enable","l2tpd_rule","0","enable","1","checked");%>>
                        <font id="l2tpEnable">Enable</font>
                  </td>
                  <td>
                  <input type="radio" value="0"  name="l2tpd_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_enable","l2tpd_rule","0","enable","0","checked");%>>
                  <font id="l2tpDisable">Disable</font>
                  </td>
		</li>
              </span>

              <li class="space3"></li>
			  <li class="title" id="l2tpServerRule">L2TP Server Rule </li> 
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="l2tpMTU">MTU: </td>
						<td><input id="byteMTU" name="l2tpd_mtu" size="4" maxLength="4" value='<%nvg_attr_get("l2tpd_mtu", "l2tpd_rule", "0", "mtu");%>'/></td>
						</tr>
					</table>
				</li>
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="l2tpLocalIP">VPN Local IP Address: </td>
						<td><input id="l2tpd_localip" name="l2tpd_localip" size="20" maxLength="15" value='<%nvg_attr_get("l2tpd_localip", "l2tpd_rule", "0", "localip");%>'/></td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="startIP">VPN Start IP Address </td>
						<td><input id="l2tpRemoteIP" name="l2tpd_remoteip" size="20" maxLength="15" value='<%nvg_attr_get("l2tpd_remoteip", "l2tpd_rule", "0", "remoteip");%>'  /></td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="maxNum">Max VPN Clients </td>
						<td><input size="3" maxLength="3" name="l2tpd_remoteip_range" id="l2tpd_remoteip_range"
                                            value='<%nvg_attr_get("l2tpd_remoteip_range", "l2tpd_rule", "$0", "remoteip_range");%>'/>
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="autoDns">Auto DNS </td>
						 <td><input type="radio" value="1" name="l2tpd_dns_auto_enable" onclick="init()"
						  <%nvg_attr_match("l2tpd_dns_auto_enable","l2tpd_rule","0","dns_auto_enable","1","checked");%>>
						  <font id="dnsEnable">Enable</font>						
						 </td>
						<td><input type="radio" value="0" name="l2tpd_dns_auto_enable" onclick="init()"
                                            <%nvg_attr_match("l2tpd_dns_auto_enable","l2tpd_rule","0","dns_auto_enable","0","checked");%>>
                       			  <font id="dnsDisable">Disable</font>						
						 </td>
					    </tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="l2tpDns">DNS</td>
						<td><input type="text" maxLength="15" size="20" name="l2tpd_dns" id="l2tpd_dns"
						value='<%nvg_attr_get("l2tpd_dns", "l2tpd_rule", "$0", "dns");%>'/>
						  </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					 <tr>
					     <td width="40%" id="l2tpChap">CHAP Enable</td>
                                        <td><input type="radio" value="1" name="l2tpd_chap_enable" onclick="init()"
                                         <%nvg_attr_match("l2tpd_chap_enable","l2tpd_rule","0","chap_enable","1","checked");%>>
                                         <font id="charpEnable">Enable</font>                  
				            </td>
				           <td><input type="radio" value="0" name="l2tpd_chap_enable" onclick="init()"
                                        <%nvg_attr_match("l2tpd_chap_enable","l2tpd_rule","0","chap_enable","0","checked");%>>
                                       <font id="charpDisable">Disable</font>                  
					    </td>
					   </tr>
					</table>
				</li>

			     <li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					<tr>
					<td width="40%" id="l2tpMscharp">MSCHAP Enable </td>
					  <td><input type="radio" value="1" name="l2tpd_mschap_enable" onclick="init()"
					  <%nvg_attr_match("l2tpd_mschap_enable","l2tpd_rule","0","mschap_enable","1","checked");%>>
                                     <font id="mscharpEnable">Enable</font>                  
				        </td>
				        <td><input type="radio" value="0" name="l2tpd_mschap_enable" onclick="init()"
                                    <%nvg_attr_match("l2tpd_mschap_enable","l2tpd_rule","0","mschap_enable","0","checked");%>>
                                   <font id="mscharpDisable">Disable</font>                  
				      </td>
				    </tr>
				 </table>
			   </li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					    <tr>
					     <td width="40%" id="mscharpV2">MSCHAP v2 Enable </td>
                                        <td><input type="radio" value="1" name="l2tpd_mschapv2_enable" onclick="init()"
                                          <%nvg_attr_match("l2tpd_mschapv2_enable","l2tpd_rule","0","mschapv2_enable","1","checked");%>>
                                         <font id="mscharpv2Enable">Enable</font>                                    
             				     </td>
             				     <td><input type="radio" value="0" name="l2tpd_mschapv2_enable" onclick="init()"
                                       <%nvg_attr_match("l2tpd_mschapv2_enable","l2tpd_rule","0","mschapv2_enable","0","checked");%>>
                                       <font id="mscharpv2Disable">Disable</font>                  
					  </td>
					 </tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="l2tpOIPsec">L2tpOIPsec Enable</td>
						<td><input type="radio" value="1" name="l2tpd_l2tpOIPsec_enable" onclick="init()"
						<%nvg_attr_match("l2tpd_l2tpOIPsec_enable","l2tpd_rule","0","l2tpOIPsec_enable","1","checked");%>>
						<font id="l2tpOIPsecEnable">Enable</font>
						</td>
						<td>
                                          <input type="radio" value="0" name="l2tpd_l2tpOIPsec_enable" onclick="init()"
                                           <%nvg_attr_match("l2tpd_l2tpOIPsec_enable","l2tpd_rule","0","l2tpOIPsec_enable","0","checked");%>>
                                           <font id="l2tpOIPsecDisable">Disable</font>                  
						 </td>
						</tr>
					</table>
				</li>
              		<li class="w_text">
                              <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="40%" id="preshareKey">Preshared Key</td>
                                  <td>
                                    <input type='password' id='l2tpd_psk' name='l2tpd_psk' size='25' maxlength='40' value='<%nvg_attr_get("l2tpd_psk", "l2tpd_rule", "0", "psk_value");%>'/>
                                  </td>
                                </tr>
                              </table>
                            </li>
                            
				<!--
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
					   <tr>
					    <td width="40%" id="proxyARP">Proxy ARP Enable</td>
					    <td><input type="radio" value="1" name="l2tpd_proxyarp_enable" onclick="init()"
					    <%nvg_attr_match("l2tpd_proxyarp_enable","l2tpd_rule","0","proxyarp_enable","1","checked");%>>
					    <font id="arpEnable">Enable</font>                  
				          </td>
				          <td>
				           <input type="radio" value="0"  name="l2tpd_proxyarp_enable" onclick="init()"
				           <%nvg_attr_match("l2tpd_proxyarp_enable","l2tpd_rule","0","proxyarp_enable","0","checked");%>>
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
					<td><input type="radio" value="1" name="l2tpd_nat_enable" onclick="init()"
					  <%nvg_attr_match("l2tpd_nat_enable","l2tpd_rule","0","nat_enable","1","checked");%>>
					  <font id="natActEnable">Enable</font>                  
				       </td>
				       <td><input type="radio" value="0" name="l2tpd_nat_enable" onclick="init()"
				         <%nvg_attr_match("l2tpd_nat_enable","l2tpd_rule","0","nat_enable","0","checked");%>>
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
        <input type=hidden value="/local/advance/vpn_l2tpd_general.asp" name="l2tpd_gen_url">
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

