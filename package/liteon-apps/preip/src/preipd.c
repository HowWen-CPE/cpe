/*
 *	preip daemon
 *
 *	Authors:
 *	simon Jiang		<simon.jiang@liteon.com>
 *	create Date:		2006.12.06
 */

/*PRE IP Daemon*/
#include <sys/types.h>		
#include <sys/socket.h>	
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <linux/in.h>	
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "ostypes.h"
//#include "sysconf.h"   /* add by peter, 2008.7.30*/
//#include "b_adminConf.h"
//#include "wlanconf.h"

#define PRE_IP_DEBUG

#define PREIP_DAEMON_INCLUDE
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#include "preip.h"
#include "nvram.h"
#include "nvram_rule.h"
//#include "mid_detail.h"

typedef int T_INT32;
/*************************************************

		Initial Global Config
**************************************************/
void init_global_config()
{
	int i = 0;
	char buf[256] = {0};
	char *value = NULL;

	//initial value
	for( i = 0; i < GLOBAL_CONFIGS_COUNT; i++ ) {
		global_configs[i].flag = NONE;
		global_configs[i].readonly = READWRITE;
		global_configs[i].item = NULL;
		global_configs[i].item_index = CLI_INDEX_NONE;
	}

	//1. init ssid
	struct item_config *ssid_item = (struct item_config *)malloc(sizeof(struct item_config));
	if (!ssid_item) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(ssid_item->name, CLI_NAME_SSID);
	strcpy(ssid_item->params[0].param_name, CLI_NAME_SSID);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 32, EZPLIB_USE_CLI);
	strcpy(ssid_item->params[0].value, buf);
	global_configs[CLI_INDEX_SSID].item = ssid_item;
	global_configs[CLI_INDEX_SSID].item_index = CLI_INDEX_SSID;


	//2. ip address
	struct item_config *ipaddr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!ipaddr) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(ipaddr->name, CLI_NAME_IPADDR);
	strcpy(ipaddr->params[0].param_name, CLI_NAME_IPADDR);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	//if (!strcmp(buf, "static")){
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 128, EZPLIB_USE_CLI);
	strcpy(ipaddr->params[0].value, buf);
	//} else {
	//	value = nvram_safe_get("lan0_ipaddr");
	//	if (value && (strlen(value) > 0) )
	//		strcpy(ipaddr->params[0].value, value);
	//}
	global_configs[CLI_INDEX_IPADDR].item = ipaddr;
	global_configs[CLI_INDEX_IPADDR].item_index = CLI_INDEX_IPADDR;

	//3. netmask
	struct item_config *netmask = (struct item_config *)malloc(sizeof(struct item_config));
	if (!netmask) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(netmask->name, CLI_NAME_NETMASK);
	strcpy(netmask->params[0].param_name, CLI_NAME_NETMASK);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	//if (!strcmp(buf, "static")){
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, 128, EZPLIB_USE_CLI);
	strcpy(netmask->params[0].value, buf);
	//}else{
	//	value = nvram_safe_get("lan0_mask");
	//	if (value)
	//		strcpy(netmask->params[0].value, value);
	//}
	global_configs[CLI_INDEX_NETMASK].item = netmask;
	global_configs[CLI_INDEX_NETMASK].item_index = CLI_INDEX_NETMASK;

	//4. connect rssi thr
	struct item_config *conn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!conn_rssi_thr) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(conn_rssi_thr->name, CLI_NAME_CONNRSSIHR);
	strcpy(conn_rssi_thr->params[0].param_name, CLI_NAME_CONNRSSIHR);
	//value = nvram_safe_get("connectrssithr");
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, 32, EZPLIB_USE_CLI);
	strcpy(conn_rssi_thr->params[0].value, buf);
	global_configs[CLI_INDEX_CONNRSSITHR].item = conn_rssi_thr;
	global_configs[CLI_INDEX_CONNRSSITHR].item_index = CLI_INDEX_CONNRSSITHR;

	//5. disconnect rssi thr
	struct item_config *disconn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!disconn_rssi_thr) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(disconn_rssi_thr->name, CLI_NAME_DISCONNRSSITHR);
	strcpy(disconn_rssi_thr->params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	//value = nvram_safe_get("disconnectrssithr");
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, 32, EZPLIB_USE_CLI);
	strcpy(disconn_rssi_thr->params[0].value, buf);
	global_configs[CLI_INDEX_DISCONNRSSITHR].item = disconn_rssi_thr;
	global_configs[CLI_INDEX_DISCONNRSSITHR].item_index = CLI_INDEX_DISCONNRSSITHR;

	//6. deviceid
	struct item_config *deviceid = (struct item_config *)malloc(sizeof(struct item_config));
	if (!deviceid) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(deviceid->name, CLI_NAME_DEVICEID);
	strcpy(deviceid->params[0].param_name, CLI_NAME_DEVICEID);
	value = nvram_safe_get("hostname");
	if (value && strcmp(value, ""))
		strcpy(deviceid->params[0].value, value);
	global_configs[CLI_INDEX_DEVICEID].item = deviceid;
	global_configs[CLI_INDEX_DEVICEID].item_index = CLI_INDEX_DEVICEID;

	//7. account
	memset(buf, 0, sizeof(buf));
	struct item_config *acount = (struct item_config *)malloc(sizeof(struct item_config));
	if (!acount) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(acount->name, CLI_NAME_ACCOUNT);
	strcpy(acount->params[0].param_name, CLI_NAME_ACCOUNT);
	ezplib_get_attr_val("http_rule", 0, "admusername", buf, 128, EZPLIB_USE_CLI);
	if (strcmp(buf, ""))
		strcpy(acount->params[0].value, buf);
	global_configs[CLI_INDEX_ACCOUNT].item = acount;
	global_configs[CLI_INDEX_ACCOUNT].item_index = CLI_INDEX_ACCOUNT;

	//8. wireless security
	memset(buf, 0, sizeof(buf));
	struct item_config *security = (struct item_config *)malloc(sizeof(struct item_config));
	if (!security) {
		printf("malloc fail\n");
		exit(0);
	}
	//param0 auth mode
	strcpy(security->name, CLI_NAME_SECURITY);

	//param1 psk
	strcpy(security->params[1].param_name, SECURITY_KEY);

	//param2 ttls/peap
	strcpy(security->params[2].param_name, SECURITY_8021X_AUTH_TYPE);

	//param4 user
	strcpy(security->params[3].param_name, SECURITY_8021X_AUTH_USERNAME);
	
	//param5 password
	strcpy(security->params[4].param_name, SECURITY_8021X_AUTH_PASSWORD);
	
	strcpy(security->params[0].param_name, SECURITY_AUTH_MODE);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", buf, 128, EZPLIB_USE_CLI);
	strcpy(security->params[0].value, buf);

	if (!strcmp(buf, "psk")) {
		//param1 psk
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "key", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[1].value, buf);
	} else if (!strcmp(buf, "psk2")) {
		//param2 psk
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "key", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[1].value, buf);
	} else if (!strcmp(buf, "wpa")){
		//para2 wpa_auth
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[2].value, buf);

		//para3 wpa_user
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[3].value, buf);

		//para4 wpa_passwd
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[4].value, buf);
	} else if (!strcmp(buf, "wpa2"))
	{
		//para2 wpa_auth
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[2].value, buf);

		//para3 wpa_user
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[3].value, buf);

		//para4 wpa_passwd
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf, 128, EZPLIB_USE_CLI);
		strcpy(security->params[4].value, buf);
	}
	
	global_configs[CLI_INDEX_SECURITY].item = security;
	global_configs[CLI_INDEX_SECURITY].item_index = CLI_INDEX_SECURITY;

	//9. dhcp
	struct item_config *dhcp = (struct item_config *) malloc(sizeof(struct item_config));
	if (!dhcp) {
		printf("malloc fail\n");
		exit(0);
	}
	strcpy(dhcp->name, CLI_NAME_DHCP);
	strcpy(dhcp->params[0].param_name, CLI_NAME_DHCP);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	strcpy(dhcp->params[0].value, buf);
	global_configs[CLI_INDEX_DHCP].item = dhcp;
	global_configs[CLI_INDEX_DHCP].item_index = CLI_INDEX_DHCP;
}

