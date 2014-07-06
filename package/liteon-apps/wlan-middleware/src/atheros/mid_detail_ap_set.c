//Added by Mario Huang 2013-07-15
#include    <stdlib.h>
#include    <stdio.h>
#include	<string.h>

#include    <sys/ioctl.h>
#include    <arpa/inet.h>
#ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
#include    <linux/types.h>
#include    <linux/socket.h>
#include    <linux/if.h>
#endif//End of #ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
#include    <linux/wireless.h>
// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>

#include	"nvram.h"
#include 	"nvram_rule.h"

#include    "mid_detail.h"
#include    "mid_common.h"

#include 	<unistd.h>
#include	<assert.h>

int get_acl_sync_list(int radio, int vap_id, ACL_LIST *acl_mac_list, ACL_LIST *asso_mac_list);

/*************************************/
/*>>>>>>>>>>general process>>>>>>>>>>*/
/*************************************/
/*
0:   legacy 11b/g mixed
1:   legacy 11b only
2:   legacy 11a only
3:   legacy 11a/b/g mixed
4:   legacy 11g only
5:   11abgn mixed
6:   11n only
7:   11gn mixed
8:   11an mixed
9:   11bgn mixed
10: 11agn mixed
11: 11n only in 5g band only
14: 11a/an/ac mixed 5g band only (Only 11ac chipset support)
*/
int set_ap_wirelessmode_channel(int radio)
{
    char cmd[128] = {0};
    char radio_name[8] = {0};
    char mode_str[32] = {0};
    char vap_name[8] = {0};
    //int htbw;
    int wlan_mode;
    int band_width;
    int wireless_mode;
    int channel; 
    int ext_channel_mode;
    int vap_num;
    int ret;
    int i;
    nvram_get_wlan_mode(radio, &wlan_mode);
    MID_ASSERT((WLAN_MODE_AP == wlan_mode), "Not in AP mode");
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    {
        printf("ERROR:Construct main AP failure!\n");
        return T_FAILURE;
    } 

    nvram_get_bandwidth(radio, &band_width);
    nvram_get_ap_wireless_mode(radio, &wireless_mode);
    nvram_get_ap_channel(radio, &channel);
    nvram_get_ap_ext_channel_mode(radio, &ext_channel_mode);

    //Only 11A,11B,11G,11BGN,11AN are supported by now
    switch(wireless_mode)
    {
        case WMODE_11A:
            strcpy(mode_str,"11A");
        break;
        case WMODE_11B:
            strcpy(mode_str,"11B");
        break;
        case WMODE_11G:
            strcpy(mode_str,"11G");
        break;
        case WMODE_11BGN:
            if(BANDWIDTH_20 == band_width)
            {
                strcpy(mode_str,"11NGHT20");
            }
            else if(BANDWIDTH_40 == band_width)
            {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel)
                {
                    strcpy(mode_str,"11NGHT40");
                }
                else if(EXT_CHANNEL_ABOVE == ext_channel_mode)
                {
                    strcpy(mode_str,"11NGHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode)
                {
                    strcpy(mode_str,"11NGHT40MINUS");
                }
            }
        break;   
        case WMODE_11AN:
            if(BANDWIDTH_20 == band_width)
            {
                strcpy(mode_str,"11NAHT20");
            }
            else if(BANDWIDTH_40 == band_width)
            {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel)
                {
                    strcpy(mode_str,"11NAHT40");
                }
                else if(EXT_CHANNEL_ABOVE == ext_channel_mode)
                {
                    strcpy(mode_str,"11NAHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode)
                {
                    strcpy(mode_str,"11NAHT40MINUS");
                }
            }
        break;
		case WMODE_11AC_MIXED:
            if(BANDWIDTH_20 == band_width)
            {
                strcpy(mode_str,"11ACVHT20");
            }
            else if(BANDWIDTH_40 == band_width)
            {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel)
                {
                    strcpy(mode_str,"11ACVHT40");
                }
				else if(EXT_CHANNEL_ABOVE == ext_channel_mode)
                {
                    strcpy(mode_str,"11ACVHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode)
                {
                    strcpy(mode_str,"11ACVHT40MINUS");
                }
            }
            else if(BANDWIDTH_80 == band_width)
            {
                    strcpy(mode_str,"11ACVHT80");
            }
        break;
        default:
        printf("No such wireless mode or is not supported currently!\n");
        return T_FAILURE;
    }


    vap_num = nvram_get_vap_num(radio);
    //Set all the vaps as same wireless mode and channel, although they can be set as different mode seperately
    i = 0;
    for(i=0; i < vap_num; i++)
    {
        ret = construct_vap(vap_name, radio, i, WLAN_MODE_AP);

        sprintf(cmd, "iwpriv %s mode %s", vap_name, mode_str);
        EXE_COMMAND(cmd);

		sprintf(cmd, "iwconfig %s channel %d", vap_name, channel);
		EXE_COMMAND(cmd);

		switch(wireless_mode){
        case WMODE_11BGN:
        case WMODE_11AN:
		case WMODE_11AC_MIXED:
			if(BANDWIDTH_40 == band_width) {
				sprintf(cmd, "iwpriv %s disablecoext 1",vap_name);
				EXE_COMMAND(cmd);
			}
			else {
				sprintf(cmd, "iwpriv %s disablecoext 0",vap_name);
				EXE_COMMAND(cmd);
			}
        	break;
        case WMODE_11A:
        case WMODE_11B:
        case WMODE_11G:
		default:
			sprintf(cmd, "iwpriv %s disablecoext 0",vap_name);
			EXE_COMMAND(cmd);
        	break;
		}

    }
    return T_SUCCESS;
}

/**
 * \brief AP Isolation 
 *   Communication between wireless clients with diffrent SSIDs)
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-02-17
 */
int set_ap_isolation(int radio)
{
    char cmd[128] = {0};
    int ret;
    char vap_name[8] = {0};
    int ap_isolation = 0;

    /*Set the isolation as enable/disable*/
    nvram_get_ap_isolation(radio, &ap_isolation);

	ret = construct_vap(vap_name, radio, 0, WLAN_MODE_AP);
	sprintf(cmd, "iwpriv %s apisolation %d", vap_name, ap_isolation);
	EXE_COMMAND(cmd);

    return ret;
}

/*Set SSID name (8 SSIDs)*/
int set_ssid(int radio, int mode, int vap_id)
{
    char ssid[128] = {0};    //2 x 'max length of ssid is 32' for ' & "
    char vap_name[8] = {0};
    int ret;
    char cmd[256] = {0};
    nvram_get_ap_ssid(radio, vap_id, ssid, 1);
    /*Construct VAP name by Radio, vap id, mode*/
    ret = construct_vap(vap_name, radio, vap_id, mode);
    if(T_FAILURE == ret)
    {
        printf("ERRO:Construc VAP failure!\n");
        return T_FAILURE;
    }
    
    /*VAP set SSID*/
    sprintf(cmd, "iwconfig %s essid \"%s\"", vap_name, ssid);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}
/*
Hidden SSID
0: disable
1: enable
*/
int set_hidden_ssid(int radio, int mode, int vap_id)
{
    char cmd[128] = {0};
    int hidden;
    char vap_name[8];
    int ret;

    nvram_get_ap_hidden(radio, &hidden, vap_id);
    ret = construct_vap(vap_name, radio, vap_id, mode);

    /*Set VAP hidden*/
    sprintf(cmd, "iwpriv %s hide_ssid %d", vap_name, hidden);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}

/*Enable/Disable BSS Isolation*/
int set_bss_isolation(int radio, int mode, int vap_id)
{
    char cmd[128];
    int bss_isolation = 0; 
    char vap_name[8];
    int ret;

    nvram_get_ap_bss_isolation(radio, vap_id, &bss_isolation);
    ret = construct_vap(vap_name, radio, vap_id, mode);

	sprintf(cmd, "iwpriv %s ap_bridge %d", vap_name, !bss_isolation);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}

/* Set DTIM (1~255) */
int set_dtim(int radio)
{
    char cmd[128] = {0};
    int dtim;
    int ret;
    int vap_num = 8;
    char vap_name[8] = {0};
    int i;
    //get dtim from NVRAM
    nvram_get_ap_dtim(radio, &dtim);

    /*Set DTIM*/
    for(i=0; i<vap_num; i++)
    {
        ret = construct_vap(vap_name, radio, i, WLAN_MODE_AP);
		if(T_FAILURE == ret)
		{
			printf("ERROR:Construct VAP failure!\n");
			return T_FAILURE;
		}
        sprintf(cmd, "iwpriv %s dtim_period %d", vap_name, dtim);
        EXE_COMMAND(cmd);
    }
    return T_SUCCESS;
}

/* Set Output Power */
int set_power(int radio)
{
	int ret = 0;
    int txpower = 0;
	int txpower_con = 0;
    int txpower_get = 0;
    int max_txpower = 0;
	int chain_num = 0;
    int wlan_mode = 0;
	char buf[256] = {0};
    char cmd[128] = {0};
    char vap_name[8] = {0};
    char txpower_s[256] = {0};
	char TempBuf_opmode[32] = {0};
	
	//Must get max txpower first
    get_max_txpower(radio, &max_txpower);

	//chainmask num: 1->0dB; 2->3dB; 3->5dB
	ret = get_chainmask_num(radio, &chain_num);
	if (T_FAILURE == ret){
		printf("TxChainMask illegal value\n");
		return T_FAILURE;
	}

	if (chain_num == 1){
		txpower_con = 0;
	} else if(chain_num == 2){
		txpower_con = 3;
	} else if(chain_num == 3){
		txpower_con = 5;
	}
	
    if(RADIO_2G == radio)
    {
    	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf_opmode,"ap"))
		{
			wlan_mode = WLAN_MODE_AP;
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "txpower", buf, 256, EZPLIB_USE_CLI);        
		}
		else
		{
			wlan_mode = WLAN_MODE_STA;
			ezplib_get_attr_val("wl_basic_rule", 0, "txpower", buf, 256, EZPLIB_USE_CLI);    
		}
    }
    else if(RADIO_5G == radio)
    {
    	ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
		if(!strcmp(TempBuf_opmode,"ap"))
		{
			wlan_mode = WLAN_MODE_AP;
			ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "txpower", buf, 256, EZPLIB_USE_CLI);            
		}
		else
		{
			wlan_mode = WLAN_MODE_STA;
			ezplib_get_attr_val("wl5g_basic_rule", 0, "txpower", buf, 256, EZPLIB_USE_CLI);        
		}
    }

	if ((WLAN_MODE_STA == wlan_mode) && (RADIO_5G == radio))
	{
		sprintf(cmd, "sleep 10 && /etc/wl/sta-txpower &");
		EXE_COMMAND(cmd);
	}
	else
	{
		strcpy(txpower_s, buf);   
		txpower_get = atoi(txpower_s);

		switch(txpower_get)
		{
			case POWER_FULL:
				txpower = max_txpower;
				break;
			case POWER_RONE:
				txpower = max_txpower - 1;
				break;
			case POWER_RTWO:
				txpower = max_txpower - 2;
				break;
			case POWER_HALF:
				txpower = max_txpower - 3;
				break;
			case POWER_RFOUR:
				txpower = max_txpower - 4;
				break;
			case POWER_RFIVE:
				txpower = max_txpower - 5;
				break;
			case POWER_QUARTER:
				txpower = max_txpower - 6;
				break;
			case POWER_RSEVEN:
				txpower = max_txpower - 7;
				break;
			case POWER_REIGHT:
				txpower = max_txpower - 8;
				break;
			case POWER_EIGHTH:
				txpower = max_txpower - 9;
				break;
			case POWER_MIN:
				txpower = txpower_con;
				break;
			default:
				printf("txpower is illegal value, check it please!!!\n");
				txpower = max_txpower;
		}

		if(WLAN_MODE_AP == wlan_mode)
		{
			ret = construct_vap(vap_name, radio, 0, WLAN_MODE_AP);
			if(T_FAILURE == ret)
			{
				printf("ERROR:Construct VAP failure!\n");
				return T_FAILURE;
			}
		}
		else if(WLAN_MODE_STA == wlan_mode)
		{

			ret = construct_vap(vap_name, radio, 0, WLAN_MODE_STA);
			if(T_FAILURE == ret)
			{
				printf("ERROR:Construct VAP failure!\n");
				return T_FAILURE;
			}
		}

		/*Set Power*/
		sprintf(cmd, "iwconfig %s txpower %d", vap_name, txpower);
		EXE_COMMAND(cmd);
	}
	
    return T_SUCCESS;
}

