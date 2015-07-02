#ifndef __MID_FUNC_H_
#define __MID_FUNC_H_
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	"mid_detail_ap_get.h"
#include	"mid_detail_ap_set.h"
#include	"mid_detail_sta.h"
#include	"mid_detail_wds.h"
#include	"mid_common_nvram.h"

int set_wlan_basic(int radio);
int set_wlan_advance(int radio);
int set_operation_mode(int radio, int mode_prev, int mode_curr);
int set_enable_radio_before_network(int radio, int mode_prev, int mode_curr);

int set_opmode_sta(int radio, int wlan_mode);
int set_opmode_ap(int radio, int wlan_mode);
int config_ssid(int radio);


#endif /*__MID_FUNC_H_*/
