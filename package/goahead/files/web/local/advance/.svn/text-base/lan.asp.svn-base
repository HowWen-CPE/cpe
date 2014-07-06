<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/local/js/validate.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var opmode = '<% getCfgZero(1, "OP_Mode"); %>';
var lanIpmode = '<% getCfgZero(1, "lanIp_mode"); %>';
var lanIP = "<%getLanIp(); %>";
var lanMask="<% getLanNetmask(); %>";
var wanProto ='<% getWanProto(); %>';

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function lanIp_mode_static()
{
	alert(_("- Switch LAN IP MODE to STATIC")+"\n"
		+_("- May loose the access to WEB UI"));
}

function lanIp_mode_dhcp()
{
	alert(_("- Switch LAN IP MODE to DHCP")+"\n"
		+_("- May loose the access to WEB UI"));
}

function lanIp_mode_switch()
{
    if (document.lanCfg.lanIpMode[0].checked == true)
	{		
		document.lanCfg.lanIpApMode.disabled = true;
	    document.lanCfg.lanNetmaskApMode.disabled = true;
        document.lanCfg.lanGatewayApMode.disabled = true;
		document.lanCfg.lan_fallback_ip.disabled = false;
		document.lanCfg.lan_fallback_mask.disabled = false;
		
	}
	else
	{
		document.lanCfg.lanIpApMode.disabled = false;
	    document.lanCfg.lanNetmaskApMode.disabled = false;
        document.lanCfg.lanGatewayApMode.disabled = false;
		document.lanCfg.lan_fallback_ip.disabled = true;
		document.lanCfg.lan_fallback_mask.disabled = true;
	}

}

function DNS1TypeSwitch()
{
   if ((document.lanCfg.dns1Type.options.selectedIndex == 0) ||
       (document.lanCfg.dns1Type.options.selectedIndex == 2))
   {
        document.lanCfg.staticPriDns.disabled = true;
        //document.lanCfg.staticPriDns.value = "0.0.0.0";
   }
   else
   {
        document.lanCfg.staticPriDns.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.lanCfg.dns2Type.options.selectedIndex == 0) ||
       (document.lanCfg.dns2Type.options.selectedIndex == 2))
   {
        document.lanCfg.staticSecDns.disabled = true;
        //document.lanCfg.staticSecDns.value = "0.0.0.0";
   }
   else
   {
        document.lanCfg.staticSecDns.disabled = false;
   }
}

function DnsUpdate()
{
      if (isDotSeparatedIp(document.lanCfg.staticPriDns.value) == 0) //dns1 not all numbers
      {
          if ((document.lanCfg.staticSecDns.value != "" ) &&
              (isDotSeparatedIp(document.lanCfg.staticSecDns.value) == 1)) //dns2 all numbers
          {
             document.lanCfg.staticPriDns.value=document.lanCfg.staticSecDns.value;
             document.lanCfg.staticSecDns.value="";
          }
          else
          {
            document.lanCfg.staticPriDns.value="";
            document.lanCfg.staticSecDns.value="";
          }  
      }
      else
      {
          if ((document.lanCfg.staticSecDns.value != "" ) &&
              (isDotSeparatedIp(document.lanCfg.staticSecDns.value) == 0)) //dns2 not all numbers
          {
             document.lanCfg.staticSecDns.value="";
          }
      }     
  
  if (document.lanCfg.dns1Type.options.selectedIndex == 2)
  {
      if (document.lanCfg.dns2Type.options.selectedIndex == 2)
      {
           document.lanCfg.staticSecDns.value="";
      }
      else
      {
          if ((document.lanCfg.staticPriDns.value != "" ) &&
              (isDotSeparatedIp(document.lanCfg.staticPriDns.value) == 1)) //dns1 all numbers
          {
             document.lanCfg.staticSecDns.value=document.lanCfg.staticPriDns.value;
          }
      }
      document.lanCfg.staticPriDns.value="";   
  }
  if (document.lanCfg.dns2Type.options.selectedIndex == 2)
  {
      document.lanCfg.staticSecDns.value="";
  }
}

function updateStatus()
{
   DNS1TypeSwitch();
   DNS2TypeSwitch();
   DnsUpdate();
}