/*Set Beacon Interval (100~1000)*/
int set_beacon_interval(int radio)
{
    int ret;
    int beacon_interval;
    char vap_name[8] = {0};
	char cmd[128] = {0};
	
    nvram_get_ap_beacon_interval(radio, &beacon_interval);

    /*bintval have to be in 100~1000 scope*/
    if(beacon_interval < 100 || beacon_interval > 1000)
    {
        printf("ERRO:Beacon interval is out of region\n");
        return T_FAILURE;
    }

    ret = construct_vap(vap_name, radio, 0, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    {
        printf("ERROR:Construct VAP failure!\n");
        return T_FAILURE;
    }
    sprintf(cmd, "iwpriv %s bintval %d", vap_name, beacon_interval);
	EXE_COMMAND(cmd);
	
    return T_SUCCESS;
}
/*************************************/
/*<<<<<<<<<<general process<<<<<<<<<<*/
/*************************************/


/**************************************/
/*>>>>>>>>>>Security Process>>>>>>>>>>*/
/**************************************/
/*
#define AUTHMODE_NONE    0
#define AUTHMODE_WPAPSK    1
#define AUTHMODE_WPA2PSK    2
#define AUTHMODE_WPAPSKWPA2PSK    3
#define AUTHMODE_WPA    4
#define AUTHMODE_WPA2    5
#define AUTHMODE_WPAWPA2    6
#define AUTHMODE_WEP    7
*/
int set_security_authmode(int radio, int vap_id, security_cfg *ap_cfg)
{
    int secmode = 0;

    nvram_get_ap_secmode(radio, vap_id, &secmode);
    switch(secmode)
    {
        case AUTHMODE_WPA2PSK:
        case AUTHMODE_WPAPSKWPA2PSK:
        case AUTHMODE_WPA2:
        case AUTHMODE_WPAWPA2:
            ap_cfg->authmode = secmode;
			printf("ap_cfg->authmode=%d\n",ap_cfg->authmode);
        break;
        default:
        MID_ERROR("Such authmode is not WPA2PSK");
		break;
    } 
	printf("set_security_authmode succeed\n");

    return T_SUCCESS;
}

int judge_vap_up(int radio, int vap_id)
{
	int unit = 0;
	char buf[32]={0};
	FILE *fp = NULL;
	char filename[64] = {0};
	
	unit = convert_vap_id(radio, vap_id);
	sprintf(filename, "%s%d.conf.pid", HOSTAPD_CFG_FILE, unit);
	
	EXE_doSystem("ifconfig | grep ath%d > /tmp/judge_ath",unit);

	if((fp = fopen("/tmp/judge_ath","r")) != NULL)
	{
		fread(buf,1,32,fp);
		printf("\n=====================buf=%s,len=%d=====================\n",buf,strlen(buf));
		fclose(fp); 
		fp = NULL;
		if(strlen(buf) > 3)
		{	
			printf("ath%d is up\n",unit);
			return 0;
		}
		else
		{
			printf("ath%d is down\n",unit);
			return -1;
		}
	}	
	return 0;
}

/**
 * \brief set security OPEN for specified VAP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-03-11 update
 */
int set_security_open(int radio, int vap_id)
{
    int ret = 0;
	int unit = 0;
	char pidname[64] = {0};
	int updown = 0;

    /*Kill previous hostapd's pid*/
	unit = convert_vap_id(radio, vap_id);
	sprintf(pidname, "%s%d.pid", HOSTAPD_PID_FILE, unit);
	EXE_doSystem("kill `cat %s`",pidname);

	/*ifup VAP according to NVRAM*/
	ret = nvram_get_vap_status(radio, vap_id, &updown);
    if(T_FAILURE == ret) {
        fprintf(stderr, "Get VAP status failure!\n");
        return T_FAILURE;
    }

	if ( updown) {
		sleep(1);
		EXE_doSystem(" ifconfig ath%d up",unit);
	}
    return T_SUCCESS;
}

int set_security_encryptype(int radio, int vap_id, security_cfg *ap_cfg)
{
	assert(NULL != ap_cfg);

    int encryp_type = 0;
	ap_cfg->flags |= CLI_BSS_F_SECURE;

    nvram_get_security_encryptype(radio, vap_id, &encryp_type);
    if(ENCRY_AES == encryp_type)
    {
        strcpy(ap_cfg->encryp_type, "CCMP");
    }
    else if(ENCRY_TKIP == encryp_type)
    {
        strcpy(ap_cfg->encryp_type, "TKIP");
    }
    else if(ENCRY_TKIPAES == encryp_type)
    {
        strcpy(ap_cfg->encryp_type, "TKIP CCMP");
    }
	else
	{
		printf("set_security_encryptype failed,encrpt_type=%d\n",encryp_type);
		return T_FAILURE;
	}
    return T_SUCCESS;
}

int set_security_passphrase(int radio, int vap_id, security_cfg *ap_cfg)
{
	assert(NULL != ap_cfg);

    char pass_phrase[256] = {0};
	int len = 0;

    nvram_get_ap_passphrase(radio, vap_id, pass_phrase);

	len = strlen(pass_phrase);
	
	if (len == MAX_PASSPHRASE)
    {
        /* check for hex string */
        if (CheckHexString(pass_phrase) < 0)
        {
            fprintf(stderr, "Invalid hex key.\n");
            return -1;
        }
        ap_cfg->flags |= CLI_BSS_F_HASHED_KEY;
    }
    else if (len < MIN_PASSPHRASE)
    {
        /* passphrase must be greater than 8 characters */
        fprintf(stderr, "Passphrase not long enough.\n");

        return -1;
    }
    else if (len > MAX_PASSPHRASE)
    {
        printf("Passphrase too long.\n");
        return -1;
    }
    else
    {
        /* valid passphrase */
        ap_cfg->flags &= ~CLI_BSS_F_HASHED_KEY;
    }
    strcpy(ap_cfg->passphrase, pass_phrase);
    return T_SUCCESS;
}

int set_security_rekey(int radio, int vap_id, security_cfg *ap_cfg)
{
	assert(NULL != ap_cfg);

    int interval = 0;

    nvram_get_ap_rekey_time_interval(radio, vap_id, &interval);
    ap_cfg->rekey_interval = interval;
    return T_SUCCESS;
}

int set_security_radius(int radio, int vap_id, security_cfg *ap_cfg)
{
	assert(NULL != ap_cfg);

    char server[256] = {0};
    int port = 0;
    char key[256] = {0};
    char ownip[256] = {0};
	int preauth=0;
    
    nvram_get_ap_radius_ipaddr(radio, vap_id, server);
    nvram_get_ap_radius_port(radio, vap_id, &port);
    nvram_get_ap_radius_key(radio, vap_id, key);
	nvram_get_ap_radius_own_ip(radio, vap_id, ownip);
	nvram_get_ap_preauth(radio, vap_id, &preauth);

    strcpy(ap_cfg->radius_addr, server);
    ap_cfg->radius_port = port;
    strcpy(ap_cfg->radius_secret, key);
	strcpy(ap_cfg->own_ipaddr, ownip);
	ap_cfg->preauth = preauth;

    return T_SUCCESS;
}

int set_security_ssid(int radio, int mode, int vap_id, security_cfg * ap_cfg)
{
	assert(NULL != ap_cfg);

    char ssid[128] = {0};    //2 x 'max length of ssid is 32' for ' & "
    char vap_name[8] = {0};
    int ret;
    char cmd[256] = {0};
    nvram_get_ap_ssid(radio, vap_id, ssid, 0);
    /*Construct VAP name by Radio, vap id, mode*/
	strcpy(ap_cfg->ssid, ssid);
    ret = construct_vap(vap_name, radio, vap_id, mode);
    if(T_FAILURE == ret)
    {
        printf("ERRO:Construc VAP failure!\n");
        return T_FAILURE;
    }
    
    /*VAP set SSID*/
    sprintf(cmd, "iwconfig %s essid \"%s\"", vap_name, ssid);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}

int construct_bridge(char *bridge)
{
	if(1)
    {
	    strcpy(bridge, "br-lan0");
	    return T_SUCCESS;
	}
	else
	{	
		strcpy(bridge, "br-wan0");
		return T_SUCCESS;
	}
}

int set_security_bridge(int radio, int vap_id)
{
    char bridge[16] = {0};
   
    //Construce bridge
    construct_bridge(bridge);

    return T_SUCCESS;
}

/**
 * \brief start hostapd for specified VAP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-03-11 update
 */
int start_hostapd(int radio, int vap_id) //int radio
{
	int unit = 0,ret=0;
    char filename[64] = {0};
	char pidname[64] = {0};
	int updown = 0;

    /*Kill previous hostapd's pid*/
	unit = convert_vap_id(radio, vap_id);
	sprintf(pidname, "%s%d.pid", HOSTAPD_PID_FILE, unit);
	EXE_doSystem("kill `cat %s`",pidname);

	/*start hostapd for VAP according to NVRAM*/
	ret = nvram_get_vap_status(radio, vap_id, &updown);
    if(T_FAILURE == ret) {
        fprintf(stderr, "Get VAP status failure!\n");
        return T_FAILURE;
    }
	
	if(updown) {
		sprintf(filename, "%s%d.conf", HOSTAPD_CFG_FILE, unit);
	    EXE_doSystem("hostapd -B %s -P %s",filename,pidname);
	}
    return T_SUCCESS;
}

int save_hostapd(int radio, int vap_id,security_cfg * apcfg) 
{
    FILE *fp;
    int wpa = 0, psk = 0, radius = 0;
    char buf[128] = {0};
    static char filename[64] = {0};
    static char backupname[64] = {0};
    char brname[16] = {0};
	int unit = 0;

	printf("save_hostapd entered\n");
	
	unit = convert_vap_id(radio, vap_id);

    sprintf(filename, "%s%d.conf", HOSTAPD_CFG_FILE, unit);
    sprintf(backupname, "%s%d.conf", HOSTAPD_CFG_BACKUP, unit);

    if (apcfg->authmode == AUTHMODE_NONE)
    {
        rename(filename, backupname);
        printf(" not wpa or 802.11\n");
        return 0;
    }

    construct_bridge(brname);
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Can't create %s\n", filename);
        return T_FAILURE;
    }

    /* configure device info */
    sprintf(buf, "interface=ath%d\n", unit);
    fputs(buf, fp);
    sprintf(buf, "bridge=%s\n", brname);
    fputs(buf, fp);
    fputs("\n", fp);
    /* ssid */
    sprintf(buf, "ssid=%s\n", apcfg->ssid);
    fputs(buf, fp);

    /* configure log message */
    fputs("logger_syslog=1\n", fp);
	fputs("logger_syslog_level=0\n", fp);
	fputs("logger_stdout=1\n", fp);
	fputs("logger_stdout_level=0\n", fp);	
    fputs("\n", fp);

    /* configure other message */
	fputs("macaddr_acl=0\n",fp);
	fputs("ignore_broadcast_ssid=1\n",fp);
	fputs("auth_algs=1\n",fp);
	
    /* WPA version */
    wpa = 0;
    radius = 0;
    psk = 0;
    switch (apcfg->authmode)
    {
    case AUTHMODE_WPA2PSK:
        wpa = 2;
        psk = 1;
        break;
    case AUTHMODE_WPAPSKWPA2PSK:
        wpa = 3, psk = 1;
        break;
    case AUTHMODE_WPA2:
        wpa = 2, radius = 1;
        break;
    case AUTHMODE_WPAWPA2:
        wpa = 3, radius = 1;
        break;
    default:
        printf("Unknown auth type\n");
		break;
    }

    /* Write out WPA version */
    if (wpa)
    {
        sprintf(buf, "wpa=%d\n", wpa);
        fputs(buf, fp);
    }

    /* write out WPAPSK info if necessary */
	
    if (psk)
    {
        if (apcfg->flags & CLI_BSS_F_HASHED_KEY)
        {
            sprintf(buf, "wpa_psk=%s\n", apcfg->passphrase);
		}
        else
        {
   		    sprintf(buf, "wpa_passphrase=%s\n", apcfg->passphrase);
		}
        fputs(buf, fp);
        fputs("wpa_key_mgmt=WPA-PSK\n", fp);
    }

    /* write out RADIUS info if necessary */
    if (radius)
    {
		fputs("wpa_key_mgmt=WPA-EAP\n", fp);
		fputs("ieee8021x=1\n", fp);
		fputs("eapol_version=2\n", fp);
		fputs("eapol_key_index_workaround=0\n", fp);
		
        sprintf(buf, "own_ip_addr=%s\n", apcfg->own_ipaddr);
		fputs(buf, fp);
		sprintf(buf,"auth_server_addr=%s\n",apcfg->radius_addr);
		fputs(buf, fp);

        sprintf(buf, "auth_server_port=%d\n", apcfg->radius_port);
        fputs(buf, fp);

        sprintf(buf, "auth_server_shared_secret=%s\n", apcfg->radius_secret);
        fputs(buf, fp);
		
		sprintf(buf,"rsn_preauth=%d\n",apcfg->preauth);
        fputs(buf, fp);
		sprintf(buf,"rsn_preauth_interfaces=%s\n",brname);
        fputs(buf, fp);
    }

    if (wpa)
    {
        /* write out cipher info */
        sprintf(buf, "wpa_pairwise=%s\n",apcfg->encryp_type);
        fputs(buf, fp);

        /* write out group key update interval */
        sprintf(buf, "wpa_group_rekey=%d\n", apcfg->rekey_interval);
        fputs(buf, fp);
    }

    fclose(fp);
	fp = NULL;
    return 0;
}


