#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "nvram.h"
#include "nvram_rule.h"

#include "mid_common.h"
//Get NVRAM System Mode
int nvram_get_system_mode(int *opmode)
{
    char buf[NVRAM_BUF_LEN] = { 0 };

    ezplib_get_attr_val("system_mode", 0, "name", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    if(!strcmp(buf, "ap"))
        {
            *opmode = OP_MODE_AP;
        }
    else if(!strcmp(buf, "normal"))
        {
            *opmode = OP_MODE_NORMAL;
        }
    else if(!strcmp(buf, "ur"))
        {
            *opmode = OP_MODE_UR;
        }
    else if(!strcmp(buf, "wisp0"))
        {
            *opmode = OP_MODE_WISP0;
        }
    else if(!strcmp(buf, "wisp1"))
        {
            *opmode = OP_MODE_WISP1;
        }
    else if(!strcmp(buf, "sta0"))
        {
            *opmode = OP_MODE_STA0;
        }
    else if(!strcmp(buf, "sta1"))
        {
            *opmode = OP_MODE_STA1;
        }
    else
        {        
            MID_ERROR("Operation mode got from NVRAM is illegal!!!");
            return T_FAILURE;
        }

    return T_SUCCESS;
}


//mid_common_nvram is only usfull for WP838 temporary
#if defined(GP_WP688)
int nvram_get_wlan_mode(int radio, int *wlan_mode)
{
	return T_SUCCESS;
}
#elif defined(GP_WP838)
/*>>>>>>>>>>NVRAM Get Functions Start>>>>>>>>>>*/
/***********************************************/

/*>>>>>>>>>>NVRAM Get Functions for Both AP and STA Start>>>>>>>>>>*/
//Get NVRAM WLAN Mode
int nvram_get_wlan_mode(int radio, int *wlan_mode)
{
    char buf[NVRAM_BUF_LEN] = { 0 };

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_mode_rule", 0, "mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }

    if(!strcmp(buf, "ap") || !strcmp(buf, "normal"))
        {
            *wlan_mode = WLAN_MODE_AP;
        }
    else if(!strcmp(buf, "client") || !strcmp(buf, "ur"))
        {
            *wlan_mode = WLAN_MODE_STA;
        }
    else
        {        
            MID_ERROR("Operation mode got from NVRAM is illegal!!!");
            return T_FAILURE;
        }

    return T_SUCCESS;
}

//Get NVRAM if the radio enable or not
int nvram_get_radio_status(int radio, int *radio_status)
{
    char buf[NVRAM_BUF_LEN] = { 0 };
    int wlan_mode = {0};

    nvram_get_wlan_mode(radio, &wlan_mode);
    MID_ASSERT((WLAN_MODE_AP == wlan_mode || WLAN_MODE_STA == wlan_mode), "WLAN mode got from NVRAM illegal");

    if(RADIO_2G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_ap_basic_rule", 0, "enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
                }
            else if(WLAN_MODE_STA == wlan_mode)
                {
                
                    ezplib_get_attr_val("wl_basic_rule", 0, "enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
                }
        }
    else if(RADIO_5G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
                }
            else if(WLAN_MODE_STA == wlan_mode)
                {
                    ezplib_get_attr_val("wl5g_basic_rule", 0, "enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
                }
        }


    if(!strcmp(buf, "1")) 
        {
            *radio_status = RADIO_UP;
        }
    else if(!strcmp(buf, "0"))
        {
            *radio_status = RADIO_DOWN;
        }

    else
        {        
            MID_ERROR("Radio status got from NVRAM illegal");
            return T_FAILURE;
        }

    return T_SUCCESS;
}


int nvram_get_vap_status(int radio, int vap_id, int *vap_status)
{
    char buf[NVRAM_BUF_LEN] = { 0 };
    /*Set VAP enable or disable*/
	

    if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl_ap_basic_rule", 0, 
			"enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 			
		if(!strcmp(buf, "0")) {
            *vap_status = VAP_DISABLE;
            return T_SUCCESS;
		}

		ezplib_get_attr_val("wl0_basic_rule", vap_id, 
			"enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);             
	}
    else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl5g_ap_basic_rule", 0, 
			"enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 			
		if(!strcmp(buf, "0")) {
            *vap_status = VAP_DISABLE;
            return T_SUCCESS;
		}

		ezplib_get_attr_val("wl1_basic_rule", vap_id, 
			"enable", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);             
    }
 
    if(!strcmp(buf, "1")) 
        {
            *vap_status = VAP_ENABLE;
        }
    else if(!strcmp(buf, "0"))
        {
            *vap_status = VAP_DISABLE;
        }

    else
        {        
            MID_ERROR("VAP status got from NVRAM illegal");
            return T_FAILURE;
        }


    return T_SUCCESS;
}

