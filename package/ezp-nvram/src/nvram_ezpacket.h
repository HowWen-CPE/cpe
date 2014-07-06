/*
 * EZ Packet product default NVRAM values
 *
 * Copyright 2006, 2007 EZ Packet Technologies
 * All Rights Reserved.
 */

/*
 * Router default NVRAM values
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: defaults.c,v 1.104 2005/05/25 12:31:20 honor Exp $
 */

#ifndef _NVRAM_EZPACKET_H
#define _NVRAM_EZPACKET_H

#include <bcmnvram.h>

/* HW dependent definition. */
#ifdef CONFIG_EZP_ARCH_RT305X
#include "nvram_ezpacket-rt305x.h"
#endif
#ifdef CONFIG_EZP_ARCH_RT3883
#include "nvram_ezpacket-rt3883.h"
#endif
#ifdef CONFIG_EZP_ARCH_AS500
#include "nvram_ezpacket-as500.h"
#endif
#ifdef CONFIG_EZP_ARCH_BRCM
#include "nvram_ezpacket-brcm.h"
#endif
#ifdef CONFIG_EZP_ARCH_OX810
#include "nvram_ezpacket-ox810.h"
#endif
#ifdef CONFIG_EZP_ARCH_RTL865X
#include "nvram_ezpacket-rtl865x.h"
#endif /* CONFIG_EZP_ARCH_RTL865X */

#if (LAN_NUM == 1)
    #define LAN_DHCPSV6_RULE "default^1^2001:db8:feed:b00::^2001:db8:feed:b00::^2001:db8:feed:b00::^liteon.com^custom^^^0^100^200^30"
    #define LAN_STATICV6_RULE_DEFAULT "2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^64^^^^^^^^^^"
#elif (LAN_NUM == 2)
    #define LAN_DHCPSV6_RULE "default^1^2001:db8:feed:b00::^2001:db8:feed:b00::^2001:db8:feed:b00::^liteon.com^custom^^^0^100^200^30|default^0^2001:db8:feed:b10::^2001:db8:feed:b10::^2001:db8:feed:b10::^liteon.com^custom^^^0^100^200^30"
    #define LAN_STATICV6_RULE_DEFAULT "2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^64^^^^^^^^^^|2001:db8:feed:b10::^1^2001:db8:feed:b10::^1^2001:db8:feed:b10::^1^64^^^^^^^^^^"
#else
    #define LAN_DHCPSV6_RULE "default^1^2001:db8:feed:b00::^2001:db8:feed:b00::^2001:db8:feed:b00::^liteon.com^custom^^^0^100^200^30"
    #define LAN_STATICV6_RULE_DEFAULT "2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^2001:db8:feed:b00::^1^64^^^^^^^^^^"
#endif
#define EZP_PROD_FW_VERSION "V1.00(BWQ.1)C0"
#define WL_COUNTRY_CODE "US"
#define WL_REGION_ABAND "7"

#if ( EZP_PROD_SUBCAT == 0 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "20000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "0"
#define BW_DBM_RULE_MAX "0"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "0"
#define DBM_MAX "0"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "4"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "4"
#define ISHOST_RULE_MAX "4"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

/* VPN comes in. Note: No VPN in this subcat.*/
#if ( WAN_NUM == 1 )

#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"

#elif ( WAN_NUM == 2 )

#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 1 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "20000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "8"
#define DBM_MAX "8"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "4"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "4"
#define ISHOST_RULE_MAX "4"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

/* VPN comes in. Note: No VPN in this subcat.*/
#if ( WAN_NUM == 1 ) /* EZP_PROD_CAT: "B" */
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 2 )

#define FL_HWADDR_RULE_MAX "64"

/* PGP consumes too much memory and leaves less for conntrack. */
#if defined(EZP_PROD_CAT_C) || defined (EZP_PROD_CAT_T) || defined (EZP_PROD_CAT_D) 
#define CT_MAX "16000"
#else
#define CT_MAX "16000"
#endif

#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"

#if defined(EZP_PROD_SUBSUBCAT_L)
#define BW_DBM_RULE_MAX "0"
#define BW_SBM_RULE_MAX "64"
#define SBM_MAX "64"
#define DBM_MAX "1"
#define BW_DBM_RULE_NUM "0"
#define BW_DBM_RULE "DBM^0^192.168.1.20^192.168.1.27^8^"
#else
#define BW_DBM_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define SBM_MAX "8"
#define DBM_MAX "8"
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^192.168.1.20^192.168.1.27^8^"
#endif

#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define FL_RULE_NUM "0"
#define FL_RULE ""
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "10"
#define USER_SBM_DEFAULT_IPADDR_START ""
#define USER_SBM_DEFAULT_IPADDR_END   ""
#define USER_SBM1 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM2 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM3 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM4 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM5 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM6 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM7 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define USER_SBM8 "^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^tcp^wan0^0^10^Game Console^1^^^^^1"
#define GuestLANtoLAN "GuestLANtoLAN^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^^wan0^0^0^Game Console^1^^^^^0"
#define GuestLANtoWAN "GuestLANtoWAN^0^"USER_SBM_DEFAULT_IPADDR_START"^"USER_SBM_DEFAULT_IPADDR_END"^^wan0^0^0^Game Console^1^^^^^0"
#define BW_SBM_RULE USER_SBM1"|"USER_SBM2"|"USER_SBM3"|"USER_SBM4"|"USER_SBM5"|"USER_SBM6"|"USER_SBM7"|"USER_SBM8"|"GuestLANtoLAN"|"GuestLANtoWAN
#if (LAN_NUM == 1)
    #define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#elif (LAN_NUM == 2)
    #define LAN_DHCPS_GUESTLAN "default^0^33^32^86400^liteon.com^^ispdns^^^^^none^opendns^opendns^192.168.1.10^24^60"
    #define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60|"LAN_DHCPS_GUESTLAN
#else
    #define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#endif
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "8"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "8"
#define ISHOST_RULE_MAX "8"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "8"
#define SCHED_RULE "^0^wireless^^^everyday^00^00^00^00^0|^0^wireless^^^0^00^00^00^00^0|^0^wireless^^^1^00^00^00^00^0|^0^wireless^^^2^00^00^00^00^0|^0^wireless^^^3^00^00^00^00^0|^0^wireless^^^4^00^00^00^00^0|^0^wireless^^^5^00^00^00^00^0|^0^wireless^^^6^00^00^00^00^0"
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "8"
#define SCHED1_RULE "^0^wireless^^^everyday^00^00^00^00^0|^0^wireless^^^0^00^00^00^00^0|^0^wireless^^^1^00^00^00^00^0|^0^wireless^^^2^00^00^00^00^0|^0^wireless^^^3^00^00^00^00^0|^0^wireless^^^4^00^00^00^00^0|^0^wireless^^^5^00^00^00^00^0|^0^wireless^^^6^00^00^00^00^0"

/* VPN comes in. Note: No VPN in this subcat except MR-108N.*/
#if ( WAN_NUM == 1 ) 
#if defined(EZP_PROD_CAT_M)
#define PPTPD_SESSION_MAX 5	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "5"
#else
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#endif
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 3 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "20000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "16"
#define DBM_MAX "16"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "16"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "16"
#define ISHOST_RULE_MAX "16"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

/* VPN comes in. */
#if ( WAN_NUM == 1 ) /* EZP_PROD_CAT: "B" "C", subcat = 3, x116 series */
#define PPTPD_SESSION_MAX 16	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "16"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 4 )

#define FL_HWADDR_RULE_MAX "128"
#define CT_MAX "60000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "16"
#define DBM_MAX "16"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 5 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "80000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "32"
#define DBM_MAX "32"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 6 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "100000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "64"
#define DBM_MAX "64"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""

#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 7 )

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "120000"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define PT_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define IGMP_PROXY_RULE "0"
#define IGMP_SNOOP_RULE "0^0^0^0^0"
#define SBM_MAX "64"
#define DBM_MAX "64"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.255^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.255^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.255^^^^^^0^0003^2^2"
#define BW_ENABLE "1"
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#define LAN_DHCPS_RULE "default^1^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "64"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "64"
#define ISHOST_RULE_MAX "64"
#define LOG_RULE "^1^^0^1^1^1^1^0^1^1^1^1^1^0"
#define SCHED_RULE_MAX "16"
#define SCHED_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED1_RULE_MAX "16"
#define SCHED1_RULE_NUM "0"
#define SCHED1_RULE ""


#define PPTPD_SESSION_MAX 64	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "64"
#define L2TPD_SESSION_MAX 64	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "64"
#define IPSEC_RULE_MAX "32"

#else

#error "Undefined product subcategory"

#endif /* EZP_PROD_SUBCAT */

#define SUBCAT_MAX  7
#define FW_UPGRADE_RULE "ftpget^192.168.100.178^^^^zyfw_info"

/* WAN amount dependent definition. */
#if ( WAN_NUM == 1 )
#define WAN_MAIN_RULE "WAN1^0^1500^1^1^0"
#define WAN_BW_RULE "A2M256K^2048^256^0^0^75^90^90^155^19^1244^155"
#define WAN_DETECT_RULE "^0^ping^^60^8"
#define WAN_WEIGHT_RULE "1^1"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^"
#define WAN_DHCP_RULE "^^86400"
#define WAN_HTC_RULE "^^86400^512^0"
#define WAN_PPPOE_RULE "chap^^^0^300^5^5^1492^1492^isp^^"
#define WAN_STATICV6_RULE "2001:db8:feed:b01::2^64^2001:db8:feed:b01::1^^"
#define WAN_DHCPV6_RULE "^^86400^stateful"
#define WAN_PPPOEV6_RULE "^^^0^300^5^5^1492^1492^isp^"
#define WAN_WWAN_RULE "1^0^0^internet^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#define WAN_PPTP_L2TP_RULE "^static^^^^^^1^300^isp^^disabled^chap"
#define WAN_DNS_RULE "none^^none^"
#define WAN_DNSV6_RULE "ispdns^^ispdns^"
#define WAN_WWAN_PROBE_RULE "^^^^^^^"
#define WAN_WWAN_MODE "auto", "hsdpa", "umts", "edge", "gprs"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^"
#define WAN_HWADDR_CLONE_RULE "0^^hwaddr^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^"
#define RT_RULE "SMTP^0^^^^^tcp^^^25^25^wan0^default^^^"
#define RT_RIP_RULE "none"
#define NAT_RULE "1"
#define FR_RULE ""
#define FR_RULE_NUM "0"
#define FR_DMZ_RULE "^0^"
#define PT_RULE "^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0|^0^all^0^0^0^0"

