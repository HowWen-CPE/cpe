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
#include	<assert.h>

#include	"nvram.h"
#include	"nvram_rule.h"
#include    "mid_common.h"
#include    "mid_detail.h"


/**
 * \brief Set STA SSID
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-20
 */
int set_sta_assoc_ssid(int radio)
{
    char ssid[128] = {0};
    char cmd[256] = {0};
    char TempBuf_opmode[8] = {0};
	char ifacename[16] = {0};
	
	
	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(!strstr(TempBuf_opmode, "sta") && !strstr(TempBuf_opmode, "wisp")) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}  
	
	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    /*Get STA SSID From NVRAM*/
	nvram_get_sta_ssid(radio, ssid, 1);
    sprintf(cmd, "iwconfig %s essid \"%s\" ", ifacename, ssid);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}

/**
 * \brief Set STA BSSID
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-21
 */
int set_sta_assoc_bssid(int radio)
{
    char cmd[64] = {0};
    char bssid[32] = {0};
	char ifacename[16] = {0};

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}
	if(T_FAILURE == nvram_get_sta_bssid(radio, bssid)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    /*Get STA BSSID From NVRAM*/
    sprintf(cmd, "iwconfig %s ap %s", ifacename, bssid);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}


/*Set STA Repeater*/
int set_sta_mac_repeater(int radio)
{
	char buf_addrmode[32];
	char buf_repeater[32];
	int addrmode;
	int mac_repeater;
	char mac_repeater_s[32];
	char addrmode_s[32];
	char cmd[128];
	char TempBuf_opmode[32];
	if(!strcmp(TempBuf_opmode, "wisp0")) {
		fprintf(stderr, "MAC Repeater setting is not needed in WISP Mode,disable it.\n");
		sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=0");
		EXE_COMMAND(cmd);
		return T_SUCCESS;
	}

	/*Get Addr Mode*/
	ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", buf_addrmode, 32, EZPLIB_USE_CLI);
	/*Get STA Channel From NVRAM*/
	strcpy(addrmode_s, buf_addrmode);
	addrmode = atoi(addrmode_s);

	/*Get MAC Repeater*/
	ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", buf_repeater, 32, EZPLIB_USE_CLI);
	/*Get STA Channel From NVRAM*/
	strcpy(mac_repeater_s, buf_repeater);
	mac_repeater = atoi(mac_repeater_s);

	if(0 == addrmode) {
		if(1 == mac_repeater) {
			sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_ENABLE);
		}
		else {
			sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_DISABLE);
		}
	}
	else {
		sprintf(cmd, "iwpriv apclii0 set MACRepeaterEn=%d", MAC_REPEATER_DISABLE);
	}

	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief assoc AP which authmode is WEP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-17
 */
