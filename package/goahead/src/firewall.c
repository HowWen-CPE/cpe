/*
 *	firewall.c -- Firewall Settings 
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: firewall.c,v 1.24.2.1 2008-07-23 08:00:37 chhung Exp $
 */

/*
 *	if  WAN or LAN ip changed, we must restart firewall.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
//#include "nvram.h"
#include "utils.h"
#include "webs.h"
#include "firewall.h"
//#include "internet.h"
#include "nvram.h"
#include "nvram_rule.h"

#define TMP_LEN 256
#define SHORT_BUF_LEN   32

#if 1 //Steve24
#define DD printf("---> %d\n", __LINE__);

//#include	"config/autoconf.h" //user config //Steve add 2008/12/24
#define FIREWALL_DOS 
//#undef FIREWALL_DOS 
#define FIREWALL_DOS_LOG
//#undef FIREWALL_DOS_LOG 
int D_NAT_FLAG=0;//Steve add
int D_NAT_INDEX=0;//Steve add
/*one-to-one nat flag and rules index by wind 2013-12-09*/
static int gOne2OneNATFlag = 0; 
static int gOne2OneNATIdx = 0;

//Steve tmp
#undef DMZ_ICMP

//Steve add 2009/12/08
#define DMZ_POST_LOOPBACK

static void websSysFirewall(webs_t wp, char_t *path, char_t *query);


int parental_control_switch(int); //Steve add 2009/02/13
int firewall_switch(int);	//Steve add 2009/02/16

char l7name[8192];						// export it for internet.c qos
										// (The actual string is about 7200 bytes.)

int getGoAHeadServerPort(void);

int isMacValid(char *str)
{
	int i, len = strlen(str);
	if(len != 17)
		return 0;

	for(i=0; i<5; i++){
		if( (!isxdigit( str[i*3])) || (!isxdigit( str[i*3+1])) || (str[i*3+2] != ':') )
			return 0;
	}
	return (isxdigit(str[15]) && isxdigit(str[16])) ? 1: 0;
}

static int isIpValid(char *str)
{
	struct in_addr addr;	// for examination
	if( (! strcmp(T("any"), str)) || (! strcmp(T("any/0"), str)))
		return 1;

	if(! (inet_aton(str, &addr))){
		printf("isIpValid(): %s is not a valid IP address.\n", str);
		return 0;
	}
	return 1;
}

static int isNumOnly(char *str){
	int i, len = strlen(str);
	for(i=0; i<len; i++){
		if((str[i] >= '0' && str[i] <= '9'))
			continue;
		return 0;
	}
	return 1;
}