#elif ( WAN_NUM == 2 )
#define WAN_MAIN_RULE "WAN1^0^1500^1^0^0|WAN2^0^1500^1^1^0"
#define WAN_BW_RULE "A2M256K^2048^256^0^0^75^90^90^155^19^1244^155|A2M256K^2048^256^0^0^75^90^90^155^19^1244^155"
#define WAN_DETECT_RULE "^0^ping^208.67.222.222^60^8|^0^ping^208.67.222.222^60^8"
#define WAN_WEIGHT_RULE "1^1|1^1"
#define WAN_FAILOVER_RULE "0^wan0^wan1"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^"
#define WAN_DHCP_RULE "^^86400|^^86400"
#define WAN_HTC_RULE "^^86400^512^0|^^86400^512^0"
#define WAN_PPPOE_RULE "chap^^^0^300^5^5^1492^1492^isp^^|chap^^^0^300^5^5^1492^1492^isp^^"
#define WAN_STATICV6_RULE "^^^^|^^^^"
#define WAN_DHCPV6_RULE "^^86400^stateful|^^86400^stateful"
#define WAN_PPPOEV6_RULE "^^^0^300^5^5^1492^1492^isp^|^^^0^300^5^5^1492^1492^isp^"
#define WAN_PPTP_L2TP_RULE "^static^^^^^^1^300^isp^^disabled^chap|^static^^^^^^1^300^isp^^disabled^chap"
#define WAN_DNS_RULE "none^^none^|none^^none^"
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
#define FR_RULE "HTTP^0^wan0^tcp^80^80^192.168.1.20^^|HTTPS^0^wan0^tcp^443^443^192.168.1.20^^|POP3^0^wan0^tcp^110^110^192.168.1.20^^|POP3S^0^wan0^tcp^995^995^192.168.1.20^^|SMTP^0^wan0^tcp^25^25^192.168.1.20^^|SMTPS^0^wan0^tcp^465^465^192.168.1.20^^|SSH^0^wan0^tcp^22^22^192.168.1.21^^|eMule^0^wan1^both^4662^4672^192.168.1.21^^"
#define FR_RULE_NUM "8"
#define FR_DMZ_RULE "^0^|^0^"

#else
#error "Undefined amount of WAN interface(s)"
#endif

/* LAN amount dependent definition. */
#if ( LAN_NUM == 1 )
    #define LAN_MAIN_RULE "LAN1^1^1500^1^1^0"
    #define LAN_STATIC_RULE_DEFAULT "192.168.1.10^24^^192.168.1.10^^0^^^192.168.1.10^24^192.168.1.10^24"
    #define LAN_HWADDR_CLONE_RULE "0^"
#elif ( LAN_NUM == 2 )
    #define LAN_MAIN_GUESTLAN "GuestLAN^0^1500^1^1^0"
    #define LAN_MAIN_RULE "LAN1^1^1500^1^1^0|"LAN_MAIN_GUESTLAN
    #define LAN_STATIC_GUESTLAN "192.168.100.1^24^^192.168.100.1^^0^^^192.168.100.1^24^192.168.1.2^24"
    #define LAN_STATIC_RULE_DEFAULT "192.168.1.10^24^^192.168.1.10^^0^^^192.168.1.10^24^192.168.1.10^24|"LAN_STATIC_GUESTLAN
    #define LAN_HWADDR_CLONE_RULE "0^"
#else
#error "Undefined amount of LAN interface(s)"
#endif

#if (WL_NUM > 0) /* WL_NUM is not zero. */

#if ( WL_NUM == 1 )
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^1^HT^1^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_AP_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^1^HT^1^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_WDS_RULE "disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#define WL_MODE_RULE "ur^2^0"
#define WL_WPS_RULE "0^^0"
#define WL_WPS_STA_RULE "0^^^^^^"
#define WL_ACL_MAX "64"
#define WL_ACL_BASIC_RULE "1^disabled"
#elif ( WL_NUM == 2 )
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_AP_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_WDS_RULE "disabled^1|disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128|15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#define WL_MODE_RULE "ur^0^0|ur^0^0"
#define WL_WPS_RULE "0^^0|0^^0"
#define WL_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#define WL_ACL_MAX "64"
#define WL_ACL_BASIC_RULE "1^disabled"
#else
#error "Undefined amount of WLAN interfaces"
#endif

#if ( WL_NUM == 1 )
  /* WDS */
  #define WL_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
  //#define WL1_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
  #define WL_WDS_SEC_WEP_RULE "|||"
  #define WL_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip"
  /* APCLI */
  #define WL_APCLI_RULE "1^CPE_2.4G^^disabled^0^0^0"
  #define WL_APCLI_SEC_WEP_RULE "1^^^^^^open"
  #define WL_APCLI_SEC_WPA_RULE "^tkip"
  #define WL_APCLI_SEC_WPA2_RULE "^aes"
  #if (WL_RULE_MAX == 2)
    #define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^1"
    #define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1"
    #define WL_SEC_RULE "disabled|disabled"
    //#define WL1_SEC_RULE "disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "^mixed^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
    #define WL_SEC_WPA2_RULE   "^mixed^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
  #elif (WL_RULE_MAX == 4)
    #define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
    #define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1|CPE_2.4G_SSID2|CPE_2.4G_SSID3"
    #define WL_SEC_RULE "disabled|disabled|disabled|disabled"
    //#define WL1_SEC_RULE "disabled|disabled|disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "12345678^mixed^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000"
    #define WL_SEC_WPA2_RULE   "12345678^mixed^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1"
    //#define WL0_SEC_5G_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
    //#define WL0_SEC_5G_WPA_RULE   "12345678^mixed^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000"
    //#define WL0_SEC_5G_WPA2_RULE   "12345678^mixed^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1"
  #else
/* default set visual interface number to 2 */
    #define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^1"
    #define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1"
    #define WL_SEC_RULE "disabled|disabled"
    //#define WL1_SEC_RULE "disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "^mixed^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
    #define WL_SEC_WPA2_RULE   "^mixed^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
  #endif
#endif

#if ( WL_NUM == 2 )
  /* WDS */
  #define WL_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled|^disabled|^disabled|^disabled|^disabled"
  #define WL_WDS_SEC_WEP_RULE "|||||||"
  #define WL_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip|^tkip|^tkip|^tkip|^tkip"
  /* APCLI */
  #define WL_APCLI_RULE "1^CPE2.4G^^disabled^0^0^0|1^CPE5G^^disabled^0^0^0"
  #define WL_APCLI_SEC_WEP_RULE "1^^^^^^open|1^^^^^^open"
  #define WL_APCLI_SEC_WPA_RULE "^tkip|^tkip"
  #define WL_APCLI_SEC_WPA2_RULE "^aes|^aes"
  #if (WL_RULE_MAX == 2)
  	#define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^1|1^0^0^1^0|0^0^0^1^1"
    #define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1|CPE_5G|CPE_5G_SSID1"
    #define WL_SEC_RULE "disabled|disabled|disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000"
    #define WL_SEC_WPA2_RULE   "^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000"
  #elif (WL_RULE_MAX == 4)
  	#define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
	#define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1|CPE_2.4G_SSID2|CPE_2.4G_SSID3|CPE_5G|CPE_5G_SSID1|CPE_5G_SSID2|CPE_5G_SSID3"
    #define WL_SEC_RULE "disabled|disabled|disabled|disabled|disabled|disabled|disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000"
    #define WL_SEC_WPA2_RULE   "^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000"
  #else
    /* default set visual interface number to 2 */
  	#define WLV_BASIC_RULE "1^0^0^1^0|0^0^0^1^1|1^0^0^1^0|0^0^0^1^1"
	#define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1|CPE_5G|CPE_5G_SSID1"
    #define WL_SEC_RULE "disabled|disabled|disabled|disabled"
    #define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
    #define WL_SEC_WPA_RULE   "^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000|^mixed^^^1812^disable^3600^5000|^tkip^^^1812^disable^3600^5000"
    #define WL_SEC_WPA2_RULE   "^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000|^mixed^^^1812^disable^3600^0^10^3000|^aes^^^1812^disable^3600^0^10^3000"
  #endif
#endif
#endif /* WL_NUM is not zero. */
/* 
 * The following macros are to stringify the macro argument. Please refer to
 * "C Preprocessor - Stringification".
 */
#define xstr(s) str(s)
#define str(s)  #s 

#define QOS_CONFIG

#ifdef QOS_CONFIG
/*direct_multicast_status*/
#define DIRECT_MULTICAST_STATUS_VALUE "1|1|1|1|1|1|1|1"
/*direct_multicast_maxnum*/
#define DIRECT_MULTICAST_MAXNUM_VALUE "10"
/*cos_classification_status*/
#define COS_CLASSIFICATION_STATUS_VALUE "0|0|0|0|0|0|0|0|0"
/*tos_classification_status*/
#define TOS_CLASSIFICATION_STATUS_VALUE "1|1|1|1|1|1|1|1|1|1|1|1"
/*heuristics_classification_status*/
#define HEURISTICS_CLASSIFICATION_STATUS_VALUE "1|1|1|1|1|1|1|1|1"
/*bwm_level_dl_index*/
#define BWM_LEVEL_DL_INDEX_VALUE "0|0|0|0|0|0|0|0"
/*bwm_level_up_index*/
#define BWM_LEVEL_UP_INDEX_VALUE "0|0|0|0|0|0|0|0"
/*qos_rule_status*/
#define QOS_RULE_STATUS_VALUE "0|0|0|0|0|0|0|0|0"
/*qos_rule_active*/
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

/*stream_priority*/
/*voice^video^data^background*/
#define STREAM_PRIORITY_VALUE "4^5^6^7"
/*sta_priority*/
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

/*qos_rule_record*/
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

/*igmp_snooping*/
/*status^age*/
#define IGMP_SNOOPING_VALUE "1^260"
/*tos_classify*/
/*voice^video^data^background*/
#define TOS_CLASSIFY_VALUE "0xE0,0xC0,0xB8^0xA0,0x80^none^none^0^0"
/*cos_classify*/
/*voice^video^data^background*/
#define COS_CLASSIFY_VALUE "none^none^none^none"
/*heuristics_classify*/
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
/*tos_mark*/
#define TOS_MARK_VALUE "1^0x8^0x5^0x0^0x0"

/*root_bw*/
#define ROOT_BW_VALUE "307200^307200^307200"

#endif