int set_sta_assoc_authWEP(int radio)
{
	char ifacename[16] = {0};
	char cmd[64] = {0};
	char key[27] = {0};
	char keyencmode[16] = {0};	// "open", "shared"
	int keyidx = 0;				//1~4
	int keytype = 0;			//0 for HEX, 1 for ASCII
	int encmode = 0;			//4 for "auto", 1 for "open", 2 for "shared"
	int wep_encry = 0;			//0 for 64bits, 1 for 128bits


	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	if(T_FAILURE == nvram_get_sta_wep(radio, key, &keyidx, &keytype, keyencmode, &wep_encry)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	if(!strcmp("open", keyencmode)) {
		encmode = 1;
	}
	else if(!strcmp("shared", keyencmode)) {
		encmode = 2;
	}
	else {
		encmode = 4;
	}

	//Step 1 authmode
	sprintf(cmd, "iwpriv %s authmode %d", ifacename, encmode);
	EXE_COMMAND(cmd);
	
	//Step 2 Key 
	sprintf(cmd, "iwconfig %s key [%d] %s%s", 
			ifacename, keyidx, keytype == 0 ? "":"s:", key);
	EXE_COMMAND(cmd);

	//Step 3 Index
	sprintf(cmd, "iwconfig %s key [%d] ", ifacename, keyidx);
	EXE_COMMAND(cmd);

	//Step 4 key open for open mode only
	if(!strcmp("open", keyencmode)) {
		sprintf(cmd, "iwconfig %s key open", ifacename);
		EXE_COMMAND(cmd);
	}

	return T_SUCCESS;
}

/**
 * \brief create wpa_suplicant config files accroding to radio & authmode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] authmode 
 * 		available collections: WPAPSK/WPA2PSK/WPA/WPA2/WEP/NONE
 * 		invalid   collections: UNDEFINED/WPAPSKWPA2PSK/WPAWPA2
 * \author frank
 * \date 2014-01-24
 * \date 2014-02-27 WPA use proto=WPA, WPA2 use proto=RSN
 */
int create_wpa_supplicant_conf(int radio, int authmode)
{
	assert(AUTHMODE_UNDEFINED != authmode);

	FILE *fp;
	char cmd[128] = {0};
	char ifacename[16] = {0};
	char buffer[14][128];
	memset(buffer, 0, sizeof(buffer));
	char buffer_write[128] = {0};
	int size = 0;

	//Both 802.1X & PSK
	char ssid[NVRAM_SSID_LEN] = {0};
	char bssid[NVRAM_BUF_LEN] = {0};
	int encryp_type =  ENCRY_NONE;
	nvram_get_sta_ssid(radio, ssid, 0);
	nvram_get_sta_bssid(radio, bssid);
	encryp_type = nvram_get_sta_crypto(radio);

	//PSK
	char psk_key[128] = {0};
	nvram_get_sta_psk(radio, psk_key);

	//802.1X
	char wpauser[NVRAM_8021X_NAME_LEN] = {0};
	char wpapasswd[NVRAM_8021X_PWD_LEN] = {0};
	int eap_mode = EAP_MODE_NONE;
	nvram_get_sta_radius_user(radio, wpauser);
	nvram_get_sta_radius_password(radio, wpapasswd);
	eap_mode = nvram_get_sta_eap_mode(radio);

	//WEP
	char key[27] = {0};
	char keyencmode[16] = {0};	// "open", "shared"
	int keyidx = 0;				//1~4
	int keytype = 0;			//0 for HEX, 1 for ASCII
	int encmode = 0;			//4 for "auto", 1 for "open", 2 for "shared"
	int wep_encry = 0;			//0 for 64bits, 1 for 128bits
	nvram_get_sta_wep(radio, key, &keyidx, &keytype, keyencmode, &wep_encry);
	if(!strcmp("open", keyencmode)) {
		encmode = 1;
	}
	else if(!strcmp("shared", keyencmode)) {
		encmode = 2;
	}
	else {
		encmode = 4;
	}


	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "%d@%s unsupported mode error!", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	sprintf(cmd, "rm -rf %s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);
	EXE_COMMAND(cmd);
	memset(cmd, 0, sizeof(cmd));

	sprintf(cmd, "touch %s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);
	EXE_COMMAND(cmd);
	memset(cmd, 0, sizeof(cmd));

	sprintf(cmd, "%s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);

	if(!( fp = fopen(cmd, "wr")))
	{
		fprintf(stderr, "Open %s error\n", cmd);
		return T_FAILURE;
	}

	int i = 0;	
	switch(authmode) {
		case AUTHMODE_WPAPSK:
		case AUTHMODE_WPA2PSK:
			sprintf(buffer[i++], "ctrl_interface=/var/run/wpa_supplicant\n");
			sprintf(buffer[i++], "ap_scan=1\n");
			sprintf(buffer[i++], "network={\n");
			sprintf(buffer[i++], "    ssid=\"%s\"\n", ssid);
			if(0 != *bssid) {
			sprintf(buffer[i++], "    bssid=%s\n", bssid);
			}
			sprintf(buffer[i++], "    scan_ssid=1\n");
			
			//proto
			if (AUTHMODE_WPA2PSK == authmode) {
			sprintf(buffer[i++], "    proto=WPA2\n");
			}
			else if (AUTHMODE_WPAPSK == authmode) {
			sprintf(buffer[i++], "    proto=WPA\n");
			}

			sprintf(buffer[i++], "    key_mgmt=WPA-PSK\n");


#if 1
            //cryption
            /* enable CCMP and TKIP together*/
			sprintf(buffer[i++], "    pairwise=CCMP TKIP\n");
#else
            //cryption
			if (ENCRY_AES == encryp_type) {
			sprintf(buffer[i++], "    pairwise=CCMP\n");
			}
			else if (ENCRY_TKIP == encryp_type) {
			sprintf(buffer[i++], "    pairwise=TKIP\n");
			}
#endif
			sprintf(buffer[i++], "    psk=%s%s%s\n", 
					strlen(psk_key) == 64 ? "":"\"",
					psk_key,
					strlen(psk_key) == 64 ? "":"\"");
			sprintf(buffer[i++], "}\n");
			break;
		case AUTHMODE_WPA:
		case AUTHMODE_WPA2:
			sprintf(buffer[i++], "ctrl_interface=/var/run/wpa_supplicant\n");
			sprintf(buffer[i++], "ap_scan=1\n");
			sprintf(buffer[i++], "network={\n");
			sprintf(buffer[i++], "    ssid=\"%s\"\n", ssid);
			if(0 != *bssid) {
			sprintf(buffer[i++], "    bssid=%s\n", bssid);
			}

			sprintf(buffer[i++], "    scan_ssid=1\n");
			sprintf(buffer[i++], "    key_mgmt=WPA-EAP\n");

			//proto
			if (AUTHMODE_WPA == authmode) {
			sprintf(buffer[i++], "    proto=WPA\n");
			}
			else if (AUTHMODE_WPA2 == authmode) {
			sprintf(buffer[i++], "    proto=RSN\n");
			}

#if 1
            //cryption
            /* enable CCMP and TKIP together*/
			sprintf(buffer[i++], "    pairwise=CCMP TKIP\n");
#else
            //cryption
			if (ENCRY_AES == encryp_type) {
			sprintf(buffer[i++], "    pairwise=CCMP\n");
			}
			else if (ENCRY_TKIP == encryp_type) {
			sprintf(buffer[i++], "    pairwise=TKIP\n");
			}
#endif

			//EAP mode
			if(EAP_MODE_PEAP == eap_mode) {
				sprintf(buffer[i++], "    eap=PEAP\n");
			}
			else if(EAP_MODE_TTLS == eap_mode) {
				sprintf(buffer[i++], "    eap=TTLS\n");
			}

			sprintf(buffer[i++], "    identity=\"%s\"\n", wpauser);
			sprintf(buffer[i++], "    password=\"%s\"\n", wpapasswd);
			sprintf(buffer[i++], "    phase2=\"auth=MSCHAPV2\"\n");
			sprintf(buffer[i++], "}\n");
			break;

		case AUTHMODE_NONE:
			sprintf(buffer[i++], "ctrl_interface=/var/run/wpa_supplicant\n");
			sprintf(buffer[i++], "ap_scan=1\n");
			sprintf(buffer[i++], "network={\n");
			sprintf(buffer[i++], "    ssid=\"%s\"\n", ssid);
			if(0 != *bssid) {
			sprintf(buffer[i++], "    bssid=%s\n", bssid);
			}

			sprintf(buffer[i++], "    scan_ssid=1\n");
			sprintf(buffer[i++], "    key_mgmt=NONE\n");
			sprintf(buffer[i++], "}\n");
			break;

		case AUTHMODE_WEP:
			sprintf(buffer[i++], "ctrl_interface=/var/run/wpa_supplicant\n");
			sprintf(buffer[i++], "ap_scan=1\n");
			sprintf(buffer[i++], "network={\n");
			sprintf(buffer[i++], "    ssid=\"%s\"\n", ssid);
			if(0 != *bssid) {
			sprintf(buffer[i++], "    bssid=%s\n", bssid);
			}
			sprintf(buffer[i++], "    scan_ssid=1\n");
			sprintf(buffer[i++], "    key_mgmt=NONE\n");
			//group
			if (0==wep_encry) {
			sprintf(buffer[i++], "    group=WEP40\n");
			}
			else if (1==wep_encry) {
			sprintf(buffer[i++], "    group=WEP104\n");
			}

			sprintf(buffer[i++], "    wep_key%d=%s%s%s\n",
				keyidx-1, 
				1==keytype? "\"" : "", 
				key,
				1==keytype? "\"" : "");
			sprintf(buffer[i++], "    wep_tx_keyidx=%d\n", keyidx-1);
			//only for shared
			if (2==encmode) {
			sprintf(buffer[i++], "    auth_alg=SHARED\n");
			}
			sprintf(buffer[i++], "}\n");
			break;

		case AUTHMODE_UNDEFINED:
		case AUTHMODE_WPAPSKWPA2PSK:
		case AUTHMODE_WPAWPA2:
		default :
			fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
			return T_FAILURE;
	}

	for(i=0; i<(sizeof(buffer)/sizeof(buffer[0])); i++) {
		memset(buffer_write, 0, sizeof(buffer_write));
		if(0 != *buffer[i]) {
			strcpy(buffer_write, buffer[i]);
		}
		else {
			break;
		}
		printf("buffer_write:%s\n",buffer_write);
		size = fwrite(buffer_write, strlen(buffer_write), 1, fp);
		if (size < 1) 
		{
			fprintf(stderr, "Write %s error\n", cmd);
		}   
	}
	fclose(fp);
	return T_SUCCESS;
}

/**
 * \brief start wpa_supplicant daemon
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-06
 */
int start_apcli_apd_daemon(int radio)
{
	char cmd[256] = {0};
	int secmode;
	char TempBuf_opmode[32] = {0};
	char apcli_br[16] = {0};
	char ifacename[16] = {0};

	/*Get the operation mode*/
	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(strstr(TempBuf_opmode, "sta")) {
		strcpy(apcli_br, nvram_get("lan0_ifname"));
	}
	else if (strstr(TempBuf_opmode, "wisp")) {
		strcpy(apcli_br, nvram_get("wan0_device"));
	}  
	else {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	/*Get security mode of station*/
	secmode = get_sta_assoc_secmode(radio);
	/*Set encryp_type according to security mode and crypto*/
	switch(secmode)
	{
		case AUTHMODE_NONE:
		case AUTHMODE_WEP:
		case AUTHMODE_WPAPSK:
		case AUTHMODE_WPA2PSK:
		case AUTHMODE_WPA:
		case AUTHMODE_WPA2:
			//wpa_supplicant -i sta0 -b br-lan0 -c /tmp/sta0.conf -dd -K &
			sprintf(cmd, "/usr/sbin/wpa_supplicant -i %s -b %s -c %s/%s.conf -P %s/wpa_supplicant_%s.pid -B", 
				ifacename, apcli_br, 
				WPA_SUPPLICANT_CFG_PATH, ifacename, 
				WPA_SUPPLICANT_PID_PATH, ifacename);
			break;
		default:
			fprintf(stderr, "ERROR:wpa_supplicant is only used for PSK & 802.1x\n");
			return T_FAILURE;
	}

	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief Kill wpa_supplicant according to PID file(/var/run/wpa_supplicant_[sta0|sta1].pid)
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-24
 * \notice If we switch from one radio to another(both WPA), MAKE SURE previous 
 *			supplicant is killed!!!
 */
int kill_apcli_apd_daemon(int radio)
{
    char cmd[128] = {0};
	char ifacename[16] = {0};

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    sprintf(cmd, "kill `cat %s/wpa_supplicant_%s.pid`", WPA_SUPPLICANT_PID_PATH, ifacename);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}


/**
 * \brief get BSSID from the associated AP
 * \return T_SUCCESS on successfully get BSSID, T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] bssid the result BSSID
 * \author frank
 * \date 2014-01-07
 */
int get_sta_assoc_bssid(int radio, char *bssid)
{
	char TempBuf_opmode[8] = {0};
	char ifacename[16] = {0};

	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(!strstr(TempBuf_opmode, "sta") && !strstr(TempBuf_opmode, "wisp")) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}  

	int ret = construct_vap(ifacename, radio, 0, WLAN_MODE_STA);
	if(T_FAILURE == ret) {
		return ret;
	}

    char cmd[128] = {0};
    FILE *fin = NULL;
    char c;
    //Get the last field on line containing 'Access Point' but doesn't contains Not-Associated
    sprintf(cmd, "iwconfig %s | awk '/Access Point/{if(0==match($0, \"Not-Associated\")){print $NF}}'> /tmp/sta_bssid", ifacename);
    EXE_COMMAND(cmd);
    fin = fopen("/tmp/sta_bssid","r");

	if(!fin) {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		fclose(fin);
		return T_FAILURE;
	}

	assert(bssid!=NULL);
    /*while ((c=fgetc(fin)) != EOF){
        ungetc(c,fin);        
        readline(bssid,fin);
		break;
    }*/
    if (fgets(bssid, 18, fin) == NULL) {
		fclose(fin);
		return T_FAILURE;
	}
	
    fclose(fin);
    EXE_COMMAND("rm -f /tmp/sta_bssid");    
    /*Get STA BSSID From NVRAM*/
    return T_SUCCESS;
}

/**
 * \brief get RSSI from the associated AP
 * \return T_SUCCESS on successfully get BSSID, T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] rssi the result RSSI 
 * \author frank
 * \date 2014-01-07
 */
int get_sta_assoc_rssi(int radio, char *rssi)
{
	char TempBuf_opmode[8] = {0};
	char ifacename[16] = {0};

	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(!strstr(TempBuf_opmode, "sta") && !strstr(TempBuf_opmode, "wisp")) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}  

	int ret = construct_vap(ifacename, radio, 0, WLAN_MODE_STA);
	if(T_FAILURE == ret) {
		return ret;
	}

	int num = 0;
	int rssiV = 0;
	char rssiStr[8] = {0};
	char cmd[256] = {0};
	//Chged by Andy Yu in 20140224: Assocapinfo Information Format Chged
	sprintf(cmd, "wlanconfig %s list assocapinfo "
		"| awk '/Chain/{gsub(\"dBm\",\"\"); print substr($0,13)}' > /tmp/rssi.dat", ifacename);
	EXE_COMMAND(cmd);

    FILE *fp;
    if (NULL == (fp = fopen("/tmp/rssi.dat", "r"))) {
        return T_FAILURE;
    } 
    else {
		assert(NULL!=rssi);
		while(!feof(fp))
		{
			if(EOF == fscanf(fp, "%d", &rssiV))
			{
				//printf("Get RSSI End\n");
				break;
			}

			sprintf(rssiStr, "%d", rssiV);
			if(num!=0)
			{
				strcat(rssi, "/");
			}
			strcat(rssi, rssiStr);
			num++;
		}
		if(strcmp(rssi, ""))
		{
			strcat(rssi, " dBm");
		}
    }
	
    fclose(fp);
	EXE_COMMAND("rm -fr /tmp/rssi.dat");
    return T_SUCCESS;
}

//Added By Andy Yu in 2014/01/23: Get Ethernet Client List
int get_ethernet_client_list(int radio, ETHERNET_CLIENT_LIST *client_list)
{
	int i = 0;
	int j = 0;
	int ret = 0;
	int rMode = 0;
	FILE *fp = NULL;
	char ip_buf[64] = { 0 };
	char mac_buf[20] = { 0 };
	char tmp_buf[64] = { 0 };
	char vap_name[8] = { 0 };
	char ModeTmpBuf[32] = { 0 };
	char cmd[128] = { 0 };
	client_list->client_num = 0;

	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		} else {
			printf("2.4G Not in Station Mode\n");
			return T_FAILURE;
		}
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		} else {
			printf("5G Not in Station Mode\n");
			return T_FAILURE;
		}
	}
	else {
		printf("ERROR:Radio error!\n");
        return T_FAILURE;
	}

	/* Get br-lan0 MAC */
	ret = get_lan_mac(tmp_buf);
	printf("++++++++++%s+++++++++++\n", tmp_buf);
	if (T_FAILURE == ret) {
		printf("ERROR: Get LAN MAC Error\n");
		return T_FAILURE;
	}

	/* Get vap name: sta0 or sta1 */
	ret = construct_vap(vap_name, radio, 0, rMode);
	if (T_FAILURE == ret) {
		printf("ERROR: Get VAP Name Error\n");
		return T_FAILURE;
	}

	sprintf(cmd, "wlanconfig %s list extsta | awk '{if(NR!=1) print $0}' > /tmp/ethCliList", vap_name);
	EXE_COMMAND(cmd);
	
	if (NULL == (fp = fopen("/tmp/ethCliList", "r"))) {
		return T_FAILURE;
	}

	i = 0;
	while (!feof(fp)) {
		ret = fscanf(fp, "%d%s%s", &j, mac_buf, ip_buf);
		if (j > 128) {
			printf("Client Max Number is 128\n");
			break;
		}
		if ( ret == EOF) {
			printf("fsacnf End\n");
			break;
		}

		if ((mac_buf[2] != ':') || (mac_buf[5] != ':') || (mac_buf[8] != ':') || (mac_buf[11] != ':')
				|| (mac_buf[14] != ':') || (!strcmp(mac_buf, tmp_buf))) {
			continue;
		}

		memset(client_list->client[i].macaddr, 0x00, sizeof(client_list->client[i].macaddr));
		memset(client_list->client[i].ipaddr, 0x00, sizeof(client_list->client[i].ipaddr));
		strcpy(client_list->client[i].macaddr, mac_buf);
		strcpy(client_list->client[i].ipaddr, ip_buf);
		printf("%d %s %s\n", i, client_list->client[i].macaddr, client_list->client[i].ipaddr);
		memset(mac_buf, 0, 20);
		memset(ip_buf, 0, 64);
		i++;
	}
	client_list->client_num = i;

	fclose(fp);
	sprintf(cmd, "rm -rf /tmp/ethCliList");
	EXE_COMMAND(cmd);
	
    return T_SUCCESS;
}

/*Get STA Current SSID*/
/**
 * \brief get the ssid of associated AP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] ssid the associated AP
 * \author frank
 * \date 2014-01-21
 */
int get_sta_assoc_ssid(int radio, char *ssid)
{
	char TempBuf_opmode[8] = {0};
	char ifacename[16] = {0};

	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(!strstr(TempBuf_opmode, "sta") && !strstr(TempBuf_opmode, "wisp")) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}  

	int ret = construct_vap(ifacename, radio, 0, WLAN_MODE_STA);
	if(T_FAILURE == ret) {
		return ret;
	}

	int bAssoc = 0;
	if(T_FAILURE == get_sta_assoc_status(radio, &bAssoc)) {
		return T_FAILURE;
	}
	if(STA_NOT_ASSOCIATED == bAssoc) {
		return T_FAILURE;
	}

    char cmd[128] = {0};
    FILE *fin = NULL;
    char c;
    //use sed to get ssid
	sprintf(cmd, "iwconfig %s | awk '/ESSID/{print}' | sed 's/[^\"]*\"\\(.*\\)\"[^\"]*/\\1/g' > /tmp/sta_ssid", ifacename);
	EXE_COMMAND(cmd);
	fin = fopen("/tmp/sta_ssid","r");
	if(NULL == fin) {
		return T_FAILURE;
	}
	int i = 0;
	assert(ssid!=NULL);
	while ((c=fgetc(fin)) != EOF){
		if( i > 31 || '\n' == c ) break;	
		ssid[i] = c;
		i++;	
	}
	ssid[i] = '\0';
	fclose(fin);
	EXE_COMMAND("rm -f /tmp/sta_ssid");    

    return T_SUCCESS;
}