static int isAllNumAndSlash(char *str){
	int i, len = strlen(str);
	for(i=0; i<len; i++){
		if( (str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '/' )
			continue;
		return 0;
	}
	return 1;
}

static int isOnlyOneSlash(char *str)
{
	int i, count=0;
	int len = strlen(str);
	for(i=0; i<len; i++)
		if( str[i] == '/')
			count++;
	return count <= 1 ? 1 : 0;
}

int isIpNetmaskValid(char *s)
{
	char str[32];
	char *slash;
	struct in_addr addr;    // for examination

	if(!s || !strlen(s)){
		return 0;
	}

	strncpy(str, s, sizeof(str));

    if( (!strcmp("any", str)) || (!strcmp("any/0", str)))
        return 1;

	if (!isAllNumAndSlash(str)){
		return 0;
	}

	if(!isOnlyOneSlash(str)){
		return 0;
	}

	slash = strchr(str, '/');
	if(slash){
		int mask;

		*slash = '\0';
		slash++;
		if(!strlen(slash)){
			return 0;
		}

		if(!isNumOnly(slash)){
			return 0;
		}

		mask = atoi(slash);
		if(mask < 0 || mask > 32){
			return 0;
		}
	}

	if(! (inet_aton(str, &addr))){
        printf("isIpNetmaskValid(): %s is not a valid IP address.\n", str);
        return 0;
    }
    return 1;
}

//happiness add 2012-02-22
//only select ip is able to login
static int getSelectedip(int eid, webs_t wp, int argc, char_t **argv)
{
    char TempBuf[33], ret[50];
    ezplib_get_attr_val("http_rule", 0, "secipaddr", TempBuf, 33, EZPLIB_USE_CLI);
    sprintf(ret, "Only ip[%s] is able to login", TempBuf);
    websWrite(wp, T("%s"), ret);

    return 0;
}


static int getDMZEnableASP(int eid, webs_t wp, int argc, char_t **argv)
{
	#if 0
	int type, value;
	char *dmze = nvram_bufget(RT2860_NVRAM, "DMZEnable");
	if(dmze)
		value = atoi(dmze);
	else
		value = 0;

	if( ejArgs(argc, argv, T("%d"), &type) == 1){
		if(type == value)
			websWrite(wp, T("selected"));
		else
			websWrite(wp, T(" "));
		return 0;
	}
	#endif
	return -1;        
}

static int  getPortForwardEnableASP(int eid, webs_t wp, int argc, char_t **argv)
{
	#if 0
	int type, value;
	char *pfe = nvram_bufget(RT2860_NVRAM, "PortForwardEnable");

	if(pfe)
		value = atoi(pfe);
	else
		value = 0;

	if( ejArgs(argc, argv, T("%d"), &type) == 1){
		if(type == value)
			websWrite(wp, T("selected"));
		else
			websWrite(wp, T(" "));
		return 0;
	}
	#endif
	return -1;
}

/* Steve, for Port Trigger */
static int  getPortTriggerEnableASP(int eid, webs_t wp, int argc, char_t **argv)
{
	#if 0
	int type, value;
	char *pfe = nvram_bufget(RT2860_NVRAM, "PortTriggerEnable");

	if(pfe)
		value = atoi(pfe);
	else
		value = 0;

	if( ejArgs(argc, argv, T("%d"), &type) == 1){
		if(type == value)
			websWrite(wp, T("selected"));
		else
			websWrite(wp, T(" "));
		return 0;
	}
	#endif
	return -1;
}

static int  getIPPortFilterEnableASP(int eid, webs_t wp, int argc, char_t **argv)
{
	#if 0
	int type, value;
	char *pfe = nvram_bufget(RT2860_NVRAM, "IPPortFilterEnable");

	if(pfe)
		value = atoi(pfe);
	else
		value = 0;

	if( ejArgs(argc, argv, T("%d"), &type) == 1){
		if(type == value)
			websWrite(wp, T("selected"));
		else
			websWrite(wp, T(" "));
		return 0;
	}
	return -1;
	#endif
}

/*
 * hide the possible "error/warn" message when deleting a non-exist chain.
 */
static void iptablesForwardFilterClear(void)
{
	doSystem("iptables -F -t filter 1>/dev/null 2>&1");
}

/*
static void iptablesForwardFilterFlush(void)
{
	doSystem("iptables -t filter -F FORWARD  1>/dev/null 2>&1");
}
*/

static void iptablesIPPortFilterFlush(void){
	doSystem("iptables -F %s 1>/dev/null 2>&1", IPPORT_FILTER_CHAIN);
}

static void iptablesIPPortFilterClear(void){
	doSystem("iptables -D FORWARD -j %s 1>/dev/null 2>&1", IPPORT_FILTER_CHAIN);
	doSystem("iptables -F %s 1>/dev/null 2>&1", IPPORT_FILTER_CHAIN);
}

static void iptablesWebContentFilterClear(void){
	doSystem("iptables -D FORWARD -j %s  1>/dev/null 2>&1", WEB_FILTER_CHAIN);
	doSystem("iptables -F %s  1>/dev/null 2>&1", WEB_FILTER_CHAIN);
}

static void iptablesDMZFlush(void){
    doSystem("iptables -t nat -F %s 1>/dev/null 2>&1", DMZ_CHAIN);
    //Steve add 2009/12/07
    #ifdef DMZ_POST_LOOPBACK
	doSystem("iptables -t nat -F %s 1>/dev/null 2>&1", DMZ_POST_CHAIN);
	#endif
}

//Steve move
static void iptablesPortFWFilterFlush(void){
	doSystem("iptables -F %s  1>/dev/null 2>&1", PORT_FORWARD_CHAIN);
}

static void iptablesPortForwardFlush(void){
	iptablesPortFWFilterFlush();
    doSystem("iptables -t nat -F %s 1>/dev/null 2>&1", PORT_FORWARD_CHAIN);
    //Steve add 2009/02/23
	doSystem("iptables -t nat -F %s 1>/dev/null 2>&1", PORT_FORWARD_POST_CHAIN);
}

#ifdef FIREWALL_DOS 
/* Steve, DoS Filter */
static void iptablesDoSFilterFlush(void){
	doSystem("iptables -F %s  1>/dev/null 2>&1", DOS_FILTER_CHAIN);
	#ifdef FIREWALL_DOS_LOG
		doSystem("iptables -F %s  1>/dev/null 2>&1", DOS_LOG_CHAIN);
	#endif
}

static void iptablesDoSFilterClear(void){
	doSystem("iptables -X %s  1>/dev/null 2>&1", DOS_FILTER_CHAIN);
	#ifdef FIREWALL_DOS_LOG
	doSystem("iptables -X %s  1>/dev/null 2>&1", DOS_LOG_CHAIN);
	#endif
}
#endif


/* Steve, for Port Trigger */
static void iptablesPortTGFilterFlush(void){
	doSystem("iptables -F %s  1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
}

static void iptablesPortTGFilterClear(void){
	doSystem("iptables -X %s  1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
}


/* Steve, for Port Trigger */
static void iptablesPortTriggerFlush(void){
	iptablesPortTGFilterFlush();
	doSystem("iptables -t nat -F %s  1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
	//printf("Steve : %s\n",__FUNCTION__);
}

static void iptablesPortTriggerClear(void){
	doSystem("iptables -t nat -D PREROUTING -j %s  1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
	doSystem("iptables -t nat -F %s  1>/dev/null 2>&1; iptables -t nat -X %s  1>/dev/null 2>&1", PORT_TRIGGER_CHAIN, PORT_TRIGGER_CHAIN);
}

static void iptablesDMZClear(void){
	doSystem("iptables -t nat -D PREROUTING -j %s 1>/dev/null 2>&1", DMZ_CHAIN);	// remove rule in PREROUTING chain
	doSystem("iptables -t nat -F %s 1>/dev/null 2>&1; iptables -t nat -X %s  1>/dev/null 2>&1", DMZ_CHAIN, DMZ_CHAIN);
	//Steve add 2009/12/07
	#ifdef DMZ_POST_LOOPBACK
	doSystem("iptables -t nat -D POSTROUTING -j %s  1>/dev/null 2>&1", DMZ_POST_CHAIN);
	doSystem("iptables -t nat -F %s  1>/dev/null 2>&1; iptables -t nat -X %s  1>/dev/null 2>&1", DMZ_POST_CHAIN, DMZ_POST_CHAIN);
	#endif
}

static void iptablesPortForwardClear(void){
	doSystem("iptables -t nat -D PREROUTING -j %s 1>/dev/null 2>&1", PORT_FORWARD_CHAIN);
	doSystem("iptables -t nat -F %s  1>/dev/null 2>&1; iptables -t nat -X %s  1>/dev/null 2>&1", PORT_FORWARD_CHAIN, PORT_FORWARD_CHAIN);
	//Steve add 2009/02/23
	doSystem("iptables -t nat -D POSTROUTING -j %s  1>/dev/null 2>&1", PORT_FORWARD_POST_CHAIN);
	doSystem("iptables -t nat -F %s  1>/dev/null 2>&1; iptables -t nat -X %s  1>/dev/null 2>&1", PORT_FORWARD_POST_CHAIN, PORT_FORWARD_POST_CHAIN);
}

//Steve add 2008/12/25
static void iptablesPOSTROUTINGFlush(void){
    doSystem("iptables -t nat -F %s 1>/dev/null 2>&1", "POSTROUTING");
}

//Steve move
static void iptablesDMZForwardClear(void)
{
	doSystem("iptables -D FORWARD -j %s  1>/dev/null 2>&1", DMZ_FORWARD_CHAIN);
	doSystem("iptables -F %s  1>/dev/null 2>&1", DMZ_FORWARD_CHAIN);
}

static void iptablesAllFilterClear(void)
{
	printf("===================	%s  =============\n", __FUNCTION__);
	iptablesForwardFilterClear();
	iptablesIPPortFilterClear();
	iptablesWebContentFilterClear();
	
#ifdef FIREWALL_DOS
	iptablesDoSFilterClear();
#endif

#if 1 //define FIREWALL_PORTTRIGGER
    //Steve
    iptablesPortTGFilterClear();
#endif
	
#if 1 //Steve add 2009/03/09
    iptablesDMZForwardClear();
#endif
	
	#if 0
	  	//printf("	Steve test:==================> iptables -P INPUT ACCEPT\n\n");
	doSystem("iptables -P INPUT ACCEPT");
	doSystem("iptables -P OUTPUT ACCEPT");
	doSystem("iptables -P FORWARD ACCEPT");
	#else
		//printf("	Steve test:==================> iptables -P INPUT DROP\n\n");
		doSystem("iptables -P INPUT DROP");
		doSystem("iptables -P FORWARD DROP");
		doSystem("iptables -P OUTPUT ACCEPT");
	#endif
}

static void iptablesAllNATClear(void)
{
	printf("===================	%s  =============\n", __FUNCTION__);
      #if 1 //define FIREWALL_PORTTRIGGER
		iptablesPortTriggerClear(); //Steve add for port trigger
      #endif 
	
        iptablesPortForwardClear();
	iptablesDMZClear();
	doSystem("iptables -F -t nat 1>/dev/null 2>&1");
}

#if 0
char *insert(char *subs, int index, char *str, char delimit)
{
	int i=0;
	char *begin, *end;
	char *result = (char *)calloc(1, sizeof(char ) * (strlen(str) + strlen(subs) + 1 + 1));

	begin = str;
	end = strchr(str, delimit);
	while(end){
		if(i == index)
			break;
		begin = end;
		end = strchr(begin+1, delimit);
		i++;
	}
	if(begin == str){
		if(strlen(str) == 0){
			strcpy(result, subs);
		}else{
			if(index == 0){
				sprintf(result, "%s%c%s", subs, delimit, str);
			}else{
				sprintf(result, "%s%c%s", str, delimit, subs);
			}
		}
	}else if(end == NULL && i != index){
		sprintf(result, "%s%c%s", str, delimit, subs);
	}else{
		strncpy(result, str, begin - str);
		sprintf(result, "%s%c", result, delimit);
		strcat(result, subs);
		strcat(result, begin);
	}
	return result;
}

char *replace(char *subs, int index, char *str, char delimit)
{
	int del[1];
	char *result;
	char *dup = strdup(str);
	del[0] = index;
	deleteNthValueMulti(del, 1, dup, delimit);
	result = insert(subs, index, dup, delimit);
	free(dup);
	return result;
}
#endif
static int getNums(char *value, char delimit)
{
	char *pos = value;
    int count=1;
    if(!pos)
    	return 0;
	while( (pos = strchr(pos, delimit))){
		pos = pos+1;
		count++;
	}
	return count;
}

static void makeDMZRule(char *buf, int len, char *wan_name, char *ip_address)
{
	#if 0
	/* iptables -t nat -A PREROUTING -i br0 -j DNAT --to 5.6.7.8 */
	//snprintf(buf, len, "iptables -t nat -A %s -j DNAT -i %s -p udp --dport ! %d --to %s", DMZ_CHAIN, wan_name, getGoAHeadServerPort(), ip_address);
	//snprintf(buf, len, "iptables -t nat -A %s -j DNAT -i %s -p tcp --dport ! %d --to %s", DMZ_CHAIN, wan_name, getGoAHeadServerPort(), ip_address);
	
	//Steve modify 	2009/02/20,In order to fixed CDRouter
	char wan_ip[16];
	int rc;
	
	if ( getIfIp(wan_name, wan_ip) == -1){
    	 printf("No WAN IP --No run\n");
    	 //Steve modified for DMZ loopback
    	 //rc = snprintf(buf, len, "iptables -t nat -A %s -j DNAT -i %s --to %s", DMZ_CHAIN, wan_name, ip_address);
    	 //rc = snprintf(buf, len, "iptables -t nat -A %s -j DNAT --to %s", DMZ_CHAIN, ip_address);
    }else{
    	 //rc = snprintf(buf, len, "iptables -t nat -A %s -j DNAT -i %s -d %s --to %s", DMZ_CHAIN, wan_name, wan_ip, ip_address);
    	 //Steve modified for DMZ loopback
    	 rc = snprintf(buf, len, "iptables -t nat -A %s -j DNAT -d %s --to %s", DMZ_CHAIN, wan_ip, ip_address);
    }
    #endif
}

/*
 *
 */
static void makeIPPortFilterRule(char *buf, int len, char *mac_address,
char *sip_1, char *sip_2, int sprf_int, int sprt_int, 
char *dip_1, char *dip_2, int dprf_int, int dprt_int, int proto, int action)
{
	#if 0
		int rc = 0;
		char *pos = buf;
    char *spifw = nvram_bufget(RT2860_NVRAM, "SPIFWEnabled");

		switch(action){
		case ACTION_DROP:
		    if (atoi(spifw) == 0)
			rc = snprintf(pos, len-rc, 
				"iptables -A %s ", IPPORT_FILTER_CHAIN);
		    else
			rc = snprintf(pos, len-rc, 
				"iptables -A %s -m state --state NEW,INVALID ", IPPORT_FILTER_CHAIN);
			break;
		case ACTION_ACCEPT:
			rc = snprintf(pos, len-rc, 
				"iptables -A %s ", IPPORT_FILTER_CHAIN);
			break;
		}
		pos = pos + rc;

		// write mac address
		if(mac_address && strlen(mac_address)){
			rc = snprintf(pos, len-rc, "-m mac --mac-source %s ", mac_address);
			pos = pos+rc;
		}

		// write source ip
		rc = snprintf(pos, len-rc, "-s %s ", sip_1);
		pos = pos+rc;
		
		// write dest ip
		rc = snprintf(pos, len-rc, "-d %s ", dip_1);
		pos = pos+rc;

		// write protocol type
		if(proto == PROTO_NONE){
			rc = snprintf(pos, len-rc, " ");
			pos = pos + rc;
		}else if(proto == PROTO_ICMP){
			rc = snprintf(pos, len-rc, "-p icmp ");
			pos = pos + rc;
		}else{
			if(proto == PROTO_TCP)
				rc = snprintf(pos, len-rc, "-p tcp ");
			else if (proto == PROTO_UDP)
				rc = snprintf(pos, len-rc, "-p udp ");
			pos = pos + rc;

			// write source port
			if(sprf_int){
				if(sprt_int)
					rc = snprintf(pos, len-rc, "--sport %d:%d ", sprf_int, sprt_int);
				else
					rc = snprintf(pos, len-rc, "--sport %d ", sprf_int);
				pos = pos+rc;
			}

			// write dest port
			if(dprf_int){
				if(dprt_int)
					rc = snprintf(pos, len-rc, "--dport %d:%d ", dprf_int, dprt_int);
				else
					rc = snprintf(pos, len-rc, "--dport %d ", dprf_int);
				pos = pos+rc;
			}
		}

		switch(action){
		case ACTION_DROP:			// 1 == ENABLE--DROP mode
			rc = snprintf(pos, len-rc, "-j DROP");
			break;
		case ACTION_ACCEPT:			// 2 == ENABLE--ACCEPT mode
			rc = snprintf(pos, len-rc, "-j ACCEPT");
			break;
		}
		#endif
}

#if defined CONFIG_hp_NAT_APP
static void makePortForwardRule(char *buf, int len, char *wan_ip, char *ip_address, int proto, int prf_int, int prt_int)
{
		int rc = 0;
		char *pos = buf;
		char lan_ip[16];

        //Steve modified 2009/02/20
		//rc = snprintf(pos, len-rc, 
		//	"iptables -t nat -A %s -j DNAT -i %s ", PORT_FORWARD_CHAIN, wan_name);
		rc = snprintf(pos, len-rc, 
			"iptables -t nat -A %s -j DNAT -d %s ", PORT_FORWARD_CHAIN, wan_ip);
		//End Steve modified 
		
		pos = pos + rc;

		// write protocol type
		if(proto == PROTO_TCP)
			rc = snprintf(pos, len-rc, "-p tcp ");
		else if (proto == PROTO_UDP)
			rc = snprintf(pos, len-rc, "-p udp ");
		else if (proto == PROTO_TCP_UDP)
			rc = snprintf(pos, len-rc, " ");
		pos = pos + rc;

		// write port
		if(prt_int != 0)
			rc = snprintf(pos, len-rc, "--dport %d:%d ", prf_int, prt_int);
		else
			rc = snprintf(pos, len-rc, "--dport %d ", prf_int);
		pos = pos + rc;

		// write remote ip
		rc = snprintf(pos, len-rc, "--to %s ", ip_address);
		
#if 1
	/* Steve, Rules at FORWARD */
	pos = pos + rc;
	//rc = snprintf(pos, len-rc,
	//	";iptables -A %s -i %s -d %s -j ACCEPT ", PORT_FORWARD_CHAIN, getWanIfNamePPP(), ip_address);
	rc = snprintf(pos, len-rc,
		";iptables -A %s -d %s -j ACCEPT ", PORT_FORWARD_CHAIN, ip_address);
	pos = pos + rc;

	// write protocol type
	if(proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	if(prt_int != 0)
		rc = snprintf(pos, len-rc, "--dport %d:%d ", prf_int, prt_int);
	else
		rc = snprintf(pos, len-rc, "--dport %d ", prf_int);
#endif

#if 1
//For lookback function
/* Steve, Rules at POSTROUTING */
	pos = pos + rc;
	//rc = snprintf(pos, len-rc,
	//	";iptables -t nat -A %s -d %s -m mark --mark %d/%d -j SNAT ", PORT_FORWARD_POST_CHAIN, ip_address, 0x20, 0x20);
	rc = snprintf(pos, len-rc,
		";iptables -t nat -A %s -d %s -s %s/24 -j SNAT ", PORT_FORWARD_POST_CHAIN, ip_address, ip_address);
	pos = pos + rc;

	// write protocol type
	if(proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	if(prt_int != 0)
		rc = snprintf(pos, len-rc, "--dport %d:%d ", prf_int, prt_int);
	else
		rc = snprintf(pos, len-rc, "--dport %d ", prf_int);
	pos = pos + rc;

	// write ip of our lan ip
	if (getIfIp(getLanIfName(), lan_ip) == -1)
	       return;
	rc = snprintf(pos, len-rc, "--to %s ", lan_ip);
#endif
}

static void makePortForwardRule_MULTI(char *buf, int len, char *wan_ip, char *ip_address, int proto, char *multi_port)
{
		int rc = 0;
		char *pos = buf;
		char lan_ip[16];

        //Steve modified 2009/02/20
		//rc = snprintf(pos, len-rc, 
		//	"iptables -t nat -A %s -j DNAT -i %s ", PORT_FORWARD_CHAIN, wan_name);
		rc = snprintf(pos, len-rc, 
			"iptables -t nat -A %s -j DNAT -d %s ", PORT_FORWARD_CHAIN, wan_ip);
		//End Steve modified 
		
		pos = pos + rc;

		// write protocol type
		if(proto == PROTO_TCP)
			rc = snprintf(pos, len-rc, "-p tcp ");
		else if (proto == PROTO_UDP)
			rc = snprintf(pos, len-rc, "-p udp ");
		else if (proto == PROTO_TCP_UDP)
			rc = snprintf(pos, len-rc, " ");
		pos = pos + rc;

		// write port
		rc = snprintf(pos, len-rc, "-m multiport --dport %s ", multi_port);
		pos = pos + rc;

		// write remote ip
		rc = snprintf(pos, len-rc, "--to %s ", ip_address);
		
#if 1
	/* Steve, Rules at FORWARD */
	pos = pos + rc;
	//rc = snprintf(pos, len-rc,
	//	";iptables -A %s -i %s -d %s -j ACCEPT ", PORT_FORWARD_CHAIN, getWanIfNamePPP(), ip_address);
	rc = snprintf(pos, len-rc,
		";iptables -A %s -d %s -j ACCEPT ", PORT_FORWARD_CHAIN, ip_address);
	pos = pos + rc;

	// write protocol type
	if(proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	rc = snprintf(pos, len-rc, "-m multiport --dport %s ", multi_port);
#endif

#if 1
//For lookback function
/* Steve, Rules at POSTROUTING */
	pos = pos + rc;
	//rc = snprintf(pos, len-rc,
	//	";iptables -t nat -A %s -d %s -m mark --mark %d/%d -j SNAT ", PORT_FORWARD_POST_CHAIN, ip_address, 0x20, 0x20);
	rc = snprintf(pos, len-rc,
		";iptables -t nat -A %s -d %s -s %s/24 -j SNAT ", PORT_FORWARD_POST_CHAIN, ip_address, ip_address);
	pos = pos + rc;

	// write protocol type
	if(proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	rc = snprintf(pos, len-rc, "-m multiport --dport %s ", multi_port);
	pos = pos + rc;

	// write ip of our lan ip
	if (getIfIp(getLanIfName(), lan_ip) == -1)
	       return;
	rc = snprintf(pos, len-rc, "--to %s ", lan_ip);
#endif
}
#else
static void makePortForwardRule(char *buf, int len, char *wan_name, char *ip_address, int proto, int prf_int, int prt_int)
{
		int rc = 0;
		char *pos = buf;

		rc = snprintf(pos, len-rc, 
			"iptables -t nat -A %s -j DNAT -i %s ", PORT_FORWARD_CHAIN, wan_name);
		pos = pos + rc;

		// write protocol type
		if(proto == PROTO_TCP)
			rc = snprintf(pos, len-rc, "-p tcp ");
		else if (proto == PROTO_UDP)
			rc = snprintf(pos, len-rc, "-p udp ");
		else if (proto == PROTO_TCP_UDP)
			rc = snprintf(pos, len-rc, " ");
		pos = pos + rc;

		// write port
		if(prt_int != 0)
			rc = snprintf(pos, len-rc, "--dport %d:%d ", prf_int, prt_int);
		else
			rc = snprintf(pos, len-rc, "--dport %d ", prf_int);
		pos = pos + rc;

		// write remote ip
		rc = snprintf(pos, len-rc, "--to %s ", ip_address);
}
#endif

/* Steve, for Port Trigger */
static void makePortTriggerRule(char *buf, int len, char *wan_name, int in_proto, int in_prf_int, int in_prt_int, int tg_proto, int tg_prf_int, int tg_prt_int)
{
	char lan_ip[16];
	int rc = 0;
	char *pos = buf;

	rc = snprintf(pos, len-rc,
		"iptables -t nat -A %s -i %s -j TRIGGER --trigger-type dnat ", PORT_TRIGGER_CHAIN, wan_name);
	pos = pos + rc;

	// write protocol type
	if(in_proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (in_proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (in_proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	if(in_prt_int != 0)
		rc = snprintf(pos, len-rc, "--dport %d:%d ", in_prf_int, in_prt_int);
	else
		rc = snprintf(pos, len-rc, "--dport %d ", in_prf_int);
	pos = pos + rc;

#if 1
	/* Eason, Rules at FORWARD_WAN */
	rc = snprintf(pos, len-rc,
		";iptables -A %s -i %s -j TRIGGER --trigger-type in ", PORT_TRIGGER_CHAIN, wan_name);
	pos = pos + rc;

	// write protocol type
	if(in_proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (in_proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (in_proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	if(in_prt_int != 0)
		rc = snprintf(pos, len-rc, "--dport %d:%d ", in_prf_int, in_prt_int);
	else
		rc = snprintf(pos, len-rc, "--dport %d ", in_prf_int);
#endif
	/* Eason, Rules at PREROUTING_RULES */
	pos = pos + rc;
	rc = snprintf(pos, len-rc,
		";iptables -t nat -A %s -i %s -j TRIGGER --trigger-type out ", PORT_TRIGGER_CHAIN, getLanIfName());
	pos = pos + rc;

	// write protocol type
	if(tg_proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "-p tcp ");
	else if (tg_proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "-p udp ");
	else if (tg_proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, " ");
	pos = pos + rc;

	// write port
	if(tg_prt_int != 0)
		rc = snprintf(pos, len-rc, "--dport %d:%d ", tg_prf_int, tg_prt_int);
	else
		rc = snprintf(pos, len-rc, "--dport %d ", tg_prf_int);
	pos = pos + rc;

	// write protocol type
	if(in_proto == PROTO_TCP)
		rc = snprintf(pos, len-rc, "--trigger-proto tcp ");
	else if (in_proto == PROTO_UDP)
		rc = snprintf(pos, len-rc, "--trigger-proto udp ");
	else if (in_proto == PROTO_TCP_UDP)
		rc = snprintf(pos, len-rc, "--trigger-proto all ");
	pos = pos + rc;

	// write port
	if(in_prt_int != 0)
		rc = snprintf(pos, len-rc, "--trigger-match %d-%d --trigger-relate %d-%d ", in_prf_int, in_prt_int, in_prf_int, in_prt_int);
	else
		rc = snprintf(pos, len-rc, "--trigger-match %d --trigger-relate %d ", in_prf_int, in_prf_int);

}

static void iptablesRemoteManagementRun(void)
{
	#if 0
	char *rmE = nvram_bufget(RT2860_NVRAM, "RemoteManagement");
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *spifw = nvram_bufget(RT2860_NVRAM, "SPIFWEnabled");

    //printf("Steve test:%s\n",__FUNCTION__); 
	if(!opmode)
		return;

     //printf("Steve test:%s 603\n",__FUNCTION__); 
	// "Gateway mode" only
	if(strcmp(opmode , "1"))
		return;

    //printf("Steve test:%s 608\n",__FUNCTION__); 
	if(rmE && atoi(rmE) == 1)
		return;

    //printf("Steve test:%s 612\n",__FUNCTION__); 
	if (atoi(spifw) == 0)
		doSystem("iptables -A INPUT -i %s -j ACCEPT", getWanIfNamePPP());
	else{
		//doSystem("iptables -A INPUT -i %s -m state --state RELATED,ESTABLISHED -j ACCEPT", getWanIfNamePPP());				
		doSystem("iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT");
		doSystem("iptables -A INPUT -i !%s -j ACCEPT", getWanIfNamePPP());
	}
	//printf("Steve test:%s 618\n",__FUNCTION__); 
	//printf("getOnePortOnly():%d",getOnePortOnly());
	
	if(getOnePortOnly()){
		// make the web server to be reachable.
		if (atoi(spifw) == 0)
			doSystem("iptables -A INPUT -i %s -d 172.32.1.254 -p tcp --dport 80 -j ACCEPT", getWanIfNamePPP());
		else
			doSystem("iptables -A INPUT -i %s -m state -d 172.32.1.254 -p tcp --dport 80 --state NEW,INVALID -j ACCEPT", getWanIfNamePPP());
	}
#if 0	
	//printf("Steve test:%s 628\n",__FUNCTION__); 
	if (atoi(spifw) == 0)
		doSystem("iptables -A INPUT -i %s -p tcp --dport 80 -j DROP", getWanIfNamePPP());
	else
		doSystem("iptables -A INPUT -i %s -m state -p tcp --dport 80 --state NEW,INVALID -j DROP", getWanIfNamePPP());
#endif
    //printf("Steve test:%s 633\n",__FUNCTION__); 
    #endif
	return;
}

//Steve move
#ifdef FIREWALL_DOS
static void iptablesDoSFilterRun(void)
{
#if 0
#ifdef FIREWALL_DOS_LOG

	char *firewallL = nvram_bufget(RT2860_NVRAM, "FirewallLog");
	
if(atoi(firewallL)==1){
    doSystem("iptables -A %s -j LOG --log-prefix \"Firewall: \" --log-level 6", DOS_LOG_CHAIN);
    doSystem("iptables -A %s -j DROP", DOS_LOG_CHAIN);
            // NMAP FIN/URG/PSH
    doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN,URG,PSH -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL ALL -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// Another Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL SYN,RST,ACK,FIN,URG -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// Null Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags ALL NONE -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// SYN/RST
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,RST SYN,RST -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// SYN/FIN -- Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,FIN SYN,FIN -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
			// FIN Stealth
	doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	//new add
	doSystem("iptables -A %s -p tcp --tcp-flags FIN,RST FIN,RST -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,FIN FIN -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,URG URG -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,PSH PSH -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	
	#if 1
	doSystem("iptables -A %s -m state --state INVALID -j DROP", DOS_FILTER_CHAIN);
	doSystem("iptables -A %s -p tcp ! --syn -m state --state NEW -j DROP", DOS_FILTER_CHAIN);
	#else
	doSystem("iptables -A %s -m state --state INVALID -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
	doSystem("iptables -A %s -p tcp ! --syn -m state --state NEW -j %s", DOS_FILTER_CHAIN, DOS_LOG_CHAIN);
    #endif
}
else{
	doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN,URG,PSH -j DROP", DOS_FILTER_CHAIN);
			// Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL ALL -j DROP", DOS_FILTER_CHAIN);
			// Another Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL SYN,RST,ACK,FIN,URG -j DROP", DOS_FILTER_CHAIN);
			// Null Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags ALL NONE -j DROP", DOS_FILTER_CHAIN);
			// SYN/RST
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,RST SYN,RST -j DROP", DOS_FILTER_CHAIN);
			// SYN/FIN -- Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,FIN SYN,FIN -j DROP", DOS_FILTER_CHAIN);
			// FIN Stealth
	doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN -j DROP", DOS_FILTER_CHAIN);
	//new add
	doSystem("iptables -A %s -p tcp --tcp-flags FIN,RST FIN,RST -j DROP", DOS_FILTER_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,FIN FIN -j DROP", DOS_FILTER_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,URG URG -j DROP", DOS_FILTER_CHAIN);
	doSystem("iptables -A %s -p tcp --tcp-flags ACK,PSH PSH -j DROP", DOS_FILTER_CHAIN);
	
	doSystem("iptables -A %s -m state --state INVALID -j DROP", DOS_FILTER_CHAIN);
	doSystem("iptables -A %s -p tcp ! --syn -m state --state NEW -j DROP", DOS_FILTER_CHAIN);
	
}
#else	
	        // NMAP FIN/URG/PSH
	doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN,URG,PSH -j DROP", DOS_FILTER_CHAIN);
			// Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL ALL -j DROP", DOS_FILTER_CHAIN);
			// Another Xmas Tree
	doSystem("iptables -A %s -p tcp --tcp-flags ALL SYN,RST,ACK,FIN,URG -j DROP", DOS_FILTER_CHAIN);
			// Null Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags ALL NONE -j DROP", DOS_FILTER_CHAIN);
			// SYN/RST
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,RST SYN,RST -j DROP", DOS_FILTER_CHAIN);
			// SYN/FIN -- Scan(possibly)
	doSystem("iptables -A %s -p tcp --tcp-flags SYN,FIN SYN,FIN -j DROP", DOS_FILTER_CHAIN);
			// FIN Stealth
	doSystem("iptables -A %s -p tcp --tcp-flags ALL FIN -j DROP", DOS_FILTER_CHAIN);
#endif

#if 0 //2009/01/19 it should be with other rules   
	doSystem("iptables -A %s -p tcp --syn -m state --state NEW -j DROP", DOS_FILTER_CHAIN);
		
	doSystem("iptables -A %s -p udp -j DROP", DOS_FILTER_CHAIN);;
		
	doSystem("iptables -A %s -p icmp --icmp-type echo-request -j DROP", DOS_FILTER_CHAIN);
#endif
#endif
}
#endif

//2009/02/10 modified
//Steve add for WAN Service base  on iptablesRemoteManagementRun
static void iptablesWANServiceRun(void)
{
	#if 0
	char *rmE = nvram_bufget(RT2860_NVRAM, "RemoteManagement");
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *spifw = nvram_bufget(RT2860_NVRAM, "SPIFWEnabled");
	char *wpfE = nvram_bufget(RT2860_NVRAM, "WANPingFilter");
	char *rmPort = nvram_bufget(RT2860_NVRAM, "RemoteManagementPort");
   	char wan_ip[16];
   	char *rmS = nvram_bufget(RT2860_NVRAM, "RemoteManagementSel");
   	char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
   	
   	char *rmSecuredSel = nvram_bufget(RT2860_NVRAM, "RemoteManagementSecuredSel");
   	char *rmSecuredIP =  nvram_bufget(RT2860_NVRAM, "RemoteManagementSecuredIP");

   // TODO: make a new chain instead of flushing the INPUT chain
	doSystem("iptables -t filter -F INPUT"); 

	//Steve add 2009/01/19
	
#ifdef	FIREWALL_DOS	
char *firewallE = nvram_bufget(RT2860_NVRAM, "FIREWALLEnabled");
	iptablesDoSFilterFlush();
	doSystem("iptables -t filter -A INPUT -j %s 1>/dev/null 2>&1", DOS_FILTER_CHAIN);
    if(atoi(firewallE) == 1){
    	iptablesDoSFilterRun();
    }
#endif   

   if(atoi(wpfE) == 0){	//Disable	
		printf("Disable WAN Ping\n");
	}else{							
		printf("Accept WAN Ping\n");
		doSystem("iptables -t filter -A INPUT -i %s -p icmp -j ACCEPT", getWanIfNamePPP());
	} 

	if(!opmode)
		return;

    //Steve modified 2009/01/2        
	
	#if 1
	    // "Gateway mode" only  Router and WISP
	    // Bridge return
		if(!(strcmp(opmode , "0")))
			return;
	#else
	    // "Gateway mode" only
		if(strcmp(opmode , "1"))
			return;
    #endif
    
    
	
	//Steve add LAN WEB 2009/02/10
	//only allow wan aceess via web, so reject lan via web
	if (!(strcmp(rmS , "2"))){ // only WAN
		doSystem("iptables -A INPUT -i %s -d %s -p tcp --dport %s -j DROP", getLanIfName(),lan_ip,rmPort);
	}
	
	//remote management
	if ((!(strcmp(rmS , "0"))) || (!(strcmp(rmS , "1")))){ // (LAN & WAN), LAN
		//Secured Client IP Address 2009/02/11
		if (!(strcmp(rmSecuredSel , "1")))
			doSystem("iptables -A INPUT -i %s -s ! %s -p tcp --dport %s -j DROP",getLanIfName(),rmSecuredIP,rmPort);		
	}
	
	if ((!(strcmp(rmS , "0"))) || (!(strcmp(rmS , "2")))){ // (LAN & WAN), WAN
		//Steve add for wan remote	
		doSystem("iptables -A INPUT -i %s -p tcp -d %s --dport %s -j ACCEPT",getWanIfNamePPP(),lan_ip,rmPort);
	}
	else{		
		//Steve add 2009/03/07 change order
    	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    		 printf("WAN IP error\n");
    		 //return;	 
    	}
   	 	else{ 
    		//printf("wan_ip:%s\n",wan_ip);  
    		doSystem("iptables -A INPUT -i %s -d %s -p tcp --dport %s -j DROP", getWanIfNamePPP(),wan_ip,rmPort);
		}	
	}
	
	//Steve add LAN WEB 2009/02/10
	//if((!(strcmp(rmS , "0"))) || (!(strcmp(rmS , "2")))){
	//	printf("WAN remote management enable \n");
	//	return;
	//}
	
    //? 
	//if(rmE && atoi(rmE) == 1)
	//	return;
	   
    //Steve mark  
	if (atoi(spifw) == 0){
		// need to check
		//doSystem("iptables -A INPUT -i %s -j ACCEPT", getWanIfNamePPP());
		doSystem("iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT");
		doSystem("iptables -A INPUT -i ! %s -j ACCEPT", getWanIfNamePPP());
	}
	else{
		//doSystem("iptables -A INPUT -i %s -m state --state RELATED,ESTABLISHED -j ACCEPT", getWanIfNamePPP());
		doSystem("iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT");
		doSystem("iptables -A INPUT -i ! %s -j ACCEPT", getWanIfNamePPP());
	}
	//printf("Steve test:%s 618\n",__FUNCTION__); 
	//printf("getOnePortOnly():%d",getOnePortOnly());

#if 0	
	if(getOnePortOnly()){
		// make the web server to be reachable.
		if (atoi(spifw) == 0)
			doSystem("iptables -A INPUT -i %s -d 172.32.1.254 -p tcp --dport 80 -j ACCEPT", getWanIfNamePPP());
		else
			doSystem("iptables -A INPUT -i %s -m state -d 172.32.1.254 -p tcp --dport 80 --state NEW,INVALID -j ACCEPT", getWanIfNamePPP());
	}
#endif
	
//because default is set DROP	
#if 0
	if (atoi(spifw) == 0){
		// need to check
		//doSystem("iptables -A INPUT -i %s -p tcp --dport 80 -j DROP", getWanIfNamePPP());
		doSystem("iptables -A INPUT -i %s -m state -p tcp --dport 80 --state NEW,INVALID -j DROP", getWanIfNamePPP());
	}
	else
		doSystem("iptables -A INPUT -i %s -m state -p tcp --dport 80 --state NEW,INVALID -j DROP", getWanIfNamePPP());
#endif
    #endif
	return;
}

//2009/01/13
static void iptablesRemoteManagementRun_Clear(int flag)
{
	#if 0
	char *rmE = nvram_bufget(RT2860_NVRAM, "RemoteManagement");
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
	char *rmPort = nvram_bufget(RT2860_NVRAM, "RemoteManagementPort");
	char wan_ip[16]={0};
 
	if(!opmode)
		return;
//Steve modified 2009/01/23
#if 1
	// "Gateway mode" only  Router and WISP
	// Bridge return
	if(!(strcmp(opmode , "0"))){
		printf("Not Gateway \n");
		return;
	}
#else
	// "Gateway mode" only
	if(strcmp(opmode , "1")){
		printf("Not Gateway\n");
		return;
	}
#endif
		
	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    	 printf("WAN IP error\n");
    	 return;	 
    }	

if(flag){
	if (atoi(rmE) == 1)
		doSystem("iptables -t nat -I PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:80"
				, getWanIfNamePPP(), wan_ip, rmPort, lan_ip);
}else{
		doSystem("iptables -t nat -D PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:80"
				, getWanIfNamePPP(), wan_ip, rmPort, lan_ip);
}
      
    #endif
	return;
}

//2009/02/09
static void iptablesRemoteManagementWeb(int flag)
{
	#if 0
	char *rmS = nvram_bufget(RT2860_NVRAM, "RemoteManagementSel");
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
	char *rmPort = nvram_bufget(RT2860_NVRAM, "RemoteManagementPort");
	char wan_ip[16]={0};
 
    char *rmSecuredSel = nvram_bufget(RT2860_NVRAM, "RemoteManagementSecuredSel");
    char *rmSecuredIP =  nvram_bufget(RT2860_NVRAM, "RemoteManagementSecuredIP");
    
     
	if(!opmode)
		return;

	// "Gateway mode" only  Router and WISP
	// Bridge return
	if(!(strcmp(opmode , "0"))){
		printf("Not Gateway \n");
		return;
	}
		
	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    	 printf("WAN IP error\n");
    	 return;	 
    }	

if(flag){
	if (!(strcmp(rmSecuredSel , "1"))){
		if ((atoi(rmS) == 0) || (atoi(rmS) == 2))
			doSystem("iptables -t nat -I PREROUTING -p tcp -i %s -d %s -s %s --dport %s -j DNAT --to %s:%s"
				, getWanIfNamePPP(), wan_ip, rmSecuredIP, rmPort, lan_ip, rmPort);
	}
	else{
	if ((atoi(rmS) == 0) || (atoi(rmS) == 2))
		doSystem("iptables -t nat -I PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:%s"
				, getWanIfNamePPP(), wan_ip, rmPort, lan_ip, rmPort);
	}
}else{
	if (!(strcmp(rmSecuredSel , "1"))){
		if ((atoi(rmS) == 0) || (atoi(rmS) == 2))
			doSystem("iptables -t nat -D PREROUTING -p tcp -i %s -d %s -s %s --dport %s -j DNAT --to %s:%s"
				, getWanIfNamePPP(), wan_ip, rmSecuredIP, rmPort, lan_ip, rmPort);
		
}else{
		doSystem("iptables -t nat -D PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:%s"
				, getWanIfNamePPP(), wan_ip, rmPort, lan_ip, rmPort);
}
}
      #endif
	return;
}

static void iptablesDMZRun(void)
{
	#if 0
	char wan_ip[16];
	char cmd[1024], *ip_address;
	char *dmz_enable = nvram_bufget(RT2860_NVRAM, "DMZEnable");
	if(!dmz_enable){
		printf("Warning: can't find \"DMZEnable\" in flash\n");
		return;
	}
	if(!atoi(dmz_enable))
		return;

	ip_address = nvram_bufget(RT2860_NVRAM, "DMZIPAddress");
	if(!ip_address){
		printf("Warning: can't find \"DMZIPAddress\" in flash\n");
		return;
	}
	
	//Steve add 2009/12/07
	#ifdef DMZ_POST_LOOPBACK
	char lan_ip[16];    
    if ( getIfIp(getLanIfName(), lan_ip) == -1){
    	 printf("LAN IP error\n");
    	 return;	 
    }
    #endif

    //Steve midified 2010/03/15
	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
       printf("WAN IP error - NO run DMZ\n");
       return;
    }
	makeDMZRule(cmd, sizeof(cmd), getWanIfNamePPP(), ip_address);
	doSystem(cmd);
	//Steve add 2009/12/07
	#ifdef DMZ_POST_LOOPBACK
	doSystem("iptables -t nat -A " DMZ_POST_CHAIN " -d %s -s %s/24 -j SNAT  --to %s", ip_address , ip_address, lan_ip);
	#endif
	#endif
	return;
}

static void iptablesIPPortFilterRun(void)
{
#if 0	
	int i=0;
	char rec[256];
	char cmd[1024];
	int sprf_int, sprt_int, proto, action;
	int dprf_int, dprt_int;
	char sprf[8], sprt[8], protocol[8];
	char dprf[8], dprt[8];
	char mac_address[32];
	char sip_1[32], sip_2[32], action_str[4];
	char dip_1[32], dip_2[32];
    char *firewall_enable, *default_policy, *rule;
    char *spifw = nvram_bufget(RT2860_NVRAM, "SPIFWEnabled");
    int mode;
	
    firewall_enable = nvram_bufget(RT2860_NVRAM, "IPPortFilterEnable");
    if(!firewall_enable){
        printf("Warning: can't find \"IPPortFilterEnable\" in flash.\n");
        return;
    }
    mode = atoi(firewall_enable);
    if(!mode)
		return;

	rule = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
	if(!rule){
		printf("Warning: can't find \"IPPortFilterRules\" in flash.\n");
		return;
	}

	default_policy = nvram_bufget(RT2860_NVRAM, "DefaultFirewallPolicy");
	// add the default policy to the end of FORWARD chain
	if(!default_policy)
		default_policy = "0";

	if(atoi(default_policy) == 1){
		//the default policy is drop
	    if (atoi(spifw) == 0)
			;//doSystem("iptables -t filter -A %s -j ACCEPT", IPPORT_FILTER_CHAIN);
	    else
		doSystem("iptables -t filter -A %s -m state --state RELATED,ESTABLISHED -j ACCEPT", IPPORT_FILTER_CHAIN);
	}

	while( (getNthValueSafe(i++, rule, ';', rec, sizeof(rec)) != -1) ){
        // get sip 1
        if((getNthValueSafe(0, rec, ',', sip_1, sizeof(sip_1)) == -1)){
			continue;
		}
		if(!isIpNetmaskValid(sip_1)){
			continue;
		}

		// we dont support ip range yet.
        // get sip 2
        //if((getNthValueSafe(1, rec, ',', sip_2, sizeof(sip_2)) == -1))
        //	continue;
		//if(!isIpValid(sip_2))
		//	continue;

		// get source port range "from"
		if((getNthValueSafe(2, rec, ',', sprf, sizeof(sprf)) == -1)){
			continue;
		}
		if( (sprf_int = atoi(sprf)) > 65535)
			continue;
		// get dest port range "to"
		if((getNthValueSafe(3, rec, ',', sprt, sizeof(sprt)) == -1)){
			continue;
		}
		if( (sprt_int = atoi(sprt)) > 65535)
			continue;

		// Destination Part
        // get dip 1
		if((getNthValueSafe(4, rec, ',', dip_1, sizeof(dip_1)) == -1)){
			continue;
		}
		if(!isIpNetmaskValid(dip_1)){
			continue;
		}
		// we dont support ip range yet
        // get sip 2
        //if((getNthValueSafe(5, rec, ',', dip_2, sizeof(dip_2)) == -1))
        //    continue;
        //if(!isIpValid(dip_2))
        //    continue;

		// get source port range "from"
		if((getNthValueSafe(6, rec, ',', dprf, sizeof(dprf)) == -1)){
			continue;
		}
		if( (dprf_int = atoi(dprf)) > 65535)
			continue;

		// get dest port range "to"
		if((getNthValueSafe(7, rec, ',', dprt, sizeof(dprt)) == -1)){
			continue;
		}
		if( (dprt_int = atoi(dprt)) > 65535)
			continue;


		// get protocol
		if((getNthValueSafe(8, rec, ',', protocol, sizeof(protocol)) == -1))
			continue;
		proto = atoi(protocol);

		// get action
        if((getNthValueSafe(9, rec, ',', action_str, sizeof(action_str)) == -1)){
            continue;
        }
        action = atoi(action_str);

        // getNthValueSafe(10) is "comment".

        // get mac address
        if((getNthValueSafe(11, rec, ',', mac_address, sizeof(mac_address)) == -1))
            continue;
		if(strlen(mac_address)){
	        if(!isMacValid(mac_address))
	        	continue;
		}

        //TODO:
		// supposed to do validation here but  we didn't do it because code size.
/*
# iptables example
# iptables -t nat -A POSTROUTING -o eth0  -s 10.10.10.0/24 -j MASQUERADE
# iptables -A FORWARD -m physdev --physdev-in ra0 --physdev-out eth2 -m state --state ESTABLISHED,RELATED -j ACCEPT
# iptables -A FORWARD -m physdev --physdev-in eth0 --physdev-out eth2 -j DROP
# iptables -A FORWARD -i eth0 -o eth2 -j DROP
# iptables -A FORWARD -p tcp --dport 139 -j DROP
# iptables -A FORWARD -i eth0 -o eth2 -m state --state NEW,INVALID -p tcp --dport 80 -j DROP
*/
		makeIPPortFilterRule(cmd, sizeof(cmd), mac_address, sip_1, sip_2, sprf_int, sprt_int, dip_1, dip_2, dprf_int, dprt_int, proto, action);
		doSystem(cmd);
	}


	switch(atoi(default_policy)){
	case 0:
		doSystem("iptables -t filter -A %s -j ACCEPT", IPPORT_FILTER_CHAIN);
		break;
	case 1:
		doSystem("iptables -t filter -A %s -j DROP", IPPORT_FILTER_CHAIN);
		break;
	}

#endif
}



//#ifdef CONFIG_hp_NAT_APP
#if defined CONFIG_hp_NAT_APP
#if 1
static void iptablesPortForwardRun(void)
{
		printf("Enter %s\n",__FUNCTION__);
		
		int i=0;
		char rec[256];
		char cmd[1024];
		char wan_name[16];
	
		int prf_int, prt_int, proto;
		char rule_name[20], rule_name2[20];
		char str_en[2], str_name[32], str_ip[32];
	    char *firewall_enable, *rule,*rule2;
	
		char wan_ip[16];
		
		//Steve
		char str_rule2[300];
	    int j=0;
	    
	    strncpy(wan_name, getWanIfNamePPP(), sizeof(wan_name)-1);
	    if ( getIfIp(wan_name, wan_ip) == -1)
			return;
	
	   for(i=0;i<NAT_APPLICATION_NUM;i++){
			sprintf(rule_name, "%s%d", "PortForwardRules", i+1);
			sprintf(rule_name2, "%s%d", "PortForwardPortNum", i+1);
		
			//rule = nvram_bufget(RT2860_NVRAM, rule_name);
			//rule2 = nvram_bufget(RT2860_NVRAM, rule_name2);
	    	if(!strlen(rule)){
	            //printf("string is null\n");
	            continue;
	    	 }
	    	 else{
	    	 	//Multiple port rule
	    	 	if(strchr(rule2, '-') || strchr(rule2, ',')){
					//printf("\nMultiple port rule\n");
					if((getNthValueSafe(0, rule, ',', str_en, sizeof(str_en)) == -1)){			
					continue;
					}
					
					if((getNthValueSafe(1, rule, ',', str_name, sizeof(str_name)) == -1)){				
					continue;
					}
					
					if((getNthValueSafe(2, rule, ',', str_ip, sizeof(str_ip)) == -1)){	
					continue;
					}
					
					if( !strcmp(str_en, "1")){
						memset(str_rule2,0,sizeof(str_rule2));
						memcpy(str_rule2,rule2,strlen(rule2));
						for(j=0;j < strlen(rule2);j++){
						   if(str_rule2[j] == '-')
						      str_rule2[j]= ':';
						}
						printf("str_rule2=%s strlen=%d\n",str_rule2,strlen(str_rule2));
						makePortForwardRule_MULTI(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_UDP, str_rule2);
						doSystem(cmd);
						makePortForwardRule_MULTI(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_TCP, str_rule2);
						doSystem(cmd);
					}
				}
	    	 	else{
	    	 		//Single port rule
	    	 		//printf("\nSingle port rule\n");
	    	 		if((getNthValueSafe(0, rule, ',', str_en, sizeof(str_en)) == -1)){			
					continue;
					}
					
					if((getNthValueSafe(1, rule, ',', str_name, sizeof(str_name)) == -1)){				
					continue;
					}
					
					if((getNthValueSafe(2, rule, ',', str_ip, sizeof(str_ip)) == -1)){	
					continue;
					}
					
					if( (prf_int = atoi(rule2)) > 65535)
					continue;
					
					if( !strcmp(str_en, "1")){
						if((prf_int != 53) && (prf_int != 69) && (prf_int != 161)){
							makePortForwardRule(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_TCP, prf_int, 0);
							doSystem(cmd);
						}
						
						if((prf_int != 21) && (prf_int != 23) && (prf_int != 25) && (prf_int != 79) && (prf_int != 80) && (prf_int != 110)){
							makePortForwardRule(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_UDP, prf_int, 0);
							doSystem(cmd);
						}
					}
				}			
	    	 }
		} 
			
		
}
#else
static void iptablesPortForwardRun(void)
{
	printf("Enter %s\n",__FUNCTION__);
	
	int i=0;
	char rec[256];
	char cmd[1024];
	char wan_name[16];

	int prf_int, prt_int, proto;
	char rule_name[20], rule_name2[20];
	char str_en[2], str_name[32], str_ip[32];
    char *firewall_enable, *rule,*rule2;

	char wan_ip[16];

    strncpy(wan_name, getWanIfNamePPP(), sizeof(wan_name)-1);
    if ( getIfIp(wan_name, wan_ip) == -1)
		return;

   for(i=0;i<NAT_APPLICATION_NUM;i++){
		
		sprintf(rule_name, "%s%d", "PortForwardRules", i+1);
		sprintf(rule_name2, "%s%d", "PortForwardPortNum", i+1);
	
		//rule = nvram_bufget(RT2860_NVRAM, rule_name);
		//rule2 = nvram_bufget(RT2860_NVRAM, rule_name2);
    	if(!strlen(rule)){
            //printf("string is null\n");
            continue;
    	 }
    	 else
    	 {
    	 	if((getNthValueSafe(0, rule, ',', str_en, sizeof(str_en)) == -1)){			
			continue;
			}
			
			if((getNthValueSafe(1, rule, ',', str_name, sizeof(str_name)) == -1)){				
			continue;
			}
			
			if((getNthValueSafe(2, rule, ',', str_ip, sizeof(str_ip)) == -1)){	
			continue;
			}
			
			if( (prf_int = atoi(rule2)) > 65535)
			continue;
			
			if( !strcmp(str_en, "1")){
				if((prf_int != 53) && (prf_int != 69) && (prf_int != 161)){
					makePortForwardRule(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_TCP, prf_int, 0);
					doSystem(cmd);
				}
				
				if((prf_int != 21) && (prf_int != 23) && (prf_int != 25) && (prf_int != 79) && (prf_int != 80) && (prf_int != 110)){
					makePortForwardRule(cmd, sizeof(cmd), wan_ip, str_ip, PROTO_UDP, prf_int, 0);
					doSystem(cmd);
				}
			}
			
    	 }

	} 
		
	
}
#endif
#else
static void iptablesPortForwardRun(void)
{
	int i=0;
	char rec[256];
	char cmd[1024];
	char wan_name[16];

	int prf_int, prt_int, proto;
	char ip_address[32], prf[8], prt[8], protocol[8];

    char *firewall_enable, *rule;


    printf("\n\n**************************%s\n\n",__FUNCTION__);

    //firewall_enable = nvram_bufget(RT2860_NVRAM, "PortForwardEnable");
    if(!firewall_enable){
        printf("Warning: can't find \"PortForwardEnable\" in flash\n");
        return;
    }
    if(atoi(firewall_enable)){
        //rule = nvram_bufget(RT2860_NVRAM, "PortForwardRules");
        if(!rule){
            printf("Warning: can't find \"PortForwardRules\" in flash\n");
            return ;
        }
    }else
		return;

//  if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1)
//      return;
	strncpy(wan_name, getWanIfNamePPP(), sizeof(wan_name)-1);

	while( (getNthValueSafe(i++, rule, ';', rec, sizeof(rec)) != -1) ){
		// get ip address
		if((getNthValueSafe(0, rec, ',', ip_address, sizeof(ip_address)) == -1)){
			printf("prf = %s\n", prf);	
			continue;
		}
		if(!isIpValid(ip_address))
			continue;

		// get port range "from"
		if((getNthValueSafe(1, rec, ',', prf, sizeof(prf)) == -1)){
			printf("prf = %s\n", prf);	
			continue;
		}
		if( (prf_int = atoi(prf)) == 0 || prf_int > 65535)
			continue;

		// get port range "to"
		if((getNthValueSafe(2, rec, ',', prt, sizeof(prt)) == -1)){
			printf("prt = %s\n", prt);	
			continue;
		}
		if( (prt_int = atoi(prt)) > 65535)
			continue;

		// get protocol
		if((getNthValueSafe(3, rec, ',', protocol, sizeof(protocol)) == -1))
			continue;
		proto = atoi(protocol);
		switch(proto){
			case PROTO_TCP:
			case PROTO_UDP:
				makePortForwardRule(cmd, sizeof(cmd), wan_name, ip_address, proto, prf_int, prt_int);
				doSystem(cmd);
				break;
			case PROTO_TCP_UDP:
				makePortForwardRule(cmd, sizeof(cmd), wan_name, ip_address, PROTO_TCP, prf_int, prt_int);
				doSystem(cmd);
				makePortForwardRule(cmd, sizeof(cmd), wan_name, ip_address, PROTO_UDP, prf_int, prt_int);
				doSystem(cmd);
				break;
			default:
				continue;
		}
	}
}
#endif




#if defined CONFIG_hp_NAT_APP
/* Steve, for Port Trigger for hp*/
static void iptablesPortTriggerRun(void)
{
	int i=0;
	char *rec;
	char cmd[1024];
	char wan_name[16];

	//int prf_int, prt_int, proto;
	int in_prf_int, in_prt_int, in_proto;
	int tg_prf_int, tg_prt_int, tg_proto;
	//char ip_address[32], prf[8], prt[8], protocol[8];
	char in_prf[8], in_prt[8], in_protocol[8];
	char tg_prf[8], tg_prt[8], tg_protocol[8];

    char *firewall_enable, *rule;
    
    char str_rule[20];
    
    
    
    printf("Enter %s\n",__FUNCTION__);

/*
    firewall_enable = nvram_bufget(RT2860_NVRAM, "PortTriggerEnable");
    if(!firewall_enable){
        printf("Warning: can't find \"PortTriggerEnable\" in flash\n");
        return;
    }
    if(atoi(firewall_enable)){
        rule = nvram_bufget(RT2860_NVRAM, "PortTriggerRules");
        if(!rule){
            printf("Warning: can't find \"PortTriggerRules\" in flash\n");
            return ;
        }
    }else
		return;8*/
		
	strncpy(wan_name, getWanIfNamePPP(), 16-1);

//	while( (getNthValueSafe(i++, rule, ';', rec, 256) != -1) ){

for(i=0;i<NAT_ADVANCE_NUM;i++){
    	
    	sprintf(str_rule, "%s%d", "PortTriggerRule", i+1);
		//rec = nvram_bufget(RT2860_NVRAM, str_rule);
    	
    if(strlen(rec)){ 	
		// get incoming protocol
		//if((getNthValueSafe(0, rec, ',', in_protocol, 8) == -1))
		//	continue;
		//in_proto = atoi(in_protocol);
		in_proto=PROTO_TCP_UDP;

		// get incoming port range "from"
		if((getNthValueSafe(1, rec, ',', in_prf, 8) == -1)){
			//printf("in_prf = %s\n", in_prf);
			continue;
		}
		if( (in_prf_int = atoi(in_prf)) == 0 || in_prf_int > 65535)
			continue;

		// get incoming port range "to"
		if((getNthValueSafe(2, rec, ',', in_prt, 8) == -1)){
			//printf("in_prt = %s\n", in_prt);
			continue;
		}
		if( (in_prt_int = atoi(in_prt)) > 65535)
			continue;

		// get trigger protocol
		//if((getNthValueSafe(3, rec, ',', tg_protocol, 8) == -1))
		//	continue;
		//tg_proto = atoi(tg_protocol);
		tg_proto=PROTO_TCP_UDP;

		// get trigger port range "from"
		if((getNthValueSafe(3, rec, ',', tg_prf, 8) == -1)){
			//printf("tg_prf = %s\n", tg_prf);
			continue;
		}
		if( (tg_prf_int = atoi(tg_prf)) == 0 || tg_prf_int > 65535)
			continue;

		// get trigger port range "to"
		if((getNthValueSafe(4, rec, ',', tg_prt, 8) == -1)){
			//printf("tg_prt = %s\n", tg_prt);
			continue;
		}
		if( (tg_prt_int = atoi(tg_prt)) > 65535)
			continue;

		switch(in_proto){
			case PROTO_TCP:
			case PROTO_UDP:
				switch(tg_proto){
					case PROTO_TCP:
					case PROTO_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					case PROTO_TCP_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					default:
						continue;
				}
				break;
			case PROTO_TCP_UDP:
				switch(tg_proto){
					case PROTO_TCP:
					case PROTO_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					case PROTO_TCP_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						
						break;
				}
				break;
			default:
				continue;
		}//switch
      }//if
		//makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
		//doSystem(cmd);
	}//for
}
#else
/* Steve, for Port Trigger */
static void iptablesPortTriggerRun(void)
{
	int i=0;
	char rec[256];
	char cmd[1024];
	char wan_name[16];

	//int prf_int, prt_int, proto;
	int in_prf_int, in_prt_int, in_proto;
	int tg_prf_int, tg_prt_int, tg_proto;
	//char ip_address[32], prf[8], prt[8], protocol[8];
	char in_prf[8], in_prt[8], in_protocol[8];
	char tg_prf[8], tg_prt[8], tg_protocol[8];

    char *firewall_enable, *rule;

    printf("\n\n**********************\n%s\n\n",__FUNCTION__);
    
    //firewall_enable = nvram_bufget(RT2860_NVRAM, "PortTriggerEnable");
    if(!firewall_enable){
        printf("Warning: can't find \"PortTriggerEnable\" in flash\n");
        return;
    }
    if(atoi(firewall_enable)){
        //rule = nvram_bufget(RT2860_NVRAM, "PortTriggerRules");
        if(!rule){
            printf("Warning: can't find \"PortTriggerRules\" in flash\n");
            return ;
        }
    }else
		return;

//  if ( getIfIp(getWanIfName(), wan_ip) == -1)
//      return;
//Steve modify 2009/01/19
	strncpy(wan_name, getWanIfNamePPP(), 16-1);

	while( (getNthValueSafe(i++, rule, ';', rec, 256) != -1) ){

		// get incoming protocol
		if((getNthValueSafe(0, rec, ',', in_protocol, 8) == -1))
			continue;
		in_proto = atoi(in_protocol);

		// get incoming port range "from"
		if((getNthValueSafe(1, rec, ',', in_prf, 8) == -1)){
			printf("in_prf = %s\n", in_prf);
			continue;
		}
		if( (in_prf_int = atoi(in_prf)) == 0 || in_prf_int > 65535)
			continue;

		// get incoming port range "to"
		if((getNthValueSafe(2, rec, ',', in_prt, 8) == -1)){
			printf("in_prt = %s\n", in_prt);
			continue;
		}
		if( (in_prt_int = atoi(in_prt)) > 65535)
			continue;

		// get trigger protocol
		if((getNthValueSafe(3, rec, ',', tg_protocol, 8) == -1))
			continue;
		tg_proto = atoi(tg_protocol);

		// get trigger port range "from"
		if((getNthValueSafe(4, rec, ',', tg_prf, 8) == -1)){
			printf("tg_prf = %s\n", tg_prf);
			continue;
		}
		if( (tg_prf_int = atoi(tg_prf)) == 0 || tg_prf_int > 65535)
			continue;

		// get trigger port range "to"
		if((getNthValueSafe(5, rec, ',', tg_prt, 8) == -1)){
			printf("tg_prt = %s\n", tg_prt);
			continue;
		}
		if( (tg_prt_int = atoi(tg_prt)) > 65535)
			continue;

		switch(in_proto){
			case PROTO_TCP:
			case PROTO_UDP:
				switch(tg_proto){
					case PROTO_TCP:
					case PROTO_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					case PROTO_TCP_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					default:
						continue;
				}
				break;
			case PROTO_TCP_UDP:
				switch(tg_proto){
					case PROTO_TCP:
					case PROTO_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
					case PROTO_TCP_UDP:
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_TCP, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, PROTO_TCP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						makePortTriggerRule(cmd, 1024, wan_name, PROTO_UDP, in_prf_int, in_prt_int, PROTO_UDP, tg_prf_int, tg_prt_int);
						doSystem(cmd);
						break;
				}
				break;
			default:
				continue;
		}

		//makePortTriggerRule(cmd, 1024, wan_name, in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int);
		//doSystem(cmd);
	}
}

#endif

//Steve move
//Steve add 2009/03/09
static void iptablesDMZForwardRun(void)
{
	#if 0
	char cmd[1024], *ip_address;
	char *dmz_enable = nvram_bufget(RT2860_NVRAM, "DMZEnable");
	if(!dmz_enable){
		printf("Warning: can't find \"DMZEnable\" in flash\n");
		return;
	}
	if(!atoi(dmz_enable))
		return;

	ip_address = nvram_bufget(RT2860_NVRAM, "DMZIPAddress");
	if(!ip_address){
		printf("Warning: can't find \"DMZIPAddress\" in flash\n");
		return;
	}
	doSystem("iptables -A " DMZ_FORWARD_CHAIN " -o %s -d %s -j ACCEPT", getLanIfName(), ip_address);
	return;
    #endif
}

//Steve move
/* Same as the file "linux/netfilter_ipv4/ipt_webstr.h" */
#define BLK_JAVA                0x01
#define BLK_ACTIVE              0x02
#define BLK_COOKIE              0x04
#define BLK_PROXY               0x08
void iptablesWebsFilterRun(void)
{
#if 0
	int i;
	int content_filter = 0;
	char entry[256];
	char *url_filter = nvram_bufget(RT2860_NVRAM, "websURLFilters");
	char *host_filter = nvram_bufget(RT2860_NVRAM, "websHostFilters");
	char *proxy = nvram_bufget(RT2860_NVRAM, "websFilterProxy");
	char *java = nvram_bufget(RT2860_NVRAM, "websFilterJava");
	char *activex = nvram_bufget(RT2860_NVRAM, "websFilterActivex");
	char *cookies = nvram_bufget(RT2860_NVRAM, "websFilterCookies");

	if(!url_filter || !host_filter || !proxy || !java || !activex || !cookies)
		return;

	// Content filter
	if(!strcmp(java, "1"))
		content_filter += BLK_JAVA;
	if(!strcmp(activex, "1"))
		content_filter += BLK_ACTIVE;
	if(!strcmp(cookies, "1"))
		content_filter += BLK_COOKIE;
	if(!strcmp(proxy, "1"))
		content_filter += BLK_PROXY;
	
	#if defined CONFIG_hp_NAT_APP
		char *ip_address = nvram_bufget(RT2860_NVRAM, "websTrustedIPAddress");
		if(!strlen(ip_address)){
			printf("websTrustedIPAddress is null\n");  
		}
		else{	
	     if(strcmp("0.0.0.0", ip_address))
	         doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp -s %s -j ACCEPT", ip_address);
	    }
	#endif	

	if(content_filter){
		// Why only 3 ports are inspected?(This idea is from CyberTAN source code)
		// TODO: use layer7 to inspect HTTP
		doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp --dport 80   -m webstr --content %d -j REJECT --reject-with tcp-reset", content_filter);
		doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp --dport 3128 -m webstr --content %d -j REJECT --reject-with tcp-reset", content_filter);
		doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp --dport 8080 -m webstr --content %d -j REJECT --reject-with tcp-reset", content_filter);
	}

	// URL filter
	i=0;
	while( (getNthValueSafe(i++, url_filter, ';', entry, sizeof(entry)) != -1) ){
		if(strlen(entry)){
			if(!strncasecmp(entry, "http://", strlen("http://")))
				strcpy(entry, entry + strlen("http://"));
			doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp -m webstr --url  %s -j REJECT --reject-with tcp-reset", entry);
		}
	}

	// HOST(Keyword) filter
	#if defined CONFIG_hp_NAT_APP
	    char *hostenable = nvram_bufget(RT2860_NVRAM, "websHostFiltersEnabled");
	    if(!strcmp(hostenable, "1")){
			i=0;
			while( (getNthValueSafe(i++, host_filter, ';', entry, sizeof(entry)) != -1) ){
				if(strlen(entry))
				doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp -m webstr --host \"%s\" -j REJECT --reject-with tcp-reset", entry);
			}
		}
	#else
		i=0;
		while( (getNthValueSafe(i++, host_filter, ';', entry, sizeof(entry)) != -1) ){
			if(strlen(entry))
				doSystem("iptables -A " WEB_FILTER_CHAIN " -p tcp -m tcp -m webstr --host %s -j REJECT --reject-with tcp-reset", entry);
		}
	#endif
#endif
	return;
}

static void iptablesAllFilterRun(void)
{
    printf("===================	%s  =============\n", __FUNCTION__);
	iptablesIPPortFilterRun();
	iptablesWebsFilterRun();

	/* system filter */	
	#if defined CONFIG_hp_NAT_APP	
	   // printf("	Steve test	=>	Enter iptablesWANServiceRun\n");
		//iptablesWANServiceRun();
	#else
		iptablesRemoteManagementRun();
    #endif

    #if 1 //Steve add 2009/03/09
    	iptablesDMZForwardRun();
    #endif 
}

//Steve add 2008/12/25
static void iptablesPOSTROUTINGRun(void){
    
    /*Steve add for portforward for POSTROUTING 2009/12/08*/
    doSystem("iptables -t nat -A POSTROUTING -j %s 1>/dev/null 2>&1", PORT_FORWARD_POST_CHAIN);
   
    /*Steve add for DMZ for POSTROUTING 2009/12/08*/
    #ifdef DMZ_POST_LOOPBACK
    doSystem("iptables -t nat -A POSTROUTING -j %s 1>/dev/null 2>&1", DMZ_POST_CHAIN);
    #endif
    doSystem("nat.sh");
}

//Steve move
//2009/03/18
#ifdef DMZ_ICMP
void iptablesICMPPREROUTING(void) {
	
	printf("===================	%s  =============\n", __FUNCTION__);
	
	char wan_ip[16];	
	char lan_ip[16];
	    
    if ( getIfIp(getLanIfName(), lan_ip) == -1){
    	 printf("LAN IP error\n");
    	 return;	 
    }
	
	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    	 printf("WAN IP error\n");
    	 return;	 
    }
    	doSystem("iptables -t nat -I PREROUTING 1 -j DNAT -d %s -p icmp --to %s", wan_ip, lan_ip);
}
#endif

static void iptablesAllNATRun(void)
{
#if 0
	#if 1 //Steve add 2008/12/25
		printf("===================	%s  =============\n", __FUNCTION__);
	    char *natE = nvram_bufget(RT2860_NVRAM, "natEnabled");
		if(!natE)
		return -1;	// 
		if(atoi(natE) == 1)	//NAT enable
		{
			#ifdef DMZ_ICMP
			//Steve add 2008/03/18
			iptablesICMPPREROUTING();
			#endif
			
			//printf("NAT ENABLE	===>\n");
			iptablesPortForwardRun();
            #if 1 //define FIREWALL_PORTTRIGGER
             iptablesPortTriggerRun();//Steve add for port trigger
			#endif
            iptablesDMZRun();
            //iptablesRemoteManagementRun_Clear(1);
            iptablesRemoteManagementWeb(1);
		}
	#else
		iptablesPortForwardRun();
		iptablesDMZRun();
	#endif
#endif
}

//2009/03/05
void iptablesNATBasic(void) {
#if 0	
	printf("===================	%s  =============\n", __FUNCTION__);
	
	char wan_ip[16];	
	char lan_ip[16];
	    
    if ( getIfIp(getLanIfName(), lan_ip) == -1){
    	 printf("LAN IP error\n");
    	 return;	 
    }
	
	char *natE = nvram_bufget(RT2860_NVRAM, "natEnabled");
	if(!natE)
		return ;	// 
	if(atoi(natE) == 1)	//NAT enable
		//doSystem("iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE",lan_ip, getWanIfNamePPP());
		doSystem("iptables -t nat -A POSTROUTING -o %s -j MASQUERADE", getWanIfNamePPP()); //aron modify to allow all Lan IP Subnet to be MASQUERADE 2009.05.22
	
	//if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    //	 printf("WAN IP error\n");
    //	 return;	 
    //}
	
	iptablesWANServiceRun();
	#if 1
    	doSystem("iptables -A FORWARD -p udp -i %s -o %s -j ACCEPT", getWanIfNamePPP(), getLanIfName());
    	doSystem("iptables -A FORWARD -p tcp -i %s -o %s -j ACCEPT", getWanIfNamePPP(), getLanIfName());
    	doSystem("iptables -A FORWARD -i %s -j ACCEPT", getLanIfName());
    	doSystem("iptables -A FORWARD -i %s -m state --state RELATED,ESTABLISHED -j ACCEPT ", getWanIfNamePPP());    
    #endif
#endif
}





static void iptablesDMZForwardFlush(void)
{
	doSystem("iptables -t filter -F " DMZ_FORWARD_CHAIN);
}

static void iptablesAllNATFlush(void)
{
	iptablesPortForwardFlush();
	#if 1 //define FIREWALL_PORTTRIGGER
		iptablesPortTriggerFlush();//Steve add for port trigger
		iptablesRemoteManagementWeb(0);//Steve add 2009/02/09
	#endif
	iptablesDMZFlush();
}

inline int getRuleNums(char *rules){
	return getNums(rules, ' ');
}

static int getDefaultFirewallPolicyASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int value;
	char *p = nvram_bufget(RT2860_NVRAM, "DefaultFirewallPolicy");
	int default_policy;
	if(!p)
		default_policy = 0;
	else
		default_policy = atoi(p);

	if( ejArgs(argc, argv, T("%d"), &value) != 1){
		return -1;
	}

	if(default_policy == value )
		websWrite(wp, T(" selected "));
#endif
	return 0;	
}

static int checkIfUnderBridgeModeASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	if(!mode)
		return -1;	// fatal error, make ASP engine complained.
	if(atoi(mode) == 0)	// bridge mode
		websWrite(wp, T(HTML_NO_FIREWALL_UNDER_BRIDGE_MODE));
#endif
	return 0;
}

/*
 * ASP function
 */
static int getPortForwardRuleNumsASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
    char *rules = nvram_bufget(RT2860_NVRAM, "PortForwardRules");
	if(!rules || !strlen(rules) ){
		websWrite(wp, T("0"));
		return 0;
	}
	websWrite(wp, T("%d"), getRuleNums(rules));
#endif	
	return 0;
}

/* Steve, for Port Trigger */
static int getPortTriggerRuleNumsASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
    char *rules = nvram_bufget(RT2860_NVRAM, "PortTriggerRules");
	if(!rules || !strlen(rules) ){
		websWrite(wp, T("0"));
		return 0;
	}
	websWrite(wp, T("%d"), getRuleNums(rules));
#endif
	return 0;
}

/*
 * ASP function
 */
static int getIPPortRuleNumsASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
    char *rules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
	if(!rules || !strlen(rules) ){
		websWrite(wp, T("0"));
		return 0;
	}

	websWrite(wp, T("%d"), getRuleNums(rules));
#endif
	return 0;
}

/*
 * ASP function
 */
static int showPortForwardRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int i=0;
	int prf_int, prt_int, proto;
	char ip_address[32], prf[8], prt[8], comment[16], protocol[8];
	char rec[128];
	char *rules = nvram_bufget(RT2860_NVRAM, "PortForwardRules");
	if(!rules)
		return 0;
	if(!strlen(rules))
		return 0;

	/* format is :
	 * [ip],[port_from],[port_to],[protocol],[comment],;
	 */
	while(getNthValueSafe(i++, rules, ';', rec, sizeof(rec)) != -1 ){
		// get ip address
		if((getNthValueSafe(0, rec, ',', ip_address, sizeof(ip_address)) == -1)){
			continue;
		}
		if(!isIpValid(ip_address)){
			continue;
		}

		// get port range "from"
		if((getNthValueSafe(1, rec, ',', prf, sizeof(prf)) == -1)){
			continue;
		}
		if( (prf_int = atoi(prf)) == 0 || prf_int > 65535){
			continue;
		}

		// get port range "to"
		if((getNthValueSafe(2, rec, ',', prt, sizeof(prt)) == -1)){
			continue;
		}
		if( (prt_int = atoi(prt)) > 65535){
			continue;
		}

		// get protocol
		if((getNthValueSafe(3, rec, ',', protocol, sizeof(protocol)) == -1)){
			continue;
		}
		proto = atoi(protocol);
		switch(proto){
			case PROTO_TCP:
			case PROTO_UDP:
			case PROTO_TCP_UDP:
				break;
			default:
				continue;
		}

		if((getNthValueSafe(4, rec, ',', comment, sizeof(comment)) == -1)){
			continue;
		}

		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td> %d&nbsp; <input type=\"checkbox\" name=\"delRule%d\"> </td>"), i, i-1 );

		// output IP address
		websWrite(wp, T("<td align=center> %s </td>"), ip_address);

		// output Port Range
		if(prt_int)
			websWrite(wp, T("<td align=center> %d - %d </td>"), prf_int, prt_int);
		else
			websWrite(wp, T("<td align=center> %d </td>"), prf_int);

		// output Protocol
        switch(proto){
            case PROTO_TCP:
				websWrite(wp, T("<td align=center> TCP </td>"));
				break;
            case PROTO_UDP:
				websWrite(wp, T("<td align=center> UDP </td>"));
				break;
            case PROTO_TCP_UDP:
				websWrite(wp, T("<td align=center> TCP + UDP </td>"));
				break;
		}

		// output Comment
		if(strlen(comment))
			websWrite(wp, T("<td align=center> %s</td>"), comment);
		else
			websWrite(wp, T("<td align=center> &nbsp; </td>"));
		websWrite(wp, T("</tr>\n"));
	}	
#endif  
	return 0;	
}

/* Steve, for Port Trigger --> NAT Advance hp *//*Steve add */
static int showNatAdvanceRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	int i=0;
	char str_Name[20]={0};
	char *rules;
	char  name[32], InportS[8], InportE[8], TriportS[8],TriportE[8];
	char  tmp_active[8];
	

	
	for(i=0;i<NAT_ADVANCE_NUM;i++){
		
        ezplib_get_attr_val("pt_rule", i, "enable", tmp_active, 
                8, EZPLIB_USE_CLI);
        //printf("enable=%s\t",tmp_active);
        
        ezplib_get_attr_val("pt_rule", i, "name", name, 
                32, EZPLIB_USE_CLI);
        //printf("name=%s\t",tmp_name);
        
        ezplib_get_attr_val("pt_rule", i, "trig_port_start", TriportS, 
                8, EZPLIB_USE_CLI);
        //printf("extif=%s\t",tmp);
        
        ezplib_get_attr_val("pt_rule", i, "trig_port_end", TriportE, 
                8, EZPLIB_USE_CLI);
        //printf("proto=%s\t",tmp);
        
        ezplib_get_attr_val("pt_rule", i, "port_start", InportS, 
                8, EZPLIB_USE_CLI);
        //printf("extport_start=%s\t",tmp_port);
        
        ezplib_get_attr_val("pt_rule", i, "port_end", InportE, 
                8, EZPLIB_USE_CLI);
        //printf("extport_end=%s\t",tmp);
			
			websWrite(wp, T("<tr>\n"));
			// output No.
			websWrite(wp, T("<td valign=\"top\"><center><span class=\"table_left\">%d</span></center></td>\n"), i+1 );
        	//Name  
			websWrite(wp, T("<td><center><span><input name=\"PortRule_Name%d\" size=15 maxlength=31 value=\"%s\" type=\"text\"></span></center></td>\n"),i+1,name);				
			//Inc S
			websWrite(wp, T("<td><center><span class=\"table_font\"><input name=\"PortRule_IncS%d\" size=10 maxlength=31 value=\"%s\" type=\"text\"></span></center></td>\n"),i+1,atoi(InportS)==0?"":InportS);
			//Inc E
			websWrite(wp, T("<td><center><span class=\"table_font\"><input name=\"PortRule_IncE%d\" size=10 maxlength=31 value=\"%s\" type=\"text\"></span></center></td>\n"),i+1,atoi(InportE)==0?"":InportE);
			//Tri S
			websWrite(wp, T("<td><center><span class=\"table_font\"><input name=\"PortRule_TriS%d\" size=10 maxlength=31 value=\"%s\" type=\"text\"></span></center></td>\n"),i+1,atoi(TriportS)==0?"":TriportS);
			//Tri E
			websWrite(wp, T("<td><center><span class=\"table_right\"><input name=\"PortRule_TriE%d\" size=10 maxlength=31 value=\"%s\" type=\"text\"></span></center></td>\n"),i+1,atoi(TriportE)==0?"":TriportE);						
			websWrite(wp, T("</tr>\n"));	
		
	}	
#endif	
	return 0;	
}



/* Steve, for Port Forward --> NAT Application hp *//*Steve add */
static int showNatApplicationRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[SHORT_BUF_LEN];
	char tmp_active[SHORT_BUF_LEN];
	char tmp_name[SHORT_BUF_LEN];
	char tmp_port[SHORT_BUF_LEN];
	char tmp_ip[SHORT_BUF_LEN];
	int i;
	int rule_num=0;
		
    rule_num=atoi(nvram_safe_get("fr_rule_num"));
    //printf("\n\n   rule_num=%d -> 111\n", rule_num);        
	
	for(i=0;i<rule_num;i++){
		ezplib_get_attr_val("fr_rule", i, "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("name=%s\t",tmp_name);
        
        ezplib_get_attr_val("fr_rule", i, "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("enable=%s\t",tmp_active);
        
        ezplib_get_attr_val("fr_rule", i, "extif", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extif=%s\t",tmp);
        
        ezplib_get_attr_val("fr_rule", i, "proto", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("proto=%s\t",tmp);
        
        ezplib_get_attr_val("fr_rule", i, "extport_start", tmp_port, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_start=%s\t",tmp_port);
        
        ezplib_get_attr_val("fr_rule", i, "extport_end", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_end=%s\t",tmp);
        
        ezplib_get_attr_val("fr_rule", i, "ipaddr", tmp_ip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("ipaddr=%s\t",tmp_ip);
        
        ezplib_get_attr_val("fr_rule", i, "inport_start", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_start=%s\t",tmp);
        
        ezplib_get_attr_val("fr_rule", i, "inport_end", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_end=%s\n",tmp);
        
			
		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td valign=\"top\"><center><span class=\"table_left\">%d</span></center></td>"), i+1);

        //Active     
        if( !strcmp(tmp_active, "1"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));
		
		// Name
		if(strlen(tmp_name))
			websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center></td>"), tmp_name);
		else
			websWrite(wp, T("<td><center><span class=\"table_font\">-                                </span></center></td>"));
		
		// Port
		websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center>"), tmp_port);
			
		// IP
		websWrite(wp, T("<td><center><span class=\"table_font\">%s </span></center></td>"), tmp_ip);
		
		//Modify
		websWrite(wp, T("<td><center><span class=\"table_right\">"));
		websWrite(wp, T("<a href=\"javascript: doFindCurrentRule(%d);doEditRule();document.NAT_APP.submit();\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatEImage%d','','images/i_edit_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_edit.gif\" name=\"fnatEImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatEImage%d\"  title=\"Edit\"/></a>"),i+1);
		websWrite(wp, T("&nbsp;&nbsp;<a href=\"javascript: doFindCurrentRule(%d);doDeleteRule();document.NAT_APP.submit();\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatDImage%d','','images/i_delete_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_delete.gif\" name=\"fnatDImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatDImage%d\"  title=\"Delete\"/>"),i+1);
		websWrite(wp, T("</a></span></center></td>"));
		websWrite(wp, T("</tr>\n"));  
    }

	return 0;	
}

/*for one to one NAT rules list display,add by wind 2013-12-09*/
static int showOneToOneRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[SHORT_BUF_LEN] = {0};
	char tmp_active[SHORT_BUF_LEN] = {0};
	char tmp_name[SHORT_BUF_LEN] = {0};
	char tmp_intip[SHORT_BUF_LEN] = {0};
	char tmp_extip[SHORT_BUF_LEN] = {0};
	char tmp_port[SHORT_BUF_LEN] = {0};
	char tmp_forward[SHORT_BUF_LEN] = {0};
	int i;
	int rule_num=0;
	rule_num=atoi(nvram_safe_get("one_to_one_num"));
	for(i=0;i<rule_num;i++){
		ezplib_get_attr_val("one_to_one_rule", i, "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "extif", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "proto", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
       
       	ezplib_get_attr_val("one_to_one_rule", i, "int_addr", tmp_intip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
	  
	  	ezplib_get_attr_val("one_to_one_rule", i, "ext_addr", tmp_extip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
      	ezplib_get_attr_val("one_to_one_rule", i, "extport_start", tmp_port, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "extport_end", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "forward_check", tmp_forward, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);

        
        ezplib_get_attr_val("one_to_one_rule", i, "intport_start", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
        ezplib_get_attr_val("one_to_one_rule", i, "intport_end", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td valign=\"top\"><center><span class=\"table_left\">%d</span></center></td>"), i+1);
        //Active     
        if( !strcmp(tmp_active, "1"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));
		
		// Name
		if(strlen(tmp_name))
			websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center></td>"), tmp_name);
		else
			websWrite(wp, T("<td><center><span class=\"table_font\">-                                </span></center></td>"));
		// internal IP
		websWrite(wp, T("<td><center><span class=\"table_font\">%s </span></center></td>"), tmp_intip);
		// external IP
		websWrite(wp, T("<td><center><span class=\"table_font\">%s </span></center></td>"), tmp_extip);
		// Port
		websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center>"), tmp_port);
			
		// server forward
	        if( !strcmp(tmp_forward, "1"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));	
		
		//Modify
		websWrite(wp, T("<td><center><span class=\"table_right\">"));
		websWrite(wp, T("<a href=\"javascript: doFindCurrentRule(%d);doEditRule();document.NAT_APP.submit();\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatEImage%d','','images/i_edit_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_edit.gif\" name=\"fnatEImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatEImage%d\"  title=\"Edit\"/></a>"),i+1);
		websWrite(wp, T("&nbsp;&nbsp;<a href=\"javascript: doFindCurrentRule(%d);doDeleteRule();document.NAT_APP.submit();\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatDImage%d','','images/i_delete_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_delete.gif\" name=\"fnatDImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatDImage%d\"  title=\"Delete\"/>"),i+1);
		websWrite(wp, T("</a></span></center></td>"));
		websWrite(wp, T("</tr>\n"));  
    }

	return 0;	
}
/* Steve, for Port Trigger */
static int showPortTriggerRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int i=0;
	//int prf_int, prt_int, proto;
	//char ip_address[32], prf[8], prt[8], comment[16], protocol[8];
	int in_prf_int, in_prt_int, in_proto;
	int tg_prf_int, tg_prt_int, tg_proto;
	char in_prf[8], in_prt[8], comment[16], in_protocol[8];
	char tg_prf[8], tg_prt[8], tg_protocol[8];
	char rec[128];
	char *rules = nvram_bufget(RT2860_NVRAM, "PortTriggerRules");
	if(!rules)
		return 0;
	if(!strlen(rules))
		return 0;

	/* format is :
	 * [in_protocol],[in_port_from],[in_port_to],[tg_protocol],[in_port_from],[in_port_to],[comment],;
	 */
	while(getNthValueSafe(i++, rules, ';', rec, 256) != -1 ){
		// get in_protocol
		if((getNthValueSafe(0, rec, ',', in_protocol, 8) == -1)){
			continue;
		}
		in_proto = atoi(in_protocol);
		switch(in_proto){
			case PROTO_TCP:
			case PROTO_UDP:
			case PROTO_TCP_UDP:
				break;
			default:
				continue;
		}
		// get incoming port range "from"
		if((getNthValueSafe(1, rec, ',', in_prf, 8) == -1)){
			continue;
		}
		if( (in_prf_int = atoi(in_prf)) == 0 || in_prf_int > 65535){
			continue;
		}

		// get incoming port range "to"
		if((getNthValueSafe(2, rec, ',', in_prt, 8) == -1)){
			continue;
		}
		if( (in_prt_int = atoi(in_prt)) > 65535){
			continue;
		}

		// get tg_protocol
		if((getNthValueSafe(3, rec, ',', tg_protocol, 8) == -1)){
			continue;
		}
		tg_proto = atoi(tg_protocol);
		switch(tg_proto){
			case PROTO_TCP:
			case PROTO_UDP:
			case PROTO_TCP_UDP:
				break;
			default:
				continue;
		}

		// get trigger port range "from"
		if((getNthValueSafe(4, rec, ',', tg_prf, 8) == -1)){
			continue;
		}
		if( (tg_prf_int = atoi(tg_prf)) == 0 || tg_prf_int > 65535){
			continue;
		}

		// get trigger port range "to"
		if((getNthValueSafe(5, rec, ',', tg_prt, 8) == -1)){
			continue;
		}
		if( (tg_prt_int = atoi(tg_prt)) > 65535){
			continue;
		}

		if((getNthValueSafe(6, rec, ',', comment, 16) == -1)){
			continue;
		}

		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td> %d&nbsp; <input type=\"checkbox\" name=\"delRule%d\"> </td>"), i, i-1 );

		// incoming Protocol
		 switch(in_proto){
			case PROTO_TCP:
				websWrite(wp, T("<td align=center> TCP </td>"));
				break;
			case PROTO_UDP:
				websWrite(wp, T("<td align=center> UDP </td>"));
				break;
			case PROTO_TCP_UDP:
				websWrite(wp, T("<td align=center> TCP + UDP </td>"));
				break;
		}

		// incoming Port Range
		if(in_prt_int)
			websWrite(wp, T("<td align=center> %d - %d </td>"), in_prf_int, in_prt_int);
		else
			websWrite(wp, T("<td align=center> %d </td>"), in_prf_int);

		// trigger Protocol
		 switch(tg_proto){
			case PROTO_TCP:
				websWrite(wp, T("<td align=center> TCP </td>"));
				break;
			case PROTO_UDP:
				websWrite(wp, T("<td align=center> UDP </td>"));
				break;
			case PROTO_TCP_UDP:
				websWrite(wp, T("<td align=center> TCP + UDP </td>"));
				break;
		}

		 // trigger Port Range
		if(tg_prt_int)
			websWrite(wp, T("<td align=center> %d - %d </td>"), tg_prf_int, tg_prt_int);
		else
			websWrite(wp, T("<td align=center> %d </td>"), tg_prf_int);

		// output Comment
		if(strlen(comment))
			websWrite(wp, T("<td align=center> %s</td>"), comment);
		else
			websWrite(wp, T("<td align=center> &nbsp; </td>"));
		websWrite(wp, T("</tr>\n"));
	}
#endif
	return 0;
}

static void getRulesPacketCount(webs_t wp, char_t *path, char_t *query)
{
#if 0
	FILE *fp;
	int i, step_in_chains=0;
	char buf[1024], *default_policy;
	int default_drop_flag;
	int index=0, pkt_count;
	int *result;

	// check if the default policy is "drop" 
	default_policy = nvram_bufget(RT2860_NVRAM, "DefaultFirewallPolicy");
	if(!default_policy)
		default_policy = "0";
	default_drop_flag = atoi(default_policy);

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));

	result = (int *)malloc(sizeof(int) * 128);
	if(!result)
		goto error;

	fp = popen("iptables -t filter -L -v", "r");
	if(!fp){
		free(result);
		goto error;
	}

	while(fgets(buf, 1024, fp) && index < 128){
		if(step_in_chains){
			if(buf[0] == '\n')
				break;
			if(buf[0] == ' ' && buf[1] == 'p' && buf[2] == 'k' && buf[3] == 't' )
				continue;
			// Skip the first one rule if default policy is drop.
			if(default_drop_flag){
				default_drop_flag = 0;
				continue;
			}
			sscanf(buf, "%d ", &pkt_count);
			result[index++] = pkt_count;
		}

		if(strstr(buf, "Chain " IPPORT_FILTER_CHAIN))
			step_in_chains = 1;
	}
	pclose(fp);

	if(index > 0)
		websWrite(wp, "%d", result[0]);
	for(i=1; i<index; i++)
		websWrite(wp, " %d", result[i]);

	free(result);
error:
	websDone(wp, 200);
#endif
	return;
}


/*
 * ASP function
 */
//Add for hp 
static int showIPPortFilterServiceRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int rule_num;
	char temp_name[32],temp_src_sip[32],temp_dst_sip[32],temp_proto[32],temp_src_eip[32],temp_dst_eip[32];
	char temp_dst_sport[32],temp_dst_eport[32],temp_src_sport[32],temp_src_eport[32];
	int i;
	
	rule_num=atoi(nvram_safe_get("fl_rule_num"));
    //printf("\n   rule_num=%d -> 111\n", rule_num);        
	
	for(i=0;i<rule_num;i++){
		
		//name
		ezplib_get_attr_val("fl_rule", i, "name", temp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("name=%s\t",temp_name);
        //src ip start
		ezplib_get_attr_val("fl_rule", i, "srcipaddr_start", temp_src_sip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_src_sip=%s\t",temp_src_sip);
        //src ip end
		ezplib_get_attr_val("fl_rule", i, "srcipaddr_end", temp_src_eip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_src_eip=%s\t",temp_src_eip);
        //dst ip start
		ezplib_get_attr_val("fl_rule", i, "dstipaddr_start", temp_dst_sip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_dst_sip=%s\t",temp_dst_sip);
        //dst ip end
		ezplib_get_attr_val("fl_rule", i, "dstipaddr_end", temp_dst_eip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_dst_eip=%s\t",temp_dst_eip);
        //proto
        ezplib_get_attr_val("fl_rule", i, "proto", temp_proto, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_proto=%s\t",temp_proto);
        
        //dstport_start
        ezplib_get_attr_val("fl_rule", i, "dstport_start", temp_dst_sport, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_dst_sport=%s\t",temp_dst_sport);
        
        //dstport_end
        ezplib_get_attr_val("fl_rule", i, "dstport_end", temp_dst_eport, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_dst_eport=%s\t",temp_dst_eport);
        
        //srcport_start
        ezplib_get_attr_val("fl_rule", i, "srcport_start", temp_src_sport, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_src_sport=%s\t",temp_src_sport);
        
        //srcport_end
        ezplib_get_attr_val("fl_rule", i, "srcport_end", temp_src_eport, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("temp_src_eport=%s\t",temp_src_eport);
        
        websWrite(wp, T("<tr>\n"));
		// output No.
		//websWrite(wp, T("<td> %d&nbsp; <input type=\"checkbox\" name=\"delRule%d\"> </td>"), i+1, i );
        websWrite(wp, T("<td><center><span class=\"table_left\"> %d </span></center></td>"), i+1);

        // output temp_name
		if(strlen(temp_name))
			websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), temp_name);
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"> &nbsp; </span></center></td>"));
			
		// output Mac address
		//websWrite(wp, T("<td><center><span class=\"table_left\"> %s </span></center></td>"), "-");

		// output DIP
		if(strlen(temp_dst_sip)){
			if( !strcmp(temp_dst_sip, temp_dst_eip))
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), temp_dst_sip);
		    else
		    	websWrite(wp, T("<td><center><span class=\"table_font\"> %s - %s </span></center></td>"), temp_dst_sip, temp_dst_eip);
		}else
			websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), "-");

		// output SIP
		if(strlen(temp_src_sip)){
			if( !strcmp(temp_src_sip, temp_src_eip))
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), temp_src_sip);
			else
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s - %s </span></center></td>"), temp_src_sip, temp_src_eip);
		
		}else
			websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), "-");
		

		// output Protocol
		if(strlen(temp_proto))
			websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"),temp_proto);
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"),"-");
				
		// output dest Port Range
		if(strlen(temp_dst_sport)){
			if(strcmp(temp_dst_sport, temp_dst_eport))
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s - %s </span></center></td>"), temp_dst_sport, temp_dst_eport);
			else
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), temp_dst_sport);
		}else
			websWrite(wp, T("<td><center><span class=\"table_font\"> - </span></center></td>"));

		// output Source Port Range
		if(strlen(temp_src_sport)){
			if(strcmp(temp_src_sport, temp_src_eport))
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s - %s </span></center></td>"), temp_src_sport, temp_src_eport);
			else
				websWrite(wp, T("<td><center><span class=\"table_font\"> %s </span></center></td>"), temp_src_sport);
		}else
			websWrite(wp, T("<td><center><span class=\"table_font\"> - </span></center></td>"));

		// output action
		websWrite(wp, T("<td><center><span class=\"table_font\"> Drop </span></center></td>"));

		// output the id of "packet count"
		websWrite(wp, T("<td><center><span class=\"table_right\"><a href=\"javascript: doFindCurrentRule(%d);doDeleteRule();document.FiewallService.submit();\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fruleImage%d','','images/i_delete_on.gif',1)\">"),i,i);       
        websWrite(wp, T("<img src=\"images/i_delete.gif\"  name=\"fruleImage\"  id=\"fruleImage%d\" title=\"Delete\"></span></center></td>"),i);
		websWrite(wp, T("</tr>\n"));
        
    }
#if 0
	int i;
	int sprf_int, sprt_int, proto;
	char mac_address[32];
	char sip_1[32], sip_2[32], sprf[8], sprt[8], protocol[8], action[4];
	char dip_1[32], dip_2[32], dprf[8], dprt[8];
	int dprf_int, dprt_int;
	char rec[256];
	char *default_policy;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
	if(!rules)
		return 0;

    default_policy = nvram_bufget(RT2860_NVRAM, "DefaultFirewallPolicy");
    // add the default policy to the end of FORWARD chain
    if(!default_policy)
		return 0;
	if(!strlen(default_policy))
		return 0;

	i=0;
	while(getNthValueSafe(i, rules, ';', rec, sizeof(rec)) != -1 && strlen(rec)){
		printf("i=%d, rec=%s, strlen(rec)=%d\n", i, rec, strlen(rec));
		// get ip 1
        if((getNthValueSafe(0, rec, ',', sip_1, sizeof(sip_1)) == -1)){
			i++;
			continue;
		}
        if(!isIpNetmaskValid(sip_1)){
			i++;
			continue;
		}
		// translate "any/0" to "any" for readable reason
		if( !strcmp(sip_1, "any/0"))
			strcpy(sip_1, "-");

		// get ip 2
        // get ip address
        if((getNthValueSafe(1, rec, ',', sip_2, sizeof(sip_2)) == -1)){
			i++;
			continue;
		}
		// dont verify cause we dont have ip range support
		//if(!isIpValid(sip_2))
        //    continue;

		// get port range "from"
		if((getNthValueSafe(2, rec, ',', sprf, sizeof(sprf)) == -1)){
			i++;
			continue;
		}
		if( (sprf_int = atoi(sprf)) > 65535){
			i++;
			continue;
		}

		// get port range "to"
		if((getNthValueSafe(3, rec, ',', sprt, sizeof(sprt)) == -1)){
			i++;
			continue;
		}
		if( (sprt_int = atoi(sprt)) > 65535){
			i++;
			continue;
		}

		// get ip 1
        if((getNthValueSafe(4, rec, ',', dip_1, sizeof(dip_1)) == -1)){
			i++;
            continue;
		}
        if(!isIpNetmaskValid(dip_1)){
			i++;
            continue;
		}
		// translate "any/0" to "any" for readable reason
		if( !strcmp(dip_1, "any/0"))
			strcpy(dip_1, "-");
		
		// get ip 2
        if((getNthValueSafe(5, rec, ',', dip_2, sizeof(dip_2)) == -1)){
			i++;
            continue;
		}
		// dont verify cause we dont have ip range support
		//if(!isIpValid(dip_2))
        //    continue;

		// get protocol
		if((getNthValueSafe(8, rec, ',', protocol, sizeof(protocol)) == -1)){
			i++;
			continue;
		}
		proto = atoi(protocol);
		switch(proto){
			case PROTO_TCP:
			case PROTO_UDP:
			case PROTO_NONE:
			case PROTO_ICMP:
				break;
			default:
				continue;
		}

		// get port range "from"
		if((getNthValueSafe(6, rec, ',', dprf, sizeof(dprf)) == -1)){
			i++;
			continue;
		}
		if( (dprf_int = atoi(dprf)) > 65535){
			i++;
			continue;
		}

		// get port range "to"
		if((getNthValueSafe(7, rec, ',', dprt, sizeof(dprt)) == -1)){
			i++;
			continue;
		}
		if( (dprt_int = atoi(dprt)) > 65535){
			i++;
			continue;
		}

		// get action
		if((getNthValueSafe(9, rec, ',', action, sizeof(action)) == -1)){
			i++;
			continue;
		}

		// get comment
		if((getNthValueSafe(10, rec, ',', comment, sizeof(comment)) == -1)){
			i++;
			continue;
		}

		// get mac address
		if((getNthValueSafe(11, rec, ',', mac_address, sizeof(mac_address)) == -1)){
			i++;
			continue;
		}
		if(!strlen(mac_address))
			gstrcpy(mac_address, T("-"));

		websWrite(wp, T("<tr>\n"));
		// output No.
		//websWrite(wp, T("<td> %d&nbsp; <input type=\"checkbox\" name=\"delRule%d\"> </td>"), i+1, i );
        websWrite(wp, T("<td><center><span class=\"table_left\"> %d </span></center></td>"), i+1);

        // output Comment
		if(strlen(comment))
			websWrite(wp, T("<td><center><span class=\"table_left\"> %s </span></center></td>"), comment);
		else
			websWrite(wp, T("<td><center><span class=\"table_left\"> &nbsp; </span></center></td>"));
			
		// output Mac address
		websWrite(wp, T("<td><center><span class=\"table_left\"> %s </span></center></td>"), mac_address);

		// output DIP
		websWrite(wp, T("<td><center><span class=\"table_left\"> %s </span></center></td>"), dip_1);
		// we dont support ip range 
		// websWrite(wp, T("<td align=center> %s-%s </td>"), ip_1, ip_2);

		// output SIP
		websWrite(wp, T("<td><center><span class=\"table_left\"> %s </span></center></td>"), sip_1);
		// we dont support ip range 
		// websWrite(wp, T("<td align=center> %s-%s </td>"), ip_1, ip_2);

		// output Protocol
        switch(proto){
            case PROTO_TCP:
				websWrite(wp, T("<td><center><span class=\"table_left\"> TCP </span></center></td>"));
				break;
            case PROTO_UDP:
				websWrite(wp, T("<td><center><span class=\"table_left\"> UDP </span></center></td>"));
				break;
            case PROTO_ICMP:
				websWrite(wp, T("<td><center><span class=\"table_left\"> ICMP </span></center></td>"));
				break;
            case PROTO_NONE:
				websWrite(wp, T("<td><center><span class=\"table_left\"> - </span></center></td>"));
				break;
		}

		// output dest Port Range
		if(dprt_int)
			websWrite(wp, T("<td><center><span class=\"table_left\"> %d - %d </span></center></td>"), dprf_int, dprt_int);
		else{
			// we re-descript the port number here because 
			// "any" word is more meanful than "0"
			if(!dprf_int){
				websWrite(wp, T("<td><center><span class=\"table_left\"> - </span></center></td>"), dprf_int);
			}else{
				websWrite(wp, T("<td><center><span class=\"table_left\"> %d </span></center></td>"), dprf_int);
			}
		}

		// output Source Port Range
		if(sprt_int)
			websWrite(wp, T("<td><center><span class=\"table_left\"> %d - %d </span></center></td>"), sprf_int, sprt_int);
		else{
			// we re-descript the port number here because 
			// "any" word is more meanful than "0"
			if(!sprf_int){
				websWrite(wp, T("<td><center><span class=\"table_left\"> - </span></center></td>"), sprf_int);
			}else{
				websWrite(wp, T("<td><center><span class=\"table_left\"> %d </span></center></td>"), sprf_int);
			}
		}


		// output action
        switch(atoi(action)){
            case ACTION_DROP:
				websWrite(wp, T("<td><center><span class=\"table_left\"> Drop </span></center></td>"), i);
				break;
            case ACTION_ACCEPT:
				websWrite(wp, T("<td><center><span class=\"table_left\"> Accept </span></center></td>"), i);
				break;
		}

		

		// output the id of "packet count"
		//websWrite(wp, T("<td><center><span class=\"top_right\"><img src=\"images/i_delete.gif\" title=\"Delete\"></span></center></td>"));
		websWrite(wp, T("<td><center><span class=\"top_right\"><a href=\"javascript: doFindCurrentRule(%d);doDeleteRule();document.FiewallService.submit();\">"),i);       
        websWrite(wp, T("<img src=\"images/i_delete.gif\" title=\"Delete\"></span></center></td>"));
        
        
		websWrite(wp, T("</tr>\n"));

		i++;
	}	  
/*
	switch(atoi(default_policy)){
		case 0:
			websWrite(wp, T("<tr><td colspan=10 id=portCurrentFilterDefaultAccept><center><span class=\"top_right\"> Others would be accepted.</span></center></td></tr>"));
			break;
		case 1:
			websWrite(wp, T("<tr><td colspan=10 id=portCurrentFilterDefaultDrop><center><span class=\"top_right\"> Others would be dropped.</span></center></td></tr>"));
			break;
	}
*/
#endif
	return 0;	
}
 
