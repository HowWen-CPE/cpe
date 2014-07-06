#ifndef __CONFIG_WP688_H
#define __CONFIG_WP688_H

#include "nvram.h"

 
/* Board dependent definition. */
#define ARCH "RT3883"

#define LAN_NUM "1"
#define WAN_NUM "1"

#ifndef WL_NUM
#define WL_NUM "1"
#endif

#ifndef WL_WDS_NUM
#define WL_WDS_NUM "4"
#endif

#ifndef WL_APCLI_NUM
#define WL_APCLI_NUM "1"
#endif

#define WL_BASIC_RULE "1^9^0^0^0"
#define WL_AP_BASIC_RULE "1^9^0^0^0"
#define WL5G_BASIC_RULE "1^8^0^0^0"
#define WL5G_AP_BASIC_RULE "1^8^0^0^0"
#define WL0_IFNAME_RULE "rai0|rai1|rai2|rai3"
#define WL1_IFNAME_RULE "ra0|ra1|ra2|ra3"
#define WL0_RULE_NUM "8"
#define WL0_RULE_MAX "8"
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM


#define WLAN_DEV_PREFIX "rai apclii"

#define WAN0_IFNAME "br-wan0"

#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "br-wan0"
#define WAN0_PORTS "4"

#define LAN0_IFNAME "br-lan0"
#define LAN0_IFNAMES "rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7 eth2"
#define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 rai0 rai1 apcli0 apclii0 wds0 wds1 wds2 wds3 wdsi0 wdsi1 wdsi2 wdsi3"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan1"

#define LAN1_IFNAME "br2"
#define LAN1_DEVICE "br2"

#define BR_RULE_GUESTLAN "GuestLAN^0^ra1^"
#define BR_RULE "LAN1^1^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7^|WAN1^0^eth2^|GuestLAN^0^rai0^"
#define BR_LAN_RULE "LAN1^1^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7 eth2^|GuestLAN^0^ra1^"
#define BR_WAN_RULE "WAN1^1^eth2^"
#define LAN_WAN_MODE_RULE "normal^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7^eth2"\
						"|ap^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7 eth2^"\
						"|wisp0^eth2^apclii0"\
						"|^^"\
						"|sta0^eth2 apclii0^"\
 
#define LAN_WAN_MODE_DEFAULT "ap"

#define BR_RULE_NUM "3"
#define BR_RULE_MAX "8"

#define STATS_RULE "^1^60^vlan1 vlan2 ppp0 br0 ra0 rai0 usb0 eth0"
#define VLAN_RULE_NUM "2"
#define VLAN_RULE_MAX "8"

#define VLAN_RULE "LAN1^0^eth2^1^1111011^|WAN1^0^eth2^2^0000111^"

#define VLANPORT_RULE_NUM "5"
#define VLANPORT_RULE_MAX "7"
#define VLANPORT_RULE "PORT1^0^1^0^011101|PORT2^1^1^0^011011|PORT3^2^1^0^010111|PORT4^3^1^0^001111|PORT5^4^2^0^000001|PORT6^5^1^1^|PORT7^6^0^1^"

#define LAN0_PORTS "0 1 2 3"
#define VLAN2_PORTS "4 5"
#define VLAN1_PORTS "0 1 2 3 5*"
#define VLAN_HWNAME "eth2"

#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "vlan3"
#define WAN1_PORTS "3"


#define HWNAT_RULE "1"

#define BW_TYPE \
        "A1M64K", "A1.5M640K", "A2M256K", "A2M512K", "A3M640K", "A4M1M", \
        "A6M256K", "A6M640K", "A8M256K", "A8M640K", "A8M800K", "A10M2M", \
        "A12M1M", "C1M128K", "C2M128K", "C2M384K", "C3M384K", "C6M640K", \
        "C8M640K", "C10M1M", "V2M2M", "V4M4M", "V10M10M", "V25M25M", \
        "V50M50M", "V100M100M","Custom"

#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
/* Board dependent definition. */

#define LAN_DHCPSV6_RULE "default^1^2001:db8:feed:b00::^2001:db8:feed:b00::^2001:db8:feed:b00::^howen.com^custom^^^0^100^200^30|default^0^2001:db8:feed:b10::^2001:db8:feed:b10::^2001:db8:feed:b10::^howen.com^custom^^^0^100^200^30"
#define LAN_STATICV6_RULE_DEFAULT "2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^64^^^^^^^^^^|2001:db8:feed:b10::^1^2001:db8:feed:b10::^1^2001:db8:feed:b10::^1^64^^^^^^^^^^"


#define WL_COUNTRY_CODE "US"
#define WL_REGION_ABAND "7"

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "20000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"

#define FL_RULE ""
#define FL_RULE_MAX "10"
#define FL_RULE_NUM "0"

#define FR_RULE ""
#define FR_RULE_MAX "10"
#define FR_RULE_NUM "0"
#define ONE_TO_ONE_RULE ""
#define ONE_TO_ONE_MAX "10"
#define ONE_TO_ONE_NUM "0"

#define PT_RULE_MAX "64"
#define WF_RULE_MAX "10"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "8"
#define DBM_MAX "8"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^DHCPS^1^254^86400^howen.com^^dnsrelay^^^^^dnsrelay^opendns^opendns^192.168.1.2^24^60^"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "4"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "4"
#define ISHOST_RULE_MAX "4"
#define LOG_RULE "^1^0.0.0.0^514^0^1^0^0^0^0^0^1^0^0^0^0^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

#define PPTPD_SESSION_MAX 32
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "10"


#define SUBCAT_MAX  1
#define FW_UPGRADE_RULE "ftpget^192.168.100.178^^^^zyfw_info"

/* WAN amount dependent definition. */
#define WAN_MAIN_RULE "WAN1^0^1500^0^0^0|WAN2^0^1500^1^1^0"
#define WAN_BW_RULE "A2M256K^2048^256^0^0^75^90^90^155^19^1244^155|A2M256K^2048^256^0^0^75^90^90^155^19^1244^155"
#define WAN_DETECT_RULE "^0^ping^208.67.222.222^60^8|^0^ping^208.67.222.222^60^8"
#define WAN_WEIGHT_RULE "1^1|1^1"
#define WAN_FAILOVER_RULE "0^wan0^wan1"
#define WAN_STATIC_RULE "^^^^^|^^^^^"
#define WAN_DHCP_RULE "^^86400|^^86400"
#define WAN_HTC_RULE "^^86400^512^0|^^86400^512^0"
#define WAN_PPPOE_RULE "chap^^^0^300^5^5^1492^1492^isp^^|chap^^^0^300^5^5^1492^1492^isp^^"
#define WAN_STATICV6_RULE "^^^^|^^^^"
#define WAN_DHCPV6_RULE "^^86400^stateful|^^86400^stateful"
#define WAN_PPPOEV6_RULE "^^^0^300^5^5^1492^1492^isp^|^^^0^300^5^5^1492^1492^isp^"
#define WAN_PPTP_L2TP_RULE "0.0.0.0^0.0.0.0^0.0.0.0"
#define WAN_PPTPC_RULE "^static^^^^^^1^300^isp^^disabled^chap|^static^^^^^^1^300^isp^^disabled^chap"
#define WAN_L2TPC_RULE "^static^^^^^^1^300^isp^^disabled^chap|^static^^^^^^1^300^isp^^disabled^chap"
#define WAN_WWAN_RULE "1^0^0^internet^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#define WAN_DNS_RULE "ispdns^^ispdns^|ispdns^^ispdns^"
#define LAN_DNS_RULE "none^^none^|none^^none^"
#define WAN_DNSV6_RULE "ispdns^^ispdns^|ispdns^^ispdns^"
#define WAN_WWAN_PROBE_RULE "^^^^^^^|^^^^^^^"
#define WAN_WWAN_MODE "auto", "hsdpa", "umts", "edge", "gprs"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^|0^61.9.192.13^^"
#define WAN_HWADDR_CLONE_RULE "0^^hwaddr^|0^^hwaddr^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^|WAN2_DDNS^0^dyndns^^^^"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^|WAN2_OPENDNS^0^^^0^"
#define RT_RULE "SMTP^0^^^^^tcp^^^25^25^wan0^default^^^"
#define RT_RIP_RULE "none"
#define NAT_RULE "1"
#define FR_DMZ_RULE "^0^|^0^"
#define PT_RULE "^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0"

