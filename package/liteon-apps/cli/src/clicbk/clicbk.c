 /******************************************************************************
* Copyright (C), 2014, LiteOn Technology Corp.
* Filename: clicbk.c
* Description: CLI Handler functions
* Created by: Mario Huang
* Revision history:
* Revision  Date               Author                    Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2014/05/05     Mario Huang              Create this file
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/time.h>

#include "cli.h"
#include "clicbk.h"
#include "clicbklib.h"
//#include "cli_sntp.h"
#include "readline.h"
#include "ethread.h"
#include "mid_detail.h"
#include "nvram.h"
#include "nvram_rule.h"

#define LONG_BUF_LEN			8192
#define SHORT_BUF_LEN			32	

extern T_BOOL8 g_su;
extern T_INT32 is_radio_exist(T_INT32 idx);
extern T_VOID change_encryption_for_11n(T_UINT8 index);
#define PROFILE (PROFILE_INDEX(pCli->unit, pCli->vap))

extern T_BOOL8 cli_debug;
extern int set_config_sta(int radio);

void init_global_config()
{
	int i = 0;
	char buf[128] = {0};
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
		uiPrintf("malloc fail\n");
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
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(ipaddr->name, CLI_NAME_IPADDR);
	strcpy(ipaddr->params[0].param_name, CLI_NAME_IPADDR);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "static")){
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 128, EZPLIB_USE_CLI);
		strcpy(ipaddr->params[0].value, buf);
	} else {
		value = nvram_safe_get("lan0_ipaddr");
		if (value && (strlen(value) > 0) )
			strcpy(ipaddr->params[0].value, value);
	}
	global_configs[CLI_INDEX_IPADDR].item = ipaddr;
	global_configs[CLI_INDEX_IPADDR].item_index = CLI_INDEX_IPADDR;

	//3. netmask
	struct item_config *netmask = (struct item_config *)malloc(sizeof(struct item_config));
	if (!netmask) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(netmask->name, CLI_NAME_NETMASK);
	strcpy(netmask->params[0].param_name, CLI_NAME_NETMASK);
	memset(buf, 0, sizeof(buf));
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "static")){
		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, 128, EZPLIB_USE_CLI);
		if (strcmp(buf, ""))
			strcpy(netmask->params[0].value, buf);
	}else{
		value = nvram_safe_get("lan0_mask");
		if (value)
			strcpy(netmask->params[0].value, value);
	}
	global_configs[CLI_INDEX_NETMASK].item = netmask;
	global_configs[CLI_INDEX_NETMASK].item_index = CLI_INDEX_NETMASK;

	//4. connect rssi thr
	struct item_config *conn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!conn_rssi_thr) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(conn_rssi_thr->name, CLI_NAME_CONNRSSIHR);
	strcpy(conn_rssi_thr->params[0].param_name, CLI_NAME_CONNRSSIHR);
	value = nvram_safe_get("connectrssithr");
	if (value && strcmp(value, ""))
		strcpy(conn_rssi_thr->params[0].value, value);
	global_configs[CLI_INDEX_CONNRSSITHR].item = conn_rssi_thr;
	global_configs[CLI_INDEX_CONNRSSITHR].item_index = CLI_INDEX_CONNRSSITHR;

    //5. disconnect rssi thr
	struct item_config *disconn_rssi_thr = (struct item_config *)malloc(sizeof(struct item_config));
	if (!disconn_rssi_thr) {
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(disconn_rssi_thr->name, CLI_NAME_DISCONNRSSITHR);
	strcpy(disconn_rssi_thr->params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	value = nvram_safe_get("disconnectrssithr");
	if (value && strcmp(value, ""))
		strcpy(disconn_rssi_thr->params[0].value, value);
	global_configs[CLI_INDEX_DISCONNRSSITHR].item = disconn_rssi_thr;
	global_configs[CLI_INDEX_DISCONNRSSITHR].item_index = CLI_INDEX_DISCONNRSSITHR;

	//6. deviceid
	struct item_config *deviceid = (struct item_config *)malloc(sizeof(struct item_config));
	if (!deviceid) {
		uiPrintf("malloc fail\n");
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
		uiPrintf("malloc fail\n");
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
		uiPrintf("malloc fail\n");
		exit(0);
	}
	strcpy(security->name, CLI_NAME_SECURITY);
	strcpy(security->params[0].param_name, SECURITY_AUTH_MODE);
	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", buf, 128, EZPLIB_USE_CLI);
	if (strcmp(buf, ""))
		strcpy(security->params[0].value, buf);
	memset(buf, 0, sizeof(buf));
	strcpy(security->params[1].param_name, SECURITY_ENC_TYPE);
	ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", buf, 128, EZPLIB_USE_CLI);
	strcpy(security->params[1].value, buf);
	
	global_configs[CLI_INDEX_SECURITY].item = security;
	global_configs[CLI_INDEX_SECURITY].item_index = CLI_INDEX_SECURITY;

	//9. dhcp
	struct item_config *dhcp = (struct item_config *) malloc(sizeof(struct item_config));
	if (!dhcp) {
		uiPrintf("malloc fail\n");
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
				for (j = 0; j < 3; j++) {
					struct param_pair *pair = NULL;
					pair = &(item->params[j]);

					if (!pair)
						continue;

					if (!strcmp(pair->param_name, SECURITY_AUTH_MODE)) {
						ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", pair->value);
					} 
					else if (!strcmp(pair->param_name, SECURITY_ENC_TYPE)) {
						ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", pair->value);
					}
					else if (!strcmp(pair->param_name, SECURITY_key)) {
						ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", pair->value);
					}
				}
				
				break;
			case CLI_INDEX_NETMASK:
				if (!strcmp(item->params[0].param_name, CLI_NAME_NETMASK)) {
					nvram_set("lan0_mask", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "mask", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_IPADDR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_IPADDR)) {
					nvram_set("lan0_ipaddr", item->params[0].value);
					ezplib_replace_attr("lan_static_rule", 0, "ipaddr", item->params[0].value);
				}
				
				break;
			case CLI_INDEX_DISCONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DISCONNRSSITHR))
					nvram_set("disconnectrssithr", item->params[0].value);
				
				break;
			case CLI_INDEX_CONNRSSITHR:
				if (!strcmp(item->params[0].param_name, CLI_NAME_CONNRSSIHR))
					nvram_set("connectrssithr", item->params[0].value);
				
				break;
			case CLI_INDEX_ACCOUNT:
				if (!strcmp(item->params[0].param_name, CLI_NAME_ACCOUNT))
					ezplib_replace_attr("http_rule", 0, "admpasswd", item->params[0].value);
				
				break;
			case CLI_INDEX_DHCP:
				if (!strcmp(item->params[0].param_name, CLI_NAME_DHCP)) {
					if (!strcmp(item->params[0].value, "dhcp")) {
						ezplib_replace_attr("lan0_proto", 0, "curr", "dhcp");
						nvram_set("lan0_ipaddr", "");
						nvram_set("lan0_mask", "");
					} else {
						ezplib_replace_attr("lan0_proto", 0, "curr", "static");
						nvram_set("lan0_ipaddr", "192.168.1.2");
						nvram_set("lan0_mask", "24");	
					}
				}
				
				break;
			default:
				uiPrintf("Invalid param");
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

	if (cli_debug) {
		for (i = 0; i < GLOBAL_CONFIGS_COUNT; i++) {
			struct item_config *item = global_configs[i].item;
			if (item) {
				printf("item: %s\n", item->name);
				for (j = 0; j < GLOBAL_ITEM_TATAL_PARAMS; j++ ) {
					struct param_pair pair = item->params[j];
					printf("param: %s, value: %s\n", pair.param_name, pair.value);
				}
			} else
				printf("item is null\n");

		}
	}
}

int set_value(const char *name ,struct item_config *item)
{
	int i = 0, j = 0;
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
					printf("pair1: %s, pair2: %s\n", pair1->param_name, pair2->param_name);
				
				if (!strncmp(pair1->param_name, pair2->param_name, strlen(pair2->param_name))) {
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

 /***********************************************************************
 * Function Name : quitCmdHandler
 * Description    : process quit command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_QUIT, quit command success
 ***********************************************************************/