/*Get STA Connected AP Wireless Mode*/
int get_sta_assoc_wmode(int radio, char *wmode)
{
    char cmd[128];
    char bssid[32], converted_bssid[32];
    FILE *fp;
    char *pBssidList = (char*)malloc(256 * sizeof (char));
    char tmpBSSID[28], tmpWMODE[16];
    char bssid_get[28], wireless_mode_get[16];

    get_sta_assoc_bssid(radio, bssid);
    //printf("bssid is %s\n", bssid);
    /*Set apcli site survey as enable*/    
    sprintf(cmd, "iwpriv apclii0 set SiteSurvey=1");
    EXE_COMMAND(cmd);    
    memset(cmd, 0, sizeof(cmd));
    /*Carry out site survey, restore the result to /tmp/site_survey_middleware.dat file*/    
    format_bssid_upper(bssid, converted_bssid);
    sprintf(cmd, "iwpriv apclii0 get_site_survey | grep \"%s\" > /tmp/site_survey_middleware.dat", converted_bssid);
    EXE_COMMAND(cmd);    
    //printf("%s\n", cmd);
    memset(cmd, 0, sizeof(cmd));

    if (NULL == (fp = fopen("/tmp/site_survey_middleware.dat","r")))
        {
            printf("Ap list is NONE\n");
        free(pBssidList);
        return T_FAILURE;
        }
    else
        {
            //fgets(pBssidList, 256, fp);
            //fgets(pBssidList, 256, fp);

            while(!feof(fp)){
                memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
                memset(tmpWMODE, 0x00, sizeof(tmpWMODE));
                memset(bssid_get, 0x00, sizeof(bssid_get));
                memset(wireless_mode_get, 0x00, sizeof(wireless_mode_get));

                fgets(pBssidList, 256, fp);

                snprintf(tmpBSSID, 19, "%s", pBssidList+39);
                format_bssid_upper(tmpBSSID, bssid_get);
                //printf("bssid_get is %s\n", bssid_get);

                snprintf(tmpWMODE, 9, "%s", pBssidList+90);
                strcpy_delspace(tmpWMODE, wireless_mode_get);
                //printf("wireless_mode_get is %s\n", wireless_mode_get);

                if(!strcmp(bssid, bssid_get))
                    {
                        strcpy(wmode, wireless_mode_get);             
                        //printf("wmode is %s\n", wmode);
                        break;
                    }
            }
        }

    free(pBssidList);
    fclose(fp);
    //sprintf(cmd, "rm -rf /tmp/site_survey_middleware.dat");
    //EXE_COMMAND(cmd);    

    return T_SUCCESS;
}