/* LAN amount dependent definition. */
#define LAN_MAIN_GUESTLAN "GuestLAN^0^1500^1^1^0"
#define LAN_MAIN_RULE "LAN1^1^1500^1^1^0|GuestLAN^0^1500^1^1^0"
#define LAN_STATIC_GUESTLAN "192.168.100.1^24^^192.168.100.1^^0^^^192.168.100.1^24^192.168.1.2^24"
#define LAN_STATIC_RULE_DEFAULT "192.168.1.2^24^192.168.1.1^8.8.8.8^^^^^192.168.1.1^24^192.168.1.2^24"
#define LAN_HWADDR_CLONE_RULE "0^"

/* WLAN amount dependent definition. */
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_AP_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"

#define WL1_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL1_AP_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_WDS_RULE "disabled^1|disabled^1"
#define WL1_WDS_RULE "disabled^1|disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128|15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#define WL_MODE_RULE "ap^0^0|ap^0^0"
#define WL1_MODE_RULE "ap^0^0|ap^0^0"
#define WL_WPS_RULE "0^^0|0^^0"
#define WL1_WPS_RULE "0^^0|0^^0"
#define WL_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#define WL1_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#define WL_ACL_MAX "64"
#define WL_ACL_BASIC_RULE "0^1"

/* WDS */
#define WL0_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
#define WL1_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
#define WL0_WDS_SEC_WEP_RULE "|||"
#define WL0_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip"

/* APCLI */
#define WL0_APCLI_RULE "1^GENERIC_AP_2G^^disabled^0^0^0"
#define WL1_APCLI_RULE "1^GENERIC_AP_5G^^disabled^0^0^0"
#define WL0_APCLI_SEC_WEP_RULE "1^^^^^^open^0"
#define WL1_APCLI_SEC_WEP_RULE "1^^^^^^open^0"
#define WL0_APCLI_SEC_WPA_RULE "^tkip"
#define WL1_APCLI_SEC_WPA_RULE "^tkip"
#define WL0_APCLI_SEC_WPA2_RULE "^aes"
#define WL1_APCLI_SEC_WPA2_RULE "^aes"

#define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#define WL1_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#define WL0_SSID_RULE "Generic_2.4G|Generic_2.4G_SSID1|Generic_2.4G_SSID2|Generic_2.4G_SSID3|Generic_2.4G_SSID4|Generic_2.4G_SSID5|Generic_2.4G_SSID6|Generic_2.4G_SSID7"
#define WL1_SSID_RULE "Generic_5G|Generic_5G_SSID1|Generic_5G_SSID2|Generic_5G_SSID3|Generic_5G_SSID4|Generic_5G_SSID5|Generic_5G_SSID6|Generic_5G_SSID7"
#define WL0_SEC_RULE "disabled|disabled|disabled|disabled|disabled|disabled|disabled|disabled"
#define WL1_SEC_RULE "disabled|disabled|disabled|disabled|disabled|disabled|disabled|disabled"
#define WL0_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"
#define WL1_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"
#define WL0_SEC_WPA_RULE   "^mixed^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"
#define WL1_SEC_WPA_RULE   "^mixed^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"
#define WL0_SEC_WPA2_RULE "^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"
#define WL1_SEC_WPA2_RULE   "^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"\
			"|^mixed^^^1812^time^3600^5000^0^10^3000^0"




#define QOS_CONFIG
#define DIRECT_MULTICAST_STATUS_VALUE "1|1|1|1|1|1|1|1"
#define DIRECT_MULTICAST_MAXNUM_VALUE "10"
#define COS_CLASSIFICATION_STATUS_VALUE "0|0|0|0|0|0|0|0|0"
#define TOS_CLASSIFICATION_STATUS_VALUE "1|1|1|1|1|1|1|1|1|1|1|1"
#define HEURISTICS_CLASSIFICATION_STATUS_VALUE "1|1|1|1|1|1|1|1|1"
#define BWM_LEVEL_DL_INDEX_VALUE "0|0|0|0|0|0|0|0"
#define BWM_LEVEL_UP_INDEX_VALUE "0|0|0|0|0|0|0|0"
#define QOS_RULE_STATUS_VALUE "0|0|0|0|0|0|0|0|0"
#define QOS_RULE_ACTIVE_DEFAULT_VALUE "0^0^0^0^0^0^0^0^0"
#define QOS_RULE_ACTIVE_VALUE \
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE"|"\
		QOS_RULE_ACTIVE_DEFAULT_VALUE
/*bwm_level*/
#define BWM_LEVEL_1 "512,512,1500,60,102,102,1500,10,256,461,1500,25,154,512,1500,25,1,1,1500,1"
#define BWM_LEVEL_2 "1024,1024,2560,110,256,256,1500,10,512,922,1500,50,256,1024,1500,50,1,1,1500,1"
#define BWM_LEVEL_3 "2048,2048,5120,110,256,256,1500,10,1536,1843,3840,50,256,2048,1500,50,1,1,1500,1"
#define BWM_LEVEL_4 "2458,4096,6144,110,256,256,1500,10,1843,4096,4608,50,358,4096,1500,50,1,1,1500,1"
#define BWM_LEVEL_5 "3686,6144,9216,110,256,256,1500,10,2765,6144,6912,50,666,6144,1664,50,1,1,1500,1"
#define BWM_LEVEL_6 "4915,8192,12288,110,256,256,1500,10,3686,8192,9216,50,973,8192,2432,50,1,1,1500,1"
#define BWM_LEVEL_7 "5120,10240,12800,110,256,256,1500,10,3840,10240,9600,50,1024,10240,2560,50,1,1,1500,1"
#define BWM_LEVEL_8 "10240,20480,25600,110,512,512,1500,10,7168,20480,17920,50,2560,20480,6400,50,1,1,1500,1"
#define BWM_LEVEL_9 "15360,30720,38400,110,512,512,1500,10,11520,30720,28800,50,3328,30720,8320,50,1,1,1500,1"
#define BWM_LEVEL_10 "15565,40960,38912,110,512,512,1500,10,11674,40960,29184,50,3379,40960,8448,50,1,1,1500,1"
#define BWM_LEVEL_11 "23347,61440,58368,110,512,512,1500,10,17510,61440,43776,50,5325,61440,13312,50,1,1,1500,1"
#define BWM_LEVEL_12 "31130,81920,77824,110,512,512,1500,10,23347,81920,58368,50,7270,81920,18176,50,1,1,1500,1"
#define BWM_LEVEL_13 "30720,102400,76800,110,1024,1024,1500,10,23040,102400,57600,50,6656,102400,16640,50,1,1,1500,1"
#define BWM_LEVEL_14 "46080,153600,115200,110,1024,1024,1500,10,34560,153600,86400,50,10496,153600,26240,50,1,1,1500,1"
#define BWM_LEVEL_15 "61440,204800,153600,110,1024,1024,1500,10,46080,204800,115200,50,14336,204800,35840,50,1,1,1500,1"
#define BWM_LEVEL_16 ",,,,,,,,,,,,,,,,,,,"
#define BWM_LEVEL_VALUE	\
		BWM_LEVEL_1"|"\
		BWM_LEVEL_2"|"\
		BWM_LEVEL_3"|"\
		BWM_LEVEL_4"|"\
		BWM_LEVEL_5"|"\
		BWM_LEVEL_6"|"\
		BWM_LEVEL_7"|"\
		BWM_LEVEL_8"|"\
		BWM_LEVEL_9"|"\
		BWM_LEVEL_10"|"\
		BWM_LEVEL_11"|"\
		BWM_LEVEL_12"|"\
		BWM_LEVEL_13"|"\
		BWM_LEVEL_14"|"\
		BWM_LEVEL_15"|"\
		BWM_LEVEL_16

