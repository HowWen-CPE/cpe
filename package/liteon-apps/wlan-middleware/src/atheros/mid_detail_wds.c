#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<assert.h>

#include    "mid_detail.h"
#include	"mid_common.h"
#include	"mid_common_nvram.h"

#include	"mid_detail_wds.h"

//#define	WDS_ESSID "Generic-WDS"
#define	WDS_ESSID "Generic-WDS"


/**
 * \brief get WDS CAP according to RADIO's MODE/BANDWIDTH/ATTENASTREAM
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-06-13
 */
unsigned char get_wds_cap(int radio)
{
#define BIT_HT20 (1<<0)
#define BIT_HT2040 (1<<1)

#define BIT_DUAL_STREAM (1<<2)
#define BIT_TRI_STREAM (1<<3)

#define BIT_VHT20 (1<<5)
#define BIT_VHT40 (1<<6)
#define BIT_VHT80 (1<<7)
	unsigned char cap = 0;
	int band_width = 0;
	int wireless_mode = 0;
	nvram_get_bandwidth(radio, &band_width);
	nvram_get_ap_wireless_mode(radio, &wireless_mode);

	switch(wireless_mode) {
		case WMODE_11AN:
		case WMODE_11BGN:
			switch(band_width) {
				case BANDWIDTH_20:
					cap |= BIT_HT20;
					break;
				case BANDWIDTH_40:
					cap |= BIT_HT2040;
					break;
				case BANDWIDTH_80:
				default:
					break;
			}
			break;
		case WMODE_11AC_MIXED:
			switch(band_width) {
				case BANDWIDTH_20:
					cap |= BIT_VHT20;
					break;
				case BANDWIDTH_40:
					cap |= BIT_VHT40;
					break;
				case BANDWIDTH_80:
					cap |= BIT_VHT80;
					break;
				default:
					break;
			}
			break;
	}

#if defined(GP_WP838)
	//cap |= BIT_TRI_STREAM;
	cap |= BIT_DUAL_STREAM; // peter modified for 2 streams.
#elif defined(GP_WR869)
	cap |= BIT_TRI_STREAM;
#else
	#error "Unsupported Product Name"	
#endif
	DEBUG("cap=%d", cap);
	return cap;
}

/**
 * \brief clear WdsList by invoke script /etc/wl/wds-clear
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-06-13
 */
int clear_wds_list(int radio, int vap_id)
{
	char cmd[32] = {0};
    int actual_id = 0;
    actual_id = convert_vap_id(radio, vap_id); 

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", actual_id);
		return T_FAILURE;
	}

	sprintf(cmd, "/etc/wl/wds-clear %d", actual_id);
	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief add MAC address into specified wdsN
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \param[in] mac XX:XX:XX:XX:XX:XX
 * \author frank
 * \date 2014-06-13
 */
