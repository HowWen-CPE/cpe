#!/bin/sh

#. /etc/log.sh

# mainly for vlan
if_valid () (
  ifconfig "$1" >&- 2>&- ||
  [ "${1%%[0-9]*}" = "br-lan" ] || [ "${1%%[0-9]*}" = "ppp" ] ||
  {
    [ "${1%%[0-9]*}" = "vlan" ] && ( 
      id=${1#vlan}
      vif=$(nvram show vlan_rule $2 ifname)
      [ -z "$vif" ] && {
        # give another try
        hwaddr=$(nvram get ${hwname}macaddr)
        vif=$(ifconfig -a | awk '/^eth.*'$hwaddr'/ {print $1; exit}' IGNORECASE=1)
      }

      [ -z "$vif" ] && return 1
      echo "# vlan$id => $vif"

      $DEBUG ifconfig $vif promisc
      $DEBUG ifconfig $vif up
      $DEBUG vconfig add $vif $id 2>&-
    )
  } ||
  { debug "# missing interface '$1' ignored"; false; }
)

# mainly for br $1: br-lanX $2: stp
ifbr_valid () (
  ifconfig "$1" >&- 2>&- ||
  [ "${1%%[0-9]*}" = "ppp" ] ||
  {
    brname="${1%%[0-9]*}" 
    [ "$brname" = "br-lan" -o "$brname" = "br-wan" ] && ( 
      id=${1#$brname}
      echo "# $1" >/dev/console

    [ "$brname" = "br-lan"  ] && brrule="br_lan" 
    [ "$brname" = "br-wan"  ] && brrule="br_wan" 
      $DEBUG brctl delbr $1 2>&-
      $DEBUG brctl addbr $1
      #Add by Kevin for Forward-Delay time of different project: WP688(2s),others(0s)
      project="$(nvram get board_model_rule)"
      [ $project = "WP688" ] && {
      $DEBUG brctl setfd $1 2
      } || {
      $DEBUG brctl setfd $1 0
      }
      $DEBUG brctl stp $1 ${2:-0}
      for sif in $(nvram show ${brrule}_rule $id ifnames); do
        sifprefix="${sif%%[0-9]*}" 
	wlan_prefix="$(nvram get wlan_dev_prefix)"
	skip_wlan_dev=0
	for wpre in $wlan_prefix 
	do
		[ $wpre = "$sifprefix" ] && skip_wlan_dev=1
	done
#	[ "$sifprefix" = "rai" -o "$sifprefix" = "apclii" -o  "$sifprefix" = "ath" ] || {
	[ $skip_wlan_dev = "1" ] || {
	$DEBUG ip link set dev $sif up allmulticast on
	$DEBUG ifconfig $sif up
	}
	$DEBUG brctl addif $1 $sif
      done
    )
  } ||
  { debug "# missing br interface '$1' ignored"; false; }
)

# $1:proto, $2:iface, $3:type, $4:num, $5:ifname

do_ifup() {


	case "$1" in
	static)
		ip=$(nvram show ${3}_static_rule $4 ipaddr)
		mask=$(nvram show ${3}_static_rule $4 mask)
		mtu=$(nvram show ${3}_main_rule $4 mtu)

		$DEBUG ip -4 addr flush dev $5
		$DEBUG ip -4 addr add $ip/$mask broadcast + dev $5
		$DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up

        gateway=$(nvram show ${3}_static_rule $4 gateway)
        dns=$(nvram show ${3}_static_rule $4 dns1)

        iface_up "$iface" "$ip" "$mask" "$gateway" "$dns"

		env -i ACTION="ifup" INTERFACE="${2}" PROTO=static TYPE="${3}" \
            NUM="${4}" IFNAME="${5}" /sbin/hotplug-call "iface" &
	;;
    directip)
        # config the directip card first
		[ -x "/sbin/ifup.$1" ] && { $DEBUG /sbin/ifup.$1 ${2} ${3} ${4} ${5}; }
        [ "$?" = "1" ] && exit

        # same as dhcp TODO: remove the same dhcp code
		dhcp_ip=$(nvram get ${2}_ipaddr)
		mtu=$(nvram show ${3}_main_rule $4 mtu)

		$DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up

		# FIXME! $DEBUG ifconfig $5 $DHCP_IP ${DHCP_NETMASK:+netmask $DHCP_NETMASK} ${mtu:+mtu $(($mtu))} broadcast + up

		dhcp_args="-i $5 ${dhcp_ip:+-r $dhcp_ip} -b -p $pidfile"
		dhcp_hostname=$(nvram show ${3}_dhcp_rule $4 hostname)
		dhcp_hostname=${dhcp_hostname%%.*}

		[ -z $dhcp_hostname ] || dhcp_args="$dhcp_args -H $dhcp_hostname"

        # FIXME!
		[ "$1" = "pptp" ] && dhcp_args="$dhcp_args -n -q" || dhcp_args="$dhcp_args -R &"

		[ -r $pidfile ] && oldpid=$(cat $pidfile 2>&-)
		${DEBUG:-eval} "udhcpc $dhcp_args"
		[ -n "$oldpid" ] && pidof udhcpc | grep "$oldpid" >&- 2>&- && {
			sleep 1
			kill -9 $oldpid
		}
		# hotplug events are handled by /usr/share/udhcpc/default.script
		;;
	dhcp*)
		pidfile=/var/run/dhcpcd-$5.pid
		[ -f $pidfile ] && $DEBUG kill -9 $(cat $pidfile) && rm -fr $pidfile
		#generating args
		vendor_class=$(nvram get brand)
		dhcp_hostname=$(nvram get hostname)

		dhcp_args="-Y -N -R $5 "
		[ -z $vendor_class ] || dhcp_args="$dhcp_args -i $vendor_class"
		[ -z $dhcp_hostname ] || dhcp_args="$dhcp_args -h $dhcp_hostname"

		#kill oldpid & clear pid files
		[ -r $pidfile ] && oldpid=$(cat $pidfile 2>&-)
		[ -n "$oldpid" ] && pidof dhcpcd | grep "$oldpid" >&- 2>&- && {
			sleep 1
			kill -9 $oldpid
		}

		#preparing
		mkdir -p /var/lib/dhcpcd
		rm -fr $pidfile
		
		#generating fallback in lan 
		[ "$3" = "lan" ] && {
			fb_ip=`nvram show lan_dhcps_rule 0 lan0_fallback_ip`
			fb_mask=`nvram show lan_dhcps_rule 0 lan0_fallback_mask`
			ip addr add $fb_ip/$fb_mask dev $5 label $5:0
		}
		dhcpcdstart $dhcp_args &
		/etc/init.d/upnpd start

		# hotplug 
		env -i ACTION="ifup" INTERFACE="${2}" PROTO=dhcp TYPE="${3}" \
            NUM="${4}" IFNAME="${5}" /sbin/hotplug-call "iface" &
	;;
	none|"")
	;;
	*)
		[ -x "/sbin/ifup.$1" ] && { $DEBUG /sbin/ifup.$1 ${2} ${3} ${4} ${5}; \
            exit; }
		echo "### ifup ${2}: ignored ${2}_proto=\"$1\" (not supported)"
	;;
	esac
}
do_ifupv6() {

	pidfile=/var/run/${2}v6.pid
	[ -f $pidfile ] && $DEBUG kill $(cat $pidfile)

	case "$1" in
	static|link-local)
        [ "${3}" = "wan" ] && {
		    ip=$(nvram show ${3}_staticv6_rule $4 ipaddr)
    		mask=$(nvram show ${3}_staticv6_rule $4 length)
	    	mtu=$(nvram show ${3}_main_rule $4 mtu)

		    $DEBUG ip -6 addr add $ip/$mask dev $5

            gateway=$(nvram show ${3}_staticv6_rule $4 gateway)
            dns=$(nvram show ${3}_staticv6_rule $4 dns1)
        }
        [ "${3}" = "lan" ] && {
            proto=$(nvram get wan0_protov6)
    		prefix=$(nvram show ${3}_staticv6_rule $4 ${proto}_prefix)
	    	suffix=$(nvram show ${3}_staticv6_rule $4 ${proto}_suffix)
		    mask=$(nvram show ${3}_staticv6_rule $4 length)
    		mtu=$(nvram show ${3}_main_rule $4 mtu)
            ip=${prefix}${suffix}
	    	$DEBUG ip -6 addr add $ip/$mask dev $5

            gateway=$(nvram show ${3}_staticv6_rule $4 gateway)
            dns=$(nvram show ${3}_staticv6_rule $4 dns1)
        }
        
        ifacev6_up "$iface" "$ip" "$mask" "$gateway" "$dns"

		env -i ACTION="ifupv6" INTERFACE="${2}" PROTO=static TYPE="${3}" \
            NUM="${4}" IFNAME="${5}" /sbin/hotplug-call "ifacev6" &
	;;
	dhcp*)
		dhcp_ip=$(nvram get ${2}_ipaddrv6)

		$DEBUG ip -6 link set dev $5 ${mtu:+mtu $(($mtu))} up
        # Disable wan device forwarding for auto configuration
        sysctl -w net.ipv6.conf.$(nvram get wan${4}_ifname).forwarding=0 > /dev/null 2> /dev/null
        # Start radvd for listening router advertisement message for gateway
        # and prefix length
        sh /etc/rc.common /etc/init.d/radvd restart
        sleep 1
        # Start dhcp client for a. stateful ipaddress b. dns, ntp or sip
        sh /etc/rc.common /etc/init.d/dhcp6c restart

        # william add for bug 3340 
        sh /etc/rc.common /etc/init.d/dhcp6s restart
	;;
	none|"")
	;;
	*)
		[ -x "/sbin/ifupv6.$1" ] && { $DEBUG /sbin/ifupv6.$1 ${2} ${3} ${4} ${5}; \
            exit; }
		echo "### ifup ${2}: ignored ${2}_proto=\"$1\" (not supported)"
	;;
	esac
}

