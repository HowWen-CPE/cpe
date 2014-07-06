<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/local/js/validate.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var dhcpMode = '<% getCfgZero(1, "dhcpMode"); %>';
var lanIP = "<%getLanIp(); %>";

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

function DNS1TypeSwitch()
{
   if ((document.lanCfg.dhcpDns1Type.options.selectedIndex == 2) ||
       (document.lanCfg.dhcpDns1Type.options.selectedIndex == 3))
   {
        document.lanCfg.dhcpPriDns.disabled = true;
        document.lanCfg.dhcpPriDns.value = "0.0.0.0";
   }else if(document.lanCfg.dhcpDns1Type.options.selectedIndex == 0){
	 document.lanCfg.dhcpPriDns.disabled = true;
        document.lanCfg.dhcpPriDns.value = "8.8.8.8";
   }else //User-Defined
   {
        document.lanCfg.dhcpPriDns.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.lanCfg.dhcpDns2Type.options.selectedIndex == 2) ||
       (document.lanCfg.dhcpDns2Type.options.selectedIndex == 3))
   {
        document.lanCfg.dhcpSecDns.disabled = true;
        document.lanCfg.dhcpSecDns.value = "0.0.0.0";
   }else if(document.lanCfg.dhcpDns2Type.options.selectedIndex == 0){
        document.lanCfg.dhcpSecDns.disabled = true;
        document.lanCfg.dhcpSecDns.value = "8.8.4.4";        
   }else //User-Defined
   {
        document.lanCfg.dhcpSecDns.disabled = false;
   }
}

