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

int CheckHexString(char *p);

/*>>>>>>>>>>Common used Macros Start>>>>>>>>>>*/
int find_pattern_from_str(char str[], char pattern[]);
int wait_for_bridge_setup(int radio, int mode);
//Execute one command and print it if in debug mode

int string_end(char *Buf);

void delspace(char *Buf1, char* Buf2);
void delspace_bssid(char *Buf1, char* Buf2);
void delspace1(char *Buf1, char* Buf2);
void delspace2(char *Buf1, char* Buf2);

void convert_bssid(char *Buf1, char* Buf2);
void strcpy_delspace(char *str1, char *str2);

void format_bssid_upper(char *str1, char *str2);
void format_bssid_lower(char *str1, char* str2);
void format_ssid(char *str1, char* str2);
int convert_special_characters(int radio, char *target_str
		, int target_len, char *origin_str);

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
int extract_channel(char channel[], char str[], char pattern[]);
//Get associated ssid of sta from a string
int extract_assoc_ssid(char ssid[], char str[], char pattern[]);

//Read one line from a file
void readline(char str[], FILE *fin);

int convert_vap_id(int radio, unsigned char vap_id);
int construct_vap(char* name, unsigned int radio, 
		unsigned char vap_id, unsigned char mode);
int construct_wds(char* name, unsigned int radio,
		unsigned char vap_id);
int get_vap_link_status(int *status, char str[], char pattern[]);

int construct_main_ap(char* name, unsigned int radio);
int vap_up_down(int radio, int vapid, int mode, int up);
int kill_all_authentication_daemon(int radio, int mode);


int create_vap(int radio, int vapid, int mode);
int destroy_vap(int radio, int vapid, int mode);
int create_all_vap(int radio, int mode);
int destroy_all_vap(int radio, int mode);
int radio_up_down(int radio, int up, int mode);

int down_all_ssid(int radio);
int nvram_get_system_mode(int *opmode);
int get_sta_assoc_secmode(int radio);

int set_config_sta(int radio);
int set_extra_sta(int radio);

int get_channel(char channel[], char str[], char pattern[]);
int get_sta_bssid(char bssid[], char str[], char pattern[]);
int get_sta_ssid(char ssid[], char str[], char pattern[]);



#endif //End of __MID_COMMON_H_