int set_security_wpa2_psk(int radio, int vap_id)
{
	printf("set_security_wpa2_psk 1 entered\n");

	security_cfg  ap_cfg;
	memset(&ap_cfg, 0, sizeof(ap_cfg));
	int ret=0;
	
	printf("set_security_wpa2_psk 2 entered\n");

	ret = set_security_ssid(radio,WLAN_MODE_AP,vap_id,&ap_cfg);
	
	ret = set_security_authmode(radio, vap_id, &ap_cfg);
	if(ret == T_FAILURE)
	{
		printf("set_security_authmode failed\n");
	}

	ret = set_security_encryptype(radio, vap_id, &ap_cfg);
	if(ret == T_FAILURE)
	{
		printf("set_security_encryptype failed\n");
	}	

	ret = set_security_passphrase(radio, vap_id, &ap_cfg);
	if(ret == T_FAILURE)
	{
		printf("set_security_passphrase failed\n");
	}		
	set_security_rekey(radio, vap_id, &ap_cfg);
	save_hostapd(radio, vap_id, &ap_cfg);
	start_hostapd( radio, vap_id);
	usleep(20);
    return T_SUCCESS;
}

int set_security_wpa2(int radio, int vap_id)
{
	printf("set_security_wpa2 1 entered \n");

	security_cfg  ap_cfg;
	memset(&ap_cfg, 0, sizeof(ap_cfg));
	int ret=0;
	printf("set_security_wpa2 2 entered \n");

	ret = set_security_ssid(radio,WLAN_MODE_AP,vap_id,&ap_cfg);
	
	ret = set_security_authmode(radio, vap_id, &ap_cfg);
	if(ret == T_FAILURE)
	{
		printf("set_security_authmode failed\n");
	}
	ret = set_security_encryptype(radio, vap_id, &ap_cfg);
	if(ret == T_FAILURE)
	{
		printf("set_security_encryptype failed\n");
	}
	set_security_radius(radio, vap_id,&ap_cfg);
	set_security_rekey(radio, vap_id, &ap_cfg);
	save_hostapd(radio, vap_id, &ap_cfg);
	start_hostapd( radio, vap_id);
	usleep(20);
    return T_SUCCESS;
}