/**
 * \brief get the authmode of associated AP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] authmode of associated AP
 * \author frank
 * \date 2014-01-08
 */
int get_sta_assoc_authmode(int radio, int *auth_mode, int *enc_type)
{
	char TempBuf_opmode[8] = {0};
	char auth_mode_get[32] = {0};
    char cmd[128] = {0};
	char ifacename[16] = {0};
	int bAssoc = 0;
    FILE *fin = NULL;

	ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	if(!strstr(TempBuf_opmode, "sta") && !strstr(TempBuf_opmode, "wisp")) {
		fprintf(stderr, "%d@%s unsupported mode error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}  

	int ret = construct_vap(ifacename, radio, 0, WLAN_MODE_STA);
	if(T_FAILURE == ret) {
		return ret;
	}

	if(T_FAILURE == get_sta_assoc_status(radio, &bAssoc)) {
		return T_FAILURE;
	}
	if(STA_NOT_ASSOCIATED == bAssoc) {
		return T_FAILURE;
	}

	//sprintf(cmd, "wlanconfig %s list assocapinfo | awk '{if(2==NR){print}}' | cut -b56-78 > /tmp/sta_security", ifacename);
	//Chged By Andy Yu in 20140214
	sprintf(cmd, "wlanconfig %s list assocapinfo | awk '/Security/{print substr($0,10)}' > /tmp/sta_security", ifacename);
	EXE_COMMAND(cmd);

	fin = fopen("/tmp/sta_security","r");
	if(NULL == fin) {
		return T_FAILURE;
	}

	if(fscanf(fin, "%s", auth_mode_get) <= 0) {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	if(strstr(auth_mode_get, "WPA1PSKWPA2PSK")){
		*auth_mode = AUTHMODE_WPAPSKWPA2PSK;
	}
	else if(strstr(auth_mode_get, "WPA2PSK")) {
		*auth_mode = AUTHMODE_WPA2PSK;
	}
	else if(strstr(auth_mode_get, "WPAPSK")) {
		*auth_mode = AUTHMODE_WPAPSK;
	}
	else if(strstr(auth_mode_get, "WPA1WPA2")) {
		*auth_mode = AUTHMODE_WPAWPA2;
	}
	else if(strstr(auth_mode_get, "WPA2")) {
		*auth_mode = AUTHMODE_WPA2;
	}
	else if(strstr(auth_mode_get, "WPA")) {
		*auth_mode = AUTHMODE_WPA;
	}
	else if(strstr(auth_mode_get, "SHARED") 
		|| strstr(auth_mode_get, "WEP")
		|| strstr(auth_mode_get, "OPEN")) {
		*auth_mode = AUTHMODE_WEP;
	}
	else if(strstr(auth_mode_get, "NONE")) {
		*auth_mode = AUTHMODE_NONE;
	}
	else {
		*auth_mode = AUTHMODE_UNDEFINED;
		fprintf(stderr, "%d@%s unknow authmode:%s\r\n", __LINE__, __FUNCTION__, auth_mode_get);
	}

	if (strstr(auth_mode_get, "TKIPAES")) {		
		*enc_type = ENCRY_TKIPAES;
	} else if (strstr(auth_mode_get, "TKIP")) {
		*enc_type = ENCRY_TKIP;
	} else if (strstr(auth_mode_get, "AES")) {
		*enc_type = ENCRY_AES;
	} else if (strstr(auth_mode_get, "NONE")) {
		*enc_type = ENCRY_NONE;
	} else {
		*enc_type = AUTHMODE_UNDEFINED;
	}

	fclose(fin);
	EXE_COMMAND("rm -f /tmp/sta_security");    

	return T_SUCCESS;
}

/**
 * \brief get information about associated AP
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] ap_info (SSID, BSSID, RSSI, AUTHMODE)
 * \author frank
 * \date 2014-01-08
 * \date 2014-03-25 update return value
 */
int get_sta_assoc_ap_info(int radio, AP_INFO *ap_info)
{
	int ret = 0;
	assert(NULL!=ap_info);
	ret += get_sta_assoc_ssid(radio, ap_info->ssid);
	ret += get_sta_assoc_bssid(radio, ap_info->bssid);
	ret += get_sta_assoc_rssi(radio, ap_info->rssi);
	ret += get_sta_assoc_authmode(radio, &(ap_info->auth_mode), &(ap_info->enc_type));
	return T_SUCCESS==ret ? T_SUCCESS: T_FAILURE;
}


/*Get STA Association Status*/
int get_sta_assoc_status(int radio, int *assoc)
{
	char bssid[32] = {0};

	if(T_FAILURE == get_sta_assoc_bssid(radio, bssid)) {
		return T_FAILURE;
	}

	if(!strcmp(bssid, "")) {
		*assoc = STA_NOT_ASSOCIATED;
	}
	else {
		*assoc = STA_ASSOCIATED;
	}
	return T_SUCCESS;
}

/**
 * \brief set wirelessmode before associatation
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-17
 */
int set_sta_assoc_wirelessmode(int radio)
{
	char cmd[64] = {0};
	char ifacename[16] = {0};
	char htbwBuf[2] = {0};
	char ModeTmpBuf[4] = {0};
	int netmode = 0;

    int ret = construct_vap(ifacename, radio, 0, WLAN_MODE_STA);
    if(T_FAILURE == ret) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}
	
	if ( RADIO_2G == radio) {
		ezplib_get_attr_val("wl_advanced_rule", 0, "htbw", htbwBuf, 2, EZPLIB_USE_CLI);
		switch(atoi(htbwBuf)) {
			case 0:			//20MHz
			    #ifdef SINGLE_RADIO_5G
                sprintf(cmd, "iwpriv %s mode 11NAHT20", ifacename);
                #else
				sprintf(cmd, "iwpriv %s mode 11NGHT20", ifacename);
                #endif
				break;
			case 1:			//20/40MHz
			    #ifdef SINGLE_RADIO_5G
                sprintf(cmd, "iwpriv %s mode 11NAHT40", ifacename);
                #else
				sprintf(cmd, "iwpriv %s mode 11NGHT40", ifacename);
                #endif
				
				break;		
			default:
				fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
				return T_FAILURE;
		}
		EXE_COMMAND(cmd);
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_advanced_rule", 0, "htbw", htbwBuf, 2, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", ModeTmpBuf, 4, EZPLIB_USE_CLI);
		netmode = atoi(ModeTmpBuf);	
		printf("__%d@%s netmode = %d\r\n", __LINE__, __FUNCTION__, netmode);	
		/* 11a/n mode */
		if (8 == netmode) {
			switch(atoi(htbwBuf)) {
				case 0:			//20MHz
					sprintf(cmd, "iwpriv %s mode 11NAHT20", ifacename);
					break;
				case 1:			//20/40MHz
					sprintf(cmd, "iwpriv %s mode 11NAHT40", ifacename);
					break;		
				default:
					fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
					return T_FAILURE;
			}
		}
		/* 11ac mode */
		else if (14 == netmode) {
			switch(atoi(htbwBuf)) {
				case 0:			//20MHz
					sprintf(cmd, "iwpriv %s mode 11ACVHT20", ifacename);
					break;
				case 1:			//40MHz
					sprintf(cmd, "iwpriv %s mode 11ACVHT40", ifacename);
					break;		
				case 2:			//80MHz
					sprintf(cmd, "iwpriv %s mode 11ACVHT80", ifacename);
					break;		
				default:
					fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
					return T_FAILURE;
			}
		}
		else {
			fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
			return T_FAILURE;
		}

		EXE_COMMAND(cmd);
	}
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}
	return T_SUCCESS;
}


