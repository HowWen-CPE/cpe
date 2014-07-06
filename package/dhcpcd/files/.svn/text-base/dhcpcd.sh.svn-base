#!/bin/sh
#
#  This is a sample /etc/dhcpcd.sh script.
#  /etc/dhcpcd.sh script is executed by dhcpcd daemon
#  any time it configures or shuts down interface.
#  The following parameters are passed to dhcpcd.exe script:
#  $1 = HostInfoFilePath, e.g  "/var/lib/dhcpcd/dhcpcd-eth0.info"
#  $2 = "up" if interface has been configured with the same
#       IP address as before reboot;
#  $2 = "down" if interface has been shut down;
#  $2 = "new" if interface has been configured with new IP address;
#
#  Sanity checks
if [ $# -lt 2 ]; then
  logger -s -p local0.err -t dhcpcd.sh "wrong usage"
  exit 1
fi

hostinfo="$1"
state="$2"

# Reading HostInfo file for configuration parameters
. "${hostinfo}"

. /etc/network.sh
. /etc/netmask.sh

NETMASK_PREFIX=`mask2prefix ${NETMASK}`

iface_clear () {
    ifacename=${1}
    type="${1%%[0-9]*}"
    num="${1#$type}"
    ipaddr="$(nvram get ${1}_ipaddr)"
    bridge_name="$(nvram get ${1}_ifname)"
    [ "${bridge_name%%[0-9]*}" = "ppp" ] && {
        bridge_name="$(nvram get ${1}_device)"
    }
    
    [ "$type" = "lan" -a "$(iface_check_wan_enabled)" = "0" ] && {
        gateway="$(nvram get ${1}_gateway)"
        ifname="$(nvram get ${1}_ifname)"
        mask="$(nvram get ${1}_mask)"

		#get lan network
    	eval $(ipcalc $ipaddr $mask)
		lan_network=$NETWORK
		
		#get forback network
		fb_ip=`nvram show lan_dhcps_rule 0 lan0_fallback_ip`
		fb_mask=`nvram show lan_dhcps_rule 0 lan0_fallback_mask`
    	eval $(ipcalc $fb_ip $fb_mask)
    	fb_network=$NETWORK
	
		#only when fallback and lan doesn't equal, do we update(del) lan route 
		if [ "$lan_network" != "$fb_network" ];then
        	iface_del_route "$num" "$ipaddr" "$mask" "$gateway" "$ifname" "$type"
		fi
    }
    [ "$type" = "wan" ] && [ -n "$ipaddr" ] && {
        gateway="$(nvram get ${1}_gateway)"
        ifname="$(nvram get ${1}_ifname)"
        mask="$(nvram get ${1}_mask)"
        
        iface_del_route "$num" "$ipaddr" "$mask" "$gateway" "$ifname" "$type"
    }
    [ -n $ipaddr ] && {
        nvram unset ${1}_ipaddr
        nvram unset ${1}_mask
        nvram unset ${1}_gateway
        nvram unset ${1}_dns
        nvram unset ${1}_domain
    }
    
	iface_update_default_route_and_dns $type

	#update lan/wan_dns_rule
	case "${type}" in 
	lan)
		usr_dns_1=""
		usr_dns_2=""
		if [ `nvram show lan_dns_rule 0 dnstype1` = "custom" ];then
			usr_dns_1=`nvram show lan_dns_rule 0 dns1`
		fi
		if [ `nvram show lan_dns_rule 0 dnstype2` = "custom" ];then
			usr_dns_2=`nvram show lan_dns_rule 0 dns2`
		fi
		if [ `nvram show lan_dns_rule 0 dnstype1` = "ispdns" ];then
			dns_1="`nvram show lan_dns_rule 0 dns1`"
			if [ -n "$dns_1" ];then
				if [ -n "$usr_dns_2" ];then
					[ "$usr_dns_2" != "$dns_1" ] && \
					sleep 5 && sed -i "/$dns_1/d" /tmp/resolv.conf&
				else
					sleep 5 && sed -i "/$dns_1/d" /tmp/resolv.conf&
				fi
			fi
			nvram replace attr lan_dns_rule 0 dns1 "" 
		fi
		if [ `nvram show lan_dns_rule 0 dnstype2` = "ispdns" ];then
			dns_2="`nvram show lan_dns_rule 0 dns2`"
			if [ -n "$dns_2" ];then
				if [ -n "$usr_dns_1" ];then
					[ "$usr_dns_1" != "$dns_2" ] && \
					sleep 5 && sed -i "/$dns_2/d" /tmp/resolv.conf&
				else
					sleep 5 && sed -i "/$dns_2/d" /tmp/resolv.conf&
				fi
			fi
			nvram replace attr lan_dns_rule 0 dns2 "" 
		fi
		;;
	wan)
		usr_dns_1=""
		usr_dns_2=""
		if [ `nvram show wan_dns_rule 0 dnstype1` = "custom" ];then
			usr_dns_1=`nvram show wan_dns_rule 0 dns1`
		fi
		if [ `nvram show wan_dns_rule 0 dnstype2` = "custom" ];then
			usr_dns_2=`nvram show wan_dns_rule 0 dns2`
		fi
		if [ `nvram show wan_dns_rule 0 dnstype1` = "ispdns" ];then
			dns_1="`nvram show wan_dns_rule 0 dns1`"
			if [ -n "$dns_1" ];then
				if [ -n "$usr_dns_2" ];then
					[ "$usr_dns_2" != "$dns_1" ] && \
					sleep 5 && sed -i "/$dns_1/d" /tmp/resolv.conf&
				else
					sleep 5 && sed -i "/$dns_1/d" /tmp/resolv.conf&
				fi
			fi
			nvram replace attr wan_dns_rule 0 dns1 "" 
		fi
		if [ `nvram show wan_dns_rule 0 dnstype2` = "ispdns" ];then
			dns_2="`nvram show wan_dns_rule 0 dns2`"
			if [ -n "$dns_2" ];then
				if [ -n "$usr_dns_1" ];then
					[ "$usr_dns_1" != "$dns_2" ] && \
					sleep 5 && sed -i "/$dns_2/d" /tmp/resolv.conf&
				else
					sleep 5 && sed -i "/$dns_2/d" /tmp/resolv.conf&
				fi
			fi
			nvram replace attr wan_dns_rule 0 dns2 "" 
		fi
		;;
	esac
	nvram commit
}