static int showIPPortFilterRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int i;
	int sprf_int, sprt_int, proto;
	char mac_address[32];
	char sip_1[32], sip_2[32], sprf[8], sprt[8], comment[16], protocol[8], action[4];
	char dip_1[32], dip_2[32], dprf[8], dprt[8];
	int dprf_int, dprt_int;
	char rec[256];
	char *default_policy;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
	if(!rules)
		return 0;

    default_policy = nvram_bufget(RT2860_NVRAM, "DefaultFirewallPolicy");
    // add the default policy to the end of FORWARD chain
    if(!default_policy)
		return 0;
	if(!strlen(default_policy))
		return 0;

	i=0;
	while(getNthValueSafe(i, rules, ';', rec, sizeof(rec)) != -1 && strlen(rec)){
		printf("i=%d, rec=%s, strlen(rec)=%d\n", i, rec, strlen(rec));
		// get ip 1
        if((getNthValueSafe(0, rec, ',', sip_1, sizeof(sip_1)) == -1)){
			i++;
			continue;
		}
        if(!isIpNetmaskValid(sip_1)){
			i++;
			continue;
		}
		// translate "any/0" to "any" for readable reason
		if( !strcmp(sip_1, "any/0"))
			strcpy(sip_1, "-");

		// get ip 2
        // get ip address
        if((getNthValueSafe(1, rec, ',', sip_2, sizeof(sip_2)) == -1)){
			i++;
			continue;
		}
		// dont verify cause we dont have ip range support
		//if(!isIpValid(sip_2))
        //    continue;

		// get port range "from"
		if((getNthValueSafe(2, rec, ',', sprf, sizeof(sprf)) == -1)){
			i++;
			continue;
		}
		if( (sprf_int = atoi(sprf)) > 65535){
			i++;
			continue;
		}

		// get port range "to"
		if((getNthValueSafe(3, rec, ',', sprt, sizeof(sprt)) == -1)){
			i++;
			continue;
		}
		if( (sprt_int = atoi(sprt)) > 65535){
			i++;
			continue;
		}

		// get ip 1
        if((getNthValueSafe(4, rec, ',', dip_1, sizeof(dip_1)) == -1)){
			i++;
            continue;
		}
        if(!isIpNetmaskValid(dip_1)){
			i++;
            continue;
		}
		// translate "any/0" to "any" for readable reason
		if( !strcmp(dip_1, "any/0"))
			strcpy(dip_1, "-");
		
		// get ip 2
        if((getNthValueSafe(5, rec, ',', dip_2, sizeof(dip_2)) == -1)){
			i++;
            continue;
		}
		// dont verify cause we dont have ip range support
		//if(!isIpValid(dip_2))
        //    continue;

		// get protocol
		if((getNthValueSafe(8, rec, ',', protocol, sizeof(protocol)) == -1)){
			i++;
			continue;
		}
		proto = atoi(protocol);
		switch(proto){
			case PROTO_TCP:
			case PROTO_UDP:
			case PROTO_NONE:
			case PROTO_ICMP:
				break;
			default:
				continue;
		}

		// get port range "from"
		if((getNthValueSafe(6, rec, ',', dprf, sizeof(dprf)) == -1)){
			i++;
			continue;
		}
		if( (dprf_int = atoi(dprf)) > 65535){
			i++;
			continue;
		}

		// get port range "to"
		if((getNthValueSafe(7, rec, ',', dprt, sizeof(dprt)) == -1)){
			i++;
			continue;
		}
		if( (dprt_int = atoi(dprt)) > 65535){
			i++;
			continue;
		}

		// get action
		if((getNthValueSafe(9, rec, ',', action, sizeof(action)) == -1)){
			i++;
			continue;
		}

		// get comment
		if((getNthValueSafe(10, rec, ',', comment, sizeof(comment)) == -1)){
			i++;
			continue;
		}

		// get mac address
		if((getNthValueSafe(11, rec, ',', mac_address, sizeof(mac_address)) == -1)){
			i++;
			continue;
		}
		if(!strlen(mac_address))
			gstrcpy(mac_address, T("-"));

		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td> %d&nbsp; <input type=\"checkbox\" name=\"delRule%d\"> </td>"), i+1, i );

		// output Mac address
		websWrite(wp, T("<td align=center> %s </td>"), mac_address);

		// output DIP
		websWrite(wp, T("<td align=center> %s </td>"), dip_1);
		// we dont support ip range 
		// websWrite(wp, T("<td align=center> %s-%s </td>"), ip_1, ip_2);

		// output SIP
		websWrite(wp, T("<td align=center> %s </td>"), sip_1);
		// we dont support ip range 
		// websWrite(wp, T("<td align=center> %s-%s </td>"), ip_1, ip_2);

		// output Protocol
        switch(proto){
            case PROTO_TCP:
				websWrite(wp, T("<td align=center> TCP </td>"));
				break;
            case PROTO_UDP:
				websWrite(wp, T("<td align=center> UDP </td>"));
				break;
            case PROTO_ICMP:
				websWrite(wp, T("<td align=center> ICMP </td>"));
				break;
            case PROTO_NONE:
				websWrite(wp, T("<td align=center> - </td>"));
				break;
		}

		// output dest Port Range
		if(dprt_int)
			websWrite(wp, T("<td align=center> %d - %d </td>"), dprf_int, dprt_int);
		else{
			// we re-descript the port number here because 
			// "any" word is more meanful than "0"
			if(!dprf_int){
				websWrite(wp, T("<td align=center> - </td>"), dprf_int);
			}else{
				websWrite(wp, T("<td align=center> %d </td>"), dprf_int);
			}
		}

		// output Source Port Range
		if(sprt_int)
			websWrite(wp, T("<td align=center> %d - %d </td>"), sprf_int, sprt_int);
		else{
			// we re-descript the port number here because 
			// "any" word is more meanful than "0"
			if(!sprf_int){
				websWrite(wp, T("<td align=center> - </td>"), sprf_int);
			}else{
				websWrite(wp, T("<td align=center> %d </td>"), sprf_int);
			}
		}


		// output action
        switch(atoi(action)){
            case ACTION_DROP:
				websWrite(wp, T("<td align=center id=portFilterActionDrop%d> Drop </td>"), i);
				break;
            case ACTION_ACCEPT:
				websWrite(wp, T("<td align=center id=portFilterActionAccept%d> Accept </td>"), i);
				break;
		}

		// output Comment
		if(strlen(comment))
			websWrite(wp, T("<td align=center> %s</td>"), comment);
		else
			websWrite(wp, T("<td align=center> &nbsp; </td>"));

		// output the id of "packet count"
		websWrite(wp, T("<td align=center id=pktCnt%d>-</td>"), i);

		websWrite(wp, T("</tr>\n"));

		i++;
	}	  

	switch(atoi(default_policy)){
		case 0:
			websWrite(wp, T("<tr><td align=center colspan=9 id=portCurrentFilterDefaultAccept> Others would be accepted.</td><td align=center id=pktCnt%d>-</td></tr>"), i);
			break;
		case 1:
			websWrite(wp, T("<tr><td align=center colspan=9 id=portCurrentFilterDefaultDrop> Others would be dropped.</td><td align=center id=pktCnt%d>-</td></tr>"), i);
			break;
	}