//Get NVRAM wireless mode
int nvram_get_ap_wireless_mode(int radio, int *wmode)
{
    //Get WLAN mode first
    int wlan_mode = {0};
    char buf[NVRAM_BUF_LEN] = { 0 };
    int net_mode;

    nvram_get_wlan_mode(radio, &wlan_mode);
    MID_ASSERT((WLAN_MODE_AP == wlan_mode || WLAN_MODE_STA == wlan_mode), "WLAN mode got from NVRAM illegal");

    if(RADIO_2G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
                }
            else if(WLAN_MODE_STA == wlan_mode)
                {
                    ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
                }
        }
    else if(RADIO_5G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "net_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
                }
            else if(WLAN_MODE_STA == wlan_mode)
                {
                    ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
                }
        }


    net_mode = atoi(buf);
    switch(net_mode)
    {
        case WMODE_11BG:
        case WMODE_11B:
        case WMODE_11A:
        case WMODE_11ABG:
        case WMODE_11G:
        case WMODE_11ABGN:
        case WMODE_11N:
        case WMODE_11GN:
        case WMODE_11AN:
        case WMODE_11BGN:
        case WMODE_11AGN:
        case WMODE_11N5G:
        case WMODE_11AC_MIXED:
            *wmode = net_mode;
            break;   

        default:
            MID_ERROR("Wireless mode illegal or not supported");
            return T_FAILURE;
    }

    return T_SUCCESS;
}

//Get NVRAM Bandwidth
int nvram_get_bandwidth(int radio, int *htbw)
{
    //Get WLAN mode first
    int wlan_mode = {0};
    char buf[NVRAM_BUF_LEN] = { 0 };

    nvram_get_wlan_mode(radio, &wlan_mode);
    MID_ASSERT((WLAN_MODE_AP == wlan_mode || WLAN_MODE_STA == wlan_mode), "WLAN mode got from NVRAM illegal");

    if(RADIO_2G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_ap_advanced_rule", 0, "htbw", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
                }
            else if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_advanced_rule",0,"htbw",buf,NVRAM_BUF_LEN,EZPLIB_USE_CLI);
                }
        
        }
    else if(RADIO_5G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl1_ap_advanced_rule",0,"htbw",buf,NVRAM_BUF_LEN,EZPLIB_USE_CLI);
                }
            else if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl1_advanced_rule",0,"htbw",buf,NVRAM_BUF_LEN,EZPLIB_USE_CLI);
                }
        }
    
    *htbw = atoi(buf); 
    MID_ASSERT((BANDWIDTH_20 == *htbw || BANDWIDTH_40 == *htbw || BANDWIDTH_80 == *htbw), "Bandwidth illegal");

    return T_SUCCESS;
}

//Get NVRAM TX Power
int nvram_get_txpower(int radio, int *txpower)
{
    //Get WLAN mode first
    int wlan_mode = {0};
    char buf[NVRAM_BUF_LEN] = { 0 };
    int nvram_txpower;

    nvram_get_wlan_mode(radio, &wlan_mode);
    MID_ASSERT((WLAN_MODE_AP == wlan_mode || WLAN_MODE_STA == wlan_mode), "WLAN mode got from NVRAM illegal");

    if(RADIO_2G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_ap_basic_rule", 0, "txpower", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);        
                }
            else if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl_basic_rule", 0, "txpower", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);    
                }
        }
    else if(RADIO_5G == radio)
        {
            if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl1_ap_basic_rule", 0, "txpower", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);            
                }
            else if(WLAN_MODE_AP == wlan_mode)
                {
                    ezplib_get_attr_val("wl1_basic_rule", 0, "txpower", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);        
                }
                    
        }
    nvram_txpower = atoi(buf);
    switch(nvram_txpower)
        {
            case 0:
                *txpower = 0;
                break;
            case 2:
                *txpower = 2;
                break;
            case 4:
                *txpower = 4;
                break;
            case 6:
                *txpower = 6;
                break;
            case 8:
                *txpower = 8;
                break;
            case 10:
                *txpower = 10;
                break;
            case 12:
                *txpower = 12;
                break;
            case 14:
                *txpower = 14;
                break;
            case 16:
                *txpower = 16;
                break;
            case 18:
                *txpower = 18;
                break;
            default:
                MID_ERROR("TxPower got from NVRAM is illegal");
                return T_FAILURE;
        }

    return T_SUCCESS;
}


/*<<<<<<<<<<NVRAM Get Functions for Both AP and STA End<<<<<<<<<<*/


/*>>>>>>>>>>NVRAM Get Functions for AP Start>>>>>>>>>>*/
//Get NVRAM VAP count
int nvram_get_vap_num(int radio)
{
    int vap_num; 
    vap_num = atoi(nvram_safe_get("wlv_rule_num"));
    
    return vap_num;
}