void write_to_nvram()
{
	int i = 0, j = 0;
	char buf[256] = {0};
	char sec_mode[32] = {0};

	for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
		struct item_config *item = global_configs[i].item;

		if (!item)
			continue;

		//check item is writable and modified.
		if (global_configs[i].readonly && !global_configs[i].flag)
			continue;

		switch(global_configs[i].item_index) {
			case CLI_INDEX_SSID:
				if (!strcmp(item->params[0].param_name, CLI_NAME_SSID))
					ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", item->params[0].value);
				
				break;
			case CLI_INDEX_DEVICEID:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DEVICEID))
					nvram_set("hostname", item->params[0].value);
				
				break;
			case CLI_INDEX_SECURITY:
				if(cli_debug)
					printf("Apply security: \n");
				
				for (j = 0; j < SECURITY_PARAM_NUM; j++) {
					memset(buf, 0, sizeof(buf));
					struct param_pair *pair = NULL;
					pair = &(item->params[j]);

					if (!pair)
						continue;
					if(cli_debug)
						printf("pair->param_name=%s, pair->value=%s\n", pair->param_name, pair->value);
					
					if (!strcmp(pair->param_name, SECURITY_AUTH_MODE)) {
						ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", pair->value);
						strcpy(sec_mode, pair->value);
					} 
					else if (!strcmp(pair->param_name, SECURITY_KEY)) {
						if (!strcmp(sec_mode, "psk"))
							ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", pair->value);
						else if (!strcmp(sec_mode, "psk2"))
							ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", pair->value);
					}else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_TYPE)) {
						ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", pair->value);
					}else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_USERNAME))
					{
						ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", pair->value);
					}else if (!strcmp(pair->param_name, SECURITY_8021X_AUTH_PASSWORD))
					{
						ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", pair->value);
					}
				}
				
				break;
			case CLI_INDEX_NETMASK:
				if (!strcmp(item->params[0].param_name, CLI_NAME_NETMASK)) {
					//printf("lan0_mask: %s\n", item->params[0].value);
					nvram_set("lan0_mask", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "mask", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_IPADDR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_IPADDR)) {
					//printf("lan0_ipaddr: %s\n", item->params[0].value);
					nvram_set("lan0_ipaddr", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "ipaddr", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_DISCONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DISCONNRSSITHR))
					ezplib_replace_attr("wl0_apcli_rule", 0, "disconnrssi", item->params[0].value);
				
				break;
			case CLI_INDEX_CONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_CONNRSSIHR))
					ezplib_replace_attr("wl0_apcli_rule", 0, "connrssi",  item->params[0].value);
				
				break;
			case CLI_INDEX_ACCOUNT:
				if (!strcmp(item->params[0].param_name, CLI_NAME_ACCOUNT))
					ezplib_replace_attr("http_rule", 0, "admpasswd", item->params[0].value);
				
				break;
			case CLI_INDEX_DHCP:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DHCP)) {
					if (!strcmp(item->params[0].value, "dhcp")) {
						ezplib_replace_attr("lan0_proto", 0, "curr", "dhcp");
						//nvram_set("lan0_ipaddr", "");
						//nvram_set("lan0_mask", "");
					} else {
						//memset(buf, 0, sizeof(buf));
						//ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 128, EZPLIB_USE_CLI);
						ezplib_replace_attr("lan0_proto", 0, "curr", "static");
						//if (strlen(buf) <= 0) {
						//	nvram_set("lan0_ipaddr", "192.168.1.2");
						//	nvram_set("lan0_mask", "24");
						//	ezplib_replace_attr("lan_static_rule", 0, "ipaddr", "192.168.1.2");
						//	ezplib_replace_attr("lan_static_rule", 0, "mask", "24");
						//}
					}
				}
				
				break;
			default:
				printf("Invalid param");
		}
	}
}

void free_global_config()
{
	int i = 0;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
		if(global_configs[i].item) {
			if (cli_debug)
				printf("%s is freed\n", global_configs[i].item->name);
			free(global_configs[i].item);
		}
	}
}

void debug_global_config()
{
	int i = 0, j = 0;

	if (1) {
	//if (cli_debug) {
		for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
			struct item_config *item = global_configs[i].item;
			if (item) {
				printf("item: %s\n", item->name);
				for (j = 0; j < GLOBAL_ITEM_TATAL_PARAMS; j++ ) {
					struct param_pair pair = item->params[j];
					printf("param: %s, value: %s\n", pair.param_name, pair.value);
				}
			} 
		}
	}
}

int set_value(const char *name ,struct item_config *item)
{
	int i = 0, j = 0, len1, len2, max_len;
	struct item_config *im = NULL;
	struct param_pair *pair1 = NULL, *pair2 = NULL;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT ; i++) {
		im = global_configs[i].item;
		if (cli_debug)
			printf("im->name: %s, i = %d\n", im->name, i);
		
		if (!im) {
			printf("im null\n");
			return T_FALSE;
		}
		if (cli_debug)
			printf("--------------%s\n", im->name);
		
		//find item_config at the global configs
		if (!strncmp(name, im->name, strlen(im->name))) {
			if (cli_debug)
				printf("find a item: %s\n", name);
			
			break;
		}
	}

	if (im) {
		for (j = 0; j < GLOBAL_ITEM_TATAL_PARAMS; j++) {
			pair1 = &(item->params[j]);

			if (!pair1 || !strlen(pair1->param_name))
				break;
			
			for (i = 0; i < GLOBAL_ITEM_TATAL_PARAMS; i++) {
				pair2 = &(im->params[i]);
				if (!strlen(pair2->param_name))
					break;
				
				if (cli_debug)
					printf("pair1: %s:, pair2: %s\n", pair1->param_name, pair2->param_name);

				len1 = strlen(pair1->param_name);
				len2 = strlen(pair2->param_name);
				max_len = (len1>len2)?len1:len2;
				
				if (!strncmp(pair1->param_name, pair2->param_name, max_len)) {
					if (cli_debug)
						printf("%s: old value=%s  new value=%s\n", pair1->param_name, pair2->value, pair1->value);
					
					strcpy(pair2->value, pair1->value);
					
					break;
				}
			}
		}
	}
	
	return T_TRUE;
}

