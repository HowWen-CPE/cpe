#include	<unistd.h>	/*sleep*/
#include	"mid_detail.h"
#include	"mid_common.h"
#include	"mid_common_nvram.h"

#include	"mid_detail_ap_get.h"
#include	"mid_detail_ap_set.h"
#include	"mid_detail_sta.h"	

#include	"nvram.h"
#include	"nvram_rule.h"

#include	"mid_func.h"
//#include 	"vlan_public.h"
//#include "vlan_enable.h"

int radio_up_down(int radio, int up, int mode)
{
    int i;
    int vap_num;
    int vap_status;
 
    /*Get VAP num from wlv_rule_num*/
    vap_num = nvram_get_vap_num(radio); 
    printf("vap_num is %d\n",vap_num);

    if(up)
    {
        /*Up the VAP which is enable*/
        if(WLAN_MODE_AP == mode)
        {
            for(i = 0; i < vap_num; i++)
                {    
                    nvram_get_vap_status(radio, i, &vap_status);
                    if(VAP_UP == vap_status)
                    {
                        vap_up_down(radio, i, mode, VAP_UP);    
                    }
                }
			for(i = 0; i < WDS_VAP_NUM; i++) 
				{
					set_ap_wds_up(radio, i);
				}
        }
        else if(WLAN_MODE_STA == mode)
        {
            vap_up_down(radio, 0, mode, VAP_UP);    
        }
        //sleep(1);
    }
    else
    {
        /*Down all the VAPs, down main VAP at last*/
        if(WLAN_MODE_AP == mode)
        {
            for(i = vap_num - 1; i >= 0; i--)
                {    
                    vap_up_down(radio, i, mode, VAP_DOWN);    
                }
			for(i = 0; i < WDS_VAP_NUM; i++) 
				{
					set_ap_wds_down(radio, i);
				}
        }
        else if(WLAN_MODE_STA == mode)
        {
            vap_up_down(radio, 0, mode, VAP_DOWN);    
        }
        //sleep(1);
    }

    return T_SUCCESS;
}

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
				//wds
				for(i=0;i<WDS_VAP_NUM;i++) {
					set_ap_wds_down(radio, i);
				}
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
            //wds
			for(i=0;i<WDS_VAP_NUM;i++) {
				set_ap_wds_down(radio, i);
			}
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
#if defined(VLAN_ENABLE)
				//Deleted by Mario Huang
				//set_wlan_vlan(radio,i);
                //preserve 3 seconds for main VAP
#endif
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
	//Deleted by Mario Huang 2014-08-12
	//set_ap_wmm(radio, 0);
	//Added by Mario Huang for VLAN test
#if defined(VLAN_ENABLE)
	updateVlan();
#endif
	//wds
	for(i=0;i<WDS_VAP_NUM;i++) {
		set_ap_wds_up(radio, i);
	}
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

	//Deleted by Mario Huang 2014-08-12
	//set_ap_wmm(radio, 0);
    return T_SUCCESS;
}

/*Switch to STA mode*/
int set_opmode_sta(int radio, int wlan_mode)
{
    printf("Enter set_opmode_sta\n");
    set_config_sta(radio);
    return T_SUCCESS;
}

int set_opmode_ap(int radio, int wlan_mode)
{
    /*Create VAPs First*/
    printf("Enter set_opmode_ap\n");
    //create_all_vap(radio, WLAN_MODE_AP);
    config_ssid(radio);

    return T_SUCCESS;
}

//Added by Mario Huang for BUG5232
int set_enable_radio_before_network(int radio, int mode_prev, int mode_curr)
{

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