/**
 * \set rssi threshold (connrssi & disconnrssi)
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author Peter
 * \date 2014/6/26
 */
int set_sta_rssi_threshold(int radio)
{

	char cmd[128] = {0};
	char ifacename[16] = {0}, buf[TMP_LEN]={0};
	int connrssi = 0, disconnrssi = 0, delta_connrssi = 0, delta_disconnrssi = 0;

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	ezplib_get_attr_val("wl0_apcli_rule", 0, "connrssi", buf, TMP_LEN, EZPLIB_USE_CLI);
    sscanf(buf, "%d", &connrssi);


    ezplib_get_attr_val("wl0_apcli_rule", 0, "disconnrssi", buf, TMP_LEN, EZPLIB_USE_CLI);
    sscanf(buf, "%d", &disconnrssi);


	if(connrssi < -85 || connrssi > -45)
	{
		printf("Err: connrssi = %d out of range, set to default (-85)\n", connrssi);
        connrssi = -85;
	}

	if((disconnrssi < -95 || disconnrssi > -55))
	{
		printf("Err: disconnrssi = %d out of range, set to default (-95)\n", disconnrssi);
        connrssi = -95;
	}

	if(connrssi - disconnrssi < 10)
	{
	    printf("Err: connrssi - disconnrssi %d < 10\n", connrssi - disconnrssi);

        connrssi = disconnrssi + 10;
	} 

    delta_connrssi = connrssi + 95;
    delta_disconnrssi = disconnrssi + 95;

    /* set connrssi threshold*/
	sprintf(cmd, "iwpriv %s connrssi %d", ifacename, delta_connrssi); 
	EXE_COMMAND(cmd);

    /* set disconnrssi threshold*/
	sprintf(cmd, "iwpriv %s disconnrssi %d", ifacename, delta_disconnrssi); 
	EXE_COMMAND(cmd);

	return T_SUCCESS;
}


