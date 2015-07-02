#ifndef __MID_DETAIL_AP_GET_H__
#define __MID_DETAIL_AP_GET_H__

#include "mid_detail.h"

/*---------AP Getters---------*/

int get_radio_status(int radio, int *radio_status);
int get_wlan_rate(int radio,int *rate);
int get_current_channel(int radio, Channel_t *channel_get);
int get_channel_list(int radio, ChannelListInfo_t *channel_list);

int get_ap_list(int radio, SCAN_AP_LIST *ap_list);
int get_sta_list(int radio, StationListInfo_t *sta_list_info);

int get_bandwidth(int radio, int *htbw);
int get_extchannel(int radio, Channel_t *extchannel_get);
int get_wirelessmode(int radio, int *mode);
int get_channel_select_mode(int radio, int *mode);
int get_ssid(int radio, int mode, int vap_id, char *ssid);
int get_dfschannel(DFS_CHAN_LIST *dfs_list);
int get_radarchannel(DFS_CHAN_LIST *radar_list);
int get_cacstate(unsigned int *cac_state);

int get_rogue_ap_list(SCAN_AP_LIST *ap_list);
int get_channel_listHT80(int radio, ChannelListInfo_t *channel_list);

#endif /*__MID_DETAIL_AP_GET_H__*/