int set_ap_security(int radio, int vap_id)
{
    int secmode = 0;
    int ret = 0;

    nvram_get_ap_secmode(radio, vap_id, &secmode);
	printf("set_ap_security,down,secmode=%d\n",secmode);
    if(AUTHMODE_WPA2PSK == secmode || AUTHMODE_WPAPSKWPA2PSK == secmode)
        {
        	printf("set_security_wpa2_psk,secmode=%d\n",secmode);
            ret = set_security_wpa2_psk(radio, vap_id);
        }
    else if(AUTHMODE_WPA2 == secmode || AUTHMODE_WPAWPA2 == secmode)
        {
        	printf("set_security_wpa2,secmode=%d\n",secmode);
            ret = set_security_wpa2(radio, vap_id);
        }
    else if(AUTHMODE_NONE == secmode)
        {
        	printf("set_security_open,secmode=%d\n",secmode);
            ret = set_security_open(radio, vap_id);
        }
     else
        {
            printf("ERROR:This Security Mode is not support currently\n");
            return T_FAILURE;
        }
	 return ret;
}

/*****************************************************************/
/*                                         5G DFS Security Setting                                         */
/*                                     Added By Andy Yu in 20140321                                   */
/*****************************************************************/
int set_ap_security_dfs(int radio)
{
	int k = 0;
	int vap_num = 0;
	if (RADIO_5G != radio)
	{
		printf("ERROR:5G Security Support DFS\n");
        return T_FAILURE;
	}
	radio_up_down(RADIO_5G, RADIO_DOWN, WLAN_MODE_AP);
	kill_all_authentication_daemon(RADIO_5G, WLAN_MODE_AP);
    vap_num = nvram_get_vap_num(RADIO_5G);

	for(k=0; k<vap_num; k++)
	{
		set_ap_security(RADIO_5G, k);
	}
	return T_SUCCESS;
}
/**************************************/
/*<<<<<<<<<<Security Process<<<<<<<<<<*/
/**************************************/