//Get NVRAM Channel index
int nvram_get_ap_channel(int radio, int *channel)
{
    //Get WLAN mode first

    char buf[NVRAM_BUF_LEN] = { 0 };
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "channel", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
        }
    *channel = atoi(buf);
    //MID_ASSERT(/*Make sure the channel number is within the llegal scale*/);
    return T_SUCCESS;
}

//Get NVRAM Extension Channel
int nvram_get_ap_ext_channel_mode(int radio, int *extcha)
{
    //Get WLAN mode first
    char buf[NVRAM_BUF_LEN] = { 0 };

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_ap_advanced_rule", 0, "extcha", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "extcha", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);        
        }
    *extcha = atoi(buf);
    MID_ASSERT((EXT_CHANNEL_ABOVE == *extcha || EXT_CHANNEL_BELOW == *extcha), "Extension channel got from NVRAM is illegal");
    return T_SUCCESS;
}

//Get NVRAM Channel select mode
int nvram_get_ap_channel_select_mode(int radio, int *sel_mode)
{
    int channel;
    
    nvram_get_ap_channel(radio, &channel);

    if(CHANNEL_AUTO == channel)
    {
        *sel_mode = CHANNEL_AUTO;
    }
    else
    {
        *sel_mode = CHANNEL_FIXED;    
    }
    printf("Channel select mode is %s\n", ((channel == 0) ? "AUTO":"Fixed"));
    return T_SUCCESS;
}

//Get NVRAM AP Isolation
int nvram_get_ap_isolation(int radio, int *ap_isolation)
{
    //Get WLAN mode first
    char buf[NVRAM_BUF_LEN] = { 0 };

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_ap_basic_rule",0,"bisolation",buf,NVRAM_BUF_LEN,EZPLIB_USE_CLI);
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl5g_ap_basic_rule",0,"bisolation",buf,NVRAM_BUF_LEN,EZPLIB_USE_CLI);
        }
    *ap_isolation = atoi(buf);

    MID_ASSERT((AP_ISOLATION_ENABLE == *ap_isolation || AP_ISOLATION_DISABLE == *ap_isolation), "AP isoaltion got from NVRAM is illegal");

    return T_SUCCESS;
}

/**
 * \brief Get NVRAM SSID for AP mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] SSID in nvram with special chars processed
 * \param[in] bProcSpecChar 0 for No Process Special Character, and 1 vice versa
 * \author frank
 * \date 2014-01-20
 */