int add_wds_list(int radio, int vap_id, const char *mac)
{
	assert(mac);
    int actual_id = 0;
    actual_id = convert_vap_id(radio, vap_id); 

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", actual_id);
		return T_FAILURE;
	}

	//prevent set empty mac addr
	if(!strcmp(mac, "")) {
		fprintf(stderr, "Target MAC for wds%d is Empty!\r\n", actual_id);
		return T_FAILURE;
	}

	char cmd[128] = {0};
	sprintf(cmd, "wlanconfig wds%d nawds add-repeater %s 0x%02X",
		actual_id, mac, (int)get_wds_cap(radio));
	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief create security dependent config files need by wpa_supplicant
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int create_wds_conf(int radio, int vap_id)
{
	FILE *fp;
	char cmd[128] = {0};
	char ifacename[16] = {0};
	char buffer[14][128];
	memset(buffer, 0, sizeof(buffer));
	char buffer_write[128] = {0};
	int size = 0;

	//PSK
	char psk_key[128] = {0};
	nvram_get_wds_passphrase(radio, vap_id, psk_key, 0);

	//actual_id
	int actual_id = 0;
	actual_id = convert_vap_id(radio, vap_id); 

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", actual_id);
		return T_FAILURE;
	}

	//ifacename
	sprintf(ifacename, "wds%d", actual_id);
	
	//File rm old
	sprintf(cmd, "rm -rf %s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);
	EXE_COMMAND(cmd);

	//File touch new
	sprintf(cmd, "touch %s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);
	EXE_COMMAND(cmd);

	//File open
	sprintf(cmd, "%s/%s.conf", WPA_SUPPLICANT_CFG_PATH, ifacename);
	if(!( fp = fopen(cmd, "wr")))
	{
		fprintf(stderr, "Open %s error\n", cmd);
		return T_FAILURE;
	}

	int i = 0;	
	sprintf(buffer[i++], "ctrl_interface=/var/run/wpa_supplicant_%s\n", ifacename);
	sprintf(buffer[i++], "ap_scan=2\n");
	sprintf(buffer[i++], "network={\n");
	sprintf(buffer[i++], "    ssid=\"%s\"\n", WDS_ESSID);
	sprintf(buffer[i++], "    mode=1\n");
	sprintf(buffer[i++], "    key_mgmt=WPA-NONE\n");
	sprintf(buffer[i++], "    proto=WPA\n");
	sprintf(buffer[i++], "    pairwise=NONE\n");
	sprintf(buffer[i++], "    group=CCMP\n");
	sprintf(buffer[i++], "    psk=%s%s%s\n", 
			strlen(psk_key) == 64 ? "":"\"",
			psk_key,
			strlen(psk_key) == 64 ? "":"\"");
	sprintf(buffer[i++], "}\n");

	//File write
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

	//File close
	fclose(fp);

	return T_SUCCESS;
}

/**
 * \brief start wds wpa_supplicant for specific radio & vap_id
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int start_wds_wpa_supplicant(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	//actual_id
	int actual_id = 0;
	actual_id = convert_vap_id(radio, vap_id); 

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", actual_id);
		return T_FAILURE;
	}

	//ifacename
	sprintf(ifacename, "wds%d", actual_id);

	//cmd
	sprintf(cmd, "/usr/sbin/wpa_supplicant -i %s -b %s -c %s/%s.conf -P %s/wpa_supplicant_%s.pid -B", 
		ifacename, "br-lan0", 
		WPA_SUPPLICANT_CFG_PATH, ifacename, 
		WPA_SUPPLICANT_PID_PATH, ifacename);

	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief wait 2 second and until check the wpa_state=COMPLETE by wpa_cli
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-07-02
 */
int wait_wds_wpa_supplicant(int radio, int vap_id)
{
	char cmd[768] = {0};
	char ifacename[16] = {0};

	//actual_id
	int actual_id = 0;
	actual_id = convert_vap_id(radio, vap_id);

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", actual_id);
		return T_FAILURE;
	}

	//ifacename
	sprintf(ifacename, "wds%d", actual_id);

	sprintf(cmd, //"echo \'check %s +++\'`uptime` >> /tmp/wds_wpa_supplicant.log;"//TODO
	"while(true);do "
		"sleep 2;"
		"wpa_cli -p /var/run/wpa_supplicant_%s status| grep wpa_state=COMPLETED;"
		"stop=$?;"
		//"wpa_cli -p /var/run/wpa_supplicant_%s status >> /tmp/wds_wpa_supplicant.log;"
		//"echo \'check %s ---\'`uptime` >> /tmp/wds_wpa_supplicant.log;"//TODO
		"if [ $stop -eq 0 ];then break;fi;"
		//"echo -e \"\\r\\n\" >> /tmp/wds_wpa_supplicant.log;"
	"done;"
	, ifacename, ifacename, ifacename, ifacename
	);

	EXE_COMMAND(cmd);
	return T_SUCCESS;
}

