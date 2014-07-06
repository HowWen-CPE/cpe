/*
 *	inic.h -- Wireless Configuration Header 
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: firewall.h,v 1.5 2008-04-14 09:26:00 yy Exp $
 */
#ifndef _FIREWALL_H
#define _FIREWALL_H

#include "utils.h"

#define PROTO_UNKNOWN	0
#define PROTO_TCP		1
#define PROTO_UDP		2
#define PROTO_TCP_UDP	3
#define PROTO_ICMP		4
#define PROTO_NONE		5
#define PROTO_ALL		6


#define RULE_MODE_DISABLE	0
#define RULE_MODE_DROP		1
#define RULE_MODE_ACCEPT	2

#define ACTION_DROP		0
#define ACTION_ACCEPT	1

#define WEB_FILTER_CHAIN	"web_filter"
#define IPPORT_FILTER_CHAIN	"macipport_filter"

#define DMZ_CHAIN			"DMZ"
#define PORT_FORWARD_CHAIN	"port_forward"
#define PORT_FORWARD_POST_CHAIN	"port_forward_post" //Steve add 2009/02/23
#define DMZ_FORWARD_CHAIN		"DMZ_FORWARD"	//Steve add 2009/03/09
#define DMZ_POST_CHAIN		"DMZ_POST"	//Steve add 2009/12/08

//steve add for port trigger
#define PORT_TRIGGER_CHAIN	"port_trigger"
#define DOS_FILTER_CHAIN	"dos_block"
#define DOS_LOG_CHAIN	"dos_log"
#define NAT_APPLICATION_NUM 10
#define NAT_ADVANCE_NUM 12

#define MAGIC_NUM			"IPCHAINS"			

#define HTML_NO_FIREWALL_UNDER_BRIDGE_MODE	\
"<img src=\"/graphics/warning.gif\"><font color=#ff0000>&nbsp; &nbsp;Warning: The current operation mode is \"Bridge mode\" and these settings may not be functional.</font>"

void iptablesFilterClear(void);
void iptablesFilterRun(char *rule);
void iptablesWebsFilterRun(void);
void formDefineFirewall(void);
void firewall_init(void);
int isIpNetmaskValid(char *s);	// export for QoS
int isMacValid(char *);			// export for QoS
#define xstr(s) str(s)
#define str(s)  #s 


//william add for ipsec
enum {
	IPSEC_ENABLE = 0,
	IPSEC_RULE_NAME,
	IPSEC_RULE_ENABLE,
	IPSEC_RULE_MODE,
	IPSEC_RULE_L2TP,
	IPSEC_RULE_LOCAL_EXTIF,
	IPSEC_RULE_LOCAL_INIPADDR,
	IPSEC_RULE_LOCAL_NETMASK,
	IPSEC_RULE_REMOTE_GATEWAY,
	IPSEC_RULE_REMOTE_INIPADDR,
	IPSEC_RULE_REMOTE_NETMASK,
	IPSEC_RULE_CONN_INIT,
	IPSEC_RULE_IKE_KEYMODE,
	IPSEC_RULE_PSK,
	IPSEC_RULE_RSA,
	IPSEC_RULE_CA,
	IPSEC_RULE_ADV,
	IPSEC_RULE_PHASE1_MODE,
	IPSEC_RULE_PHASE1_ID,
	IPSEC_RULE_PHASE1_LIFETIME,
	IPSEC_RULE_PHASE2_LIFETIME,
	IPSEC_RULE_PHASE1_AUTH,
	IPSEC_RULE_PHASE1_ENCRYPT,
	IPSEC_RULE_PHASE1_GROUP,
	IPSEC_RULE_PHASE2_AUTH,
	IPSEC_RULE_PHASE2_ENCRYPT,
	IPSEC_RULE_PHASE2_GROUP,
	IPSEC_RULE_DPD_ENABLE,
	IPSEC_RULE_DPD_INTERVAL,
	IPSEC_RULE_DPD_TIMEOUT,
};

enum {
    PPTPD_ENABLE = 0,
    PPTPD_MTU,
    PPTPD_LOCALIP,
    PPTPD_LOCALIP_RANGE,
    PPTPD_REMOTEIP,
    PPTPD_REMOTEIP_RANGE,
    PPTPD_DNS_AUTO_ENABLE,
    PPTPD_DNS,
    PPTPD_CHAP_ENABLE2,
    PPTPD_MSCHAP_ENABLE2,
    PPTPD_MSCHAPV2_ENABLE2,
    PPTPD_EAP_ENABLE2,
    PPTPD_PAP_ENABLE2,
    PPTPD_MPPE128_ENABLE,
    PPTPD_PROXYARP_ENABLE,
    PPTPD_NAT_ENABLE,
    PPTPD_USERNAME,
    PPTPD_PASSWD
};