#endif
	return 0;	
}

static int showDMZIPAddressASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char dmz_ip[256]={0};
#if 1	
	ezplib_get_attr_val("fr_dmz_rule", 0, "ipaddr", dmz_ip, 
                256, EZPLIB_USE_CLI);

	if(!strlen(dmz_ip)){
		websWrite(wp, T("%s"), "0.0.0.0");
		return 0;
	}
	
	websWrite(wp, T("%s"), dmz_ip);
#endif
	return 0;	
}
static void ipportFilterDelete(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;
//	char *firewall_enable;

    char *rules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
    if(!rules || !strlen(rules) )
        return;

	rule_count = getRuleNums(rules);
	if(!rule_count)
		return;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	for(i=0, j=0; i< rule_count; i++){
		snprintf(name_buf, 16, "delRule%d", i);
		value = websGetVar(wp, name_buf, NULL);
		if(value){
			deleArray[j++] = i;
		}
	}
	if(!j){
	    websHeader(wp);
	    websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
	    websFooter(wp);
	    websDone(wp, 200);		
		return;
	}

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "IPPortFilterRules", rules);
	nvram_commit(RT2860_NVRAM);

	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();

    websHeader(wp);
    websWrite(wp, T("s<br>\n") );
    websWrite(wp, T("fromPort: <br>\n"));
    websWrite(wp, T("toPort: <br>\n"));
    websWrite(wp, T("protocol: <br>\n"));
    websWrite(wp, T("comment: <br>\n"));
    websFooter(wp);
    websDone(wp, 200);
#endif
	return;
}

static void portForwardDelete(webs_t wp, char_t *path, char_t *query)
{
#if 0
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;
	char *firewall_enable;

    char *rules = nvram_bufget(RT2860_NVRAM, "PortForwardRules");
    if(!rules || !strlen(rules) )
        return;

	rule_count = getRuleNums(rules);
	if(!rule_count)
		return;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	for(i=0, j=0; i< rule_count; i++){
		snprintf(name_buf, 16, "delRule%d", i);
		value = websGetVar(wp, T(name_buf), NULL);
		if(value){
			deleArray[j++] = i;
		}
	}

    if(!j){
        websHeader(wp);
        websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
        websFooter(wp);
        websDone(wp, 200);
        return;
    }

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "PortForwardRules", rules);
	nvram_commit(RT2860_NVRAM);

	// restart iptables if it is running
	firewall_enable = nvram_bufget(RT2860_NVRAM, "PortForwardEnable");
	if(firewall_enable){
		if(atoi(firewall_enable)){
			// call iptables
			iptablesPortForwardFlush();
			iptablesPortForwardRun();
		}
	}

    websHeader(wp);
    websWrite(wp, T("s<br>\n") );
    websWrite(wp, T("fromPort: <br>\n"));
    websWrite(wp, T("toPort: <br>\n"));
    websWrite(wp, T("protocol: <br>\n"));
    websWrite(wp, T("comment: <br>\n"));
    websFooter(wp);
    websDone(wp, 200);
#endif
	return;
}