update_resolv () {
    local route_args; route_args=""
    local ifacetype=${1}
    echo -n > /tmp/resolv.conf
    i=0
    limit=$(nvram get ${ifacetype}_num)
    while [ $i -lt $limit ];
    do
		dns_1=""
		dns_2=""
		case `nvram show ${ifacetype}_dns_rule 0 dnstype1` in
			custom|ispdns)
				dns_1=`nvram show ${ifacetype}_dns_rule 0 dns1`
			;;
			*|none)
				dns_1=""
			;;
		esac
		case `nvram show ${ifacetype}_dns_rule 0 dnstype2` in
			custom|ispdns)
				dns_2=`nvram show ${ifacetype}_dns_rule 0 dns2`
			;;
			*|none)
				dns_2=""
			;;
		esac
		echo "" > /tmp/resolv.conf
		[ -n $dns_1 ] && echo "nameserver "$dns_1 >> /tmp/resolv.conf
		[ -n $dns_2 ] && echo "nameserver "$dns_2 >> /tmp/resolv.conf

        i=$(expr $i + 1)
    done
}

iface_update_default_route_and_dns () {
local route_args; route_args=""
local ifacetype=${1}
[ "$ifacetype" = "lan" ] || ifacetype="wan"
echo -n > /tmp/resolv.conf
i=0
limit=$(nvram get ${ifacetype}_num)

# "dnsassigned" check is used if wan is up but no valid DNS assigned, 
# ie : user selects ISP dns but ISP fails to give us one.
#dnsassigned="0"

while [ $i -lt $limit ];
do
    gateway=$(nvram get ${ifacetype}${i}_gateway)
    proto=$(nvram show ${ifacetype}${i}_proto 0 curr)
    
    [ -n "$gateway" ] && {
    ifname=$(nvram get ${ifacetype}${i}_ifname)
    weight=$(nvram show ${ifacetype}_weight_rule ${i} weight)
    src=$(nvram get ${ifacetype}${i}_ipaddr)

    # this is really a hack for directip
    [ "$proto" = "directip" ] && \
        ip route replace $gateway proto static dev $ifname src $src 
    route_args="$route_args nexthop via $gateway dev $ifname weight ${weight:-1}"
	}
    i=$(expr $i + 1)
done
update_resolv ${ifacetype}
if [ -n "$route_args" ]; then
    #ip -4 route replace default proto static $route_args
    ip route replace default via $gateway dev $ifname
    ip -4 route flush cache
else
    ip -4 route del default
    ip -4 route flush cache
fi
}