/****************************/
/*<<<<<<<<<<wmm process<<<<<<<<<<*/
/*
wmm_enable^ack_enable^ac_be^ac_bk^ac_vi^ac_vo^
cwmin_be^cwmax_be^aifs_be^txop_be^cwmin_bk^cwmax_bk^aifs_bk^txop_bk^
cwmin_vi^cwmax_vi^aifs_vi^txop_vi^cwmin_vo^cwmax_vo^aifs_vo^txop_vo^bss
*/

/****************************/

int set_ap_wmm_enable(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	char buf1[NVRAM_BUF_LEN] = {0};  
    
    /*Get the Radio and wlan_mode*/
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
    	printf("case radio2g, mode = ap\n");
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "wmm_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
    }
	else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "wmm_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
    }
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "wmm_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);

		//WLAN5G WMM Trigger DFS Mechanism: Void Driver Bug -> Destroy All 5G VAP
		printf("case radio5g, mode = ap,buf1=%s\n",buf1);
		wmmcfg->wmm_enable = atoi(buf1);	

		int i = 0;
		int vap_num = 0;
		int	vap_enable = 0;
		char vap_name[8] = {0};
		vap_num = nvram_get_vap_num(radio); 
		for(i=0; i < vap_num; i++)
		{
			nvram_get_vap_status(radio, i, &vap_enable);
			if(VAP_ENABLE == vap_enable)
			{
				construct_vap(vap_name, radio, i, wmmcfg->mode);
				EXE_doSystem("iwpriv %s wmm %d",vap_name,wmmcfg->wmm_enable);
			}
		}

		return T_SUCCESS;
    }
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "wmm_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
    }
	printf("case radio2g, mode = ap,buf1=%s\n",buf1);
	wmmcfg->wmm_enable = atoi(buf1);	

	EXE_doSystem("iwpriv %s wmm %d",wmmcfg->vap_name,wmmcfg->wmm_enable);

	return T_SUCCESS;

}