/* Steve, for Port Trigger */
static void portTriggerDelete(webs_t wp, char_t *path, char_t *query)
{
#if 0
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;
	char *firewall_enable;

	char *strDelPort;
	char *strDelAllPort;
	//2007/11/16 RL add delete all
	strDelPort    = websGetVar(wp, T("deleteSelPortTrigger"), T(""));
	strDelAllPort = websGetVar(wp, T("deleteAllPortTrigger"), T(""));

    char *rules = nvram_bufget(RT2860_NVRAM, "PortTriggerRules");
    if(!rules || !strlen(rules) )
        return;

	rule_count = getRuleNums(rules);
	if(!rule_count)
		return;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	/* Delete entry */
	if (strDelPort[0])
	{
		for(i=0, j=0; i< rule_count; i++){
			snprintf(name_buf, 16, "delRule%d", i);
			value = websGetVar(wp, T(name_buf), NULL);
			if(value){
				deleArray[j++] = i;
			}
		}

    if(!j){
        websHeader(wp);
        websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
        websFooter(wp);
        websDone(wp, 200);
        return;
    }
	}

	/* Delete all entry */
	if ( strDelAllPort[0])
		for(i=0, j=0; i< rule_count; i++)
			deleArray[j++] = i;

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "PortTriggerRules", rules);

	// restart iptables if it is running
	firewall_enable = nvram_bufget(RT2860_NVRAM, "PortTriggerEnable");
	if(firewall_enable){
		if(atoi(firewall_enable)){
			// call iptables
			iptablesPortTriggerFlush();
			iptablesPortTriggerRun();
		}
	}

	/* Grace mark.
    websHeader(wp);
    websWrite(wp, T("s<br>\n") );
    websWrite(wp, T("fromPort: <br>\n"));
    websWrite(wp, T("toPort: <br>\n"));
    websWrite(wp, T("protocol: <br>\n"));
    websWrite(wp, T("comment: <br>\n"));
    websFooter(wp);
    websDone(wp, 200);*/

	char_t *submitUrl;
	submitUrl = websGetVar(wp, T("deptfil_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
#endif
	return;
}

static void ipportFilter_icmp(webs_t wp, char_t *path, char_t *query)
{
	char *submitUrl;
	char *wpfE = websGetVar(wp, T("pingFrmWANFilterEnabled"), T(""));

	if(!wpfE || !strlen(wpfE))
		return;
      
    //system("/etc/rc.common /etc/rc.d/S60network-acl stop fw"); 
    system("/sbin/ezp-iptables fw stop");
    ezplib_replace_attr("fw_rule", 0, "icmp_ping_enable", atoi(wpfE)   ? "1" : "0");    
    //system("/etc/rc.common /etc/rc.d/S60network-acl start fw");  
	system("/sbin/ezp-iptables fw start");
	printf("nvram_commit\n");
	nvram_commit();
	
	setWebMessage(0, NULL);
	
	submitUrl = websGetVar(wp, T("service_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);     
}

//Steve move
static void iptablesBasicSetting(void)
{
#if 0
	char *firewall_enable;

	firewall_enable = nvram_bufget(RT2860_NVRAM, "IPPortFilterEnable");

	// flush  ipport   filter   chain
    iptablesIPPortFilterFlush();

	if(!firewall_enable || !atoi(firewall_enable))
		return;
    iptablesIPPortFilterRun();
#endif
	return;
}

static void ipportFilter_enable(webs_t wp, char_t *path, char_t *query)
{
	char *firewall_enable, *submitUrl;

	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fl\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fl");
	
	firewall_enable = websGetVar(wp, T("portFilterEnabled"), T(""));  
    nvram_set("fl_enable",  atoi(firewall_enable)   ? "1" : "0");	
      
    printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start fl\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start fl");
	  
	printf("nvram_commit\n");
	nvram_commit();

	setWebMessage(0, NULL);
	submitUrl = websGetVar(wp, T("service_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);      
}

static void ipportFilter_enable2(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char *firewall_enable, *submitUrl, *default_policy;

	firewall_enable = websGetVar(wp, T("portFilterEnabled"), T(""));
	default_policy = websGetVar(wp, T("defaultFirewallPolicy"), T("0"));
      
    printf("%s\n",__FUNCTION__);
    printf("firewall_enable: %d\n",atoi(firewall_enable));
      
	switch(atoi(firewall_enable)){
	case 0:
		nvram_set(RT2860_NVRAM, "IPPortFilterEnable", "0");
		break;
	case 1:
		nvram_set(RT2860_NVRAM, "IPPortFilterEnable", "1");
		break;
	}	
	
	switch(atoi(default_policy)){
	case 1:
		nvram_set(RT2860_NVRAM, "DefaultFirewallPolicy", "1");
		break;
	case 0:
	default:
		nvram_set(RT2860_NVRAM, "DefaultFirewallPolicy", "0");
		break;
	}
	
	nvram_commit(RT2860_NVRAM);
	
	iptablesBasicSetting();

	submitUrl = websGetVar(wp, T("service_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);       
#endif
}


//add for hp
static void ipportFilter_service(webs_t wp, char_t *path, char_t *query)
{
    char rule[8192];
	char *mac_address;
	char *sip_1, *sip_2, *sprf, *sprt, *protocol, *action_str, *comment;
	char *dip_1, *dip_2, *dprf, *dprt;
	char *IPPortFilterRules;
	
	char *pflag,*pindex;
	char_t *submitUrl;
	int i, j, rule_count;
	int sprf_int, sprt_int, dprf_int, dprt_int, proto, action;

	//Steve add
	char temp_str[65];
	int flag=0;
	char tmp[TMP_LEN];
	int nrule,i_index;
	char *nv_enable = "fl_enable";
    char *nv_rule_num = "fl_rule_num";
    char *nv_rule_max = "fl_rule_max";
    char *nv_rule = "fl_rule";
    char str_buf[10]={0};
	
    pflag = websGetVar(wp, T("Service_rtnFlag"), T(""));
    pindex = websGetVar(wp, T("Service_rtnIndex"), T(""));
	mac_address = websGetVar(wp, T("mac_address"), T(""));
	sip_1 = websGetVar(wp, T("sip_address"), T("any"));
	sip_2 = websGetVar(wp, T("sip_address2"), T(""));
	sprf = websGetVar(wp, T("sFromPort"), T("0"));
	sprt = websGetVar(wp, T("sToPort"), T(""));
	dip_1 = websGetVar(wp, T("dip_address"), T("any"));
	dip_2 = websGetVar(wp, T("dip_address2"), T(""));
	dprf = websGetVar(wp, T("dFromPort"), T("0"));
	dprt = websGetVar(wp, T("dToPort"), T(""));
	protocol = websGetVar(wp, T("protocol"), T(""));
	//action_str = websGetVar(wp, T("action"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));	

    //adjust service name exist?
     char serviceName[SHORT_BUF_LEN];
     int z;
	 if(! strcmp(pflag, "0")) {
	 	for(z = 0; z < atoi(nvram_safe_get(nv_rule_num)); z++){
			ezplib_get_attr_val("fl_rule", z, "name", serviceName,
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if(!strcmp(comment, serviceName)){
				setWebMessage(1, "service name already exsits!");
				websRedirect(wp, "/local/advance/fw_s2.asp");
				return;
			}
		}
	}
	
	//printf("mac_address = %s , len=%d \n",mac_address,strlen(mac_address));
	printf("\nsip_1 = %s , len=%d \n",sip_1,strlen(sip_1));
	printf("sip_2 = %s , len=%d \n",sip_2,strlen(sip_2));
	printf("dip_1 = %s , len=%d \n",dip_1,strlen(dip_1));
	printf("dip_2 = %s , len=%d \n",dip_2,strlen(dip_2));
	printf("dprf = %s , len=%d \n",dprf,strlen(dprf));
	printf("dprt = %s , len=%d \n\n",dprt,strlen(dprt));

if(! strcmp(pflag, "0"))
{
	printf("==========> Apply \n");	
	
	#if 1
	if(!sip_1 || !dip_1 || !sprf || !dprf){
	    strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end2;
	}
		    
	if(!strlen(sip_1) && !strlen(dip_1) && !strlen(sprf) && !strlen(dprf)){
		strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end2;
	}
	
	#else
    if(!mac_address || !sip_1 || !dip_1 || !sprf || !dprf){
	    strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end2;
	}
		    
	if(!strlen(mac_address) && !strlen(sip_1) && !strlen(dip_1) && !strlen(sprf) && !strlen(dprf)){
		strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end2;
	}
	
	// we dont trust user input.....
	if(strlen(mac_address)){
		if(!isMacValid(mac_address)){
		    strcpy(temp_str, "MAC is invaild");
			flag=1;
			goto end2;
		}
	}
	#endif
	
	if(strlen(sip_1)){
		if(!isIpNetmaskValid(sip_1)){
			strcpy(temp_str, "IP error");
			flag=1;
			goto end2;
		}
	}else
		sip_1 = T("");
		
	if(strlen(sip_2)){
		if(!isIpNetmaskValid(sip_2)){
			strcpy(temp_str, "IP error");
			flag=1;
			goto end2;
		}
	}else
		sip_2 = T("");
		
	//
	if((strlen(sip_1))&&!(strlen(sip_2))){
		sip_2 = websGetVar(wp, T("sip_address"), T(""));
	}

	if(strlen(dip_1)){
		if(!isIpNetmaskValid(dip_1)){
			strcpy(temp_str, "IP error");
			flag=1;
			goto end2;
		}
	}else
    	dip_1 = T("");
    
    if(strlen(dip_2)){
		if(!isIpNetmaskValid(dip_2)){
			strcpy(temp_str, "IP error");
			flag=1;
			goto end2;
		}
	}else
    	dip_2 = T("");
    	
    //
	if((strlen(dip_1))&&!(strlen(dip_2))){
		dip_2 = websGetVar(wp, T("dip_address"), T(""));
	}	

	if(! strcmp(protocol, T("TCP"))){
		proto = PROTO_TCP;
		protocol = T("tcp");}
	else if( !strcmp(protocol, T("UDP"))){
		proto = PROTO_UDP;
		protocol = T("udp");}
	else if( !strcmp(protocol, T("None"))){
		proto = PROTO_NONE;
		protocol = T("");}
	else if( !strcmp(protocol, T("ALL"))){
		proto = PROTO_ALL;
		protocol = T("");}
	else if( !strcmp(protocol, T("BOTH"))){
		proto = PROTO_TCP_UDP;
		protocol = T("both");}	
	else
		goto end2;

	if(!strlen(sprf) || proto == PROTO_NONE || proto == PROTO_ICMP || proto == PROTO_ALL){
		sprf_int = 0;
		sprf = T(""); 
	}else{
		sprf_int = atoi(sprf);
		if(sprf_int == 0 || sprf_int > 65535){
			strcpy(temp_str, "Please set port between 1~65535");
			flag=1;
			goto end2;
		}
	}

	if(!strlen(sprt) || proto == PROTO_NONE || proto == PROTO_ICMP || proto == PROTO_ALL){
		if(!strlen(sprf)){
			sprt_int = 0;
			sprt = T(""); 
		}else{
			sprt = websGetVar(wp, T("sFromPort"), T("0"));
		}	
	}else{
		sprt_int = atoi(sprt);
		if(sprt_int ==0 || sprt_int > 65535){
			strcpy(temp_str, "Please set port between 1~65535");
			flag=1;
			goto end2;
		}
	}
	

	if(!strlen(dprf) || proto == PROTO_NONE || proto == PROTO_ICMP || proto == PROTO_ALL){
		dprf_int = 0;
		dprf = T(""); 
	}else{
		dprf_int = atoi(dprf);
		if(dprf_int ==0 || dprf_int > 65535){
			strcpy(temp_str, "Please set port between 1~65535");
			flag=1;
			goto end2;
		}
	}

	if(!strlen(dprt) || proto == PROTO_NONE || proto == PROTO_ICMP || proto == PROTO_ALL){
		if(!strlen(dprf)){
			dprt_int = 0;
			dprt = T(""); 
		}
		else
			dprt=websGetVar(wp, T("dFromPort"), T("0"));
	}else{
		dprt_int = atoi(dprt);
		if(dprt_int ==0 || dprt_int > 65535){
			strcpy(temp_str, "Please set port between 1~65535");
			flag=1;
			goto end2;
		}
	}

	//if(! (strcmp(action_str, T("Accept"))))
	//	action = ACTION_ACCEPT;
	//else if(! (strcmp(action_str, T("Drop"))))
		action = ACTION_DROP;
	//else
	//	return;

	if(strlen(comment) > 32)
		goto end2;
	// i know you will try to break our box... ;) 
	if(strchr(comment, ';') || strchr(comment, ','))
		goto end2;
		
//name^enable^extif^inif^srcipaddr_start^srcipaddr_end^dstipaddr_start^dstipaddr_end^
//proto^srcport_start^srcport_end^dstport_start^dstport_end^action^mark^bw_idx^sched_idx
	#if 1
	snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                     comment, "1", "", "", sip_1, sip_2, dip_1,dip_2, 
                     protocol, sprf,sprt,dprf,dprt,"0","","","");	
	#else
	snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                     comment, "0", "", "", sip_1, sip_1, dip_1,dip_1, 
                     "all", sprf,sprt,dprf,dprf,"0","","","");	
	#endif	
		
	printf("@@Normal Apply===>\n");
	nrule =atoi(nvram_safe_get(nv_rule_num));
	printf("nrule=%d\n", nrule);
	snprintf(tmp, sizeof(tmp), "%d", nrule+1);
	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fl\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fl");
	nvram_set(nv_rule_num, tmp);
	ezplib_append_rule(nv_rule, rule);
	printf("rule=%s\n", rule);
	
	nrule =atoi(nvram_safe_get(nv_rule_num));
	printf("now nrule=%d\n", nrule);
	
	for(i=0; i < nrule; i++)
	{
		sprintf(str_buf,"%04x",i+1);
		printf("str_buf=%s\n",str_buf);
		ezplib_replace_attr("fl_rule", i, "mark", str_buf);
		sprintf(str_buf,"%d",i);
		printf("str_buf=%s\n",str_buf);
		ezplib_replace_attr("fl_rule", i, "bw_idx", str_buf);
		ezplib_replace_attr("fl_rule", i, "sched_idx", str_buf);
	}

}

if(! strcmp(pflag, "2"))
{
	printf("==========> Delete \n");
	printf("pindex:%s \n",pindex);		
	i=0;
	i=atoi(pindex);
	printf("i:%d \n",i);
	
	nrule =atoi(nvram_safe_get(nv_rule_num));
    printf("nrule=%d\n", nrule);
	snprintf(tmp, sizeof(tmp), "%d", nrule-1);
    nvram_set(nv_rule_num, tmp);
	
	//===================================
	if(nrule==1){
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fl");
		nvram_set(nv_rule,"");
	}else{
		i_index=atoi(pindex);
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fl");
		ezplib_delete_rule(nv_rule, i_index);
	}
}

doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start fl");
	
printf("nvram_commit\n");
nvram_commit();

end2:

	//Steve add 
    if(flag)
    	setWebMessage(1, (char *)&temp_str);
    else
    	setWebMessage(0, NULL);
    	
submitUrl = websGetVar(wp, T("service_url"), T(""));   // hidden page
websRedirect(wp, submitUrl);  	
		return;
}


//add for hp
static void ipportFilter_service2(webs_t wp, char_t *path, char_t *query)
{
#if 0   
    
    char rule[8192];
	char *mac_address;
	char *sip_1, *sip_2, *sprf, *sprt, *protocol, *action_str, *comment;
	char *dip_1, *dip_2, *dprf, *dprt;
	char *IPPortFilterRules;
	
	//char *portFilter_enable;
	
	char *pflag,*pindex;
	char_t *submitUrl;
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;
	
	int sprf_int, sprt_int, dprf_int, dprt_int, proto, action;

	printf("hp:%s\n",__FUNCTION__);
	
    pflag = websGetVar(wp, T("Service_rtnFlag"), T(""));
    pindex = websGetVar(wp, T("Service_rtnIndex"), T(""));
	mac_address = websGetVar(wp, T("mac_address"), T(""));
	sip_1 = websGetVar(wp, T("sip_address"), T("any"));
	sip_2 = websGetVar(wp, T("sip_address2"), T(""));
	sprf = websGetVar(wp, T("sFromPort"), T("0"));
	sprt = websGetVar(wp, T("sToPort"), T(""));
	dip_1 = websGetVar(wp, T("dip_address"), T("any"));
	dip_2 = websGetVar(wp, T("dip_address2"), T(""));
	dprf = websGetVar(wp, T("dFromPort"), T("0"));
	dprt = websGetVar(wp, T("dToPort"), T(""));
	protocol = websGetVar(wp, T("protocol"), T(""));
	action_str = websGetVar(wp, T("action"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));	

if(! strcmp(pflag, "0"))
{
	printf("==========> Apply \n");	
    if(!mac_address || !sip_1 || !dip_1 || !sprf || !dprf)
		goto end2;
		    
	if(!strlen(mac_address) && !strlen(sip_1) && !strlen(dip_1) && !strlen(sprf) && !strlen(dprf))
		goto end2;
	
	// we dont trust user input.....
	if(strlen(mac_address)){
		if(!isMacValid(mac_address))
			goto end2;
	}
	
	if(strlen(sip_1)){
		if(!isIpNetmaskValid(sip_1))
			goto end2;
	}else
		sip_1 = T("any/0");

	if(strlen(dip_1)){
		if(!isIpNetmaskValid(dip_1))
			goto end2;
	}else
    	dip_1 = T("any/0");

	sip_2 = dip_2 = "0";

	if(! strcmp(protocol, T("TCP")))
		proto = PROTO_TCP;
	else if( !strcmp(protocol, T("UDP")))
		proto = PROTO_UDP;
	else if( !strcmp(protocol, T("None")))
		proto = PROTO_NONE;
	else if( !strcmp(protocol, T("ICMP")))
		proto = PROTO_ICMP;
	else
		goto end2;

	if(!strlen(sprf) || proto == PROTO_NONE || proto == PROTO_ICMP){
		sprf_int = 0;
	}else{
		sprf_int = atoi(sprf);
		if(sprf_int == 0 || sprf_int > 65535)
			goto end2;
	}

	if(!strlen(sprt) || proto == PROTO_NONE || proto == PROTO_ICMP){
		sprt_int = 0;
	}else{
		sprt_int = atoi(sprt);
		if(sprt_int ==0 || sprt_int > 65535)
			goto end2;
	}

	if(!strlen(dprf) || proto == PROTO_NONE || proto == PROTO_ICMP){
		dprf_int = 0;
	}else{
		dprf_int = atoi(dprf);
		if(dprf_int ==0 || dprf_int > 65535)
			goto end2;
	}

	if(!strlen(dprt) || proto == PROTO_NONE || proto == PROTO_ICMP){
		dprt_int = 0;
	}else{
		dprt_int = atoi(dprt);
		if(dprt_int ==0 || dprt_int > 65535)
			goto end2;
	}

	if(! (strcmp(action_str, T("Accept"))))
		action = ACTION_ACCEPT;
	else if(! (strcmp(action_str, T("Drop"))))
		action = ACTION_DROP;
	else
		goto end2;

	if(strlen(comment) > 32)
		goto end2;
	// i know you will try to break our box... ;) 
	if(strchr(comment, ';') || strchr(comment, ','))
		goto end2;

	if(   ( IPPortFilterRules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules")) && strlen(IPPortFilterRules)){
		snprintf(rule, sizeof(rule), "%s;%s,%s,%d,%d,%s,%s,%d,%d,%d,%d,%s,%s", IPPortFilterRules, sip_1, sip_2, sprf_int, sprt_int, dip_1, dip_2, dprf_int, dprt_int, proto, action, comment, mac_address);
	}else{
		snprintf(rule, sizeof(rule), "%s,%s,%d,%d,%s,%s,%d,%d,%d,%d,%s,%s", sip_1, sip_2, sprf_int, sprt_int, dip_1, dip_2, dprf_int, dprt_int, proto, action, comment, mac_address);
	}

	nvram_set(RT2860_NVRAM, "IPPortFilterRules", rule);
	nvram_commit(RT2860_NVRAM);

	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();
	
}

if(! strcmp(pflag, "2"))
{
	printf("==========> Delete \n");
	printf("pindex:%s \n",pindex);	
	i=0;
	i=atoi(pindex);
	printf("i:%d \n",i);
	

    char *rules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules");
    if(!rules || !strlen(rules) )
        goto end2;

	rule_count = 1;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	deleArray[0] = i;		

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "IPPortFilterRules", rules);
	nvram_commit(RT2860_NVRAM);

	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();
		
}


end2:
submitUrl = websGetVar(wp, T("service_url"), T(""));   // hidden page
websRedirect(wp, submitUrl);  
#endif
return;	
}





static void ipportFilter(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char rule[8192];
	char *mac_address;
	char *sip_1, *sip_2, *sprf, *sprt, *protocol, *action_str, *comment;
	char *dip_1, *dip_2, *dprf, *dprt;
	char *IPPortFilterRules;
	
	int sprf_int, sprt_int, dprf_int, dprt_int, proto, action;

	mac_address = websGetVar(wp, T("mac_address"), T(""));

	sip_1 = websGetVar(wp, T("sip_address"), T("any"));
	sip_2 = websGetVar(wp, T("sip_address2"), T(""));
	sprf = websGetVar(wp, T("sFromPort"), T("0"));
	sprt = websGetVar(wp, T("sToPort"), T(""));

	dip_1 = websGetVar(wp, T("dip_address"), T("any"));
	dip_2 = websGetVar(wp, T("dip_address2"), T(""));
	dprf = websGetVar(wp, T("dFromPort"), T("0"));
	dprt = websGetVar(wp, T("dToPort"), T(""));

	protocol = websGetVar(wp, T("protocol"), T(""));
	action_str = websGetVar(wp, T("action"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));

	if(!mac_address || !sip_1 || !dip_1 || !sprf || !dprf)
		return;

	if(!strlen(mac_address) && !strlen(sip_1) && !strlen(dip_1) && !strlen(sprf) && !strlen(dprf))
		return;

	// we dont trust user input.....
	if(strlen(mac_address)){
		if(!isMacValid(mac_address))
			return;
	}

	if(strlen(sip_1)){
		if(!isIpNetmaskValid(sip_1))
			return;
	}else
		sip_1 = T("any/0");

	if(strlen(dip_1)){
		if(!isIpNetmaskValid(dip_1))
			return;
	}else
    	dip_1 = T("any/0");

	sip_2 = dip_2 = "0";

	if(! strcmp(protocol, T("TCP")))
		proto = PROTO_TCP;
	else if( !strcmp(protocol, T("UDP")))
		proto = PROTO_UDP;
	else if( !strcmp(protocol, T("None")))
		proto = PROTO_NONE;
	else if( !strcmp(protocol, T("ICMP")))
		proto = PROTO_ICMP;
	else
		return;

	if(!strlen(sprf) || proto == PROTO_NONE || proto == PROTO_ICMP){
		sprf_int = 0;
	}else{
		sprf_int = atoi(sprf);
		if(sprf_int == 0 || sprf_int > 65535)
			return;
	}

	if(!strlen(sprt) || proto == PROTO_NONE || proto == PROTO_ICMP){
		sprt_int = 0;
	}else{
		sprt_int = atoi(sprt);
		if(sprt_int ==0 || sprt_int > 65535)
			return;
	}

	if(!strlen(dprf) || proto == PROTO_NONE || proto == PROTO_ICMP){
		dprf_int = 0;
	}else{
		dprf_int = atoi(dprf);
		if(dprf_int ==0 || dprf_int > 65535)
			return;
	}

	if(!strlen(dprt) || proto == PROTO_NONE || proto == PROTO_ICMP){
		dprt_int = 0;
	}else{
		dprt_int = atoi(dprt);
		if(dprt_int ==0 || dprt_int > 65535)
			return;
	}

	if(! (strcmp(action_str, T("Accept"))))
		action = ACTION_ACCEPT;
	else if(! (strcmp(action_str, T("Drop"))))
		action = ACTION_DROP;
	else
		return;

	if(strlen(comment) > 32)
		return;
	// i know you will try to break our box... ;) 
	if(strchr(comment, ';') || strchr(comment, ','))
		return;

	if(   ( IPPortFilterRules = nvram_bufget(RT2860_NVRAM, "IPPortFilterRules")) && strlen(IPPortFilterRules)){
		snprintf(rule, sizeof(rule), "%s;%s,%s,%d,%d,%s,%s,%d,%d,%d,%d,%s,%s", IPPortFilterRules, sip_1, sip_2, sprf_int, sprt_int, dip_1, dip_2, dprf_int, dprt_int, proto, action, comment, mac_address);
	}else{
		snprintf(rule, sizeof(rule), "%s,%s,%d,%d,%s,%s,%d,%d,%d,%d,%s,%s", sip_1, sip_2, sprf_int, sprt_int, dip_1, dip_2, dprf_int, dprt_int, proto, action, comment, mac_address);
	}

	nvram_set(RT2860_NVRAM, "IPPortFilterRules", rule);
	nvram_commit(RT2860_NVRAM);

	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();

	websHeader(wp);
	websWrite(wp, T("mac: %s<br>\n"), mac_address);	
	websWrite(wp, T("sip1: %s<br>\n"), sip_1);	
	websWrite(wp, T("sip2: %s<br>\n"), sip_2);	
	websWrite(wp, T("sFromPort: %s<br>\n"), sprf);
	websWrite(wp, T("sToPort: %s<br>\n"), sprt);
	websWrite(wp, T("dip1: %s<br>\n"), dip_1);	
	websWrite(wp, T("dip2: %s<br>\n"), dip_2);	
	websWrite(wp, T("dFromPort: %s<br>\n"), dprf);
	websWrite(wp, T("dToPort: %s<br>\n"), dprt);
	websWrite(wp, T("protocol: %s<br>\n"), protocol);
	websWrite(wp, T("action: %s<br>\n"), action_str);
	websWrite(wp, T("comment: %s<br>\n"), comment);

    websFooter(wp);
    websDone(wp, 200);   
#endif     
    return;
	
}


static void portForward(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char rule[8192];
	char *ip_address, *pfe, *prf, *prt, *protocol, *comment;
	char *PortForwardRules;

	int prf_int, prt_int, proto;

	pfe = websGetVar(wp, T("portForwardEnabled"), T(""));
	ip_address = websGetVar(wp, T("ip_address"), T(""));
	prf = websGetVar(wp, T("fromPort"), T(""));
	prt = websGetVar(wp, T("toPort"), T(""));
	protocol = websGetVar(wp, T("protocol"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));

	if(!pfe && !strlen(pfe))
		return;

	if(!atoi(pfe)){
		nvram_set(RT2860_NVRAM, "PortForwardEnable", "0");
		iptablesPortForwardFlush();		//disable
		//no chainge in rules
		goto end;
	}

	if(!strlen(ip_address) && !strlen(prf) && !strlen(prt) && !strlen(comment)){	// user choose nothing but press "apply" only
		nvram_set(RT2860_NVRAM, "PortForwardEnable", "1");
		iptablesPortForwardFlush();
		iptablesPortForwardRun();
		// no change in rules
		goto end;
	}

	if(!ip_address && !strlen(ip_address))
		return;
	if(!isIpValid(ip_address))
		return;

	// we dont trust user input.....
	if(!prf && !strlen(prf))
		return;
	if(!(prf_int = atoi(prf)) )
		return;
	if(prf_int > 65535)
		return;

	if(!prt)
		return;
	if(strlen(prt)){
		if( !(prt_int = atoi(prt)) )
			return;
		if(prt_int < prf_int)
			return;
		if(prt_int > 65535)
			return;
	}else{
		prt_int = 0;
	}

	if(! strcmp(protocol, "TCP"))
		proto = PROTO_TCP;
	else if( !strcmp(protocol, "UDP"))
		proto = PROTO_UDP;
	else if( !strcmp(protocol, "TCP&UDP"))
		proto = PROTO_TCP_UDP;
	else
		return;

	if(strlen(comment) > 32)
		return;
	/* i know you will try to break our box... ;) */
	if(strchr(comment, ';') || strchr(comment, ','))
		return;

	nvram_set(RT2860_NVRAM, "PortForwardEnable", "1");

	if(( PortForwardRules = nvram_bufget(RT2860_NVRAM, "PortForwardRules")) && strlen( PortForwardRules) )
		snprintf(rule, sizeof(rule), "%s;%s,%d,%d,%d,%s",  PortForwardRules, ip_address, prf_int, prt_int, proto, comment);
	else
		snprintf(rule, sizeof(rule), "%s,%d,%d,%d,%s", ip_address, prf_int, prt_int, proto, comment);

	nvram_set(RT2860_NVRAM, "PortForwardRules", rule);
	nvram_commit(RT2860_NVRAM);

	iptablesPortForwardFlush();
	// call iptables
	iptablesPortForwardRun();

end:
	websHeader(wp);
	websWrite(wp, T("portForwardEnabled: %s<br>\n"), pfe);
	websWrite(wp, T("ip: %s<br>\n"), ip_address);
	websWrite(wp, T("fromPort: %s<br>\n"), prf);
	websWrite(wp, T("toPort: %s<br>\n"), prt);
	websWrite(wp, T("protocol: %s<br>\n"), protocol);
	websWrite(wp, T("comment: %s<br>\n"), comment);

    websFooter(wp);
    websDone(wp, 200);    
#endif    
}
//Steve add

static int getCfgNat(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;
	char rule_name[30]={0};
	char rule_portnum[30]={0};
	char *str_rules,*str_portnum;
	char  active[8],name[32],ip_address[32];
	char num_idx[3]={0};
	
	char tmp_active[SHORT_BUF_LEN];
	char tmp_name[SHORT_BUF_LEN];
	char tmp_port[SHORT_BUF_LEN];
	char tmp_ip[SHORT_BUF_LEN];

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}

if(D_NAT_FLAG==1){
	
	ezplib_get_attr_val("fr_rule", (D_NAT_INDEX-1), "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
    ezplib_get_attr_val("fr_rule", (D_NAT_INDEX-1), "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
         
    ezplib_get_attr_val("fr_rule", (D_NAT_INDEX-1), "extport_start", tmp_port, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
  
    ezplib_get_attr_val("fr_rule", (D_NAT_INDEX-1), "ipaddr", tmp_ip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
    
    if(! strcmp(field, "nat_service")){
	   return websWrite(wp, T("%s"), tmp_name);
	}
    
    if(! strcmp(field, "nat_active")){
		if(! strcmp(tmp_active, "1"))
				return websWrite(wp, T("%s"), "1");
		else
				return websWrite(wp, T("%s"), "0");
	}
	if(! strcmp(field, "nat_ip"))
	   return websWrite(wp, T("%s"), tmp_ip);
	   
	if(! strcmp(field, "nat_port")){
	   return websWrite(wp, T("%s"), tmp_port);
	}
	
	if(! strcmp(field, "nat_flag")){
	   return websWrite(wp, T("%s"), "1");
	}
	
	if(! strcmp(field, "nat_index")){
	   printf("D_NAT_INDEX=%d\n",D_NAT_INDEX);
	   return websWrite(wp, T("%d"), D_NAT_INDEX);
	}
	
}
else{
	
	if(! strcmp(field, "nat_active"))
			return websWrite(wp, T("%s"), "0");
		
		   
	if(! strcmp(field, "nat_service")){
	   return websWrite(wp, T("%s"), "");
	}
	   
	if(! strcmp(field, "nat_ip"))
	   return websWrite(wp, T("%s"), "0.0.0.0");  
	   
	if(! strcmp(field, "nat_flag")){
	   return websWrite(wp, T("%s"), "0");
	}   
}
	
	   
	 
	/*
	value = nvram_bufget(RT2860_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	*/
	return 0;
}

/*get one-to-one nat configure, add by wind 2013-12-09*/
static int getCfg_OneToOneNat(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field = NULL;
	char *value = NULL;
	char rule_name[30]={0};
	char rule_portnum[30]={0};
	char *str_rules = NULL,*str_portnum = NULL;
	char  active[8] = {0},name[32] = {0},ip_address[32] = {0};
	char tmp_active[SHORT_BUF_LEN] = {0};
	char tmp_name[SHORT_BUF_LEN] = {0};
	char tmp_intIp[SHORT_BUF_LEN] = {0};
	char tmp_extIp[SHORT_BUF_LEN] = {0};
	char tmp_port[SHORT_BUF_LEN] = {0};
	char tmp_forward_check[SHORT_BUF_LEN] = {0};
	char tmp_index[SHORT_BUF_LEN] = {0};

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}

	if(gOne2OneNATFlag==1){
		sprintf(tmp_index, "%d", gOne2OneNATIdx);
		ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        
    	ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
    	ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "int_addr", tmp_intIp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
    	ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "ext_addr", tmp_extIp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
    	ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "extport_start", tmp_port, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
  
    	ezplib_get_attr_val("one_to_one_rule", (gOne2OneNATIdx-1), "forward_check", tmp_forward_check, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
     	if(! strcmp(field, "edit_index")){
	   		return websWrite(wp, T("%s"), tmp_index);
		}	
    	if(! strcmp(field, "OneToOne_service")){
	   		return websWrite(wp, T("%s"), tmp_name);
		}	
    
    	if(! strcmp(field, "OneToOne_active")){
			if(! strcmp(tmp_active, "1"))
				return websWrite(wp, T("%s"), "1");
			else
				return websWrite(wp, T("%s"), "0");
		}
		if(! strcmp(field, "int_ip"))
	   		return websWrite(wp, T("%s"), tmp_intIp);
		if(! strcmp(field, "ext_ip"))
	   		return websWrite(wp, T("%s"), tmp_extIp);   
		if(! strcmp(field, "forward_port")){
	   		return websWrite(wp, T("%s"), tmp_port);
		}
		if(! strcmp(field, "forward_check1")){
	   		if(!strcmp(tmp_forward_check,"1"))
				return websWrite(wp, T("%s"), "checked");
	   		else
				return websWrite(wp, T("%s"), "");
		}
		if(! strcmp(field, "forward_check2")){
	   		if(!strcmp(tmp_forward_check,"0"))
				return websWrite(wp, T("%s"), "checked");
	   		else
				return websWrite(wp, T("%s"), "");
		}
		if(! strcmp(field, "OneToOne_flag")){
	   		return websWrite(wp, T("%s"), "1");
		}
	
		if(! strcmp(field, "OneToOne_index")){
	   		printf("gOne2OneNATIdx=%d\n",gOne2OneNATIdx);
	   		return websWrite(wp, T("%d"), gOne2OneNATIdx);
		}	
	
	}
	else{
	
		if(! strcmp(field, "OneToOne_active"))
			return websWrite(wp, T("%s"), "1");
		if(! strcmp(field, "OneToOne_service")){
	   		return websWrite(wp, T("%s"), "");
		}	
	   
		if(! strcmp(field, "int_ip"))
	   		return websWrite(wp, T("%s"), "0.0.0.0");
		if(! strcmp(field, "ext_ip"))
	   		return websWrite(wp, T("%s"), "0.0.0.0"); 
		if(! strcmp(field, "forward_check1"))
	   		return websWrite(wp, T("%s"), "checked"); 
		if(! strcmp(field, "forward_check2"))
	   		return websWrite(wp, T("%s"), ""); 
		if(! strcmp(field, "OneToOne_flag")){
	   		return websWrite(wp, T("%s"), "0");
		}	   
	}
	return 0;
}

/* Added by Bruce, 2012-5-16 */
/*
  *   return: the recoard index.
  */
int query_nat_database(char * p_active, char *p_name, char *p_port, char *ip_address, int rule_idx)
{
	char *nv_enable = "fr_enable";
    	char *nv_rule_num = "fr_rule_num";
    	char *nv_rule_max = "fr_rule_max";
    	char *nv_rule = "fr_rule";
	int nrule  = 0, i = 0;
	char act_buf[8], name_buf[32], port_buf[8], ip_buf[64];
	char *pb_name = name_buf, *pb_port = port_buf, *pb_ip = ip_buf,
		 tmp_buf[32], *tmp_p_port = NULL, tmp_pb_port = NULL, *p = NULL;
	int start_port = 0, end_port = 0, new_start_port = 0, new_end_port = 0;
	int flag = 0;
	printf("<<<<<<<<<<<<<<<<<<<<active: %s, name: %s, port: %s, ip: %s\n", p_active, p_name, p_port, ip_address);
	nrule =atoi(nvram_safe_get(nv_rule_num));
	memset(tmp_buf, 0, sizeof(tmp_buf));	
	tmp_p_port = strstr(p_port, "-");
	if (tmp_p_port != NULL) {
		snprintf(tmp_buf, (tmp_p_port -p_port) , "%s", p_port);
		p = strtok(p_port, "-");
		if (p != NULL) {
			new_start_port = atoi(p);
			printf("new_start_port=%d\n", new_start_port);
		}														
		new_end_port = atoi(tmp_p_port + 1);
		printf("new_end_port=%d\n", new_end_port);
	} else {
		new_start_port = atoi(p_port);
		printf("single new_start_port=%d\n", new_start_port);
	}
	for(i = 0 ; i < nrule; i++ ) {
		printf("<<<<<<<<<<<<<<<<<<<<rule number: %d\n", i);
		memset( pb_name, 0, sizeof( pb_name));
		memset( pb_port, 0, sizeof( pb_port));
		memset( pb_ip, 0, sizeof(pb_ip));
		ezplib_get_attr_val("fr_rule", i, "extport_start", pb_port, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("fr_rule", i, "ipaddr", pb_ip, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("fr_rule", i, "name", pb_name, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		tmp_pb_port = strstr(pb_port, "-");
		if (tmp_pb_port != NULL) {
			p = rindex(pb_port, '-');
			if (p != NULL) 
				end_port = atoi(p+1);
			p = strtok(pb_port, "-");
			if (p != NULL) {
				start_port = atoi(p);
				printf("start_port=%d\n", start_port);
		 	}
			printf("end_port=%d\n", end_port);
		} else {
				start_port = atoi(pb_port);
				printf("single start_port = %d\n", start_port);
		}
		printf("<<<<<<<<<<<<<<<<<<name: %s, port: %s, ip address: %s\n", pb_name, pb_port, pb_ip);
		if( (tmp_p_port == NULL) && (tmp_pb_port == NULL) )   {
			if ( (new_start_port == start_port )) {
				if ( rule_idx != i ) {
					printf("1<<<<<<<<<<<<<<<<<<<<<<<discovery rule existed: %d\n", i);
					return i;
				}
			}
		}else if ( (tmp_p_port == NULL) && (tmp_pb_port != NULL)) {
			if ( (new_start_port >= start_port) && ( new_start_port <= end_port) ) {
				if ( rule_idx != i ) {
					printf("2<<<<<<<<<<<<<<<<<<<<<<<discovery rule existed: %d\n", i);
					return i;
				}
			}
		} else if ( (tmp_p_port != NULL) && ( tmp_pb_port == NULL) ) {
				if ( (new_start_port <= start_port ) && ( start_port <= new_end_port ) ) {
					if ( rule_idx != i ) {
						printf("2<<<<<<<<<<<<<<<<<<<<<<<discovery rule existed: %d\n", i);
						return i;
					}
				}
		}else if ( (tmp_p_port != NULL) && ( tmp_pb_port == NULL) ) {
				if ( (new_start_port <= start_port ) && ( start_port <= new_end_port ) ) {
					if ( rule_idx != i ) {
						printf("3<<<<<<<<<<<<<<<<<<<<<<<discovery rule existed: %d\n", i);
						return i;
					}
				}
		}else if( (tmp_p_port != NULL) && (tmp_pb_port != NULL) ) {
				if ( (new_end_port >= start_port) && ( new_end_port <= end_port) )
					flag = 1;
				else if ( (new_start_port >= start_port) && ( new_start_port <= end_port) )
						flag = 1;
				else if ( (new_start_port < start_port) && ( new_end_port > end_port) )
					flag = 1;
				if ( (flag == 1) && (rule_idx != i) ) {
					printf("4<<<<<<<<<<<<<<<<<<<<<<<discovery rule existed: %d\n", i);
					return i;
				}
		}
	}
	return -1;
}

/* Steve, for Port Forward --> NAT Application hp */
static void portForward_natAPP(webs_t wp, char_t *path, char_t *query)
{	

	char rule[8192];
	char *submitUrl,*ip_address, *p_active, *p_port, *protocol, *p_name;
	char *pflag,*pindex;
	char *PortForwardRules;
	char rulr_name1[30]={0};
	char rulr_name2[30]={0};
	int i;
	
	//Steve add
	char temp_str[65];
	int flag=0;
	
	//Steve new add
	char tmp[TMP_LEN];
	int nrule,i_index;
	char *nv_enable = "fr_enable";
    char *nv_rule_num = "fr_rule_num";
    char *nv_rule_max = "fr_rule_max";
    char *nv_rule = "fr_rule";

	printf("\n%s\n","__FUNCTION__");
	
	p_active = websGetVar(wp, T("PortRule_Active"), T(""));
	p_name = websGetVar(wp, T("PortRule_Name"), T(""));
	p_port = websGetVar(wp, T("PortRule_Port"), T(""));
	ip_address = websGetVar(wp, T("PortRule_SrvAddr"), T(""));
	pflag = websGetVar(wp, T("PortFor_rtnFlag"), T(""));
	pindex = websGetVar(wp, T("PortFor_rtnCurIndex"), T(""));
	
	
#if 1	
	printf("PortRule_Active: %s\n",p_active);
	printf("PortRule_Name: %s\n",p_name);
	printf("PortRule_Port: %s\n",p_port);
	printf("PortRule_SrvAddr: %s\n",ip_address);
	printf("PortFor_rtnFlag: %s\n",pflag);
	printf("PortFor_rtnCurIndex: %s\n",pindex);
#endif
if(! strcmp(pflag, "0"))
{
	printf("==========> Apply \n");
	if(!p_active && !strlen(p_active)){
		strcpy(temp_str, "CGI parameter error.");
		flag=1;
		goto end;
	}

	if(!strlen(p_port) && !strlen(p_name)){	// user choose nothing but press "apply" only
		strcpy(temp_str, "Setting error.");
		flag=1;
		goto end;
	}

	if(!ip_address && !strlen(ip_address)){
		strcpy(temp_str, "CGI parameter error.");
		flag=1;
		goto end;
	}
	if(!isIpValid(ip_address)){
		strcpy(temp_str, "IP error.");
		flag=1;
		goto end;
	}
	
	if(! strcmp(ip_address, "0.0.0.0")){
		strcpy(temp_str, "IP error.");
		flag=1;
		goto end;
	}
     
	if(strlen(p_name) > 32){
		strcpy(temp_str, "Length is too log.");
		flag=1;
		goto end;
	}
	/* i know you will try to break our box... ;) */
	if(strchr(p_name, ';') || strchr(p_name, ',')){
		strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end;
	}

	//nvram_set(RT2860_NVRAM, "PortForwardEnable", "1");
	//snprintf(rule, sizeof(rule), "%s,%s,%s,%s",p_name,p_name, ip_address, "3");
	snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s^%s^%s",
		p_name, p_active, "wan0", "both", p_port, p_port, ip_address,
		"", "");
	
	//temp
	/*
	if(strchr(p_port, '-') || strchr(p_port, ',') || strchr(p_port, ';')){
		printf("\n================= tmp \n");
		strcpy(temp_str, "Invaild setting");
		flag=1;
		goto end;
	}*/

	if(D_NAT_FLAG==0)
	{
	    printf("@@Normal Apply===>\n");
	   	nrule =atoi(nvram_safe_get(nv_rule_num));
		printf("nrule=%d\n", nrule);
		snprintf(tmp, sizeof(tmp), "%d", nrule+1);
		printf("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fr\n");
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fr");
		nvram_set(nv_rule_num, tmp);
		ezplib_append_rule(nv_rule, rule);
		printf("rule=%s\n", rule);	  
	}

	if(D_NAT_FLAG==1)
	{
		if (query_nat_database(p_active, p_name, p_port, ip_address, D_NAT_INDEX-1) != -1 ) {
			strcpy(temp_str, "The port already existed");
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<The rule already existed\n");
			flag = 1;
			goto end;
		}
		 printf("@@Edit Apply===>\n");
		 printf("@@Edit D_NAT_INDEX:%d\n",D_NAT_INDEX);
		 printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fr\n");
		 doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fr");
		 ezplib_replace_rule(nv_rule, D_NAT_INDEX-1, rule);
	}
	
	D_NAT_FLAG=0;
	
	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start fr\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start fr");
	printf("nvram_commit\n");
	nvram_commit();
}

if(! strcmp(pflag, "1"))
{
	printf("==========> Edit \n");
	i=0;
	i=atoi(pindex);
	
	if(i <= NAT_APPLICATION_NUM && i > 0)
		D_NAT_INDEX=i;
		
	printf("D_NAT_INDEX=%d\n",D_NAT_INDEX);
		
	D_NAT_FLAG=1;
}

if(! strcmp(pflag, "2"))
{
	printf("==========> Delete\n");
	D_NAT_FLAG=0;
	
	nrule =atoi(nvram_safe_get(nv_rule_num));
    printf("nrule=%d\n", nrule);
	snprintf(tmp, sizeof(tmp), "%d", nrule-1);
    nvram_set(nv_rule_num, tmp);
	
	//===================================
	if(nrule==1){
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fr");
		nvram_set(nv_rule,"");
	}else{
		i_index=atoi(pindex);
		i_index=i_index-1;
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop fr");
		ezplib_delete_rule(nv_rule, i_index);
	}
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start fr");
	
	printf("nvram_commit\n");
	nvram_commit();
}
	/*cancel*/
	if(! strcmp(pflag, "3"))
	{
		D_NAT_FLAG=0;
	}


end:
    
    //Steve add 
    if(flag)
    	setWebMessage(1, (char *)&temp_str);
    else
    	setWebMessage(0, NULL);
    	
    	
    submitUrl = websGetVar(wp, T("nat_app_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
}

/* 
  * add by wind 2013-12-10
 *  process one-to-one NAT form
 *
 */
static void oneToOne_natAPP(webs_t wp, char_t *path, char_t *query)
{	

	char rule[8192] = {0};
	char *submitUrl = NULL,*p_name = NULL,*p_active = NULL;
	char *int_addr = NULL,*ext_addr = NULL,*p_port = NULL,*p_check = NULL;
	char *pflag = NULL,*pindex = NULL;
	int i;
	
	//Steve add
	char temp_str[65] = {0};
	int flag=0;
	
	//Steve new add
	char tmp[TMP_LEN] = {0};
	int nrule,i_index;
    char *nv_rule_num = "one_to_one_num";
    char *nv_rule_max = "one_to_one_max";
    char *nv_rule = "one_to_one_rule";
	p_active = websGetVar(wp, T("PortRule_Active"), T(""));
	p_name = websGetVar(wp, T("PortRule_Name"), T(""));
	int_addr = websGetVar(wp, T("PortRule_IntAddr"), T(""));
	ext_addr = websGetVar(wp, T("PortRule_ExtAddr"), T(""));
	p_port = websGetVar(wp, T("PortRule_Port"), T(""));
	p_check = websGetVar(wp, T("forward_enable"), T(""));
	pflag = websGetVar(wp, T("PortFor_rtnFlag"), T(""));
	pindex = websGetVar(wp, T("PortFor_rtnCurIndex"), T(""));
	/*normal apply*/	
	if(! strcmp(pflag, "0"))
	{
		if(!p_active && !strlen(p_active)){
			strcpy(temp_str, "CGI parameter error.");
			flag=1;
			goto end;
		}
		/*check internal IP*/
		if(!int_addr && !strlen(int_addr)){
			strcpy(temp_str, "CGI parameter error.");
			flag=1;
			goto end;
		}
		if(!isIpValid(int_addr)){
			strcpy(temp_str, "IP error.");
			flag=1;
			goto end;
		}
	
		if(! strcmp(int_addr, "0.0.0.0")){
			strcpy(temp_str, "IP error.");
			flag=1;
			goto end;
		}
     	/*check extternal IP*/
	 	if(!ext_addr && !strlen(ext_addr)){
			strcpy(temp_str, "CGI parameter error.");
			flag=1;
			goto end;
		}
		if(!isIpValid(ext_addr)){
			strcpy(temp_str, "IP error.");
			flag=1;
			goto end;
		}
	
		if(! strcmp(ext_addr, "0.0.0.0")){
			strcpy(temp_str, "IP error.");
			flag=1;
			goto end;
		}
	
		if(strlen(p_name) > 32){
			strcpy(temp_str, "Length is too log.");
			flag=1;
			goto end;
		}

		snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s^%s^%s",
			p_name, p_active, "wan0", "", int_addr, ext_addr, p_port,p_port,p_check,
			"", "");
		/*normal apply*/
		if(gOne2OneNATFlag==0)
		{
	   		nrule =atoi(nvram_safe_get(nv_rule_num));
			snprintf(tmp, sizeof(tmp), "%d", nrule+1);
			doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop onetoone");
			nvram_set(nv_rule_num, tmp);
			ezplib_append_rule(nv_rule, rule);
		}
		/*after edit,then apply*/
		if(gOne2OneNATFlag==1)
		{
		 	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop onetoone");
		 	ezplib_replace_rule(nv_rule, gOne2OneNATIdx-1, rule);
		}
	
		gOne2OneNATFlag=0;
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start onetoone");
		nvram_commit();
	}
	/*edit*/
	if(! strcmp(pflag, "1"))
	{
		i=0;
		i=atoi(pindex);		
		gOne2OneNATIdx=i;		
		gOne2OneNATFlag=1;
	}
	/*delete*/
	if(! strcmp(pflag, "2"))
	{
		gOne2OneNATFlag=0;
		nrule =atoi(nvram_safe_get(nv_rule_num));
		snprintf(tmp, sizeof(tmp), "%d", nrule-1);
    	nvram_set(nv_rule_num, tmp);
		if(nrule==1){
			doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop onetoone");
			nvram_set(nv_rule,"");
		}else{
			i_index=atoi(pindex);
			i_index=i_index-1;
			doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop onetoone");
			ezplib_delete_rule(nv_rule, i_index);
		}
		doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start onetoone");
		nvram_commit();
	}
	/*cancel*/
	if(! strcmp(pflag, "3"))
	{
		gOne2OneNATFlag=0;
	}

end:
    
    //Steve add 
    if(flag)
    	setWebMessage(1, (char *)&temp_str);
    else
    	setWebMessage(0, NULL);
    	
    	
    submitUrl = websGetVar(wp, T("nat_app_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
}


/*
  *  Added by Bruce Liu, 2013-5-17
  *  Return: 
  *        -1: *src included non-digital the charactor.
  *	       0: is digital string.
  */

int isint(char * src)
{
	int i = 0;

	for ( i = 0 ; i < strlen(src); i++ )
	{
		if ( !isdigit(src[i]) ) {
			return -1;
		}
	}
	
	return 0;
}

/* Steve, for Port Trigger --> NAT Advance hp */
static void portTrigger_natADV(webs_t wp, char_t *path, char_t *query)
{	
#if 1
	int i=0;
	char *submitUrl,*p_Name, *p_IncS, *p_IncE, *p_TriS, *p_TriE;
	char str_Name[20]={0};
	char str_IncS[20]={0};
	char str_IncE[20]={0};
	char str_TriS[20]={0};
	char str_TriE[20]={0};
	char str_rule[20]={0};
	char rule[1024];
	char *nv_rule = "pt_rule";
	int flag = 0;
	char buf[32];
	
	printf("%s\n",__FUNCTION__);
	
	printf("/etc/rc.d/T60ipt-trigger stop\n");
	doSystem("/etc/init.d/ipt-trigger stop");
	
	for(i=0;i<NAT_ADVANCE_NUM;i++){
		sprintf(str_Name, "%s%d", "PortRule_Name", i+1);
		sprintf(str_IncS, "%s%d", "PortRule_IncS", i+1);
		sprintf(str_IncE, "%s%d", "PortRule_IncE", i+1);
		sprintf(str_TriS, "%s%d", "PortRule_TriS", i+1);
		sprintf(str_TriE, "%s%d", "PortRule_TriE", i+1);
		
		p_Name = websGetVar(wp, T(str_Name), T(""));
		p_IncS = websGetVar(wp, T(str_IncS), T(""));
		p_IncE = websGetVar(wp, T(str_IncE), T(""));
		p_TriS = websGetVar(wp, T(str_TriS), T(""));
		p_TriE = websGetVar(wp, T(str_TriE), T(""));

		if( ( isint(p_IncS) ) || ( isint(p_IncE) ) || (isint(p_TriS) ) || (isint(p_TriE) ) ) {
			printf("<<<<<<<<<<<<<<<<<<<<<<<<invalid port\n");
			flag = 1;
			strcpy(buf, "invalid port");
			break;
		} else if((atoi(p_IncS)==0) && (atoi(p_IncE)==0) && (atoi(p_TriS)==0) && (atoi(p_TriE)==0)){
			//printf("%d = 00000000\n",i+1);
			sprintf(rule, "%s^%s^%s^%s^%s^%s^%s", "", "0","all", p_IncS, p_IncE, p_TriS, p_TriE);	
			//nvram_bufset(RT2860_NVRAM, str_rule, "");
			//printf("rule=%s\n", rule);
			ezplib_replace_rule(nv_rule, i, rule);
		}else{
			sprintf(rule, "%s^%s^%s^%s^%s^%s^%s", p_Name,"1","all", p_IncS, p_IncE, p_TriS, p_TriE);
			//printf("rule=%s\n", rule);
			ezplib_replace_rule(nv_rule, i, rule);
	   }
	}
	
	printf("/etc/rc.d/T60ipt-trigger start\n");
	doSystem("/etc/init.d/ipt-trigger start");
	
	printf("nvram_commit\n");
	nvram_commit();

	//Steve add 2009/03/18
     #if 1//Steve add 2009/03/18	
	 if(!flag)
		setWebMessage(0, NULL);
	 else
	 	setWebMessage(1, buf);
	#endif
      
    submitUrl = websGetVar(wp, T("nat_adv_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
#endif
}


#if 1 //define FIREWALL_PORTTRIGGER
//Setve add port Trigger
static void portTrigger(webs_t wp, char_t *path, char_t *query)
{
	#if 0
	char rule[1024];
	//char *ip_address, *pfe, *prf, *prt, *protocol, *comment;
	char *pfe, *in_prf, *in_prt, *in_protocol, *comment;
	char *tg_prf, *tg_prt, *tg_protocol;
	char *PortTriggerRules;

	//int prf_int, prt_int, proto;
	int in_prf_int, in_prt_int, in_proto;
	int tg_prf_int, tg_prt_int, tg_proto;

	char_t *submitUrl;

	pfe = websGetVar(wp, T("portTriggerEnabled"), T(""));
	in_prf = websGetVar(wp, T("in_fromPort"), T(""));
	in_prt = websGetVar(wp, T("in_toPort"), T(""));
	in_protocol = websGetVar(wp, T("in_protocol"), T(""));
	tg_prf = websGetVar(wp, T("tg_fromPort"), T(""));
	tg_prt = websGetVar(wp, T("tg_toPort"), T(""));
	tg_protocol = websGetVar(wp, T("tg_protocol"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));

	if(!pfe && !strlen(pfe))
		return;

	if(!atoi(pfe)){
		nvram_set(RT2860_NVRAM, "PortTriggerEnable", "0");
		iptablesPortTriggerFlush();		//disable
		//no chainge in rules
		goto end;
	}

	if(!strlen(in_prf) && !strlen(in_prt) && !strlen(tg_prf) && !strlen(tg_prt) && !strlen(comment)){	// user choose nothing but press "apply" only
		nvram_set(RT2860_NVRAM, "PortTriggerEnable", "1");
		iptablesPortTriggerFlush();
		iptablesPortTriggerRun();
		// no change in rules
		goto end;
	}

	// we dont trust user input.....
	if(!in_prf && !strlen(in_prf))
		return;
	if(!(in_prf_int = atoi(in_prf)) )
		return;
	if(in_prf_int > 65535)
		return;

	if(!in_prt)
		return;
	if(strlen(in_prt)){
		if( !(in_prt_int = atoi(in_prt)) )
			return;
		if(in_prt_int < in_prf_int)
			return;
		if(in_prt_int > 65535)
			return;
	}else{
		in_prt_int = 0;
	}

	if(! strcmp(in_protocol, "TCP"))
		in_proto = PROTO_TCP;
	else if( !strcmp(in_protocol, "UDP"))
		in_proto = PROTO_UDP;
	else if( !strcmp(in_protocol, "TCP&UDP"))
		in_proto = PROTO_TCP_UDP;
	else
		return;


	// we dont trust user input.....
	if(!tg_prf && !strlen(tg_prf))
		return;
	if(!(tg_prf_int = atoi(tg_prf)) )
		return;
	if(tg_prf_int > 65535)
		return;

	if(!tg_prt)
		return;
	if(strlen(tg_prt)){
		if( !(tg_prt_int = atoi(tg_prt)) )
			return;
		if(tg_prt_int < tg_prf_int)
			return;
		if(tg_prt_int > 65535)
			return;
	}else{
		tg_prt_int = 0;
	}

	if(! strcmp(tg_protocol, "TCP"))
		tg_proto = PROTO_TCP;
	else if( !strcmp(tg_protocol, "UDP"))
		tg_proto = PROTO_UDP;
	else if( !strcmp(tg_protocol, "TCP&UDP"))
		tg_proto = PROTO_TCP_UDP;
	else
		return;

	if(strlen(comment) > 32)
		return;
	/* i know you will try to break our box... ;) */
	if(strchr(comment, ';') || strchr(comment, ','))
		return;

#if 0
	nvram_set(RT2860_NVRAM, "PortTriggerEnable", "1");

	if(( PortTriggerRules = nvram_bufget(RT2860_NVRAM, "PortTriggerRules")) && strlen( PortTriggerRules) )
		snprintf(rule, 1024, "%s;%d,%d,%d,%d,%d,%d,%s",  PortTriggerRules, in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int, comment);
	else
		snprintf(rule, 1024, "%d,%d,%d,%d,%d,%d,%s", in_proto, in_prf_int, in_prt_int, tg_proto, tg_prf_int, tg_prt_int, comment);

	nvram_set(RT2860_NVRAM, "PortTriggerRules", rule);
#endif

	iptablesPortTriggerFlush();
	// call iptables
	iptablesPortTriggerRun();

end:
    //printf("===> url ==> \n");
	submitUrl = websGetVar(wp, T("ptfil_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
	#endif
}
#endif




static void BasicSettings(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char *default_policy, *firewall_enable;

	firewall_enable = websGetVar(wp, T("portFilterEnabled"), T(""));
	default_policy = websGetVar(wp, T("defaultFirewallPolicy"), T("0"));

	switch(atoi(firewall_enable)){
	case 0:
		nvram_set(RT2860_NVRAM, "IPPortFilterEnable", "0");
		break;
	case 1:
		nvram_set(RT2860_NVRAM, "IPPortFilterEnable", "1");
		break;
	}

	switch(atoi(default_policy)){
	case 1:
		nvram_set(RT2860_NVRAM, "DefaultFirewallPolicy", "1");
		break;
	case 0:
	default:
		nvram_set(RT2860_NVRAM, "DefaultFirewallPolicy", "0");
		break;
	}
	nvram_commit(RT2860_NVRAM);

	iptablesBasicSetting();

	websHeader(wp);
	websWrite(wp, T("default_policy: %s<br>\n"), default_policy);
    websFooter(wp);
    websDone(wp, 200);   
#endif     
}

static void DMZ(webs_t wp, char_t *path, char_t *query)
{
	#if 0
	char *dmzE, *ip_address;

	dmzE = websGetVar(wp, T("DMZEnabled"), T(""));
	ip_address = websGetVar(wp, T("DMZIPAddress"), T(""));

	// someone use malform page.....
	if(!dmzE && !strlen(dmzE))
		return;

	// we dont trust user input, check all things before doing changes
	if(atoi(dmzE) && !isIpValid(ip_address))	// enable && invalid mac address
		return;

	iptablesDMZFlush();
	if(atoi(dmzE) == 0){		// disable
		nvram_set(RT2860_NVRAM, "DMZEnable", "0");
	}else{					// enable
		nvram_set(RT2860_NVRAM, "DMZEnable", "1");
		if(strlen(ip_address)){
			nvram_set(RT2860_NVRAM, "DMZIPAddress", ip_address);
		}
	}

	nvram_commit(RT2860_NVRAM);
	iptablesDMZRun();

	websHeader(wp);
	websWrite(wp, T("DMZEnabled: %s<br>\n"), dmzE);
	websWrite(wp, T("ip_address: %s<br>\n"), ip_address);
    websFooter(wp);
    websDone(wp, 200);        
    #endif
}

//Steve add for hp
static void DMZ_NAT(webs_t wp, char_t *path, char_t *query)
{
#if 1
	char *natE, *ip_address;
	//int natflag=0;
	int dmz_enable=0;
	char rule[256]={0};

    printf("Steve:%s\n",__FUNCTION__);
	natE = websGetVar(wp, T("NATEnabled"), T(""));
	ip_address = websGetVar(wp, T("NATServerIPAddress"), T(""));
    if(! strcmp(ip_address, "")){
      strcpy(ip_address, "0.0.0.0");
    }
  		
	printf("natE: %s \n",natE);		
	nvram_set("nat_rule",  atoi(natE)   ? "1" : "0");	

   if(! strcmp(ip_address, "0.0.0.0")){
		dmz_enable=0;
	}
	else{
		dmz_enable=1;
		}		
	sprintf(rule, "DMZ%d^%d^%s", 0, dmz_enable, ip_address);
	nvram_set("fr_dmz_rule", rule);	
	
	printf("nvram_commit\n");
	nvram_commit();
	
    doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start fr");
    doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start snat");
    doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start onetoone");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start devide");
    doSystem("NUM=0  /etc/rc.common /etc/init.d/ipt-trigger stop");
    doSystem("NUM=0  /etc/rc.common /etc/init.d/ipt-trigger start");
    //change by wind 2013-12-12
    //doSystem("/sbin/ezp-nat");	
	
    #if 1//Steve add 2009/03/18		
		setWebMessage(0, NULL);
	#endif

    char_t *submitUrl;
    submitUrl = websGetVar(wp, T("nat_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
#endif
}

//Steve add for hp
static void FIREWALL_ACTIVE(webs_t wp, char_t *path, char_t *query)
{
	char *firewallE;
	int natflag=0;
    char_t *submitUrl;
    char rule[256]={0};
    
	firewallE = websGetVar(wp, T("FIREWALLEnabled"), T(""));
		
	doSystem("/etc/rc.common /etc/init.d/network-acl stop fw");
	
	if(atoi(firewallE)==1){
		ezplib_replace_attr("fw_rule", 0, "state_enable", "1");
		ezplib_replace_attr("fw_rule", 0, "syn_enable", "1");
	}else{
		ezplib_replace_attr("fw_rule", 0, "state_enable", "0");
		ezplib_replace_attr("fw_rule", 0, "syn_enable", "0");
	}
	
	//sprintf(rule, "1^%s^%s^%s^0^%s", firewallE, firewallE, firewallE, firewallE);
	//nvram_set("fw_rule", rule);	

    doSystem("/etc/rc.common /etc/init.d/network-acl start fw");
     
	printf("nvram_commit\n");
	nvram_commit();
    
    #if 1//Steve add 2009/03/18		
		setWebMessage(0, NULL);
	#endif
	
    submitUrl = websGetVar(wp, T("firewall_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
}
//Kissth add for opendns
static void opendns_set(webs_t wp, char_t *path, char_t *query)
{
     char_t *service_enable, *redircet_enable;
     char_t *username, *passwd, *opendns_label;

     service_enable = websGetVar(wp, T("opendns_service_enable"), T("0"));  
     redircet_enable = websGetVar(wp, T("opendns_redircet_enable"), T("0"));  
     username = websGetVar(wp, T("opendns_username"), T(""));
     passwd = websGetVar(wp, T("opendns_password"), T(""));
     opendns_label = websGetVar(wp, T("opendns_lable"), T(""));
	 
     system("/etc/init.d/opendns stop");
     ezplib_replace_attr("wan_opendns_rule", 0, "enable", service_enable);
     if( atoi(service_enable) == 1 ) {
          ezplib_replace_attr("wan_opendns_rule", 0, "redirect", redircet_enable);
          ezplib_replace_attr("wan_opendns_rule", 0, "username", username);
          ezplib_replace_attr("wan_opendns_rule", 0, "passwd", passwd);
          ezplib_replace_attr("wan_opendns_rule", 0, "label", opendns_label);
     }
     nvram_commit();
     system("/etc/init.d/opendns start");
     websRedirect(wp, "local/advance/opendns.asp");
}
/*//william add for ipsec START*/ 

int valid_ipsec(webs_t wp)
{
    int i, nrule;
    char tmp[LONG_BUF_LEN];
    char *val, *mode, *ike_keymode;

    /* Enable of the whole forwarding mechanism. */
/*
    snprintf(tmp, sizeof(tmp), "ipsec_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &ipsec_variables[IPSEC_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {
        return TRUE;
    }
	*/
        /* Enable */
        snprintf(tmp, sizeof(tmp), "enable");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_ENABLE]) == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "name");
        val = websGetVar(wp, tmp, "");
        if (valid_strict_name(wp, val, &ipsec_variables[IPSEC_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* VPN Mode */
        snprintf(tmp, sizeof(tmp), "mode");
        mode = websGetVar(wp, tmp, "");
        if (valid_choice(wp, mode, &ipsec_variables[IPSEC_RULE_MODE])  == FALSE) {

            return FALSE;
        }

        /* L2TP Enable */
        if (!strcmp(mode, "roadwarrior")) {
            snprintf(tmp, sizeof(tmp), "l2tp");
            val = websGetVar(wp, tmp, "");
            if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_L2TP]) == FALSE) {

                return FALSE;
            }
        }

        /* Local WAN Interface */
        snprintf(tmp, sizeof(tmp), "local_extif");
        val = websGetVar(wp, tmp, "");
        if (valid_if(wp, val, &ipsec_variables[IPSEC_RULE_LOCAL_EXTIF]) == FALSE) {

            return FALSE;
        }

        /* Local Internal IP Address */
        snprintf(tmp, sizeof(tmp), "local_inipaddr");
        val = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, val, &ipsec_variables[IPSEC_RULE_LOCAL_INIPADDR]) == FALSE) {

            return FALSE;
        }

        /* Local Internal Netmask */
        snprintf(tmp, sizeof(tmp), "local_netmask");
        val = websGetVar(wp, tmp, "");
        if (valid_netmask(wp, val, 
                    &ipsec_variables[IPSEC_RULE_LOCAL_NETMASK]) == FALSE) {

            return FALSE;
        }

        /* Remote External IP Address */
        snprintf(tmp, sizeof(tmp), "remote_gateway");
        val = websGetVar(wp, tmp, "");
        if (valid_strict_name(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_GATEWAY]) == FALSE) {

            return FALSE;
        }

        /* Remote Internal IP Address */
        snprintf(tmp, sizeof(tmp), "remote_inipaddr");
        val = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_INIPADDR]) 
                        == FALSE) {

            return FALSE;
        }
        /* Remote Netmask */
        snprintf(tmp, sizeof(tmp), "remote_netmask");
        val = websGetVar(wp, tmp, "");
        if (valid_netmask(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_NETMASK]) == FALSE) {

            return FALSE;
        }

        /* Connection Initiation */
        snprintf(tmp, sizeof(tmp), "conn_init");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_CONN_INIT]) == FALSE) {

            return FALSE;
        }

        /* Key Type */
        snprintf(tmp, sizeof(tmp), "ike_keymode");
        ike_keymode = websGetVar(wp, tmp, "");
        if (valid_choice(wp, ike_keymode, 
                    &ipsec_variables[IPSEC_RULE_IKE_KEYMODE]) == FALSE) {

            return FALSE;
        }

        /* PSK */
        if (!strcmp(ike_keymode, "psk")) {
            snprintf(tmp, sizeof(tmp), "psk");
            val = websGetVar(wp, tmp, "");
            if (valid_length_range(wp, val, 
                        &ipsec_variables[IPSEC_RULE_PSK]) == FALSE) {

                return FALSE;
            }
        }

        /* RSA */
        if (!strcmp(ike_keymode, "rsa")) {
            snprintf(tmp, sizeof(tmp), "rsa");
            val = websGetVar(wp, tmp, "");
            if (valid_length_range(wp, val, 
                        &ipsec_variables[IPSEC_RULE_RSA]) == FALSE) {

                return FALSE;
            }
        }

        /* CA */
        /* TODO */

        /* Advanced Options */
        snprintf(tmp, sizeof(tmp), "adv");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_ADV]) 
                == FALSE) {

            return FALSE;
        }

        /* Phase1 Mode */
        snprintf(tmp, sizeof(tmp), "phase1_mode");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_MODE]) == FALSE) {

            return FALSE;
        }

        /* Phase1 ID */
#if EZPACKET
        snprintf(tmp, sizeof(tmp), "phase1_id");
        val = websGetVar(wp, tmp, "");
        /* TODO: to ensure the ID format. */
        if (valid_name(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_ID]) == FALSE) {

            return FALSE;
        }
#endif

        /* Phase1 Lifetime */
        snprintf(tmp, sizeof(tmp), "phase1_lifetime");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_LIFETIME]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Lifetime */
        snprintf(tmp, sizeof(tmp), "phase2_lifetime");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_LIFETIME]) == FALSE) {

            return FALSE;
        }
        /* Phase1 Auth */
        snprintf(tmp, sizeof(tmp), "phase1_auth");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_AUTH]) == FALSE) {

            return FALSE;
        }

        /* Phase1 Encrypt */
        snprintf(tmp, sizeof(tmp), "phase1_encrypt");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_ENCRYPT]) == FALSE) {

            return FALSE;
        }

        /* Phase1 Group */
        snprintf(tmp, sizeof(tmp), "phase1_group");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_GROUP]) == FALSE) {

            return FALSE;
        }


        /* Phase2 Auth */
        snprintf(tmp, sizeof(tmp), "phase2_auth");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_AUTH]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Encrypt */
        snprintf(tmp, sizeof(tmp), "phase2_encrypt");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_ENCRYPT]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Group */
        snprintf(tmp, sizeof(tmp), "phase2_group");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_GROUP]) == FALSE) {

            return FALSE;
        }
        /* DPD Enable */
        snprintf(tmp, sizeof(tmp), "dpd_enable");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_ENABLE]) == FALSE) {

            return FALSE;
        }
        /* DPD Interval */
        snprintf(tmp, sizeof(tmp), "dpd_interval");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_INTERVAL]) == FALSE) {

            return FALSE;
        }
        /* DPD Timeout */
        snprintf(tmp, sizeof(tmp), "dpd_timeout");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_TIMEOUT]) == FALSE) {

            return FALSE;
        }
    //}
    return TRUE;
}