/*voice^video^data^background*/
#define STREAM_PRIORITY_VALUE "4^5^6^7"		
#define STA_PRIORITY_DEFAULT_VALUE "none,1"
#define STA_PRIORITY_VALUE \
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE"|"\
		STA_PRIORITY_DEFAULT_VALUE

#define QOS_RULE_RECORD_DEFAULT_VALUE \
    	"none,none,none,none,none,none,none,none,none"
#define QOS_RULE_RECORD_VALUE \
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE"|"\
		QOS_RULE_RECORD_DEFAULT_VALUE

/*status^age*/
#define IGMP_SNOOPING_VALUE "1^260"
/*voice^video^data^background*/
#define TOS_CLASSIFY_VALUE "0xE0,0xC0,0xB8^0xA0,0x80^none^none^0^0"
/*voice^video^data^background*/
#define COS_CLASSIFY_VALUE "none^none^none^none"
/*(d_count^b_count^ipg_min^ipg_max^pl_min^pl_max)*/
#define VOICE_HEURISTICS_CLASSIFY	"600,10000,15,275,70,400"
#define VIDEO_HEURISTICS_CLASSIFY	"50000,500000,0,65,1000,1518"
#define DATA_HEURISTICS_CLASSIFY	"0,0,0,0,0,0"
#define BACKGROUND_HEURISTICS_CLASSIFY	"0,0,0,0,0,0"
#define HEURISTICS_CLASSIFY_VALUE	\
		VOICE_HEURISTICS_CLASSIFY"^"\
		VIDEO_HEURISTICS_CLASSIFY"^"\
		DATA_HEURISTICS_CLASSIFY"^"\
		BACKGROUND_HEURISTICS_CLASSIFY
#define TOS_MARK_VALUE "1^0x8^0x5^0x0^0x0"
#define ROOT_BW_VALUE "307200^307200^307200"

/* unsort */
#define WAN_TUNNEL6_RULE "72.52.104.74^2001:470:1f04:d1c::2/64^2001:470:1f04:d1c::1"
#define GUEST_LAN_RULE "0^rai3^br2^1^192.168.2.1^24^0^0^1^liteon_guest.com^33^32"
#define GUEST_5G_LAN_RULE "0^ra3^br2^1^192.168.2.1^24^0^0^1^liteon_5g_guest.com^33^32"


#define GROUP1 "group^RWGROUP^v1^rw"
#define GROUP2 "group^RWGROUP^v2c^rw"
#define GROUP3 "group^RWGROUP^usm^rw"    
#define GROUP4 "group^ROGROUP^v1^ro"
#define GROUP5 "group^ROGROUP^v2c^ro"
#define GROUP6 "group^ROGROUP^usm^ro"    
#define SNMPD_GROUP_RULE GROUP1"|"GROUP2"|"GROUP3"|"GROUP4"|"GROUP5"|"GROUP6

#define ACCESS1 "access^ROGROUP^\"\"^any^noauth^exact^all^none^none"
#define ACCESS2 "access^RWGROUP^\"\"^any^noauth^exact^all^all^all"
#define SNMPD_ACCESS_RULE ACCESS1"|"ACCESS2

#define XBOX_APP "appl^0^1^1^Game Console^XBox Live^3074,3390,3932,5555^88,1900,3074,3776,3777^1^50^1^1^50^1^1^10^1^1^10^1"
#define PLAYSTATION_APP "appl^0^1^1^Game Console^PlayStation^5223^3478,3479,3658,4658^1^50^1^1^50^1^1^10^1^1^10^1"
#define MSN_GAME_APP "appl^0^1^1^Game Console^MSN Game Zone^6667,28800:29000^6667,28800:29000^1^50^1^1^50^1^1^10^1^1^10^1"
#define BATTLENET_APP "appl^0^1^1^Game Console^Battlenet^40,112,6112,4000,6113:9119^^1^50^1^0^50^1^1^10^1^0^10^1"
#define VOIP_APP "appl^0^1^1^VoIP^VoIP^5060^5060^1^50^1^0^50^1^1^10^1^0^10^1"
#define IM_APP "appl^0^1^1^Instant Messenger^Instant Messenger^6891:6901,1863^1863,5190,6901^1^50^1^0^50^1^1^10^1^0^10^1"
#define WEB_APP "appl^0^1^1^Web Surfing^Web Surfing^80,443^^1^50^1^0^50^1^1^10^1^0^10^1"
#define FTP_APP "appl^0^1^1^P2P/FTP^FTP^21,22^^1^50^1^0^50^1^1^10^1^0^10^1"
#define EMULE_APP "appl^0^1^1^P2P/FTP^eMule^4662^4672^1^50^1^0^50^1^1^10^1^0^10^1"
#define BT_APP "appl^0^1^1^P2P/FTP^BitTorrent^tport^uport^1^50^1^1^50^1^1^10^1^1^10^1"
#define EMAIL_APP "appl^0^1^1^E-Mail^E-Mail^110,995,25,465,587,220,143,993^^1^50^1^0^50^1^1^10^1^0^10^1"
#define BW_APP_RULE XBOX_APP"|"\
					PLAYSTATION_APP"|"\
					MSN_GAME_APP"|"\
					BATTLENET_APP"|"\
					VOIP_APP"|"\
					IM_APP"|"\
					WEB_APP"|"\
					FTP_APP"|"\
					EMULE_APP"|"\
					BT_APP"|"\
					EMAIL_APP

#define UPNP_FRIENDLYNAME   "LITEON Internet Sharing Gateway"
#define MANUFACTURER        "LITEON Communications Corp."
#define MANUFACTURERURL     "http://www.howen.com"
#define MODELDESCRIPTION    "LITEON Internet Sharing Gateway"
#define MODELNAME           "LITEON Internet Sharing Gateway"
#define MODELNUMBER         ""
#define MODELURL            ""
#define SERIALNUMBER        "A19003848"
#define UDN                 "uuid:75802409-bccb-40e7-8e6c-fa095ecce13e"
#define PRESENTATIONURL     ""
#define UPNP_XML_RULE		UPNP_FRIENDLYNAME"^"\
							MANUFACTURER"^"\
							MANUFACTURERURL"^"\
							MODELDESCRIPTION"^"\
							MODELNAME"^"\
							MODELNUMBER"^"\
							MODELURL"^"\
							SERIALNUMBER"^"\
							UDN"^"\
							PRESENTATIONURL

#define HTTP_RULE_DEFAULT	"^1^0^admin^admin^guest^admin^admin^80^^5"
#define SNMPD_COM2SEC_RULE_DEFAULT	"com2sec^ro^default""|""com2sec^rw^default"
#define BW_EZMODESORT_RULE	"Game Console|VoIP|Instant Messenger|Web Surfing|P2P/FTP|E-Mail"


/* For fw upgrade/downgrade only */
#define LAN_DHCPS_GUESTLAN "default^0^33^32^86400^howen.com^^ispdns^^^^^none^opendns^opendns^192.168.1.10^24^60"

#define USER_SBM_DEFAULT_IPADDR_START ""
#define USER_SBM_DEFAULT_IPADDR_END   ""
#define GuestLANtoLAN "GuestLANtoLAN^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^^wan0^0^0^Game Console^1^^^^^0"
#define GuestLANtoWAN "GuestLANtoWAN^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^^wan0^0^0^Game Console^1^^^^^0"