case "${state}" in
    up|new)

	#logger 
	case "${state}" in
	up)
		logger -s -p local0.info -t dhcpcd.sh \
		"interface ${INTERFACE} has been configured with old IP=${IPADDR}";
		;;
	new)
		logger -s -p local0.info -t dhcpcd.sh \
		"interface ${INTERFACE} has been configured with new IP=${IPADDR}";
		;;
	esac

	#update fallback iface
	if [ ${INTERFACE} = "br-lan0" ]; then
		fb_ip=`nvram show lan_dhcps_rule 0 lan0_fallback_ip`
		fb_mask=`nvram show lan_dhcps_rule 0 lan0_fallback_mask`
		ip addr add $fb_ip/$fb_mask dev ${INTERFACE} label ${INTERFACE}:0
	fi


	if [ ${INTERFACE} = "br-lan0" ]; then
		#update route table
		if [ -n "${GATEWAYS}" ];then 
			GATEWAY=`echo ${GATEWAYS} | cut -d' ' -f1`;
			#iface_add_route 0 ${IPADDR} ${NETMASK_PREFIX} ${GATEWAY} ${INTERFACE} lan
			nvram set lan0_gateway="${GATEWAY}"
		fi

		#update dns resolv
		dns_1=""
		dns_2=""
		case `nvram show lan_dns_rule 0 dnstype1` in
			ispdns)
				dns_1=`echo ${DNSSERVERS} | cut -d' ' -f1`
				nvram replace attr lan_dns_rule 0 dns1 $dns_1
			;;
			custom)
				dns_1=`nvram show lan_dns_rule 0 dns1`
			;;
			*|none)
				dns_1=""
			;;
		esac
		case `nvram show lan_dns_rule 0 dnstype2` in
			ispdns)
				dns_2=`echo ${DNSSERVERS} | cut -d' ' -f2`
				nvram replace attr lan_dns_rule 0 dns2 $dns_2
			;;
			custom)
				dns_2=`nvram show lan_dns_rule 0 dns2`
			;;
			*|none)
				dns_2=""
			;;
		esac
		echo "" > /tmp/resolv.conf
		[ -n $dns_1 ] && echo "nameserver "$dns_1 >> /tmp/resolv.conf
		[ -n $dns_2 ] && echo "nameserver "$dns_2 >> /tmp/resolv.conf

		#update nvarm
		nvram set lan0_ipaddr="${IPADDR}";
		nvram set lan0_mask="${NETMASK_PREFIX}";
		nvram commit;

		#update trigger
		[ ${state} = "new" ] && env -i ACTION="ifup" INTERFACE="lan0" PROTO=dhcp TYPE="lan" \
		NUM="0" IFNAME="$INTERFACE" /sbin/hotplug-call "iface" &
	fi
	if [ ${INTERFACE} = "br-wan0" ]; then

		#update route table
		if [ -n "${GATEWAYS}" ];then 
			GATEWAY=`echo ${GATEWAYS} | cut -d' ' -f1`;
			#iface_add_route 0 ${IPADDR} ${NETMASK_PREFIX} ${GATEWAY} ${INTERFACE}
			nvram set wan0_gateway="${GATEWAY}"
			#iface_update_default_route_and_dns wan
		fi

		#update dns resolv
		[ `nvram show wan0_proto 0 curr` = "dhcp" ] && {
		dns_1=""
		dns_2=""
		case `nvram show wan_dns_rule 0 dnstype1` in
			ispdns)
				dns_1=`echo ${DNSSERVERS} | cut -d' ' -f1`
				nvram replace attr wan_dns_rule 0 dns1 $dns_1
			;;
			custom)
				dns_1=`nvram show wan_dns_rule 0 dns1`
			;;
			*|none)
				dns_1=""
			;;
		esac
		case `nvram show wan_dns_rule 0 dnstype2` in
			ispdns)
				dns_2=`echo ${DNSSERVERS} | cut -d' ' -f2`
				nvram replace attr wan_dns_rule 0 dns2 $dns_2
			;;
			custom)
				dns_2=`nvram show wan_dns_rule 0 dns2`
			;;
			*|none)
				dns_2=""
			;;
		esac
		echo "" > /tmp/resolv.conf
		[ -n $dns_1 ] && echo "nameserver "$dns_1 >> /tmp/resolv.conf
		[ -n $dns_2 ] && echo "nameserver "$dns_2 >> /tmp/resolv.conf
		}

		#update nvram
		nvram set wan0_ipaddr="${IPADDR}"; 
		nvram set wan0_mask="${NETMASK_PREFIX}";
		nvram commit;
        #start pptp/l2tp
        num=0
        iface=wan$num
        ifname=$(nvram get ${iface}_device)
        ifdev=$(nvram get ${iface}_ifname)
        proto=$(nvram show ${iface}_proto 0 curr)
        if [ `nvram show wan0_proto 0 curr` = "pptp" -a `nvram show wan_pptpc_rule 0 iptype` = "dhcp" ]; then
        	[ ${state} = "new" ] && {
        		/sbin/ifup.pptp $proto $ifdev wan$num $num down
        		sleep 3
        		/sbin/ifup.pptp wan$num wan $num $ifname up
        	}
        elif [ `nvram show wan0_proto 0 curr` = "l2tp" -a `nvram show wan_l2tpc_rule 0 iptype` = "dhcp" ]; then
        	[ ${state} = "new" ] && {
        		/sbin/ifup.l2tp $proto $ifdev wan$num $num down
        		sleep 3
        		/sbin/ifup.l2tp wan$num wan $num $ifname up
        	}
        else
        	#update trigger
			[ ${state} = "new" ] && env -i ACTION="ifup" INTERFACE="wan0" PROTO=dhcp TYPE="wan" \
			NUM="0" IFNAME="$INTERFACE" /sbin/hotplug-call "iface" &
		fi
	fi

	

    # Put your code here for when the interface has been brought up with an
    # old IP address here
    ;;

    down) 
    logger -s -p local0.info -t dhcpcd.sh \
    "interface ${INTERFACE} has been brought down";
	[ ${INTERFACE} = "br-lan0" ] && {
		iface_clear lan0
		#update trigger
		env -i ACTION="ifdown" INTERFACE="lan0" PROTO=dhcp TYPE="lan" \
		NUM="0" IFNAME="$INTERFACE" /sbin/hotplug-call "iface" &
	};
	[ ${INTERFACE} = "br-wan0" ] && {
		num=0
		iface=wan$num
		ifdev=$(nvram get ${iface}_ifname)
		proto=$(nvram show ${iface}_proto 0 curr)
		#stop dhcp
		iface_clear wan0
		#stop pptp/l2tp
		if [ `nvram show wan0_proto 0 curr` = "pptp" -a `nvram show wan_pptpc_rule 0 iptype` = "dhcp" ]; then
			/sbin/ifup.pptp $proto $ifdev wan$num $num down
		elif [ `nvram show wan0_proto 0 curr` = "l2tp" -a `nvram show wan_l2tpc_rule 0 iptype` = "dhcp" ]; then
			/sbin/ifup.l2tp $proto $ifdev wan$num $num down
		else
		   #update trigger
		   env -i ACTION="ifdown" INTERFACE="wan0" PROTO=dhcp TYPE="wan" \
		   NUM="0" IFNAME="$INTERFACE" /sbin/hotplug-call "iface" &
		fi
	};
    # Put your code here for the when the interface has been shut down
    ;;
esac
exit 0