#if 1
int 
save_ipsec(webs_t wp)
{
    int i, nrule, change = 0;
    int64_t map = 0;
    char rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char *enable, *rule_num, *rule_enable, *name, *adv, *mode, 
         *l2tp, *local_extif, *local_inipaddr, *local_netmask, *remote_gw, 
         *remote_inipaddr, *remote_netmask, *conn_init, *ph1_mode, 
         *ph1_auth, *ph1_encrypt, *ph1_id, *ph1_group, *ph2_group,
         *dpd_enable, *dpd_interval, *dpd_timeout,
         *ph2_auth, *ph2_encrypt, *ph1_lifetime, *ph2_lifetime, 
         *ike_keymode, *psk, *rsa, *ca, *remote_id_type, *remote_id_value,
         *local_id_type, *local_id_value;
    char *rule_set = "ipsec_rule";
    char_t *submitUrl;

    submitUrl = websGetVar(wp, T("ipsec_gen_url"), T(""));   // hidden page
	if (strcmp(submitUrl, "")) {
		/* Enable of the whole forwarding mechanism. */
		snprintf(tmp, sizeof(tmp), "ipsec_enable");
		enable = websGetVar(wp, tmp, "");
		if (strcmp(enable, nvram_safe_get("ipsec_enable"))) {
			//config_preaction(&map, v, s, "NUM=0", "");
			nvram_set("ipsec_enable", enable);
			change = 1;
		}
	} else {

		nvram_set(rule_set, "");

		snprintf(tmp, sizeof(tmp), "ipsec_rule_num");
		rule_num = websGetVar(wp, tmp, "0");
		nrule = atoi(rule_num);

		snprintf(tmp, sizeof(tmp), "%d", nrule);
		if (nrule != atoi(nvram_safe_get("ipsec_rule_num"))) {
			/* In case the situation that the last rule is deleted but others are
			 * kept the same and the check flow will treat it as 'nothing
			 * changed'.
			 */
			//config_preaction(&map, v, s, "NUM=0", "");
			nvram_set("ipsec_rule_num", tmp);
			change = 1;
		}

		for (i = 0; i < nrule; i++) {
			/* Enable */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_enable", i);
			rule_enable = websGetVar(wp, tmp, "");

			/* Name */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_name", i);
			name = websGetVar(wp, tmp, "");

			/* VPN Mode */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_mode", i);
			mode = websGetVar(wp, tmp, "");

			/* L2TP Enable */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_l2tp", i);
			l2tp = websGetVar(wp, tmp, "");

			/* Local WAN Interface */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_extif", i);
			local_extif = websGetVar(wp, tmp, "");

			/* Local Internal IP Address */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_inipaddr", i);
			local_inipaddr = websGetVar(wp, tmp, "");

			/* Local Internal Netmask */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_netmask", i);
			local_netmask = websGetVar(wp, tmp, "");

			/* Remote External IP Address */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_gateway", i);
			remote_gw = websGetVar(wp, tmp, "");

			/* Remote Internal IP Address */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_inipaddr", i);
			remote_inipaddr = websGetVar(wp, tmp, "");

			/* Remote Internal Netmask */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_netmask", i);
			remote_netmask = websGetVar(wp, tmp, "");

			/* Connection Initiation */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_conn_init", i);
			conn_init = websGetVar(wp, tmp, "");

			/* Key Type */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_ike_keymode", i);
			ike_keymode = websGetVar(wp, tmp, "");

			/* PSK */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_psk", i);
			psk = websGetVar(wp, tmp, "");

			/* Remote ID Type */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_id_type", i);
			remote_id_type = websGetVar(wp, tmp, "");

			/* Remote ID Value */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_id_value", i);
			remote_id_value = websGetVar(wp, tmp, "");

			/* Local ID Type */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_id_type", i);
			local_id_type = websGetVar(wp, tmp, "");

			/* Remote ID Value */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_id_value", i);
			local_id_value = websGetVar(wp, tmp, "");

			/* RSA */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_rsa", i);
			rsa = websGetVar(wp, tmp, "");

			/* CA */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_ca", i);
			ca = websGetVar(wp, tmp, "");

			/* Advanced Options*/
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_adv", i);
			adv = websGetVar(wp, tmp, "");

			/* Phase 1 Mode */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_mode", i);
			ph1_mode = websGetVar(wp, tmp, "");

			/* Phase 1 ID */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_id", i);
			ph1_id = websGetVar(wp, tmp, "");

			/* Phase 1 Lifetime */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_lifetime", i);
			ph1_lifetime = websGetVar(wp, tmp, "");

			/* Phase 2 Lifetime */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_lifetime", i);
			ph2_lifetime = websGetVar(wp, tmp, "");

			/* Phase 1 Authentication */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_auth", i);
			ph1_auth = websGetVar(wp, tmp, "");

			/* Phase 1 Encryption */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_encrypt", i);
			ph1_encrypt = websGetVar(wp, tmp, "");

			/* Phase 1 Group */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_group", i);
			ph1_group = websGetVar(wp, tmp, "");

			/* Phase 2 Authentication */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_auth", i);
			ph2_auth = websGetVar(wp, tmp, "");

			/* Phase 2 Encryption */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_encrypt", i);
			ph2_encrypt = websGetVar(wp, tmp, "");

			/* Phase 2 Group */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_group", i);
			ph2_group = websGetVar(wp, tmp, "");

			/* DPD Enable */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_enable", i);
			dpd_enable = websGetVar(wp, tmp, "");

			/* DPD Interval */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_interval", i);
			dpd_interval = websGetVar(wp, tmp, "");

			/* DPD Timeout */
			snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_timeout", i);
			dpd_timeout = websGetVar(wp, tmp, "");

			snprintf(rule, sizeof(rule),
					"%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s"
					"^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
					name, rule_enable, mode, l2tp, local_extif, 
					local_inipaddr, local_netmask, remote_gw, 
					remote_inipaddr, remote_netmask, conn_init, 
					ike_keymode, psk, rsa, ca, adv, ph1_mode, ph1_id, 
					ph1_lifetime, ph2_lifetime, ph1_auth, 
					ph1_encrypt, ph1_group, ph2_auth, ph2_encrypt, 
					ph2_group, dpd_enable, dpd_interval, dpd_timeout,remote_id_type,
					remote_id_value,local_id_type,local_id_value);

			if (ezplib_get_rule(rule_set, i, old_rule, LONG_BUF_LEN) < 0) {
				/* i-th rule doesn't exist */
				//config_preaction(&map, v, s, "NUM=0", "");
				system("/etc/init.d/ipsec stop");
				ezplib_append_rule(rule_set, rule);
				change = 1;
			} else {
				if (strcmp(rule, old_rule)) {
					//config_preaction(&map, v, s, "NUM=0", "");
					system("/etc/init.d/ipsec stop");
					ezplib_replace_rule(rule_set, i, rule);
					change = 1;
				}
			}
		}
	}

	if (change) {
		//config_postaction(map, s, "NUM=0", "");
		nvram_commit();
		system("/etc/init.d/ipsec restart");
	}

	return change;
}