/**
 * \brief set Extra Parameter on switching to STA Mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-29
 */
int set_extra_sta(int radio)
{
	char cmd[128] = {0};
	char ifacename[16] = {0};

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    /* Extra command for mode switch */
	sprintf(cmd, "iwpriv %s extap 1", ifacename);	
	EXE_COMMAND(cmd);
	sprintf(cmd, "iwpriv wifi%d dcs_enable 0", radio);	
	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief set Radio radio to STA Mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-02-08
 * \date 2014-02-27 add macro for compile
 */
int set_config_sta(int radio)
{
	char cmd[128] = {0};
	char ifacename[16] = {0};

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}
	/* Down Interface first */
#if 1
        //Modified by Mario Huang,down first and terminate later
        vap_up_down(radio, 0, WLAN_MODE_STA, VAP_DOWN);
	/* use wpa_cli terminate to stop wpa_supplicant */
	sprintf(cmd, "/usr/sbin/wpa_cli -p /var/run/wpa_supplicant -i %s terminate", ifacename);
	EXE_COMMAND(cmd);
#else
	/* use kill `cat pidfile` to stop wpa_supplicant */
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_DOWN);
	kill_apcli_apd_daemon(radio);
#endif

	/* Power & Wireless Mode */
	//set_power(radio);
	set_sta_assoc_wirelessmode(radio);

    /*set rssi threshold*/
    set_sta_rssi_threshold(radio);

    /* set address mode*/
    set_sta_addr_mode(radio);
    
    /*Get security mode of station*/
    int secmode = nvram_get_sta_secmode(radio);
	switch(secmode) {
#if 0
		/* USE original iwconfig/iwpriv to config WEP and NONE*/
		case AUTHMODE_NONE:
			sprintf(cmd, "iwconfig %s key off", ifacename);		
			EXE_COMMAND(cmd);
			sprintf(cmd, "iwpriv %s authmode 1", ifacename);		
			EXE_COMMAND(cmd);
			set_sta_assoc_ssid(radio);
			set_sta_assoc_bssid(radio);
			vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
			break;
		case AUTHMODE_WEP:
			sprintf(cmd, "iwconfig %s key off", ifacename);		
			EXE_COMMAND(cmd);
			set_sta_assoc_authWEP(radio);
			set_sta_assoc_ssid(radio);
			set_sta_assoc_bssid(radio);
			vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
			break;
#else
		/* USE wpa_supplicant to config WEP and NONE*/
		case AUTHMODE_NONE:
		case AUTHMODE_WEP:
#endif
		case AUTHMODE_WPAPSK:
		case AUTHMODE_WPA2PSK:
		case AUTHMODE_WPAPSKWPA2PSK:
		case AUTHMODE_WPA:
		case AUTHMODE_WPA2:
		case AUTHMODE_WPAWPA2:
			vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
			create_wpa_supplicant_conf(radio, secmode);
			start_apcli_apd_daemon(radio);
			break;
		case AUTHMODE_UNDEFINED:
			fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
			break;
	}
	set_power(radio);

    return T_SUCCESS;
}