function CheckValue()
{
   if (dhcpMode != "DHCPS") //DHCP Server not enabled
   { 
         alertbottom('Please enable DHCP Server firstly before using LAN Static DHCP Table or DNS Server.');
         return false;
   } 
   
   if((document.lanCfg.dhcpStaticMac1.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac2.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac3.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac4.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac5.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac6.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac7.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac8.value != "00:00:00:00:00:00"))
   {   
      if (dhcpMode != "DHCPS") //DHCP Server not enabled
      { 
         alertbottom('Please enable DHCP Server before using LAN Static DHCP Table.');
         return false;
      }
   }   
  
   //Static1
   if (document.lanCfg.dhcpStaticMac1.value.length == 0) {
   		document.lanCfg.dhcpStaticMac1.value = document.lanCfg.dhcpStaticMac1.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac1.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac1.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac1.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac1.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac1.value;
			value1=address_value.substr(0,2);
			val1=parseInt(value1,16);
			if(val1 & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac1.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp1, false))
		            return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp1.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp1.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp1.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp1.focus();
        		return false;
		    }
    }
   else
   {
       if (document.lanCfg.dhcpStaticIp1.value.length == 0) {
   		document.lanCfg.dhcpStaticIp1.value = document.lanCfg.dhcpStaticIp1.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp1.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp1.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac1.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac1.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac1.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac1.focus();
			return false;
		}
	}		 
   }    
   //Static2
   if (document.lanCfg.dhcpStaticMac2.value.length == 0) {
		document.lanCfg.dhcpStaticMac2.value = document.lanCfg.dhcpStaticMac2.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac2.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac2.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac2.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac2.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac2.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac2.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp2, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp2.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp2.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp2.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp2.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp2.value.length == 0) {
   		document.lanCfg.dhcpStaticIp2.value = document.lanCfg.dhcpStaticIp2.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp2.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp2.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac2.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac2.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac2.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac2.focus();
			return false;
		}
	}		 
   }
   //Static3
   if (document.lanCfg.dhcpStaticMac3.value.length == 0) {
		document.lanCfg.dhcpStaticMac3.value = document.lanCfg.dhcpStaticMac3.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac3.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac3.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac3.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac3.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac3.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac3.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp3, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp3.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp3.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp3.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp3.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp3.value.length == 0) {
   		document.lanCfg.dhcpStaticIp3.value = document.lanCfg.dhcpStaticIp3.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp3.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp3.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac3.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac3.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac3.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac3.focus();
			return false;
		}
	}		 
   }
   //Static4
   if (document.lanCfg.dhcpStaticMac4.value.length == 0) {
		document.lanCfg.dhcpStaticMac4.value = document.lanCfg.dhcpStaticMac4.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac4.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac4.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac4.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac4.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac4.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac4.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp4, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp4.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp4.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp4.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp4.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp4.value.length == 0) {
   		document.lanCfg.dhcpStaticIp4.value = document.lanCfg.dhcpStaticIp4.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp4.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp4.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac4.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac4.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac4.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac4.focus();
			return false;
		}
	}		 
   }
   //Static5
   if (document.lanCfg.dhcpStaticMac5.value.length == 0) {
		document.lanCfg.dhcpStaticMac5.value = document.lanCfg.dhcpStaticMac5.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac5.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac5.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac5.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac5.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac5.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac5.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp5, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp5.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp5.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp5.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp5.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp5.value.length == 0) {
   		document.lanCfg.dhcpStaticIp5.value = document.lanCfg.dhcpStaticIp5.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp5.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp5.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac5.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac5.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac5.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac5.focus();
			return false;
		}
	}		 
   }
   //Static6
   if (document.lanCfg.dhcpStaticMac6.value.length == 0) {
		document.lanCfg.dhcpStaticMac6.value = document.lanCfg.dhcpStaticMac6.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac6.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac6.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac6.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac6.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac6.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac6.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp6, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp6.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp6.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp6.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp6.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp6.value.length == 0) {
   		document.lanCfg.dhcpStaticIp6.value = document.lanCfg.dhcpStaticIp6.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp6.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp6.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac6.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac6.focus();
			return false;
		}		
		if(document.lanCfg.dhcpStaticMac6.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac6.focus();
			return false;
		}
	}		 
   }
   //Static7
   if (document.lanCfg.dhcpStaticMac7.value.length == 0) {
		document.lanCfg.dhcpStaticMac7.value = document.lanCfg.dhcpStaticMac7.defaultValue;
		alertbottom("Error. MAC address is empty.");
		document.lanCfg.dhcpStaticMac7.focus();
		return false;
   }
   if(document.lanCfg.dhcpStaticMac7.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac7.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac7.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac7.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac7.focus();
				return false;				
			}

			if (!checkIpAddr(document.lanCfg.dhcpStaticIp7, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp7.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp7.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp7.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp7.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp7.value.length == 0) {
   		document.lanCfg.dhcpStaticIp7.value = document.lanCfg.dhcpStaticIp7.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp7.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp7.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac7.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac7.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac7.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac7.focus();
			return false;
		}
	}		 
   }
   //Static8
   if (document.lanCfg.dhcpStaticMac8.value.length == 0) {
	   document.lanCfg.dhcpStaticMac8.value = document.lanCfg.dhcpStaticMac8.defaultValue;
	   alertbottom("Error. MAC address is empty.");
	   document.lanCfg.dhcpStaticMac8.focus();
	   return false;
   }
   if(document.lanCfg.dhcpStaticMac8.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac8.value)) {
				alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac8.focus();
				return false;
			}
			address_value=document.lanCfg.dhcpStaticMac8.value;
			value=address_value.substr(0,2);
			val=parseInt(value,16);
			if(val & 0x01 == 1){
				alertbottom(_("Broadcast or Multicast MAC Address not support!"));
        		       document.lanCfg.dhcpStaticMac8.focus();
				return false;				
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp8, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp8.value)) 
		    {
        		//alertbottom('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alertbottom('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp8.focus();
        		return false;
		    }
		    if (document.lanCfg.dhcpStaticIp8.value == lanIP) 
		    {
        		alertbottom('Invalid IP address! It should not be the same as current LAN IP address.');
        		document.lanCfg.dhcpStaticIp8.focus();
        		return false;
		    }
   }
   else
   {
       if (document.lanCfg.dhcpStaticIp8.value.length == 0) {
   		document.lanCfg.dhcpStaticIp8.value = document.lanCfg.dhcpStaticIp8.defaultValue;
		alertbottom("Error. IP address is empty.");
		document.lanCfg.dhcpStaticIp8.focus();
		return false;
	}
	if(document.lanCfg.dhcpStaticIp8.value != "0.0.0.0") {
              var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;			
		if (!re.test(document.lanCfg.dhcpStaticMac8.value)) {
			alertbottom("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
			document.lanCfg.dhcpStaticMac8.focus();
			return false;
		}
		if(document.lanCfg.dhcpStaticMac8.value == "00:00:00:00:00:00"){
			alertbottom("Please enter a valid MAC address for the specified IP address.");
			document.lanCfg.dhcpStaticMac8.focus();
			return false;
		}
	}		 
   }
   
    //DNS
     if (document.lanCfg.dhcpDns1Type.options.selectedIndex == 1) //DNS1 User-Defined
     {
         if(document.lanCfg.dhcpPriDns.value == "")
         {
            alertbottom('First DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.lanCfg.dhcpPriDns.value == "0.0.0.0")
         {
            alertbottom('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.lanCfg.dhcpPriDns, false))
				return false;
     }
     if (document.lanCfg.dhcpDns2Type.options.selectedIndex == 1)
     {
         if(document.lanCfg.dhcpSecDns.value == "")
         {
            alertbottom('Second DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.lanCfg.dhcpSecDns.value == "0.0.0.0")
         {
            alertbottom('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.lanCfg.dhcpSecDns, false))
				 return false;
     }
   
   showWebMessage(2, "");
   return true;
}

function updateStatus()
{
     DNS1TypeSwitch();
     DNS2TypeSwitch();

}

function initValue()
{
   var dns1_type = "<% getCfgGeneral(1, "dhcp_dns1_type"); %>";
   var dns2_type = "<% getCfgGeneral(1, "dhcp_dns2_type"); %>";
   
   //for dns1 and dns2 types
  if (dns1_type == "GOOGLEDNS")
   {   
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 0;
   }
   else if (dns1_type == "USER")
   {
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 1;
   }
   else if (dns1_type == "RELAY")
   {   
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 2;
   } 
   else if (dns1_type == "NONE")
   {   
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 3;
   }   
   if (dns2_type == "GOOGLEDNS")
   {   
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 0;
   }
   else if (dns2_type == "USER")
   {
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 1;
   }
   else if (dns2_type == "RELAY")
   {   
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 2;
   }   
   else if (dns2_type == "NONE")
   {   
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 3;
   }        
      var e = document.getElementById("lanAdvtitle");
	e.innerHTML = _("lanAdv title");
	e = document.getElementById("lanAdvMAC");
	e.innerHTML = _("lanAdv MAC");
	e = document.getElementById("lanAdvIP");
	e.innerHTML = _("lanAdv IP");
	e = document.getElementById("lanAdvDNS");
	e.innerHTML = _("lanAdv DNS");
		e = document.getElementById("lanAdvDNStitle");
	e.innerHTML = _("lanAdvDNS title");
		e = document.getElementById("lanAdvDNSServer1");
	e.innerHTML = _("lanAdvDNS Server1");
			e = document.getElementById("lanAdvDNSServer1Defined");
	e.innerHTML = _("lanAdv DNSServer1Defined");
			e = document.getElementById("lanAdvDNSServer1Relay");
	e.innerHTML = _("lanAdv DNSServer1Relay");
			e = document.getElementById("lanAdvDNSServer2");
	e.innerHTML = _("lanAdv DNSServer2");
			e = document.getElementById("lanAdvDNSServer2Defined");
	e.innerHTML = _("lanAdv DNSServer2Defined");
			e = document.getElementById("lanAdvDNSServer2Relay");
	e.innerHTML = _("lanAdv DNSServer2Relay");
	e = document.getElementById("lan_static_dhcp_table");
	e.innerHTML = _("lan static dhcp table");
        
	
	e = document.getElementById("lanAdvApply");
	e.value = _("lanAdv Apply");
	e = document.getElementById("lanAdvReset");
	e.value = _("lanAdv Reset");

   updateStatus();
   
   if (dhcpMode != "DHCPS") //DHCP Server not enabled
   { 
       document.lanCfg.dhcpDns1Type.disabled = true;
       document.lanCfg.dhcpDns2Type.disabled = true;   
   } 
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLan_StaicLease_Dns" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
				<li class="title" id="lanAdvtitle">LAN Static DHCP Table</li>
				<li></li>
				<li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" cellpadding="0" cellspacing="0" >
                          
                          <tr>
                            <td height="26" colspan="10"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td background="images/table_top_center.gif" class="table_title" id="lan_static_dhcp_table"> LAN Static DHCP Table </td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
                          
                          <tr >
                            <td width="5%"><center>
                              <span class="top_left">#</span>
                            </center>                            </td>
                            <td width="46%"><center>
                              <span class="top_font" id="lanAdvMAC">MAC Address</span>
                            </center>                            </td>
                            <!--<td width="20%"><center>
                              <span class="top_font">Direction</span>
                            </center>                            </td>-->
                            <td width="46%"><center>
                              <span class="top_right" id="lanAdvIP">IP Address </span>
                            </center>                            </td>
                            <!--<td width="15%"><center>
                              <span class="top_font">Priority</span>
                            </center>                            </td>
                            <td width="15%"><center>
                              <span class="top_right">Modify </span>
                            </center>                            </td>-->
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">1</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac1" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac1"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp1" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp1"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">2</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac2" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac2"); %>" />
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp2" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp2"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>                            
                          <tr >
                            <td><center>
                              <span class="table_left">3</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac3" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac3"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp3" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp3"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>                          
                          <tr >
                            <td><center>
                              <span class="table_left">4</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac4" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac4"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp4" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp4"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">5</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac5" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac5"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp5" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp5"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">6</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac6" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac6"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp6" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp6"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">7</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac7" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac7"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp7" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp7"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">8</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac8" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac8"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp8" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp8"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                          <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                          
                          <tr>
                            <td height="5" colspan="10"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                                  <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                                  <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
                                </tr>
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                  </table>
			    </li>
			    <li class="space3"></li>
                <li class="title"  id="lanAdvDNS" >DNS Server</li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="lanAdvDNStitle">DNS Servers Assigned by DHCP Server </td>
                    </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr height=15>
                      <td width="150" id="lanAdvDNSServer1">First DNS Server : </td>
                      <td width="150" ><select name="dhcpDns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="GOOGLEDNS" id="lanAdvDNSServer1Google">Google DNS Server</option>                          
                          <option value="USER" id="lanAdvDNSServer1Defined">User-Defined</option>
                          <option value="RELAY" id="lanAdvDNSServer1Relay">DNS Relay</option>
                        </select>
                      </td>
                      <td>
                          <input type="text" name="dhcpPriDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "dhcpPriDns"); %>">
                      </td>
                    </tr>
                    <tr height=15>
                      <td id="lanAdvDNSServer2">Second DNS Server : </td>
                      <td><select name="dhcpDns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="GOOGLEDNS" id="lanAdvDNSServer2Google">Google DNS Server</option>                          
                          <option value="USER" id="lanAdvDNSServer2Defined">User-Defined</option>
                          <option value="RELAY" id="lanAdvDNSServer2Relay">DNS Relay</option>
                        </select>
                      </td>
                      <td>
                          <input type="text" name="dhcpSecDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "dhcpSecDns"); %>">
                      </td>
                    </tr>
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
        <input type=submit value="Apply" id="lanAdvApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanAdvReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan_dhcp_advance.asp" name="lan_dhcp_advance_url">
        </li></center>
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
