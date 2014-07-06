//Added by Mario Huang 2013-07-15
#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <unistd.h>

#include "nvram.h"
#include "nvram_rule.h"

#include "mid_func.h"
#include "mid_detail.h"
#include "mid_common.h"

#if defined(GP_WP688)
int set_wlan_basic(int radio)
{
    char buf[TMP_LEN];
    char ssid_enable_s[TMP_LEN];
    int ssid_enable;
    char buf_nvram[TMP_LEN];
    char radio_nvram_enable_s[TMP_LEN];
    int radio_nvram_enable;
    int i;
    int ssid_num;
    int ret;
    printf("Enter set_wlan_basic\n");

    ezplib_get_attr_val("wl_ap_basic_rule", 0, "enable", buf_nvram, TMP_LEN, EZPLIB_USE_CLI); 			
    strcpy(radio_nvram_enable_s, buf_nvram);  	
    radio_nvram_enable = atoi(radio_nvram_enable_s);

    if(radio_nvram_enable)
        {
            printf("radio_nvram_enable 1\n");
            ret = radio_up_down(radio, RADIO_UP);
        }
    else
        {
            printf("radio_nvram_enable 0\n");
            ret = radio_up_down(radio, RADIO_DOWN);
            return T_SUCCESS;
        }
/*RADIO*/
    /*net mode and channel*/
    ret = set_wirelessmode_channel(radio);
    /*ap isolation*/
    ret = set_ap_isolation(radio);

/*VAP*/
    ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
    //for(i=1; i < ssid_num; i++)
    for(i=0; i < ssid_num; i++)
    	{
    		/*Set VAP enable or disable*/
		if(RADIO_2G == radio)
			{
				ezplib_get_attr_val("wl0_basic_rule", i, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 			
			}
		else if(RADIO_5G == radio)
			{
				ezplib_get_attr_val("wl1_basic_rule", i, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 			
			}
		
		strcpy(ssid_enable_s, buf);  	
		ssid_enable = atoi(ssid_enable_s);
    		if(SSID_ENABLE == ssid_enable)
    			{
                               /*Configure Security Before ssid up*/
                               set_ap_security(radio, i);
                               /*Configure ACL Before ssid up*/
                               set_acl(radio, i);

    				ret = set_enable_ssid(radio, WLAN_MODE_AP, i);
			       if(T_FAILURE == ret)
    					{
				    		printf("ERRO:Enable SSID failure!\n");
						return T_FAILURE;
    					}
                               vap_up_down(radio, i, WLAN_MODE_AP, VAP_UP);
                               //sleep(1);
    			}
		else if(SSID_DISABLE == ssid_enable)
			{
    				ret = set_disable_ssid(radio, WLAN_MODE_AP, i);
			       if(T_FAILURE == ret)
    					{
				    		printf("ERRO:Disable SSID failure!\n");
						return T_FAILURE;
    					}
			}
		memset(ssid_enable_s, 0, sizeof(ssid_enable_s));
		ssid_enable = 0;	
    	}
    return T_SUCCESS;
}

int set_wlan_advance(int radio)
{
    int ret;
    /*dtim*/
    ret = set_dtim(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set DTIM failure!\n");
            return T_FAILURE;
        }
    /*txpower*/
    ret = set_power(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set POWER failure!\n");
            return T_FAILURE;
        }
    /*bcn*/
    ret = set_beacon_interval(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set BCN Interval failure!\n");
            return T_FAILURE;
        }
    return T_SUCCESS;
}
#elif defined(GP_WP838)
int set_wlan_basic(int radio)
{
    int vap_enable;
    int radio_nvram_enable;
    int i;
    int vap_num;
    int wlan_mode = -1;
    nvram_get_radio_status(radio, &radio_nvram_enable);
    nvram_get_wlan_mode(radio, &wlan_mode);

    if(RADIO_DOWN == radio_nvram_enable)
        {
            //Kill all the security daemon first
//            if(WLAN_MODE_AP == wlan_mode)
//            {
                radio_up_down(radio, RADIO_DOWN, wlan_mode);
                kill_all_authentication_daemon(radio, wlan_mode);
//            }
//            else if(WLAN_MODE_STA == wlan_mode)
//            {
//                kill_all_authentication_daemon(radio, wlan_mode);
//                radio_up_down(radio, RADIO_DOWN, wlan_mode);
//            }
            return T_SUCCESS;
        }

//    if(WLAN_MODE_AP == wlan_mode)
//        {
            //Down all the VAPs first
            radio_up_down(radio, RADIO_DOWN, wlan_mode);
            //Kill all the security daemon first
            kill_all_authentication_daemon(radio, wlan_mode);
//        }
//    else if(WLAN_MODE_STA == wlan_mode)
//        {
            //Kill all the security daemon first
//            kill_all_authentication_daemon(radio, wlan_mode);
            //Down all the VAPs first
//            radio_up_down(radio, RADIO_DOWN, wlan_mode);
//        }
    /*net mode and channel*/
    set_ap_wirelessmode_channel(radio);
    /*ap isolation*/
    vap_num = nvram_get_vap_num(radio); 
    //for(i=1; i < vap_num; i++)
    for(i=0; i < vap_num; i++)
	{
        nvram_get_vap_status(radio, i, &vap_enable);
        if(VAP_ENABLE == vap_enable)
            {
				set_ssid(radio, WLAN_MODE_AP, i);//add by frank
				set_acl(radio, i);
				//set_enable_ssid(radio, WLAN_MODE_AP, i);
				set_hidden_ssid(radio, WLAN_MODE_AP, i);//add by frank
				set_bss_isolation(radio, WLAN_MODE_AP, i);//add by frank
				set_ap_security(radio, i);
                //preserve 3 seconds for main VAP
                if(0==i) {
					sleep(3);
				}
            }
        else if(VAP_DISABLE == vap_enable)
            {
                //set_disable_ssid(radio, WLAN_MODE_AP, i);
                //vap_up_down(radio, i, WLAN_MODE_AP, VAP_DOWN);
            }		
	}
	set_ap_isolation(radio);//add by frank, impl on 2014-02-17
	set_ap_wmm(radio, 0);
    return T_SUCCESS;
}

int set_wlan_advance(int radio)
{
    int ret;
    int wlan_mode = -1;
    nvram_get_wlan_mode(radio, &wlan_mode);
    radio_up_down(radio, RADIO_DOWN, wlan_mode);
    /*dtim*/
    ret = set_dtim(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set DTIM failure!\n");
            return T_FAILURE;
        }
    /*bcn*/
    ret = set_beacon_interval(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set BCN Interval failure!\n");
            return T_FAILURE;
        }

    radio_up_down(radio, RADIO_UP, wlan_mode);

    /*txpower*/
    ret = set_power(radio);
    if(T_FAILURE == ret)
        {
            printf("ERRO:Set POWER failure!\n");
            return T_FAILURE;
        }
	set_ap_wmm(radio, 0);
    return T_SUCCESS;
}
#endif



/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
int config_ssid(int radio)
{
    set_wlan_basic(radio);
    set_wlan_advance(radio);
    return T_SUCCESS;
}

/*Switch to STA mode*/
#if defined(GP_WP688)
int set_opmode_sta(int radio, int wlan_mode)
{
    /*rai0 up*/
    vap_up_down(radio, 0, WLAN_MODE_AP, VAP_UP);
    
    config_apcli(radio);
    set_device_mode(radio);
    //config_ssid(radio);
    set_apcli_basic(radio);
    /*apclii0 up*/
    vap_up_down(radio, 0, WLAN_MODE_STA, VAP_UP);
    /*Set all the vaps except apclii0 down*/
    down_all_ssid(radio);
    return T_SUCCESS;
}
#elif defined(GP_WP838)
int set_opmode_sta(int radio, int wlan_mode)
{
    printf("Enter set_opmode_sta\n");
    set_config_sta(radio);
    return T_SUCCESS;
}
#endif

/*Switch to AP mode*/
#if defined(GP_WP688)
int set_opmode_ap(int radio, int wlan_mode)
{
    //printf("Enter set_opmode_ap\n");
    /*radio down*/
    radio_up_down(radio, RADIO_DOWN);
    /*rai0 up*/
    radio_up_down(radio, RADIO_UP);
    /*Config all the ssids*/
    config_ssid(radio);
    /*set device mode*/
    set_device_mode(radio);

    return T_SUCCESS;
}
#elif defined(GP_WP838)
int set_opmode_ap(int radio, int wlan_mode)
{
    /*Create VAPs First*/
    printf("Enter set_opmode_ap\n");
    //create_all_vap(radio, WLAN_MODE_AP);
    config_ssid(radio);

    return T_SUCCESS;
}
#endif

int set_operation_mode(int radio, int mode_prev, int mode_curr)
{
    int ssid_num = 0;
    int i;
    printf("%s(%s, %s, %s)\r\n", __FUNCTION__, 
            radio == RADIO_2G ? "RADIO_2G" : "RADIO_5G", 
            mode_prev == WLAN_MODE_AP ? "WLAN_MODE_AP" : "WLAN_MODE_STA", 
            mode_curr == WLAN_MODE_AP ? "WLAN_MODE_AP" : "WLAN_MODE_STA");
    MID_ASSERT((WLAN_MODE_AP == mode_curr) || (WLAN_MODE_STA == mode_curr), "Error:Such wlan mode is not supported currently");
    MID_ASSERT((WLAN_MODE_AP == mode_prev) || (WLAN_MODE_STA == mode_prev), "Error:Such wlan mode is not supported currently");
    //We must make sure the bridge has been setup and the VAP has been added into the bridge before configure VAPs
    wait_for_bridge_setup(radio, mode_curr);

    if((mode_prev == mode_curr) && (mode_curr == WLAN_MODE_AP))
    {
        ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
        //for(i=1; i < ssid_num; i++)
        for(i=0; i < ssid_num; i++)
        { 
            set_ap_security(radio, i);
        }
        return T_SUCCESS;
    }

    switch(mode_curr)
        {
            case WLAN_MODE_AP:
                set_opmode_ap(radio, mode_curr);
                break;  
            case WLAN_MODE_STA:
                set_opmode_sta(radio, mode_curr);
                break;  
            default:
                printf("Error:Such wlan mode is not supported current, please check it\n");
                return T_FAILURE;
        }
    return T_SUCCESS;
}
//Added by Mario Huang for BUG5232
int set_enable_radio_before_network(int radio, int mode_prev, int mode_curr)
#if defined(GP_WP688)
{
    radio_up_down(radio, RADIO_UP);
    return T_SUCCESS;
}
#elif defined(GP_WP838)
{

    int wlan_mode = -1;
    printf("%s(%s, %s, %s)\r\n", __FUNCTION__, 
            radio == RADIO_2G ? "RADIO_2G" : "RADIO_5G", 
            mode_prev == WLAN_MODE_AP ? "WLAN_MODE_AP" : "WLAN_MODE_STA",
            mode_curr == WLAN_MODE_AP ? "WLAN_MODE_AP" : "WLAN_MODE_STA");
    MID_ASSERT((WLAN_MODE_AP == mode_curr) || (WLAN_MODE_STA == mode_curr), "Error:Such wlan mode is not supported currently");
    MID_ASSERT((WLAN_MODE_AP == mode_prev) || (WLAN_MODE_STA == mode_prev), "Error:Such wlan mode is not supported currently");
    if((mode_prev == mode_curr) && (mode_curr == WLAN_MODE_AP))
    {
        /*Only when ap-->ap, ap-->router or router-->ap is in this situation, we do noting in wlan for this situration*/
        return T_SUCCESS;
    }
    else if((mode_prev == mode_curr) && (mode_curr == WLAN_MODE_STA))
    {
        //Kill all the security daemon first
        kill_all_authentication_daemon(radio, mode_prev);
        /*Only when sta-->wisp or wisp-->sta is in this situation, so down the vap and configure it later, 
        do not need to re-create vap*/
        radio_up_down(radio, RADIO_DOWN, mode_prev);
        return T_SUCCESS;
    }
    else
    {
        destroy_all_vap(radio, mode_prev);
        create_all_vap(radio, mode_curr);
    }

    return T_SUCCESS;
}
#endif
