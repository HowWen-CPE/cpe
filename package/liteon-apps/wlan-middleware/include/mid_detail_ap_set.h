#ifndef __MID_DETAIL_AP_SET_H__
#define __MID_DETAIL_AP_SET_H__

#include "mid_detail.h"

/*---------AP Setters---------*/

/*---------Basic---------*/
int set_ap_wirelessmode_channel(int radio);
int set_ap_isolation(int radio);
int set_ssid(int radio, int mode, int vap_id);
int set_security_ssid(int radio, int mode, int vap_id, security_cfg *ap_cfg);
int set_hidden_ssid(int radio, int mode, int vap_id);
int set_bss_isolation(int radio, int mode, int vap_id);

/*---------Security---------*/
int set_ap_security(int radio, int vap_id);
int set_ap_security_dfs(int radio);

/*---------ACL---------*/
int set_acl(int radio, int vap_id);

/*---------Advanced---------*/
int set_dtim(int radio);
int set_power(int radio);
int set_beacon_interval(int radio);

/*---------WMM---------*/
int set_ap_wmm(int radio, int vap_id);
int set_ap_wmm_enable(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_cwmin(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_cwmax(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_aifs(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_txop(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_ac(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_noack(int radio, int vap_id, wmm_cfg  *wmmcfg);


#endif /*__MID_DETAIL_AP_SET_H__*/