int get_value(const char *name, struct item_config *item)
{
	int i = 0;
	struct item_config *im = NULL;

	for (i = 0; i < GLOBAL_CONFIGS_COUNT ; i++) {
		im = global_configs[i].item;
		if (!im) {
			if (cli_debug)
			printf("im null, i = %d\n", i);
			return T_FALSE;
		}

		//find item_config at the global configs
		if (cli_debug)
			printf("name1: %s, name2: %s\n", name, im->name);
		if (!strncmp(name, im->name, strlen(im->name))) {
			if (cli_debug)
				printf("name: %s, exsisted\n", im->name);
			memcpy(item, im, sizeof(struct item_config));
			break;
		}		
	}

	return T_TRUE;
}

/*
 * get device name from config for dual radio or single radio device.
 * add by Tony 2008.2.29 -->
 */
//#define DUAL_RADIO (get_wifi_status() == ON24G_ON5G)
//#define sysname(syscfg) (DUAL_RADIO ? (syscfg)->cDualRadioDevName : (syscfg)->cSglRadioDevName)
/* <-- add by Tony 2008.2.29 */

/*get ethernet mac address*/
static void GetMacAddress(char* ifacename,char *pMacAddr)
{
    struct ifreq ifr;
    
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return;
    memset(&ifr, 0, sizeof(ifr));  
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFHWADDR,&ifr)>=0)
    {		       
    	memcpy(pMacAddr,ifr.ifr_hwaddr.sa_data,6);	
    }
    close(s);		
}

/*get IP address and IP net mask*/
static void GetIp_ipMask(char* ifacename,u32* ipaddr, u32* ipmask)
{
    struct ifreq ifr;
    struct sockaddr_in *pskin;
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return;
	
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFADDR,&ifr)>=0)
    {	
        pskin = (struct sockaddr_in*)&ifr.ifr_addr;
    	 *ipaddr =(pskin->sin_addr.s_addr );	
    }
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFNETMASK,&ifr)>=0)
    {	
        pskin = (struct sockaddr_in*)&ifr.ifr_addr;
    	*ipmask = (pskin->sin_addr.s_addr );	
    }

#ifdef PRE_IP_DEBUG
   printf("preipd: get ip:%08x  get ip net mask:%08x\n",*ipaddr,*ipmask);
#endif

    close(s);		
}


/* set IP address */
void setIpAddress (char *ifrname, u32 addr)
{
   struct ifreq ifr;
   struct sockaddr_in *pskin;
   int fd,err=0;
   u32 bdcst=0;

   if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
     return ;
   memset(&ifr, 0, sizeof(ifr));
   strcpy(ifr.ifr_name, ifrname);
   pskin = (struct sockaddr_in*)&ifr.ifr_addr;

#ifdef PRE_IP_DEBUG
   printf("preipd: set ip address:%08x\n",addr);
#endif
   
   pskin->sin_family = AF_INET;
   pskin->sin_addr.s_addr = htonl(addr);
   if((err = ioctl(fd,SIOCSIFADDR,&ifr)) < 0)
   {
     close(fd);
     return ;
   }

   /**
     If Ip Address is set to zero,
     no need to set netmask and broadcast address
     **/
   if(addr == 0)
   {
     close(fd);
     return;
   }
	   
}

/* set IP net mask*/
void setIpMask (char *ifrname, u32 mask)
{
   struct ifreq ifr;
   struct sockaddr_in *pskin;
   int fd,err=0;
   u32 bdcst=0;

   if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
     return ;
   memset(&ifr, 0, sizeof(ifr));
   strcpy(ifr.ifr_name, ifrname);
   pskin = (struct sockaddr_in*)&ifr.ifr_addr;

#ifdef PRE_IP_DEBUG
   printf("preipd: set ip net mask:%08x\n",mask);
#endif
   
   pskin->sin_family = AF_INET;
   pskin->sin_addr.s_addr = htonl(mask);
   if((err = ioctl(fd,SIOCSIFNETMASK,&ifr)) < 0)
   {
     close(fd);
     return;
   }

}


int preip_get_mac(u8 *mac)
{
    struct ifreq ifr;
    
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return T_FAILURE;
    memset(&ifr, 0, sizeof(ifr)); 
    
    strcpy(ifr.ifr_name, "eth0");
    if(ioctl(s,SIOCGIFHWADDR,&ifr)>=0)
    {		       
    	memcpy(mac,ifr.ifr_hwaddr.sa_data,6);	
    }
    close(s);
    
    return T_SUCCESS;
}

/*******************************************
	DEVICE NAME BEGIN
********************************************/

 /***********************************************************************
 * Function Name : sub_replace
 * Description    : funciton@special character replace with 
 *parameter: source src str,sub str,replace str
 * Input         : source, the source string
 *                    sub, sub string to be replace
 *                    rep, string used to replace the sub string
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/

char *sub_replace(char *source, char *sub, char *rep)
{
	char *result;
	char *pc1, *pc2, *pc3;
	int isource, isub, irep;
	isub = strlen(sub);
	irep = strlen(rep);
	isource = strlen(source);
	if (strlen(sub) == 0)	//sub[0]=='\0'; is empty str or not!
		return strdup(source);
	result = (char *)malloc(((irep > isub) ? (float)strlen(source) / isub * irep + 1 : isource) * sizeof(char));
	pc1 = result;
	while (*source != '\0') {
		pc2 = source;
		pc3 = sub;
		while (*pc2 == *pc3 && *pc3 != '\0' && *pc2 != '\0')
			pc2++, pc3++;
		if (strlen(pc3) == 0) {
			pc3 = rep;
			while (*pc3 != '\0')
				*pc1++ = *pc3++;
			pc2--;
			source = pc2;
		} else
			*pc1++ = *source;
		source++;
	}
	*pc1 = '\0';
	return result;
}

/*******************************************
 * Function Name : get_sysname_from_nvram
 * Description    : get system name from nvram
 * Input         :
 * Output        : sysname, device name
 * Return value  : T_SUCCESS, get success
 *                       T_FAILURE, get failure
 *********************************************/
int get_sysname_from_nvram(char *sysname)
{
	char *name_get=NULL;
	char *str=NULL;
    
	name_get = nvram_safe_get("hostname");

	if(strlen(name_get) <= 0)
	{
		printf("Get system name from nvram error!\n");
		return T_FAILURE;
	}
	
	strcpy(sysname, name_get);
	if(strstr(sysname,"\"")){
		str = sub_replace(sysname,"\"","&#34;");
		strcpy(sysname,str);
		free(str);
	}
	
	return T_SUCCESS;
}