static void vpn_ipsec2(webs_t wp, char_t *path, char_t *query)
{
	int ret = -1;
	char_t *submitUrl;

	ret = save_ipsec(wp);

	printf("save ipsec return value %d\n", ret);

	setWebMessage(0, NULL);

	submitUrl = websGetVar(wp, T("ipsec_gen_url"), T(""));   // hidden page
	if (!strcmp(submitUrl, "")) {
		submitUrl = websGetVar(wp, T("ipsec_url"), T(""));   // hidden page
	}
	websRedirect(wp, submitUrl);
}
#endif

int
ej_get_ipsec_status(int eid, webs_t wp, int argc, char_t ** argv)
{ 
	    system("/usr/bin/vpnstatus");
		return 1;
}

int isMaximum(int eid, webs_t wp, int argc, char_t ** argv)
{ 
     int rule_nums = atoi(nvram_safe_get("ipsec_rule_num"));
	 int max_num = atoi(nvram_safe_get("ipsec_rule_max"));
     if(rule_nums == max_num)
	      return websWrite(wp, "1");  //tables is full
	 else
	      return websWrite(wp, "0");
}

int
showIpsecVpnRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[SHORT_BUF_LEN];
	char tmp_active[SHORT_BUF_LEN];
	char tmp_name[SHORT_BUF_LEN];
	char tmp_ip[SHORT_BUF_LEN];
	int i;
	int rule_num=0;

    rule_num=atoi(nvram_safe_get("ipsec_rule_num"));
    //printf("\n\n   rule_num=%d -> 111\n", rule_num);        
	
	for(i=0;i<rule_num;i++){

		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td valign=\"top\"><center><span class=\"table_left\">%d</span></center></td>\n"), i+1);

         // Name
		ezplib_get_attr_val("ipsec_rule", i, "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("name=%s\t",tmp_name);
		if(strlen(tmp_name))
			websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center></td>\n"), tmp_name);
		else
			websWrite(wp, T("<td><center><span class=\"table_font\">- </span></center></td>\n"));
		
		//Active     
        ezplib_get_attr_val("ipsec_rule", i, "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("enable=%s\t",tmp_active);
		if( !strcmp(tmp_active, "1"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>\n"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>\n"));
#if 0 //SMB router just has one wan.
        //Extern interface 
        ezplib_get_attr_val("ipsec_rule", i, "local_extif", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extif=%s\t",tmp);
		websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center>\n"), tmp);
#endif	
        //Remote gateway 
        ezplib_get_attr_val("ipsec_rule", i, "remote_gateway", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("proto=%s\t",tmp);
		websWrite(wp, T("<td><center><span class=\"table_font\">%s</span></center>\n"), tmp);

        //Remote iner ip address 
        ezplib_get_attr_val("ipsec_rule", i, "remote_inipaddr", tmp_ip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_start=%s\t",tmp_port);

		//Remote iner ip netmask 
        ezplib_get_attr_val("ipsec_rule", i, "remote_netmask", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_end=%s\t",tmp);
		websWrite(wp, T("<td><center><span class=\"table_font\">%s / %s</span></center>\n"), tmp_ip, tmp);
        
        ezplib_get_attr_val("ipsec_status_rule", i, "phase1", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_start=%s\t",tmp);
        if( !strcmp(tmp, "None"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>\n"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>\n"));
		
        ezplib_get_attr_val("ipsec_status_rule", i, "phase2", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_end=%s\n",tmp);
		if( !strcmp(tmp, "None"))
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>\n"));
		else
			websWrite(wp, T("<td><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>\n"));
		
		//Modify
		websWrite(wp, T("<td style=\"white-space:nowrap\" ><center><span class=\"table_right\">\n"));
		websWrite(wp, T("<a href=\"javascript: doEditRule(%d);\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatEImage%d','','images/i_edit_on.gif',1)\">"), i, i+1);
		websWrite(wp, T("<img src=\"images/i_edit.gif\" name=\"fnatEImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatEImage%d\"  title=\"Edit\"/></a>\n"),i+1);
		websWrite(wp, T("&nbsp;&nbsp;<a href=\"javascript: doDeleteRule(%d);\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatDImage%d','','images/i_delete_on.gif',1)\">"), i, i+1);
		websWrite(wp, T("<img src=\"images/i_delete.gif\" name=\"fnatDImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatDImage%d\"  title=\"Delete\"/>\n"),i+1);
		websWrite(wp, T("</a></span></center></td>\n"));
		websWrite(wp, T("</tr>\n"));  
    }
	return 0;	
}

int
valid_pptpd(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int nrule;
    int i;

    snprintf(tmp, sizeof(tmp), "pptpd_enable");
    if(nvram_get("pptpd_user_rule_max")) {
        pptpd_variables[PPTPD_LOCALIP_RANGE].argv[1] = nvram_get("pptpd_user_rule_max");
        pptpd_variables[PPTPD_REMOTEIP_RANGE].argv[1] = nvram_get("pptpd_user_rule_max");
    }
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* If hardware NAT is enabled, we can not start PPTPD. */
    ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                            sizeof(tmp), EZPLIB_USE_CLI);
    if ((*val == '1') && !strcmp(tmp, "1")) {
        websDebugWrite(wp, "%s.<br> %s.<br>",
                lang_translate("Start VPN/PPTP failed"),
                lang_translate("Need to turn off Hardware Accelerator first"));
        return FALSE;
    }
    snprintf(tmp, sizeof(tmp), "pptpd_mtu");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &pptpd_variables[PPTPD_MTU]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_localip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_LOCALIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_REMOTEIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip_range");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &pptpd_variables[PPTPD_REMOTEIP_RANGE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_dns_auto_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_DNS_AUTO_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {
        /* If not enable Auto DNS */
        snprintf(tmp, sizeof(tmp), "pptpd_dns");
        val = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_DNS]) == FALSE) {
            return FALSE;
        }
    }

    snprintf(tmp, sizeof(tmp), "pptpd_chap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_CHAP_ENABLE2]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mschap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MSCHAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mschapv2_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MSCHAPV2_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_eap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_EAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_pap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_PAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mppe128_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MPPE128_ENABLE]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_proxyarp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_PROXYARP_ENABLE]) 
            == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "pptpd_nat_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_NAT_ENABLE]) == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "pptpd_user_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_username", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &pptpd_variables[PPTPD_USERNAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_passwd", i);
        val = websGetVar(wp, tmp, "");
        /* The password is shown as a public key for the administrator. */
        if (valid_name(wp, val, &pptpd_variables[PPTPD_PASSWD]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int 
save_pptpd(webs_t wp)
{
    char tmp[TMP_LEN], buf[TMP_LEN];    
    char *enable, *mtu, *localip, *localip_range, *remoteip, *remoteip_range,
         *dns_auto_enable, *dns, *chap_enable, *mschap_enable, 
         *mschapv2_enable, *eap_enable, *pap_enable, *mppe128_enable, 
         *proxyarp_enable, *nat_enable, *user_rule_num, *username, *passwd,
         *provider, *ipaddr, *name, *redialperiod;
	char_t *submitUrl;
    char *nv_rule1 = "pptpd_rule";
    char *nv_rule2 = "pptpd_user_rule",
         *nv_rule2_num = "pptpd_user_rule_num",
         *nv_rule2_max = "pptpd_user_rule_max";
    int nrule, len,change = 0, max;
    int64_t map = 0;
    int i;

    submitUrl = websGetVar(wp, T("pptpd_gen_url"), T(""));   // hidden page
	if ( strcmp(submitUrl, "") ) {

		snprintf(tmp, sizeof(tmp), "pptpd_enable");
		enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_name");
		name = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_redialperiod");
		redialperiod = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_mtu");
		mtu = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_localip");
		localip = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_remoteip");
		remoteip = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_remoteip_range");
		remoteip_range = websGetVar(wp, tmp, "");
		localip_range = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_dns_auto_enable");
		dns_auto_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_dns");
		dns = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_chap_enable");
		chap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_mschap_enable");
		mschap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_mschapv2_enable");
		mschapv2_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_eap_enable");
		eap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_pap_enable");
		pap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_mppe128_enable");
		mppe128_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "pptpd_proxyarp_enable");
		proxyarp_enable = websGetVar(wp, tmp, "");
    
		snprintf(tmp, sizeof(tmp), "pptpd_nat_enable");
		nat_enable = websGetVar(wp, tmp, "");

		/* mtu = mru in this case */
		len = snprintf(buf, TMP_LEN, 
				"%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
				enable, name, redialperiod, mtu, mtu, localip, localip_range,
				remoteip, remoteip_range, dns_auto_enable, dns,
				chap_enable, mschap_enable, mschapv2_enable, eap_enable,
				pap_enable, mppe128_enable, proxyarp_enable, nat_enable);
		if (len >= TMP_LEN) {
			return 0;
		}

		ezplib_get_rule(nv_rule1, 0, tmp, TMP_LEN);
		if (strcmp(buf, tmp)) {
			//config_preaction(&map, v, s, "NUM=0", "");
			system("/etc/init.d/pptpd stop");
			nvram_set(nv_rule1, buf);
			change = 1;
		}

	} else {

		snprintf(tmp, sizeof(tmp), nv_rule2_num);
		user_rule_num = websGetVar(wp, tmp, "0");
		nrule = atoi(user_rule_num);
		max = atoi(nvram_safe_get(nv_rule2_max));
		nrule = (nrule >= max) ? max : nrule;

		nvram_set(nv_rule2, "");

		if (nrule != atoi(nvram_safe_get(nv_rule2_num))) {
			/* In case the situation that the last rule is deleted but others are
			 * kept the same and the check flow will treat it as 'nothing
			 * changed'.
			 */
			//config_preaction(&map, v, s, "NUM=0", "");
			system("/etc/init.d/pptpd stop");
			snprintf(tmp, sizeof(tmp), "%d", nrule);
			nvram_set(nv_rule2_num, tmp);
			change = 1;
		}

		for (i = 0; i < nrule; i++) {
			snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_enable", i);
			enable = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_username", i);
			username = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_provider", i);
			provider = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_passwd", i);
			passwd= websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_ipaddr", i);
			ipaddr = websGetVar(wp, tmp, "");

			len = snprintf(buf, TMP_LEN, "%s^%s^%s^%s^%s", 
					enable, "", provider, "", ipaddr);
			if (len >= TMP_LEN) {
				return 0;
			}
			if (ezplib_get_rule(nv_rule2, i, tmp, TMP_LEN) < 0) {
				/* i-th rule doesn't exist */
				//config_preaction(&map, v, s, "NUM=0", "");
				system("/etc/init.d/pptpd stop");
				ezplib_append_rule(nv_rule2, buf);
				ezplib_replace_attr("pptpd_user_rule", i, "username", username);
				ezplib_replace_attr("pptpd_user_rule", i, "passwd", passwd);
				change = 1;
			} else if (strcmp(buf, tmp)) {
				//config_preaction(&map, v, s, "NUM=0", "");
				system("/etc/init.d/pptpd stop");
				ezplib_replace_rule(nv_rule2, i, buf);
				ezplib_replace_attr("pptpd_user_rule", i, "username", username);
				ezplib_replace_attr("pptpd_user_rule", i, "passwd", passwd);
				change = 1;
			}
		}
	}
	if (change) {
		snprintf(tmp, TMP_LEN, "NUM=%d", i);
		/* Not defined the postaction yet. */
		//config_postaction(map, s, tmp, "");
		nvram_commit();
		system("/etc/init.d/pptpd start");
	}

	return change;
}

/* william, for ipsec vpn --> ipsec vpn Liteon  END */

//start william add for pptpd
static void vpn_pptpd(webs_t wp, char_t *path, char_t *query)
{
	int ret = -1;
	char_t *submitUrl;

	ret = save_pptpd(wp);

	printf("save pptpd return value %d\n", ret);

	setWebMessage(0, NULL);
	
    submitUrl = websGetVar(wp, T("pptpd_gen_url"), T(""));   // hidden page
	if (!strcmp(submitUrl, "")) {
		submitUrl = websGetVar(wp, T("pptpd_url"), T(""));   // hidden page
	}
	websRedirect(wp, submitUrl);
}
//end william add for pptpd

int save_l2tpd(webs_t wp){
    char tmp[TMP_LEN], buf[TMP_LEN];    
    char *enable, *mtu, *localip, *localip_range, *remoteip, *remoteip_range,
         *dns_auto_enable, *dns, *chap_enable, *mschap_enable, 
         *mschapv2_enable, *eap_enable, *pap_enable, *l2tpOIPsec_enable, *psk_value,
         *proxyarp_enable, *nat_enable, *user_rule_num, *username, *passwd,
         *provider, *ipaddr, *name, *redialperiod;
	char_t *submitUrl;
    char *nv_rule1 = "l2tpd_rule";
    char *nv_rule2 = "l2tpd_user_rule",
         *nv_rule2_num = "l2tpd_user_rule_num",
         *nv_rule2_max = "l2tpd_user_rule_max";
    int nrule, len,change = 0, max;
    int64_t map = 0;
    int i;

    submitUrl = websGetVar(wp, T("l2tpd_gen_url"), T(""));   // hidden page
	if ( strcmp(submitUrl, "") ) {

		snprintf(tmp, sizeof(tmp), "l2tpd_enable");
		enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_name");
		name = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_redialperiod");
		redialperiod = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_mtu");
		mtu = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_localip");
		localip = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_remoteip");
		remoteip = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_remoteip_range");
		remoteip_range = websGetVar(wp, tmp, "");
		localip_range = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_dns_auto_enable");
		dns_auto_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_dns");
		dns = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_chap_enable");
		chap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_mschap_enable");
		mschap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_mschapv2_enable");
		mschapv2_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_eap_enable");
		eap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_pap_enable");
		pap_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_l2tpOIPsec_enable");
		l2tpOIPsec_enable = websGetVar(wp, tmp, "");

		snprintf(tmp, sizeof(tmp), "l2tpd_psk");
		psk_value = websGetVar(wp, tmp, "");
		
		snprintf(tmp, sizeof(tmp), "l2tpd_proxyarp_enable");
		proxyarp_enable = websGetVar(wp, tmp, "");
    
		snprintf(tmp, sizeof(tmp), "l2tpd_nat_enable");
		nat_enable = websGetVar(wp, tmp, "");

		/* mtu = mru in this case */
		len = snprintf(buf, TMP_LEN, 
				"%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
				enable, name, redialperiod, mtu, mtu, localip, localip_range,
				remoteip, remoteip_range, dns_auto_enable, dns,
				chap_enable, mschap_enable, mschapv2_enable, eap_enable,
				pap_enable, proxyarp_enable, nat_enable,l2tpOIPsec_enable, psk_value);
		if (len >= TMP_LEN) {
			return 0;
		}

		ezplib_get_rule(nv_rule1, 0, tmp, TMP_LEN);
		if (strcmp(buf, tmp)) {
			system("/etc/init.d/xl2tpd stop");
			nvram_set(nv_rule1, buf);
			change = 1;
		}

	} else {

		snprintf(tmp, sizeof(tmp), nv_rule2_num);
		user_rule_num = websGetVar(wp, tmp, "0");
		nrule = atoi(user_rule_num);
		max = atoi(nvram_safe_get(nv_rule2_max));
		nrule = (nrule >= max) ? max : nrule;

		nvram_set(nv_rule2, "");

		if (nrule != atoi(nvram_safe_get(nv_rule2_num))) {
			/* In case the situation that the last rule is deleted but others are
			 * kept the same and the check flow will treat it as 'nothing
			 * changed'.
			 */
			system("/etc/init.d/xl2tpd stop");
			snprintf(tmp, sizeof(tmp), "%d", nrule);
			nvram_set(nv_rule2_num, tmp);
			change = 1;
		}

		for (i = 0; i < nrule; i++) {
			snprintf(tmp, sizeof(tmp), "l2tpd_user_rule%d_enable", i);
			enable = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "l2tpd_user_rule%d_username", i);
			username = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "l2tpd_user_rule%d_provider", i);
			provider = websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "l2tpd_user_rule%d_passwd", i);
			passwd= websGetVar(wp, tmp, "");

			snprintf(tmp, sizeof(tmp), "l2tpd_user_rule%d_ipaddr", i);
			ipaddr = websGetVar(wp, tmp, "");

			len = snprintf(buf, TMP_LEN, "%s^%s^%s^%s^%s", 
					enable, "", provider, "", ipaddr);
			if (len >= TMP_LEN) {
				return 0;
			}
			if (ezplib_get_rule(nv_rule2, i, tmp, TMP_LEN) < 0) {
				/* i-th rule doesn't exist */
				system("/etc/init.d/xl2tpd stop");
				ezplib_append_rule(nv_rule2, buf);
				ezplib_replace_attr("l2tpd_user_rule", i, "username", username);
				ezplib_replace_attr("l2tpd_user_rule", i, "passwd", passwd);
				change = 1;
			} else if (strcmp(buf, tmp)) {
				system("/etc/init.d/xl2tpd stop");
				ezplib_replace_rule(nv_rule2, i, buf);
				ezplib_replace_attr("l2tpd_user_rule", i, "username", username);
				ezplib_replace_attr("l2tpd_user_rule", i, "passwd", passwd);
				change = 1;
			}
		}
	}
	if (change) {
		snprintf(tmp, TMP_LEN, "NUM=%d", i);
		/* Not defined the postaction yet. */
		nvram_commit();
		system("/etc/init.d/xl2tpd start");
	}

	return change;
}

/*Support l2tp setting*/
static void vpn_l2tpd(webs_t wp, char_t *path, char_t *query)
{
	int ret = -1;
	char_t *submitUrl;

	ret = save_l2tpd(wp);

	printf("save l2tpd return value %d\n", ret);

	setWebMessage(0, NULL);
	
    submitUrl = websGetVar(wp, T("l2tpd_gen_url"), T(""));   // hidden page
	if (!strcmp(submitUrl, "")) {
		submitUrl = websGetVar(wp, T("l2tpd_url"), T(""));   // hidden page
	}
	websRedirect(wp, submitUrl);
}

//start william add for vpn passthrough
static void save_vpn_passthrough(webs_t wp, char_t *path, char_t *query)
{
    char tmp[TMP_LEN], buf[TMP_LEN];    
    char *enable, *ipsec_enable, *pptp_enable, *l2tp_enable,*pppoe_enable;
	char_t *submitUrl;
    char *nv_rule = "smb_vpn_passthrough_rule";
    int nrule, len,change = 0;
    int64_t map = 0;
    int i;

    submitUrl = websGetVar(wp, T("vpn_pass_url"), T(""));   // hidden page
	if ( strcmp(submitUrl, "") ) {

		snprintf(tmp, sizeof(tmp), "ipsec_enable");
		ipsec_enable = websGetVar(wp, tmp, "0");

		snprintf(tmp, sizeof(tmp), "pptp_enable");
		pptp_enable = websGetVar(wp, tmp, "0");

		snprintf(tmp, sizeof(tmp), "l2tp_enable");
		l2tp_enable = websGetVar(wp, tmp, "0");
		snprintf(tmp, sizeof(tmp), "pppoe_enable");
		pppoe_enable = websGetVar(wp, tmp, "0");

		len = snprintf(buf, TMP_LEN, 
				"%s^%s^%s^%s",
				ipsec_enable, pptp_enable, l2tp_enable,pppoe_enable);
		if (len >= TMP_LEN) {
			return 0;
		}

		ezplib_get_rule(nv_rule, 0, tmp, TMP_LEN);
		if (strcmp(buf, tmp)) {
			system("/etc/init.d/smb_vpn_passthrough stop");
			nvram_set(nv_rule, buf);
			change = 1;
		}

	}
	if (change) {
		nvram_commit();
		system("/etc/init.d/smb_vpn_passthrough start");
	}

	setWebMessage(0, NULL);
	
	websRedirect(wp, submitUrl);
}
//end william add for vpn passthrough
/*ftp passthrough add by wind 2013-08-19*/
static void save_ftp_passthrough(webs_t wp, char_t *path, char_t *query)
{
	char tmp[TMP_LEN] = {0}; 
    char buf[TMP_LEN] = {0}; 
	char *ftp_enable = NULL;
	char_t *submitUrl = NULL;
	char *nv_rule = "smb_ftp_passthrough_rule";
	submitUrl = websGetVar(wp, T("ftp_pass_url"), T(""));   // hidden page
	if ( strcmp(submitUrl, "") ){
		snprintf(tmp, sizeof(tmp), "ftp_enable");
		ftp_enable = websGetVar(wp, tmp, "0");
		strncpy(buf,ftp_enable,sizeof(buf));
		printf(">>>>>>>>>ftp_enable:%s",ftp_enable);
		nvram_set(nv_rule, buf);
		nvram_commit();
		system("/etc/init.d/smb_ftp_passthrough start");
	}    
	setWebMessage(0, NULL);
	websRedirect(wp, submitUrl);
}

/*h232 passthrough add by wind 2013-08-19*/
static void save_h323_passthrough(webs_t wp, char_t *path, char_t *query)
{
	char tmp[TMP_LEN] = {0}; 
	char buf[TMP_LEN] = {0}; 
	char *h323_enable = NULL;
	char_t *submitUrl = NULL;
	char *nv_rule = "smb_h323_passthrough_rule";
	submitUrl = websGetVar(wp, T("h323_pass_url"), T(""));   // hidden page
	if ( strcmp(submitUrl, "") ){
		snprintf(tmp, sizeof(tmp), "h323_enable");
		h323_enable = websGetVar(wp, tmp, "0");
		strncpy(buf,h323_enable,sizeof(buf));
		printf(">>>>>>>>>h323_enable:%s",h323_enable);
		nvram_set(nv_rule, buf);
		nvram_commit();
		system("/etc/init.d/smb_h323_passthrough start");
	}    
	setWebMessage(0, NULL);
	websRedirect(wp, submitUrl);
}