int nvram_get_ap_ssid(int radio, int vap_id, char *ssid, int bProcSpecChar)
{
    char buf[NVRAM_SSID_LEN] = { 0 };

    /*Get the SSID from NVRAM*/
    if(RADIO_2G == radio) {
        ezplib_get_attr_val("wl0_ssid_rule", vap_id, "ssid", buf, NVRAM_SSID_LEN, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio) {
        ezplib_get_attr_val("wl1_ssid_rule", vap_id, "ssid", buf, NVRAM_SSID_LEN, EZPLIB_USE_CLI);
    }
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

    strcpy(ssid, buf);

    //Process special characters
	if(bProcSpecChar) {
		int i, j;
		for(i=0,j=0;j<strlen(buf);i++,j++) {
			if( '\"' == buf[j]
				|| '`' == buf[j]
				|| '\\' == buf[j]
				) {
				ssid[i] = '\\';
				i++;
			}
			ssid[i] = buf[j];
		}
		ssid[i] = '\0';
	}

    return T_SUCCESS;
}

//Get NVRAM Hidden
int nvram_get_ap_hidden(int radio, int *hidden, int vap_id)
{
    char buf[NVRAM_BUF_LEN] = { 0 };
    /*Get the hidden value from NVRAM first*/
    if(RADIO_2G == radio)
    {
        ezplib_get_attr_val("wl0_basic_rule", vap_id, "hidden", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio)
    {
        ezplib_get_attr_val("wl1_basic_rule", vap_id, "hidden", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
    *hidden = atoi(buf);
    MID_ASSERT((*hidden == HIDE_SSID_ENABLE || *hidden == HIDE_SSID_DISABLE), "AP hidden got from NVRAM is illegal");
    return T_SUCCESS;
}

//Get NVRAM BSS Isolation
int nvram_get_ap_bss_isolation(int radio, int vap_id, int *bss_isolation)
{
    char buf[NVRAM_BUF_LEN] = { 0 };
    /*Get bss isolation of current SSID from NVRAM*/
    if(RADIO_2G == radio)
    {
        ezplib_get_attr_val("wl0_basic_rule", vap_id, "isolation", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio)
    {
        ezplib_get_attr_val("wl1_basic_rule", vap_id, "isolation", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
	*bss_isolation = atoi(buf);
    MID_ASSERT((BSS_ISOLATION_ENABLE == *bss_isolation || BSS_ISOLATION_DISABLE == *bss_isolation), "BSS isolation got from NVRAM is illegal");

    return T_SUCCESS;
}

//Get NVRAM DTIM
int nvram_get_ap_dtim(int radio, int *dtim)
{
    //Get WLAN mode first
    char buf[NVRAM_BUF_LEN] = { 0 };

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_ap_advanced_rule", 0, "dtim", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "dtim", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI); 
        }

    *dtim = atoi(buf);
    //MID_ASSERT(/*Make sure dtim is within llegal scale*/);

    return T_SUCCESS;
}

//Get NVRAM BCN
int nvram_get_ap_beacon_interval(int radio, int *bcn)
{
    //Get WLAN mode first
    char buf[NVRAM_BUF_LEN] = { 0 };

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl_ap_advanced_rule", 0, "bcn", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);        
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "bcn", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);        
        }

    *bcn = atoi(buf);
    //MID_ASSERT(/*Make sure interval is in the llegal scale*/);
    return T_SUCCESS;
}
#if 0
/*Get NVRAM wpa cap*/
int nvram_get_ap_wpacap(int radio, int vap_id, int *wpacap)
{   
    char buf[NVRAM_BUF_LEN];
    char wpacap_s[NVRAM_BUF_LEN];
    int wpacap;
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "wpacap", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "wpacap", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    *wpacap = atoi(buf);
    MID_ASSERT(/*Make sure wpacap is within the llegal scale*/)
    return T_SUCCESS; 

}
#endif

/*disabled|wep|psk|psk2|psk2mixed|wpa|wpa2|wpa2mixed*/
/*Get NVRAM security mode*/
int nvram_get_ap_secmode(int radio, int vap_id, int *secmode)
{   
    char buf[NVRAM_BUF_LEN] = { 0 };
     /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }  
 
    if(!strcmp(buf, "disabled"))
        {
            *secmode = AUTHMODE_NONE;
        }
    else if(!strcmp(buf, "wep"))
        {
            *secmode = AUTHMODE_WEP;
        }
    else if(!strcmp(buf, "psk"))
        {
            *secmode = AUTHMODE_WPAPSK;
        }
    else if(!strcmp(buf, "psk2"))
        {
            *secmode = AUTHMODE_WPA2PSK;
        }
    else if(!strcmp(buf, "psk2mixed"))
        {
            *secmode = AUTHMODE_WPAPSKWPA2PSK;
        }
    else if(!strcmp(buf, "wpa"))
        {
            *secmode = AUTHMODE_WPA;
        }
    else if(!strcmp(buf, "wpa2"))
        {
            *secmode = AUTHMODE_WPA2;
        }
    else if(!strcmp(buf, "wpa2mixed"))
        {
            *secmode = AUTHMODE_WPAWPA2;
        }
    else
        {
            MID_ERROR("Illegal authentication mode got from NVRAM");
            return T_FAILURE;
        }

    return T_SUCCESS;
}
  
int nvram_get_security_encryptype(int radio, int vap_id, int *encryp_type)
{
    char buf[TMP_LEN];
    char crypto_s[TMP_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
    {
        ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "crypto", buf, TMP_LEN, EZPLIB_USE_CLI);       
    }
    else if(RADIO_5G == radio)
    {
    
        ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "crypto", buf, TMP_LEN, EZPLIB_USE_CLI);       
    }

    strcpy(crypto_s, buf);    


    if(!strcmp(crypto_s, "aes"))
    {
        *encryp_type = ENCRY_AES;
    }
    else if(!strcmp(crypto_s, "tkip"))
    {
        *encryp_type = ENCRY_TKIP;
    }
    else
    {
        *encryp_type = ENCRY_TKIPAES;
    }
    return T_SUCCESS;
}

 
/*Get NVRAM key in WPA2 mode*/
//int nvram_get_ap_passphrase(int radio, int vap_id, char *key, int key_len)
int nvram_get_ap_passphrase(int radio, int vap_id, char *key)
{   
    char buf[NVRAM_PSK_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(key, buf);
    return T_SUCCESS; 
}

/*Get NVRAM rekey mode*/
int nvram_get_ap_rekey_mode(int radio, int vap_id, int *rekey_mode)
{   
    char buf[NVRAM_BUF_LEN];
    char rekeymode_s[NVRAM_BUF_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_mode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(rekeymode_s, buf);    
   
    if(!strcmp(rekeymode_s, "time"))
        {
            *rekey_mode = REKEY_MODE_TIME;
        }
    else if(!strcmp(rekeymode_s, "pkt"))
        {
            *rekey_mode = REKEY_MODE_PKT;
        }
    else
        {
            *rekey_mode = REKEY_MODE_DISABLE;
        }
 
    return T_SUCCESS; 

}

/*Get NVRAM rekey time interval*/
int nvram_get_ap_rekey_time_interval(int radio, int vap_id, int *interval)
{   
    char buf[NVRAM_BUF_LEN];
    int rekey_mode;
    int ret;
    /*Get rekey method first*/
    ret = nvram_get_ap_rekey_mode(radio, vap_id, &rekey_mode);
    if(T_FAILURE == ret)
    {
        printf("ERRO:Get rekey method failure!\n");
        return T_FAILURE;
    }   

    MID_ASSERT((REKEY_MODE_TIME == rekey_mode), "Rekey mode is not time");
    /*Get the Radio according to rekey method*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_time_interval", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_time_interval", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    *interval = atoi(buf);        
    return T_SUCCESS; 
}

int nvram_get_ap_rekey_packet_interval(int radio, int vap_id, int *interval)
{   
    char buf[NVRAM_BUF_LEN];
    int rekey_mode;
    int ret;
    /*Get rekey method first*/
    ret = nvram_get_ap_rekey_mode(radio, vap_id, &rekey_mode);
    if(T_FAILURE == ret)
    {
        printf("ERRO:Get rekey method failure!\n");
        return T_FAILURE;
    }   

    MID_ASSERT((REKEY_MODE_PKT == rekey_mode), "Rekey mode is not pkt");
    /*Get the Radio according to rekey method*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_pkt_interval", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_pkt_interval", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    *interval = atoi(buf);        
    return T_SUCCESS; 
}
/*Get NVRAM radius ip addr*/
int nvram_get_ap_radius_ipaddr(int radio, int vap_id, char *server)
{   
    char buf[NVRAM_BUF_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_ipaddr", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_ipaddr", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(server, buf);    
    //MID_ASSERT(/*Make sure the format is IP address format*/);
    return T_SUCCESS; 
}

/*Get NVRAM radius port*/
int nvram_get_ap_radius_port(int radio, int vap_id, int *port)
{   
    char buf[NVRAM_BUF_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_port", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_port", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    *port = atoi(buf);
    return T_SUCCESS; 
}

/*Get NVRAM radius key*/
int nvram_get_ap_radius_key(int radio, int vap_id, char *key)
{   
    char buf[NVRAM_8021X_PWD_LEN];

    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_key", buf, NVRAM_8021X_PWD_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_key", buf, NVRAM_8021X_PWD_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(key, buf);    
    //Process special characters
    return T_SUCCESS; 
}

/*Get NVRAM radius ip addr*/
int nvram_get_ap_radius_own_ip(int radio, int vap_id, char *ownip)
{   
    char *own_ip = NULL;
    //How to Get own IP??? 
    own_ip = nvram_safe_get("lan0_ipaddr"); 

    strcpy(ownip, own_ip);    
    return T_SUCCESS; 
}

/*Get NVRAM pmk period*/
int nvram_get_ap_pmk_period(int radio, int vap_id, int *pmk_period)
{
    char buf[NVRAM_BUF_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "pmkperiod", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "pmkperiod", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    *pmk_period = atoi(buf);
    
    //if pmkperiod is within the llegal value scale

    return T_SUCCESS;
}

/*Get NVRAM preauth*/
int nvram_get_ap_preauth(int radio, int vap_id, int *preauth)
{   
    char buf[NVRAM_BUF_LEN] = {0};
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "preauth", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "preauth", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);       
        }

    *preauth = atoi(buf);
    return T_SUCCESS;
}

int nvram_get_ap_acl_policy(int radio, int vap_id, int *policy)
{
	assert(policy != NULL);
    char acl_rule[32] = {0};
    char enable[32] = {0};
    char buf1[32] = {0};
    char buf2[32] = {0};
    if(RADIO_2G == radio)
        {
            sprintf(acl_rule, "wl0%d_acl_basic_rule", vap_id);
        }
    else
        {
            sprintf(acl_rule, "wl1%d_acl_basic_rule", vap_id);
        }
    ezplib_get_attr_val(acl_rule, 0, "enable", buf1, 32, EZPLIB_USE_CLI);       
    strcpy(enable, buf1);    
    ezplib_get_attr_val(acl_rule, 0, "defpolicy", buf2, 32, EZPLIB_USE_CLI);       
    strcpy(policy, buf2); 
    if(!strcmp(enable, "disable") || !strcmp(enable, "0"))
        {
            *policy = ACL_POLICY_DISABLE;  
        }
    else if(!strcmp(policy, "allow") || !strcmp(policy, "1"))
        {
            *policy = ACL_POLICY_ALLOW;  
        }
    else if(!strcmp(policy, "deny") || !strcmp(policy, "2"))
        {
            *policy = ACL_POLICY_DENY;  
        }
    else
        {
            printf("ERROR:Get ACL Policy error!!!");
			return T_FAILURE;
        }
	return T_SUCCESS;
}

int nvram_get_ap_acl_list(int radio, int vap_id, ACL_LIST *acl_lst)
{
    char acl_num_rule[32];
    int acl_num;
    char acl_rule[32];
    char buf[32];
    char mac[32];
    int i;
    /*Get ACL NUM first*/
    if(RADIO_2G == radio)
	{
		sprintf(acl_num_rule, "wl0%d_acl_num", vap_id);
		sprintf(acl_rule, "wl0%d_acl_rule", vap_id);
	}
    else
	{
		sprintf(acl_num_rule, "wl1%d_acl_num", vap_id);
		sprintf(acl_rule, "wl1%d_acl_rule", vap_id);
	}
    acl_num = atoi(nvram_safe_get(acl_num_rule));
    printf("ACL num is %d",acl_num);
    /*acl_list->acl_num may not equal to acl_num of nvram*/
    acl_lst->acl_num = 0;
    for(i=0; i<acl_num; i++)
	{
		ezplib_get_attr_val(acl_rule, i, "flag", buf, 32, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "0") || !strcmp(buf, "1")) {
			ezplib_get_attr_val(acl_rule, i, "mac", buf, 32, EZPLIB_USE_CLI);       
			strcpy(mac, buf);    
			strcpy(acl_lst->mac[i].addr, mac);        
			memset(mac, 0, sizeof(mac));
			
			acl_lst->acl_num = acl_lst->acl_num + 1;
		}
	}
    return T_SUCCESS;
}

/*<<<<<<<<<<NVRAM Get Functions for AP End<<<<<<<<<<*/


/*>>>>>>>>>>NVRAM Get Functions for STA Start>>>>>>>>>>*/
/**
 * \brief Get NVRAM SSID for STA mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] SSID in nvram with special chars processed
 * \param[in] bProcSpecChar 0 for No Process Special Character, and 1 vice versa
 * \author frank
 * \date 2014-02-08
 */
int nvram_get_sta_ssid(int radio, char *ssid, int bProcSpecChar)
{   
    char buf[NVRAM_SSID_LEN] = { 0 };

    /*Get STA SSID From NVRAM*/
    if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, NVRAM_SSID_LEN, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_apcli_rule", 0, "ssid", buf, NVRAM_SSID_LEN, EZPLIB_USE_CLI);
    }
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}
	
	assert(NULL != ssid);
    strcpy(ssid, buf);
    //Process special characters
	if(bProcSpecChar) {
		int i, j;
		for(i=0,j=0;j<strlen(buf);i++,j++) {
			if( '\"' == buf[j]
				|| '`' == buf[j]
				|| '\\' == buf[j]
				) {
				ssid[i] = '\\';
				i++;
			}
			ssid[i] = buf[j];
		}
		ssid[i] = '\0';
	}

    return T_SUCCESS;
}

/*Get NVRAM STA BSSID*/
/**
 * \brief Get NVRAM BSSID for STA mode
 * \return T_SUCCESS on success & T_FAILURE on failure or if bssid is NULL
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] BSSID  XX:XX:XX:XX:XX:XX
 * \author frank
 * \date 2014-01-21
 */
int nvram_get_sta_bssid(int radio, char *bssid)
{
    char buf[NVRAM_BUF_LEN] = {0};

    /*Get STA SSID From NVRAM*/
    if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
    else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_apcli_rule", 0, "bssid", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
    }
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	if(0 == strlen(buf)) {
		return T_FAILURE;
	}
	
	assert(NULL != bssid);
    strcpy(bssid, buf);
    return T_SUCCESS;
}

/**
 * \brief Get NVRAM Security Mode for STA mode
 * \return Auth Mode
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-24
 */
int nvram_get_sta_secmode(int radio)
{
	char secmode_s[32] = {0};
	if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", secmode_s, 32, EZPLIB_USE_CLI); 
	}
	else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_apcli_rule", 0, "secmode", secmode_s, 32, EZPLIB_USE_CLI); 
	}
	else {
		return AUTHMODE_UNDEFINED;
	}

	if(!strcmp(secmode_s, "wep")) {
		return AUTHMODE_WEP;
	}
	else if(!strcmp(secmode_s, "wpa")) {
		return AUTHMODE_WPA;
	}
	else if(!strcmp(secmode_s, "wpa2")) {
		return AUTHMODE_WPA2;
	}
	else if(!strcmp(secmode_s, "psk")) {
		return AUTHMODE_WPAPSK;
	}
	else if(!strcmp(secmode_s, "psk2")) {
		return AUTHMODE_WPA2PSK;
	}
	else if(!strcmp(secmode_s, "disabled")) {
		return AUTHMODE_NONE;
	}
	return AUTHMODE_UNDEFINED;
}