#define BW_SBM_RULE ""
#define BW_SBM_RULE_NUM "0"
#define CT_MAX "20000"
#define FR_DMZ_RULE "^0^|^0^"
#define SCHED1_RULE ""
#define SCHED1_RULE_NUM "0"
#define SCHED_RULE ""
#define SCHED_RULE_NUM "0"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^|0^61.9.192.13^^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^|WAN2_DDNS^0^dyndns^^^^"
#define WAN_DHCP_RULE "^^86400|^^86400"
#define WAN_DHCPV6_RULE "^^86400^stateful|^^86400^stateful"
#define WAN_DNS_RULE "none^^none^|none^^none^"
#define WAN_DNSV6_RULE "ispdns^^ispdns^|ispdns^^ispdns^"
#define WAN_HWADDR_CLONE_RULE "0^^hwaddr^|0^^hwaddr^"
#define WAN_MAIN_RULE "WAN1^0^1500^1^0^0|WAN2^0^1500^1^1^0"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^|WAN2_OPENDNS^0^^^0^"
#define WAN_PPPOE_RULE "chap^^^0^300^5^5^1492^1492^isp^^|chap^^^0^300^5^5^1492^1492^isp^^"
#define WAN_PPPOEV6_RULE "^^^0^300^5^5^1492^1492^isp^|^^^0^300^5^5^1492^1492^isp^"
#define WAN_PPTP_L2TP_RULE "^static^^^^^^1^300^isp^^disabled^chap|^static^^^^^^1^300^isp^^disabled^chap"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^"
#define WAN_STATICV6_RULE "^^^^|^^^^"
#define WAN_WWAN_PROBE_RULE "^^^^^^^|^^^^^^^"
#define WL_ACL_BASIC_RULE "1^disabled"
#define WL1_WDS_RULE "disabled^1|disabled^1"
#define WL1_MODE_RULE "ur^0^0|ur^0^0"
#define WL1_WPS_RULE "0^^0|0^^0"
#define WL1_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#define WL1_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#define WL1_IFNAME_RULE "ra0|ra1|ra2|ra3"
#define WL5G_SSID_RULE "CPE_5G|CPE_5G_SSID1|CPE_5G_SSID2|CPE_5G_SSID3"
#define WL1_SEC_RULE "disabled|disabled"
#define WL0_SEC_5G_WEP_RULE   "1^^^^^^open|1^^^^^^open|1^^^^^^open|1^^^^^^open"
#define WL0_SEC_5G_WPA_RULE   "12345678^mixed^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000|^tkip^^^1812^time^3600^5000^3000"
#define WL0_SEC_5G_WPA2_RULE   "12345678^mixed^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1|^aes^^^1812^time^3600^5000^0^10^3000^1"
#define WL1_APCLI_RULE "1^CPE_5G^^disabled^0^0"
#define WL1_APCLI_SEC_WEP_RULE "1^^^^^^open"
#define WL1_APCLI_SEC_WPA_RULE "^tkip"
#define WL1_APCLI_SEC_WPA2_RULE "^aes"
#define WL1_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
#define WAN_DNS_RULE "ispdns^^ispdns^|ispdns^^ispdns^"
#define WAN_MAIN_RULE "WAN1^1^1500^1^0^0|WAN2^1^1500^1^1^0"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^"
#define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#define WL_IFNAME_RULE "rai0|rai1|rai2|rai3"
#define WL_SSID_RULE "CPE_2.4G|CPE_2.4G_SSID1|CPE_2.4G_SSID2|CPE_2.4G_SSID3|CPE_2.4G_SSID4|CPE_2.4G_SSID5|CPE_2.4G_SSID6|CPE_2.4G_SSID7"
#define WL1_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^0^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1"
#define WL1_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_AP_ADVANCED_RULE "1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^0^1^0^33^5^0^1^64^0^1^0^1^2000|1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^1^0^1^2000"
#define WL_RULE_MAX 8
#define WL_RULE_NUM 8
#define WL_WDS_RULE "disabled^1|disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128|15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#define WL_WPS_RULE "0^^0|0^^0"
#define WL_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#define WL_ACL_MAX "64"
/* frank add, meaningless above, meaningful below*/
#define WL5G_BASIC_RULE "1^8^100^0^0"
#define WL_AP_BASIC_RULE "1^9^100^0^0"
#define WL_BASIC_RULE "1^9^100^0^0"
#define WL_SEC_WPA_RULE   "12345678^mixed^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"\
			"|^tkip^^^1812^time^3600^5000^3000"
#define WL_SEC_WEP_RULE   "1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"\
			"|1^^^^^^open|1^^^^^^open"
#define WL_SEC_RULE "disabled|disabled|disabled|disabled|disabled|disabled|disabled|disabled"
#define WL_SEC_WPA2_RULE   "12345678^mixed^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"\
			"|^aes^^^1812^time^3600^5000^0^10^3000^1"
