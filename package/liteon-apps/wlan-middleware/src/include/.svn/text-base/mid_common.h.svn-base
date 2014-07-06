/******************************************************************************
**
** FILE NAME    : common_use.h
** PROJECT      : LGP
** MODULES      : Middleware
**
** DATE         : 29 Nov 2013
** AUTHOR       : Mario Huang
** DESCRIPTION  : 
**
**
** HISTORY
** $Date        $Author         $Comment
** 29 Nov 2013  Mario Huang     Create File
*******************************************************************************/
#ifndef __MID_COMMON_H_
#define __MID_COMMON_H_

#include	<stdlib.h>
#include	<stdio.h>
#include <string.h>

#include "mid_detail.h"
/*>>>>>>>>>>Common used Macros Start>>>>>>>>>>*/
int find_pattern_from_str(char str[], char pattern[]);
int wait_for_bridge_setup(int radio, int mode);
//Execute one command and print it if in debug mode
#if defined(GP_WP688)
int string_end(char *Buf);
void delspace(char *Buf1, char* Buf2);
void delspace_bssid(char *Buf1, char* Buf2);
void convert_bssid(char *Buf1, char* Buf2);
void delspace1(char *Buf1, char* Buf2);
void delspace2(char *Buf1, char* Buf2);
#elif defined(GP_WP838)
int string_end(char *str);
void strcpy_delspace(char *str1, char *str2);
void format_bssid_upper(char *str1, char *str2);
void format_bssid_lower(char *str1, char* str2);
void format_ssid(char *str1, char* str2);
int convert_special_characters(int radio, char *target_str, int target_len, char *origin_str);
#endif
/*>>>>>>>>>>Infterfaces>>>>>>>>>>*/
int isLeapYear(int year);
date sec2date(long sec);
int length(char str[]);
int null_string(char *str);

/* add by frank*/
unsigned char centerHT80(int channel);

/* Added by Andy in 2014/01/23: Get Lan Mac for Ethernet Client List */
int get_lan_mac(char *lanhwaddr);
int get_hwaddr(char *ifname, char *ptr, int length);

//Get channel from string
#if defined(GP_WP688)
int extract_channel(char channel[], char str[], char pattern[]);
#elif defined(GP_WP838)
#endif

//Get associated bssid of sta from a string
#if defined(GP_WP688)
int extract_assoc_bssid(char bssid[], char str[], char pattern[]);
#elif defined(GP_WP838)
#endif
//Get associated ssid of sta from a string
#if defined(GP_WP688)
int extract_assoc_ssid(char ssid[], char str[], char pattern[]);
#elif defined(GP_WP838)
#endif
//Read one line from a file

void readline(char str[], FILE *fin);

int convert_vap_id(int radio, unsigned char vap_id);
int construct_vap(char* name, unsigned int radio, unsigned char vap_id, unsigned char mode);
#if defined(GP_WP688)
int get_vap_link_status(int *status, char str[], char pattern[]);
#elif defined(GP_WP838)
#endif
int construct_main_ap(char* name, unsigned int radio);
int vap_up_down(int radio, int vapid, int mode, int up);
#if defined(GP_WP688)
int radio_up_down(int radio, int up);
#elif defined(GP_WP838)
int kill_all_authentication_daemon(int radio, int mode);
int radio_up_down(int radio, int up, int mode);
#endif
int create_vap(int radio, int vapid, int mode);
int destroy_vap(int radio, int vapid, int mode);
int create_all_vap(int radio, int mode);
int destroy_all_vap(int radio, int mode);
#if defined(GP_WP688)
int down_all_ssid(int radio);
#elif defined(GP_WP838)
#endif
/*<<<<<<<<<<Infterfaces<<<<<<<<<<*/
int nvram_get_system_mode(int *opmode);
#if defined(GP_WP688)
#elif defined(GP_WP838)
int nvram_get_radio_status(int radio, int *radio_status);
int nvram_get_vap_status(int radio, int vap_id, int *vap_status);
int nvram_get_ap_wireless_mode(int radio, int *wmode);
int nvram_get_bandwidth(int radio, int *htbw);
int nvram_get_txpower(int radio, int *txpower);
int nvram_get_connrssi(int radio, int *connrssi);
int nvram_get_disconnrssi(int radio, int *disconnrssi);
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
int nvram_get_sta_wep(int radio, char *key, int *key_idx, int *key_type, char *encmode, int* wep_encry);
int nvram_get_sta_eap_mode(int radio);
int nvram_get_sta_radius_user(int radio, char *radius_user);
int nvram_get_sta_radius_password(int radio, char *radius_pwd);
#endif
int nvram_get_wlan_mode(int radio, int *wlan_mode);
int get_sta_assoc_secmode(int radio);

int set_config_sta(int radio);
int set_extra_sta(int radio);

int get_channel(char channel[], char str[], char pattern[]);
int get_sta_bssid(char bssid[], char str[], char pattern[]);
int get_sta_ssid(char ssid[], char str[], char pattern[]);


#endif //End of __MID_COMMON_H_