/*Get NVRAM Address Mode*/
int nvram_get_sta_addrmode(int radio, int *addr_mode)
{
    char buf[NVRAM_BUF_LEN] = {0};
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }
    else
        {
            ezplib_get_attr_val("wl1_apcli_rule", 0, "addrmode", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }
    *addr_mode = atoi(buf);
    return T_SUCCESS;
}

/*Get NVRAM MAC Repeater*/
int nvram_get_sta_mac_repeater(int radio, int *mac_repeater)
{
    char buf[NVRAM_BUF_LEN];

    /*Get MAC Repeater*/
    if(RADIO_2G == radio)
        {
    ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }
    else
        {
    ezplib_get_attr_val("wl1_apcli_rule", 0, "macrepeater", buf, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
        }

    /*Get STA Channel From NVRAM*/
    *mac_repeater = atoi(buf);
    MID_ASSERT((MAC_REPEATER_ENABLE == *mac_repeater || MAC_REPEATER_DISABLE == *mac_repeater), "MAC repeater got from NVRAM illegal");
    return T_SUCCESS;
}

/**
 * \brief Get NVRAM STA crypto
 * \return encry_type ENCRY_AES or ENCRY_TKIP
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-24
 * \date 2014-02-27 WPA/WPA2 use wpa_crypto instead of crypto
 */
int nvram_get_sta_crypto(int radio) 
{   
	char crypto[NVRAM_BUF_LEN] = {0};
	int secmode = nvram_get_sta_secmode(radio);

	if (RADIO_2G == radio) {
		switch(secmode) {
			case AUTHMODE_WPA:
			case AUTHMODE_WPA2:
				ezplib_get_attr_val("wl0_wpa_auth_rule", 0, 
					"wpa_crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPAPSK:
				ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, 
					"crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPA2PSK:
				ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, 
					"crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			default:
				return ENCRY_NONE;
		}
	}
	else if (RADIO_5G == radio) {
		switch(secmode) {
			case AUTHMODE_WPA:
			case AUTHMODE_WPA2:
				ezplib_get_attr_val("wl1_wpa_auth_rule", 0, 
					"wpa_crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPAPSK:
				ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, 
					"crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPA2PSK:
				ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, 
					"crypto", crypto, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
				break;
			default:
				return ENCRY_NONE;
		}
	}
	else {
		return ENCRY_NONE;
	}	

	if(!strcmp(crypto, "aes")) {
		return ENCRY_AES;
	}
	else if(!strcmp(crypto, "tkip")) {
		return ENCRY_TKIP;
	}

	return ENCRY_NONE;
}

/**
 * \brief Get NVRAM STA KEY
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] key Pre-Shared Key (Personal Edition)
 * \author frank
 * \date 2014-01-24
 */
int nvram_get_sta_psk(int radio, char *key) 
{
	assert(NULL != key);

	char buf[NVRAM_PSK_LEN] = {0};

	int secmode = nvram_get_sta_secmode(radio);
	if (RADIO_2G == radio) {
		switch(secmode) {
			case AUTHMODE_WPAPSK:
				ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, 
					"key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPA2PSK:
				ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, 
					"key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);
				break;
			default:
				fprintf(stderr, "%d@%s"
					" psk is unnecessary for current secmode!\r\n"
					, __LINE__, __FUNCTION__);
				return T_FAILURE;
		}
	}
	else if (RADIO_5G == radio) {
		switch(secmode) {
			case AUTHMODE_WPAPSK:
				ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, 
					"key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);
				break;
			case AUTHMODE_WPA2PSK:
				ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, 
					"key", buf, NVRAM_PSK_LEN, EZPLIB_USE_CLI);
				break;
			default:
				fprintf(stderr, "%d@%s"
					" psk is unnecessary for current secmode!\r\n"
					, __LINE__, __FUNCTION__);
				return T_FAILURE;
		}
	}
	else {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}	

	strcpy(key, buf);
	return T_SUCCESS;
}

/**
 * \brief get selected WEP keyid 
 * \return the keyid [1~4]
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-20
 */
int nvram_get_sta_wep_default_keyid(int radio)
{
	char wep_tx_keyidx[8] = {0}; 
	int keyidx = 0;		//1~4

	/*Get key id from NVRAM */
	if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", wep_tx_keyidx, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key_index", wep_tx_keyidx, NVRAM_BUF_LEN, EZPLIB_USE_CLI);
	}
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		assert(radio == RADIO_2G || radio == RADIO_5G);
	}
	keyidx = atoi(wep_tx_keyidx);
	return keyidx;
}