//check if lan and inputlan ip are on the same net 
function isSameNet(lan_input,lan,netmask){
      var  lanValue=atoi(lan, 1)*256*256*256 +atoi(lan, 2)*256*256+ atoi(lan, 3)*256+atoi(lan, 4);
      var  inputValue=atoi(lan_input.value, 1)*256*256*256 +atoi(lan_input.value, 2)*256*256+ atoi(lan_input.value, 3)*256+atoi(lan_input.value, 4);  
	
      var netmaskValue=atoi(netmask.value, 1)*256*256*256 +atoi(netmask.value, 2)*256*256+ atoi(netmask.value, 3)*256+atoi(netmask.value, 4); 
      var lan_tmp=lanValue&netmaskValue;
      var input_tmp=inputValue&netmaskValue;
      
        if( input_tmp == lan_tmp ){               
		return true;
	}else{               
		return false;
	}
}

//check whether the input netmask is changed or not. 
function checkNetmaskChange(input_netmask, netmask){
      var  inputValue=atoi(input_netmask, 1)*256*256*256 +atoi(input_netmask, 2)*256*256+ atoi(input_netmask, 3)*256+atoi(input_netmask, 4);  
      var netmaskValue=atoi(netmask, 1)*256*256*256 +atoi(netmask, 2)*256*256+ atoi(netmask, 3)*256+atoi(netmask, 4); 
      
      if( inputValue == netmaskValue ){               
            return true;
	}else{               
	     return false;
	}
}

