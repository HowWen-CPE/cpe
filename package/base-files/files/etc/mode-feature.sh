#!/bin/sh /etc/rc.common 
set -x
router_mode_features="pppoe pptp dhcpc dhcps dns nat firewall trigger ipsec pptps l2tps vpn ftp h323 upnp snmp"
ap_mode_features="dhcpc"
wisp_mode_features="pppoe pptp dhcpc dhcps dns nat firewall trigger ipsec pptps l2tps vpn ftp h323 upnp snmp"
sta_mode_features="dhcpc"
feature_judge() {
   service_name=$1
   sys_mode="$(nvram show system_mode 0 name)"
   case "$sys_mode" in
     normal)
	for service in $router_mode_features
	do
	    [ $service = "$service_name" ] &&
	       return 1
	done
	return 0
     ;;
     ap)
   	for service in $ap_mode_features
	do
	    [ $service = "$service_name" ] &&
		return 1			
	done
	return 0
     ;;
     wisp0|wisp1)
   	for service in $wisp_mode_features
	do
	   [ $service = "$service_name" ] &&
		return 1			
	done
	return 0
     ;;
     sta0|sta1)
   	for service in $sta_mode_features
	do
	   [ $service = "$service_name" ] &&
		return 1				
	done
	return 0  
   ;;
   *)
    echo "success.....";;
   esac
}		