int preip_get_deviceid(u8 *deviceid)
{
	char sysname[256]={0};

	if(!deviceid)
		return T_FAILURE;
	
	if(T_FAILURE == get_sysname_from_nvram(sysname))
	{
		return T_FAILURE;
	}

    strcpy(deviceid, sysname);

	printf("%s:%d deviceid: %s\n", __FUNCTION__, __LINE__, deviceid);
    
    return T_SUCCESS;
}

int preip_get_dhcp(u8 *dhcp)
{
	char buf_proto[32] = {0};

	if(!dhcp)
		return T_FAILURE;
	
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf_proto, 32, EZPLIB_USE_CLI);

	if (!strcmp(buf_proto, "dhcp"))
		*dhcp = 1; // enable
	else 
		*dhcp = 0; // disable

	printf("%s:%d dhcp: %d\n", __FUNCTION__, __LINE__, *dhcp);
    
    return T_SUCCESS;
}

int preip_get_ip(u8 *ip)
{
    char *value = NULL;
	char value2[32] = {0};
	char buf[32] = {0};
	//struct item_config item;

	if(!ip)
		T_FAILURE;
	
	//memset(&item, 0, sizeof(struct item_config));
	//get_value(CLI_NAME_DHCP, &item);
	ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	//if (!strcmp(item.params[0].value, "static")){
	if (!strcmp(value2, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 32, EZPLIB_USE_CLI);
		if (buf == NULL)
			goto IP_ERR;
		else{
			if(!strcmp(buf, ""))
				goto IP_ERR;
			//else
			//	strcpy(ip, buf);
		}
		//memset(&item, 0, sizeof(struct item_config));
		//get_value(CLI_NAME_IPADDR, &item);
		//if (cli_debug)
		//	printf("name: %s, param name: %s, param value: %s\n", item.name, item.params[0].param_name, item.params[0].value);
	}else{
		value = nvram_safe_get("lan0_ipaddr");
		if (value ==NULL)
			goto IP_ERR;
		else
		{
			if(!strcmp(value, ""))
				goto IP_ERR;
			else
				strcpy(buf, value);
		}
	}

	printf("%s:%d IP: %s\n", __FUNCTION__, __LINE__, buf);
	value = strtok(buf, ".");
	if(value)
		ip[0] = atoi(value);
	value = strtok(NULL, ".");
	if(value)
		ip[1] = atoi(value);
	value = strtok(NULL, ".");
	if(value)
		ip[2] = atoi(value);
	value = strtok(NULL, ".");
	if(value)
		ip[3] = atoi(value);
	
    return T_SUCCESS;

	IP_ERR:
		ip[0] = 0;
		ip[1] = 0;
		ip[2] = 0;
		ip[3] = 0;
	return T_FAILURE;
}

int preip_get_netmask(u8 *netmask)
{

    char *value = NULL;
	char value2[32] = {0};
	char buf[32];
	//struct item_config item;

	if(!netmask)
		return T_FAILURE;

	ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	//memset(&item, 0, sizeof(struct item_config));
	//get_value(CLI_NAME_DHCP, &item);
	//if (!strcmp(item.params[0].value, "static")){
	if (!strcmp(value2, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, 32, EZPLIB_USE_CLI);
		if (buf == NULL)
			goto ERR_NETMASK;
		else{
			if(!strcmp(buf, ""))
				goto ERR_NETMASK;
			else
				*netmask = atoi(buf);
		}
	}else{
		value = nvram_safe_get("lan0_mask");
		if (value ==NULL)
			goto ERR_NETMASK;
		else
		{
			if(!strcmp(value, ""))
				goto ERR_NETMASK;
			else
				*netmask = atoi(value);
		}
	}

	printf("%s:%d netmask: %d\n", __FUNCTION__, __LINE__, *netmask);
    
    return T_SUCCESS;

	ERR_NETMASK:
		*netmask = 0;
		return T_FAILURE;
}

int preip_get_essid(u8 *essid)
{
	char buf[64] = {0};

	if(!essid)
		return T_FAILURE;

	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "")){
		strcpy(essid, "");
	}else{
		strcpy(essid, buf);
	}

	printf("%s:%d essid: %s\n", __FUNCTION__, __LINE__, essid);
    
    return T_SUCCESS;
}

int preip_get_rssithr(u8 *rssithr_conn, u8 *rssithr_disconn)
{
	char buf_1[32] = {0};
	char buf_2[32] = {0};

	if(!rssithr_conn || !rssithr_disconn)
		return T_FAILURE;

	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf_1, 32, EZPLIB_USE_CLI);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf_2, 32, EZPLIB_USE_CLI);

    *rssithr_conn = -(atoi(&buf_1[1]));
    *rssithr_disconn = -(atoi(&buf_2[1]));

	printf("%s:%d rssithr_conn: %d, rssithr_disconn: %d\n", __FUNCTION__, __LINE__, (char)*rssithr_conn, (char)*rssithr_disconn);
    
    return T_SUCCESS;
}

/*******************asso_status*******************************/
#define RADIO_2G 0
int getAPConnectStatus(int radio, int *associated)
{
	int ret = 0;
	int assotmp = 0;
	char iOpMode[32] = {0};
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, sizeof(iOpMode), EZPLIB_USE_CLI);
	}	
	if (!strcmp(iOpMode, "client")) {
		printf("%s:%d, iOpMode = %s\n", __FUNCTION__, __LINE__, iOpMode);
		ret = get_sta_assoc_status(radio, &assotmp);
		printf("%s:%d, assotmp = %d\n", __FUNCTION__, __LINE__, assotmp);
		if (T_FAILURE == ret) {
			*associated = 2;
		} else {
			*associated = assotmp;
		}
	} else {
		*associated = 3;
	}

	return T_SUCCESS;
}

int preip_get_asso_status(u8 *asso_status)
{
	int associated = 999;

	if(!asso_status)
		return T_FAILURE;
	
	//0->Disassociated, 1->Associated, 2->Unkown, 3-> --
	getAPConnectStatus(RADIO_2G, &associated);
	printf("%s:%d associated: %d\n", __FUNCTION__, __LINE__, associated);
	switch(associated) {
		case 1:
			*asso_status = 1;
			break;
		case 0:
			*asso_status = 0;
			break;
		default:
			*asso_status = 0;
			
	}
    
    return T_SUCCESS;
}

int preip_get_rssi(u8 *rssi)
{
	int rssi1 = 0, rssi2 = 0;
	char TempBuf[32] = {0};
	int ret = 0;

	if(!rssi)
		return T_FAILURE;

	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);

	if (!strcmp(TempBuf, "client")) {
		//param: 0, means first radio
		ret = get_sta_assoc_rssi_for_preip(RADIO_2G, &rssi1, &rssi2);
		if (ret != 0) {
			*rssi = -200; // error rssi for
		} else {
			//strcpy(rssi, (u8 *)rssi_arr);
			*rssi = rssi1 > rssi2 ? rssi1:rssi2;
		}
	}

	printf("%s:%d rssi:%d, rssi1: %d, rssi2: %d\n", __FUNCTION__, __LINE__, (char)*rssi, rssi1, rssi2);

    return T_SUCCESS;
}

