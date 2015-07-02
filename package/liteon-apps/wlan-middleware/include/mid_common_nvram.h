#ifndef __MID_COMMON_NVRAM_H__
#define __MID_COMMON_NVRAM_H__

#include "mid_detail.h"
int nvram_get_wlan_mode(int radio, int *wlan_mode);
int nvram_get_radio_status(int radio, int *radio_status);
int nvram_get_vap_status(int radio, int vap_id, int *vap_status);
int nvram_get_ap_wireless_mode(int radio, int *wmode);
int nvram_get_bandwidth(int radio, int *htbw);
int nvram_get_txpower(int radio, int *txpower);
int nvram_get_vap_num(int radio);
int nvram_get_ap_channel(int radio, int *channel);
int nvram_get_ap_ext_channel_mode(int radio, int *extcha);
int nvram_get_ap_channel_select_mode(int radio, int *sel_mode);
int nvram_get_ap_isolation(int radio, int *ap_isolation);
int nvram_get_ap_ssid(int radio, int vap_id, char *ssid, int bProcSpecChar);
int nvram_get_ap_hidden(int radio, int *hidden, int vap_id);
int nvram_get_ap_bss_isolation(int radio, int vap_id, int *bss_isolation);
int nvram_get_ap_dtim(int radio, int *dtim);
int nvram_get_ap_beacon_interval(int radio, int *bcn);
int nvram_get_ap_wpacap(int radio, int vap_id, int *wpacap);
int nvram_get_ap_secmode(int radio, int vap_id, int *secmode);
int nvram_get_security_encryptype(int radio, int vap_id, int *encryp_type);
int nvram_get_ap_passphrase(int radio, int vap_id, char *key);
int nvram_get_ap_rekey_mode(int radio, int vap_id, int *rekey_mode);
int nvram_get_ap_rekey_time_interval(int radio, int vap_id, int *interval);
int nvram_get_ap_rekey_packet_interval(int radio, int vap_id, int *interval);
int nvram_get_ap_radius_ipaddr(int radio, int vap_id, char *server);
int nvram_get_ap_radius_port(int radio, int vap_id, int *port);
int nvram_get_ap_radius_key(int radio, int vap_id, char *key);
int nvram_get_ap_radius_own_ip(int radio, int vap_id, char *ownip);
int nvram_get_ap_pmk_period(int radio, int vap_id, int *pmk_period);
int nvram_get_ap_preauth(int radio, int vap_id, int *preauth);
int nvram_get_ap_acl_policy(int radio, int vap_id, int *policy);
int nvram_get_ap_acl_list(int radio, int vap_id, ACL_LIST *acl_lst);

int nvram_get_sta_ssid(int radio, char *ssid, int bProcSpecChar);
int nvram_get_sta_bssid(int radio, char *bssid);
int nvram_get_sta_secmode(int radio);
int nvram_get_sta_addrmode(int radio, int *addr_mode);
int nvram_get_sta_mac_repeater(int radio, int *mac_repeater);
int nvram_get_sta_crypto(int radio); 
int nvram_get_sta_psk(int radio, char *key); 
int nvram_get_sta_wep(int radio, char *key
	, int *key_idx, int *key_type, char *encmode, int* wep_encry);
int nvram_get_sta_eap_mode(int radio);
int nvram_get_sta_radius_user(int radio, char *radius_user);
int nvram_get_sta_radius_password(int radio, char *radius_pwd);

int nvram_get_wds_mode(int radio, int *mode);
int nvram_get_wds_mac(int radio, int vap_id, char *mac);
int nvram_get_wds_sec_mode(int radio, int vap_id, int *secmode);
int nvram_get_wds_passphrase(int radio, int vap_id, char *key, int bProcSpecChar);

int nvram_get_lan_bridge(int radio, int vap_id, char *bridge);
#endif /*__MID_COMMON_NVRAM_H__*/
