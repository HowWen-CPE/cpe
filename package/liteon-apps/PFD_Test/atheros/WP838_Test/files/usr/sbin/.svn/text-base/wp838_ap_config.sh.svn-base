#!/bin/sh
#
# WP838 WiFi Configure Script, Sean@Liteon, t013-10-t8
#

dbg_config()
{
	echo "$1 $2 $3 $4 $5 $6 $7 $8 $9"
	$1 $2 $3 $4 $5 $6 $7 $8 $9
}

config_2g()
{
	local tg_wlv_num="$(nvram get wifi0_rule_num)"
	local tg_wlv_idx=0

	#while [ "$tg_wlv_idx" -lt "$tg_wlv_num" ];
	#do
		ifconfig ath0 down
		brctl delif br0 ath0
		wlanconfig ath0 create wlandev wifi0 wlanmode ap
		iwpriv ath0 mode "$(nvram get ath0_ap_ch_mode)"
		iwconfig ath0 channel "$(nvram get ath0_ap_primary_ch)"
		iwconfig ath0 essid "$(nvram get ath0_ssid)"
		ifconfig ath0 up
		brctl addif br0 ath0

		tg_wlv_idx=$(($tg_wlv_idx+1))
	#done


}

config_5g()
{
	local fg_wlv_num="$(nvram get wifi1_rule_num)"
	local fg_wlv_idx=1

	#while [ "$fg_wlv_idx" -le "$fg_wlv_num" ];
	#do
		ifconfig ath1 down
		brctl delif br0 ath1
		wlanconfig ath1 create wlandev wifi1 wlanmode ap
		iwpriv ath1 mode "$(nvram get ath1_ap_ch_mode_2)"
		iwconfig ath1 channel "$(nvram get ath1_ap_primary_ch_2)"
		iwconfig ath1 essid "$(nvram get ath1_ssid_2)"
		ifconfig ath1 up 
		brctl addif br0 ath1
		
		fg_wlv_idx=$(($fg_wlv_idx+1))
	#done
}

# Add br0 & up
dbg_config brctl addbr br0
dbg_config ifconfig br0 192.168.1.2 up

dbg_config ifconfig eth1 up
dbg_config brctl addif br0 eth1

# Configure WiFi and up
config_2g
config_5g