iface_update_defaultv6_route_and_dns () {
local route_args; route_args=""
echo -n > /tmp/resolv.conf
i=0
limit=$(nvram get wan_num)

# "dnsassigned" check is used if wan is up but no valid DNS assigned, 
# ie : user selects ISP dns but ISP fails to give us one.
dnsassigned="0"

while [ $i -lt $limit ];
do
    gateway=$(nvram get wan${i}_gatewayv6)
    proto=$(nvram show wan${i}_proto 0 curr)
    
    [ -n "$gateway" ] && {
    ifname=$(nvram get wan${i}_ifname)
    src=$(nvram get wan${i}_ipaddrv6)

    route_args="$route_args via $gateway dev $ifname"
#    dns="$(nvram show wan_dnsv6_rule 0 dns1)"
#    [ -n "$dns" ] && {
#        echo "nameserver $dns" >> /tmp/resolv.conf
#        dnsassigned="1"
#        }
#    dns="$(nvram show wan_dnsv6_rule 0 dns2)"
#    [ -n "$dns" ] && {
#        echo "nameserver $dns" >> /tmp/resolv.conf
#        dnsassigned="1"
#        }
    #if both dns are assigned none, we don't want to have default dns
#    [ "$(nvram show wan_dnsv6_rule 0 dnstype1)" = "none" -a \
#        "$(nvram show wan_dnsv6_rule 0 dnstype2)" = "none" ] && dnsassigned="1"
    }
    i=$(expr $i + 1)
done

# If no dns assigned, we set opendns as default DNS
#[ "$dnsassigned" = "0" ] && {
#    echo "nameserver 2001:708:140:220::3" >> /tmp/resolv.conf
#    }
update_resolv wan
if [ -n "$route_args" ]; then
    ip -6 route replace default $route_args
    ip -6 route flush cache
else
    ip -6 route del default
    ip -6 route flush cache
fi
}