/**
 * \brief get WEP related stuff for STA mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] key WEP key, special chars is processed
 * \param[out] key_idx 1~4
 * \param[out] key_type 0:HEX, 1:ASCII
 * \param[out] key_encmode "open" or "shared"
 * \param[out] wep_encry 0:64bits, 1:128bits
 * \author frank
 * \date 2014-01-20
 */
int nvram_get_sta_wep(int radio, char *key, int *key_idx, int *key_type, char *key_encmode, int*wep_encry)
{
	assert(key != NULL);
	assert(key_idx != NULL);
	assert(key_type != NULL);
	assert(key_encmode != NULL);
	char buf[27] = {0}; //max length is 2 * ASCII 13 + 1
	char wep_tx_keyidx[8] = {0}; 
	char wep_keytype[2] = {0}; 
	char wep_encmode[16] = {0};		// "open", "shared"
	char encry[2] = {0};

	int keyidx = 0;

	/*Get key from NVRAM*/
	if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", wep_tx_keyidx, 8, EZPLIB_USE_CLI); 
		keyidx = atoi(wep_tx_keyidx);
		sprintf(wep_tx_keyidx, "key%d", keyidx);
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, wep_tx_keyidx, buf, 27, EZPLIB_USE_CLI); 
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", wep_keytype, 2, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "encmode", wep_encmode, 16, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "wep_encry", encry, 2, EZPLIB_USE_CLI);
	}
	else if (RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key_index", wep_tx_keyidx, 8, EZPLIB_USE_CLI); 
		keyidx = atoi(wep_tx_keyidx);
		sprintf(wep_tx_keyidx, "key%d", keyidx);
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, wep_tx_keyidx, buf, 27, EZPLIB_USE_CLI); 
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "keytype", wep_keytype, 2, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "encmode", wep_encmode, 16, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "wep_encry", encry, 2, EZPLIB_USE_CLI);
	}
	else {
		fprintf(stderr, "__%d@%s error\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}

	strcpy(key_encmode, wep_encmode);
	*key_type = atoi(wep_keytype);
	*key_idx = keyidx;
	*wep_encry = atoi(encry);

	strcpy(key, buf);
#if 0
    int i, j;
	for(i=0,j=0;j<strlen(buf);i++,j++) {
		if( '\"' == buf[j] 
			|| '`' == buf[j]    
			|| '\\' == buf[j]) {
			key[i] = '\\';
			i++;
		}
		key[i] = buf[j];
	}
#endif

	return T_SUCCESS;
}

/**
 * \brief Get NVRAM authentication mode
 * \return EAP_MODE NONE|TTLS|PEAP
 * \param[in] radio RADIO_2G or RADIO_5G
 * \author frank
 * \date 2014-01-24
 */
int nvram_get_sta_eap_mode(int radio)
{   
	char buf[32] = {0};

	/*Get the Radio*/
	if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, 
			"wpa_auth", buf, 32, EZPLIB_USE_CLI); 
	}
	else if(RADIO_5G == radio)
	{
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, 
			"wpa_auth", buf, 32, EZPLIB_USE_CLI); 
	}
	else {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}	

	if (!strcmp(buf, "1")) {
		return EAP_MODE_PEAP;
	}
	else if (!strcmp(buf, "0")){
		return EAP_MODE_TTLS;
	}

	return EAP_MODE_NONE;
}