int set_ap_wmm_cwmin(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	char buf1[NVRAM_BUF_LEN] = {0},buf2[NVRAM_BUF_LEN] = {0};
	char buf3[NVRAM_BUF_LEN] = {0},buf4[NVRAM_BUF_LEN] = {0};   
    
    /*Get the Radio and wlan_mode*/
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmin_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmin_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmin_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmin_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
    else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmin_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmin_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmin_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmin_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmin_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmin_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmin_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmin_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmin_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmin_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmin_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmin_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
	
	wmmcfg->cwmin_bk = atoi(buf1);	
	wmmcfg->cwmin_be = atoi(buf2);	
	wmmcfg->cwmin_vi = atoi(buf3);	
	wmmcfg->cwmin_vo = atoi(buf4);	
	
	

	EXE_doSystem("iwpriv %s setwmmparams 1 0 0 %d",wmmcfg->vap_name,wmmcfg->cwmin_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 1 0 %d",wmmcfg->vap_name,wmmcfg->cwmin_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 2 0 %d",wmmcfg->vap_name,wmmcfg->cwmin_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 3 0 %d",wmmcfg->vap_name,wmmcfg->cwmin_vo);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 0 1 %d",wmmcfg->vap_name,wmmcfg->cwmin_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 1 1 %d",wmmcfg->vap_name,wmmcfg->cwmin_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 2 1 %d",wmmcfg->vap_name,wmmcfg->cwmin_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 1 3 1 %d",wmmcfg->vap_name,wmmcfg->cwmin_vo);	
	
	return T_SUCCESS;

}
int set_ap_wmm_cwmax(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	char buf1[NVRAM_BUF_LEN] = {0},buf2[NVRAM_BUF_LEN] = {0};
	char buf3[NVRAM_BUF_LEN] = {0},buf4[NVRAM_BUF_LEN] = {0};   
    
    /*Get the Radio and wlan_mode*/
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmax_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmax_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmax_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "cwmax_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
    else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmax_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmax_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmax_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "cwmax_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmax_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmax_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmax_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "cwmax_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmax_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmax_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmax_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "cwmax_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
	
	wmmcfg->cwmax_bk = atoi(buf1);	
	wmmcfg->cwmax_be = atoi(buf2);	
	wmmcfg->cwmax_vi = atoi(buf3);	
	wmmcfg->cwmax_vo = atoi(buf4);	
	
	

	EXE_doSystem("iwpriv %s setwmmparams 2 0 0 %d",wmmcfg->vap_name,wmmcfg->cwmax_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 1 0 %d",wmmcfg->vap_name,wmmcfg->cwmax_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 2 0 %d",wmmcfg->vap_name,wmmcfg->cwmax_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 3 0 %d",wmmcfg->vap_name,wmmcfg->cwmax_vo);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 0 1 %d",wmmcfg->vap_name,wmmcfg->cwmax_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 1 1 %d",wmmcfg->vap_name,wmmcfg->cwmax_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 2 1 %d",wmmcfg->vap_name,wmmcfg->cwmax_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 2 3 1 %d",wmmcfg->vap_name,wmmcfg->cwmax_vo);
	
	return T_SUCCESS;

}

int set_ap_wmm_aifs(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	char buf1[NVRAM_BUF_LEN] = {0},buf2[NVRAM_BUF_LEN] = {0};
	char buf3[NVRAM_BUF_LEN] = {0},buf4[NVRAM_BUF_LEN] = {0};   
    
    /*Get the Radio and wlan_mode*/
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "aifs_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "aifs_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "aifs_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "aifs_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
    else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "aifs_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "aifs_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "aifs_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "aifs_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "aifs_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "aifs_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "aifs_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "aifs_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "aifs_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "aifs_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "aifs_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "aifs_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
	
	wmmcfg->aifs_bk = atoi(buf1);	
	wmmcfg->aifs_be = atoi(buf2);	
	wmmcfg->aifs_vi = atoi(buf3);	
	wmmcfg->aifs_vo = atoi(buf4);	
	
	

	EXE_doSystem("iwpriv %s setwmmparams 3 0 0 %d",wmmcfg->vap_name,wmmcfg->aifs_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 1 0 %d",wmmcfg->vap_name,wmmcfg->aifs_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 2 0 %d",wmmcfg->vap_name,wmmcfg->aifs_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 3 0 %d",wmmcfg->vap_name,wmmcfg->aifs_vo);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 0 1 %d",wmmcfg->vap_name,wmmcfg->aifs_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 1 1 %d",wmmcfg->vap_name,wmmcfg->aifs_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 2 1 %d",wmmcfg->vap_name,wmmcfg->aifs_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 3 3 1 %d",wmmcfg->vap_name,wmmcfg->aifs_vo);
	return T_SUCCESS;

}

int set_ap_wmm_txop(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	char buf1[NVRAM_BUF_LEN] = {0},buf2[NVRAM_BUF_LEN] = {0};
	char buf3[NVRAM_BUF_LEN] = {0},buf4[NVRAM_BUF_LEN] = {0};   
    
    /*Get the Radio and wlan_mode*/
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "txop_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "txop_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "txop_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "txop_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
    else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "txop_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "txop_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "txop_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "txop_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "txop_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "txop_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "txop_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "txop_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }/*
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "txop_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "txop_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "txop_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "txop_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
    }*/
	
	wmmcfg->txop_bk = atoi(buf1);	
	wmmcfg->txop_be = atoi(buf2);	
	wmmcfg->txop_vi = atoi(buf3);	
	wmmcfg->txop_vo = atoi(buf4);	
	
	

	EXE_doSystem("iwpriv %s setwmmparams 4 0 0 %d",wmmcfg->vap_name,wmmcfg->txop_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 1 0 %d",wmmcfg->vap_name,wmmcfg->txop_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 2 0 %d",wmmcfg->vap_name,wmmcfg->txop_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 3 0 %d",wmmcfg->vap_name,wmmcfg->txop_vo);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 0 1 %d",wmmcfg->vap_name,wmmcfg->txop_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 1 1 %d",wmmcfg->vap_name,wmmcfg->txop_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 2 1 %d",wmmcfg->vap_name,wmmcfg->txop_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 4 3 1 %d",wmmcfg->vap_name,wmmcfg->txop_vo);
	
	return T_SUCCESS;

}

int set_ap_wmm_ac(int radio, int vap_id, wmm_cfg  *wmmcfg)
{

	char buf1[NVRAM_BUF_LEN] = {0},buf2[NVRAM_BUF_LEN] = {0};
	char buf3[NVRAM_BUF_LEN] = {0},buf4[NVRAM_BUF_LEN] = {0};   
    
    
	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);
	if(RADIO_2G == radio )
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "ac_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "ac_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "ac_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "ac_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
		printf("wlan-middleware:ac_bk=%s,ac_be=%s,ac_vi=%s,ac_vo=%s\n",buf1,buf2,buf3,buf4);

	}
	else if(RADIO_5G == radio )
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "ac_bk", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "ac_be", buf2, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "ac_vi", buf3, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "ac_vo", buf4, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
		printf("wlan-middleware:ac_bk=%s,ac_be=%s,ac_vi=%s,ac_vo=%s\n",buf1,buf2,buf3,buf4);

	}
	
	wmmcfg->ac_bk = atoi(buf1);	
	wmmcfg->ac_be = atoi(buf2);	
	wmmcfg->ac_vi = atoi(buf3);	
	wmmcfg->ac_vo = atoi(buf4);	
	

	EXE_doSystem("iwpriv %s setwmmparams 5 0 1 %d",wmmcfg->vap_name,wmmcfg->ac_be);
	
	EXE_doSystem("iwpriv %s setwmmparams 5 1 1 %d",wmmcfg->vap_name,wmmcfg->ac_bk);
	
	EXE_doSystem("iwpriv %s setwmmparams 5 2 1 %d",wmmcfg->vap_name,wmmcfg->ac_vi);
	
	EXE_doSystem("iwpriv %s setwmmparams 5 3 1 %d",wmmcfg->vap_name,wmmcfg->ac_vo);        

	return T_SUCCESS;
	
}