/**
 * \brief get AP list according to radio
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] ap_list scan AP result list
 * \author andy
 * \date 2014-01
 */
int get_ap_list(int radio, SCAN_AP_LIST *ap_list)
{
	FILE *fp;
	int ret = 0;
	int rMode = 0;
	int apNum = 0;
	int apNTWType = 0;
	int apChannel = 0;
	int apSignal = 0;
	int apEncryption = 0;
	int apAuthmode = 0;
	char cmd[128] = {0};
	char apSsid[64] = {0};
	char vap_name[8] = {0};
	char apWMode[8] = {0};
	char apBssid[32] = {0};
	char ModeTmpBuf[32] = {0};
    char *pBssidList = (char*)malloc(65536 * sizeof (char));
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		}
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(ModeTmpBuf, "ap")) {
			rMode = WLAN_MODE_AP;
		}
		else if (!strcmp(ModeTmpBuf, "client")) {
			rMode = WLAN_MODE_STA;
		}
	}
	else {
		fprintf(stderr, "ERROR:Radio error!\n");
        return T_FAILURE;
	}
	
	construct_vap(vap_name, radio, 0, rMode);
	
    /*iwlist staN scanning | awk -f /etc/wl/rogueap.awk*/    
    sprintf(cmd, "iwlist %s scanning | awk -f /etc/wl/rogueap.awk > /tmp/scanrlt", vap_name);
    EXE_COMMAND(cmd);    
    
    /*Set ap_num as 0 first*/
    ap_list->ap_num = 0;
    if (NULL == (fp = fopen("/tmp/scanrlt","r")))
    {
        fprintf(stderr, "Ap list is NONE\n");
        free(pBssidList);
        return T_FAILURE;        
    }
    else
    {
        while(!feof(fp)){
			memset(apBssid, 0x00, sizeof(apBssid));
			memset(apSsid, 0x00, sizeof(apSsid));
			memset(apWMode, 0x00, sizeof(apWMode));

			ret = fscanf(fp, "%d%s%d%d%d%s%d%d%[^\n]", &apNum, apBssid, 
				&apNTWType, &apChannel, &apSignal, apWMode, &apEncryption, &apAuthmode, apSsid);
			if ( 9 != ret) {
				printf("fscanf Number:%d\n", ret);
				printf("fsacnf End\n");
				break;
			}

			memset(ap_list->ap[apNum - 1].ssid, 0x00, sizeof(ap_list->ap[apNum - 1].ssid));
            memset(ap_list->ap[apNum - 1].bssid, 0x00, sizeof(ap_list->ap[apNum - 1].bssid));
			memset(ap_list->ap[apNum - 1].rssi_str, 0x00, sizeof(ap_list->ap[apNum - 1].rssi_str));
			memset(ap_list->ap[apNum - 1].wmode, 0x00, sizeof(ap_list->ap[apNum - 1].wmode));

			/* AP BSSID */
			strcpy(ap_list->ap[apNum - 1].bssid, apBssid);

			/* AP Network Type: 0->Auto; 1->Ad-hoc; 2->Managed; 3->Repeater; 4->Master; 5->Secondary; 6->Monitor; 7->Unknow*/
			if (4 == apNTWType) {
				ap_list->ap[apNum - 1].network_type = 1;
			}
			else {
				ap_list->ap[apNum - 1].network_type = 0;
			}

			/* AP Channel*/
			ap_list->ap[apNum - 1].channel = apChannel;

			/* AP Signal*/
			ap_list->ap[apNum - 1].rssi = - apSignal;
			sprintf(ap_list->ap[apNum - 1].rssi_str, "%d dBm", ap_list->ap[apNum - 1].rssi);
			
			/* AP Wireless Mode*/
			strcpy(ap_list->ap[apNum - 1].wmode, apWMode);

			/* AP Encryption*/
			ap_list->ap[apNum -1].encry = apEncryption;

			/* AP Auth_mode*/
			ap_list->ap[apNum -1].auth_mode = apAuthmode;

			/* AP SSID*/
			strncpy(ap_list->ap[apNum -1].ssid, &apSsid[2], (strlen(apSsid) - 3));

			/* AP Number */
			ap_list->ap_num = apNum;
			if(ap_list->ap_num == 128)
            {
                break;
            }
        }
    }

    free(pBssidList);
    fclose(fp);
