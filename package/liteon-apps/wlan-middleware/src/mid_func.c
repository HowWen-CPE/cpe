//Added by Mario Huang 2013-07-15
#include    <stdio.h>

#include	"nvram.h"
#include	"nvram_rule.h"

#include	"mid_func.h"
#include	"mid_detail.h"
#include	"mid_common.h"


#include	"mid_detail_ap_get.h"
#include	"mid_detail_ap_set.h"
#include	"mid_detail_sta.h"	




/*****************************************************************/
/*Op Mode Switch                                                 */
/*****************************************************************/
int config_ssid(int radio)
{
    set_wlan_basic(radio);
    set_wlan_advance(radio);
    set_ap_wmm(radio, 0);
    return T_SUCCESS;
}

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
		/*reset wds on this radio, add by frank, IMPORTANT,
		 * otherwise wlanconfig wdsN list sta will be empty
		 * after 2 times of switch mode.
		*/
        set_ap_wds(radio);
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