# Add/delete the routing tables for a individual interface.
# $1:num  $2:ipaddr $3:mask $4:gateway $5:ifname $6:tbl_prefix
iface_add_route () {
    local tbl_prefix="$6"
    local ifacetype=wan
    local mode=`nvram show system_mode 0 name`
    local ifname=$5
    [ -n "$6" ] && ifacetype=$6
    [ -z "$6" -o "$6" = "wan" ] && tbl_prefix="1"
    [ "$6" = "lan" ] && tbl_prefix="2"
    [ -z "$num" ] && num=$1
    proto=$(nvram show wan${1}_proto 0 curr)
    eval $(ipcalc $2 $3)
    [ "$proto" = "directip" ] && \
        ip route add table ${tbl_prefix}${num} $4 dev $5 src $2 proto static
    ip route add table ${tbl_prefix}${num} $NETWORK/$PREFIX dev $5 src $2 proto static
    [ "$mode" = "ap" -o "$mode" = "repeater" ] && \
        ip route add table ${tbl_prefix}${num} default via $4 dev $5 proto static
    [ "${ifname%%[0-9]*}" = "br-wan" ] && \
        ip route add table ${tbl_prefix}${num} default via $4 dev $5 proto static
    ip rule add prio ${tbl_prefix}${num} from $2 lookup ${tbl_prefix}$num
    eval $(ipcalc $(nvram get ${ifacetype}${num}_ipaddr) $(nvram get ${ifacetype}${num}_mask))
    ip route add table ${tbl_prefix}${num} $NETWORK/$PREFIX \
        dev $(nvram get ${ifacetype}${num}_ifname) proto static
    # Add wan->lan direction routing table
    [ -z "$6" -o "$6" = "wan" ] && {
        eval $(ipcalc $(nvram get lan0_ipaddr) $(nvram get lan0_mask))
        ip route add table 1${num} $NETWORK/$PREFIX dev \
            $(nvram get lan0_ifname) proto static
    }
    # Implement multiple wan behavior
    #[ -z "$6" -o "$6" = "wan" ] && ip rule add from $NETWORK/$PREFIX \
    #    fwmark $(nvram get wan${1}_rtmark)/$(nvram get rt_mask) \
    #    lookup ${tbl_prefix}${num}
    ip rule add from $NETWORK/$PREFIX \
        fwmark $(nvram get ${ifacetype}${1}_rtmark)/$(nvram get rt_mask) \
        lookup ${tbl_prefix}${num}
}