#if 1
    sprintf(cmd, "rm -rf /tmp/scanrlt");
	EXE_COMMAND(cmd);
#else
        int ap_num = ap_list->ap_num;
        printf("ap_num after is:%d\n",ap_num);
        int j;
        for(j=0; j< ap_num; j++)
            {
                printf("SSID%d:%s\n",j,ap_list->ap[j].ssid);
                printf("BSSID%d:%s\n",j,ap_list->ap[j].bssid);
                printf("RSSI%d:%d\n",j,ap_list->ap[j].rssi);
                printf("Channel%d:%d\n",j,ap_list->ap[j].channel);
                printf("AuthMode%d:%d\n",j,ap_list->ap[j].auth_mode);
                printf("ENCRY%d:%d\n",j,ap_list->ap[j].encry);
                printf("NET_TYPE%d:%d\n",j,ap_list->ap[j].network_type);
            }
#endif

    return T_SUCCESS;    
}
/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
int get_sta_assoc_secmode(radio)
{
	return nvram_get_sta_secmode(radio);
}
#if 0
int set_device_mode(int radio, int op_mode)
{
    int device_mode;
    char radio_name[8];
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
            fprintf(stderr, "ERRO:Construct main AP failure!\n");
            return T_FAILURE;
        }
    sprintf(cmd, "iwpriv %s set DeviceMode=%d", radio_name, device_mode);
    printf("CMD in set_device_mode is %s\n", cmd);
    EXE_COMMAND(cmd);
    return T_SUCCESS;
}
#endif
int set_apcli_disconnect_ap(int radio)
{
    return T_SUCCESS;
}

int config_apcli(int radio)
{
    return T_SUCCESS;
}

int set_apcli_basic(int radio)
{
    return T_SUCCESS;
}

/*Set STA Disallow TKIP*/
int set_sta_disallow_tkip(int radio, int disallow)
{
    return T_SUCCESS;
}

int set_sta_authmode(int radio)
{
    return T_SUCCESS;
}

int set_sta_encryp_type(int radio)
{
    return T_SUCCESS;
}

int set_sta_wpa_psk(int radio)
{
    return T_SUCCESS;
}

int set_sta_default_keyid(int radio)
{
    return T_SUCCESS;
}

int set_sta_wep_key(int radio)
{
    return T_SUCCESS;
}

int set_sta_enable(int radio, int enable)
{
    return T_SUCCESS;
}

int set_sta_addr_mode(int radio)
{
    char cmd[128]={0};
    char TempBuf_opmode[32]={0};
    char buf[32]={0};
    char addrmode[32]={0};
    char ifacename[16] = {0};

	if(T_FAILURE == construct_vap(ifacename, radio, 0, WLAN_MODE_STA)) {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    if(RADIO_2G == radio)
    {
        ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", buf, 32, EZPLIB_USE_CLI);
    }
    else
    {
        ezplib_get_attr_val("wl1_apcli_rule", 0, "addrmode", buf, 32, EZPLIB_USE_CLI);
    }
    strcpy(addrmode, buf);

    if(RADIO_2G == radio)
	{
	    ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
	}
    if(!strcmp(TempBuf_opmode, "wisp0"))
    {
        strcpy(addrmode, "0");
    }

    /* set to driver*/
    if(*addrmode == '0')
    {
        /* disable sta-wds, enable sta-ext*/
        sprintf(cmd, "iwpriv %s wds 0 && iwpriv %s extap 1", ifacename, ifacename);
        EXE_COMMAND(cmd);
    }else if(*addrmode == '1')
    {
        /* enable sta-wds, disable sta-ext*/
        sprintf(cmd, "iwpriv %s wds 1 && iwpriv %s extap 0", ifacename, ifacename);
        EXE_COMMAND(cmd);
    }else
    {
        printf("ERROR address mode %s, ignored!!!\n", addrmode);
    }
    
    return T_SUCCESS;
}


/*Set STA Security*/
int set_sta_security(int radio)
{
    set_sta_authmode(radio);
    set_sta_encryp_type(radio);
    set_sta_wpa_psk(radio);
    set_sta_default_keyid(radio);
    set_sta_wep_key(radio);
    return T_SUCCESS;
}
