//Added by Mario Huang 2013-07-15
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>/* sleep */

#include	<sys/ioctl.h>
#include	<arpa/inet.h>
#ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
  #include	<linux/types.h>
  #include	<linux/socket.h>
  #include	<linux/if.h>
#endif
#include	<linux/wireless.h>
#include 	"nvram.h"
#include "nvram_rule.h"
#include "mid_common.h"
#include "mid_detail.h"
// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>
int set_channel(int radio)
{
    char cmd[128] = {0};
    char radio_name[8] = {0};
    char buf[TMP_LEN] = {0};
    char channel_s[TMP_LEN] = {0};
    int channel;
    int ret;
    char TempBuf_opmode[32] = {0};
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
            /*Do not set channel while mode is not AP*/
    		printf("Do not set channel while mode is not AP\n");
            return T_SUCCESS;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    	{
    		printf("ERROR:Construct main AP error!\n");
		return T_FAILURE;
    	}
    /*Get channel number from NVRAM*/
    if(RADIO_2G == radio)
    	{
            if(mode_ap)
                {
	            ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
	            ezplib_get_attr_val("wl_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
	            ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
	            ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    strcpy(channel_s, buf);	
    channel = atoi(channel_s);
    printf("Channel in set_channel is %d\n",channel);

    /*auto channel selection*/
    if(CHANNEL_AUTO == channel)
        {
             sprintf(cmd, "iwpriv %s set AutoChannelSel=2", radio_name);
	     printf("CMD in set_channel:%s\n", cmd);
             system(cmd);                   	
             //Added by Mario Huang, wait 6 seconds for autochannel select scan
             sleep(6);	
        }
    else
	{
             sprintf(cmd, "iwpriv %s set Channel=%d", radio_name, channel);
	     printf("%s\n", cmd);
             system(cmd);                   	
	}
	
    return T_SUCCESS;
}

/*
Set extension mode
0: UP
1: DOWN
*/
int set_extchannel(int radio)
{
    char cmd[128] = {0};
    char radio_name[8] = {0};
    int ret;
    char extcha_s[TMP_LEN] = {0};
    int channel;
    char channel_s[TMP_LEN] = {0};
    char buf_channel[TMP_LEN] = {0};
    char buf_extchannel[TMP_LEN] = {0};
    int extcha;
    char TempBuf_opmode[32] = {0};
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
            /*Do not set ext channel while mode is not AP*/
            printf("Do not set ext channel while mode is not AP\n");
            return T_SUCCESS;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    /*Do not set ext channel if channel selection is auto mode*/
    if(RADIO_2G == radio)
    	{
	    if(mode_ap)
                {
                    ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", buf_channel, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl_basic_rule", 0, "channel", buf_channel, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    else if(RADIO_5G == radio)
    	{
	    if(mode_ap)
                {
 	            ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "channel", buf_channel, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
 	            ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", buf_channel, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    strcpy(channel_s, buf_channel);	
    channel = atoi(channel_s);
    /*auto channel selection, return*/
    if(CHANNEL_AUTO == channel)
        {
            /*Channel selection is auto mode, do not need to set ext channel*/
            printf("Channel selection mode is auto, ext channel setting is not supporting!\n");
            return T_SUCCESS;
        }
	
    /*Get ext channel mode from NVRAM*/
    if(RADIO_2G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl_ap_advanced_rule", 0, "extcha", buf_extchannel, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl_advanced_rule", 0, "extcha", buf_extchannel, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "extcha", buf_extchannel, TMP_LEN, EZPLIB_USE_CLI);     	
                }
            else
                {
                    ezplib_get_attr_val("wl1_advanced_rule", 0, "extcha", buf_extchannel, TMP_LEN, EZPLIB_USE_CLI);     	
                }
    	}
    strcpy(extcha_s, buf_extchannel);	
    extcha = atoi(extcha_s);
    if(extcha != EXT_CHANNEL_ABOVE && extcha != EXT_CHANNEL_BELOW)
    	{
    		printf("ERROR:Wrong extention mode!\n");
		return T_FAILURE;
    	}
    /*Construct radio*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    	{
    		printf("ERROR:Construct_main_ap error!\n");
		return T_FAILURE;
    	}
	
    /*Set extension channel*/
    sprintf(cmd, "iwpriv %s set HtExtcha=%d", radio_name, extcha);
    printf("CMD in set_extchannel:%s\n", cmd);
    system(cmd);
	
    return T_SUCCESS;
}
/*
Set bandwidth
0: 20MHz
1: 40MHz
*/
int set_ht_bandwidth(int radio)
{
    char cmd[128] = {0};
    char radio_name[8] = {0};
    int ret;
    int htbw;
    char buf[TMP_LEN] = {0}; 
    char htbw_s[TMP_LEN] = {0};

    char TempBuf_opmode[32] = {0};
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

    if(RADIO_2G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl_ap_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                    ezplib_get_attr_val("wl1_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
    	}
    strcpy(htbw_s, buf);
    htbw = atoi(htbw_s); 
    if(mode_ap)
        {
            ret = construct_main_ap(radio_name, radio);
            if(T_FAILURE == ret)
            	{
    	        	printf("ERROR:Construct main AP failure!\n");
	            	return T_FAILURE;
            	}
        }
    else
        {
            ret = construct_vap(radio_name, radio, 0, WLAN_MODE_STA);
            if(T_FAILURE == ret)
            	{
    	        	printf("ERROR:Construct VAP failure!\n");
	            	return T_FAILURE;
            	}
 
        }


    sprintf(cmd, "iwpriv %s set HtBw=%d", radio_name, htbw);
    printf("CMD in set_ht_bandwidth:%s\n", cmd);
    system(cmd);

    memset(cmd, 0, sizeof(cmd));
    /*Set HtBssCoex*/
    if(htbw == BANDWIDTH_20)
        {
            sprintf(cmd, "iwpriv %s set HtBssCoex=%d", radio_name, BSS_COEX_DISABLE);
        }
    else if(htbw == BANDWIDTH_40)
        {
            sprintf(cmd, "iwpriv %s set HtBssCoex=%d", radio_name, BSS_COEX_ENABLE);
        }
    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}



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
int set_wirelessmode_channel(int radio)
{
    char cmd[128] = {0};
    char radio_name[8] = {0};
    int mode_id;
    char buf[TMP_LEN]; 
    //int htbw;
    char net_mode_s[TMP_LEN] = {0};
    int net_mode;
    int ret;
    char TempBuf_opmode[32];
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    if(mode_ap)
        {
            /*Construct AP*/
            ret = construct_main_ap(radio_name, radio);
            if(T_FAILURE == ret)
    	        {
    		        printf("ERROR:Construct main AP failure!\n");
		            return T_FAILURE;
            	} 
       }
    else
        {
            ret = construct_vap(radio_name, radio, 0, WLAN_MODE_STA);
            if(T_FAILURE == ret)
            	{
    	        	printf("ERROR:Construct VAP failure!\n");
	            	return T_FAILURE;
            	}
 
        }
    /*Get Wireless Mode From NVRAM*/
    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
	}
    else if(RADIO_5G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl_5g_asic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
    	}
    strcpy(net_mode_s, buf);
    net_mode = atoi(net_mode_s);
    switch(net_mode)
    	{
            /*Only BGN is supported now*/
            case WMODE_11BGN:
		  mode_id = WMODE_11BGN_ID;
                sprintf(cmd, "iwpriv %s set WirelessMode=%d", radio_name, mode_id);
                system(cmd);
                        printf("CMD in set_wirelessmode_channel:%s\n", cmd);
	     break;   
	     default:
		  printf("No such net mode or is not support currently!\n");
		  return T_FAILURE;
    	}
    /*Set net_mode, only BGN and AN need to set net_mode now*/
    if(WMODE_11BGN == net_mode|| WMODE_11AN == net_mode)
      {
	    ret = set_ht_bandwidth(radio);
	    if(T_FAILURE == ret)
	    	{
	    		printf("ERROR:set bandwidth error!\n");
			return T_FAILURE;
	    	}
      }

    /*Set Channel*/
    ret = set_channel(radio);
    if(T_FAILURE == ret)
	{
            printf("ERROR:set channel error!\n");
            return T_FAILURE;
    }

    /*Set Ext Channel*/
    if(WMODE_11BGN == net_mode|| WMODE_11AN== net_mode)
      {
	    ret = set_extchannel(radio);     
	    if(T_FAILURE == ret)
	    	{
	    		printf("ERROR:set ext channel error!\n");
			return T_FAILURE;
	    	}
      }

    return T_SUCCESS;
}


/*
Enable/Disable AP Isolation (Communication between wireless clients with diffrent SSIDs)
0: Disable
1: Enable
*/
int set_ap_isolation(int radio)
{
    char cmd[128];
    char buf[TMP_LEN]; 
    char radio_name[8];
    char bisolation_s[TMP_LEN];
    int bisolation;
    int ret;
    char TempBuf_opmode[32];
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    	{
    		printf("ERROR:Construct main AP failure!\n");
		return T_FAILURE;
    	}
    
    /*Get isolation value by reading NVRAM*/
    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl_ap_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
	}
    else if(RADIO_5G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl5g_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
    	}
	else
		{
    		fprintf(stderr, "ERROR:radio = %d!\n", radio);
			return T_FAILURE;
		}

    strcpy(bisolation_s, buf);  
    bisolation = atoi(bisolation_s);

    /*Set the isolation as enable/disable*/
    sprintf(cmd, "iwpriv %s set NoForwardingBTNBSSID=%d", radio_name, bisolation);
    printf("CMD in set_ap_isolation:%s\n", cmd);
    system(cmd);
	return T_SUCCESS;
}

/****************************************************************************************************************************/
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/*Set and Get WLAN Basic Settings for VAPs                                                                                                                                                         */
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/****************************************************************************************************************************/
/*Set SSID name (8 SSIDs)*/
int set_ssid(int radio, int mode, int vap_id)
{
    char cmd[128] = {0};
    char buf[33] = {0}; 
    char ssid[128] = {0};	//2 x 'max length of ssid is 32' for ' & "
    char vap_name[8] = {0};
    int ret;
    /*Get the SSID from NVRAM*/
    if(RADIO_2G == radio)
    	{
           ezplib_get_attr_val("wl0_ssid_rule", vap_id, "ssid", buf, 33, EZPLIB_USE_CLI);
    	}
    else if(RADIO_5G == radio)
    	{
           ezplib_get_attr_val("wl1_ssid_rule", vap_id, "ssid", buf, 33, EZPLIB_USE_CLI);
    	}
//    strcpy(ssid, buf);    	   	
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


    /*Construct VAP name by Radio, vap id, mode*/
    ret = construct_vap(vap_name, radio, vap_id, mode);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construc VAP failure!\n");
		return T_FAILURE;
    	}
	
    /*VAP set SSID*/
    sprintf(cmd, "iwpriv %s set SSID=\"%s\"", vap_name, ssid);
    printf("CMD in set_ssid:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

/*
Hidden SSID
0: disable
1: enable
*/
int set_hidden_ssid(int radio, int mode, int vap_id)
{
    char cmd[128];
    char buf[TMP_LEN];
    char hidden_s[TMP_LEN];
    int hidden;
    char vap_name[8];
    int ret;
    /*Get the hidden value from NVRAM first*/
    if(RADIO_2G == radio)
    	{
           ezplib_get_attr_val("wl0_basic_rule", vap_id, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI);
    	}
    else if(RADIO_5G == radio)
    	{
           ezplib_get_attr_val("wl1_basic_rule", vap_id, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI);
    	}
    strcpy(hidden_s, buf);   
    hidden = atoi(hidden_s);

    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, mode);   
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construc VAP failure!\n");
		return T_FAILURE;
    	}

    /*Set VAP hidden*/
    sprintf(cmd, "iwpriv %s set HideSSID=%d", vap_name, hidden);
    printf("CMD in set_hidden_ssid:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}
/*Enable/Disable BSS Isolation*/
int set_bss_isolation(int radio, int mode, int vap_id)
{
    char cmd[128];
    char buf[TMP_LEN];
    char isolation_s[TMP_LEN];
    int isolation;
    char vap_name[8];
    int ret;
    /*Get bss isolation of current SSID from NVRAM*/
    if(RADIO_2G == radio)
    	{
           ezplib_get_attr_val("wl0_basic_rule", vap_id, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI);
    	}
    else if(RADIO_5G == radio)
    	{
           ezplib_get_attr_val("wl1_basic_rule", vap_id, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI);
    	}
    strcpy(isolation_s, buf);   
    isolation = atoi(isolation_s);

    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, mode);    
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construc VAP failure!\n");
		return T_FAILURE;
    	}

    /*Set VAP isolation enable/disable*/
    sprintf(cmd, "iwpriv %s set NoForwarding=%d", vap_name, isolation);
    printf("CMD in set_bss_isolation:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

/*Enable SSID(except default SSID)*/
int set_enable_ssid(int radio, int mode, int vap_id)
{
    char vap_name[8] = {0};
    int ret;
    //mark by frank XXX
    /*Set SSID name*/
    //ret = set_ssid(radio, mode, vap_id);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Set SSID failure!\n");
		return T_FAILURE;
    	}
    /*Set hideen*/
    ret = set_hidden_ssid(radio, mode, vap_id);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Set hidden failure!\n");
		return T_FAILURE;
    	}
    /*Set BSS isolation*/
    ret = set_bss_isolation(radio, mode, vap_id);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Set bss isolation failure!\n");
		return T_FAILURE;
    	}
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, mode);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}

    /*Set SSID name*/
    ret = set_ssid(radio, mode, vap_id);
    return ret;
}