# $1:num $2:ip $3:mask $4:gateway $5:ifname $6:iface_type
iface_del_route () {
    local tbl_prefix="$6"
    local ifacetype=wan
    [ -n "$6" ] && ifacetype=$6
    [ -z "$6" -o "$6" = "wan" ] && tbl_prefix="1"
    [ "$6" = "lan" ] && tbl_prefix="2"
    [ -z "$num" ] && num=$1
    eval $(ipcalc $(nvram get ${ifacetype}${1}_ipaddr) $(nvram get ${ifacetype}${1}_mask))
    ip rule del from $NETWORK/$PREFIX \
        fwmark $(nvram get ${ifacetype}${1}_rtmark)/$(nvram get rt_mask) \
        lookup ${tbl_prefix}${num}
    # Implement multiple wan behavior
    # [ -z "$6" -o "$6" = "wan" ] &&  ip route del table ${tbl_prefix}${num} \
    #    $NETWORK/$PREFIX dev $(nvram get ${ifacetype}${1}_ifname) proto static
    ip route del table ${tbl_prefix}${num} \
        $NETWORK/$PREFIX dev $(nvram get ${ifacetype}${1}_ifname) proto static
    eval $(ipcalc $2 $3)
    ip rule del prio ${tbl_prefix}${num} from $2 lookup ${tbl_prefix}$num
    ip route del table ${tbl_prefix}${num} default via $4 dev $5 proto static
    ip route del table ${tbl_prefix}${num} $NETWORK/$PREFIX dev $5 src $2 proto static
    proto=$(nvram show ${ifacetype}${1}_proto 0 curr)
    [ "$proto" = "directip" ] && \
        ip route del table ${tbl_prefix}${num} $4 dev $5 src $2 proto static
    ip route del $NETWORK/$PREFIX dev $5 src $2
}

# set input dns(s) into lan_dns_rule or wan_dns_rule but not commit
# $1 : dns1
# $2 : dns2
# $3 : lan wan
update_dns_setting () {
    local dns1=""
    local dns2=""
    local ifacetype="${3}"

    local dnstype1="$(nvram show ${ifacetype}_dns_rule 0 dnstype1)"
    local dnstype2="$(nvram show ${ifacetype}_dns_rule 0 dnstype2)"
    case "$dnstype1" in
    "ispdns")
        dns1="${1}"
    ;;
    "custom")
        dns1="$(nvram show ${ifacetype}_dns_rule 0 dns1)"
    ;;
    "none")
        dns1=""
    ;;
    *)
        dns1="${1}"
    ;;
    esac
    case "$dnstype2" in
    "ispdns")
        if [ "$dnstype1" = "none" ];then
           dns2="${1}"
        else		
           dns2="${2}"
        fi
    ;;
    "custom")
        dns2="$(nvram show ${ifacetype}_dns_rule 0 dns2)"
    ;;
    "none")
        dns2=""
    ;;
    *)
        dns2="${2}"
    ;;
    esac
    nvram set ${ifacetype}_dns_rule="${dnstype1}^${dns1}^${dnstype2}^${dns2}"
}

update_dnsv6_setting () {
    local dns1=""
    local dns2=""
    local dnstype1="$(nvram show wan_dnsv6_rule 0 dnstype1)"
    local dnstype2="$(nvram show wan_dnsv6_rule 0 dnstype2)"
    case "$dnstype1" in
    "ispdns")
        dns1="${1}"
    ;;
    "custom")
        dns1="$(nvram show wan_dnsv6_rule 0 dns1)"
    ;;
    "none")
        dns1=""
    ;;
    *)
        dns1="${1}"
    ;;
    esac
    case "$dnstype2" in
    "ispdns")
        dns2="${2}"
    ;;
    "custom")
        dns2="$(nvram show wan_dnsv6_rule 0 dns2)"
    ;;
    "none")
        dns2=""
    ;;
    *)
        dns2="${2}"
    ;;
    esac
    nvram set wan_dnsv6_rule="${dnstype1}^${dns1}^${dnstype2}^${dns2}"
}

iface_check_wan_enabled()
{
    local wannum="$(nvram get wan_num)"
    local idx="0"
    local wanenabled="0"
    while [ "$idx" -lt "$wannum" ];
    do
        [ "$(nvram show wan_main_rule 0 enable)" = "1" ] && wanenabled="1"
        [ -n "$(nvram get wan${idx}_ipaddr)" ] && wanenabled="1"
        idx="$(($idx + 1))"
    done
    echo $wanenabled
}