int set_ap_wmm_noack(int radio, int vap_id, wmm_cfg  *wmmcfg)
{

	char buf1[NVRAM_BUF_LEN] = {0};  
	int i=0;
    
  	printf("\nwmmcfg->mode = %d\n",wmmcfg->mode);

    if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl0_ap_wmm_rule", vap_id, "ack_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
    }
	else if(RADIO_2G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl0_sta_wmm_rule", vap_id, "ack_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
	}
    else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_AP)
    {
        ezplib_get_attr_val("wl1_ap_wmm_rule", vap_id, "ack_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
	}
	else if(RADIO_5G == radio && wmmcfg->mode == WLAN_MODE_STA)
    {
        ezplib_get_attr_val("wl1_sta_wmm_rule", vap_id, "ack_enable", buf1, NVRAM_BUF_LEN, EZPLIB_USE_CLI);		
	}	
	wmmcfg->ack_enable = atoi(buf1);
	for(i=0; i<4; i++)
	{
		EXE_doSystem("iwpriv %s setwmmparams 6 %d 0 %d",wmmcfg->vap_name,i,wmmcfg->ack_enable);
	}	

	return T_SUCCESS;
	
}

int set_ap_wmm(int radio, int vap_id)
{
	wmm_cfg  wmmcfg;
	wmm_cfg *wmm = &wmmcfg;
	memset(&wmmcfg, 0, sizeof(wmmcfg));
	int rMode = -1;
	char vap_name[8] = {0};

	nvram_get_wlan_mode(radio, &rMode);
	printf("\nrMode = %d\n",rMode);
		
	if(rMode == WLAN_MODE_STA)
	{
	//	wmm->bss = WMM_SET_STA_ONLY;
		if(radio == RADIO_2G)
		{
			strcpy(wmm->vap_name,"sta0");
		}
		else if(radio == RADIO_5G)
		{
			strcpy(wmm->vap_name,"sta1");
		}
			
	}
	else if(rMode == WLAN_MODE_AP)
	{
	//	wmm->bss = WMM_SET_AP_ONLY;
		
		construct_vap(vap_name, radio, vap_id, rMode);
		
		strcpy(wmm->vap_name,vap_name);
	}
	wmm->mode = rMode;

	set_ap_wmm_enable(radio,vap_id,&wmmcfg);
	printf("\nwmm->wmm_enable=%d,wmm->bss=%d,wmm->vap_name=%s\n",wmm->wmm_enable,wmm->bss,wmm->vap_name);
	if(wmm->wmm_enable == 1)
	{
			/*Admission control is only set on station side, ap side cannot be set*/
		if(rMode == WLAN_MODE_AP)
		{
			set_ap_wmm_cwmin(radio,vap_id,&wmmcfg);
			set_ap_wmm_cwmax(radio,vap_id,&wmmcfg);
			set_ap_wmm_aifs(radio,vap_id,&wmmcfg);
			set_ap_wmm_txop(radio,vap_id,&wmmcfg);
			set_ap_wmm_ac(radio,vap_id,&wmmcfg);
		}

		set_ap_wmm_noack(radio,vap_id,&wmmcfg);
	}

	return T_SUCCESS;	
}