/*Enable SSID(except default SSID)*/
int set_disable_ssid(int radio, int mode, int vap_id)
{
    char vap_name[8] = {0};
    int ret;
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, mode);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}

    /*Set VAP Enable*/
    return vap_up_down(radio, vap_id, mode, VAP_DOWN);
}

/****************************************************************************************************************************/
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/*Set and Get WLAN Advanced Settings                                                                                                                                                                */
/*                                                                                                                                                                                                                     */
/*                                                                                                                                                                                                                     */
/****************************************************************************************************************************/
/* Set DTIM (1~5) */
int set_dtim(int radio)
{
    char cmd[128];
    char radio_name[8];
    char buf[TMP_LEN];
    char dtim_s[TMP_LEN];
    int dtim;
    int ret;
    char TempBuf_opmode[32];
    int mode_ap;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl_ap_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
           else
               {
                   ezplib_get_attr_val("wl_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
               }
	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
                   ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
            else
                {
                   ezplib_get_attr_val("wl1_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
                }
	}

    strcpy(dtim_s, buf);  	
    dtim = atoi(dtim_s);
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct main AP failure!\n");
		return T_FAILURE;
    	}

    /*Set DTIM*/
    sprintf(cmd, "iwpriv %s set DtimPeriod=%d", radio_name, dtim);
    printf("CMD in set_dtim:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}
/* Set Output Power */
int set_power(int radio)
{
    char buf[TMP_LEN];
    char cmd[128];
    char radio_name[8];
    char txpower_s[TMP_LEN];
    int txpower;
    int txpower_get;
    char TempBuf_opmode[32];
    int mode_ap;
    int ret;
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");
    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
           		ezplib_get_attr_val("wl_ap_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);     	
               }
           else
               {
           		ezplib_get_attr_val("wl_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);     	
               }
	}
    else if(RADIO_5G == radio)
    	{
           if(mode_ap)
               {
           		ezplib_get_attr_val("wl5g_ap_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);     	    	
               }
           else
               {
           		ezplib_get_attr_val("wl5g_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);     	    	
               }
	}
    strcpy(txpower_s, buf);   
    txpower_get = atoi(txpower_s);
    switch(txpower_get)
        {
            case 0:
                txpower = 0;
                break;
            case 2:
                txpower = 2;
                break;
            case 4:
                txpower = 4;
                break;
            case 6:
                txpower = 6;
                break;
            case 8:
                txpower = 8;
                break;
            case 10:
                txpower = 10;
                break;
            case 12:
                txpower = 12;
                break;
            case 14:
                txpower = 14;
                break;
            case 16:
                txpower = 16;
                break;
            case 18:
                txpower = 18;
                break;
            default:
                printf("txpower is illegal value, check it please!!!\n");
                txpower = 0;
#if 0
            case 0:
                txpower = 100;
                break;
            case 2:
                txpower = 90;
                break;
            case 4:
                txpower = 80;
                break;
            case 6:
                txpower = 70;
                break;
            case 8:
                txpower = 60;
                break;
            case 10:
                txpower = 50;
                break;
            case 12:
                txpower = 40;
                break;
            case 14:
                txpower = 30;
                break;
            case 16:
                txpower = 20;
                break;
            case 18:
                txpower = 10;
                break;
            default:
                printf("txpower is illegal value, check it please!!!\n");
                txpower = 100;
#endif
        }

    if(mode_ap)
        {
            ret = construct_main_ap(radio_name, radio);
            if(T_FAILURE == ret)
            	{
    	        	printf("ERROR:Construct main AP failure!\n");
	            	return T_FAILURE;
            	}
        }
    else
        {
            ret = construct_vap(radio_name, radio, 0, WLAN_MODE_STA);
            if(T_FAILURE == ret)
            	{
    	        	printf("ERROR:Construct VAP failure!\n");
	            	return T_FAILURE;
            	}
 
        }

    /*Set Power*/
    sprintf(cmd, "iwpriv %s set CutTxPower=%d", radio_name, txpower);
    printf("CMD in set_power:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

/*Set Beacon Interval (20~1024)*/
int set_beacon_interval(int radio)
{
    char cmd[128] = {0};
    char buf[TMP_LEN];
    char beacon_interval_s[TMP_LEN];
    int beacon_interval;
    char radio_name[8];
    int ret;
    char TempBuf_opmode[32];
    int mode_ap;
	int i = 0;
	char mac_cmd[128] = {0};
	StationListInfo_t sli;
	memset(&sli, 0x00, sizeof(StationListInfo_t));
	
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            mode_ap = 1;
        }
    else
        {
            mode_ap = 0;
        }
    printf("opmode is %s\n", mode_ap ? "ap":"Not ap");

    if(RADIO_2G == radio)
    	{
           if(mode_ap)
               {
                   ezplib_get_attr_val("wl_ap_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI);     	
               }
           else
               {
                   ezplib_get_attr_val("wl_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI);     	
               }
	}
    else if(RADIO_5G == radio)
    	{
            if(mode_ap)
                {
                    ezplib_get_attr_val("wl1_ap_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI);     	    	
                }
            else
                {
                    ezplib_get_attr_val("wl1_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI);     	    	
                }
	}

    strcpy(beacon_interval_s, buf);   
    beacon_interval = atoi(beacon_interval_s);
    /*Construct AP*/
    ret = construct_main_ap(radio_name, radio);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct main AP failure!\n");
		return T_FAILURE;
    	}

    /*bintval have to be in 20~1024 scope*/
    if(beacon_interval < 20 || beacon_interval > 1024)
    	{
    		printf("ERRO:Beacon interval is out of region\n");
		return T_FAILURE;
    	}
	
	/* Added KickMAC CMD by Andy in 20140221: Kick All Clients Before Setting Beacon Interval*/
	get_sta_list(radio, &sli);
	for(i=0; i<sli.sta_num; i++)
	{
		sprintf(mac_cmd, "iwpriv %s set KickMAC=%02X:%02X:%02X:%02X:%02X:%02X", radio_name,
			sli.sta_list[i].Addr[0], sli.sta_list[i].Addr[1], sli.sta_list[i].Addr[2],
			sli.sta_list[i].Addr[3], sli.sta_list[i].Addr[4], sli.sta_list[i].Addr[5]);
		system(mac_cmd);
		printf("CMD in set_beacon_interval:%s\n",mac_cmd);
	}
	
    sprintf(cmd, "iwpriv %s set BeaconPeriod=%d", radio_name, beacon_interval);
    printf("CMD in set_beacon_interval:%s\n", cmd);
    system(cmd);

	return T_SUCCESS;
}

/**************************************************************/
/*Security                                                    */
/**************************************************************/
int get_nvram_wpacap(int radio, int vap_id)
{   
    char buf[TMP_LEN];
    char wpacap_s[TMP_LEN];
    int wpacap;
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(wpacap_s, buf);  	
    wpacap = atoi(wpacap_s);
    printf("wpacap in get_nvram_wpacap is %d\n", wpacap);
    return wpacap; 
}

int set_security_authmode_wpa2psk(int radio, int vap_id)
{
    char cmd[128] = {0};
    int ret;
    char authmode[32];
    char vap_name[8];

    char buf[TMP_LEN];
    char security_mode_s[TMP_LEN];
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
            printf("ERRO:Construct vap failure!\n");
            return T_FAILURE;
    	}
     /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }  

    strcpy(security_mode_s, buf);  	
    if(!strcmp(security_mode_s, "psk2"))
        {
            sprintf(authmode, "WPA2PSK");
        }
    else if(!strcmp(security_mode_s, "psk2mixed"))
        {
            sprintf(authmode, "WPAPSKWPA2PSK");
        }
    /*Set security wpa2*/
    sprintf(cmd, "iwpriv %s set AuthMode=%s", vap_name, authmode);
    printf("CMD in set_security_wpa2:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_security_authmode_8021x(int radio, int vap_id)
{
    char cmd[128] = {0};
    int ret;
    char authmode[32];
    char vap_name[8];

    char buf[TMP_LEN];
    char security_mode_s[TMP_LEN];
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}

     /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }  

    strcpy(security_mode_s, buf);  	
    if(!strcmp(security_mode_s, "wpa2"))
        {
            sprintf(authmode, "WPA2");
        }
    else if(!strcmp(security_mode_s, "wpa2mixed"))
        {
            sprintf(authmode, "WPA1WPA2");
        }
    /*Set security wpa2*/
    sprintf(cmd, "iwpriv %s set AuthMode=%s", vap_name, authmode);
    printf("CMD in set_security_authmode_8021x:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_security_authmode_open(int radio, int vap_id)
{
    char cmd[128];
    int ret;
    char vap_name[8];
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}

    /*Set security wpa2*/
    sprintf(cmd, "iwpriv %s set AuthMode=OPEN", vap_name);
    printf("CMD in set_security_authmode_open:%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_security_encryptype_psk(int radio, int vap_id)
{
    char cmd[128];
    char buf[TMP_LEN];
    char buf_secmode[TMP_LEN];
    char crypto_s[TMP_LEN];
    int ret;
    char encrypset[TMP_LEN];
    char vap_name[8];
    int psk2mixed = 0;
    char security_mode_s[TMP_LEN];
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
    /*Deleted by Mario Huang*/
    //iwpacap = get_nvram_wpacap(radio, vap_id);

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf_secmode, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf_secmode, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(security_mode_s, buf_secmode);  
    printf("security_mode is %s\n", security_mode_s);	
    if(!strcmp(security_mode_s, "psk2mixed"))
        {
            printf("Is psk2mixed\n");	
            psk2mixed = 1;
        }
    else
        {
            printf("Not psk2mixed\n");	
            psk2mixed = 0;
        }
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
   	{
            printf("ERRO:Construct vap failure!\n");
            return T_FAILURE;
    	}

    if(!strcmp(crypto_s, "aes"))
        {
            sprintf(encrypset, "AES");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            printf("CMD in set_security_encryptype_psk:%s\n", cmd);
            system(cmd);
        }
    else if(!strcmp(crypto_s, "tkip"))
        {
            sprintf(encrypset, "TKIP");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            printf("CMD in set_security_encryptype_psk:%s\n", cmd);
            system(cmd);

	    memset(cmd, 0, sizeof(cmd));
            sprintf(cmd, "iwpriv %s set WpaMixPairCipher=WPA_TKIPAES_WPA2_TKIP", vap_name);
            system(cmd);
            printf("%s\n", cmd);
        }
    else
        {
            sprintf(encrypset, "TKIPAES");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            printf("CMD in set_security_encryptype_psk:%s\n", cmd);
            system(cmd);
	    memset(cmd, 0, sizeof(cmd));
            if(psk2mixed)
                {
                    sprintf(cmd, "iwpriv %s set WpaMixPairCipher=WPA_TKIPAES_WPA2_TKIPAES", vap_name);
                    system(cmd);
                    printf("%s\n", cmd);
                }
        }
    return T_SUCCESS;
}

int set_security_encryptype_8021x(int radio, int vap_id)
{
    char cmd[128];
    char buf[TMP_LEN];
    char buf_secmode[TMP_LEN];
    char crypto_s[TMP_LEN];
    int ret;
    char encrypset[TMP_LEN];
    char vap_name[8];
    int wpa2mixed = 0;
    char security_mode_s[TMP_LEN];
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

    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf_secmode, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf_secmode, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(security_mode_s, buf_secmode);
    printf("security_mode_s is %s\n", security_mode_s);  	
    if(!strcmp(security_mode_s, "wpa2mixed"))
        {
            printf("Is wpa2mixed\n");  	
            wpa2mixed = 1;
        }
    else
        {
            printf("Not wpa2mixed\n");  	
            wpa2mixed = 0;
        }

    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
   	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    if(!strcmp(crypto_s, "aes"))
        {
            sprintf(encrypset, "AES");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            system(cmd);
            printf("CMD in set_security_encryptype_8021x:%s\n", cmd);
        }
    else if(!strcmp(crypto_s, "tkip"))
        {
            sprintf(encrypset, "TKIP");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            system(cmd);
            printf("CMD in set_security_encryptype_8021x:%s\n", cmd);

	    memset(cmd, 0, sizeof(cmd));
            sprintf(cmd, "iwpriv %s set WpaMixPairCipher=WPA_TKIPAES_WPA2_TKIP", vap_name);
            system(cmd);
            printf("%s\n", cmd);
        }
    else
        {
            sprintf(encrypset, "TKIPAES");
            sprintf(cmd, "iwpriv %s set EncrypType=%s", vap_name, encrypset);
            system(cmd);
            printf("CMD in set_security_encryptype_8021x:%s\n", cmd);
	    memset(cmd, 0, sizeof(cmd));
            if(wpa2mixed)
                {
                    sprintf(cmd, "iwpriv %s set WpaMixPairCipher=WPA_TKIPAES_WPA2_TKIPAES", vap_name);
                    system(cmd);
                    printf("%s\n", cmd);
                }
            
        }
    return T_SUCCESS;
}

int set_security_encryptype_none(int radio, int vap_id)
{
    char cmd[128];
    int ret;
    char vap_name[8];

    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
   	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    sprintf(cmd, "iwpriv %s set EncrypType=NONE", vap_name);
    system(cmd);
    printf("CMD in set_security_encryptype_none:%s\n", cmd);
        
    return T_SUCCESS;
}

int set_security_8021x_enable(int radio, int vap_id, int enable)
{
    char vap_name[8];
    char cmd[128];
    int ret;
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    /*Disable 802.1X*/
    if(IEEE8021X_ENABLE == enable)
        {
            sprintf(cmd, "iwpriv %s set IEEE8021X=1", vap_name);
        }
    else
        {
            sprintf(cmd, "iwpriv %s set IEEE8021X=0", vap_name);
        }
    system(cmd);
    printf("CMD in set_security_8021x_enable:%s\n", cmd);
    return T_SUCCESS;
}

int get_nvram_wpapsk(int radio, int vap_id, char *wpapsk)
{   
    char buf[TMP_LEN] = {0};
    char key_s[TMP_LEN] = {0};
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "key", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "key", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(key_s, buf);  	
    strcpy(wpapsk, key_s);  	
    return T_SUCCESS; 
}

int set_security_wpapsk(int radio, int vap_id)
{
    char cmd[512] = {0};
    char wpapsk[256] = {0};
    char wpapsk_tr[256] = {0};
    int ret;
    char vap_name[8] = {0};
    ret = get_nvram_wpapsk(radio, vap_id, wpapsk);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Get wpapsk failure!\n");
		return T_FAILURE;
    	}

	int i, j;
	for(i=0,j=0;j<strlen(wpapsk);i++,j++) {
		if( '\"' == wpapsk[j] 
			|| '`' == wpapsk[j]
			|| '\\' == wpapsk[j]
			) {
			wpapsk_tr[i] = '\\';
			i++;
		}
		wpapsk_tr[i] = wpapsk[j];
	}



    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    /*Set Key*/
    sprintf(cmd, "iwpriv %s set WPAPSK=\"%s\"", vap_name, wpapsk_tr);
    printf("CMD in set_security_wpapsk:%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    /*Set Default Key ID, default key id in WPA/WPA2 mode is 2*/
    sprintf(cmd, "iwpriv %s set DefaultKeyID=2", vap_name);
    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int get_nvram_rekey_method(int radio, int vap_id, char *method)
{   
    char buf[TMP_LEN];
    char rekeymode_s[TMP_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_mode", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_mode", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(rekeymode_s, buf);  	
    strcpy(method, rekeymode_s);  	
    return T_SUCCESS; 
}

int get_nvram_rekey_interval(int radio, int vap_id, int *interval)
{   
    char buf[TMP_LEN];
    char rekeyinterval_s[TMP_LEN];
    char method[64];
    int ret;
    /*Get rekey method first*/
    ret = get_nvram_rekey_method(radio, vap_id, method);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Get rekey method failure!\n");
		return T_FAILURE;
    	}   

    /*Get the Radio according to rekey method*/
    if(!strcmp(method, "time"))
        {
            if(RADIO_2G == radio)
                {
                    ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI);       
         	}
            else if(RADIO_5G == radio)
                {
                    ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI);       
                }
        }
    else if(!strcmp(method, "pkt"))
        {
            if(RADIO_2G == radio)
                {
                    ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "rekey_pkt_interval", buf, TMP_LEN, EZPLIB_USE_CLI);       
         	}
            else if(RADIO_5G == radio)
                {
                    ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "rekey_pkt_interval", buf, TMP_LEN, EZPLIB_USE_CLI);       
                }
        }
    else
        {
            return T_SUCCESS;
        }
    strcpy(rekeyinterval_s, buf);  	
    *interval = atoi(rekeyinterval_s);  	
    return T_SUCCESS; 
}

int set_security_rekey(int radio, int vap_id)
{
    int ret;
    char method[64];
    char method_set[64];
    int interval;
    char vap_name[8];
    char cmd[128];

    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}

    /*Get rekey method*/
    ret = get_nvram_rekey_method(radio, vap_id, method);
    if(T_FAILURE == ret)
    	{
            printf("ERRO:Get rekey method failure!\n");
            return T_FAILURE;
    	}
    if(!strcmp(method, "time"))
        {
            sprintf(method_set, "TIME");
        }
    else if(!strcmp(method, "pkt"))
        {
            sprintf(method_set, "PKT");
        }
    else
        {
            sprintf(method_set, "DISABLE");
        }

    /*Set Rekey Method*/
    sprintf(cmd, "iwpriv %s set RekeyMethod=%s", vap_name, method_set);
    printf("CMD in set_security_rekey:%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));

    /*Get rekey interval*/
    ret = get_nvram_rekey_interval(radio, vap_id, &interval);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Get rekey interval failure!\n");
		return T_FAILURE;
    	}
    /*Set Rekey Interval*/
    if(!strcmp(method, "time") || !strcmp(method, "pkt"))
        {
            sprintf(cmd, "iwpriv %s set RekeyInterval=%d", vap_name, interval);
            printf("%s\n", cmd);
            system(cmd);
        }
    return T_SUCCESS;
}

/*Get NVRAM Radius Server*/
int get_nvram_radius_server(int radio, int vap_id, char *server)
{   
    char buf[TMP_LEN];
    char radius_server[TMP_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(radius_server, buf);  	
    strcpy(server, radius_server);  	
    return T_SUCCESS; 
}

/*Get NVRAM Radius Port*/
int get_nvram_radius_port(int radio, int vap_id, int *port)
{   
    char buf[TMP_LEN];
    char radius_port[TMP_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(radius_port, buf);  	
    *port = atoi(radius_port);
    return T_SUCCESS; 
}

/*Get NVRAM Radius Key*/
int get_nvram_radius_key(int radio, int vap_id, char *key)
{   
    char buf[TMP_LEN];
    char radius_key[TMP_LEN];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(radius_key, buf);  	
    strcpy(key, radius_key);  	
    return T_SUCCESS; 
}

/*Get NVRAM Radius Own IP*/
int get_nvram_own_ip(int radio, int vap_id, char *ownip)
{   
    char *own_ip = NULL;
    own_ip = nvram_safe_get("lan0_ipaddr"); 

    strcpy(ownip, own_ip);  	
    return T_SUCCESS; 
}

int set_security_radius(int radio, int vap_id)
{
    char vap_name[8];
    char server[TMP_LEN];
    int port;
    char key[TMP_LEN] = {0};
    char key_tr[TMP_LEN] = {0};
    char ownip[TMP_LEN];
    char cmd[128];
    int ret;
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    
    /*Get Radius Server from NVRAM*/
    get_nvram_radius_server(radio, vap_id, server);
    /*Get Radius Port from NVRAM*/
    get_nvram_radius_port(radio, vap_id, &port);
    /*Get Radius Key from NVRAM*/
    get_nvram_radius_key(radio, vap_id, key);
    /*Get Own IP Addr from NVRAM*/
    get_nvram_own_ip(radio, vap_id, ownip);


	int i, j;
	for(i=0,j=0;j<strlen(key);i++,j++) {
		if( '\"' == key[j] 
			|| '`' == key[j]
			|| '\\' == key[j]
			) {
			key_tr[i] = '\\';
			i++;
		}
		key_tr[i] = key[j];
	}



     /*Set Radius Server*/
    sprintf(cmd, "iwpriv %s set RADIUS_Server=\'%s\'", vap_name, server);
    printf("CMD in set_security_radius:%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set RADIUS_Port=\'%d\'", vap_name, port);
    printf("%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set RADIUS_Key=\"%s\"", vap_name, key_tr);
    printf("%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set own_ip_addr=\'%s\'", vap_name, ownip);
    printf("%s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set DefaultKeyID=2", vap_name);
    printf("%s\n", cmd);
    system(cmd);
    /*Set eap bridge*/ 
    //memset(cmd, 0, sizeof(cmd));
    //sprintf(cmd, "iwpriv %s set EAPifname=br-lan0", vap_name);
    //printf("%s\n", cmd);
    //system(cmd);   
    /*Set preauth bridge*/ 
    //memset(cmd, 0, sizeof(cmd));
    //sprintf(cmd, "iwpriv %s set PreAuthifname=br-lan0", vap_name);
    //printf("%s\n", cmd);
    //system(cmd);
    return T_SUCCESS;
}

int set_security_bridge(int radio, int vap_id)
{
    char vap_name[8];
    char cmd[128];
    int ret;
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    
    /*Set eap bridge*/ 
    //memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set EAPifname=br-lan0", vap_name);
    printf("%s\n", cmd);
    system(cmd);   
    /*Set preauth bridge*/ 
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "iwpriv %s set PreAuthifname=br-lan0", vap_name);
    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_security_pmk_cacheperiod(int radio, int vap_id)
{
    char buf[TMP_LEN];
    char pmk_period[TMP_LEN];
    int pmkperiod;
    char vap_name[8];
    char cmd[128];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "pmkperiod", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "pmkperiod", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(pmk_period, buf);  	
    pmkperiod = atoi(pmk_period);
    sprintf(cmd, "iwpriv %s set PMKCachePeriod=%d", vap_name, pmkperiod);
    printf("CMD in set_security_pmk_cacheperiod is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_security_preauth(int radio, int vap_id)
{
    char buf[TMP_LEN];
    char pre_auth[TMP_LEN];
    int preauth;
    char vap_name[8];
    char cmd[128];
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_wpa2_rule", vap_id, "preauth", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_wpa2_rule", vap_id, "preauth", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(pre_auth, buf);  	
    preauth = atoi(pre_auth);
    sprintf(cmd, "iwpriv %s set PreAuth=%d", vap_name, preauth);
    printf("CMD in set_security_preauth is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_start_8021x_daemon(int radio)
{
    char cmd[128];
    /*Kill the old daemon first*/
    sprintf(cmd, "killall rt2860apd");
    printf("CMD in set_start_8021x_daemon is %s\n", cmd);
    system(cmd);
    sleep(1);

    memset(cmd, 0, sizeof(cmd));
    /*Start daemon*/
    sprintf(cmd, "rt2860apd");
    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int get_security_mode(int radio, int vap_id)
{
    char buf[TMP_LEN];
    char security_mode_s[TMP_LEN];
    int secmode;
    /*Get the Radio*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("wl0_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
	}
    else if(RADIO_5G == radio)
        {
    
            ezplib_get_attr_val("wl1_sec_rule", vap_id, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);       
        }

    strcpy(security_mode_s, buf);  	
    if(!strcmp(security_mode_s, "wep"))
        {
            secmode = SECMODE_WEP;

        }
    else if(!strcmp(security_mode_s, "wpa"))
        {
            secmode = SECMODE_WPA;
        }
    else if(!strcmp(security_mode_s, "wpa2") || !strcmp(security_mode_s, "wpa2mixed"))
        {
            secmode = SECMODE_WPA2;
        }
    else if(!strcmp(security_mode_s, "psk"))
        {
            secmode = SECMODE_PSK;
        }
    else if(!strcmp(security_mode_s, "psk2") || !strcmp(security_mode_s, "psk2mixed"))
        {
            secmode = SECMODE_PSK2;
        }
    else
        {
            secmode = SECMODE_OPEN;
        }

    return secmode; 
}

int set_security_wpa2_psk(int radio, int vap_id)
{
    char TempBuf_opmode[32];
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif   
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            set_security_authmode_wpa2psk(radio, vap_id);
            set_security_encryptype_psk(radio, vap_id);
            /*Added by Mario Huang 2013-08-28*/
            set_ssid(radio, WLAN_MODE_AP, vap_id);
            set_security_8021x_enable(radio, vap_id, IEEE8021X_DISABLE);
            set_security_wpapsk(radio, vap_id);
            set_security_rekey(radio, vap_id);
            return T_SUCCESS;
        }
    else
        {
            printf("ERROR:This opmode is not supported\n");
            return T_FAILURE;
        }
}

int set_security_wpa2_8021x(int radio, int vap_id)
{
    char TempBuf_opmode[32];
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif   
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            set_security_authmode_8021x(radio, vap_id);
            set_security_encryptype_8021x(radio, vap_id);
            /*Added by Mario Huang 2013-08-28*/
            set_ssid(radio, WLAN_MODE_AP, vap_id);
            /*set IEEE8021X=1 only when Radius-WEP or Radius-NONE mode*/
            set_security_8021x_enable(radio, vap_id, IEEE8021X_DISABLE);
            set_security_radius(radio, vap_id);
            set_security_rekey(radio, vap_id);
            set_security_pmk_cacheperiod(radio, vap_id);
            set_security_preauth(radio, vap_id);
            set_security_bridge(radio, vap_id);
            set_ssid(radio, WLAN_MODE_AP, vap_id);
            set_start_8021x_daemon(radio);
            return T_SUCCESS;
        }
    else
        {
            printf("ERROR:This opmode is not supported\n");
            return T_FAILURE;
        }

}

int set_security_none(int radio, int vap_id)
{
    char TempBuf_opmode[32];
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif   
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            set_security_authmode_open(radio, vap_id);
            set_security_encryptype_none(radio, vap_id);
            /*Added by Mario Huang 2013-08-28*/
            set_ssid(radio, WLAN_MODE_AP, vap_id);
            return T_SUCCESS;
        }
    else
        {
            printf("ERROR:This opmode is not supported\n");
            return T_FAILURE;
        }

}

int set_ap_security(int radio, int vap_id)
{
    int secmode;
    int ret;
    secmode = get_security_mode(radio, vap_id);
    if(SECMODE_PSK2 == secmode)
        {
            ret = set_security_wpa2_psk(radio, vap_id);
            return ret;
        }
    else if(SECMODE_WPA2 == secmode)
        {
            ret = set_security_wpa2_8021x(radio, vap_id);
            return ret;
        }
    else if(SECMODE_OPEN == secmode)
        {
            ret = set_security_none(radio, vap_id);
            return ret;
        }
     else
        {
            printf("ERROR:This Security Mode is not support currently\n");
            return T_FAILURE;
        }
}


/**************************************************************/
/*ACL                                                         */
/**************************************************************/
int get_nvram_acl_policy(int radio, int vap_id)
{
    char acl_rule[32] = {0};
    char policy[32] = {0};
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
            return ACL_POLICY_DISABLE;  
        }
    else if(!strcmp(policy, "allow") || !strcmp(policy, "1"))
        {
            return ACL_POLICY_ALLOW;  
        }
    else if(!strcmp(policy, "deny") || !strcmp(policy, "2"))
	{
            return ACL_POLICY_DENY;  
	}
    else
        {
            printf("ERROR:Get ACL Policy error!!!");
        }
	return ACL_POLICY_UNDEFINED;  
}

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
    policy = get_nvram_acl_policy(radio, vap_id);
    sprintf(cmd, "iwpriv %s set AccessPolicy=%d", vap_name, policy);
    printf("CMD in set_acl_policy is %s\n", cmd);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    /*Clear All*/
    sprintf(cmd, "iwpriv %s set ACLClearAll=1", vap_name);
    printf("%s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int get_nvram_acl_list(int radio, int vap_id, ACL_LIST *acl_lst)
{
    char acl_num_rule[32] = {0};
    int acl_num;
    char acl_rule[32] = {0};
    char buf[32] = {0};
    char mac[32] = {0};
    char acl_enable_s[32] = {0};
    int acl_enable;
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
            strcpy(acl_enable_s, buf);  	
            acl_enable = atoi(acl_enable_s);  	
            if(ACL_DELETED == acl_enable)
                {
                    continue;
                }
            ezplib_get_attr_val(acl_rule, i, "mac", buf, 32, EZPLIB_USE_CLI);       
            strcpy(mac, buf);  	
            strcpy(acl_lst->mac[i].addr, mac);  	
            memset(mac, 0, sizeof(mac));
            acl_lst->acl_num = acl_lst->acl_num + 1;
        }
    return T_SUCCESS;
}

int set_acl_modify_list(int radio, int vap_id)
{
    ACL_LIST acl_list;
    char acl_list_string[2048] = "";
    char vap_name[8];
    int ret;
    char cmd[2048];
    int i;
    /*Construct VAP name*/
    ret = construct_vap(vap_name, radio, vap_id, WLAN_MODE_AP);
    if(T_FAILURE == ret)
    	{
    		printf("ERRO:Construct vap failure!\n");
		return T_FAILURE;
    	}
    //set_acl_policy(radio, vap_id);
    get_nvram_acl_list(radio, vap_id, &acl_list);
    //strcat(acl_list_string, "\"");
    for(i=0; i<acl_list.acl_num; i++)
        {
            strcat(acl_list_string, acl_list.mac[i].addr);
            strcat(acl_list_string, ";");
        }   
    //strcat(acl_list_string, "\"");

    sprintf(cmd, "iwpriv %s set ACLAddEntry=\'%s\'", vap_name, acl_list_string);
    printf("CMD in set_acl is %s\n", cmd);
    system(cmd);
    return T_SUCCESS;
}

int set_acl(int radio, int vap_id)
{
    char TempBuf_opmode[32];
    /*Get the operation mode*/
    if(RADIO_2G == radio)
        {
            ezplib_get_attr_val("system_mode", 0, "name", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#if 0
    else if(RADIO_5G == radio)
        {
            ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf_opmode, 32, EZPLIB_USE_CLI);
        }
#endif   
    if(!strcmp(TempBuf_opmode, "ap") || !strcmp(TempBuf_opmode, "normal"))
        {
            set_acl_policy(radio, vap_id);
            set_acl_modify_list(radio, vap_id);
            return T_SUCCESS;
        }
    else
        {
            printf("ERROR:This opmode is not supported in set_security_wpa2_psk\n");
            return T_FAILURE;
        }
}
/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
int set_device_mode(int radio)
{
    int device_mode;
    char radio_name[VAP_NAME_LEN];
    char cmd[128];
    int ret;
    int op_mode = 0;
    nvram_get_system_mode(&op_mode);
    switch(op_mode)
        {
            case OP_MODE_NORMAL:
            case OP_MODE_AP:
                device_mode = 0;
                break;  
            case OP_MODE_STA0:
                device_mode = 1;
                break;  
            case OP_MODE_WISP0:
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
    system(cmd);
    return T_SUCCESS;
}
int set_ap_wmm(int radio, int vap_id)
{
	return T_SUCCESS;
}
int set_ap_wmm_enable(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_cwmin(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_cwmax(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_aifs(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_txop(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_ac(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}
int set_ap_wmm_noack(int radio, int vap_id, wmm_cfg  *wmmcfg)
{
	return T_SUCCESS;
}

int set_ap_security_dfs(int radio)
{
	return T_SUCCESS;
}