/**
 *\brief nvram_factory_default The factory default settings
 *\showinitializer
 *\note Sort in alphabet order
 */
nvram_tuple_t nvram_factory_default[] = {                        
	{ "adtool_rule", "any^^4^any^^4^any^^4",
		NVRAM_NONE,
		0
	},

	{ "AP1_Channel", "36", 
		NVRAM_NONE,
		0 
	},
	{ "AP_Channel", "0", 
		NVRAM_NONE,
		0 
	},

	{ "arch", ARCH, 
		NVRAM_PROTECTED,
		0 
	},        

	{ "board_model_rule", "WP688",
		NVRAM_PROTECTED,
		0
	},

	{ "bootcount", "",
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},        

	{ "brand", "LITEON", 
		NVRAM_NONE,
		0 
	},

	{"bridge_rule_default", "0",
		NVRAM_NONE,
		0
	},
	{"bridge_rule", "",
		NVRAM_DEFAULT,
		0
	},

	{ "system_mode", LAN_WAN_MODE_DEFAULT,
		NVRAM_NONE,
		0
	},

	{ "br_rule", BR_RULE,
		NVRAM_NONE,
		0
	},
	{ "lan_wan_mode_rule", LAN_WAN_MODE_RULE,
 		NVRAM_NONE,
 		0
 	},
	{ "br_rule_max", BR_RULE_MAX,
		NVRAM_NONE,
		0
	},
	{ "br_rule_num", BR_RULE_NUM,
		NVRAM_NONE,
		0
	},

	{ "br_lan_rule", BR_LAN_RULE,
 		NVRAM_NONE,
 		0
 	},
	{ "br_lan_rule_max", BR_RULE_MAX,
		NVRAM_NONE,
		0
	},
	{ "br_lan_rule_num", BR_RULE_NUM,
		NVRAM_NONE,
		0
	},

	{ "br_wan_rule", BR_WAN_RULE,
		NVRAM_NONE,
		0
	},
	{ "br_wan_rule_max", BR_RULE_MAX,
		NVRAM_NONE,
		0
	},
	{ "br_wan_rule_num", BR_RULE_NUM,
		NVRAM_NONE,
		0
	},



	{ "bw_app_rule", BW_APP_RULE,
		NVRAM_NONE,
		0 
	},
	{ "bw_app_rule_max", "32", 
		NVRAM_NONE,
		0 
	},
	{ "bw_app_rule_num", "11",
		NVRAM_NONE,
		0 
	}, 

	{ "bw_ezmodesort_rule", BW_EZMODESORT_RULE,
		NVRAM_NONE,
		0 
	},
	{ "bw_ezmodesort_rule_max", "6", 
		NVRAM_NONE,
		0 
	},
	{ "bw_ezmodesort_rule_num", "6", 
		NVRAM_NONE,
		0 
	},

	{ "bwm_level", BWM_LEVEL_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "bwm_level_dl_index", BWM_LEVEL_DL_INDEX_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "bwm_level_up_index", BWM_LEVEL_UP_INDEX_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "bw_monitor_rule", "1",
		NVRAM_NONE,
		0 
	},

    { "bw_rule", "0^32768^32768",
        NVRAM_NONE,
        0 
    },
	{ "bw_sbm_mask", "0x00FF0000", 
		NVRAM_NONE,
		0 
	},
	{ "bw_sbm_rule", BW_SBM_RULE,
		NVRAM_NONE,
		0 
	},
	{ "bw_sbm_rule_max", BW_SBM_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "bw_sbm_rule_num", BW_SBM_RULE_NUM, 
		NVRAM_NONE,
		0 
	},
	{ "bw_tc_mask", "0x00FF0000", 
		NVRAM_NONE,
		0 
	},

	{ "cos_classification_status", COS_CLASSIFICATION_STATUS_VALUE, 
		NVRAM_NONE,
		0 
	},    
	{ "cos_classify", COS_CLASSIFY_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "cron_rule", "1^1^02 4 * * *", 
		NVRAM_NONE,
		0 
	},

	{ "ct_max", CT_MAX, 
		NVRAM_NONE,
		0 
	},

	{ "direct_multicast_maxnum", DIRECT_MULTICAST_MAXNUM_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "direct_multicast_status", DIRECT_MULTICAST_STATUS_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "FirstConfig", "1", 
		NVRAM_NONE,
		0 
	},

	{ "fl_enable", "1", 
		NVRAM_NONE,
		0 
	},  
	{ "fl_hwaddr_enable", "1", 
		NVRAM_NONE,
		0 
	},
	{ "fl_hwaddr_rule", "", 
		NVRAM_EMPTY,
		0 
	},
	{ "fl_hwaddr_rule_default", "1", 
		NVRAM_NONE,
		0 
	},
	{ "fl_hwaddr_rule_max", FL_HWADDR_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "fl_hwaddr_rule_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "fl_mark_default", "0x0000FFFF", 
		NVRAM_NONE,
		0 
	},      
	{ "fl_mask", "0x0000FFFF", 
		NVRAM_NONE,
		0 
	},      
	{ "fl_rule", FL_RULE, 
		NVRAM_EMPTY,
		0 
	}, 
	{ "fl_rule_default", "1", 
		NVRAM_NONE,
		0 
	},
	{ "fl_rule_max", FL_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "fl_rule_num", FL_RULE_NUM, 
		NVRAM_NONE,
		0 
	},

	{ "fr_dmz_rule", FR_DMZ_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "fr_enable", "1", 
		NVRAM_NONE,
		0 
	},
	{ "fr_rule", FR_RULE, 
		NVRAM_EMPTY,
		0 
	},
	{ "fr_rule_max", FR_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "fr_rule_num", FR_RULE_NUM, 
		NVRAM_NONE,
		0 
	},
	{ "one_to_one_rule", ONE_TO_ONE_RULE,
		NVRAM_NONE,
		0
		},
	{ "one_to_one_max", ONE_TO_ONE_MAX,
	 	NVRAM_NONE,
	 	0
	 	},
	{ "one_to_one_num", ONE_TO_ONE_NUM,
	  	NVRAM_NONE,
	  	0
	  	},
	{ "fw_info_rule", "",
		NVRAM_TEMP,
		0 
	},   
	{ "fw_rule", "1^1^1^1^1^0^1^0", 
		NVRAM_NONE,
		0 
	},
	{ "fw_upgrade_rule", FW_UPGRADE_RULE,
		NVRAM_NONE,
		0 
	},
	{ "fw_version", EZP_PROD_VERSION, 
		NVRAM_NONE, 
		0 
	},   

	{ "guest1_lan_rule", GUEST_5G_LAN_RULE,
		NVRAM_NONE,
		0
	},
	{ "guest_lan_rule", GUEST_LAN_RULE,
		NVRAM_NONE,
		0
	},

	{ "heuristics_classification_status", HEURISTICS_CLASSIFICATION_STATUS_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "heuristics_classify", HEURISTICS_CLASSIFY_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "hostname", "HoWen",
		NVRAM_NONE,
		0 
	},           


	{ "http_rule_default", HTTP_RULE_DEFAULT,
		NVRAM_NONE,
		0 
	},
	{ "http_rule", HTTP_RULE_DEFAULT,
		NVRAM_DEFAULT,
		0 
	},

	{ "https_enable", "0", 
		NVRAM_NONE,
		0 
	},
	{ "https_port", "443", 
		NVRAM_NONE,
		0 
	},

	{ "hwnat_rule", HWNAT_RULE, 
		NVRAM_NONE,
		0 
	}, 

	{ "ifup_types", "lan wan", 
		NVRAM_NONE, 
		0 
	},   

	{ "igd", "CPE IGD Version 1.00", 
		NVRAM_NONE,
		0 
	},  

	{ "igmp_proxy_rule", IGMP_PROXY_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "igmp_snooping", IGMP_SNOOPING_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "igmp_snoop_rule", IGMP_SNOOP_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "ipsec_enable", "0",
		NVRAM_NONE,
		0    
	},   
	{ "ipsec_rule", "",  
		NVRAM_NONE,
		0    
	},
	{ "ipsec_rule_max", IPSEC_RULE_MAX,
		NVRAM_NONE,
		0    
	},
	{ "ipsec_rule_num", "0", 
		NVRAM_NONE,
		0    
	},
	{ "ipsec_status_rule", "",
		NVRAM_TEMP,
		0
	},

	{ "l2tpd_rule", "0^l2tp-server^60^1460^1460^192.168.48.1^" 
		xstr(L2TPD_SESSION_MAX) 
			"^192.168.49.1^" 
			xstr(L2TPD_SESSION_MAX) 
			"^1^^0^0^1^0^0^0^0^0^",
		NVRAM_NONE,
		0 
	},
	{ "l2tpd_session_max", xstr(L2TPD_SESSION_MAX), 
		NVRAM_NONE,
		0 
	},
	{ "l2tpd_user_rule", "",
		NVRAM_NONE,
		0 
	},
	{ "l2tpd_user_rule_max", L2TPD_USER_RULE_MAX,
		NVRAM_NONE,
		0 
	},
	{ "l2tpd_user_rule_num", "0",
		NVRAM_NONE,
		0 
	},
	{ "wlan_dev_prefix", WLAN_DEV_PREFIX, 
		NVRAM_NONE,
		0 
	},  

	{ "lan0_device", LAN0_DEVICE, 
		NVRAM_NONE,
		0 
	},  
	{ "lan0_dns", "", 
		NVRAM_TEMP,
		0 
	},           
	{ "lan0_gateway", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan0_gatewayv6", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan0_hwname", LAN0_HWNAME, 
		NVRAM_NONE,
		0 
	},  
	{ "lan0_ifname", LAN0_IFNAME, 
		NVRAM_NONE,
		0 
	},  
	{ "lan0_ifnames", LAN0_IFNAMES, 
		NVRAM_NONE,
		0 
	},  
	{ "lan0_ipaddr", "", 
		NVRAM_TEMP,
		0 
	},        
	{ "lan0_ipaddrv6", "2001:db8:feed:b00::1",
		NVRAM_TEMP,
		0 
	},       
	{ "lan0_mask", "", 
		NVRAM_TEMP,
		0 
	},
	{ "lan0_maskv6", "64", 
		NVRAM_TEMP,
		0 
	},          
	{ "lan0_netmask", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan0_ports", LAN0_PORTS, 
		NVRAM_NONE,
		0 
	}, 
	{ "lan0_prefixv6", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan0_proto", "static^static", 
		NVRAM_NONE,
		0 
	},  
	{ "lan0_protov6", "static", 
		NVRAM_NONE,
		0 
	}, 
	{ "lan0_wins", "", 
		NVRAM_TEMP,
		0 
	},          


	{ "lan1_dns", "", 
		NVRAM_TEMP,
		0 
	},           
	{ "lan1_gateway", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan1_gatewayv6", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan1_ifname", LAN1_IFNAME, 
		NVRAM_NONE,
		0 
	},  
	{ "lan1_ipaddr", "", 
		NVRAM_TEMP,
		0 
	},        
	{ "lan1_ipaddrv6", "2001:db8:feed:b00::1",
		NVRAM_TEMP,
		0 
	},        
	{ "lan1_mask", "", 
		NVRAM_TEMP,
		0 
	},          
	{ "lan1_maskv6", "64", 
		NVRAM_TEMP,
		0 
	},          
	{ "lan1_netmask", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan1_prefixv6", "", 
		NVRAM_TEMP,
		0 
	},       
	{ "lan1_proto", "static^static", 
		NVRAM_NONE,
		0 
	},  
	{ "lan1_protov6", "static", 
		NVRAM_NONE,
		0 
	},  
	{ "lan1_wins", "", 
		NVRAM_TEMP,
		0 
	},          
	{ "lan_dhcps_rule", LAN_DHCPS_RULE, 
		NVRAM_NONE,
		0 
	}, 
	{ "lan_dhcpsv6_rule", LAN_DHCPSV6_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "lan_dns_rule", LAN_DNS_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "lang_default", "EN", 
		NVRAM_NONE,
		0 
	},
	{ "lang", "", 
		NVRAM_DEFAULT,
		0 
	},

	{ "lan_hwaddr_clone_rule", LAN_HWADDR_CLONE_RULE, 
		NVRAM_NONE,
		0
	},
	{ "lan_hwaddr_rule_default", "", 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0
	},
	{ "lan_main_rule", LAN_MAIN_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "lan_num", LAN_NUM, 
		NVRAM_NONE,
		0 
	},  
	{ "lan_static_rule_default", LAN_STATIC_RULE_DEFAULT, 
		NVRAM_NONE,
		0 
	}, 
	{ "lan_static_rule", "", 
		NVRAM_DEFAULT,
		0 
	},
	{ "lan_staticv6_rule_default", LAN_STATICV6_RULE_DEFAULT, 
		NVRAM_NONE,
		0 
	},  
	{ "lan_staticv6_rule", "", 
		NVRAM_DEFAULT,
		0 
	},
	{ "lan_st_rule", "", 
		NVRAM_TEMP,
		0 
	},
	{ "lan_stv6_rule", "", 
		NVRAM_TEMP,
		0 
	},


	{ "license_invalid", "0",
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},        
	{ "license_key", "HW-WCHS01", 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},              

	{ "log_index", "show_all", 
		NVRAM_NONE,
		0 
	},
	{ "log_rule", LOG_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "log_selected_num", "2", 
		NVRAM_NONE,
		0 
	},

	{ "machine_id", "", 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},        

	{ "model", "Internet Gateway Device", 
		NVRAM_NONE,
		0 
	},              

	{ "mss_enable", "1", 
		NVRAM_NONE,
		0 
	},

	{ "nat_pass_rule", "1^1^1^1^1^1^1", 
		NVRAM_NONE,
		0 
	}, 
	{ "nat_rule", NAT_RULE, 
		NVRAM_NONE,
		0 
	}, 

	{ "no_root_swap", "1", 
		NVRAM_NONE,
		0 
	},

	{ "ntp_rule", "^1^pool^pool.ntp.org^^GMT0BST,M3.5.0/2,M10.5.0/2^0^^^^^^^0^^^^^^^^0^", 
		NVRAM_NONE,
		0 
	},

	{ "port_device_rule", "eth2^4", 
		NVRAM_NONE,
		0 
	},

	{ "pptpd_rule", "0^pptp-server^60^1444^1444^192.168.38.1^" 
		xstr(PPTPD_SESSION_MAX) 
			"^192.168.39.1^" 
			xstr(PPTPD_SESSION_MAX) 
			"^1^^0^0^1^0^0^1^0^0",
		NVRAM_NONE,
		0 
	},
	{ "pptpd_session_max", xstr(PPTPD_SESSION_MAX), 
		NVRAM_NONE,
		0 
	},
	{ "pptpd_user_rule", "",
		NVRAM_NONE,
		0 
	},
	{ "pptpd_user_rule_max", PPTPD_USER_RULE_MAX,
		NVRAM_NONE,
		0 
	}, 
	{ "pptpd_user_rule_num", "0",
		NVRAM_NONE,
		0 
	},  

	{ "prod_cat", xstr(EZP_PROD_CAT), 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},           
	{ "prod_fw_version", EZP_PROD_FW_VERSION, 
		NVRAM_NONE, 
		0 
	},   
	{ "prod_subcat", xstr(EZP_PROD_SUBCAT), 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},        
	{ "prod_subsubcat", xstr(EZP_PROD_SUBSUBCAT), 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},        

	{ "psyscontact", "", 
		NVRAM_NONE,
		0 
	},       
	{ "psyslocation", "deskTop", 
		NVRAM_NONE,
		0 
	},        
	{ "psysservices", "sysservices^72", 
		NVRAM_NONE,
		0 
	}, 

	{ "pt_enable", "1", 
		NVRAM_NONE,
		0 
	}, 
	{ "pt_rule", PT_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "pt_rule_max", PT_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "pt_rule_num", "12", 
		NVRAM_NONE,
		0 
	},

	{ "qos_rule_active", QOS_RULE_ACTIVE_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "qos_rule_record", QOS_RULE_RECORD_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "qos_rule_status", QOS_RULE_STATUS_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "reserved_mask", "0x0F000000", 
		NVRAM_NONE,
		0 
	},      

	{ "rip_conf_rule_default", "0^0^0^0^0^0^0^0",
		NVRAM_NONE,
		0
	},
	{ "rip_conf_rule", "",
		NVRAM_DEFAULT,
		0
	},
	{ "rip_mode","0",
		NVRAM_NONE,
		0
	},       

	{ "ripng_config","lan&wan",
		NVRAM_NONE,
		0
	},

	{ "root_bw", ROOT_BW_VALUE,        
		NVRAM_NONE,
		0 
	},

	{ "rrd_enable", "0", 
		NVRAM_NONE,
		0 
	},    

	{ "rt_conn_enable", "1", 
		NVRAM_NONE,
		0 
	},   
	{ "rt_enable", "1", 
		NVRAM_NONE,
		0 
	},   
	{ "rt_mask", "0xF0000000", 
		NVRAM_NONE,
		0 
	},   
	{ "rt_rip_rule", RT_RIP_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "rt_rule", "",
		NVRAM_EMPTY,
		0 
	}, 
	{ "rt_rule_max", RT_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "rt_rule_num", "0",
		NVRAM_NONE,
		0 
	},

	{ "sched1_enable", "0", 
		NVRAM_NONE,
		0 
	},
	{ "sched1_enable_bak", "0", 
		NVRAM_NONE,
		0 
	},       
	{ "sched1_rule", SCHED1_RULE,
		NVRAM_NONE,
		0 
	},   
	{ "sched1_rule_max", SCHED1_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "sched1_rule_num", SCHED1_RULE_NUM, 
		NVRAM_NONE,
		0 
	},

	{ "sched_enable", "0", 
		NVRAM_NONE,
		0 
	},
	{ "sched_enable_bak", "0", 
		NVRAM_NONE,
		0 
	},       
	{ "sched_rule", SCHED_RULE,
		NVRAM_NONE,
		0 
	},   
	{ "sched_rule_max", SCHED_RULE_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "sched_rule_num", SCHED_RULE_NUM, 
		NVRAM_NONE,
		0 
	},


	{ "smb_vpn_passthrough_rule", "0^0^0^0", 
		NVRAM_NONE,
		0 
	}, 

	{ "smb_ftp_passthrough_rule", "0", 
		NVRAM_NONE,
		0 
	}, 

	{ "smb_h323_passthrough_rule", "0", 
		NVRAM_NONE,
		0 
	}, 

	{ "sm_mode", "fast", 
		NVRAM_NONE,
		0 
	},   

	{ "snat_enable", "1", 
		NVRAM_NONE,
		0 
	},

	{ "snmpd_access_rule", SNMPD_ACCESS_RULE,
		NVRAM_NONE,
		0 
	}, 
	{ "snmpd_com2sec_rule_default", SNMPD_COM2SEC_RULE_DEFAULT,
		NVRAM_NONE,
		0 
	}, 
	{ "snmpd_com2sec_rule", "", 
		NVRAM_DEFAULT,
		0 
	},
	{ "snmpd_group_rule", SNMPD_GROUP_RULE,
		NVRAM_NONE,
		0 
	}, 
	{ "snmpd_ro_community", "public", 
		NVRAM_NONE,
		0 	
	},
	{ "snmpd_ro_passwd", "12345678", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_ro_username", "admin", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_rw_community", "private", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_rw_passwd", "12345678", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_rw_username", "user", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_trap_ipadress", "0.0.0.0", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_upgrade_rule_default", "1^^^1^^", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_upgrade_rule", "", 
		NVRAM_DEFAULT,
		0 
	},
	{ "snmpd_user_rule_default", "1^1^1^0", 
		NVRAM_NONE,
		0 
	},
	{ "snmpd_user_rule", "", 
		NVRAM_DEFAULT,
		0 
	}, 
	{ "snmpd_view_rule", "view^all^included^.1^80",
		NVRAM_NONE,
		0 
	}, 
	{ "snmpd_sel_id", "0",
		NVRAM_NONE,
		0 
	}, 

	
	{ "ssh_rule", "^1^8123", 
		NVRAM_NONE,
		0 
	},

	{ "sta_priority", STA_PRIORITY_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "stats_ifname", "", 
		NVRAM_TEMP,
		0 
	},
	{ "stats_interval", "", 
		NVRAM_TEMP,
		0 
	},
	{ "stats_proto", "", 
		NVRAM_TEMP,
		0 
	},
	{ "stats_rule", STATS_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "stats_type", "", 
		NVRAM_TEMP,
		0 
	},

	{ "stream_priority", STREAM_PRIORITY_VALUE, 
		NVRAM_NONE,
		0 
	},

	{ "telnet_enable", "1", 
		NVRAM_NONE,
		0 
	},
	{ "telnet_port", "23", 
		NVRAM_NONE,
		0 
	},

	{ "tos_classification_status", TOS_CLASSIFICATION_STATUS_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "tos_classify", TOS_CLASSIFY_VALUE, 
		NVRAM_NONE,
		0 
	},
	{ "tos_mark", TOS_MARK_VALUE,        
		NVRAM_NONE,
		0 
	},
	{ "tos_rule", "1^1^1^1^1^1^1", 
		NVRAM_NONE,
		0 
	}, 

	{ "ttl_rule", "^0^64", 
		NVRAM_NONE,
		0 
	},

	{ "turbonat_enable", "0", 
		NVRAM_NONE,
		0 
	},   

	{ "udn", "uuid:75802409-bccb-40e7-8e6c-fa095ecce13e", 
		NVRAM_NONE,
		0 
	},  

	{ "upnp_rule", "^1^0^wan0^lan0^5555",
        NVRAM_NONE,
		0 
	},
	{ "upnpxml_rule", UPNP_XML_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "vlan1hwname", VLAN_HWNAME,       
		NVRAM_NONE,                     
		0                               
	},
	{ "vlan1ports", VLAN1_PORTS,        
		NVRAM_NONE,                     
		0                               
	},
	{ "vlan2hwname", VLAN_HWNAME,       
		NVRAM_NONE,                     
		0                               
	},
	{ "vlan2ports", VLAN2_PORTS,        
		NVRAM_NONE,                     
		0                               
	},
	{ "vlanport_rule", VLANPORT_RULE,
		NVRAM_NONE,
		0 
	},	
	{ "vlanport_rule_max", VLANPORT_RULE_MAX,
		NVRAM_NONE,
		0 
	},
	{ "vlanport_rule_num", VLANPORT_RULE_NUM,
		NVRAM_NONE,
		0 
	},      
	{ "vlan_rule", VLAN_RULE,
		NVRAM_NONE,
		0 
	},	
	{ "vlan_rule_max", VLAN_RULE_MAX,
		NVRAM_NONE,
		0 
	},
	{ "vlan_rule_num", VLAN_RULE_NUM,
		NVRAM_NONE,
		0 
	},      



	{ "wan0_bw_mode", "auto",
		NVRAM_NONE,
		0
	}, 
	{ "wan0_device", WAN0_DEVICE, 
		NVRAM_NONE,
		0 
	},       
	{ "wan0_device_backup", WAN0_DEVICE, 
		NVRAM_NONE,
		0 
	},        
	{ "wan0_dns", "",
		NVRAM_TEMP,
		0 
	},           
	{ "wan0_dnsv6", "",
		NVRAM_TEMP,
		0 
	},          
	{ "wan0_failcount", "",
		NVRAM_TEMP,
		0 
	},          
	{ "wan0_gateway", "",
		NVRAM_TEMP,
		0 
	},       
	{ "wan0_gatewayv6", "",
		NVRAM_TEMP,
		0 
	}, 
	{ "wan0_hwname", WAN0_HWNAME, 
		NVRAM_NONE,
		0 
	},        
	{ "wan0_ifname", WAN0_IFNAME, 
		NVRAM_NONE,
		0 
	},	
	{ "wan0_ifnames", WAN0_IFNAMES, 
		NVRAM_NONE,
		0 
	},       
	{ "wan0_ipaddr", "",
		NVRAM_TEMP,
		0 
	},        
	{ "wan0_ipaddrv6", "",
		NVRAM_TEMP,
		0 
	},        
	{ "wan0_mask", "", 
		NVRAM_TEMP,
		0 
	},          
	{ "wan0_maskv6", "", 
		NVRAM_TEMP,
		0 
	},        
	{ "wan0_netmask", "",
		NVRAM_TEMP,
		0 
	},       
	{ "wan0_ports", WAN0_PORTS, 
		NVRAM_NONE,
		0 
	}, 
	{ "wan0_prefixv6", "", 
		NVRAM_TEMP,
		0 
	},         
	{ "wan0_proto", "dhcp^dhcp", 
		NVRAM_NONE,
		0 
	},        
	{ "wan0_protov6", "link-local", 
		NVRAM_NONE,
		0 
	},        
	{ "wan0_rtmark", "0x10000000", 
		NVRAM_NONE,
		0 
	}, 
	{ "wan0_upnp_ifname", "",
		NVRAM_TEMP,
		0 
	},          
	{ "wan0_upnp_proto", "",
		NVRAM_TEMP,
		0 
	},          
	{ "wan0_wins", "",
		NVRAM_TEMP,
		0 
	},         



	{ "wan_bigpond_rule", WAN_BIGPOND_RULE,
		NVRAM_NONE,
		0 
	}, 
	{ "wan_ddns_rule", WAN_DDNS_RULE, 
		NVRAM_NONE,
		0 
	},	
	{ "wan_dhcp_rule", WAN_DHCP_RULE, 
		NVRAM_NONE,
		0 
	},   
	{ "wan_dhcpv6_rule", WAN_DHCPV6_RULE, 
		NVRAM_NONE,
		0 
	},   
	{ "wan_dns_rule", WAN_DNS_RULE,
		NVRAM_NONE,
		0 
	},
	{ "wan_dnsv6_rule", WAN_DNSV6_RULE,
		NVRAM_NONE,
		0 
	},
	{ "wan_hwaddr_clone_rule", WAN_HWADDR_CLONE_RULE, 
		NVRAM_NONE,
		0
	},  
	{ "wan_hwaddr_rule_default", "", 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0
	},
	{ "wan_main_rule", WAN_MAIN_RULE, 
		NVRAM_NONE,
		0 
	},	
	{ "wan_num", WAN_NUM,     
		NVRAM_NONE,
		0 
	},
	{ "wan_opendns_rule", WAN_OPENDNS_RULE, 
		NVRAM_NONE,
		0 
	},  
	{ "wan_pppoe_rule", WAN_PPPOE_RULE, 
		NVRAM_NONE,
		0 
	},  	
	{ "wan_pppoev6_rule", WAN_PPPOEV6_RULE, 
		NVRAM_NONE,
		0 
	},  	
	{ "wan_pptp_l2tp_rule", WAN_PPTP_L2TP_RULE, 
		NVRAM_TEMP,
		0 
	},  	
	{ "wan_pptpc_rule", WAN_PPTPC_RULE, 
		NVRAM_NONE,
		0 
	},  	
	{ "wan_l2tpc_rule", WAN_L2TPC_RULE, 
		NVRAM_NONE,
		0 
	},  	
	{ "wan_static_rule", WAN_STATIC_RULE, 
		NVRAM_NONE,
		0 
	},  
	{ "wan_staticv6_rule", WAN_STATICV6_RULE, 
		NVRAM_NONE,
		0 
	},  
	{ "wan_st_rule", "", 
		NVRAM_TEMP,
		0 
	},
	{ "wan_stv6_rule", "", 
		NVRAM_TEMP,
		0 
	},
	{ "wan_tunnel6_rule", WAN_TUNNEL6_RULE, 
		NVRAM_NONE,
		0 
	},  
	{ "wan_up_down_state_rule", "0^0",
		NVRAM_TEMP,
		0 
	},          
	{ "wan_weight_type", "1", 
		NVRAM_NONE,
		0 
	},	
	{ "wan_wwan_probe_rule", WAN_WWAN_PROBE_RULE, 
		NVRAM_TEMP,
		0 
	},  
	{ "wan_wwan_rule", WAN_WWAN_RULE, 
		NVRAM_NONE,
		0 
	}, 


	{ "weather_rule", "1^UKXX1428^c", 
		NVRAM_NONE,
		0 
	},

	{ "wf_content_rule", "0^0^0^0^0^",
		NVRAM_NONE,
		0
	}, 

	{ "wf_enable", "1",
		NVRAM_NONE,
		0
	}, 
	{ "wf_rule", "",
		NVRAM_EMPTY,
		0
	},
	{ "wf_rule_max", WF_RULE_MAX,
		NVRAM_NONE,
		0
	},
	{ "wf_rule_num", "0",
		NVRAM_NONE,
		0
	},

	{ "WISP1_Channel", "0", 
		NVRAM_NONE,
		0 
	},
	{ "WISP_Channel", "0", 
		NVRAM_NONE,
		0 
	},

	{ "wl00_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl00_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl00_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl00_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl01_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl01_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl01_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl01_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl02_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl02_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl02_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl02_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl03_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl03_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl03_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl03_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl04_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl04_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl04_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl04_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl05_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl05_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl05_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl05_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl06_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl06_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl06_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl06_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl07_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl07_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl07_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl07_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl0_apcli_rule", WL0_APCLI_RULE, 
		NVRAM_NONE,
		0 
	},      
	{ "wl0_apcli_rule_max", APCLI_RULE_MAX,  
		NVRAM_NONE,
		0 
	},   
	{ "wl0_apcli_rule_num", APCLI_RULE_MAX,   
		NVRAM_NONE,
		0 
	},   
	{ "wl0_apcli_sec_wep_rule", WL0_APCLI_SEC_WEP_RULE, 
		NVRAM_NONE,
		0 
	},     
	{ "wl0_apcli_sec_wpa2_rule", WL0_APCLI_SEC_WPA2_RULE, 
		NVRAM_NONE,
		0 
	},   
	{ "wl0_apcli_sec_wpa_rule", WL0_APCLI_SEC_WPA_RULE, 
		NVRAM_NONE,
		0 
	},    
	{"wl0_wpa_auth_rule_default", "^^^",
		NVRAM_NONE,
		0
	},
	{"wl0_wpa_auth_rule", "",
		NVRAM_DEFAULT,
		0
	},

	{ "wl0_basic_rule", WL0_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_ifname_rule", WL0_IFNAME_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_sec_rule", WL0_SEC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_sec_wep_rule", WL0_SEC_WEP_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_sec_wpa2_rule", WL0_SEC_WPA2_RULE, 
		NVRAM_NONE,
		0 
	}, 
	{ "wl0_sec_wpa_rule", WL0_SEC_WPA_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_ssid_rule", WL0_SSID_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl0_wds_basic_rule", WL0_WDS_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},  
	{ "wl0_wds_rule_max", WDS_RULE_MAX,
		NVRAM_NONE,
		0 
	},   
	{ "wl0_wds_rule_num", WDS_RULE_MAX,
		NVRAM_NONE,
		0 
	},   
	{ "wl0_wds_sec_wep_rule", "|||", 
		NVRAM_NONE,
		0 
	},
	{ "wl0_wds_sec_wpa_rule", "^tkip|^tkip|^tkip|^tkip", 
		NVRAM_NONE,
		0 
	},

	{ "wl10_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl10_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl10_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl10_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl11_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl11_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl11_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl11_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl12_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl12_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl12_acl_num", "0", 
		NVRAM_NONE,
		0 
	},
	{ "wl12_acl_rule", "", 
		NVRAM_NONE,
		0 
	},

	{ "wl13_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl13_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl13_acl_num", "0", 
		NVRAM_NONE,
		0 
	}, 
	{ "wl13_acl_rule", "", 
		NVRAM_NONE,
		0 
	}, 

	{ "wl14_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl14_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl14_acl_num", "0", 
		NVRAM_NONE,
		0 
	}, 
	{ "wl14_acl_rule", "", 
		NVRAM_NONE,
		0 
	}, 


	{ "wl15_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl15_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl15_acl_num", "0", 
		NVRAM_NONE,
		0 
	}, 
	{ "wl15_acl_rule", "", 
		NVRAM_NONE,
		0 
	}, 


	{ "wl16_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl16_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl16_acl_num", "0", 
		NVRAM_NONE,
		0 
	}, 
	{ "wl16_acl_rule", "", 
		NVRAM_NONE,
		0 
	}, 


	{ "wl17_acl_basic_rule", WL_ACL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl17_acl_max", WL_ACL_MAX, 
		NVRAM_NONE,
		0 
	},
	{ "wl17_acl_num", "0", 
		NVRAM_NONE,
		0 
	}, 
	{ "wl17_acl_rule", "", 
		NVRAM_NONE,
		0 
	}, 




	{ "wl1_advanced_rule", WL1_ADVANCED_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl1_ap_advanced_rule", WL1_AP_ADVANCED_RULE, 
		NVRAM_NONE,
		0 
	},


	{ "wl1_apcli_rule", WL1_APCLI_RULE, 
		NVRAM_NONE,
		0 
	},      
	{ "wl1_apcli_rule_max", APCLI_RULE_MAX,   
		NVRAM_NONE,
		0 
	},   
	{ "wl1_apcli_rule_num", APCLI_RULE_MAX,   
		NVRAM_NONE,
		0 
	},   
	{ "wl1_apcli_sec_wep_rule", WL1_APCLI_SEC_WEP_RULE, 
		NVRAM_NONE,
		0 
	},     
	{ "wl1_apcli_sec_wpa2_rule", WL1_APCLI_SEC_WPA2_RULE, 
		NVRAM_NONE,
		0 
	}, 
	{ "wl1_apcli_sec_wpa_rule", WL1_APCLI_SEC_WPA_RULE, 
		NVRAM_NONE,
		0 
	},    
	{"wl1_wpa_auth_rule_default", "^^^",
		NVRAM_NONE,
		0
	},
	{"wl1_wpa_auth_rule", "",
		NVRAM_DEFAULT,
		0
	},

	{ "wl1_basic_rule", WL1_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_ifname_rule", WL1_IFNAME_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_mode_rule", WL1_MODE_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_origset_rule", "1",
		NVRAM_NONE,
		0 
	},

	{ "wl1_sec_rule", WL1_SEC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_sec_wep_rule", WL1_SEC_WEP_RULE, 
		NVRAM_NONE,
		0 
	}, 
	{ "wl1_sec_wpa2_rule", WL1_SEC_WPA2_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_sec_wpa_rule", WL1_SEC_WPA_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl1_ssid_rule", WL1_SSID_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl1_wds_basic_rule", WL1_WDS_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_wds_rule", WL1_WDS_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl1_wds_rule_max", WDS_RULE_MAX,    
		NVRAM_NONE,
		0 
	},  
	{ "wl1_wds_rule_num", WDS_RULE_MAX,     
		NVRAM_NONE,
		0 
	},   
	{ "wl1_wds_sec_wep_rule", "|||", 
		NVRAM_NONE,
		0 
	},
	{ "wl1_wds_sec_wpa_rule", "^tkip|^tkip|^tkip|^tkip", 
		NVRAM_NONE,
		0 
	},
	{ "wl1_wps_rule", WL1_WPS_RULE, 
		NVRAM_NONE,
		0 
	}, 
	{ "wl1_wps_sta_rule", WL1_WPS_STA_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl5g_basic_rule", WL5G_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl5g_ap_basic_rule", WL5G_AP_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_ap_device_rule", "rai0^rai1^rai2^rai3^rai4^rai5^rai6^rai7", 
		NVRAM_NONE,
		0 
	},

	{ "wl_sta_device_rule", "apclii0", 
		NVRAM_NONE,
		0 
	},

	{ "wl_advanced_rule", WL_ADVANCED_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wlan_st_rule", "", 
		NVRAM_TEMP,
		0 
	},

	{ "wl_ap_advanced_rule", WL_AP_ADVANCED_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl_ap_basic_rule", WL_AP_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_basic_rule", WL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_countrycode_default", WL_COUNTRY_CODE, 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},
	{ "wl_countrycode", "", 
		NVRAM_DEFAULT,
		0 
	},

	{ "wl_easy_mode_rule", "0", 
		NVRAM_NONE,
		0 
	},       

	{ "wl_enabled_rule", "0", 
		NVRAM_TEMP,
		0 
	},      
	
	{ "wl_ieee80211h_default", "1", 
		NVRAM_NONE,
		0 
	},
	{ "wl_ieee80211h", "", 
		NVRAM_DEFAULT,
		0 
	},

	{ "wl_led_status_rule", "9^9",
		NVRAM_TEMP,
		0 
	},

	{ "wl_mode_rule", WL_MODE_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_num", WL_NUM, 
		NVRAM_NONE,
		0 
	},

	{ "wl_origset_rule", "1",
		NVRAM_NONE,
		0 
	},

	{ "wl_region_default", "0", 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},
	{ "wl_region", "", 
		NVRAM_DEFAULT,
		0 
	},

	{ "wl_regionaband_default", WL_REGION_ABAND, 
		NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
		0 
	},
	{ "wl_regionaband", "", 
		NVRAM_DEFAULT,
		0 
	},

	{ "wl_rule_max", WL_NUM,    
		NVRAM_NONE,
		0 
	},   
	{ "wl_rule_num", WL_NUM,     
		NVRAM_NONE,
		0 
	},   

	{ "wlv_rule_max", WL0_RULE_MAX,     
		NVRAM_NONE,
		0 
	},   
	{ "wlv_rule_num", WL0_RULE_NUM,     
		NVRAM_NONE,
		0 
	},   

	{ "wl_wds_rule", WL_WDS_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_wisp_mode_rule", "wisp^ap", 
		NVRAM_NONE,
		0 
	},

	{ "wl_wme_rule", WL_WME_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wl_wps_rule", WL_WPS_RULE, 
		NVRAM_NONE,
		0 
	},
	{ "wl_wps_sta_rule", WL_WPS_STA_RULE, 
		NVRAM_NONE,
		0 
	},

	{ "wol_rule", "",
		NVRAM_NONE,
		0 
	},

	{ "zyfw_fw_file", "", 
		NVRAM_NONE,
		0 
	},
	{ "zyfw_size", "", 
		NVRAM_NONE,
		0 
	},

	{ 0, 0, 0 ,0 }
};

#endif/*__CONFIG_WP688_H*/