int preip_get_rssi_per_chain(u8 *rssi_per_chain)
{
	int rssi1 = 0, rssi2 = 0;
	char TempBuf[32] = {0};
	int ret = 0;

	if(!rssi_per_chain)
		return T_FAILURE;

	rssi_per_chain[0]=(u8)-200;
    rssi_per_chain[1]=(u8)-200;
    rssi_per_chain[2]=0;

	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);

	if (!strcmp(TempBuf, "client")) {
		//param: 0, means first radio
		ret = get_sta_assoc_rssi_for_preip(RADIO_2G, &rssi1, &rssi2);
		if (ret != 0) {
			rssi_per_chain[0] = (u8)-200; // chain0 error rssi
			rssi_per_chain[1] = (u8)-200; // chain1 error rssi for
			rssi_per_chain[2]=0;
		} else {
			rssi_per_chain[0]=(u8)rssi1;
    		rssi_per_chain[1]=(u8)rssi2;
    		rssi_per_chain[2]=0;
		}
	}	

	printf("%s:%d rssi1: %d, rssi2: %d\n", __FUNCTION__, __LINE__, rssi1, rssi2);

    return T_SUCCESS;
}

int preip_get_security(u8 *security)
{
	char auth_mode[32] = {0};
    char wpa_auth_type[32]={0};
	char enc_type[32] = {0};
	preip_wifi_security_t *preip_sec = NULL;

	if(!security)
		return T_FAILURE;
	
	preip_sec = (preip_wifi_security_t *)security;

	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", auth_mode, 32, EZPLIB_USE_CLI);
	
	if (!strcmp(auth_mode, "none")) {
		//strcpy(security, "open");
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_OPEN; //open
	}
	else if (!strcmp(auth_mode, "disabled")) {
		//strcpy(security, "open");
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_OPEN; //open
	}
	else if (!strcmp(auth_mode, "psk")) {
		//ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", enc_type, 32, EZPLIB_USE_CLI);
		//strcpy(security, "wpa-psk");
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPAPSK; //wpa-psk
	} 
	else if (!strcmp(auth_mode, "psk2")) {
		//ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", enc_type, 32, EZPLIB_USE_CLI);
        //strcpy(security, "wpa2-psk");
        preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA2PSK; //wpa2-psk
	}
    else if (!strcmp(auth_mode, "wpa")) {
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth_type, 32, EZPLIB_USE_CLI);
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA; //WPA
		
        if(*wpa_auth_type == '0')
        {
            //strcpy(security, "wpa ttls");
            preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_TTLS; //wpa ttls
        }else
        {
            //strcpy(security, "wpa peap");
			preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_PEAP; //wpa peap
        }
	}
    else if (!strcmp(auth_mode, "wpa2")) {
        ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth_type, 32, EZPLIB_USE_CLI);
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA2; //WPA2

        if(*wpa_auth_type == '0')
        {
            //strcpy(security, "wpa2 ttls");
            preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_TTLS; //wpa ttls
        }else
        {
            //strcpy(security, "wpa2 peap");
            preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_PEAP; //wpa peap
        }
	}
    else if (!strcmp(auth_mode, "wep")) {
		//strcpy(security, "wep");
		preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WEP;
	}
	else {
		//strcpy(security, "Invalid security type!");
		preip_sec->authmode = 99;
	}

	printf("%s:%d security: %d\n", __FUNCTION__, __LINE__, (char)preip_sec->authmode);

    return T_SUCCESS;
}

 /***********************************************************************
 * Function Name : check_systemname
 * Description    : check if the charactors of system name are leagul
 * Input         : str, system name to be checked
 * Output        : 
 * Return value  : T_TRUE, ckeck un-passed
 *                       T_FALSE, check passed
 ***********************************************************************/
int check_systemname(char *str)
{
	int i;
	int str_len=0;
	str_len = strlen(str);
	for (i=0; i<str_len; i++) {
		if ( (str[i] >= '0' && str[i] <= '9') ||
				(str[i] >= 'a' && str[i] <= 'z') ||
				(str[i] >= 'A' && str[i] <= 'Z') ||
				(str[i] == '_') || (str[i] == '-'))
			continue;
		else
			return T_FALSE;
	}
	return T_TRUE;
}

int preip_set_deviceid(u8 *deviceid)
{
	char sys_name[33]={0} ;
	char *token_get = NULL;
	int token_len = 0;
	struct item_config item;

    printf("preip_set_deviceid: %s\n", deviceid);
	memset(&item, 0, sizeof(struct item_config));

	if ( NULL == deviceid )
		return T_FAILURE;

	token_len = strlen(deviceid);
	if(token_len > 32)
	{
		printf("Device name length can not be more than 32 charactors\n");
		return T_FAILURE;
	}
	else if(token_len < 0)
	{
		printf("Device name can not be empty\n");
		return T_FAILURE;
	}
	strcpy(sys_name, token_get);
	if(T_FALSE == check_systemname(sys_name))
	{
		printf("The only valid characters for a system name are letters numbers and a hyphen -_\n");
		return T_FAILURE;
	}
	//nvram_set("hostname", sys_name);
	strcpy(item.name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].param_name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].value, sys_name);

	set_value(CLI_NAME_DEVICEID, &item);

    return T_SUCCESS;
}

int preip_set_dhcp(u8 dhcp)
{
	struct item_config item;
	char *p_dhcp = NULL;
	
    printf("preip_set_dhcp: %s\n", dhcp ? "enabled" : "disabled");

	memset(&item, 0, sizeof(struct item_config));

	p_dhcp = dhcp ? "enable" : "disable";

	strcpy(item.name, "dhcp");
	strcpy(item.params[0].param_name, "dhcp");

	if (!strcmp(dhcp, "enable")) {
		strcpy(item.params[0].value, "dhcp");
		set_value("dhcp", &item);
	} else if (!strcmp(dhcp, "disable")){
		strcpy(item.params[0].value, "static");
		set_value("dhcp",&item);
	} else {
		printf("Invalid value\n");
	}

    return T_SUCCESS;
}

/***********************************************************************
* Function Name : verifyIP
* Description    : verify the ip
* Input         : 
* Return value  : legal IP
***********************************************************************/
int verifyIP(char *IpAddr)
{
	if (NULL == IpAddr)
	{
		return T_TRUE;
	}
	return (INADDR_NONE != inet_addr(IpAddr));
}

 
/***********************************************************************
* Function Name : IsValidIpAddress
* Description    : check if the ip address is legal
* Input		  : IpAddr, ip address
* Output		  :
* Return value  : T_TRUE, valid ip address
* 					  T_FALSE, illegal ip address
***********************************************************************/
int IsValidIpAddress(char *IpAddr)
{
 T_INT32 ii,kk;

 for (ii = 24; ii >= 0; ii -= 8)
 {
	 /* Skip any leading stuff (e.g., spaces, '[') */
	 while (IpAddr != '\0' && !isdigit(*IpAddr))
		 IpAddr++;
	 kk =(T_INT32)atoi(IpAddr);
	 if (kk < 0 || 255 < kk)
		 return T_FALSE;
	 if ( ((IpAddr = strchr(IpAddr, '.')) == NULL) && (ii > 0) )
		 return T_FALSE;
	 IpAddr++;
  }
  return T_TRUE;
}