#define FL_RULE ""
#define FL_RULE_MAX "64"
#define FL_RULE_NUM "0"
#define FR_RULE ""
#define FR_RULE_MAX "64"
#define FR_RULE_NUM "0"
#define WLAN_DEV_PREFIX "rai apclii"
#define WL_MODE_RULE "normal^2^0|normal^2^0"
#define WAN0_IFNAME "br-wan0"
#define WAN0_DEVICE "br-wan0"
#define LAN_WAN_MODE_DEFAULT "normal^0"
#define LAN_WAN_MODE_RULE "normal^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7^eth2|ap^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7 eth2^|wisp0^rai1 rai2 rai3 rai4 rai5 rai6 rai7 eth2^apclii0|wisp1^rai0 rai2 rai3 rai4 rai5 rai6 rai7 eth2^rai1|repeater^^"
#define LAN_STATIC_RULE_DEFAULT "192.168.1.1^24^^8.8.8.8^^^^^192.168.1.1^24^192.168.1.2^24"
#define LAN_NUM 1
#define LAN_DHCPS_RULE "default^0^33^32^86400^liteon.com^^dnsrelay^^^^^none^opendns^opendns^192.168.1.10^24^60"
#define LAN0_IFNAMES "rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7"
#define LAN0_IFNAME "br-lan0"
#define BR_RULE "LAN1^1^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7^|WAN1^0^eth2^|GuestLAN^0^rai0^"
#define BR_LAN_RULE "LAN1^1^rai0 rai1 rai2 rai3 rai4 rai5 rai6 rai7^|GuestLAN^0^ra1^"
#define BR_WAN_RULE "WAN1^1^eth2^"
#define BR_RULE_NUM "3"
#define BR_RULE_MAX "8"
static struct nvram_tuple nvram_ezpacket_default[] = {
    { "fw_version", xstr(EZP_PROD_VERSION), 
        NVRAM_NONE, 
        0 
    },   
    /* Added for different customer's version definition. */
    { "prod_fw_version", EZP_PROD_FW_VERSION, 
        NVRAM_NONE, 
        0 
    },   
    { "ifup_types", "lan wan", 
        NVRAM_NONE, 
        0 
    },   
    { "wan_num", xstr(WAN_NUM),     /* How many WAN interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wan_weight_type", "1", 
        NVRAM_NONE,
        0 
    },  /* WAN weight type [0 (manual) |1 (auto) ] */

#if (WAN_NUM >= 1)
    /* WAN0 */
    /* WAN H/W parameters */

    { "wan0_ifname", WAN0_IFNAME, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan0_ifnames", WAN0_IFNAMES, 
        NVRAM_NONE,
        0 
    },       /* WAN interface names */
    { "wan0_hwname", WAN0_HWNAME, 
        NVRAM_NONE,
        0 
    },        /* WAN driver name (e.g. et1) */
    { "wan0_device", WAN0_DEVICE, 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "wan0_device_backup", WAN0_DEVICE, 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "psyslocation", "deskTop", 
        NVRAM_NONE,
        0 
    },        /* WAN driver name (e.g. et1) */
    { "psyscontact", "everyone@local.com", 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "psysservices", "sysservices^72", 
        NVRAM_NONE,
        0 
    }, 
    {"rip_mode","0",
       NVRAM_NONE,
       0

    },       /*rip mode: 0 rip,1 ripng, 2 both*/
 	{"ripng_config","lan&wan",
	   NVRAM_NONE,
       0
	},/*interface value:lan/wan/lan&wan*/
    { "telnet_enable", "1", 
        NVRAM_NONE,
        0 
    },
    { "telnet_port", "23", 
        NVRAM_NONE,
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
    { "snmpd_ro_username", "admin", 
        NVRAM_NONE,
        0 
    },
    { "snmpd_ro_passwd", "12345678", 
        NVRAM_NONE,
        0 
    },
    { "snmpd_rw_username", "user", 
        NVRAM_NONE,
        0 
    },
    { "snmpd_rw_passwd", "12345678", 
        NVRAM_NONE,
        0 
    },
    { "snmpd_ro_community", "public", 
        NVRAM_NONE,
        0 
   },
   { "snmpd_rw_community", "private", 
        NVRAM_NONE,
        0 
   },
   { "snmpd_trap_ipadress", "192.168.1.10", 
        NVRAM_NONE,
        0 
   },
	/* WAN device name */
    /* MR routers use wwan as default*/
    { "wan0_proto", "static", 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
    { "wan0_protov6", "link-local", 
        NVRAM_NONE,
        0 
    },        /* IPv6 WAN proto */
    { "wan0_rtmark", "0x10000000", 
        NVRAM_NONE,
        0 
    }, /* WAN device routing mark. */
    { "wan0_ports", WAN0_PORTS, 
        NVRAM_NONE,
        0 
    }, /* WAN device port assignment. */
    { "wan0_bw_mode", "auto",
        NVRAM_NONE,
        0
    }, /* WAN DBM mode: auto/expert */

    /* WAN temporary parameters */
    { "wan0_ipaddr", "",
       NVRAM_TEMP,
       0 
    },        /* WAN IP address */
    /* WAN temporary parameters */
    { "wan0_ipaddrv6", "",
       NVRAM_TEMP,
       0 
    },        /* WAN IP address */
    { "wan0_netmask", "",
       NVRAM_TEMP,
       0 
    },       /* WAN netmask */
    { "wan0_mask", "", 
       NVRAM_TEMP,
       0 
    },          /* WAN netmask2 */
    { "wan0_prefixv6", "", 
       NVRAM_TEMP,
       0 
    },          /* WAN netmask2 */
    { "wan0_maskv6", "", 
       NVRAM_TEMP,
       0 
    },          /* WAN netmask2 */
    { "wan0_gateway", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "wan0_gatewayv6", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "wan0_dns", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */
    { "wan0_dnsv6", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */
    { "wan0_wins", "",
       NVRAM_TEMP,
       0 
    },          /* x.x.x.x x.x.x.x ... */
    { "wan0_failcount", "",
       NVRAM_TEMP,
       0 
    },          /* WAN detection failure count */
    { "wan0_upnp_proto", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP proto */
    { "wan0_upnp_ifname", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP ifname */
    /* Because we have v4/v6 on the same device, we have to check both when
     * trying to bring down an interface */
    { "wan_up_down_state_rule", "0^0",
       NVRAM_TEMP,
       0 
    },          /* v4^v6 */

#endif

    /* WAN main rules */
    { "wan_main_rule", WAN_MAIN_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp */
    /* WAN proto parameters rules */
    { "wan_static_rule", WAN_STATIC_RULE, 
        NVRAM_NONE,
        0 
    },  /* ipaddr^mask^gateway^dns1^dns2^wins */
    { "wan_dhcp_rule", WAN_DHCP_RULE, 
        NVRAM_NONE,
        0 
    },   /* hostname^domain^lease*/
    { "wan_pppoe_rule", WAN_PPPOE_RULE, 
        NVRAM_NONE,
        0 
    },  /* auth^username^passwd^demand^idletime^echo_failure^redialperiod^
         * mru^mtu^pppiptype[isp|custom]^custom_ipaddr^servname
         */
    /* V6 WAN proto parameters rules */
    { "wan_staticv6_rule", WAN_STATICV6_RULE, 
        NVRAM_NONE,
        0 
    },  /* ipaddr^length^gateway^dns1^dns2 */
    { "wan_dhcpv6_rule", WAN_DHCPV6_RULE, 
        NVRAM_NONE,
        0 
    },   /* hostname^domain^lease^type[0:stateless|1:stateful] */
    { "wan_pppoev6_rule", WAN_PPPOEV6_RULE, 
        NVRAM_NONE,
        0 
    },  /* username^passwd^servicename^
         * demand^idletime^echo_failure^redialperiod^
         * mru^mtu^pppiptype[isp|custom]^custom_ipaddr */
#define WAN_TUNNEL6_RULE "72.52.104.74^2001:470:1f04:d1c::2/64^2001:470:1f04:d1c::1"
    { "wan_tunnel6_rule", WAN_TUNNEL6_RULE, 
        NVRAM_NONE,
        0 
    },   /* brokerv4addr^v6ipaddr_length^v6gateway */
    { "wan_wwan_rule", WAN_WWAN_RULE, 
        NVRAM_NONE,
        0 
    },  /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
         * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
         * ctrltty^devname^flag^turbolink
         */
    /* wan_pptp_l2tp_rule */
    { "wan_pptp_l2tp_rule", WAN_PPTP_L2TP_RULE, 
        NVRAM_NONE,
        0 
    },  /* serverip^iptype[static|dhcp]^ipaddr^netmask^gateway^username^
           passwd^nailup^timeout^pppiptype[isp|custom]^custom_ipaddr^
           encmode[disabled|mppe]^auth[chap|pap|none] */
    { "wan_wwan_probe_rule", WAN_WWAN_PROBE_RULE, 
        NVRAM_TEMP,
        0 
    },  /* brand^model^signal^numtty^datatty^ctrltty^devname^flag */
    { "wan_bigpond_rule", WAN_BIGPOND_RULE,
        NVRAM_NONE,
        0 
    },  /* enable^server^username^passwd */
    { "wan_dns_rule", WAN_DNS_RULE,
        NVRAM_NONE,
        0 
    },  /* dnstype1[ispdns|custom|none]^dns1^dnstype2[ispdns|custom|none]^dns2 */
    { "wan_dnsv6_rule", WAN_DNSV6_RULE,
        NVRAM_NONE,
        0 
    },  /* dnstype1[ispdns|custom|none]^dns1^dnstype2[ispdns|custom|none]^dns2 */
    /* WAN hwaddr */ 
    { "wan_hwaddr_rule_default", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },  /* hwaddr */
    /* WAN hwaddr clone */ 
    { "wan_hwaddr_clone_rule", WAN_HWADDR_CLONE_RULE, 
        NVRAM_NONE,
        0
    },  /* enable[0|1]^hwaddr^addrtype[ipaddr|hwaddr]^ipaddr */
    /* DDNS */
    { "wan_ddns_rule", WAN_DDNS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^type[dyndns|tzo|zoneedit]^username^
           passwd^hostname */
    /* OpenDNS */
    { "wan_opendns_rule", WAN_OPENDNS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^username^passwd^redirect[0|1]^label */
    /* WAN status rule */
    { "wan_st_rule", "", 
        NVRAM_TEMP,
        0 
    },  /* uptime */
    /* WAN status rule */
    { "wan_stv6_rule", "", 
        NVRAM_TEMP,
        0 
    },  /* uptime */
    /* LAN status rule */
    { "lan_st_rule", "", 
        NVRAM_TEMP,
        0 
    },  /* uptime */
    /* LAN status rule */
    { "lan_stv6_rule", "", 
        NVRAM_TEMP,
        0 
    },  /* uptime */
    /* WLAN status rule */
    { "wlan_st_rule", "", 
        NVRAM_TEMP,
        0 
    },  /* uptime */
    { "lan_num", xstr(LAN_NUM), 
        NVRAM_NONE,
        0 
    },  /* How many LAN interfaces are available.*/
#if LAN_NUM >= 1
    /* LAN0 */
    /* LAN H/W parameters */

    { "lan0_ifname", LAN0_IFNAME, 
        NVRAM_NONE,
        0 
    },  /* LAN interface name */
    { "lan0_ifnames", LAN0_IFNAMES, 
        NVRAM_NONE,
        0 
    },  /* LAN interface names */
    { "lan0_hwname", LAN0_HWNAME, 
        NVRAM_NONE,
        0 
    },  /* LAN driver name (e.g. et1) */
    { "lan0_device", LAN0_DEVICE, 
        NVRAM_NONE,
        0 
    },  /* LAN device name */
    { "lan0_proto", "static", 
        NVRAM_NONE,
        0 
    },  /* LAN device name */
    { "lan0_protov6", "static", 
        NVRAM_NONE,
        0 
    },  /* LAN IPv6 device name */
    { "lan0_ports", LAN0_PORTS, 
        NVRAM_NONE,
        0 
    }, /* LAN device port assignment. */

    /* LAN temporary parameters */
    { "lan0_ipaddr", "", 
       NVRAM_TEMP,
        0 
    },        /* LAN IP address */
    /* LAN temporary parameters */
    { "lan0_ipaddrv6", "2001:db8:feed:b00::1",
       NVRAM_TEMP,
        0 
    },        /* LAN IPv6 address */
    { "lan0_netmask", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN netmask */
    { "lan0_mask", "", 
       NVRAM_TEMP,
        0 
    },
    /* LAN netmask2 */
    { "lan0_prefixv6", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN IPv6 prefix */
    { "lan0_maskv6", "64", 
       NVRAM_TEMP,
        0 
    },          /* LAN IPv6 netmask2 */
    { "lan0_gateway", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN gateway */
    { "lan0_gatewayv6", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN IPv6 gateway */
    { "lan0_dns", "", 
       NVRAM_TEMP,
        0 
    },           /* x.x.x.x x.x.x.x ... */
    { "lan0_wins", "", 
       NVRAM_TEMP,
        0 
    },          /* x.x.x.x x.x.x.x ... */
#if LAN_NUM >= 2
    /* LAN0 */

    { "lan1_ifname", LAN1_IFNAME, 
        NVRAM_NONE,
        0 
    },  /* LAN interface name */
    { "lan1_proto", "static", 
        NVRAM_NONE,
        0 
    },  /* LAN device name */
    { "lan1_protov6", "static", 
        NVRAM_NONE,
        0 
    },  /* LAN IPv6 device name */
    /* LAN temporary parameters */
    { "lan1_ipaddr", "", 
       NVRAM_TEMP,
        0 
    },        /* LAN IP address */
    { "lan1_ipaddrv6", "2001:db8:feed:b00::1",
       NVRAM_TEMP,
        0 
    },        /* LAN IPv6 address */
    { "lan1_netmask", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN netmask */
    { "lan1_mask", "", 
       NVRAM_TEMP,
        0 
    },          /* LAN netmask2 */
    { "lan1_prefixv6", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN IPv6 prefix */
    { "lan1_maskv6", "64", 
       NVRAM_TEMP,
        0 
    },          /* LAN IPv6 netmask2 */
    { "lan1_gateway", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN gateway */
    { "lan1_gatewayv6", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN IPv6 gateway */
    { "lan1_dns", "", 
       NVRAM_TEMP,
        0 
    },           /* x.x.x.x x.x.x.x ... */
    { "lan1_wins", "", 
       NVRAM_TEMP,
        0 
    },          /* x.x.x.x x.x.x.x ... */
#endif /* LAN_NUM >= 2*/
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_RT3883)
    { "vlan_rule", VLAN_RULE,
       NVRAM_NONE,
        0 
    },      /* name^enable[0|1]^ifname^vid^portmember^flag */
    { "vlan_rule_num", xstr(VLAN_RULE_NUM),
       NVRAM_NONE,
        0 
    },      
    { "vlan_rule_max", xstr(VLAN_RULE_MAX),
       NVRAM_NONE,
        0 
    },
    { "vlanport_rule", VLANPORT_RULE,
       NVRAM_NONE,
        0 
    },       /* name^portid^pvid^tag^gmemb */
    { "vlanport_rule_num", xstr(VLANPORT_RULE_NUM),
       NVRAM_NONE,
        0 
    },      
    { "vlanport_rule_max", xstr(VLANPORT_RULE_MAX),
       NVRAM_NONE,
        0 
    },
	{ "wlan_dev_prefix", WLAN_DEV_PREFIX, 
		NVRAM_NONE,
		0 
	},  

	{ "lan_wan_mode_rule", LAN_WAN_MODE_RULE,
 		NVRAM_NONE,
 		0
 	},
	{ "system_mode", LAN_WAN_MODE_DEFAULT,
		NVRAM_NONE,
		0
	},
    { "br_rule", BR_RULE,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
    { "br_rule_num", BR_RULE_NUM,
       NVRAM_NONE,
        0
    },
    { "br_rule_max", BR_RULE_MAX,
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
#ifdef CONFIG_EZP_ARCH_RT305X
    NVRAM_EZPACKET_DEFAULT_RT305X_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_2
#endif
#elif defined(CONFIG_EZP_ARCH_RT3883)
    NVRAM_EZPACKET_DEFAULT_RT3883_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_RT3883_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_RT3883_WAN_NUM_2
#endif
#endif
#endif

#ifdef CONFIG_EZP_ARCH_BRCM
    NVRAM_EZPACKET_DEFAULT_BRCM_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_2
#endif
#endif
#endif

#ifdef CONFIG_EZP_ARCH_AS500
    NVRAM_EZPACKET_DEFAULT_AS500_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_2
#endif
    NVRAM_EZPACKET_DEFAULT_AS500_SWAP
    NVRAM_EZPACKET_DEFAULT_AS500_INITD
#endif

    { "lan_main_rule", LAN_MAIN_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^weight */
    { "lan_hwaddr_rule_default", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },  /* hwaddr */
    { "lan_static_rule_default", LAN_STATIC_RULE_DEFAULT, 
        NVRAM_NONE,
        0 
    },  /* ipaddr^mask^gateway^dns^wins^alias_enable^alias_ipaddr^alias_netmask^
           ipaddr_normal^mask_normal^ipaddr_ap^mask_ap  */
    { "lan_static_rule", "", 
        NVRAM_DEFAULT,
        0 
    },  /* ipaddr^mask^gateway^dns^wins^alias_enable^alias_ipaddr^alias_netmask^
           ipaddr_normal^mask_normal^ipaddr_ap^mask_ap  */
    { "lan_staticv6_rule_default", LAN_STATICV6_RULE_DEFAULT, 
        NVRAM_NONE,
        0 
    },  /* pppoe_prefix^pppoe_suffix^static_prefix^static_suffix^dhcp_prefix^dhcp_suffix^
         * length^gateway^dns^wins^alias_enable^alias_ipaddr^alias_netmask^
         * ipaddr_normal^mask_normal^ipaddr_ap^mask_ap */
    { "lan_staticv6_rule", "", 
        NVRAM_DEFAULT,
        0 
    },  /* pppoe_prefix^pppoe_suffix^static_prefix^static_suffix^dhcp_prefix^dhcp_suffix^
         * length^gateway^dns^wins^alias_enable^alias_ipaddr^alias_netmask^
         * ipaddr_normal^mask_normal^ipaddr_ap^mask_ap */
    /* DHCP server parameters */
    { "lan_dhcps_rule", LAN_DHCPS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^start^num^lease^domain^wins^
           dnstype[ispdns|opendns|custom|none]^dnsaddr1^
           dnsaddr2^dnsaddr3^dnsaddr4^dnstype2^dnstype3^dnstype4 */
    /* LAN hwaddr clone */
    { "lan_dhcpsv6_rule", LAN_DHCPSV6_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^pppoe_prefix^static_prefix^dhcp_prefix^
           domain^
           dnstype[auto|custom]^
           dnsaddr1^dnsaddr2^type[1|0]^
           start^end^lifetime" */
    /* LAN hwaddr clone */
    { "lan_hwaddr_clone_rule", LAN_HWADDR_CLONE_RULE, 
        NVRAM_NONE,
        0
    },  /* enable[0|1]^hwaddr[XX:XX:XX:XX:XX:XX] */
    /* Guest lan rule */
#define GUEST_LAN_RULE "0^rai3^br2^1^192.168.2.1^24^0^0^1^liteon_guest.com^33^32"
    { "guest_lan_rule", GUEST_LAN_RULE,
        NVRAM_NONE,
        0
    },  /* enable^wif[ra0 ~ ra3]^bif[brxx]^lanid^ipaddr^netmask^bwen^bw[in kbits][0:disabled]^prio[1~3]^domain^start^count
         * domain : domain name for this dns group
         * start : dhcp offering start ip address
         * count : total count dhcp server can offer
         * */
#define GUEST_5G_LAN_RULE "0^ra3^br2^1^192.168.2.1^24^0^0^1^liteon_5g_guest.com^33^32"
    { "guest1_lan_rule", GUEST_5G_LAN_RULE,
        NVRAM_NONE,
        0
    },  /* enable^wif[ra0 ~ ra3]^bif[brxx]^lanid^ipaddr^netmask^bwen^bw[in kbits][0:disabled]^prio[1~3]^domain^start^count
         * domain : domain name for this dns group
         * start : dhcp offering start ip address
         * count : total count dhcp server can offer
         * */
    /* MASK */
    { "rt_mask", "0xF0000000", 
        NVRAM_NONE,
        0 },      /* Routing mask: 4-bit */
    { "reserved_mask", "0x0F000000", 
        NVRAM_NONE,
        0 },      /* Reserved mask: 4-bit */
    { "fl_mask", "0x0000FFFF", 
        NVRAM_NONE,
        0 },      /* FL mask: 16-bit */
    { "fl_mark_default", "0x0000FFFF", 
        NVRAM_NONE,
        0 },      /* FL mask: 16-bit */

    /* turboNAT */
    { "turbonat_enable", "0", 
        NVRAM_NONE,
        0 
    },   /* enable [0|1] */
    /* Session Manager */
    { "sm_mode", "fast", 
        NVRAM_NONE,
        0 
    },   /* mode [fast|regular|slow] */
    /* Bandwidth */
    /*{ "bw_rule", BW_RULE,*/
    { "bw_rule", "0^32768^32768",
        NVRAM_NONE,
        0 
    },   /* enable^bw_upload^bw_download */
    { "bw_monitor_rule", "1",
        NVRAM_NONE,
        0 
    },   /* enable */
    { "bw_tc_mask", "0x00FF0000", 
        NVRAM_NONE,
        0 
    },   /* BW mask */
    /* SBM */
    { "bw_sbm_mask", "0x00FF0000", 
        NVRAM_NONE,
        0 
    },   /* BW mask */
    /*{ "bw_app_rule", BW_APP_RULE, */
    /* Easy mode */
    { "bw_ezmodesort_rule", "Game Console|VoIP|Instant Messenger|Web Surfing|P2P/FTP|E-Mail",
        NVRAM_NONE,
        0 
    },   /* catname */
    { "bw_ezmodesort_rule_num", "6", 
        NVRAM_NONE,
        0 
    },
    { "bw_ezmodesort_rule_max", "6", 
        NVRAM_NONE,
        0 
    },
    { "bw_app_rule_num", "11",
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_app_rule_max", "32", 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
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
    { "bw_app_rule", XBOX_APP"|"PLAYSTATION_APP"|"MSN_GAME_APP"|"BATTLENET_APP"|"VOIP_APP"|"IM_APP"|"WEB_APP"|"FTP_APP"|"EMULE_APP"|"BT_APP"|"EMAIL_APP,
        NVRAM_NONE,
        0 
    }, /* name^enable^index^prio^cat^serv^
          tcp_port[port|port_beg:port_end]^
          udp_port[port|port_beg:port_end]^
          lan_tcp_enable^lan_tcp_bw^lan_tcp_min[0:max bw|1:min bw]^
          lan_udp_enable^lan_udp_bw^lan_udp_min[0:max bw|1:min bw]^
          wan_tcp_enable^wan_tcp_bw^wan_tcp_min[0:max bw|1:min bw]^
          wan_udp_enable^wan_udp_bw^wan_udp_min[0:max bw|1:min bw] */
#define ACCESS1 "access^ROGROUP^\"\"^any^noauth^exact^all^none^none"
#define ACCESS2 "access^RWGROUP^\"\"^any^noauth^exact^all^all^all"
    { "snmpd_access_rule", ACCESS1"|"ACCESS2,
        NVRAM_NONE,
        0 
    }, 
#define GROUP1 "group^RWGROUP^v1^rw"
#define GROUP2 "group^RWGROUP^v2c^rw"
#define GROUP3 "group^RWGROUP^usm^rw"    
#define GROUP4 "group^ROGROUP^v1^ro"
#define GROUP5 "group^ROGROUP^v2c^ro"
#define GROUP6 "group^ROGROUP^usm^ro"    
	{ "snmpd_group_rule", GROUP1"|"GROUP2"|"GROUP3"|"GROUP4"|"GROUP5"|"GROUP6,
        NVRAM_NONE,
        0 
    }, 
	{ "snmpd_view_rule", "view^all^included^.1^80",
        NVRAM_NONE,
        0 
    }, 
    { "bw_sbm_rule", BW_SBM_RULE,
        NVRAM_NONE,
        0 
    }, /* name^enable^inipaddr_start^inipaddr_end^proto^extif^
          bw_towan^bw_tolan^apptype^prio^
          outipaddr_start^outipaddr_end^inport^outport^reserve_bw */
    { "bw_sbm_rule_num", BW_SBM_RULE_NUM, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_sbm_rule_max", BW_SBM_RULE_MAX, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    /* Policy-based routing */
    { "rt_enable", "1", 
        NVRAM_NONE,
        0 
    },   /* Routing [0|1] */
    //{ "rt_rule", RT_RULE, 
    { "rt_rule", "",
        NVRAM_EMPTY,
        0 
    }, /* name^enable[0|1]^srcipaddr_start^srcipaddr_end^dstipaddr_start^
          dstipaddr_end^proto^srcport_start^srcport_end^dstport_start^
          dstport_end^extif^routing_type[default|static]^gateway_ipaddr^
          netmask^matric */
    //{ "rt_rule_num", "1",
    { "rt_rule_num", "0",
        NVRAM_NONE,
        0 
    }, /* The amount of existing RT rules */
    { "rt_rule_max", RT_RULE_MAX, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing RT rules */

    /*  routing */
    { "rt_conn_enable", "1", 
        NVRAM_NONE,
        0 
    },   /* Routing cache [0|1] */
    { "rt_rip_rule", RT_RIP_RULE, 
        NVRAM_NONE,
        0 
    }, /* rip[v1/v2/none] */
    { "nat_rule", NAT_RULE, 
        NVRAM_NONE,
        0 
    }, /* enable */
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_RT3883)
    { "hwnat_rule", HWNAT_RULE, 
        NVRAM_NONE,
        0 
    }, /* enable */
#endif
    /* web url/content filtering rule */
    { "wf_enable", "1",
        NVRAM_NONE,
        0
    }, /* web url filtering [0|1] */
    { "wf_content_rule", "0^0^0^0^0^",
        NVRAM_NONE,
        0
    }, /* activex_enable^java_enable^proxy_enable^cookie_enable^url_enable^trustipaddr */
    { "wf_rule", "",
        NVRAM_EMPTY,
        0
    }, /* enable^keyword^type^access */
    { "wf_rule_num", "0",
        NVRAM_NONE,
        0
    },
    { "wf_rule_max", WF_RULE_MAX,
        NVRAM_NONE,
        0
    },
    /* IGMP related rule */
    { "igmp_proxy_rule", IGMP_PROXY_RULE, 
        NVRAM_NONE,
        0 
    },   /* enable */
    { "igmp_snoop_rule", IGMP_SNOOP_RULE, 
        NVRAM_NONE,
        0 
    },   /* enable^p1^p2^p3^p4 */
    /* Schedule rule */
    { "sched_enable", "0", 
        NVRAM_NONE,
        0 
    },       /* Enable/Disable scheduling function  */
    { "sched1_enable", "0", // for 5g
        NVRAM_NONE,
        0 
    },       /* Enable/Disable scheduling function  */
    /* Schedule rule */
    { "sched_enable_bak", "0", 
        NVRAM_NONE,
        0 
    },       /* Enable/Disable scheduling function  */
    { "sched1_enable_bak", "0", // for 5g
        NVRAM_NONE,
        0 
    },       /* Enable/Disable scheduling function  */
    { "sched_rule", SCHED_RULE,
        NVRAM_NONE,
        0 
    },   /* name^enable[0|1]^cat^month^date^wkday^beghr^bmin^endhr^endmin */
         /* cat:   category, which will cause invoking action
          * month: apply month
          * date:  apply date of month
          * wkday: apply day of week
          * beghr,begmin: begin hour and minute of day
          * endhr,endmin: end hour and minute of day
          * wlanstatus: wlan status
          */
    { "sched1_rule", SCHED1_RULE,
        NVRAM_NONE,
        0 
    },   /* name^enable[0|1]^cat^month^date^wkday^beghr^bmin^endhr^endmin */
         /* cat:   category, which will cause invoking action
          * month: apply month
          * date:  apply date of month
          * wkday: apply day of week
          * beghr,begmin: begin hour and minute of day
          * endhr,endmin: end hour and minute of day
          * wlanstatus: wlan status
          */
    { "sched_rule_num", SCHED_RULE_NUM, 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */
    { "sched_rule_max", SCHED_RULE_MAX, 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */
    { "sched1_rule_num", SCHED1_RULE_NUM, 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */
    { "sched1_rule_max", SCHED1_RULE_MAX, 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */

    { "fw_upgrade_rule", FW_UPGRADE_RULE,
        NVRAM_NONE,
        0 
    },   /* method^server^path^username^passwd^inf_filename */
         /* rule set 0 for default online firmware upgrade settings */
    { "fw_info_rule", "",
        NVRAM_TEMP,
        0 
    },   /* model^version^revision^date^size^filename^release_note^
            info_ok[1:got fw info | 0:no info]^
            fw_ok[1:got fw | 0:no fw]^
            relnote_ok[1:got release note | 0:no release note] */
         /* rule set 0 for default online firmware upgrade settings */
    
    /* RRDCOLLECT */
    { "rrd_enable", "0", 
        NVRAM_NONE,
        0 },    /* RRDcollect client */

    /* Cron */
    { "cron_rule", "1^1^02 4 * * *", 
        NVRAM_NONE,
        0 
    }, /* enable^ntp_enable[0|1]^ntp_interval */

    /* NTP */
    //{ "ntp_rule", "^1^pool^pool.ntp.org^^UTC-8^^^^^^^^^^^^^^",
    { "ntp_rule", "^1^pool^pool.ntp.org^^GMT0BST,M3.5.0/2,M10.5.0/2^0^^^^^^^0^^^^^^^^0^", 
        NVRAM_NONE,
        0 
    }, /* name^enable[0|1]^type^pool^ipaddr^zone^
          custom_time[0|1]^year^mon^date^hour^min^sec^
          daylight_saving[0|1]^
          ds_start_mon^ds_start_day^ds_start_hour^
          ds_end_mon^ds_end_day^ds_end_hour^customized^
          custom_server[0|1]^serv_ipaddr */

    /* SSH */
    { "ssh_rule", "^1^8123", 
        NVRAM_NONE,
        0 
    },       /* name^enable[0|1]^port */

/* Added by Abocom */
    /* log parameters */
    { "log_selected_num", "9", 
        NVRAM_NONE,
        0 
    },        /* Log selection number */
    { "log_index", "show_all", 
        NVRAM_NONE,
        0 
    },
    /* Log Index */    
    { "log_rule", LOG_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^ipaddr^ipt_enable^web_mgmt^dns^ppp^
           upnp^wireless^ntp^sys_warning^dhcp_serv^dhcp_cli^
           ddns^firewall */
    /* uPnP */
	{ "upnp_rule", "^1^0^wan0^lan0^5555",
        NVRAM_NONE,
		0 
	},
    /* name^enable[0|1]^pmp_enable[0|1]^extif^inif^port */

    /* Filters */
    { "fl_enable", "1", 
        NVRAM_NONE,
        0 
    },    /* fl_enable [0|1] */
    { "fl_rule", FL_RULE, 
        NVRAM_EMPTY,
        0 
    },    /* name^enable[0|1]^extif^inif^proto^srcip^srcmask^
                                dstip^dstmask^srcport^dstport^
                                action[0(DROP)|1(ACCEPT)]^mark^bw_idx^
                                sched_idx */

    { "fl_rule_num", FL_RULE_NUM, 
        NVRAM_NONE,
        0 
    },      /* The amount of existing log rules */
    { "fl_rule_max", FL_RULE_MAX, 
        NVRAM_NONE,
        0 
    },
    { "fl_rule_default", "1", 
        NVRAM_NONE,
        0 
    },

    { "fl_hwaddr_enable", "1", 
        NVRAM_NONE,
        0 
    },    /* fl_hwaddr_enable [0|1] */
    { "fl_hwaddr_rule", "", 
        NVRAM_EMPTY,
        0 
    },    /* name^acl_enable[0|1]^dhcp_enable^arp_enable^hwaddr^
             action[0(DROP)|1(ACCEPT)]^ipaddr */
    { "fl_hwaddr_rule_num", "0", 
        NVRAM_NONE,
        0 
    }, /* The amount of existing fl_hwaddr rules */
    { "fl_hwaddr_rule_max", FL_HWADDR_RULE_MAX, 
        NVRAM_NONE,
        0 
    },
    { "fl_hwaddr_rule_default", "1", 
        NVRAM_NONE,
        0 
    },

    /* Port forwards */
    { "fr_enable", "1", 
        NVRAM_NONE,
        0 
    },  /* fr_enable [0|1] */
    { "fr_rule", FR_RULE, 
        NVRAM_EMPTY,
        0 
    },  /* name^enable[0|1]^extif^proto[tcp|udp|both]^extport_start^extport_end^ipaddr^inport_start^inport_end */
    { "fr_rule_num", FR_RULE_NUM, 
        NVRAM_NONE,
        0 
    },      /* fr_rule_num */
    { "fr_rule_max", FR_RULE_MAX, 
        NVRAM_NONE,
        0 
    },      /* fr_rule_max */
    { "fr_dmz_rule", FR_DMZ_RULE, 
        NVRAM_NONE,
        0 
    },      /* name^enable[0|1]^ipaddr */

    /* Port triggle */
    { "pt_enable", "1", 
        NVRAM_NONE,
        0 
    },  /* pt_enable [0|1] */
    { "pt_rule", PT_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable^proto[tcp|udp|all]^port_start^port_end^trig_port_start^trig_port_end */
    { "pt_rule_num", "12", 
        NVRAM_NONE,
        0 
    },  /* pt_rule_num */
    { "pt_rule_max", PT_RULE_MAX, 
        NVRAM_NONE,
        0 
    },  /* pt_rule_max */

    /* Wireless interface */
    { "wl_num", xstr(WL_NUM), 
        NVRAM_NONE,
        0 
    },      /* Enable (1) or disable (0) radio */
    /* Wireless parameters */
    { "wl_region_default", "0", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code default */
    { "wl_region", "", 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    { "wl_countrycode_default", WL_COUNTRY_CODE, 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code */
    { "wl_countrycode", WL_COUNTRY_CODE, 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    { "wl_regionaband_default", WL_REGION_ABAND, 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code */
    { "wl_regionaband", "", 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    { "wl_ieee80211h_default", "1", 
        NVRAM_NONE,
        0 
    },      /* IEEE802.11H */
    { "wl_ieee80211h", "", 
        NVRAM_DEFAULT,
        0 
    },      /* IEEE802.11H */
    /* Physical interface related rules*/
    { "wl_rule_num", xstr(WL_NUM),     /* How many WLan interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl_rule_max", xstr(WL_NUM),     /* How many WLan interfaces. */
        NVRAM_NONE,
        0 
    },   
    /* Virtual interface related rules for wl0/wl1... */
    { "wlv_rule_num", xstr(WL_RULE_NUM) ,     /* How many virtual wl0 interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wlv_rule_max", xstr(WL_RULE_MAX),     /* Max virtual wl0 interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl_enabled_rule", "0", 
        NVRAM_TEMP,
        0 
    },      /* Enable (1) or disable (0) radio */
    /* This setting reserved the must original wireless setting
     * such as hardware button status. Some application ,like schedular, 
     * may change the wifi at run time. We can use this setting to restore
     * the original setting. 
     * Currently, only /sbin/ezp-wifionoff may change its value */
    { "wl_origset_rule", "1",
        NVRAM_NONE,
        0 
    },
    { "wl1_origset_rule", "1",
        NVRAM_NONE,
        0 
    },
    { "wl0_basic_rule", WL0_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^net_mode^txpower^channel^bisolation */
	{ "wl_basic_rule", WL_BASIC_RULE, 
		NVRAM_NONE,
		0 
	},
    { "wl_ap_basic_rule", WL_AP_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^net_mode^txpower^channel^bisolation */
    { "wl5g_basic_rule", WL5G_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },
	{ "wl_ap_advanced_rule", WL_AP_ADVANCED_RULE, 
        NVRAM_NONE,
        0 
    },
	{ "wl_advanced_rule", WL_ADVANCED_RULE, 
        NVRAM_NONE,
        0 
    },      /* infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^antdiv^
               gmode^gmode_protection^afterburner^frameburst^
               txburst[0:disable|1:enable->1]^
               pktaggr[0:disable|1:enable->0]^
               txmode[0:None|1:CCK|2:OFDM->0]^

               rdg[0:disable|1:enablei->0]^
               linkadapt[0:disable|1:enable->0]^
               htc[0:disable|1:enable->0]^

               htbw[0:20|1:20/40->1]^
               gi[0:long|1:short->1]^
               opmode[0:mixed|1:GreenField->0]^
               mcs[0~15,32,33=auto->33]^
               mpdu[0 none, 1~7 -> 5]^
               amsdu[0:disable|1:enable->0]^

               autoba[0:disable|1:enable->1]^
               bawinsize[1~64,default=8,iot=64->64]^
               badecline[0:disable|1:enable->1->0]^
               extcha^wdstxmode[CCK|OFDM|HTMIX|GREENFIELD]
               => 1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^0^0
             */
    { "wl1_advanced_rule", WL1_ADVANCED_RULE, 
        NVRAM_NONE,
        0 
    },    /* infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^antdiv^
               gmode^gmode_protection^afterburner^frameburst^
               txburst[0:disable|1:enable->1]^
               pktaggr[0:disable|1:enable->0]^
               txmode[0:None|1:CCK|2:OFDM->0]^

               rdg[0:disable|1:enablei->0]^
               linkadapt[0:disable|1:enable->0]^
               htc[0:disable|1:enable->0]^

               htbw[0:20|1:20/40->1]^
               gi[0:long|1:short->1]^
               opmode[0:mixed|1:GreenField->0]^
               mcs[0~15,32,33=auto->33]^
               mpdu[0 none, 1~7 -> 5]^
               amsdu[0:disable|1:enable->0]^

               autoba[0:disable|1:enable->1]^
               bawinsize[1~64,default=8,iot=64->64]^
               badecline[0:disable|1:enable->1->0]^
               extcha^wdstxmode[CCK|OFDM|HTMIX|GREENFIELD]
               => 1^ap^default^0^2346^2346^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^0^0
             */
    { "wl_wds_rule", WL_WDS_RULE, 
        NVRAM_NONE,
        0 
    },      /* mode^timeout */  /*mode: disabled/bridge/repeater*/
    { "wl1_wds_rule", WL1_WDS_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl_wme_rule", WL_WME_RULE, 
        NVRAM_NONE,
        0 
    },      /* sta_bk^Sta_be^sta_vi^sta_vo^ap_bk^ap_be^ap_vi^ap_vo^no_ack^max_assoc */
    { "wl_mode_rule", WL_MODE_RULE, 
        NVRAM_NONE,
        0 
    },      /* mode[normal|ap|ur|wisp]^
             * auto_bridge[0: normal mode|1:auto bridge mode|2:auto ip mode]^
             * ab_status[0:auto bridge/ip not change|1:auto bridge/ip changed] */
    { "wl1_mode_rule", WL1_MODE_RULE, 
        NVRAM_NONE,
        0 
    }, 
    /* mode[normal|ap|ur|wisp]^
             * auto_bridge[0: normal mode|1:auto bridge mode|2:auto ip mode]^
             * ab_status[0:auto bridge/ip not change|1:auto bridge/ip changed] */
    { "wl_wps_rule", WL_WPS_RULE, 
        NVRAM_NONE,
        0 
    },      /* mode[0:disabled|7:enabled]^pin^configured[0:unconfigured|1:configured] */
    { "wl1_wps_rule", WL1_WPS_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl_wps_sta_rule", WL_WPS_STA_RULE, 
        NVRAM_NONE,
        0 
    },      /* mode[0:disabled|1:pin mode|2:PBC mode]^pin^reg_ssid^reg_auth^reg_enc^reg_keyindex^reg_key */
    { "wl1_wps_sta_rule", WL1_WPS_STA_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl_led_status_rule", "9^9",
        NVRAM_TEMP,
        0 
    },      /* wl^wps */
    { "wl1_basic_rule", WL1_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl0_ifname_rule", WL_IFNAME_RULE, 
        NVRAM_NONE,
        0 
    },      /* ssid */
    { "wl1_ifname_rule", WL1_IFNAME_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl0_ssid_rule", WL_SSID_RULE, 
        NVRAM_NONE,
        0 
    },      /* ssid */
    { "wl1_ssid_rule", WL5G_SSID_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl0_sec_rule", WL_SEC_RULE, 
        NVRAM_NONE,
        0 
    },      /* secmode */
    { "wl0_sec_wep_rule", WL_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^
               keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl0_sec_wpa_rule", WL_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^session_timeout */
    { "wl0_sec_wpa2_rule", WL_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout^wpacap */
    { "wl1_sec_rule", WL1_SEC_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl1_sec_wep_rule", WL0_SEC_5G_WEP_RULE, 
        NVRAM_NONE,
        0 
    },   
    /* key_index^key1^key2^key3^key4^
               keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl1_sec_wpa_rule", WL0_SEC_5G_WPA_RULE, 
        NVRAM_NONE,
        0 
    },   
    /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^session_timeout */
    { "wl1_sec_wpa2_rule", WL0_SEC_5G_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },    
    /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout^wpacap */
/* acl policy can apply to each ssid, we add rule according to ssid number */
    { "wl00_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl00_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl00_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl00_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl01_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl01_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl01_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl01_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl02_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl02_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl02_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl02_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl03_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl03_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl03_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl03_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl04_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl04_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl04_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl04_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl05_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl05_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl05_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl05_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl06_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl06_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl06_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl06_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl07_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl07_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl07_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl07_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */


/*  5g acl policy can apply to each ssid, we add rule according to ssid number */
    { "wl10_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl10_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl10_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl10_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl11_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl11_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl11_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl11_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl12_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl12_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl12_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl12_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl13_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl13_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl13_acl_max", WL_ACL_MAX, 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl13_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */

    /* wds related rules */
    { "wl0_wds_rule_num", xstr(WDS_RULE_MAX),     /* How wds interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl0_wds_rule_max", xstr(WDS_RULE_MAX),     //Max wds interfaces. 
        NVRAM_NONE,
        0 
    },
    { "wl0_wds_basic_rule", WL_WDS_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* hwaddr^secmode */
    { "wl1_wds_rule_num", xstr(WDS_RULE_MAX),     // How wds interfaces. 
        NVRAM_NONE,
        0 
    },
    { "wl1_wds_rule_max", xstr(WDS_RULE_MAX),     // Max wds interfaces. 
        NVRAM_NONE,
        0 
    },
    { "wl1_wds_basic_rule", WL1_WDS_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl0_wds_sec_wep_rule", WL_WDS_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key */
    { "wl0_wds_sec_wpa_rule", WL_WDS_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl1_wds_sec_wep_rule", "|||", 
        NVRAM_NONE,
        0 
    },
    { "wl1_wds_sec_wpa_rule", "^tkip|^tkip|^tkip|^tkip", 
        NVRAM_NONE,
        0 
    },
    /* apcli related rules */
    { "wl0_apcli_rule_num", xstr(APCLI_RULE_MAX),   // How apcli interfaces. 
        NVRAM_NONE,
        0 
    },   
    { "wl0_apcli_rule_max", xstr(APCLI_RULE_MAX),   // Max apcli interfaces. 
        NVRAM_NONE,
        0 
    },  
    { "wl0_apcli_rule", WL_APCLI_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^ssid^bssid^secmode */
    { "wl0_apcli_sec_wep_rule", WL_APCLI_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl0_apcli_sec_wpa_rule", WL_APCLI_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl0_apcli_sec_wpa2_rule", WL_APCLI_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl1_apcli_rule_num", xstr(APCLI_RULE_MAX),   // How apcli interfaces. 
        NVRAM_NONE,
        0 
    },  
    { "wl1_apcli_rule_max", xstr(APCLI_RULE_MAX),   /// Max apcli interfaces. 
        NVRAM_NONE,
        0 
    },
    { "wl1_apcli_rule", WL1_APCLI_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl1_apcli_sec_wep_rule", WL1_APCLI_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl1_apcli_sec_wpa_rule", WL1_APCLI_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },
    { "wl1_apcli_sec_wpa2_rule", WL1_APCLI_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },
    /* for firewall */
    { "fw_rule", "1^1^1^1^1^0^1^0", 
        NVRAM_NONE,
        0 
    },  /* enable^syn_enable^icmp_enable^icmp_redir_enable
           ^icmp_ping_enable^rpfilter_enable^state_enable^
           wantolan_enable[0:drop non-DNAT wan-to-lan connection request|1:otherwise] */
    { "nat_pass_rule", "1^1^1^1^1^1^1", 
        NVRAM_NONE,
        0 
    },  /* enable^ipsec_enable^pptp_enable^l2tp_enable^
           h323_enable^rtsp_enable^mms_enable */
    /* TTL */
    { "ttl_rule", "^0^64", 
        NVRAM_NONE,
        0 
    },        /* TTL name^enable[1|0]^value */
    /* TOS */
    { "tos_rule", "1^1^1^1^1^1^1", 
        NVRAM_NONE,
        0 
    },   /* TOS enable^icmp_enable^dns_enable^ssh_enable^
            telnet_enable^check_enable^ack_enable */
    /* MSS */
    { "mss_enable", "1", 
        NVRAM_NONE,
        0 
    },        /* TOS [1|0] */

    /* SNAT */
    { "snat_enable", "1", 
        NVRAM_NONE,
        0 
    },        /* SNAT [1|0] */

    /* Conntrack max amount */
    { "ct_max", CT_MAX, 
        NVRAM_NONE,
        0 
    },        /* SNAT [1|0] */
    /* Web server parameters */
    { "http_rule_default", "^1^0^admin^1234^guest^1234^admin^80^^5", 
        NVRAM_NONE,
        0 
    }, /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port^secipaddr^adm_timeout */
    { "http_rule", "", 
        NVRAM_DEFAULT,
        0 
    },  /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port^secipaddr^adm_timeout */
    {"rip_conf_rule_default", "0^0^0^0^0^0^0^0",
      NVRAM_NONE,
      0
    },
   {"rip_conf_rule", "",
      NVRAM_DEFAULT,
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
    {"wl0_wpa_auth_rule_default", "^^^",
      NVRAM_NONE,
      0
    },
    {"wl0_wpa_auth_rule", "",
      NVRAM_DEFAULT,
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
    {"board_model_rule", "",
      NVRAM_NONE,
      0
    },
	{ "board_model_rule_default", "",
		NVRAM_NONE,
		0
	},
    { "snmpd_user_rule_default", "1^1^1^0", 
        NVRAM_NONE,
        0 
    }, /* v1_enable[0|1]^v2c_enable[0|1]^usm_enable[0|1]*/
    { "snmpd_user_rule", "", 
        NVRAM_DEFAULT,
        0 
    }, 
    { "snmpd_com2sec_rule_default", "com2sec^ro^default""|""com2sec^rw^default", 
        NVRAM_NONE,
        0 
    }, 
    { "snmpd_com2sec_rule", "", 
        NVRAM_DEFAULT,
        0 
    },/*system status^filename^ipaddr^protocol^server username^server password*/
    { "snmpd_upgrade_rule_default", "1^^^1^^", 
        NVRAM_NONE,
        0 
    },/* v1_enable[0|1]^v2c_enable[0|1]^usm_enable[0|1]*/
    { "snmpd_upgrade_rule", "", 
        NVRAM_DEFAULT,
        0 
    },/* v1_enable[0|1]^v2c_enable[0|1]^usm_enable[0|1]*/
    { "adtool_rule", "any^^4^any^^4^any^^4",
        NVRAM_NONE,
        0
    }, /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
    { "stats_rule", STATS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^stime^ifnames */
    { "stats_type", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_ifname", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_proto", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_interval", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_time hourly:60, daily:600, weekly: 3600, monthly: 3600*6 */
    { "no_root_swap", "1", 
        NVRAM_NONE,
        0 
	}, /* no_root_swap */ 
    /* enable^pptp_enable^ipsec_enable^l2tp_enable */
    { "smb_vpn_passthrough_rule", "0^0^0", 
        NVRAM_NONE,
        0 
    }, 
    /* IPsec */
	{ "ipsec_enable", "0",
		NVRAM_NONE,
		0    
	},   
#ifdef IPSEC_RULE_MAX
#undef IPSEC_RULE_MAX
#define IPSEC_RULE_MAX "32"
#else 
#define IPSEC_RULE_MAX "32"
#endif
	{ "ipsec_rule_max", IPSEC_RULE_MAX,
		NVRAM_NONE,
		0    
	}, /* value */
	{ "ipsec_rule_num", "0", 
		NVRAM_NONE,
		0    
	}, /* value */
	{ "ipsec_rule", "",  
		NVRAM_NONE,
		0    
	},
	/*
 name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout

	 */
	{ "ipsec_status_rule", "",
		NVRAM_TEMP,
		0
	}, /* phase1[none|established]^phase2[none|established] */

#undef PPTPD_SESSION_MAX
#undef PPTPD_USER_RULE_MAX
#undef L2TPD_SESSION_MAX
#undef L2TPD_USER_RULE_MAX
#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
	/*pptpd*/
    { "pptpd_rule", "0^pptp-server^60^1460^1482^192.168.38.1^" xstr(PPTPD_SESSION_MAX) "^192.168.39.1^" xstr(PPTPD_SESSION_MAX) "^1^^0^0^1^0^0^1^0^1",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^name^redialperiod^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable[0|1]^dns^chap_enable[0|1]^mschap_enable[0|1]^mschapv2_enable[0|1]^eap_enable[0|1]^pap_enable[0|1]^mppe128_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */

    { "pptpd_user_rule", "",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^username^provider^passwd^ipaddr */
    { "pptpd_user_rule_num", "0",
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "pptpd_user_rule_max", PPTPD_USER_RULE_MAX,
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "pptpd_session_max", xstr(PPTPD_SESSION_MAX), 
        NVRAM_NONE,
        0 
    },  /* value */
    /*l2tpd*/
    { "l2tpd_rule", "0^l2tp-server^1400^1400^192.168.48.1^" xstr(L2TPD_SESSION_MAX) "^192.168.49.1^" xstr(L2TPD_SESSION_MAX) "^1^^1^0^1^1",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^name^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable^dns^chap_enable[0|1]^pap_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */
    { "l2tpd_user_rule", "",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^username^provider^passwd^ipaddr */
    { "l2tpd_user_rule_num", "0",
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "l2tpd_user_rule_max", L2TPD_USER_RULE_MAX,
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */

	{ "wol_rule", "",
		NVRAM_NONE,
		0 
	},  /* name^enable^bcast^hwaddr^iface^passwd */
	{ "lang_default", "EN", 
		NVRAM_NONE,
		0 
	},  /* Default web language support */
	{ "lang", "", 
		NVRAM_DEFAULT,
		0 
	},  /* Web language support */
	{ "brand", "LITEON", 
		NVRAM_NONE,
		0 
	},  /* Brand name - CSS style and SSID. */
#define UPNP_FRIENDLYNAME   "LITEON CPE Internet Sharing Gateway"
#define MANUFACTURER        "LITEON Communications Corp."
#define MANUFACTURERURL     "http://www.liteon.com"
#define MODELDESCRIPTION    "LITEON CPE Internet Sharing Gateway"
#define MODELNAME           "LITEON Internet Sharing Gateway"
#define MODELNUMBER         "CPE"
#define MODELURL            ""
#define SERIALNUMBER        "A19003848"
#define UDN                 "uuid:75802409-bccb-40e7-8e6c-fa095ecce13e"
#define PRESENTATIONURL     ""
    { "upnpxml_rule", UPNP_FRIENDLYNAME"^"MANUFACTURER"^"MANUFACTURERURL"^"MODELDESCRIPTION"^"MODELNAME"^"MODELNUMBER"^"MODELURL"^"SERIALNUMBER"^"UDN"^"PRESENTATIONURL, 
        NVRAM_NONE,
        0 
    },  /* friendlyName^manufacturer^manufacturerURL^modelDescription^
         * modelName^modelNumber^modelURL^serialNumber^UDN^presentationURL */
    { "igd", "CPE IGD Version 1.00", 
        NVRAM_NONE,
        0 
    },  /* Brand name - CSS style and SSID. */
    { "udn", "uuid:75802409-bccb-40e7-8e6c-fa095ecce13e", 
        NVRAM_NONE,
        0 
    },  /* Brand name - CSS style and SSID. */
    { "model", "CPE Internet Gateway Device", 
        NVRAM_NONE,
        0 
    },              /* Product model */
    { "license_key", "HoWen-000001", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },              /* Product model */
    { "hostname", "WP688",
        NVRAM_NONE,
        0 
    },           /* Hostname */
    { "connectrssithr", "0",
        NVRAM_NONE,
        0 
    },           /* Hostname */
    { "disconnectrssithr", "0",
        NVRAM_NONE,
        0 
    },           /* Hostname */
    { "prod_cat", xstr(EZP_PROD_CAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Product category */
    { "prod_subcat", xstr(EZP_PROD_SUBCAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Product subcategory */
    { "prod_subsubcat", xstr(EZP_PROD_SUBSUBCAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Product sub-subcategory */
    { "machine_id", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Machine id */
    { "arch", ARCH, 
        NVRAM_PROTECTED,
        0 
    },        /* Arch */
    { "license_invalid", "0",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* License status */
    { "bootcount", "",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* License bootcount */
/* Added by Abocom */
    { "weather_rule", "1^UKXX1428^c", 
        NVRAM_NONE,
        0 
    },        /* enable^city^degree */
    { "wl_easy_mode_rule", "0", 
        NVRAM_NONE,
        0 
    },        /* mode 0->2.4g  1->5g */
    { "FirstConfig", "1", 
        NVRAM_NONE,
        0 
    },        /* FirstConfig for web server */
     { "zyfw_fw_file", "", 
        NVRAM_NONE,
        0 
    },        /* file name for online FW upgrade */
    { "zyfw_size", "", 
        NVRAM_NONE,
        0 
    },        /* file size for online FW upgrade */
    { "WISP_Channel", "0", 
        NVRAM_NONE,
        0 
    },        /* Save WISP's wireless Channel */	
    { "WISP1_Channel", "0", 
        NVRAM_NONE,
        0 
    },        /* Save WISP's wireless 5g Channel */	
    { "AP_Channel", "0", 
        NVRAM_NONE,
        0 
    },		/* Save AP's wireless Channel */
    { "AP1_Channel", "36", 
        NVRAM_NONE,
        0 
    },		/* Save AP's wireless Channel */
    { "wl_wisp_mode_rule", "wisp^ap", 
        NVRAM_NONE,
        0 
    }, /*wifi 2.4g as wisp,wifi 5g as 
       ap mode when choose wisp mode*/
    /* v1_enable[0|1]^v2c_enable[0|1]^usm_enable[0|1]*/
#ifdef QOS_CONFIG

    /*port_status*/
    { "direct_multicast_status", DIRECT_MULTICAST_STATUS_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*maxnum*/
    { "direct_multicast_maxnum", DIRECT_MULTICAST_MAXNUM_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*port_status*/
    { "tos_classification_status", TOS_CLASSIFICATION_STATUS_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*port_status*/
    { "cos_classification_status", COS_CLASSIFICATION_STATUS_VALUE, 
        NVRAM_NONE,
        0 
    },    
    /*port_status*/
    { "heuristics_classification_status", HEURISTICS_CLASSIFICATION_STATUS_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*level*/
    { "bwm_level_dl_index", BWM_LEVEL_DL_INDEX_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*level*/
    { "bwm_level_up_index", BWM_LEVEL_UP_INDEX_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*port_status*/
    { "qos_rule_status", QOS_RULE_STATUS_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*x*/
    { "qos_rule_active", QOS_RULE_ACTIVE_VALUE, 
        NVRAM_NONE,
        0 
    },
	/*rule*/
    { "bwm_level", BWM_LEVEL_VALUE, 
        NVRAM_NONE,
        0 
    },
	/*rule*/
    { "stream_priority", STREAM_PRIORITY_VALUE, 
        NVRAM_NONE,
        0 
    },
	/*level*/
    { "sta_priority", STA_PRIORITY_VALUE, 
        NVRAM_NONE,
        0 
    },
	/*rule*/
    { "qos_rule_record", QOS_RULE_RECORD_VALUE, 
        NVRAM_NONE,
        0 
    },
	/*status^age*/
    { "igmp_snooping", IGMP_SNOOPING_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*voice^video^data^background*/
    { "tos_classify", TOS_CLASSIFY_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*voice^video^data^background*/
    { "cos_classify", COS_CLASSIFY_VALUE, 
        NVRAM_NONE,
        0 
    },
    /*(d_count^b_count^ipg_min^ipg_max^pl_min^pl_max)*/
    { "heuristics_classify", HEURISTICS_CLASSIFY_VALUE, 
        NVRAM_NONE,
        0 
    },
/* (status^voice^video^data^background) */
    { "tos_mark", TOS_MARK_VALUE,        
        NVRAM_NONE,
        0 
    },
/* (wlantolan^lanto2.4Gwlan^lanto5Gwlan) */
    { "root_bw", ROOT_BW_VALUE,        
        NVRAM_NONE,
        0 
    },
#endif
    { 0, 0, 0 ,0 }
};

#endif /*_NVRAM_EZPACKET_H */