iface_up () {
    lock /tmp/.iface
    nvram set ${1}_ipaddr=$2
    nvram set ${1}_mask=$3
    nvram set ${1}_gateway=$4
    nvram set ${1}_dns=$5
    nvram set ${1}_domain=$6
    type="${1%%[0-9]*}"
    num="${1#$type}"
    local DNS1=$(echo -n "$5" | awk '{ print $1}' )
    local DNS2=$(echo -n "$5" | awk '{ print $2}' )
    update_dns_setting "$DNS1" "$DNS2" "$type"
    nvram fset ${type}_st_rule="$(cat /proc/uptime | awk '{print $1}' | sed 's/...$//')"
    # Start up lan while wan if not enabled
    
    [ "$type" = "lan" ] && {
        ifname="$(nvram get ${1}_ifname)"
        iface_add_route "$num" "$2" "$3" "$4" "$ifname" "$type"
        #if wan is not activated, use lan setting as default
        [ "$(iface_check_wan_enabled)" = "0" ] && \
            iface_update_default_route_and_dns lan
    }
    [ "$type" = "wan" ] && {
        ifname="$(nvram get ${1}_ifname)"
        iface_add_route $num $2 $3 $4 $ifname
        iface_update_default_route_and_dns wan
    }
    log_usr ${type}$(($num+1)) up OK [${2}]
    lock -u /tmp/.iface
}

ifacev6_up () {
    [ -z "${2}" -o -z "${3}" -o -z "${4}" ] && return
    lock /tmp/.ifacev6
    # If we didn't get gateway, mask and ip address at the same time, just
    # return
    nvram fset ${1}_ipaddrv6=$2
    nvram fset ${1}_maskv6=$3
    nvram fset ${1}_gatewayv6=$4
    nvram fset ${1}_dnsv6=$5
    nvram fset ${1}_domainv6=$6
    nvram fset ${1}_prefixv6=$7
    type="${1%%[0-9]*}"
    num="${1#$type}"
    local DNS1=$(echo -n "$5" | awk '{ print $1}' )
    local DNS2=$(echo -n "$5" | awk '{ print $2}' )
    update_dnsv6_setting "$DNS1" "$DNS2"
    nvram fset ${type}_stv6_rule="$(cat /proc/uptime | awk '{print $1}' | sed 's/...$//')"
    [ "$type" = "wan" ] && {
        ifname="$(nvram get ${1}_ifname)"
        iface_update_defaultv6_route_and_dns
    }
    log_usr ${type}$(($num+1)) up OK [${2}]
    lock -u /tmp/.ifacev6
}

#when PPP conneciton is broken then call iface_down2 function
#this function not delete bridge interface,eg:br-wan0