int quitCmdHandler(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	/* Something important need to be done before termination */

	return CLI_PARSE_QUIT;
}
//Deleted by Mario Huang
#if 0
int antennaCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("Antenna: %s\n", "");
	return CLI_PARSE_OK;
}
#endif
 /***********************************************************************
 * Function Name : ipAddrCliCmdGet
 * Description    : process ip address getting command
 * Input         : @pCli, cli control structure
 *                    @pTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("IP Address: %s\n", "");
	return CLI_PARSE_OK;
}
//Deleted by Mario Huang
#if 0
int ipMaskCliCmdGet(CLI * pCli, char *p, struct parse_token_s *pTbl)
{
	uiPrintf("IP Subnet Mask: %s\n", "");
	return CLI_PARSE_OK;
}
#endif
 /***********************************************************************
 * Function Name : ipAddrCliCmdSet
 * Description    : process ip address setting command
 * Input         : @pCli, cli control structure
 *                    @pNxtTbl, token table
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl)
{

	return CLI_PARSE_NO_VALUE;
}
//Deleted by Mario Huang
#if 0
int ipMaskCliCmdSet(CLI * pCli, char *p, struct parse_token_s *pNxtTbl)
{

	return CLI_PARSE_NO_VALUE;
}
#endif
 /***********************************************************************
 * Function Name : systemNameGet
 * Description    : process device name getting command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int systemNameGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char sysname[256]={0};
	if(T_FAILURE == get_sysname_from_nvram(sysname))
	{
		return T_FAILURE;
	}

	uiPrintf("Device id: %s\n", sysname);
	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : systemNameSet
 * Description    : process device name setting command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int systemNameSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char sys_name[33]={0} ;
	char *token_get = NULL;
	//char buf[256]={0};
	int token_len = 0;
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	//if ( tokenCount(pCli) > 1 ) {
	//	uiPrintf(SETFALIED);
	//	return CLI_PARSE_NOMESSAGE;
	//}
	//Added by Mario Huang for print
	//uiPrintf("Enter %s,%d",__func__,__LINE__);
	token_get = tokenPop(pCli);
	if ( token_get == NULL )
		return CLI_PARSE_NO_VALUE;

	token_len = strlen(token_get);
	if(token_len > 32)
	{
		uiPrintf("Device name length can not be more than 32 charactors\n");
		return CLI_PARSE_INPUT_ERROR;
	}
	else if(token_len < 0)
	{
		uiPrintf("Device name can not be empty\n");
		return CLI_PARSE_NO_VALUE;
	}
	strcpy(sys_name, token_get);
	if(T_FALSE == check_systemname(sys_name))
	{
		uiPrintf("The only valid characters for a system name are letters numbers and a hyphen -_\n");
		return CLI_PARSE_INPUT_ERROR;
	}
	//nvram_set("hostname", sys_name);
	strcpy(item.name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].param_name, CLI_NAME_DEVICEID);
	strcpy(item.params[0].value, sys_name);

	set_value(CLI_NAME_DEVICEID, &item);

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : rebootCmdHandler
 * Description    : process device reboot command
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int rebootCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	system("reboot");
	
	return CLI_PARSE_OK;

}
 /***********************************************************************
 * Function Name : accountGet
 * Description    :  get account information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int accountGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{	
	//char nv_admpass[TMP_LEN];
	char nv_admuser[TMP_LEN];

	ezplib_get_attr_val("http_rule", 0, "admusername", nv_admuser, TMP_LEN, EZPLIB_USE_CLI);
	//ezplib_get_attr_val("http_rule", 0, "admpasswd", nv_admpass, TMP_LEN, EZPLIB_USE_CLI);

	uiPrintf("Username: %s\n", nv_admuser);
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
 * Function Name : accountSet
 * Description    :  store account information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int accountSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	  char *new_nv_admpass = NULL;          //new password
	  //char admpass[TMP_LEN];
	  //char admuser[TMP_LEN];
	  int total_params = 1;                 //number of total params
	  int new_password_length = 0;
	  struct item_config item;

	  if ( tokenCount(pCli) != total_params ) {
		  uiPrintf(SETFALIED);
		  return CLI_PARSE_NOMESSAGE;
	  }

	  //new password
	  new_nv_admpass = tokenPop(pCli);
	  
	  if (!new_nv_admpass)
	  	  return CLI_PARSE_NO_VALUE;

	  if (!strlen(new_nv_admpass))
	  	  return CLI_PARSE_NO_VALUE;

	  //store new password
	  new_password_length = strlen(new_nv_admpass);
	  if (new_password_length < 4 || new_password_length > 64) {
			uiPrintf("Password length from 4 to 64\n");
			return CLI_PARSE_OK;
	  }

	  memset(&item, 0, sizeof(struct item_config));
	  strcpy(item.name, CLI_NAME_ACCOUNT);
	  strcpy(item.params[0].param_name, CLI_NAME_ACCOUNT);
	  strcpy(item.params[0].value, new_nv_admpass);
	  set_value(CLI_NAME_ACCOUNT, &item);
	  //ezplib_replace_attr("http_rule", 0, "admpasswd", new_nv_admpass);
	  
	  //nvram_commit();

	  return CLI_PARSE_OK;
}
/***********************************************************************
  * Function Name : rssithrSet
  * Description    :  set disconnrssithr and connrssithr
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int rssithrSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 2; //total params
	char *disconn_rssi_str = NULL, *conn_rssi_str = NULL; //input params rssi
	int disconn_rssi = 0, conn_rssi = 0;
	char buf[32] = {0};
	struct item_config item;

	if ( tokenCount(pCli) != total_params ) {
		  //uiPrintf(SETFALIED);
		  uiPrintf("Usage: set connrssithr disconn_value conn_value\n");
		  uiPrintf("       disconn_value -- value is integer, range from -55 to -95\n");
		  uiPrintf("       conn_value -- value is integer, range from -45 to -85\n");
		  return CLI_PARSE_NOMESSAGE;
	}

	disconn_rssi_str = tokenPop(pCli);
	conn_rssi_str = tokenPop(pCli);

	if (!disconn_rssi_str || !strlen(disconn_rssi_str) ||
		!conn_rssi_str || !strlen(conn_rssi_str))
		return CLI_PARSE_NO_VALUE;

	disconn_rssi = atoi(disconn_rssi_str);
	conn_rssi = atoi(conn_rssi_str);

	#define CONN_RSSI_MAX -45
	#define CONN_RSSI_MIN -85
	if (conn_rssi > CONN_RSSI_MAX || conn_rssi < CONN_RSSI_MIN) {
		uiPrintf("connrssithr from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}
	#undef CONN_RSSI_MAX
	#undef CONN_RSSI_MIN

	#define DISCONN_RSSI_MAX -55
	#define DiSCONN_RSSI_MIN -95
	if (disconn_rssi > DISCONN_RSSI_MAX || disconn_rssi < DiSCONN_RSSI_MIN) {
		uiPrintf("disconnrssi from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}

	#undef DISCONN_RSSI_MAX
	#undef DiSCONN_RSSI_MIN

	if ((conn_rssi - disconn_rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconnrssithr 10\n");
		return CLI_PARSE_OK;
	}

	sprintf(buf, "%d", conn_rssi);
	if (strncmp(conn_rssi_str, buf, strlen(conn_rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", disconn_rssi);
	if (strncmp(disconn_rssi_str, buf, strlen(disconn_rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}

	//connect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].param_name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].value, conn_rssi_str);
	set_value(CLI_NAME_CONNRSSIHR, &item);

	//disconnect rssi thr
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].value, disconn_rssi_str);
	set_value(CLI_NAME_DISCONNRSSITHR, &item);

	return CLI_PARSE_OK;
}
   /***********************************************************************
  * Function Name : connectRssiThresholdSet
  * Description    :  store connect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int connectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 1; //total params
	char *rssi_str = NULL; //input params rssi
	int rssi = 0;
	char buf[32] = {0};
	int disconn_rssi = 0;
	struct item_config item;
	
	if ( tokenCount(pCli) != total_params ) {
		  //uiPrintf(SETFALIED);
		  uiPrintf("Usage: set connrssithr value\n");
		  uiPrintf("       value -- value is integer, range from -45 to -85\n");
		  return CLI_PARSE_NOMESSAGE;
	}

	rssi_str = tokenPop(pCli);

	if (!rssi_str || !strlen(rssi_str))
		return CLI_PARSE_NO_VALUE;

	rssi = atoi(rssi_str);

	#define CONN_RSSI_MAX -45
	#define CONN_RSSI_MIN -85
	if (rssi > CONN_RSSI_MAX || rssi < CONN_RSSI_MIN) {
		uiPrintf("connrssithr from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}
	#undef CONN_RSSI_MAX
	#undef CONN_RSSI_MIN

	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_DISCONNRSSITHR, &item);
	disconn_rssi = atoi(item.params[0].value);

	if ((rssi - disconn_rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconnrssithr 10\n");
		return CLI_PARSE_OK;
	}
	
	sprintf(buf, "%d", rssi);

	if (strncmp(rssi_str, buf, strlen(rssi_str))) {
		uiPrintf("Invalid value, value must be digit\n");
		return CLI_PARSE_OK;
	}
		
	//nvram_set("connectrssithr", rssi_str);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].param_name, CLI_NAME_CONNRSSIHR);
	strcpy(item.params[0].value, rssi_str);

	set_value(CLI_NAME_CONNRSSIHR, &item);
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : connectRssiThresholdGet
  * Description    :  get connect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int connectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *ptr = NULL;
	//system("echo 'Connect Rssi Threshold:' $(iwpriv sta0 get_connrssi | awk '{print $2}' | cut -c 14-)");
	ptr = nvram_safe_get("connectrssithr");

	if (ptr)
		uiPrintf("Connect Rssi Threshold: %d\n", atoi(ptr));
	else
		uiPrintf("Connect Rssi Threshold: --\n");
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : disconnectRssiThresholdGet
  * Description    :  get disconnect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int disconnectRssiThresholdGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	//system("echo 'Disconnect Rssi Threshold:' $(iwpriv sta0 get_disconnrssi | awk '{print $2}' | cut -c 17-)");
	char *ptr = NULL;
	
	ptr = nvram_safe_get("disconnectrssithr");

	if (ptr)
		uiPrintf("Disconnect Rssi Threshold: %d\n", atoi(ptr));
	else
		uiPrintf("Disconnect Rssi Threshold: --\n");

	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : disconnectRssiThresholdSet
  * Description    :  store disconnect RssiThreshold
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int disconnectRssiThresholdSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	int total_params = 1; //total params
	char *rssi_str = NULL; //input params rssi
	int rssi = 0;
	char buf[32] = {0};
	struct item_config item;
	int conn_rssi = 0;

	if ( tokenCount(pCli) != total_params ) {
		uiPrintf("Usage: set disrssithr value\n");
		uiPrintf("value is integer, range from -200 to 200\n");
		return CLI_PARSE_NOMESSAGE;
	}

	rssi_str = tokenPop(pCli);

	if (!rssi_str || !strlen(rssi_str))
	  	return CLI_PARSE_NO_VALUE;

	rssi = atoi(rssi_str);
	#define DISCONN_RSSI_MAX -55
	#define DiSCONN_RSSI_MIN -95
	if (rssi > DISCONN_RSSI_MAX || rssi < DiSCONN_RSSI_MIN) {
		uiPrintf("disconnrssi from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
		return CLI_PARSE_OK;
	}

	#undef DISCONN_RSSI_MAX
	#undef DiSCONN_RSSI_MIN

	memset(&item, 0, sizeof(struct item_config));
	get_value(CLI_NAME_CONNRSSIHR, &item);
	conn_rssi = atoi(item.params[0].value);

	if ((conn_rssi - rssi) < 10) {
		uiPrintf("Connrssithr must be greater than disconnrssithr 10\n");
		return CLI_PARSE_OK;
	}
	
	sprintf(buf, "%d", rssi);

	if (strncmp(rssi_str, buf, strlen(rssi_str))) {
	   	uiPrintf("Invalid value, value must be digit\n");
	   	return CLI_PARSE_OK;
	}

	//nvram_set("disconnectrssithr", rssi_str);
	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].param_name, CLI_NAME_DISCONNRSSITHR);
	strcpy(item.params[0].value, rssi_str);
	set_value(CLI_NAME_DISCONNRSSITHR, &item);

	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : rssiGet
  * Description    :  get  rssi
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int rssiGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char rssi[32] = {0};
	char TempBuf[32] = {0};
	int ret = 0;

	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);

	if (!strcmp(TempBuf, "client")) {
		//param: 0, means first radio
		ret = get_sta_assoc_rssi(0, rssi);
		if (ret != 0) {
			uiPrintf("Rssi: -/-\n");
		} else {
			uiPrintf("Rssi: %s\n", rssi);
		}
	}
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : macGet
  * Description    :  get mac
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int macGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
    //char modeBuf[SHORT_BUF_LEN] = {0};
	//char deviceBuf[SHORT_BUF_LEN] = {0};
	char hwaddr[SHORT_BUF_LEN] = {0};

	//char ifname[16] = {0};
    char device[16] = {0};
    //char hwaddr[64];
    char *if_ptr = NULL;
    //int ret = -1;

	#if 0
	/*get wireless mac address */
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", modeBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	if (!strcmp(modeBuf, "client")) {
		ezplib_get_attr_val("wl_ap_device_rule", 0, "sta_device", deviceBuf, 
				SHORT_BUF_LEN, EZPLIB_USE_CLI);
	} 

	if (!show_hwaddr(deviceBuf, hwaddr, SHORT_BUF_LEN)) {
		uiPrintf("Wlan Mac: %s", hwaddr);
	} else {
        uiPrintf("Wlan Not enabled");
    }
	#endif
	/* get bridge-lan's mac address */
    if_ptr = nvram_safe_get("lan0_ifname");

    //PPPoE interface doesn't have MAC address
	memset(hwaddr, 0, sizeof(hwaddr));
    if (!strncmp(if_ptr, "ppp", 3)) {
        if (snprintf(device, 16, "lan0_device") >= 16)
            return -1;
        if_ptr = nvram_safe_get(device);
    }

    if (!show_hwaddr(if_ptr, hwaddr, 64)) {
 		uiPrintf("mac: %s\n", hwaddr);
    } else {
        uiPrintf("mac: --:--:--:--:--:--\n");
	}
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : ssidGet
  * Description    :  get ssid
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ssidGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	struct item_config item;
	#if 0
    char buf[65] = {0};
	FILE *fp = NULL;
	int len = 0;
	
	system("iwconfig apclii0 | grep ESSID | sed 's/^.*ESSID:\"//g' | sed 's/\".*$//g' > /tmp/apcli_essid");

	if (NULL == (fp = fopen("/tmp/apcli_essid", "r"))) {
		uiPrintf("CPE disconnect");
		return CLI_PARSE_ERROR;
	} else {
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		rewind(fp);
		fgets(buf, len, fp);
		fclose(fp);

		uiPrintf("CPE Ssid: %s", buf);
	}
	
	return CLI_PARSE_OK;

	#else
	//char *value;
	char buf[TMP_LEN] = {0};

	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "")){
		uiPrintf("disconnect\n");
	}else{
		uiPrintf("ssid: %s\n", buf);
	}

	memset(&item, 0, sizeof(struct item_config));
	get_value("ssid", &item);

	if (cli_debug)
		uiPrintf("Ram name = %s, value = %s\n", item.params[0].param_name,item.params[0].value);

	return CLI_PARSE_OK;
	#endif
}
  /***********************************************************************
  * Function Name : ssidSet
  * Description    :  set ssid
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ssidSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *ssid;
	char buf[256]={0};
	int total_params = 0;
	struct item_config item;
	
	total_params =  tokenCount(pCli);
	ssid = tokenPop(pCli);
	
	while(ssid) {
		strcat(buf, ssid);
		total_params--;
		if (total_params)
			strcat(buf, " ");
		ssid = tokenPop(pCli);
	}
	ssid = buf;
	//printf("ssid = %s\n", ssid);
	if ( !ssid || !strlen(ssid))
		return CLI_PARSE_NO_VALUE;

	#define SSID_MIN_LEN 1
	#define SSID_MAX_MAX 32
	if ( strlen(ssid) > SSID_MAX_MAX || strlen(ssid) < SSID_MIN_LEN) {
		uiPrintf("Length of ssid from 1 to 32\n");
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
	
	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : securityGet
  * Description    :  get wireless security
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int securityGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	#if 0
	int ret = 0;
	char TempBuf[32] = {0};
	AP_INFO ap_info;

	memset(&ap_info, 0, sizeof(AP_INFO));
	ezplib_get_attr_val("system_mode", 0, "name", TempBuf, 32, EZPLIB_USE_CLI);
	if ((!strcmp(TempBuf, "wisp1")) || (!strcmp(TempBuf, "sta0"))) {
		//param: 0, means first radio
		ret = get_sta_assoc_ap_info(0, &ap_info);
		if (ret != 0) {
			//uiPrintf("ERROR: Get AuthMode Failure\n");
			uiPrintf("CPE is not working\n");
		} else {
			switch(ap_info.auth_mode) {
				case AUTHMODE_NONE:
					uiPrintf("Security: none/");
					break;
				case AUTHMODE_WPAPSK:
					uiPrintf("Security: wpa-psk/");
					break;
				case AUTHMODE_WPA2PSK:
					uiPrintf("Security: wpa2-psk/");
					break;
				case AUTHMODE_WPAPSKWPA2PSK:
					uiPrintf("Security: WPA2-Mixed-PSK/");
					break;
				case AUTHMODE_WEP:
					uiPrintf("Security: wep/");
					break;
				default:
					uiPrintf("Security: unkown security/");
			}

			switch (ap_info.enc_type) {
				case ENCRY_NONE:
					uiPrintf("none\n");
					break;
				case ENCRY_AES:
					uiPrintf("AES\n");
					break;
				case ENCRY_TKIP:
					uiPrintf("TKIP\n");
					break;
				case ENCRY_TKIPAES:
					uiPrintf("TKIPAES\n");
					break;
				case ENCRY_WEP:
					uiPrintf("WEP\n");
					break;
				default:
					uiPrintf("unkown\n");
			}
				
		}
	}
	#else
	char auth_mode[32] = {0};
	char enc_type[32] = {0};

	ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", auth_mode, 32, EZPLIB_USE_CLI);

	ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", enc_type, 32, EZPLIB_USE_CLI);
	printf("%s, %s\n", auth_mode, enc_type);
	if (!strcmp(auth_mode, "none")) {
		uiPrintf("Security: none\n");
	}
	else if (!strcmp(auth_mode, "disabled")) {
		uiPrintf("Security: wep\n");
	}
	else if (!strcmp(auth_mode, "psk")) {
		if (!strcmp(enc_type, "aes")) {
			uiPrintf("Security: WPA-psk/AES\n");
		}
		else if (!strcmp(enc_type, "tkip")) {
			uiPrintf("Security: WPA-psk/TKIP\n");
		} 
		else if (!strcmp(enc_type, "aestkip") || !strcmp(enc_type, "tkipaes")) {
			uiPrintf("Security: WPA-psk/AESTKIP\n");
		} 
	} 
	else if (!strcmp(auth_mode, "psk2")) {
		if (!strcmp(enc_type, "aes")) {
			uiPrintf("Security: WPA2-psk/AES\n");	
		}
		else if (!strcmp(enc_type, "tkip")) {
			uiPrintf("Security: WPA2-psk/TKIP\n");
		} 
		else if (!strcmp(enc_type, "aestkip") || !strcmp(enc_type, "tkipaes")) {
			uiPrintf("Security: WPA2-psk/AESTKIP\n");
		} 
	}
	else {
		uiPrintf("Invalid encryption type\n");
	}
	
	#endif

	return CLI_PARSE_OK;
}
  /***********************************************************************
  * Function Name : securitySet
  * Description    :  set wireless security
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int securitySet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
   char *mode = NULL;     //auth mode value
   char *enc_type = NULL; //ecryption type value
   char *psk = NULL;      //psk value
   char *option_m = NULL; //auth mode
   char *option_e = NULL; // encryption type
   char *option_p = NULL; //pre-shared key
   int total_param = 6;   //total params
   int mode_int = 0;
   int enc_type_int = 0;
   struct item_config item;
   
   if ( tokenCount(pCli) != total_param) {
	   uiPrintf("Usage: set security -m auth -e enc_type -p psk\n");
	   uiPrintf("-m auth mode. 0-none, 1-WPAPSK, 2-WPA2PSK, 3-WPA/WPA2PSK\n");
	   uiPrintf("-e encryption type. 0-none, 1-TKIP, 2-AES, 3-TKIPAES\n");
	   uiPrintf("-p pre-shared key\n");
	   return CLI_PARSE_NOMESSAGE;
   }

   //option: m
   option_m = tokenPop(pCli);
   //auth mode
   mode = tokenPop(pCli);
   //option: e
   option_e = tokenPop(pCli);
   //encryption type
   enc_type = tokenPop(pCli);
   //option: p
   option_p = tokenPop(pCli);
   //pre-shared key
   psk = tokenPop(pCli);
   
   if ( !mode || !enc_type || !psk || !option_m || !option_e || !option_p )
	   return CLI_PARSE_NO_VALUE;

   #define OPTION_M "-m"
   #define OPTION_E "-e"
   #define OPTION_P "-p"

   if (strncmp(option_m, OPTION_M, strlen(OPTION_M)) ||
   	    strncmp(option_e, OPTION_E, strlen(OPTION_E)) ||
   	     strncmp(option_p, OPTION_P, strlen(OPTION_P))) {
	   return CLI_PARSE_INVALID_PARAMETER;
   }
   #undef OPTION_M
   #undef OPTION_E
   #undef OPTION_P

   if (!strlen(mode) || !strlen(enc_type))
   	   return CLI_PARSE_NO_VALUE;

   if (strlen(psk) < 8) {
   	   uiPrintf("Pre shared key should be larger than 8\n");
	   return CLI_PARSE_OK;
   }

   memset(&item, 0, sizeof(struct item_config));
   strcpy(item.name, CLI_NAME_SECURITY);
   //first parameter
   strcpy(item.params[0].param_name, SECURITY_AUTH_MODE);
   //second parameter
   strcpy(item.params[1].param_name, SECURITY_ENC_TYPE);
   //third parameter
   strcpy(item.params[2].param_name, SECURITY_key);

   mode_int = atoi(mode);
   enc_type_int = atoi(enc_type);
   if (mode_int == AUTHMODE_WEP) {
	   strcpy(item.params[0].value, "disabled");
   } 
   else if (mode_int == AUTHMODE_WPAPSK) {
	   strcpy(item.params[0].value, "psk");
	   if (enc_type_int == ENCRY_TKIP) {
		   strcpy(item.params[1].value, "tkip");
	   } 
	   else if (enc_type_int == ENCRY_AES) {
		   strcpy(item.params[1].value, "aes");
	   } 
	   strcpy(item.params[2].value, psk);
    } 
	else if ((mode_int == AUTHMODE_WPA2PSK) || (mode_int == AUTHMODE_WPAPSKWPA2PSK)) {
	   strcpy(item.params[0].value, "psk2");
	   if (enc_type_int == ENCRY_TKIP) {
		  strcpy(item.params[1].value, "tkip");
	   } 
	   else if (enc_type_int == ENCRY_AES) {
	      strcpy(item.params[1].value, "aes");
	   } 
	   ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", psk);
	    strcpy(item.params[2].value, psk);
	} else {
		uiPrintf("Invalid security, authMode: %s, encType: %s\n", 
			mode, enc_type);
	}

	set_value(CLI_NAME_SECURITY, &item);

	return CLI_PARSE_OK;
}
   /***********************************************************************
  * Function Name : ipmaskGet
  * Description    :  get netmask
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ipmaskGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *value;
	char value2[32] = {0};
	char buf[TMP_LEN];

	ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	if (!strcmp(value2, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "mask", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (buf == NULL)
			uiPrintf("NetMask: 0.0.0.0\n");
		else{
			if(!strcmp(buf, ""))
				uiPrintf("NetMask: 0.0.0.0\n");
			else
				uiPrintf("NetMask: %s\n", buf);
		}
	}else{
		value = nvram_safe_get("lan0_mask");
		if (value ==NULL)
			uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
		else
		{
			if(!strcmp(value, ""))
				uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
			else
				uiPrintf("NetMask: %s\n", value);
		}
	}
	
	return CLI_PARSE_OK;
}
/***********************************************************************
  * Function Name : ipmaskSet
  * Description    :  set netmask
  * Input		  : @pCli, cli control structure
  * 				   @pToken, token
  * 				   @pNxtTbl, next token
  * Output		  : 
  * Return value  : CLI_PARSE_OK, command success
  ***********************************************************************/
int ipmaskSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *net_mask;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;
	}

	net_mask = tokenPop(pCli);
	if ( !net_mask || !strlen(net_mask) )
		return CLI_PARSE_NO_VALUE;

	if ( is_integer(net_mask) && atoi(net_mask) > 0 && atoi(net_mask) <= 32) {
		ezplib_get_attr_val("lan0_proto", 0, "curr", value, 32, EZPLIB_USE_CLI);			

		if (!strcmp(value, "static")) {
			br_get = nvram_safe_get("lan0_ifname");
			if((!br_get) || (strlen(br_get) == 0))
			{
				uiPrintf("can not get bridge name\n");
				return T_FALSE;
			}
			strcpy(br_name, br_get);
			strcpy(item.name, CLI_NAME_NETMASK);
			strcpy(item.params[0].param_name, CLI_NAME_NETMASK);
			strcpy(item.params[0].value, net_mask);
			set_value(CLI_NAME_NETMASK, &item);
		}
		else{
			uiPrintf("Can not modify Net Mask when DHCP client enabled\n");
			
			return CLI_PARSE_ERROR;
		}
		
		return CLI_PARSE_OK;
	} else {
		uiPrintf("Invalid Net Mask\n");
		
		return CLI_PARSE_NOMESSAGE;
	}
	
	return CLI_PARSE_OK;
}
/***********************************************************************
* Function Name : dhcpGet
* Description	 :  get dhcp status
* Input 		: @pCli, cli control structure
*					 @pToken, token
*					 @pNxtTbl, next token
* Output		: 
* Return value	: CLI_PARSE_OK, command success
***********************************************************************/
int dhcpGet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char buf_proto[32] = {0};
	
	ezplib_get_attr_val("lan0_proto", 0, "curr", buf_proto, 32, EZPLIB_USE_CLI);

	if (!strcmp(buf_proto, "dhcp"))
		uiPrintf("DHCP: enable\n");
	else 
		uiPrintf("DHCP: disable\n");
	
	return CLI_PARSE_OK;
}
/***********************************************************************
 * Function Name : dhcpSet
 * Description    : enable or disable dhcp function
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int dhcpSet(CLI *pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *dhcp = NULL;
	struct item_config item;

	memset(&item, 0, sizeof(struct item_config));
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;
	}

	dhcp = tokenPop(pCli);
	if ( !dhcp || !strlen(dhcp) )
		return CLI_PARSE_NO_VALUE;

	memset(&item, 0, sizeof(struct item_config));
	strcpy(item.name, "dhcp");
	strcpy(item.params[0].param_name, "dhcp");

	if (!strcmp(dhcp, "enable")) {
		//ezplib_replace_attr("lan0_proto", 0, "curr", "dhcp");
		//nvram_set("lan0_ipaddr", "");
		//nvram_set("lan0_mask", "");
		strcpy(item.params[0].value, "dhcp");
		set_value("dhcp", &item);
	} else if (!strcmp(dhcp, "disable")){
		//ezplib_replace_attr("lan0_proto", 0, "curr", "static");
		//nvram_set("lan0_ipaddr", "192.168.1.2");
		//nvram_set("lan0_mask", "24");
		strcpy(item.params[0].value, "static");
		set_value("dhcp",&item);
	} else {
		uiPrintf("Invalid value\n");
	}

	return CLI_PARSE_OK;
}


 /***********************************************************************
 * Function Name : ipaddrGet
 * Description    : get ip address from nvram
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipaddrGet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *value;
	char value2[32] = {0};
	char buf[TMP_LEN];
	struct item_config item;

	ezplib_get_attr_val("lan0_proto", 0, "curr", value2, 32, EZPLIB_USE_CLI);
	if (!strcmp(value2, "static")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (buf == NULL)
			uiPrintf("IP Address: 0.0.0.0\n");
		else{
			if(!strcmp(buf, ""))
				uiPrintf("IP Address: 0.0.0.0\n");
			else
				uiPrintf("IP Address: %s\n", buf);
		}
		memset(&item, 0, sizeof(struct item_config));
		get_value(CLI_NAME_IPADDR, &item);
		uiPrintf("name: %s, param name: %s, param value: %s\n", item.name, item.params[0].param_name, item.params[0].value);
	}else{
		value = nvram_safe_get("lan0_ipaddr");
		if (value ==NULL)
			uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
		else
		{
			if(!strcmp(value, ""))
				uiPrintf("Please wait no more than one minute. DHCP is now going on.\n");
			else
				uiPrintf("IP Address: %s\n", value);
		}
	}
	return CLI_PARSE_OK;
}
 
 /***********************************************************************
 * Function Name : ipAddrSet
 * Description    : set ip address
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int ipAddrSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *pIP;
	char value[32] = {0};
	char *br_get=NULL;
	char br_name[32]={0};
	struct item_config item;

	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;

	}

	pIP = tokenPop(pCli);
	if ( pIP == NULL )
		return CLI_PARSE_NO_VALUE;

	if ( verifyIP(pIP) == 0 ) {
		uiPrintf("Invalid IP Address\n");
		return CLI_PARSE_NOMESSAGE;
	}

	if ( IsValidIpAddress(pIP) ) {
		ezplib_get_attr_val("lan0_proto", 0, "curr", value, 32, EZPLIB_USE_CLI);			

		if (!strcmp(value, "static")) {
			br_get = nvram_safe_get("lan0_ifname");
			if((!br_get) || (strlen(br_get) == 0))
			{
				uiPrintf("can not get bridge name\n");
				return T_FALSE;
			}
			strcpy(br_name, br_get);
			//nvram_set("lan0_ipaddr", pIP);
			//ezplib_replace_attr("lan_static_rule", lan_num, "ipaddr", pIP);
			memset(&item, 0, sizeof(struct item_config));
			strcpy(item.name, CLI_NAME_IPADDR);
			strcpy(item.params[0].param_name, CLI_NAME_IPADDR);
			strcpy(item.params[0].value, pIP);
			set_value(CLI_NAME_IPADDR, &item);
		}
		else{
			uiPrintf("Can not modify IP address when DHCP client enabled\n");
			
			return CLI_PARSE_ERROR;
		}
		
		return CLI_PARSE_OK;
	} else {
		uiPrintf("Invalid IP Address\n");
		
		return CLI_PARSE_NOMESSAGE;
	}
}
 /***********************************************************************
 * Function Name : cancelcfgCmdHandler
 * Description    : restore data
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int cancelcfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	init_global_config();

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : cancelcfgCmdHandler
 * Description    : restore data
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/ 
int accessCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	return CLI_PARSE_ACCESS_LINUX;
}
 /***********************************************************************
 * Function Name : debugCmdHandler
 * Description    : print debug information
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int debugCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	char *debug = NULL;
	
	if ( tokenCount(pCli) > 1 ) {
		uiPrintf(SETFALIED);
		return CLI_PARSE_NOMESSAGE;

	}

	debug = tokenPop(pCli);
	if ( debug == NULL )
		return CLI_PARSE_NO_VALUE;

	if (atoi(debug) == 1) {
		cli_debug = 1;
	} else {
		cli_debug = 0;
	}

	return CLI_PARSE_OK;
}
 /***********************************************************************
 * Function Name : applycfgCmdHandler
 * Description    : store data to flash
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
 int applycfgCmdHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
 {
    char cmd[128] = {0};
	char *sys_name = NULL;
	char *connrssi = NULL;
	char *disconnrssi = NULL;

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
	connrssi = nvram_safe_get("connectrssithr");
	disconnrssi = nvram_safe_get("disconnectrssithr");

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 connrssi %d", (atoi(connrssi) + 95));
	if (cli_debug)
		printf("%s\n", cmd);
	system(cmd);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "iwpriv sta0 disconnrssi %d", (atoi(disconnrssi) + 95));
	if (cli_debug)
		printf("%s\n", cmd);
	system(cmd);
	
	return CLI_PARSE_OK;
 }
  /***********************************************************************
 * Function Name : factorydefaultSet
 * Description    : restore config to factory
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
 int factorydefaultSet(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
 {
 	system("nvram factory");
 	
	return CLI_PARSE_OK;
 }
 /***********************************************************************
 * Function Name : versionHandler
 * Description    : set firmware version
 * Input         : @pCli, cli control structure
 *                    @pToken, token
 *                    @pNxtTbl, next token
 * Output        : 
 * Return value  : CLI_PARSE_OK, command success
 ***********************************************************************/