/*********************************/
/*>>>>>>>>>>ACL Process>>>>>>>>>>*/
/*********************************/
int set_acl_policy(int radio, int vap_id)
{
    int policy;
    char vap_name[8];
    int ret;
    char cmd[128];
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    {
        printf("ERRO:Construct vap failure!\n");
        return T_FAILURE;
    }
    nvram_get_ap_acl_policy(radio, vap_id, &policy);
    //Set ACL Policy by CLI
    if(ACL_POLICY_DISABLE == policy)
    {
        sprintf(cmd, "iwpriv %s maccmd %d", vap_name, ATH_ACL_DISABLE);
    }
    else if(ACL_POLICY_ALLOW == policy)
    {
        sprintf(cmd, "iwpriv %s maccmd %d", vap_name, ATH_ACL_ALLOW);
    }
        else if(ACL_POLICY_DENY == policy)
    {
    sprintf(cmd, "iwpriv %s maccmd %d", vap_name, ATH_ACL_DENY);
    }
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}

int acl_add_mac(int radio, int vap_id, char *mac)
{
    char cmd[256];
    char mac_addr[32] = {0};
    char vap_name[8] = {0};
    construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    strcpy(mac_addr, mac);
    sprintf(cmd, "iwpriv %s addmac %s", vap_name, mac_addr);
    EXE_COMMAND(cmd);
    return T_SUCCESS;   
}

int acl_del_mac(int radio, int vap_id, char *mac)
{
    char mac_addr[32] = {0};
    char vap_name[8] = {0};
    char cmd[256];
    construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    strcpy(mac_addr, mac);
    sprintf(cmd, "iwpriv %s delmac %s", vap_name, mac_addr);
    EXE_COMMAND(cmd);
    return T_SUCCESS;   
}

int acl_flush(int radio, int vap_id)
{
	char cmd[128] = {0};
    char vap_name[8] = {0};
    construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    sprintf(cmd, "iwpriv %s maccmd %d", vap_name, ATH_ACL_FLUSH);
    EXE_COMMAND(cmd);
    return T_SUCCESS;   
}

int acl_sync(int radio, int vap_id)
{
	int i = 0;
	int ret = 0;
	int policy = 0;
	char cmd[256] = {0};
	char vap_name[8] = {0};
	char TestStr[2048] = {0};
	ACL_LIST acl_mac_list;
	ACL_LIST asso_mac_list;
	memset(&acl_mac_list, 0, sizeof(ACL_LIST));
	memset(&acl_mac_list, 0, sizeof(ACL_LIST));
	construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);

	//Get ACL Policy
	nvram_get_ap_acl_policy(radio, vap_id, &policy);
	if (T_FAILURE == ret) {
		printf("ERROR:Get ACL Policy error!!!");
		return T_FAILURE;
	} else if (ACL_POLICY_DISABLE == policy) {
		printf("ACL Disable\n");
		return T_SUCCESS;
	}

	//Get Association List and ACL List
	get_acl_sync_list(radio, vap_id, &acl_mac_list, &asso_mac_list);

	//If Association List is Zero, Not Execute Kickmac CMD
	if (0 == asso_mac_list.acl_num) {
		printf("Asso List Zero\n");
		return T_SUCCESS;
	}

	if (ACL_POLICY_ALLOW == policy) {
		if (0 == acl_mac_list.acl_num) {
			for(i = 0; i < asso_mac_list.acl_num; i++) {
				sprintf(cmd, "iwpriv %s kickmac %s", vap_name, asso_mac_list.mac[i].addr);
   				EXE_COMMAND(cmd);
			}
		} else {
			for(i = 0; i < acl_mac_list.acl_num; i++) {
				strncat(TestStr, acl_mac_list.mac[i].addr, 2048);
				strncat(TestStr, "|", 2048);
			}
			
			for (i = 0; i< asso_mac_list.acl_num; i++) {
				if (strstr(TestStr, asso_mac_list.mac[i].addr) == NULL) {
					sprintf(cmd, "iwpriv %s kickmac %s", vap_name, asso_mac_list.mac[i].addr);
   					EXE_COMMAND(cmd);
				}
			}
		}
	} else if (ACL_POLICY_DENY == policy) {
		for(i = 0; i < acl_mac_list.acl_num; i++) {
			sprintf(cmd, "iwpriv %s kickmac %s", vap_name, acl_mac_list.mac[i].addr);
   			EXE_COMMAND(cmd);
		}
	}

	return T_SUCCESS;
}

int set_acl(int radio, int vap_id)
{
    int i = 0;
	int ret = 0;
    int policy = 0;
	ACL_LIST acl_list;
	memset(&acl_list, 0, sizeof(ACL_LIST));

	//Flush ACL List
	acl_flush(radio, vap_id);
	
    //Set ACL List
	nvram_get_ap_acl_list(radio, vap_id, &acl_list);
	for(i=0; i<acl_list.acl_num; i++)
	{
		acl_add_mac(radio, vap_id, acl_list.mac[i].addr);
	}
	
	//Set ACL Policy
	nvram_get_ap_acl_policy(radio, vap_id, &policy);
    set_acl_policy(radio, vap_id);

	//ACL Sync
	ret = acl_sync(radio, vap_id);
	if (T_FAILURE == ret) {
		printf("ERROR:ACL Sync Failure\n");
		return T_FAILURE;
	}
	
    return T_SUCCESS;
}
/*********************************/
/*<<<<<<<<<<ACL Process<<<<<<<<<<*/
/*********************************/
#if 0
/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
int set_device_mode(int radio, int op_mode)
{
    int device_mode;
    char radio_name[RAL_VAP_NAME_LEN];
    char cmd[128];
    int ret;
    switch(op_mode)
        {
            case OP_MODE_NORMAL:
            case OP_MODE_AP:
                device_mode = 0;
                break;  
            case OP_MODE_STA0:
                device_mode = 1;
                break;  
            case OP_MODE_WISP:
                device_mode = 2;
                break;  
            default:
                printf("Error:Such operation mode is not supported current, please check it\n");
                return T_FAILURE;
        }   
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Construct main AP failure!\n");
            return T_FAILURE;
        }
    sprintf(cmd, "iwpriv %s set DeviceMode=%d", radio_name, device_mode);
    printf("CMD in set_device_mode is %s\n", cmd);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}
#endif
/*********************************/
/*<<<<<<<<<<STA Process<<<<<<<<<<*/
/*********************************/


