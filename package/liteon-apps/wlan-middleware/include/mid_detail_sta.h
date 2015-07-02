#ifndef __MID_DETAIL_STA_H__
#define __MID_DETAIL_STA_H__

#include "mid_detail.h"

/*---------STA Getters & Setters---------*/


int get_sta_assoc_ssid(int radio, char *ssid);
int get_sta_assoc_bssid(int radio, char *bssid);
int get_sta_assoc_authmode(int radio, int *auth_mode);
int get_sta_assoc_status(int radio, int *assoc);
int get_sta_assoc_ap_info(int radio, AP_INFO *ap_info);
int get_sta_assoc_rssi(int radio, char *rssi);
int get_sta_assoc_wmode(int radio, char *wmode);
int get_ethernet_client_list(int radio, ETHERNET_CLIENT_LIST *client_list);

int set_config_sta(int radio);
int set_apcli_disconnect_ap(int radio);


#endif		/* __MID_DETAIL_STA_H__ */