/**
 * \brief stop wds wpa_supplicant for specific radio & vap_id
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int stop_wds_wpa_supplicant(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	//actual_id
	int actual_id = 0;
	actual_id = convert_vap_id(radio, vap_id); 

	//ifacename
	sprintf(ifacename, "wds%d", actual_id);

	//cmd
	sprintf(cmd, "/usr/sbin/wpa_cli -p /var/run/wpa_supplicant_%s terminate", ifacename);
	EXE_COMMAND(cmd);

	return T_SUCCESS;	
}

/**
 * \brief Brief method for 'iwconfig wdsN essid XXXXX'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int set_wds_essid(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	sprintf(ifacename, "wds%d", convert_vap_id(radio, vap_id));
	sprintf(cmd, "iwconfig %s essid %s", ifacename, WDS_ESSID);
	EXE_COMMAND(cmd);
	
	return T_SUCCESS;
}

/**
 * \brief Brief method for 'iwpriv wdsN mode 11NXXHTXX'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int set_wds_mode(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

    char mode_str[32] = {0};
    int wireless_mode = 0;
    int band_width = 0;
    int channel = 0; 
    int ext_channel_mode = 0;


	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

    nvram_get_bandwidth(radio, &band_width);
    nvram_get_ap_channel(radio, &channel);
    nvram_get_ap_ext_channel_mode(radio, &ext_channel_mode);
    nvram_get_ap_wireless_mode(radio, &wireless_mode);

	switch(wireless_mode) {
        case WMODE_11A:
        case WMODE_11B:
        case WMODE_11G:
        break;
        case WMODE_11BGN:
            if(BANDWIDTH_20 == band_width) {
                strcpy(mode_str,"11NGHT20");
            }
            else if(BANDWIDTH_40 == band_width) {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel) {
                    fprintf(stderr, "Auto Chan is NOT supported by WDS.\r\n");
                    return T_FAILURE;
                }
                else if(EXT_CHANNEL_ABOVE == ext_channel_mode) {
                    strcpy(mode_str,"11NGHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode) {
                    strcpy(mode_str,"11NGHT40MINUS");
                }
            }
        break;   
        case WMODE_11AN:
            if(BANDWIDTH_20 == band_width) {
                strcpy(mode_str,"11NAHT20");
            }
            else if(BANDWIDTH_40 == band_width) {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel) {
                    fprintf(stderr, "Auto Chan is NOT supported by WDS.\r\n");
                    return T_FAILURE;
                }
                else if(EXT_CHANNEL_ABOVE == ext_channel_mode) {
                    strcpy(mode_str,"11NAHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode) {
                    strcpy(mode_str,"11NAHT40MINUS");
                }
            }
        break;
		case WMODE_11AC_MIXED:
            if(BANDWIDTH_20 == band_width) {
                strcpy(mode_str,"11ACVHT20");
            }
            else if(BANDWIDTH_40 == band_width) {
                //If channel selection is auto
                if(CHANNEL_AUTO == channel) {
                    fprintf(stderr, "Auto Chan is NOT supported by WDS.\r\n");
                    return T_FAILURE;
                }
				else if(EXT_CHANNEL_ABOVE == ext_channel_mode) {
                    strcpy(mode_str,"11ACVHT40PLUS");
                }
                else if(EXT_CHANNEL_BELOW == ext_channel_mode) {
                    strcpy(mode_str,"11ACVHT40MINUS");
                }
            }
            else if(BANDWIDTH_80 == band_width) {
                    strcpy(mode_str,"11ACVHT80");
            }
        break;
        default:
        fprintf(stderr, "mode %d is not supported!\n", wireless_mode);
        return T_FAILURE;
    }

    sprintf(cmd, "iwpriv %s mode %s", ifacename, mode_str);
	EXE_COMMAND(cmd);	

	return T_SUCCESS;
}

/**
 * \brief Brief method for 'iwconfig wdsN channel M'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int set_wds_channel(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

    int channel = 0; 

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	sprintf(ifacename, "wds%d", convert_vap_id(radio, vap_id));
    nvram_get_ap_channel(radio, &channel);
	if(CHANNEL_AUTO == channel) {
		fprintf(stderr, "Auto Chan is NOT supported by WDS.\r\n");
		return T_FAILURE;
	}

	sprintf(cmd, "iwconfig %s channel %d", ifacename, channel);
	EXE_COMMAND(cmd);

	return T_SUCCESS;
}

/**
 * \brief Brief method for 'iwpriv wdsN wds 1'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int set_wds_wds(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	sprintf(ifacename, "wds%d", convert_vap_id(radio, vap_id));
	sprintf(cmd, "iwpriv %s wds 1", ifacename);
	EXE_COMMAND(cmd);

	return T_SUCCESS;
}

/**
 * \brief Brief method for 'wlanconfig wdsN nawds mode 2'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \author frank
 * \date 2014-06-13
 */