int preip_set_ip(u8 *ip)
{
	char *pIP = NULL;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item;

	printf("preip_set_ip: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
	pIP = ip;

	if ( verifyIP(pIP) == 0 ) {
		printf("Invalid IP Address\n");
		return T_FAILURE;
	}

	if ( IsValidIpAddress(pIP) ) {
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_DHCP, &item);

		if (!strcmp(item.params[0].value, "static")) {
			memset(&item, 0, sizeof(struct item_config));
			strcpy(item.name, CLI_NAME_IPADDR);
			strcpy(item.params[0].param_name, CLI_NAME_IPADDR);
			strcpy(item.params[0].value, pIP);
			set_value(CLI_NAME_IPADDR, &item);
		}
		else{
			printf("Can not modify IP address when DHCP client enabled\n");
			
			return T_FAILURE;
		}
		
		return T_SUCCESS;
	} else {
		printf("Invalid IP Address\n");
	}

	return T_SUCCESS;
}

  /***********************************************************************
 * Function Name : is_integer
 * Description    : check if a string are digital
 * Input         : str, system name to be checked
 * Output        : 
 * Return value  : T_TRUE, ckeck un-passed
 *                       T_FALSE, check passed
 ***********************************************************************/
int is_integer(char *str)
{
	int i;
	int str_len=0;
	str_len = strlen(str);
	for (i=0; i<str_len; i++) {
		if ( str[i] >= '0' && str[i] <= '9')
			continue;
		else
			return T_FALSE;
	}
	return T_TRUE;
}

int preip_set_netmask(u8 netmask)
{
 	char *net_mask;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item = {0};

	printf("preip_set_netmask: %d\n", netmask);
	memset(&item, 0, sizeof(struct item_config));

	if ( is_integer(net_mask) && atoi(net_mask) > 0 && atoi(net_mask) <= 32) {
		//ezplib_get_attr_val("lan0_proto", 0, "curr", value, 32, EZPLIB_USE_CLI);
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_DHCP, &item);

		if (!strcmp(item.params[0].value, "static")) {
			strcpy(item.name, CLI_NAME_NETMASK);
			strcpy(item.params[0].param_name, CLI_NAME_NETMASK);
			strcpy(item.params[0].value, net_mask);
			set_value(CLI_NAME_NETMASK, &item);
		}
		else{
			printf("Can not modify Net Mask when DHCP client enabled\n");
			
			return T_FAILURE;
		}
		
		return T_SUCCESS;
	} else {
		printf("Invalid Net Mask\n");
		
		return T_FAILURE;
	}
	
    return T_SUCCESS;
}

int preip_set_essid(u8 *essid)
{
	char *ssid = NULL;
	char buf[256] = {0};
	int total_params = 0;
	struct item_config item;

	printf("preip_set_essid: %s\n", essid);
	ssid = essid;
	if ( !ssid || !strlen(ssid))
		return T_FAILURE;

#define SSID_MIN_LEN 1
#define SSID_MAX_MAX 32
	if ( strlen(ssid) > SSID_MAX_MAX || strlen(ssid) < SSID_MIN_LEN) {
		printf("Length of ssid from 1 to 32\n");
	}
#undef SSID_MIN_LEN
#undef SSID_MAX_MAX

	//ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", ssid);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_SSID);
	strcpy(item.params[0].param_name, CLI_NAME_SSID);
	strcpy(item.params[0].value, ssid);
	set_value(CLI_NAME_SSID, &item);
	//nvram_commit();

    return T_SUCCESS;
}

