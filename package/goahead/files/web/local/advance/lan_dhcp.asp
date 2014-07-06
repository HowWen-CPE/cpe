<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>    
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/local/js/validate.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var dhcpMode = '<% getCfgZero(1, "dhcpMode"); %>';
var dhcpStartIP = '<% getCfgGeneral(1, "dhcpStart"); %>';

function isValid(str)
{
	for (var i=0; i<str.length; i++) {
		if(str.charAt(0)=='0')
			return 0;
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') )
			continue;
		return 0;
	}
	return 1;

}

function checkSubnet(ip, mask, client)
{
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

function checkClientRange(gw,start,end)
{
	gw_d = atoi(gw,4);
	gw_d += atoi(gw,3)*256;
  	gw_d += atoi(gw,2)*256;
  	gw_d += atoi(gw,1)*256;

  	start_d = atoi(start,4);
  	start_d += atoi(start,3)*256;
  	start_d += atoi(start,2)*256;
  	start_d += atoi(start,1)*256;

  	end_d = atoi(end,4);
  	end_d += atoi(end,3)*256;
  	end_d += atoi(end,2)*256;
  	end_d += atoi(end,1)*256;
  	if( start_d <= end_d ){
		if((gw_d == end_d))
			return false;
		else
			return true;
	}

  	return false;
}

function CheckValue()
{ 
    d1 = atoi(document.lanCfg.dhcpStart.value, 1);
    d2 = atoi(document.lanCfg.dhcpStart.value, 2);
    d3 = atoi(document.lanCfg.dhcpStart.value, 3);
    d4 = atoi(document.lanCfg.dhcpStart.value, 4);
    d5 = d4 + parseInt(document.lanCfg.dhcpPoolSize.value,10)-1;
    
    dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d5;
    //alertbottom(dhcp_end_ip); 
    document.lanCfg.dhcpEnd.value = dhcp_end_ip;
   
    if (document.lanCfg.dhcpModeType.options.selectedIndex == 1) 
    {
        if (!checkIpAddr(document.lanCfg.dhcpStart, false))
				return false;
        if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStart.value)) {
        		alertbottom('Invalid DHCP start address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStart.value = document.lanCfg.dhcpStart.defaultValue;
        		document.lanCfg.dhcpStart.focus();
        		return false;
        }
        if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpEnd.value)) {
        		alertbottom('Invalid DHCP end address with pool size allocated! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpPoolSize.value = document.lanCfg.dhcpPoolSize.defaultValue;
        		document.lanCfg.dhcpPoolSize.focus();
        		return false;
        }
        if ( !isValid(document.lanCfg.dhcpPoolSize.value) ) {
        		alertbottom('Invalid Pool Size value!');
        		document.lanCfg.dhcpPoolSize.focus();
        		return false;
        }
		
        if ( !checkClientRange(document.lanCfg.lanIP.value, document.lanCfg.dhcpStart.value, document.lanCfg.dhcpEnd.value) ) {
        		alertbottom('Invalid DHCP address range!');
        		document.lanCfg.dhcpStart.focus();
        		return false;
        }
	
	
        if(d5 > 254 )
        {
            alertbottom('Invalid DHCP address range!');
            return false;
        }
    }

    if (document.lanCfg.dhcpModeType.options.selectedIndex == 2) 
    {

        if (!checkIpAddr(document.lanCfg.remoteDHCPServer, false))
	        return false;
    }

    document.lanCfg.dhcpGateway.value = document.lanCfg.lanIP.value;
    document.lanCfg.dhcpStarting.value = d4;
    showWebMessage(2, "");
    return true;
}

function updateStatus()
{

        var form=document.lanCfg;
	    if (dhcpMode == "NONE") //DHCP Server
        {     
        	form.dhcpModeType.options.selectedIndex = 0;
        	form.dhcpStart.disabled = true;
        	form.dhcpPoolSize.disabled = true;
        	form.remoteDHCPServer.disabled = true;
	    }
        else if (dhcpMode == "DHCPS")
        {
            form.dhcpModeType.options.selectedIndex = 1;
            form.dhcpStart.disabled = false;
            form.dhcpPoolSize.disabled = false;
            form.remoteDHCPServer.disabled = true;
        }
	    else
        {
            form.dhcpModeType.options.selectedIndex = 2;
            form.dhcpStart.disabled = true;
            form.dhcpPoolSize.disabled = true;
            form.remoteDHCPServer.disabled = false;        
        }
    lanIp=document.lanCfg.lanIP.value;
	lanMask=document.lanCfg.lanMask.value;
	ip_1 = atoi(lanIp, 1);
 	mask_1 = atoi(lanMask, 1);
  	ip1 = ip_1 & mask_1;
  
  	ip_2 = atoi(lanIp, 2);
 	mask_2 = atoi(lanMask, 2);
  	ip2 = ip_2 & mask_2;
  
  	ip_3 = atoi(lanIp, 3);
  	mask_3 = atoi(lanMask, 3);
  	ip3 = ip_3 & mask_3;
       ip4 = dhcpStartIP;
       var startIpAddr = ip1 + "." + ip2 +"." + ip3 + "." + ip4;
       document.lanCfg.dhcpStart.value = startIpAddr;
       document.lanCfg.dhcpStart.defaultValue = startIpAddr; //aron add for user input check
}