iface_down2 () {
    ifacename=${1}
    type="${1%%[0-9]*}"
    num="${1#$type}"
    ipaddr="$(nvram get ${1}_ipaddr)"
                    
    [ "$type" = "lan" -a "$(iface_check_wan_enabled)" = "0" ] && {
        gateway="$(nvram get ${1}_gateway)"
        ifname="$(nvram get ${1}_ifname)"
        mask="$(nvram get ${1}_mask)"
        iface_del_route "$num" "$ipaddr" "$mask" "$gateway" "$ifname" "$type"
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
    [ "$type" = "wan" ] && iface_update_default_route_and_dns $type
    log_usr "${type}$(($num+1))" down OK [${ipaddr}]
    lock -u /tmp/.iface.changestate.${ifacename}.lock
    [ -f /tmp/.iface.changestate.${ifacename}.lock.pid ] && rm /tmp/.iface.changestate.${ifacename}.lock.pid
}

iface_down () {
    ifacename=${1}
    type="${1%%[0-9]*}"
    num="${1#$type}"
    ipaddr="$(nvram get ${1}_ipaddr)"
    bridge_name="$(nvram get ${1}_ifname)"
    [ "${bridge_name%%[0-9]*}" = "ppp" ] && {
        bridge_name="$(nvram get ${1}_device)"
    }
    [ "$type" = "lan" -o "$type" = "wan" ] && {
      ifconfig $bridge_name down
      brctl delbr $bridge_name
    }
    
    [ "$type" = "lan" -a "$(iface_check_wan_enabled)" = "0" ] && {
        gateway="$(nvram get ${1}_gateway)"
        ifname="$(nvram get ${1}_ifname)"
        mask="$(nvram get ${1}_mask)"
        iface_del_route "$num" "$ipaddr" "$mask" "$gateway" "$ifname" "$type"
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
    
    [ "$type" = "wan" ] && iface_update_default_route_and_dns $type 
    
    #log_usr "${type}$(($num+1))" down OK [${ipaddr}]
    lock -u /tmp/.iface.changestate.${ifacename}.lock
    [ -f /tmp/.iface.changestate.${ifacename}.lock.pid ] && rm /tmp/.iface.changestate.${ifacename}.lock.pid
}


ifacev6_down () {
    lock /tmp/.ifacev6
    
    ifacename=${1}
    type="${1%%[0-9]*}"
    num="${1#$type}"
    ipaddr="$(nvram get ${1}_ipaddrv6)"
    
    [ "$type" = "lan" -a "$(iface_check_wan_enabled)" = "0" ] && {
        gateway="$(nvram get ${1}_gatewayv6)"
        ifname="$(nvram get ${1}_ifnamev6)"
        mask="$(nvram get ${1}_maskv6)"
        iface_delv6_route "$num" "$ipaddr" "$mask" "$gateway" "$ifname" "$type"
    }
    [ "$type" = "wan" ] && [ -n "$ipaddr" ] && {
        gateway="$(nvram get ${1}_gatewayv6)"
        ifname="$(nvram get ${1}_ifnamev6)"
        mask="$(nvram get ${1}_maskv6)"
        
        iface_delv6_route $num $ipaddr $mask $gateway $ifname
    }
    [ -n $ipaddr ] && {
        nvram unset ${1}_ipaddrv6
        nvram unset ${1}_maskv6
        nvram unset ${1}_gatewayv6
        nvram unset ${1}_dnsv6
        nvram unset ${1}_domainv6
    }
    
    [ "$type" = "wan" ] && iface_update_defaultv6_route_and_dns 
    
    log_usr "${type}$(($num+1))" down OK [${ipaddr}]
    lock -u /tmp/.ifacev6
    lock -u /tmp/.ifacev6.changestate.${ifacename}.lock
    [ -f /tmp/.ifacev6.changestate.${ifacename}.lock.pid ] && rm /tmp/.ifacev6.changestate.${ifacename}.lock.pid
}

# 1:$iface 2:$type 3:$num 4:$ifname
iface_detect() {
    local err; err=0
    local detect_type; detect_type=""
    # PPP only uses ping.
    [ "${4%%[0-9]*}" = "ppp" ] && detect_type="ping"
    [ -z "$detect_type" ] && {
        detect_type=$(nvram show wan_detect_rule $3 type)
    }
    # arping
    [ "$detect_type" = "arping" ] &&
    { 
        arping -q -c 1 -w 5 -I $4 -s $(nvram get ${iface}_ipaddr) \
            $(nvram get ${iface_gateway}) || err=1
    }
    # ping 
    [ "$detect_type" = "ping" ] &&
    { 
        pinghost="$(nvram show wan_detect_rule $3 host)"
    	[ -z $pinghost ] && pinghost="$(nvram get ${iface}_dns)"
    	[ -z $pinghost ] && pinghost="208.67.222.222"
        ping -q -c 8 -I "$(nvram get ${iface}_ifname)" "$pinghost" || err=1
    }

    [ -z "$(nvram get ${iface}_failcount)" ] && \
        nvram set ${iface}_failcount="0"
        
    [ "$err" = "0" ] && {
        [ "$(nvram get ${iface}_failcount)" -ge \
            "$(nvram show wan_detect_rule $3 threshold)" ] && {
            log_usr ${type}$(($num+1)) wan-detect OK 
        }
        nvram set ${iface}_failcount="0"
    }
    [ "$err" = "1" ] && {
        nvram set ${iface}_failcount="$(($(nvram get ${iface}_failcount) + 1))"
        [ "$(nvram get ${iface}_failcount)" -ge \
            "$(nvram show wan_detect_rule $3 threshold)" ] && {
            
            [ "$(nvram get ${iface}_failcount)" = \
                "$(nvram show wan_detect_rule $3 threshold)" ] && \
                log_usr ${type}$(($num+1)) wan-detect Failed 
            err=2
        }
    }
    return $err
}