int preip_set_rssithr(u8 rssithr_conn, u8 rssithr_disconn)
{
	int disconn_rssi = 9999, conn_rssi = 9999;
	char buf[32] = {0};
	struct item_config item;

	printf("preip_set_rssithr: %d %d\n", (char)rssithr_conn, (char)rssithr_disconn);

	disconn_rssi = atoi(rssithr_disconn);
	conn_rssi = atoi(rssithr_conn);

	#define CONN_RSSI_MAX -45
	#define CONN_RSSI_MIN -85
	if (conn_rssi > CONN_RSSI_MAX || conn_rssi < CONN_RSSI_MIN) {
		printf("Connrssithr must be from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
		return T_FAILURE;
	}
	#undef CONN_RSSI_MAX
	#undef CONN_RSSI_MIN

	#define DISCONN_RSSI_MAX -55
	#define DiSCONN_RSSI_MIN -95
	if (disconn_rssi > DISCONN_RSSI_MAX || disconn_rssi < DiSCONN_RSSI_MIN) {
		printf("Disconnrssi must be from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
		return T_FAILURE;
	}

	#undef DISCONN_RSSI_MAX
	#undef DiSCONN_RSSI_MIN

	if ((conn_rssi - disconn_rssi) < 10) {
		printf("Connrssithr must be greater than disconn_value 10\n");
		return T_FAILURE;
	}

	//connect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].param_name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].value, rssithr_conn);
	set_value(CLI_NAME_CONNRSSIHR, &item);

	//disconnect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].value, rssithr_disconn);
	set_value(CLI_NAME_DISCONNRSSITHR, &item);

	return T_SUCCESS;
}

int preip_set_security(u8 *security)
{
	int len_psk;
	char *option_m=NULL;
	char *option_2=NULL;
	char *option_3=NULL;
	char *option_4=NULL;
	struct item_config item;
	
	memset(&item, 0, sizeof(struct item_config));
	printf("preip_set_security\n");

	#if 0
	if(NULL == security)
		return T_FAILURE;

   if(option_m = tokenPop(pCli)){
		if (strcmp(option_m, "open")==0){
			if(argc != 1)
			{
				goto security_usage;
			}
			
			strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
			strcpy(item.params[0].value,  "disabled");
		}else if(strcmp(option_m, "wpa-psk")==0 || strcmp(option_m, "wpa2-psk")==0){
			if(argc != 2)
			{
				goto security_usage;
			}9P}
		   //pre-shared key
			option_2 = tokenPop(pCli);
		   
			len_psk=strlen(option_2);
			if(len_psk < 8 || len_psk > 64){
				printf("The length of pre-shared key should be from 8 to 64\n");

				return T_FAILURE;
			}

			/* should be hex digital*/
			if(len_psk == 64 && is_hex_digital_str(option_2)!= T_TRUE){
				printf("The pre-shared key should be hex when length is 64 bytes\n");

				return T_FAILURE;
			}

			strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
			if(strcmp(option_m, "wpa-psk")==0){
				strcpy(item.params[0].value,"psk"); /* wpa-psk*/
			}else{
				strcpy(item.params[0].value,"psk2"); /* wpa2-psk*/
			}
			
			strcpy(item.params[1].param_name, SECURITY_KEY);
			strcpy(item.params[1].value, option_2); /* key*/
			
		}else if(strcmp(option_m, "wpa")==0 || strcmp(option_m, "wpa2")==0){
			if(argc != 4)
			{
				goto security_usage;
			}

			option_2 = tokenPop(pCli);

			if(!(strcmp(option_2, "peap")==0 || strcmp(option_2, "ttls")==0))
			{
				goto security_usage;
			}

			option_3 = tokenPop(pCli);

			if((strlen(option_3) > 64))
			{
				printf("The length user name should no more than 64\n");

				return CLI_PARSE_NOMESSAGE;
			}

			option_4 = tokenPop(pCli);

			if((strlen(option_4) > 64))
			{
				printf("The length password should no more than 64\n");

				return CLI_PARSE_NOMESSAGE;
			}

			strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
			strcpy(item.params[0].value, option_m); /* wpa/wpa2*/

			strcpy(item.params[1].param_name, SECURITY_8021X_AUTH_TYPE);
			if(strcmp(option_2, "ttls")==0)
			{
				strcpy(item.params[1].value, "0"); /* ttls*/
			}else if(strcmp(option_2, "peap")==0)
			{
				strcpy(item.params[1].value, "1"); /* peap*/
			}

			strcpy(item.params[2].param_name, SECURITY_8021X_AUTH_USERNAME);
			strcpy(item.params[2].value, option_3); /* user*/

			strcpy(item.params[3].param_name, SECURITY_8021X_AUTH_PASSWORD);
			strcpy(item.params[3].value, option_4); /* password*/
			
		}
		else if (strcmp(option_m, "wep")==0){
			printf("wep isn't supported at command line\n");
		}else{
			printf("Invalid command\n");
		}
   }

	set_value(CLI_NAME_SECURITY, &item);

	#endif

	return T_SUCCESS;
}

void preip_set_apply_save(void)
{

	char cmd[128] = {0};
	char *sys_name = NULL;
	char *connrssi = NULL;
	char *disconnrssi = NULL;
	char buf[32] = {0};
	char br_name[32]={0}, *br_get = NULL;
	char buf_proto[32] = {0};

	printf("preip_set_apply_save\n");

	//write to nvram
	write_to_nvram();

	//save to flash forever
	nvram_commit();

	//restart network
	if (set_config_sta(RADIO_2G)) {
		printf("set_config_sta error!\n");
	}

	//set system name
	sys_name = nvram_safe_get("hostname");
	sprintf(cmd, "echo %s > /proc/sys/kernel/hostname",sys_name);
	system(cmd);

	//connectrssi and disconnectrssi
	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, 32, EZPLIB_USE_CLI);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 connrssi %d", (atoi(buf) + 95));
	//printf("%s\n", cmd);
	system(cmd);

	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, 32, EZPLIB_USE_CLI);
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 disconnrssi %d", (atoi(buf) + 95));
	//printf("%s\n", cmd);
	system(cmd);

	//set ipaddr
	system("/etc/init.d/network-lan restart 2>/dev/null 1>/dev/null &");

    return T_SUCCESS;
}

/*daemon init function*/
static int Daemon_Init(void)
{
  int devnullfd;
  chdir ("/");
  switch (fork() )
  {
    case -1:
      printf("Daemon_Init: error in fork\r\n");
      exit(1);
    case 0: /** Child **/
      break;
    default:
      /** Parent **/
      exit(0);
  }
  if ( setsid() == -1 )
  {
    printf("Daemon_Init: setsid error\r\n");   
    exit(1);
  }


  devnullfd = open("/dev/null", O_RDWR);
  if ( devnullfd == -1 )
  {
    printf("Null device open failed\r\n"); 
    return(1);
  }
  close(0);
  close(1);
  close(2);

  dup2(devnullfd, 0);
  dup2(devnullfd, 1);
  dup2(devnullfd, 2);
  close(devnullfd);
  return(0);
}

	
/*gcc-3.4.4-2.16.1 no define of NLMSG_HDRLEN*/
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))