function checkNetMask(field)
{      //(field.value != "0.0.0.0") && (field.value != "255.255.255.255")&&
        if ((field.value != "128.0.0.0") &&
            (field.value != "192.0.0.0") && (field.value != "224.0.0.0") &&
            (field.value != "240.0.0.0") && (field.value != "248.0.0.0") &&
            (field.value != "252.0.0.0") && (field.value != "254.0.0.0") &&
            (field.value != "255.0.0.0") && (field.value != "255.128.0.0") &&
            (field.value != "255.192.0.0") && (field.value != "255.224.0.0") &&
            (field.value != "255.240.0.0") && (field.value != "255.248.0.0") &&
            (field.value != "255.252.0.0") && (field.value != "255.254.0.0") &&
            (field.value != "255.255.0.0") && (field.value != "255.255.128.0") &&
            (field.value != "255.255.192.0") && (field.value != "255.255.224.0") &&
            (field.value != "255.255.240.0") && (field.value != "255.255.248.0") &&
            (field.value != "255.255.252.0") && (field.value != "255.255.254.0") &&
            (field.value != "255.255.255.0") && (field.value != "255.255.255.128") &&
            (field.value != "255.255.255.192") && (field.value != "255.255.255.224") &&
            (field.value != "255.255.255.240") && (field.value != "255.255.255.248") &&
            (field.value != "255.255.255.252"))
        {
		alertbottom("Error. Invalid Subnet Mask.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

function checkLanIpValid(lanIp, lanMask)
{

  ip_1 = atoi(lanIp, 1);
  mask_1 = atoi(lanMask, 1);
  subNet_1 = ip_1 & mask_1;
  
  ip_2 = atoi(lanIp, 2);
  mask_2 = atoi(lanMask, 2);
  subNet_2 = ip_2 & mask_2;
  
  ip_3 = atoi(lanIp, 3);
  mask_3 = atoi(lanMask, 3);
  subNet_3 = ip_3 & mask_3;
  
  ip_4 = atoi(lanIp, 4);
  mask_4 = atoi(lanMask, 4);
  subNet_4 = ip_4 & mask_4;
  
  subNet = subNet_1 + "." + subNet_2 +"." + subNet_3 + "." + subNet_4;
  if(lanIp == subNet) //all bits of ip representation for station part are all "0" 
  {
     alertbottom("Invalid Lan IP Address.");
     document.lanCfg.lanIp.value = document.lanCfg.lanIp.defaultValue;
     document.lanCfg.lanIp.focus();
     return false;
  }

   if(lanMask == "255.255.255.252")
   {
      subNet_4 = subNet_4 + 3; //all bits of ip representation for station part are all "1" 
   }
   else if(lanMask == "255.255.255.248")
   {
      subNet_4 = subNet_4 + 7;
   }
   else if(lanMask == "255.255.255.240")
   {
      subNet_4 = subNet_4 + 15;
   }
   else if(lanMask == "255.255.255.224")
   {
      subNet_4 = subNet_4 + 31;
   }
   else if(lanMask == "255.255.255.192")
   {
      subNet_4 = subNet_4 + 63;
   }
   else if(lanMask == "255.255.255.128")
   {
      subNet_4 = subNet_4 + 127;
   }
   else if(lanMask == "255.255.255.0")
   {
      subNet_4 = subNet_4 + 255;
   }

   subNet = subNet_1 + "." + subNet_2 +"." + subNet_3 + "." + subNet_4;
   if(lanIp == subNet) //all bits of ip representation for station part are all "1" 
   {
     alertbottom("Invalid Lan IP Address.");
     document.lanCfg.lanIp.value = document.lanCfg.lanIp.defaultValue;
     document.lanCfg.lanIp.focus();
     return false;
   }

   return true;
}

function checkSubnet(ip, mask, client)
{
  if(client=="0.0.0.0")
     return true;
  
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  client_d = atoi(client, 1);

  if ( (ip_d & mask_d) != (client_d & mask_d ) )
  {
	return false;
  }

  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  client_d = atoi(client, 2);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
  {
	return false;
  }

  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  client_d = atoi(client, 3);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
  {
	return false;
  }

  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  client_d = atoi(client, 4);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
  {
    return false;
  }
  return true;
}

function checkSubnetValid(field1,field2,field3)
{
     if(!checkSubnet(field1.value,field2.value,field3.value))
     {
         alertbottom('subnet is invalid');
         return false;
     }
     return true;
}

function getSubnet(ip, mask)
{
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  subnet_1 = (ip_d & mask_d)
  
  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  subnet_2 = (ip_d & mask_d)
  
  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  subnet_3 = (ip_d & mask_d)
  
  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  subnet_4 = (ip_d & mask_d)
	
  subnet_all = subnet_1 + "." + subnet_2 +"." + subnet_3 + "." + subnet_4;

  return subnet_all;
}

function getDhcpStartIp(lanSubnet, lanIp)
{
   i_4 = atoi(lanIp, 4);
   
   s_1 = atoi(lanSubnet, 1);
   s_2 = atoi(lanSubnet, 2);
   s_3 = atoi(lanSubnet, 3);
   s_4 = atoi(lanSubnet, 4);
   
   s_4 = s_4 +1; //starting dhcp ip in normally
   //if(s_4 == i_4) //same as LAN IP Address, so plus 1
   //   s_4 = s_4 +1;
   
   start_ip = s_1 + "." + s_2 +"." + s_3 + "." + s_4;
   return start_ip;
}

function getDhcpEndIp(lanSubnet, lanIp, lanMask)
{
   i_4 = atoi(lanIp, 4);
   
   s_1 = atoi(lanSubnet, 1);
   s_2 = atoi(lanSubnet, 2);
   s_3 = atoi(lanSubnet, 3);
   s_4 = atoi(lanSubnet, 4);
   
   //class C +
   if(lanMask == "255.255.255.252")
   {
      s_4 = s_4 + 2; //end dhcp ip in normally (2*2 -2)
   }
   else if(lanMask == "255.255.255.248")
   {
      s_4 = s_4 + 6; //end dhcp ip in normally (2*2*2 -2)
   }
   else if(lanMask == "255.255.255.240")
   {
      s_4 = s_4 + 14; //end dhcp ip in normally (2*2*2*2 -2)
   }
   else if(lanMask == "255.255.255.224")
   {
      s_4 = s_4 + 30; //end dhcp ip in normally (2*2*2*2*2 -2)
   }
   else if(lanMask == "255.255.255.192")
   {
      s_4 = s_4 + 62; //end dhcp ip in normally (2*2*2*2*2*2 -2)
   }
   else if(lanMask == "255.255.255.128")
   {
      s_4 = s_4 + 126; //end dhcp ip in normally (2*2*2*2*2*2*2 -2)
   }
   else if(lanMask == "255.255.255.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally (2*2*2*2*2*2*2 -2)
   }
   
   //class B +
   else if(lanMask == "255.255.254.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.252.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.248.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.240.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.224.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.192.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.128.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.255.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   
   //class A +
   else if(lanMask == "255.254.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.252.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.248.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.240.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.224.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.192.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.128.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "255.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   
   //class A
   else if(lanMask == "254.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "252.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "248.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "240.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "224.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "192.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   else if(lanMask == "128.0.0.0")
   {
      s_4 = s_4 + 254; //end dhcp ip in normally
   }
   
   
   //if(s_4 == i_4) //same as LAN IP Address, so minus 1
   //     s_4 = s_4 - 1;
        
   end_ip = s_1 + "." + s_2 +"." + s_3 + "." + s_4;
   return end_ip;
}

function CheckValue()
{   
    if (opmode == "0" || opmode == "6" || opmode == "7") //Router, WISP0, WISP1 
    {

        if (!checkIpAddr(document.lanCfg.lanIp, false))
		    return false;
	    if (!checkIpAddr(document.lanCfg.lanNetmask, true))
		    return false;
	    if (!checkNetMask(document.lanCfg.lanNetmask))
				return false;
				
		if (!checkLanIpValid(document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value))
		    return false;
	    if (wanProto == "static"){
              var wanIPAddr = "<% getWanIpAdd(); %>";
	        if(isSameNet(document.lanCfg.lanIp,wanIPAddr,document.lanCfg.lanNetmask)){
                  alertbottom('LAN and WAN is in the same network segment,error.');
                  return false;
		 }
	    }	    	
	    document.lanCfg.dhcpGateway.value = document.lanCfg.lanIp.value;
	    document.lanCfg.dhcpNetmask.value = document.lanCfg.lanNetmask.value; //aron add 2009.05.25
	    //aron add for special IP treatment
		subnet = getSubnet(document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value);
		//alertbottom("subnet is " + subnet);
		dhcp_possible_starting = getDhcpStartIp(subnet, document.lanCfg.lanIp.value);
		//alertbottom("dhcp_possible_starting is " + dhcp_possible_starting);
		dhcp_possible_ending = getDhcpEndIp(subnet, document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value);
		//alertbottom("dhcp_possible_ending is " + dhcp_possible_ending); 
	        d1 = atoi(document.lanCfg.lanIp.value, 1);
                d2 = atoi(document.lanCfg.lanIp.value, 2);
                d3 = atoi(document.lanCfg.lanIp.value, 3);
                d4 = document.lanCfg.dhcpStart.value;
                var dhcpStartIp = d1 + "." + d2 +"." + d3 + "." + d4;
	        document.lanCfg.dhcpStart.value=dhcpStartIp;
		//same subnet, just use original dhcp start ip address
		//if ( checkSubnet(document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value, lanMask)== true)
		if ( checkNetmaskChange(document.lanCfg.lanNetmask.value, lanMask)== true)
		{
	    	    if ( checkSubnet(document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value, document.lanCfg.dhcpStart.value)== true)
	    	        dhcp_starting = document.lanCfg.dhcpStart.value;
	    	    else
	    	        dhcp_starting = dhcp_possible_starting;
	    	                      		        	           			
	    	    //alertbottom("same subnet and use default as start ip is " + dhcp_starting);
	    
	    	if(dhcp_starting == document.lanCfg.lanIp.value) //Ex:lan ip change to 10.1.1.10
	    	{
	        	//dd4 = atoi(dhcp_starting, 4); //Ex:lan ip change to 10.1.1.10
	        	//pp4 = atoi(dhcp_possible_starting, 4); //Ex:possible is 10.1.1.9
	        	//alertbottom("dd4 is " + dd4);
	        	//alertbottom("pp4 is " + pp4);
	        	//if(dd4 > pp4)
	        	//   dd4 = dd4 - 1; //Ex:back to 10.1.1.9
	        	//else if(dd4 < pp4)
	        	//   dd4 = dd4 + 1; //Ex:add to 10.1.1.10
	        	//pp1 = atoi(dhcp_starting, 1);
	        	//pp2 = atoi(dhcp_starting, 2);
	        	//pp3 = atoi(dhcp_starting, 3);
	        	//dhcp_starting = pp1 + "." + pp2 +"." + pp3 + "." + dd4;
	        	dhcp_starting = dhcp_possible_starting;
	        	//alertbottom("lan ip conflict, modified start ip is " + dhcp_starting);
	    	}
	    
		}
		else //different subnet, so use the possible start ip address instead
		{
	    	     dhcp_starting = dhcp_possible_starting;
	    	//alertbottom("diffrent subnet and use possible as start ip is " + dhcp_starting);
	    
	    	/* special consideration
	    	dd4 = atoi(document.lanCfg.dhcpStart.value, 4);
	    	pp4 = atoi(dhcp_possible_starting, 4);
	    	ss4 = atoi(dhcp_possible_ending, 4);
	   	if((pp4 < dd4) && (dd4 < ss4)) //check if the last byte of original dhcp start ip is
	   	{                              //large than possible start ip and less than possible end ip
	      	pp1 = atoi(dhcp_possible_starting, 1);
	      	pp2 = atoi(dhcp_possible_starting, 2);
	      	pp3 = atoi(dhcp_possible_starting, 3);
	      	dhcp_starting = pp1 + "." + pp2 +"." + pp3 + "." + dd4;
	      	alertbottom("modified start ip is " + dhcp_starting);
	    }
	    */
	}

	d1 = atoi(document.lanCfg.lanIp.value, 1);
    d2 = atoi(document.lanCfg.lanIp.value, 2);
    d3 = atoi(document.lanCfg.lanIp.value, 3);
    e1=document.lanCfg.dhcpPoolSize.value;                                                                 
    e2=document.lanCfg.dhcpStart.value;                                                                           
    dhcp_pool=parseInt(e1,10);                                                                                    
    dhcp_end=atoi(e2,4);                                                                                   
    d4 = dhcp_pool + dhcp_end - 1;                                                                                
    var dhcpEndIp = d1 + "." + d2 +"." + d3 + "." + d4;
	document.lanCfg.dhcpEnd.value=dhcpEndIp;
	//same subnet, just use original dhcp end ip address
	if ( checkNetmaskChange(document.lanCfg.lanNetmask.value, lanMask)== true)
	{
	    if ( checkSubnet(document.lanCfg.lanIp.value, document.lanCfg.lanNetmask.value, document.lanCfg.dhcpStart.value)== true)
	       dhcp_ending = document.lanCfg.dhcpEnd.value;
	    else
	       dhcp_ending = dhcp_possible_ending;
	    //alertbottom("same subnet and use default as end ip is " + dhcp_ending);
	    
	    if(dhcp_ending == document.lanCfg.lanIp.value)
	    {
	        dhcp_ending = dhcp_possible_ending;
	        //alertbottom("lan ip conflict, modified end ip is " + dhcp_ending);
	    }
	}
	else //different subnet, so use the possible end ip address instead
	{
	    dhcp_ending = dhcp_possible_ending;
	    //alertbottom("different subnet and use possible as end ip is " + dhcp_ending);
	}
	/* special consideration
	original_dhcp_pool_size = parseInt(document.lanCfg.dhcpPoolSize.value,10);
	aa4 = atoi(dhcp_starting, 4);
	ee4 = atoi(dhcp_ending, 4);
	possible_dhcp_pool_size = ee4 - aa4 + 1;
	alertbottom("original_dhcp_pool_size is " + original_dhcp_pool_size);
	alertbottom("possible_dhcp_pool_size is " + possible_dhcp_pool_size);
	if(original_dhcp_pool_size < possible_dhcp_pool_size)
	{
	     dhcpPoolSize = original_dhcp_pool_size;
	     alertbottom("use original pool size, dhcpPoolSize is " + dhcpPoolSize); 
	     bb1 = atoi(dhcp_starting, 1);
	     bb2 = atoi(dhcp_starting, 2);
	     bb3 = atoi(dhcp_starting, 3);
	     bb4 = atoi(dhcp_starting, 4);
	     bb4 = bb4 + dhcpPoolSize - 1;
	     dhcp_ending = bb1 + "." + bb2 +"." + bb3 + "." + bb4;
	     alertbottom("modified end ip is " + dhcp_ending);
	}
	else
	{
	     dhcpPoolSize = possible_dhcp_pool_size;
	     alertbottom("use possible pool size, dhcpPoolSize is " + dhcpPoolSize);
	}
	*/
	aa4 = atoi(dhcp_starting, 4);
	ee4 = atoi(dhcp_ending, 4);
	possible_dhcp_pool_size = ee4 - aa4 + 1;
	dhcpPoolSize = possible_dhcp_pool_size;
	//alertbottom("dhcpPoolSize is " + dhcpPoolSize);
	
	//-------- aron add 2009.02.25
	//d1 = atoi(document.lanCfg.lanIp.value, 1);
        //d2 = atoi(document.lanCfg.lanIp.value, 2);
        //d3 = atoi(document.lanCfg.lanIp.value, 3);
        //d_start_part4 = atoi(document.lanCfg.dhcpStart.value, 4);
        //d_end_part4 = atoi(document.lanCfg.dhcpEnd.value, 4);
    
        //dhcp_start_ip = d1 + "." + d2 +"." + d3 + "." + d_start_part4;
        //dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d_end_part4;
        //alertbottom(dhcp_start_ip);
        //alertbottom(dhcp_end_ip); 
        //document.lanCfg.dhcpStart.value = dhcp_start_ip;
        //document.lanCfg.dhcpEnd.value = dhcp_end_ip;
        //----------------------------------------------
        
        //aron add for special IP treatment
        document.lanCfg.dhcpStart.value = aa4;
        document.lanCfg.dhcpEnd.value = dhcp_ending; 
        document.lanCfg.dhcpPoolSize.value = dhcpPoolSize;
    }
    else if(opmode == "1" || opmode == "8" || opmode == "9") //AP, STA0, STA1
    {
		if (document.lanCfg.lanIpMode[0].checked == true)
		{
			if(!checkIpAddr(document.lanCfg.lan_fallback_ip, false)) {
				alertbottom("Fallip invalid");
				document.lanCfg.lan_fallback_ip.focus();
				return false;
			}
		    if (!checkNetMask(document.lanCfg.lan_fallback_mask)) {
		    alertbottom("Fallmask invalid");
		    document.lanCfg.lan_fallback_mask.focus();
				return false;
			}
		}
        if (document.lanCfg.lanIpMode[1].checked == true) //fixed IP
	    {			
			if ((document.lanCfg.dns1Type.options.selectedIndex == 0) ||
                 (document.lanCfg.dns2Type.options.selectedIndex == 0))
            {
                alertbottom('LAN is Fixed IP, please set DNS Assignment to User-Defined or None.');
                return false;
            }
			if (!checkIpAddr(document.lanCfg.lanIpApMode, false))
		        return false;
	             if (!checkIpAddr(document.lanCfg.lanNetmaskApMode, true))
		        return false;
		    //if (!checkIpAddr(document.lanCfg.lanGatewayApMode, false))
		    if (!checkLanGatewayIpAddr(document.lanCfg.lanGatewayApMode, false)) {
                alertbottom('LAN is Fixed IP, please set Gateway IP Address.');
		        return false;
		    }
		    if(!checkSubnetValid(document.lanCfg.lanIpApMode,document.lanCfg.lanNetmaskApMode,document.lanCfg.lanGatewayApMode))
		       return false;
		    if (!checkNetMask(document.lanCfg.lanNetmaskApMode))
				return false;
	    }
	    //DNS
        if (document.lanCfg.dns1Type.options.selectedIndex == 1) //DNS1 User-Defined
        {
            if (!checkIpAddr(document.lanCfg.staticPriDns, false))
		        return false;

            if(document.lanCfg.staticPriDns.value == "")
            {
                alertbottom('First DNS Server should not be empty with User-Defined.');
                return false;
            }
            if(document.lanCfg.staticPriDns.value == "0.0.0.0")
            {
                alertbottom('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
                return false;
            }
            if (!checkIpAddr(document.lanCfg.staticPriDns, false))
				return false;
        }
        if (document.lanCfg.dns2Type.options.selectedIndex == 1)
        {
            if (!checkIpAddr(document.lanCfg.staticSecDns, false))
		        return false;
            if(document.lanCfg.staticSecDns.value == "")
            {
                alertbottom('Second DNS Server should not be empty with User-Defined.');
                return false;
            }
            if(document.lanCfg.staticSecDns.value == "0.0.0.0")
            {
                alertbottom('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
                return false;
            }
            if (!checkIpAddr(document.lanCfg.staticSecDns, false))
				return false;
        } 
        
        /*
        document.lanCfg.DhcpGateway.value = document.lanCfg.lanIpApMode.value;
	    d1 = atoi(document.lanCfg.lanIpApMode.value, 1);
        d2 = atoi(document.lanCfg.lanIpApMode.value, 2);
        d3 = atoi(document.lanCfg.lanIpApMode.value, 3);
        d_start_part4 = atoi(document.lanCfg.DhcpStart.value, 4);
        d_end_part4 = atoi(document.lanCfg.DhcpEnd.value, 4);
    
        dhcp_start_ip = d1 + "." + d2 +"." + d3 + "." + d_start_part4;
        dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d_end_part4; 
        document.lanCfg.DhcpStart.value = dhcp_start_ip;
        document.lanCfg.DhcpEnd.value = dhcp_end_ip;
        */
    }
    showWebMessage(2, "");
    return true;
}

function initTranslation(){
	var e = document.getElementById("IP_Address");
	e.innerHTML = _("lan Address");
	e = document.getElementById("IP_Subnet_Mask");
	e.innerHTML = _("lan Subnet");

	e = document.getElementById("lan_ip_config");
	e.innerHTML = _("lan IP_Config");
	e = document.getElementById("Get_From_DHCP");
	e.innerHTML = _("lan Get_From_DHCP");
	e = document.getElementById("Use_defined_IP");
	e.innerHTML = _("lan Fixed_IP");
	e = document.getElementById("ap_IP_Address");
	e.innerHTML = _("lan Address");
	e = document.getElementById("ap_IP_Subnet_Mask");
	e.innerHTML = _("lan Subnet");
	e = document.getElementById("ap_Gateway_IP_Address");
	e.innerHTML = _("lan Gateway");
	e = document.getElementById("DNS Assignment");
	e.innerHTML = _("lan DNS Assignment");
	e = document.getElementById("First DNS Server");
	e.innerHTML = _("lan First DNS Server");
	e = document.getElementById("Second DNS Server");
	e.innerHTML = _("lan Second DNS Server");
	e = document.getElementById("lanDNSISP");
	e.innerHTML = _("lan DNS ISP");
	e = document.getElementById("lanDNSUSER");
	e.innerHTML = _("lan DNS User");
	e = document.getElementById("lanDNSNONE");
	e.innerHTML = _("lan DNS None");
	e = document.getElementById("lanDNSISP2");
	e.innerHTML = _("lan DNS ISP");
	e = document.getElementById("lanDNSUSER2");
	e.innerHTML = _("lan DNS User");
	e = document.getElementById("lanDNSNONE2");
	e.innerHTML = _("lan DNS None");
	e = document.getElementById("lanApply");
	e.value = _("lan Apply");
	e = document.getElementById("lanReset");
	e.value = _("lan Reset");

	e = document.getElementById("lan_fallback_ip_id");
	e.innerHTML = _("lan fallback ip");
	e = document.getElementById("lan_fallback_mask_id");
	e.innerHTML = _("lan fallback mask");

	
}

function initValue()
{
	initTranslation();

    //var opmode = "<% getCfgZero(1, "OperationMode"); %>";
    var dns1_type = "<% getCfgGeneral(1, "lan_dns1_type"); %>";
    var dns2_type = "<% getCfgGeneral(1, "lan_dns2_type"); %>";

    show_div(false, "routing_setting");
    show_div(false, "ap_setting");
    show_div(true,"ethernet_fromdhcp");
        
    if (opmode == "0" || opmode == "6" || opmode == "7") //Router, WISP0, WISP1
    {
        show_div(true, "routing_setting");
    }
    else if(opmode == "1" || opmode == "8" || opmode == "9") //AP, STA0, STA1
    {
        show_div(true, "ap_setting");
        if (lanIpmode == "0") //LAN IP get from DHCP Server

        {     
	        document.lanCfg.lanIpMode[0].checked = true;
	        document.lanCfg.lanIpApMode.disabled = true;
	        document.lanCfg.lanNetmaskApMode.disabled = true;
            document.lanCfg.lanGatewayApMode.disabled = true;
			document.lanCfg.lan_fallback_ip.disabled = false;
			document.lanCfg.lan_fallback_mask.disabled = false;
	    }
        else //use fixed LAN IP
        {
            document.lanCfg.lanIpMode[1].checked = true;
            document.lanCfg.lanIpApMode.disabled = false;
	        document.lanCfg.lanNetmaskApMode.disabled = false;
            document.lanCfg.lanGatewayApMode.disabled = false;
			document.lanCfg.lan_fallback_ip.disabled = true;
			document.lanCfg.lan_fallback_mask.disabled = true;
        }
        
        //for dns1 and dns2 types
        if (dns1_type == "ISP")
        {
            document.lanCfg.dns1Type.options.selectedIndex = 0;
        }
        else if (dns1_type == "USER")
        {
            document.lanCfg.dns1Type.options.selectedIndex = 1;
        }
        else if (dns1_type == "NONE")
        {   
            document.lanCfg.dns1Type.options.selectedIndex = 2;

        }    
        if (dns2_type == "ISP")
        {
            document.lanCfg.dns2Type.options.selectedIndex = 0;
        }
        else if (dns2_type == "USER")
        {
            document.lanCfg.dns2Type.options.selectedIndex = 1;
        }
        else if (dns2_type == "NONE")
        {   
            document.lanCfg.dns2Type.options.selectedIndex = 2;
        }        
        updateStatus();
    }
//    parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}

</script>
</head>



<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanIP" onSubmit="return CheckValue()">

    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="lan_ip_config">LAN TCP/IP </li>
       <span id = "routing_setting" class = "off" >
       <!-- for Routing Mode (BEGIN) -->
                <li class="w_text">
                  <table id=LanIPAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="IP_Address">IP Address :</td>
                      <td><input name="lanIp" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_ipaddr"); %>" /></td>
                    </tr>
                  </table>
                  <input type=hidden name="dhcpGateway">
                  <input type=hidden name="dhcpNetmask">
                  <input type=hidden name="dhcpStart" value="<% getCfgGeneral(1, "dhcpStart"); %>"></tr>
                  <input type=hidden name="dhcpEnd" value="<% getCfgGeneral(1, "dhcpEnd"); %>"></tr>
                  <input type=hidden name="dhcpPoolSize" value="<% getCfgGeneral(1, "lan_dhcp_pool_size"); %>"></tr>
                </li> 
                <li class="w_text">
                  <table id=LanIPNetMask width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="IP_Subnet_Mask">IP Subnet Mask :</td>
                      <td><input name="lanNetmask" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_netmask"); %>" /></td>
                    </tr>
                  </table>
                </li>
       <!-- for Routing Mode (END)-->
       </span>
       <span id = "ap_setting" class = "off" >

       <!-- for AP Mode (BEGIN) -->
       <span id="ethernet_fromdhcp" class="off" >
                <li class="w_text">
                  <table id="lanIp_client" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wlanIpMode"><input type="radio" value="0" onclick="lanIp_mode_switch();lanIp_mode_dhcp();" name="lanIpMode" />
                      	<font id="Get_From_DHCP"> Get from DHCP Server </font></td>
                    </tr>
                  </table>
                </li>
				<li class="w_text">
                  <table id="fallback_ip" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td  width="40%" id="lan_fallback_ip_id">FallBack IP:</td>
					  <td>
					  		<input name="lan_fallback_ip" maxlength=15 size=15 value="<% getLanFallBackIp(); %>" />
					  </td>
                    </tr>
                  </table>
                </li>
				<li class="w_text">
                  <table id="fallback_mask" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lan_fallback_mask_id"> FallBack mask:</td>
					 <td>
					 	<input name="lan_fallback_mask" maxlength=15 size=15 value="<% getLanFallBackNetmask(); %>" />
					 </td>
                    </tr>
                  </table>
                </li>
        </span>
                <li class="w_text">
                  <table id="lanIp_fixed" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wlanIpMode"><input type="radio" value="1" onclick="lanIp_mode_switch();lanIp_mode_static();"
                        name="lanIpMode" /><font id="Use_defined_IP">Use Defined LAN IP Address </font></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanIPAddrApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ap_IP_Address">IP Address :</td>
                      <!--<td><input name="lanIpApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_ipaddr"); %>" /></td>-->
                        <td><input name="lanIpApMode" maxlength=15 size=15 
                        value="<% getLanIp(); %>" /></td>
                    </tr>
                  </table>
                  <input type=hidden name="DhcpGateway">
                  <input type=hidden name="DhcpStart" value="<% getCfgGeneral(1, "dhcpStart"); %>"></tr>
                  <input type=hidden name="DhcpEnd" value="<% getCfgGeneral(1, "dhcpEnd"); %>"></tr>
                </li> 
                <li class="w_text">
                  <table id=LanIPNetMaskApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ap_IP_Subnet_Mask">IP Subnet Mask :</td>
                      <!--<td><input name="lanNetmaskApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_netmask"); %>" /></td>-->
                        <td><input name="lanNetmaskApMode" maxlength=15 size=15 
                        value="<% getLanNetmask(); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanGatewayApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ap_Gateway_IP_Address">Gateway IP Address :</td>
                      <!--<td><input name="lanGatewayApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_gateway"); %>" /></td>-->
                        <td><input name="lanGatewayApMode" maxlength=15 size=15 
                        value="<% getLanIfGateway(); %>" /></td>
                    </tr>
                  </table>
                </li> 
                
                
                <li class="space3"></li>
                <li class="title" id="DNS Assignment" >DNS Assignment </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="First DNS Server" >First DNS Server : </td>
                      <td><select name="dns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="ISP" id="lanDNSISP">From ISP</option>
                          <option value="USER" id="lanDNSUSER">User-Defined</option>
                          <option value="NONE" id="lanDNSNONE">None</option>
                        </select>
                          <!--<input name="staticPriDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_primary_dns"); %>"></td>-->
                          <input name="staticPriDns" maxlength=15 size=15 value="<% getLanDns(1); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="Second DNS Server">Second DNS Server : </td>
                      <td><select name="dns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="ISP" id="lanDNSISP2">From ISP</option>
                          <option value="USER" id="lanDNSUSER2">User-Defined</option>
                          <option value="NONE" id="lanDNSNONE2">None</option>
                        </select>
                          <!--<input name="staticSecDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_secondary_dns"); %>"></td>-->
                          <input name="staticSecDns" maxlength=15 size=15 value="<% getLanDns(2); %>"></td>
                    </tr>
                  </table>
                </li> 
       <!-- for AP Mode (END)-->
       </span>
                <li></li>
				<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=submit value="Apply" id="lanApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan.asp" name="lan_url">
        </li>
        </center>
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