function dhcpModeTypeSwitch()
{
    var form=document.lanCfg;
	if (document.lanCfg.dhcpModeType.options.selectedIndex == 0) {
	    form.dhcpStart.disabled=true;
        form.dhcpPoolSize.disabled=true;
        form.remoteDHCPServer.disabled=true;
	}
	else if (document.lanCfg.dhcpModeType.options.selectedIndex == 1) {
	    form.dhcpStart.disabled=false;
        form.dhcpPoolSize.disabled=false;
        form.remoteDHCPServer.disabled=true;
	}
	else if (document.lanCfg.dhcpModeType.options.selectedIndex == 2) {
	    form.dhcpStart.disabled=true;
        form.dhcpPoolSize.disabled=true;
        form.remoteDHCPServer.disabled=false;
	}
}

function initValue()
{
      var e = document.getElementById("lanDhcptitle");
	e.innerHTML = _("LanDhcp title");
	e = document.getElementById("dhcpMode");
	e.innerHTML = _("LanDhcp Mode");
	e = document.getElementById("lanDhcpIP");
	e.innerHTML = _("LanDhcp IP");
	e = document.getElementById("lanDhcpPool");
	e.innerHTML = _("LanDhcp PoolSize");
	e = document.getElementById("serverAddr");
	e.innerHTML = _("LanDhcp serverAddr");	
	e = document.getElementById("dhcpNone");
	e.innerHTML = _("LanDhcp None");
	e = document.getElementById("dhcpServer");
	e.innerHTML = _("LanDhcp Server");
	e = document.getElementById("dhcpRelay");
	e.innerHTML = _("LanDhcp Relay");	
	e = document.getElementById("DHCPModeNote1_0");
	e.innerHTML = _("DHCP Mode note1_0");	
	e = document.getElementById("DHCPModeNote1_1");
	e.innerHTML = _("DHCP Mode note1_1");	
	
	e = document.getElementById("lanDhcpApply");
	e.value = _("lanDhcp Apply");
	e = document.getElementById("lanDhcpReset");
	e.value = _("lanDhcp Reset");	
	
   updateStatus();
}
</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanDhcp" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="lanDhcptitle">LAN DHCP Setup</li>
                <li class="w_text">
                  <table id=LanDhcpType width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="dhcpMode">DHCP Mode </td>
                      <td><select name="dhcpModeType" size="1" onChange="dhcpModeTypeSwitch();">
                          <option value="NONE" id="dhcpNone">None</option>
                          <option value="DHCPS" id="dhcpServer">DHCP Server</option>
                          <option value="RELAY" id="dhcpRelay">DHCP Relay</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <!--  
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="19%" id="lanDhcpIP"><span class="cfig_word">
                        IP Pool Starting Address</td>
                        <td><input name="dhcpStart" maxlength=15 size=15 />
                      </span></td>
                      <td width="8%" id="lanDhcpPool">Pool Size </td>
                      <td><input type="text" name="dhcpPoolSize" size="3" maxlength="3" value="<% getCfgGeneral(1, "lan_dhcp_pool_size"); %>" /></td>
                    </tr>
                    <tr><input type=hidden name="dhcpEnd"></tr>
                    <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                    <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                    <tr><input type=hidden name="dhcpGateway"></tr>
                    <tr><input type=hidden name="dhcpStarting"></tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanDhcpIP"><span class="cfig_word">
                        IP Pool Starting Address</td>
                        <td><input name="dhcpStart" maxlength=15 size=15 />
                      </span></td>
                    </tr>
                    <tr><input type=hidden name="dhcpEnd"></tr>
                    <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                    <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                    <tr><input type=hidden name="dhcpGateway"></tr>
                    <tr><input type=hidden name="dhcpStarting"></tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanDhcpPool">Pool Size </td>
                      <td><input name="dhcpPoolSize" size="3" maxlength="3" value="<% getCfgGeneral(1, "lan_dhcp_pool_size"); %>" type="text" onKeyUp="value=value.replace(/[^\d]/g,'');" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=dhcpServerAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="serverAddr">Remote DHCP Server</td>
                      <td><input type="text" name="remoteDHCPServer" size="15" maxlength="15" value="<% getCfgGeneral(1, "remote_dhcp_server_addr"); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                   <tr>
                     <td><span class="i_note" id="DHCPModeNote1_0">Note:</span></td>
                   </tr>
                  </table>
                </li>                
                <li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                   <tr>
                    <td class="i_note_a" id="DHCPModeNote1_1">
                      DHCP Relay: In this mode, the NAT function should be disabled, otherwise the DHCP Relay does not take effect.
                    </td>
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
        <input type=submit value="Apply" id="lanDhcpApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanDhcpReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan_dhcp.asp" name="lan_dhcp_url">
        </li>
        </center>
</body>
<script>
function alertbottom(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