/*user call kernel to setup vlan bridge filter by netlink socket*/
T_INT32 preipSend(int skfd,int type,char *buf, int len)
{
    
    struct sockaddr_nl src_local;
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr *nlh = NULL;
    struct nlmsghdr rcvNlh;
    fd_set rfds;
    struct timeval tv;
    int errorNo = 0;
	
    printf("preipSend: type=%d, len=%d\n", type, len);
    
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));
	
    if(nlh == NULL){
	 errorNo = 1;
        printf("%s,nlh malloc error\n",__func__);
        goto malloc_error;
    }
	
    memset(nlh, 0, sizeof(struct nlmsghdr));

    memset(&src_local, 0, sizeof(struct sockaddr_nl));
    src_local.nl_family = AF_NETLINK;     
    src_local.nl_pid = getpid(); 
    src_local.nl_groups = 0;  /* not in mcast groups */
    if(bind(skfd, (struct sockaddr *)&src_local,sizeof(struct sockaddr_nl)) != 0){
	 errorNo = 2;
        printf("%s,bind error\n",__func__);
        goto error_return;
    }

    memset(&dest_kernel, 0, sizeof(struct sockaddr_nl));
    dest_kernel.nl_family = AF_NETLINK;
    dest_kernel.nl_pid = 0;   /* For Linux Kernel */
    dest_kernel.nl_groups = 0; 

	
    /* Fill the netlink message header */
    //nlh->nlmsg_flags = NLM_F_ACK; 	/*request kernel response*/
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct  nl_preip_info));
    nlh->nlmsg_pid = getpid();  /* self pid */
    ((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_type = type;
    if(type != NLI_TYPE_HELLO)
        memcpy(((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_buf,buf,len);

    sendto(skfd,(char*)nlh,nlh->nlmsg_len,0,(struct sockaddr*)&dest_kernel,sizeof(dest_kernel));
normal_return:
    printf("%s return ok\n",__func__);
    free(nlh);
    return 0;
error_return:
    free(nlh);
malloc_error:
    printf("%s return error, error no:%d\n",__func__,errorNo);
    return -1;
}


T_INT32 preipWaiting(int skfd,char *buf, int len)
{
    
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr* rcvNlh;
    fd_set rfds;
    int errorNo = 0;
    int ret;

    printf("preipWaiting: entry\n");
  
    FD_ZERO(&rfds);
    FD_SET(skfd, &rfds);
	
    memset(&dest_kernel, 0, sizeof(struct sockaddr_nl));
    dest_kernel.nl_family = AF_NETLINK;
    dest_kernel.nl_pid = 0;   /* For Linux Kernel */
    dest_kernel.nl_groups = 0; 
    rcvNlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));

    dest_kernel_len = sizeof(dest_kernel);
    if (select(skfd+1, &rfds, NULL, NULL, NULL) > 0){
        printf("%s,%d: select\n", __func__, __LINE__);
        if ((ret=recvfrom(skfd, rcvNlh,NLMSG_LENGTH(sizeof(struct nl_preip_info)),0,(struct sockaddr*)&dest_kernel,&dest_kernel_len)) > 0){
            printf("%s,%d: recvfrom ret=%d\n", __func__, __LINE__, ret);
             if(((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_type == NLI_TYPE_RUN){
                 printf("%s,%d: len=%d\n", __func__, __LINE__, len);
                 memcpy(buf,((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_buf,len);
        	  goto normal_return;/*have ack response*/
    	    }else{
    	        errorNo = 1;
    	    }
        }else{
            errorNo = 2;
        }
    }else
        errorNo = 3;
            	
    printf("%s return error, error no:%d\n",__func__,errorNo);
    free(rcvNlh);
    return -1;
normal_return:
    printf("%s return ok\n",__func__);
    free(rcvNlh);
    return ret;
}

#if 0
/* discovery response packet*/
typedef struct Preip_dscv_resp
{
    u8 mac[6];             /* device mac*/
    u8 deviceid[33];       /* device id*/
    u8 dhcp;               /* 0: disabled; 1: enabled. */
    u8 ip[4];              /* static ip address or dynamic ip from server*/
    u8 netmask;            /* netmask, bit number*/
    u8 essid[33];          /* essid */
    u8 lock_to_bssid[6];   /* lock to bssid */
    u8 country_code;       /* country code*/
    u8 channel_list[32];   /* supported channel list*/
    u8 rssithr_conn;       /* connect rssi threshold */
    u8 rssithr_disconn;    /* disconnect rssi threshold */
    u8 tx_power;           /* 0: full; -1: -1dB; ...; -9: -9dB; -10: min */
    u8 security[128];      /* security */
    u8 asso_status;        /* 0: Disassociated; 1: Associated*/
    u8 bssid[6];           /* associated bssid */
    u8 channel;            /* 36, 40, ..., 165*/
    u8 bandwidth;          /* 0: 20MHz; 1: 40MHz; 2: 80MHz; 3: 160MHz*/
    u8 ieee80211mode;      /* 0: 11bgn; 1: 11bg; 2:11b; 3:11an; 4:11a; 5: 11ac*/
    u8 rssi;               /* rssi */
    u8 rssi_per_chain[3];  /* rssi for ch0, ch1, ch2 (0 for 2x2 device)*/
} Preip_dscv_resp_t;
#endif


int preip_process_discovery(int skfd, PreipFramInfo_t *frame_info)
{
    int ret;
    Preip_dscv_resp_t *p_resp = &frame_info->preip_all.preip_data_buf.resp_data;

    frame_info->preip_all.preip_hdr.id = PREIP_ID_DISCOVERY_RESP;

    preip_get_mac(p_resp->mac);
    preip_get_deviceid(p_resp->deviceid);
    preip_get_dhcp(&p_resp->dhcp);
    preip_get_ip(p_resp->ip);
    preip_get_netmask(&p_resp->netmask);
    preip_get_essid(p_resp->essid);
    preip_get_rssithr(&p_resp->rssithr_conn, &p_resp->rssithr_disconn);
    preip_get_asso_status(&p_resp->asso_status);
    preip_get_rssi(&p_resp->rssi);
    preip_get_rssi_per_chain(p_resp->rssi_per_chain);
    preip_get_security(p_resp->security);

	printf("%s:%d mac:%s, deviceid:%s, dhcp:%d, ip:%s, netmask:%d, essid:%s, \
		connRssi:%d, disconnRssi:%d, rssi:%d \n", __FUNCTION__, __LINE__,p_resp->mac, 
		p_resp->deviceid, p_resp->dhcp, p_resp->ip, p_resp->netmask, p_resp->essid,
		p_resp->rssithr_conn, p_resp->rssithr_disconn, p_resp->rssi);

    ret = preipSend(skfd, NLI_TYPE_RUN, (char *)frame_info, sizeof(PreipFramInfo_t));

    if(ret < 0)
    {
        printf("preipSend error\n");

        return -1;
    }

    return 0;
}


int preip_process_set(PreipFramInfo_t *frame_info)
{
    Preip_set_item_t *p_item;

    p_item = &frame_info->preip_all.preip_data_buf.set_item_data;

    switch(p_item->item_id)
    {
    case PREIP_ID_SET_DHCP:
        preip_set_dhcp(p_item->item.dhcp);
        break;
    case PREIP_ID_SET_IP:
        preip_set_ip(p_item->item.ip);
        break;
    case PREIP_ID_SET_MASK:
        preip_set_netmask(p_item->item.mask);
        break;
    case PREIP_ID_SET_ESSID:
        preip_set_essid(p_item->item.essid);
        break;
    case PREIP_ID_SET_RSSITHR:
        preip_set_rssithr(p_item->item.rssi_threshold[0],p_item->item.rssi_threshold[1]);
        break;
    case PREIP_ID_SET_DEVID:
        preip_set_deviceid(p_item->item.deviceid);
        break;
    case PREIP_ID_SET_SECURITY:
        preip_set_security(p_item->item.security);
        break;
    case PREIP_ID_SET_APPLY_SAVE:
        preip_set_apply_save();
        break;
    default:
        printf("unsupported set item id %x\n", p_item->item_id);
        break;
    }

    return 0;
}

/*preip daemon main function*/
int main(int argc, char *argv[])
{
    int readlen = 0;
    char readBuf[PREIP_MSG_SIZE];
    char writeBuf[PREIP_MSG_SIZE];
    char ifname[16];
    char cmd[150];  /* enlarge the length of cmd and prevent stack overflow! bug1275 by roger 2008-5-14*/
    int skfd;
    PreipFramInfo_t frame_info;

#ifdef PRE_IP_DEBUG
    printf("preipd start...\n");
    sleep(1);
#endif
    //Daemon_Init();	
    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PREIP);
    if(skfd < 0){
        printf("%s,sock error\n",__func__);
        return 0;
    }
    preipSend(skfd,NLI_TYPE_HELLO,NULL,0);

	//load config to ram
	init_global_config();

	//print config
	debug_global_config();
	
    while(1)
    {
        memset((u8 *)&frame_info, 0x0, sizeof(frame_info));

        readlen = preipWaiting(skfd,(char *)&frame_info, sizeof(frame_info));

#ifdef PRE_IP_DEBUG
        printf("preipd: read length:%d \n",readlen);
#endif

        if (readlen < 0)
            continue;

    	if (readlen < sizeof(frame_info))
        {
            printf("length < sizeof(frame_info)\n");
            continue;
        }
         
        switch(frame_info.preip_all.preip_hdr.id)
        {
        case PREIP_ID_DISCOVERY:
            printf("discovery request\n");
            preip_process_discovery(skfd, &frame_info);
            break;
        case PREIP_ID_SET_ITEM:
            printf("set item\n");
            preip_process_set(&frame_info);
            break;
        default:
            break;
        }
    
    }

	//free config
	free_global_config();

    return 0;
}