#define ARGV(args...) ((char *[]) { args, NULL })

static struct variable ipsec_variables[] = {
	{longname: "IPsec Enable", argv:ARGV("0", "1"), nullok: FALSE},
	{longname: "IPsec Rule Name", argv:ARGV("20"), nullok: FALSE},
	{longname: "IPsec Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
	{longname: "IPsec Rule Mode", argv:ARGV("net2net", "rw_client", "rw_server"), nullok: FALSE},
	{longname: "IPsec Rule L2TP", argv:ARGV("0", "1"), nullok: TRUE},
	{longname: "IPsec Rule Local External Interface", argv:ARGV("wan"), nullok: FALSE},
	{longname: "IPsec Rule Local Internal IP Address", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Local Netmask", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Remote Gateway", argv:ARGV("30"), nullok: TRUE},
	{longname: "IPsec Rule Remote IP Address", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Remote Netmask", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Connection Initiation", argv:ARGV("0", "1"), nullok: TRUE},
	{longname: "IPsec Rule IKE Key Mode", argv:ARGV("psk", "rsa", "caauth"), nullok: FALSE},
	{longname: "IPsec Rule PSK Key", argv:ARGV("8","40"), nullok: TRUE},
	{longname: "IPsec Rule RSA Key", argv:ARGV("20","1024"), nullok: TRUE},
	{longname: "IPsec Rule CA", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Advanced", argv:ARGV("0", "1"), nullok: TRUE },
	{longname: "IPsec Rule Phase1 Mode", argv:ARGV("main", "aggressive"), nullok: TRUE},
	{longname: "IPsec Rule Phase1 ID", argv:ARGV(""), nullok: TRUE},
	{longname: "IPsec Rule Phase1 Lifetime", argv:ARGV("1200", "86400"), nullok: TRUE},
	{longname: "IPsec Rule Phase2 Lifetime", argv:ARGV("1200", "86400"), nullok: TRUE},
	{longname: "IPsec Rule Phase1 Authentication", argv:ARGV("md5", "sha1"), nullok: TRUE},
	{longname: "IPsec Rule Phase1 Encryption", argv:ARGV("des", "3des", "aes128"), nullok: TRUE},
	{longname: "IPsec Rule Phase1 Group", argv:ARGV("dh1", "dh2", "dh5"), nullok: TRUE},
	{longname: "IPsec Rule Phase2 Authentication", argv:ARGV("md5", "sha1"), nullok: TRUE},
	{longname: "IPsec Rule Phase2 Encryption", argv:ARGV("null", "des", "3des", "aes128"), nullok: TRUE},
	{longname: "IPsec Rule Phase2 Group", argv:ARGV("none", "dh1", "dh2", "dh5"), nullok: TRUE},
	{longname: "IPsec Dead Peer Protection", argv:ARGV("0", "1"), nullok: TRUE},
	{longname: "IPsec Dead Peer Protection Interval", argv:ARGV("10", "1200"), nullok: TRUE},
	{longname: "IPsec Dead Peer Protection Timeout", argv:ARGV("30", "3600"), nullok: TRUE}
};

#define PPTPD_MAX_COUNT 254

static struct variable pptpd_variables[] = {
    {longname: "PPTPD Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MTU", argv:ARGV("600","1500"), nullok: FALSE},
    {longname: "PPTPD VPN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD VPN IP Address Range", 
            argv:ARGV("1", xstr(PPTPD_SESSION_MAX)), nullok: FALSE},
    {longname: "PPTPD Remote IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD Remote IP Address Range", 
            argv:ARGV("1", xstr(PPTPD_SESSION_MAX)), nullok: FALSE},
    {longname: "PPTPD Auto DNS", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD DNS", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD CHAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MSCHAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MSCHAP v2 Enable", argv:ARGV("0", "1"),
            nullok: FALSE},
    {longname: "PPTPD EAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD PAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MPPE128 Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD Proxy ARP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD NAT Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "PPTPD Password", argv:ARGV("20"), nullok: FALSE}
};


#endif // _FIREWALL_H