/**
 * \brief Get NVRAM 802.1X user name for STA Mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] radius_user user name for STA Mode
 * \author frank
 * \date 2014-01-24
 */
int nvram_get_sta_radius_user(int radio, char *radius_user)
{   
	assert(NULL != radius_user);

	char buf[NVRAM_8021X_NAME_LEN] = {0};

	if(RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, 
				"wpa_user", buf, NVRAM_8021X_NAME_LEN, EZPLIB_USE_CLI); 
	}
	else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, 
				"wpa_user", buf, NVRAM_8021X_NAME_LEN, EZPLIB_USE_CLI); 
	}
	else {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}	

	strcpy(radius_user, buf);
	return T_SUCCESS;
}

/**
 * \brief Get NVRAM 802.1X password for STA Mode
 * \return T_SUCCESS on success & T_FAILURE on failure
 * \param[in] radio RADIO_2G or RADIO_5G
 * \param[out] radius_pwd user name for STA Mode
 * \author frank
 * \date 2014-01-24
 */
int nvram_get_sta_radius_password(int radio, char *radius_pwd)
{   
	assert(NULL != radius_pwd);

	char buf[NVRAM_8021X_PWD_LEN] = {0};

	/*Get the Radio*/
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", buf, 64, EZPLIB_USE_CLI); 
	}
	else if(RADIO_5G == radio) {
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, "wpa_passwd", buf, 64, EZPLIB_USE_CLI); 
	}
	else {
		fprintf(stderr, "%d@%s error!\r\n", __LINE__, __FUNCTION__);
		return T_FAILURE;
	}	

	strcpy(radius_pwd, buf);

	return T_SUCCESS;
}
#endif //Endof GP_WP838