int versionHandler(CLI * pCli, char *pToken, struct parse_token_s *pNxtTbl)
{
	FILE *fp;
	char c;
	char ch;
	int count=0,i=0;

	if ( tokenCount(pCli) > 0 ) {
		return CLI_PARSE_TOO_MANY;
	}

	if(!(fp=fopen("/version","r"))){
		uiPrintf("cant open file!\n");
		return T_FAILURE;
	}
	ch=fgetc(fp);
	while(ch!= EOF)
	{
		ch=fgetc(fp);
		count++;
	}
	fclose(fp);

	char *buffer = (char*)malloc(count*sizeof(char));
	if(buffer != NULL)
	{
		memset(buffer,0,count*sizeof(char));
	}
	if (!(fp = fopen("/version", "r")))
	{
		uiPrintf("cant open file!\n");
		free(buffer);
		return T_FAILURE;
	}
	while ((c = getc(fp)) != EOF) {
		//uiPrintf("%c",c);
		buffer[i] = c;
		i++;
	}
	i--;
	fclose(fp);
	buffer[i] = '\0';
	uiPrintf("\nFirmware Version %s\n",buffer);
	free(buffer);
	return CLI_PARSE_OK;
}

  /***********************************************************************
 * Function Name : applyIfAddress
 * Description    : used to config ip address
 * Input         : 
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/
int applyIfAddress(void)
{
	//TODO:
	return T_SUCCESS;
}
  /***********************************************************************
 * Function Name : applyWlanBasicSettings
 * Description    : used to config basic wlan settings
 * Input         : 
 * Output        : 
 * Return value  : T_SUCCESS, command success
 ***********************************************************************/
int applyWlanBasicSettings(void)
{
	//TODO:Use nvram instead
	return T_SUCCESS;
}