char *firewallE;
//Steve add for hp
static void SPI_ACTIVE(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char *spiE;
	int natflag=0;
    char_t *submitUrl;
    
    //printf("Steve:%s\n",__FUNCTION__);
    
	spiE = websGetVar(wp, T("SPIFWEnabled"), T(""));
		
	//printf("spiE: %s \n",spiE);	
	nvram_bufset(RT2860_NVRAM, "SPIFWEnabled",   atoi(spiE)   ? "1" : "0" );
	
	nvram_commit(RT2860_NVRAM); 
	
	iptablesWANServiceRun();
    
	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();
    
    submitUrl = websGetVar(wp, T("spi_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
	#endif
}



static void websSysFirewall(webs_t wp, char_t *path, char_t *query)
{
	#if 0
	char *rmE = websGetVar(wp, T("remoteManagementEnabled"), T(""));
	char *wpfE = websGetVar(wp, T("pingFrmWANFilterEnabled"), T(""));
	char *spifw = websGetVar(wp, T("spiFWEnabled"), T("1"));

	// someone use malform page.....
	if(!rmE || !strlen(rmE))
		return;
	if(!wpfE || !strlen(wpfE))
		return;
	if(!spifw || !strlen(spifw))
		return;

	// TODO: make a new chain instead of flushing the INPUT chain
	doSystem("iptables -t filter -F INPUT");

	if(atoi(rmE) == 0){		// disable
		nvram_bufset(RT2860_NVRAM, "RemoteManagement", "0");
	}else{					// enable
		nvram_bufset(RT2860_NVRAM, "RemoteManagement", "1");
	}

	if(atoi(wpfE) == 0){		// disable
		nvram_bufset(RT2860_NVRAM, "WANPingFilter", "0");
	}else{					// enable
		nvram_bufset(RT2860_NVRAM, "WANPingFilter", "1");
		doSystem("iptables -t filter -A INPUT -i %s -p icmp -j DROP", getWanIfNamePPP());
	}

	if(atoi(spifw) == 0){		// disable
		nvram_bufset(RT2860_NVRAM, "SPIFWEnabled", "0");
	}else{					// enable
		nvram_bufset(RT2860_NVRAM, "SPIFWEnabled", "1");
	}
	nvram_commit(RT2860_NVRAM);

	iptablesRemoteManagementRun();

	iptablesIPPortFilterFlush();
	iptablesIPPortFilterRun();

	websHeader(wp);
	websWrite(wp, T("RemoteManage: %s<br>\n"), rmE);
	websWrite(wp, T("WANPingFilter: %s<br>\n"), wpfE);
	websWrite(wp, T("SPIFWEnabled: %s<br>\n"), spifw);
    websFooter(wp);
    websDone(wp, 200);        
	#endif
}





static void websURLFilterDelete(webs_t wp, char_t *path, char_t *query)
{
#if 0
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;

    char *rules = nvram_bufget(RT2860_NVRAM, "websURLFilters");
    if(!rules || !strlen(rules) )
        return;

	rule_count = getRuleNums(rules);
	if(!rule_count)
		return;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	for(i=0, j=0; i< rule_count; i++){
		snprintf(name_buf, 16, "DR%d", i);
		value = websGetVar(wp, name_buf, NULL);
		if(value){
			deleArray[j++] = i;
		}
	}
	if(!j){
	    websHeader(wp);
	    websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
	    websFooter(wp);
	    websDone(wp, 200);		
		return;
	}

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "websURLFilters", rules);
	nvram_commit(RT2860_NVRAM);

	doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	iptablesWebsFilterRun();

    websHeader(wp);
    websWrite(wp, T("Delete : success<br>\n") );
    websFooter(wp);
    websDone(wp, 200);
#endif
	return;
}

static void websHostFilterDelete(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	int i, j, rule_count;
	char_t name_buf[16];
	char_t *value;
	int *deleArray;

    char *rules = nvram_bufget(RT2860_NVRAM, "websHostFilters");
    if(!rules || !strlen(rules) )
        return;

	rule_count = getRuleNums(rules);
	if(!rule_count)
		return;

	deleArray = (int *)malloc(rule_count * sizeof(int));

	for(i=0, j=0; i< rule_count; i++){
		snprintf(name_buf, 16, "DR%d", i);
		value = websGetVar(wp, name_buf, NULL);
		if(value){
			deleArray[j++] = i;
		}
	}
	if(!j){
	    websHeader(wp);
	    websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
	    websFooter(wp);
	    websDone(wp, 200);		
		return;
	}

	deleteNthValueMulti(deleArray, rule_count, rules, ';');
	free(deleArray);

	nvram_set(RT2860_NVRAM, "websHostFilters", rules);
	nvram_commit(RT2860_NVRAM);

	doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	iptablesWebsFilterRun();

    websHeader(wp);
    websWrite(wp, T("Delete : success<br>\n") );
    websFooter(wp);
    websDone(wp, 200);
#endif
	return;
}



static void webContentFilter(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char *proxy = websGetVar(wp, T("websFilterProxy"), T(""));
	char *java = websGetVar(wp, T("websFilterJava"), T(""));
	char *activex = websGetVar(wp, T("websFilterActivex"), T(""));
	char *cookies = websGetVar(wp, T("websFilterCookies"), T(""));

	// someone use malform page.....
	if(!proxy || !java || !activex || !cookies)
		return;

	nvram_bufset(RT2860_NVRAM, "websFilterProxy",   atoi(proxy)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "websFilterJava",    atoi(java)    ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "websFilterActivex", atoi(activex) ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "websFilterCookies", atoi(cookies) ? "1" : "0" );
	nvram_commit(RT2860_NVRAM);

	doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	iptablesWebsFilterRun();

	websRedirect(wp, "firewall/content_filtering.asp");
	return;

	websHeader(wp);
	websWrite(wp, T("Proxy: %s<br>\n"),  atoi(proxy) ? "enable" : "disable");
	websWrite(wp, T("Java: %s<br>\n"),   atoi(java) ? "enable" : "disable");
	websWrite(wp, T("Activex: %s<br>\n"), atoi(activex) ? "enable" : "disable");
	websWrite(wp, T("Cookies: %s<br>\n"), atoi(cookies) ? "enable" : "disable");
    websFooter(wp);
    websDone(wp, 200);
 #endif
}

static void websURLFilter(webs_t wp, char_t *path, char_t *query)
{
	#if 0
	char *urlfilters = nvram_bufget(RT2860_NVRAM, "websURLFilters");
	char *rule = websGetVar(wp, T("addURLFilter"), T(""));
	char *new_urlfilters;
	if(!rule)
		return;
	if(strchr(rule, ';'))
		return;

	if(!urlfilters || !strlen(urlfilters))
		nvram_bufset(RT2860_NVRAM, "websURLFilters", rule);
	else{
		if(! (new_urlfilters = (char *)malloc(sizeof(char) * (strlen(urlfilters)+strlen(rule)+1))))
			return;
		new_urlfilters[0] = '\0';
		strcat(new_urlfilters, urlfilters);
		strcat(new_urlfilters, ";");
		strcat(new_urlfilters, rule);
		nvram_bufset(RT2860_NVRAM, "websURLFilters", new_urlfilters);
		free(new_urlfilters);
	}
	nvram_commit(RT2860_NVRAM);

	doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	iptablesWebsFilterRun();

    websHeader(wp);
    websWrite(wp, T("add URL filter: %s<br>\n"), rule);
    websFooter(wp);
    websDone(wp, 200);
    #endif
}

static void websHostFilter(webs_t wp, char_t *path, char_t *query)
{
	#if 0
	char *hostfilters = nvram_bufget(RT2860_NVRAM, "websHostFilters");
	char *rule = websGetVar(wp, T("addHostFilter"), T(""));
	char *new_hostfilters;
	if(!rule)
		return;
	if(strchr(rule, ';'))
		return;

	if(!hostfilters || !strlen(hostfilters))
		nvram_bufset(RT2860_NVRAM, "websHostFilters", rule);
	else{
		if(! (new_hostfilters = (char *)malloc(sizeof(char) * (strlen(hostfilters)+strlen(rule)+1))))
			return;
		new_hostfilters[0] = '\0';
		strcat(new_hostfilters, hostfilters);
		strcat(new_hostfilters, ";");
		strcat(new_hostfilters, rule);
		nvram_bufset(RT2860_NVRAM, "websHostFilters", new_hostfilters);
		free(new_hostfilters);
	}
	nvram_commit(RT2860_NVRAM);

	doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	iptablesWebsFilterRun();

    websHeader(wp);
    websWrite(wp, T("add Host filter: %s<br>\n"), rule);
    websFooter(wp);
    websDone(wp, 200);
    #endif
}

char *getNameIntroFromPat(char *filename)
{
	static char result[512];
	char buf[512], *begin, *end, *desh;
	char path_filename[512];
	int rc;
	FILE *fp;

	sprintf(path_filename, "%s/%s", "/etc_ro/l7-protocols", filename);
	if(! (fp = fopen(path_filename, "r")))
		return NULL;
	result[0] = '\0';
	rc = fgets(buf, sizeof(buf), fp);
	if(rc){
		// find name
		begin = buf + 2;
		if(! ( desh = strchr(buf, '-'))){
			printf("warning: can't find %s name.\n", filename);
			fclose(fp);
			return "N/A#N/A";
		}
		end = desh;
		if(*(end-1) == ' ')
			end--;
		*end = '\0';
		strncat(result, begin, sizeof(result));
		strncat(result, "#", sizeof(result));

		// find intro
		if(!(end = strchr(desh+1, '\n'))){
			printf("warning: can't find %s intro.\n", filename);
			fclose(fp);
			return "N/A#N/A";
		}
		*end = '\0';
		strncat(result, desh + 2 , sizeof(result));
	}else{
		printf("warning: can't read %s intro.\n", filename);
		fclose(fp);
		return "N/A#N/A";
	}

	fclose(fp);
	return result;	
}


void LoadLayer7FilterName(void)
{
	char *delim;
	struct dirent *dir;
	DIR *d;
	char *intro;

	l7name[0] = '\0';
	if(!(d = opendir("/etc_ro/l7-protocols")))
		return;
	
	while(dir = readdir(d)){
		if(dir->d_name[0] == '.')
			continue;
		if(!(delim = strstr(dir->d_name, ".pat")) )
			continue;
		
		intro = getNameIntroFromPat(dir->d_name);

		*delim = '\0';
		if(l7name[0] == '\0'){
			strncat(l7name, dir->d_name, sizeof(l7name));
			strncat(l7name, "#", sizeof(l7name));
			strncat(l7name, intro, sizeof(l7name));
		}else{
			strncat(l7name, ";", sizeof(l7name));
			strncat(l7name, dir->d_name, sizeof(l7name));
			strncat(l7name, "#", sizeof(l7name));
			strncat(l7name, intro, sizeof(l7name));
		}
	}
	closedir(d);
}

static int getLayer7FiltersASP(int eid, webs_t wp, int argc, char_t **argv)
{
	websLongWrite(wp, l7name);
	return 0;	
}
//===============================================================================================
//2009/01/22 put the code togather for hp
//Add for hp 2009/01/21
static int showTrustedIPAddressASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char TrustedIPAddress[32]={0};
	
	ezplib_get_attr_val("wf_content_rule", 0, "trustipaddr", TrustedIPAddress, 32, EZPLIB_USE_CLI);
	
	if(!strlen(TrustedIPAddress)){
		websWrite(wp, T("%s"), "0.0.0.0");
		return 0;
	}
	
	websWrite(wp, T("%s"), TrustedIPAddress);
	return 0;	
}

//Steve add for hp
static void webSCFilter(webs_t wp, char_t *path, char_t *query)
{
	#if 1
	char *proxy = websGetVar(wp, T("websFilterProxy"), T(""));
	char *java = websGetVar(wp, T("websFilterJava"), T(""));
	char *activex = websGetVar(wp, T("websFilterActivex"), T(""));
	char *cookies = websGetVar(wp, T("websFilterCookies"), T(""));
	char *url_s = websGetVar(wp, T("url_str"), T(""));
	char *url_enabled = websGetVar(wp, T("cfKeyWord_Enable"), T(""));
	char *ip_address = websGetVar(wp, T("websTrustedIPAddress"), T(""));
	
	char_t *submitUrl;
	
	//Steve add
	char temp_str[65];
	int flag=0;
	char str_word[129];
	int rule_num=0;
	int i;
	char rule[1024];
	char *nv_rule = "wf_rule";
	char str_tmp[10];
	
	if(!isIpValid(ip_address)){//invalid mac address
		strcpy(temp_str, "IP is invaild");
		flag=1;
		goto end;
	}

	// someone use malform page.....
	if(!proxy || !java || !activex || !cookies){
		strcpy(temp_str, "CGI parameter error");
		flag=1;
		goto end;
	}
	
	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop wf\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl stop wf");
	
	ezplib_replace_attr("wf_content_rule", 0, "activex_enable", atoi(activex) ? "1" : "0");
	ezplib_replace_attr("wf_content_rule", 0, "java_enable", atoi(java) ? "1" : "0");
	ezplib_replace_attr("wf_content_rule", 0, "proxy_enable", atoi(proxy) ? "1" : "0");
	ezplib_replace_attr("wf_content_rule", 0, "cookie_enable", atoi(cookies) ? "1" : "0");
	if(!strcmp(ip_address,"0.0.0.0"))
		ezplib_replace_attr("wf_content_rule", 0, "trustipaddr", "");
	else
		ezplib_replace_attr("wf_content_rule", 0, "trustipaddr", ip_address);
    
    ezplib_replace_attr("wf_content_rule", 0, "url_enable", atoi(url_enabled) ? "1" : "0");
    
    printf("url_s=%s, strlen(url_s)=%d\n",url_s,strlen(url_s));
    
    if(strlen(url_s)!=0){
    	printf("wf_rule=""\n");
    	nvram_set("wf_rule", "");	
    	
    	rule_num=getRuleNums(url_s);
    	sprintf(str_tmp,"%d",rule_num);
    	nvram_set("wf_rule_num", str_tmp);	
    	printf("rule_num=%d str_tmp=%s\n",rule_num,str_tmp);
    	
    	for(i=0;i<rule_num;i++){
    		if((getNthValueSafe(i, url_s, ' ', str_word, sizeof(str_word)) != -1)){	
    			printf("str_word=%s \n len=%d\n",str_word, strlen(str_word));
    			sprintf(rule, "%s^%s^url^0", atoi(url_enabled) ? "1" : "0", "");
				
    			ezplib_append_rule(nv_rule, rule);
			ezplib_replace_attr("wf_rule", i, "keyword",str_word);
    		}	
    	}
    }else{
    	printf("wf_rule=""\n");
    	nvram_set("wf_rule", "");
    	nvram_set("wf_rule_num", "0");	
	}
    
    
    printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start wf\n");
	doSystem("NUM=0  /etc/rc.common /etc/init.d/network-acl start wf");
	
    printf("nvram_commit\n");
	nvram_commit();
    
	//nvram_bufset(RT2860_NVRAM, "websFilterProxy",   atoi(proxy)   ? "1" : "0" );
	//nvram_bufset(RT2860_NVRAM, "websFilterJava",    atoi(java)    ? "1" : "0" );
	//nvram_bufset(RT2860_NVRAM, "websFilterActivex", atoi(activex) ? "1" : "0" );
	//nvram_bufset(RT2860_NVRAM, "websFilterCookies", atoi(cookies) ? "1" : "0" );	
	//nvram_bufset(RT2860_NVRAM, "websHostFiltersEnabled", atoi(url_enabled) ? "1" : "0" );
	//nvram_bufset(RT2860_NVRAM, "websHostFilters", url_s);
	
	//nvram_bufset(RT2860_NVRAM, "websTrustedIPAddress", ip_address);
	
	//nvram_commit(RT2860_NVRAM);

    //printf("url_s:%s\n",url_s);

	//doSystem("iptables -t filter -F " WEB_FILTER_CHAIN);
	//iptablesWebsFilterRun();

end:	
	//Steve add 
    if(flag)
    	setWebMessage(1, (char *)&temp_str);
    else
    	setWebMessage(0, NULL);
    	
    submitUrl = websGetVar(wp, T("scfilter_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
	#endif
}

//Steve add for hp
static void REMOTE_WAN(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char *rmE = websGetVar(wp, T("remoteManagementEnabled"), T(""));
    char *rmPort = websGetVar(wp, T("remoteManagementPort"), T(""));
    char *rmPort_ram = nvram_bufget(RT2860_NVRAM, "RemoteManagementPort");
    char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
    char_t *submitUrl;
    char wan_ip[16];
    
    printf("Enter %s \n",__FUNCTION__);
    
	if(atoi(rmE) == 0){		// disable
		nvram_bufset(RT2860_NVRAM, "RemoteManagement", "0");
	}else{					// enable
		nvram_bufset(RT2860_NVRAM, "RemoteManagement", "1");
		nvram_bufset(RT2860_NVRAM, "RemoteManagementPort", rmPort);
	}
	
	nvram_commit(RT2860_NVRAM); 
	
	//iptablesWANServiceRun();
	
	if ( getIfIp(getWanIfNamePPP(), wan_ip) == -1){
    	 printf("WAN IP error\n");
    	 //return;	 
    }
    
#if 0    
    printf("wanIF:%s\n",getWanIfNamePPP());
    printf("wanIP:%s\n",wan_ip);
    printf("lanIP:%s\n",lan_ip);
    printf("rmPort:%s\n",rmPort);
#endif	
	
	if(atoi(rmE) == 0){
		doSystem("iptables -t nat -D PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:80"
				, getWanIfNamePPP(), wan_ip, rmPort_ram, lan_ip);
	}
	else{
	    doSystem("iptables -t nat -D PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:80"
				, getWanIfNamePPP(), wan_ip, rmPort_ram, lan_ip);
		doSystem("iptables -t nat -I PREROUTING -p tcp -i %s -d %s --dport %s -j DNAT --to %s:80"
				, getWanIfNamePPP(), wan_ip, rmPort, lan_ip);
	}
	
    submitUrl = websGetVar(wp, T("remote_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
#endif
}


//Steve 2009/02/04
static void easy_parent_control(webs_t wp, char_t *path, char_t *query)
{
	char *url_s = websGetVar(wp, T("url_str"), T(""));	
	char_t *submitUrl;
	char str_word[129];
	int rule_num=0;
	int i;
	char rule[1024];
	char *nv_rule = "wf_rule";
	char str_tmp[10];
	char TempBuf[32];
	
	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop wf\n");
	system("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop wf");
	
	ezplib_get_attr_val("wf_content_rule", 0, "url_enable", TempBuf, 32, EZPLIB_USE_CLI);
    
    printf("url_s:%s\n",url_s);
    
    if(strlen(url_s)!=0){
    	printf("wf_rule=""\n");
    	nvram_set("wf_rule", "");	
    	
    	rule_num=getRuleNums(url_s);
    	sprintf(str_tmp,"%d",rule_num);
    	nvram_set("wf_rule_num", str_tmp);	
    	printf("rule_num=%d str_tmp=%s\n",rule_num,str_tmp);
    	
    	for(i=0;i<rule_num;i++){
    		if((getNthValueSafe(i, url_s, ';', str_word, sizeof(str_word)) != -1)){	
    			printf("str_word=%s \n len=%d\n",str_word, strlen(str_word));
    			sprintf(rule, "%s^%s^url^0", TempBuf, str_word);
    			ezplib_append_rule(nv_rule, rule);
    		}	
    	}
    }else{
    	printf("wf_rule=""\n");
    	nvram_set("wf_rule", "");
    	nvram_set("wf_rule_num", "0");	
	}
    printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start wf\n");
	system("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start wf");
    
    printf("nvram_commit\n");
	nvram_commit();

end:	
    submitUrl = websGetVar(wp, T("epc_url"), T(""));   // hidden page
	websRedirect(wp, submitUrl);
}

//2009/02/09
static void remote_web(webs_t wp, char_t *path, char_t *query)
{
	char *rmS = websGetVar(wp, T("remoteManagementSel"), T(""));
    char *rmPort = websGetVar(wp, T("remoteManagementPort"), T(""));
    //char *rmPort_ram = nvram_bufget(RT2860_NVRAM, "RemoteManagementPort");
    //char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
    char wan_ip[16];
    
    char *rmSecuredSel = websGetVar(wp, T("btnwww"), T(""));
    char *rmSecuredIP = websGetVar(wp, T("txtwwwip"), T(""));
     FILE *stream = -1;
    char port_buffer[16];

#if 1
    #define SYS_PORT_LIST_PATH "/tmp/socket_port_list.tmp"
    system("netstat -tan | awk '{print $4}' | awk 'BEGIN{FS=\":\"}{if(length($2)) print $2;}' > /tmp/socket_port_list.tmp ");

    if ((stream = fopen(SYS_PORT_LIST_PATH, "r")) == NULL) {
		printf("open %s fail\n", SYS_PORT_LIST_PATH);
		fclose(stream);
		system("rm -rf /tmp/socket_port_list.tmp");
		return -1;	
    }

   memset(port_buffer, 0, 16);
   while ( ( fgets(port_buffer, 15, stream) ) != NULL) {
   	printf("***********************************************buffer: %s, remote port: %s\n", port_buffer, rmPort);
	if( ( atoi(port_buffer) == atoi(rmPort)  ) && (atoi(port_buffer) != 80) && ( atoi(port_buffer) != 8080) ) {
		fclose(stream);
		system("rm -rf /tmp/socket_port_list.tmp");
		websRedirect(wp, "local/advance/remote_web.asp");
		return 0;
	}
	memset(port_buffer, 0, 16);
   }

   fclose(stream);
   system("rm -rf /tmp/socket_port_list.tmp");
   #undef SYS_PORT_LIST_PATH
 #endif
    
    printf("Enter %s \n",__FUNCTION__);
	
	ezplib_replace_attr("http_rule", 0, "rmgt_enable", rmS);
	
	//Steve marked temporarily
	printf("Steve marked http_rule -> port temporarily \n");
	#if 1
	ezplib_replace_attr("http_rule", 0, "port", rmPort);
	#endif
	
	if(strcmp(rmSecuredSel,"1")==0)
		ezplib_replace_attr("http_rule", 0, "secipaddr", rmSecuredIP);
	else
		ezplib_replace_attr("http_rule", 0, "secipaddr", "");
	
	printf("nvram_commit()\n");
	nvram_commit(); 
		

#if 0		
   	//aron add  2009.07.03
    system("echo 1 > /var/keep_udhcpd");
    system("echo 1 > /var/wait_cgi");
    doSystem("killall -q -USR2 udhcpd");
    	
  	websHeader(wp);
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
    websFooter(wp);
    websDone(wp, 200);
    
    doSystem("echo 1 > /var/RemoteWeb");  //aron add
    doSystem("remote_restart.sh &");
	/*-------------------------------*/
	system("sleep 3 && reboot &");
	websHeader(wp);
	websWrite(wp, T("<script>\n"));
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding_reboot.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
	
	//submitUrl = websGetVar(wp, T("remote_url"), T(""));   // hidden page
	//websRedirect(wp, submitUrl);
#else
	/*---Disable restart-----------------------2013-05-07----Ricardo------*/
	websHeader(wp);
	websWrite(wp, T("<script>\n")); 
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding2.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);

	doSystem("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fw");
	doSystem("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start fw");
	doSystem("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fr");
	doSystem("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start fr");
	doSystem("NUM=0  /etc/rc.common /etc/rc.d/S70goahead start");

#endif

}

//2009/02/13
int parental_control_switch(int flag)
{
	char *value;
	int rule_num=0;
	int i;
	
	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop wf\n");
	system("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop wf");
	
	value=nvram_safe_get("wf_rule");
	
	if(strlen(value)!=0)
	{   
		rule_num=atoi(nvram_safe_get("wf_rule_num"));
		for(i=0;i<rule_num;i++)
			ezplib_replace_attr("wf_rule", i, "enable", flag ? "1" : "0");
	}
	
	ezplib_replace_attr("wf_content_rule", 0, "url_enable", flag ? "1" : "0");

	printf("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start wf\n");
	system("NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start wf");
	
	printf("nvram_commit\n");
	nvram_commit();
	
	return 0;
}

//2009/02/16
int firewall_switch(int flag)
{		
	system("/etc/rc.common /etc/rc.d/S60network-acl stop fw");
	
	if(flag==1){
		ezplib_replace_attr("fw_rule", 0, "state_enable", "1");
		ezplib_replace_attr("fw_rule", 0, "syn_enable", "1");
		ezplib_replace_attr("fw_rule", 0, "icmp_enable", "1");
		ezplib_replace_attr("fw_rule", 0, "icmp_redir_enable", "1");
	}else{
		ezplib_replace_attr("fw_rule", 0, "state_enable", "0");
		ezplib_replace_attr("fw_rule", 0, "syn_enable", "0");
		ezplib_replace_attr("fw_rule", 0, "icmp_enable", "0");
		ezplib_replace_attr("fw_rule", 0, "icmp_redir_enable", "0");
	}
	
	system("/etc/rc.common /etc/rc.d/S60network-acl start fw");
     
	printf("nvram_commit\n");
	nvram_commit();  
}

//End 
//============================================================================================




void firewall_init(void)
{
#if 0
//Steve add 2009/01/22	
//char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
char *mode;
if((!strcmp(mode, "1" )) ||(!strcmp(mode, "2" ))){ //1:Router 2:WISP
	
	unsigned char phy_wan_ip[16];	
	
	LoadLayer7FilterName();

	// init filter
	iptablesAllFilterClear();
	iptablesAllNATClear();
	
	
	if (getIfIp(getWanIfNamePPP(), phy_wan_ip) == -1) {
		doSystem("iptables -P INPUT ACCEPT");
		doSystem("iptables -P FORWARD ACCEPT");
		//return;
	}
	
	// make a new chain
	doSystem("iptables -A FORWARD -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu 1>/dev/null 2>&1");
#ifdef	FIREWALL_DOS
	/* DoS Filter @ INPUT */
	printf("===================	Filter Chain  =============\n");
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", DOS_FILTER_CHAIN);
	doSystem("iptables -t filter -A INPUT -j %s 1>/dev/null 2>&1", DOS_FILTER_CHAIN);
	
	#ifdef FIREWALL_DOS_LOG
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", DOS_LOG_CHAIN);
	doSystem("iptables -t filter -A INPUT -j %s 1>/dev/null 2>&1", DOS_LOG_CHAIN);
	#endif
#endif	
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", WEB_FILTER_CHAIN);
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", IPPORT_FILTER_CHAIN);
	doSystem("iptables -t filter -A FORWARD -j %s 1>/dev/null 2>&1", WEB_FILTER_CHAIN);
	doSystem("iptables -t filter -A FORWARD -j %s 1>/dev/null 2>&1", IPPORT_FILTER_CHAIN);
#if 1 //define FIREWALL_PORTTRIGGER
	/* Port Trigger @ FORWARD *///Steve
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
	doSystem("iptables -t filter -A FORWARD -j %s 1>/dev/null 2>&1", PORT_TRIGGER_CHAIN);
#endif	
	/* Port Forward @ FORWARD */
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", PORT_FORWARD_CHAIN);
	doSystem("iptables -t filter -A FORWARD -j %s 1>/dev/null 2>&1", PORT_FORWARD_CHAIN);
#if 1	
	/* DMZ @ FORWARD *///Steve 2009/03/09 add
	doSystem("iptables -t filter -N %s 1>/dev/null 2>&1", DMZ_FORWARD_CHAIN);
	doSystem("iptables -t filter -A FORWARD -j %s 1>/dev/null 2>&1", DMZ_FORWARD_CHAIN);
#endif
    iptablesAllFilterRun();

	// init NAT(DMZ)
	// We use -I instead of -A here to prevent from default MASQUERADE NAT rule 
	// being in front of us.
	// So "port forward chain" has the highest priority in the system, and "DMZ chain" is the second one.
	//iptablesAllNATClear();
	printf("===================	NAT Chain  =============\n");
#if 1 //define FIREWALL_PORTTRIGGER
	/* Port Trigger @ PREROUTING *///Steve
	doSystem("iptables -t nat -N %s 1>/dev/null 2>&1; iptables -t nat -I PREROUTING 1 -j %s 1>/dev/null 2>&1", PORT_TRIGGER_CHAIN, PORT_TRIGGER_CHAIN);
#endif	
	doSystem("iptables -t nat -N %s 1>/dev/null 2>&1; iptables -t nat -I PREROUTING 2 -j %s 1>/dev/null 2>&1", PORT_FORWARD_CHAIN, PORT_FORWARD_CHAIN);
	doSystem("iptables -t nat -N %s 1>/dev/null 2>&1; iptables -t nat -I PREROUTING 3 -j %s 1>/dev/null 2>&1", DMZ_CHAIN, DMZ_CHAIN);
	
	/* Port Forward @ POSTROUTING *///Setve 2009/02/23 
	doSystem("iptables -t nat -N %s 1>/dev/null 2>&1; iptables -t nat -A POSTROUTING -j %s 1>/dev/null 2>&1", PORT_FORWARD_POST_CHAIN, PORT_FORWARD_POST_CHAIN);
	
	/* DMZ Forward @ POSTROUTING *///Setve 2009/12/07
	#ifdef DMZ_POST_LOOPBACK
	doSystem("iptables -t nat -N %s 1>/dev/null 2>&1; iptables -t nat -A POSTROUTING -j %s 1>/dev/null 2>&1", DMZ_POST_CHAIN, DMZ_POST_CHAIN);
	#endif
	iptablesAllNATRun();
	iptablesNATBasic();
}//if	
#endif
}

void firewall_fini(void)
{
	iptablesAllFilterClear();
	iptablesAllNATClear();
}
#endif//Steve24

void formDefineFirewall(void)
{
	#if 1
	websAspDefine(T("getDefaultFirewallPolicyASP"), getDefaultFirewallPolicyASP);
	websFormDefine(T("BasicSettings"), BasicSettings);

	websAspDefine(T("getIPPortFilterEnableASP"), getIPPortFilterEnableASP);
	websAspDefine(T("showIPPortFilterRulesASP"), showIPPortFilterRulesASP);
	websAspDefine(T("getIPPortRuleNumsASP"), getIPPortRuleNumsASP);
	websFormDefine(T("ipportFilter"), ipportFilter);
	websFormDefine(T("ipportFilterDelete"), ipportFilterDelete);
	websFormDefine(T("getRulesPacketCount"), getRulesPacketCount);

	websFormDefine(T("DMZ"), DMZ);	
	websAspDefine(T("getDMZEnableASP"), getDMZEnableASP);
	websAspDefine(T("showDMZIPAddressASP"), showDMZIPAddressASP);

	websAspDefine(T("getPortForwardEnableASP"), getPortForwardEnableASP);
	websAspDefine(T("showPortForwardRulesASP"), showPortForwardRulesASP);
	websAspDefine(T("getPortForwardRuleNumsASP"), getPortForwardRuleNumsASP);
	websFormDefine(T("portForward"), portForward);
	websFormDefine(T("portForwardDelete"), portForwardDelete);

	/* Steve, for Port Forward --> NAT Application hp */
	websAspDefine(T("showNatApplicationRulesASP"), showNatApplicationRulesASP);
	websAspDefine(T("showNatAdvanceRulesASP"), showNatAdvanceRulesASP);
	websAspDefine(T("showIPPortFilterServiceRulesASP"), showIPPortFilterServiceRulesASP);
	websAspDefine(T("getCfgNat"), getCfgNat);
	websAspDefine(T("showTrustedIPAddressASP"), showTrustedIPAddressASP);
	
	websFormDefine(T("portForward_natAPP"), portForward_natAPP);
	websFormDefine(T("portTrigger_natADV"), portTrigger_natADV);
	websFormDefine(T("FIREWALL_ACTIVE"), FIREWALL_ACTIVE);
	websFormDefine(T("OPENDNS"), opendns_set);
	websFormDefine(T("DMZ_NAT"), DMZ_NAT);
	websFormDefine(T("SPI_ACTIVE"), SPI_ACTIVE);
	websFormDefine(T("webSCFilter"), webSCFilter); 
	websFormDefine(T("ipportFilter_service"), ipportFilter_service);
	websFormDefine(T("ipportFilter_enable"), ipportFilter_enable);
	websFormDefine(T("ipportFilter_icmp"), ipportFilter_icmp);
	websFormDefine(T("REMOTE_WAN"), REMOTE_WAN);
	websFormDefine(T("easy_parent_control"), easy_parent_control);
	websFormDefine(T("remote_web"), remote_web);

	/*for one-to-one nat, add by wind 2013-12-09 */
	websAspDefine(T("getCfg_OneToOneNat"), getCfg_OneToOneNat);
	websAspDefine(T("showOneToOneRulesASP"), showOneToOneRulesASP);
	websFormDefine(T("oneToOne_natAPP"), oneToOne_natAPP);
	//Steve add tmp
	websFormDefine(T("ipportFilter_service2"), ipportFilter_service2);
	websFormDefine(T("ipportFilter_enable2"), ipportFilter_enable2);
	
	/* Steve, for Port Trigger */
	websAspDefine(T("getPortTriggerEnableASP"), getPortTriggerEnableASP);
	websAspDefine(T("showPortTriggerRulesASP"), showPortTriggerRulesASP);
	websAspDefine(T("getPortTriggerRuleNumsASP"), getPortTriggerRuleNumsASP);
	websFormDefine(T("portTrigger"), portTrigger);
	websFormDefine(T("portTriggerDelete"), portTriggerDelete);
	

	websFormDefine(T("websSysFirewall"), websSysFirewall);

	websFormDefine(T("webContentFilter"), webContentFilter);
	websFormDefine(T("websURLFilterDelete"), websURLFilterDelete);
	websFormDefine(T("websHostFilterDelete"), websHostFilterDelete);
	websFormDefine(T("websHostFilter"), websHostFilter);
	websFormDefine(T("websURLFilter"), websURLFilter);

	websAspDefine(T("getLayer7FiltersASP"), getLayer7FiltersASP);

	websAspDefine(T("checkIfUnderBridgeModeASP"), checkIfUnderBridgeModeASP);
	//william add for pptpd 
	websFormDefine(T("vpn_pptpd_general"), vpn_pptpd);
	websFormDefine(T("vpn_pptpd_user_rule"), vpn_pptpd);
	//Kissth add for l2tpd 
	websFormDefine(T("vpn_l2tpd_general"), vpn_l2tpd);
	websFormDefine(T("vpn_l2tpd_user_rule"), vpn_l2tpd);

	websFormDefine(T("vpn_passthrough"), save_vpn_passthrough);
	//ftp passthrough add by wind 2013-08-20
	websFormDefine(T("ftp_passthrough"), save_ftp_passthrough);
	//h323 passthrough add by wind 2013-08-20
	websFormDefine(T("h323_passthrough"), save_h323_passthrough);
	//ipsec
	websAspDefine(T("get_ipsec_status"), ej_get_ipsec_status);
	websFormDefine(T("vpn_ipsec_general"), vpn_ipsec2);
	websFormDefine(T("vpn_ipsec_rule"), vpn_ipsec2);
	websAspDefine(T("isMaximum"), isMaximum);
	websAspDefine(T("showIpsecVpnRulesASP"), showIpsecVpnRulesASP);

    //happiness add for selected ip warning
    websAspDefine(T("getSelectedip"), getSelectedip);
	#endif
}