int set_wds_nawds_mode(int radio, int vap_id)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	sprintf(ifacename, "wds%d", convert_vap_id(radio, vap_id));
	sprintf(cmd, "wlanconfig %s nawds mode 2", ifacename);
	EXE_COMMAND(cmd);

	return T_SUCCESS;
}

/**
 * \brief Brief method for 'ifconfig wdsN up/down'
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[in] vap_id 0~7
 * \param[in] updown RADIO_UP or RADIO_DOWN
 * \author frank
 * \date 2014-06-13
 */
int set_wds_ifconfig_updown(int radio, int vap_id, int updown)
{
	char cmd[256] = {0};
	char ifacename[16] = {0};

	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	sprintf(ifacename, "wds%d", convert_vap_id(radio, vap_id));
	sprintf(cmd, "ifconfig %s %s", ifacename, updown==RADIO_UP? "up":"down");
	EXE_COMMAND(cmd);

	return T_SUCCESS;
}

int set_ap_wds_down(int radio, int vap_id)
{
	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	//Stop Down Del 
	stop_wds_wpa_supplicant(radio, vap_id);

	//MAC-Clear
	/* Either clear remote MAC list or not will be ok, cause interface has
	 * been down to prevent WDS function; MAC surely will be cleared before 
	 * MAC-Add, by frank
	 */
	//clear_wds_list(radio, vap_id);	
	
	//Down Intf
	set_wds_ifconfig_updown(radio, vap_id, RADIO_DOWN);

	return T_SUCCESS;
}

int set_ap_wds_up(int radio, int vap_id)
{
	//prevent invalid wds interface
	if(vap_id < 0 || vap_id > WDS_VAP_NUM) {
		fprintf(stderr, "wds%d is not supported!\r\n", convert_vap_id(radio, vap_id));
		return T_FAILURE;
	}

	int wdsmode = 0;
	int secmode = 0;
	char mac[NVRAM_BUF_LEN+NVRAM_BUF_LEN] = {0};

	nvram_get_wds_mode(radio, &wdsmode);
	if(WDS_MODE_DISABLED == wdsmode) {
		return T_SUCCESS;
	}

	//iwconfig iwpriv 
	set_wds_essid(radio, vap_id);
	set_wds_mode(radio, vap_id);
	set_wds_channel(radio, vap_id);
	set_wds_nawds_mode(radio, vap_id);

	//MAC-Clear
	clear_wds_list(radio, vap_id);	
	
	//VAP Up
	nvram_get_wds_mac(radio, vap_id, mac);
    //printf("strlen(mac)=%d\n", strlen(mac));
    //printf("nvram_get_wds_mac(%d,%d,%s);\n", radio, vap_id, mac);
	if(strcmp(mac, "") ){
		set_wds_ifconfig_updown(radio, vap_id, RADIO_UP);
	}
	else {
        
		return T_SUCCESS;
	}

	//Start 
	nvram_get_wds_sec_mode(radio, vap_id, &secmode);
	if (WDS_SEC_MODE_NONE != secmode) {
		create_wds_conf(radio, vap_id);
		start_wds_wpa_supplicant(radio, vap_id);	

		/*IMPORTANT by frank:
		 * sleep a while to make sure wpa_supplicant run as daemon, 
		 * then the following MAC-Add process won't be reset 
		 * by wpa_supplicant
		 */
		wait_wds_wpa_supplicant(radio, vap_id);
	}

	//MAC-Add
	add_wds_list(radio, vap_id, mac);
    printf("add_wds_list(radio, vap_id, mac);\n", radio, vap_id, mac);

	return T_SUCCESS;
}

int set_ap_wds(int radio)
{
	int i = 0;
	for(i=0;i<WDS_VAP_NUM;i++) {
		set_ap_wds_down(radio, i);
	}

	for(i=0;i<WDS_VAP_NUM;i++) {
		set_ap_wds_up(radio, i);
	}
	return T_SUCCESS;
}

