#ifndef __MDI_DETAIL_H_
#define __MDI_DETAIL_H_
#include    <stdlib.h>
#include    <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef DEBUG_MIDDLEWARE
    #define DEBUG_MIDDLEWARE
#endif

#define EXE_doSystem(X, ...)	do{ \
	char *cmd = (char *)malloc(128*sizeof(char));\
	sprintf(cmd,X,##__VA_ARGS__);\
	printf("\033[1;33;40m");\
	printf(cmd);\
	printf("\033[0m");\
	printf("\n");\
	system(cmd);\
	free(cmd);\
} while(0)

#define EXE_COMMAND(cmd)  do{ \
							  printf("\033[1;33;40m");\
							  printf(cmd);\
							  printf("\033[0m");\
                              printf("\n"); \
                              system(cmd); \
                            } while(0)

//DEBUG 
#if defined(DEBUG_MIDDLEWARE) 
  #define MID_ASSERT(cond, format)  do{ if(!(cond)) printf(__FILE__ ":%u:%s: " format "\n", __LINE__, __FUNCTION__); } while(0)
#else
  #define MID_ASSERT(cond, format)
#endif

//ERROR Process 
#if defined(DEBUG_MIDDLEWARE)
  #define MID_ERROR(format)  do{ printf(__FILE__ ":%u:%s: " format "\n", __LINE__, __FUNCTION__); } while(0)
#else
  #define MID_ERROR(format)
#endif

#define SINGLE_RADIO 1
//#define SINGLE_RADIO_2G 1
//#define SINGLE_RADIO_2G_ID 0
#define SINGLE_RADIO_5G 1
#define SINGLE_RADIO_5G_ID 0

#define MAC_LEN 15
#define ACL_UNCHANGED 0
#define ACL_ADD 1
#define ACL_DELETED 2
#define SITE_SURVEY_ENABLE 1
#define SITE_SURVEY_DISABLE 0
#define APCLI_NOT_ASSOCIATED 0
#define APCLI_ASSOCIATED 1

#define VAP_NAME_LEN                32 
#define RADIOA_VAP_MAX_NUM          32
#define EXT_CHANNEL_ABOVE           1
#define EXT_CHANNEL_BELOW           0

#define TMP_LEN 256 //from httpd common.h
#define SSID_ENABLE             1
#define SSID_DISABLE            0

#define RADIO_2G                0
#define RADIO_5G                1

#define RADIO_UP                1
#define RADIO_DOWN              0

#define VAP_UP            1
#define VAP_DOWN                0

#define ACL_POLICY_DISABLE      0
#define ACL_POLICY_ALLOW        1
#define ACL_POLICY_DENY 2
#define ACL_POLICY_UNDEFINED	-1

#define WPA_COMPATIBLE  1

#define IEEE8021X_ENABLE 1
#define IEEE8021X_DISABLE 0

#define BSS_COEX_DISABLE 0
#define BSS_COEX_ENABLE 1

#define BANDWIDTH_20 0
#define BANDWIDTH_40 1
#define BANDWIDTH_80 2

#define MAC_REPEATER_ENABLE 1
#define MAC_REPEATER_DISABLE 0  

#define STA_ENABLE 1
#define STA_DISABLE 0

#define T_SUCCESS  0
#define T_FAILURE  -1

#define CHANNEL_AUTO 0
#define CHANNEL_FIXED 1
#define MAC_ADDR_LENGTH 6

#define OP_MODE_NORMAL  0
#define OP_MODE_AP      1
#define OP_MODE_UR      5
#define OP_MODE_WISP0    6
#define OP_MODE_WISP1 7
#define OP_MODE_STA0    8
#define OP_MODE_STA1    9

#define WLAN_MODE_AP    0
#define WLAN_MODE_STA    1

#if defined(GP_WP688)
#define MAX_STA_NUM    32
#elif defined(GP_WP838)
#define MAX_STA_NUM    64
#else
#define MAX_STA_NUM    32
#endif

#define MAX_CHANNEL_NUM 32

#define NVRAM_BUF_LEN    32
#define NVRAM_SSID_LEN    33
#define NVRAM_PSK_LEN    256
#define NVRAM_8021X_NAME_LEN    65
#define NVRAM_8021X_PWD_LEN    65

#define YEAR 1970
/*<<<<<<<<<<Common used Macros End<<<<<<<<<<*/

#if defined(GP_WP688)
#elif defined(GP_WP838)
#define VAP_ENABLE    1
#define VAP_DISABLE    0
#define EAP_MODE_NONE 0
#define EAP_MODE_PEAP 1
#define EAP_MODE_TTLS 2
#define POWER_FULL 0
#define POWER_RONE 2
#define POWER_RTWO 4
#define POWER_HALF 6 
#define POWER_RFOUR 8
#define POWER_RFIVE 10
#define POWER_QUARTER 12
#define POWER_RSEVEN 14
#define POWER_REIGHT 16
#define POWER_EIGHTH 18
#define POWER_MIN 20
#endif //End of #if defined(GP_WP688)

#if defined(GP_WP688)
#define AP_NAME_2G          "rai"
#define WDS_NAME_2G         "wdsi"
#define STA_NAME_2G               "apclii"
#define AP_NAME_5G          "ra"
#define WDS_NAME_5G         "wds"
#define STA_NAME_5G               "apcli"
#elif defined(GP_WP838)
#define AP_NAME_2G          "ath"
#define WDS_NAME_2G         ""
#define STA_NAME_2G         "sta"
#define AP_NAME_5G          "ath"
#define WDS_NAME_5G         ""
#define STA_NAME_5G         "sta"
#endif //End of #if defined(GP_WP688)

#if defined(GP_WP688)
#define SECMODE_OPEN 0
#define SECMODE_WEP 1
#define SECMODE_WPA 2
#define SECMODE_WPA2 3
#define SECMODE_PSK 4
#define SECMODE_PSK2 5
#define STA_AUTHMODE_NONE    0
#define STA_AUTHMODE_WPAPSK    1
#define STA_AUTHMODE_WPA2PSK    2
#define STA_AUTHMODE_WPA1PSKWPA2PSK    3
#define STA_AUTHMODE_WPA    4
#define STA_AUTHMODE_WPA2    5
#define STA_AUTHMODE_WPA1WPA2    6
#define STA_AUTHMODE_WEP    7
#elif defined(GP_WP838)
#define AUTHMODE_UNDEFINED    -1
#define AUTHMODE_NONE    0
#define AUTHMODE_WPAPSK    1
#define AUTHMODE_WPA2PSK    2
#define AUTHMODE_WPAPSKWPA2PSK    3
#define AUTHMODE_WPA    4
#define AUTHMODE_WPA2    5
#define AUTHMODE_WPAWPA2    6
#define AUTHMODE_WEP    7
#endif //End of #if defined(GP_WP688)

#if defined(GP_WP688)
#define STA_ENCRY_NONE    0
#define STA_ENCRY_AES    1
#define STA_ENCRY_TKIP    2    
#define STA_ENCRY_TKIPAES    3    
#define STA_ENCRY_WEP    4    
#elif defined(GP_WP838)
#define ENCRY_NONE    0
#define ENCRY_AES    1
#define ENCRY_TKIP    2    
#define ENCRY_TKIPAES    3    
#define ENCRY_WEP    4    
#endif //End of #if defined(GP_WP688)

#if defined(GP_WP688)
#elif defined(GP_WP838)
#define ACL_UNCHANGED_MAC 0
#define ACL_ADD_MAC 1
#define ACL_DELETED_MAC 2
#endif //End of #if defined(GP_WP688)

#if defined(GP_WP688)
#elif defined(GP_WP838)
#define AP_ISOLATION_ENABLE 1
#define AP_ISOLATION_DISABLE 0
#define BSS_ISOLATION_ENABLE 1
#define BSS_ISOLATION_DISABLE 0
#define HIDE_SSID_ENABLE    1
#define HIDE_SSID_DISABLE    0
#define REKEY_MODE_TIME    1
#define REKEY_MODE_PKT    2
#define REKEY_MODE_DISABLE    0
#define STA_NOT_ASSOCIATED 0 
#define STA_ASSOCIATED 1
#endif //End of #if defined(GP_WP688)

#define	RADIO2_DEFAULT	"wifi0"
#define	RADIO5_DEFAULT	"wifi1"

#include <linux/wireless.h>

#ifndef SIOCDEVPRIVATE
/*SIOCDEVPRIVATE is defined in wireless.h, mark by frank*/
#define SIOCDEVPRIVATE                          0x8BE0
#endif

#ifndef SIOCIWFIRSTPRIV
#define SIOCIWFIRSTPRIV                         SIOCDEVPRIVATE
#endif

#ifndef RTPRIV_IOCTL_GET_MAC_TABLE
#define RTPRIV_IOCTL_GET_MAC_TABLE              (SIOCIWFIRSTPRIV + 0x0F)
#endif

#ifndef RTPRIV_IOCTL_GET_RADAR_STATUS
#define RTPRIV_IOCTL_GET_RADAR_STATUS           (SIOCIWFIRSTPRIV + 0x0B)
#endif

#if defined(GP_WP688)

#define NETWORK_TYPE_IN    1
#define NETWORK_TYPE_AD    0    
#define SITE_SURVEY_FILE "/proc/sitesurvey"
#elif defined(GP_WP838)
#define ATH_ACL_DISABLE    	0
#define ATH_ACL_ALLOW    	1
#define ATH_ACL_DENY    	2
#define ATH_ACL_FLUSH    	3
#define DFS_GET_NOL			14
#define DFS_IS_CAC_DONE		20
#define SIOCGATHPHYERR      0x89F5
#endif //End of #if defined(GP_WP688)

//const int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
#define HOSTAPD_CFG_FILE "/tmp/ath"
#define HOSTAPD_CFG_BACKUP "/tmp/bak.ath"
#define HOSTAPD_PID_FILE "/var/run/hostapd_ath"
#define WPA_SUPPLICANT_CFG_PATH "/tmp/"
#define WPA_SUPPLICANT_PID_PATH "/var/run/"

#define CLI_BSS_F_FREE              0x01
#define CLI_BSS_F_SSID_SUPRESS      0x02
#define CLI_BSS_F_SECURE            0x04
#define CLI_BSS_F_HASHED_KEY        0x08
#define CLI_BSS_F_VAP_FORWARDING    0x10

#define MAX_PASSPHRASE 64
#define MIN_PASSPHRASE 8

#define WMM_SET_AP_ONLY 0
#define WMM_SET_STA_ONLY 1

enum
{
    CLI_AUTH_OPEN = 0,
    CLI_AUTH_SHARED,
    CLI_AUTH_AUTO,
    CLI_AUTH_8021X,
    CLI_AUTH_WPA,
    CLI_AUTH_WPAPSK,
    CLI_AUTH_WPA2,
    CLI_AUTH_WPA2PSK,
    CLI_AUTH_WPA_AUTO,
    CLI_AUTH_WPA_AUTO_PSK,
    CLI_MAX_AUTH
};


//Used to set security
typedef struct security_cfg_s
{
	char vap_name[8];   /* interface name for this bss */
	int flags;
	char ssid[33];
	int authmode;
	int rekey_interval;//As time
	char encryp_type[16];
	int preauth;
	char passphrase[256];
	char radius_addr[16];
	char own_ipaddr[16];
	int radius_port;
	char radius_secret[256];
	char bridge_addr[16];
}security_cfg;

//Used to set wmm
/*
wmm_enable^ack_enable^ac_be^ac_bk^ac_vi^ac_vo^
cwmin_be^cwmax_be^aifs_be^txop_be^cwmin_bk^cwmax_bk^aifs_bk^txop_bk^
cwmin_vi^cwmax_vi^aifs_vi^txop_vi^cwmin_vo^cwmax_vo^aifs_vo^txop_vo^bss
*/

typedef struct wmm_cfg_s
{
	char vap_name[8];  /* interface name for this bss */
	int mode;
	int wmm_enable;
	int ack_enable;
	int ac_be;
	int ac_bk;
	int ac_vi;
	int ac_vo;
	int cwmin_be;
	int cwmax_be;
	int aifs_be;
	int txop_be;
	int cwmin_bk;
	int cwmax_bk;
	int aifs_bk;
	int txop_bk;
	int cwmin_vi;
	int cwmax_vi;
	int aifs_vi;
	int txop_vi;
	int cwmin_vo;
	int cwmax_vo;
	int aifs_vo;
	int txop_vo;
	int bss;
}wmm_cfg;

enum
{
    CLI_CIPHER_AUTO = 0,
    CLI_CIPHER_WEP,
    CLI_CIPHER_TKIP,
    CLI_CIPHER_AES,
    CLI_MAX_CIPHER
};


typedef struct _ACL_MAC{
    char       addr[32];
    int        flag; //Added by Mario Huang 
}ACL_MAC;

typedef struct _ACL_LIST{
    int       acl_num;
    ACL_MAC       mac[128];
}ACL_LIST;

typedef struct _SCAN_AP_STATE{
    char       ssid[64];
    char       bssid[32];
	char	   rssi_str[16];
	char	   wmode[8];
    int       rssi;
    int       channel;
    int       auth_mode;
    int       encry;
    int       network_type;
	int       flag;
}SCAN_AP_STATE;

typedef struct _SCAN_AP_LIST{
    int       ap_num;
    SCAN_AP_STATE        ap[256];
}SCAN_AP_LIST;

typedef struct _AP_INFO{
    char       ssid[64];
    char       bssid[32];
    char       rssi[32];
    int       auth_mode;
	int		  enc_type;
}AP_INFO;

typedef struct _DFS_CHAN_ELEM{
    int       chan_num;
	int		  chan_freq;
}DFS_CHAN_ELEM;

typedef struct _DFS_CHAN_LIST{
    int       dfs_num;
	DFS_CHAN_ELEM	chan_list[255];
}DFS_CHAN_LIST;

typedef struct _DFS_RADAR_LIST {
    char    ad_name[16];    			/* if name, e.g. "ath0" */
    unsigned short ad_id;
#define    ATH_DIAG_DYN    0x8000       /* allocate buffer in caller */
#define    ATH_DIAG_IN    0x4000        /* copy in parameters */
#define    ATH_DIAG_OUT    0x0000       /* copy out results (always) */
#define    ATH_DIAG_ID    0x0fff
    unsigned short ad_in_size;        	/* pack to fit, yech */
    void*    ad_in_data;
    void*    ad_out_data;
    unsigned int    ad_out_size;
}DFS_RADAR_LIST;

typedef struct _DFS_RADAR_HANDLER {
	int	s;
	DFS_RADAR_LIST atd;
}DFS_RADAR_HANDLER;

typedef struct _DFS_REQ_NOLELEM {
    unsigned short		nol_freq; 	        /* NOL channel frequency */
    unsigned short		nol_chwidth;
    unsigned long		nol_start_ticks; 	/* OS ticks when the NOL timer started */
    unsigned long       nol_timeout_ms; 	/* Nol timeout value in msec */
}DFS_REQ_NOLELEM;

typedef struct _DFS_REQ_NOLINFO {
    unsigned int	ic_nchans;
    DFS_REQ_NOLELEM dfs_nol[255];
}DFS_REQ_NOLINFO;


#if 0
enum vap_mode_t
{
    MODE_AP,
    MODE_WDS,
    MODE_APCLI,
};
#endif

/* wireless mode list */
enum wireless_mode_t
{
    WMODE_11BG = 0,
    WMODE_11B = 1,
    WMODE_11A = 2,
    WMODE_11ABG = 3,
    WMODE_11G = 4,
    WMODE_11ABGN = 5,
    WMODE_11N = 6,
    WMODE_11GN = 7,
    WMODE_11AN = 8,
    WMODE_11BGN = 9,
    WMODE_11AGN = 10,
    WMODE_11N5G = 11,
    WMODE_11AC_MIXED = 14,//11a/an/ac mixed 5g band only (Only 11ac chipset support)
};
/* ralink supported wireless mode id*/
/*
0:   legacy 11b/g mixed
1:   legacy 11b only
2:   legacy 11a only
3:   legacy 11a/b/g mixed
4:   legacy 11g only
5:   11abgn mixed
6:   11n only
7:   11gn mixed
8:   11an mixed
9:   11bgn mixed
10: 11agn mixed
11: 11n only in 5g band only
14: 11a/an/ac mixed 5g band only (Only 11ac chipset support)
*/
enum wireless_mode_id_t
{
    WMODE_11BG_ID = 0,
    WMODE_11B_ID = 1,
    WMODE_11A_ID = 2,
    WMODE_11ABG_ID = 3,
    WMODE_11G_ID = 4,
    WMODE_11ABGN_ID = 5,
    WMODE_11N_ID = 6,
    WMODE_11GN_ID = 7,
    WMODE_11AN_ID = 8,
    WMODE_11BGN_ID = 9,
    WMODE_11AGN_ID = 10,
    WMODE_11N5G_ID = 11,
    WMODE_11AC_MIXED_ID = 14,//11a/an/ac mixed 5g band only (Only 11ac chipset support)
};

//Added by Andy Yu in 2013/10/12: Get Ethernet Client List
typedef struct _ETHERNET_CLIENT_STATE{
    char       macaddr[20];
    char       ipaddr[64];
}ETHERNET_CLIENT_STATE;

typedef struct _ETHERNET_CLIENT_LIST{
    int       client_num;
    ETHERNET_CLIENT_STATE        client[128];
}ETHERNET_CLIENT_LIST;

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} date;

typedef union _MACHTTRANSMIT_SETTING {
    struct  {
        unsigned short  MCS:7;  // MCS
        unsigned short  BW:1;   //channel bandwidth 20MHz or 40 MHz
        unsigned short  ShortGI:1;
        unsigned short  STBC:2; //SPACE
        unsigned short  rsv:3;
        unsigned short  MODE:2; // Use definition MODE_xxx.
    } field;
    unsigned short      word;
} MACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
    unsigned char            ApIdx;
    unsigned char            Addr[6];
    unsigned char            Aid;
    unsigned char            Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
    unsigned char            MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
    char                     AvgRssi0;
    char                     AvgRssi1;
    char                     AvgRssi2;
    unsigned int             ConnectedTime;
// Tommy , add association Time ,2009/1/20 11:38
    //unsigned long         AssociationTime;    
    MACHTTRANSMIT_SETTING    TxRate;
// Patch EZP
    unsigned int        LastRxRate; 
    short        StreamSnr[3]; 
    short        SoundingRespSnr[3];
// Patch EZP
} RT_802_11_MAC_ENTRY;

typedef struct _RT_802_11_MAC_TABLE {
    unsigned long            Num;
    RT_802_11_MAC_ENTRY      Entry[32]; //MAX_LEN_OF_MAC_TABLE = 32 
} RT_802_11_MAC_TABLE;

//_RT1_802_11_MAC_ENTRY for 5g
typedef struct _RT1_802_11_MAC_ENTRY {
    unsigned char       ApIdx;
    unsigned char       Addr[6];
    unsigned char       Aid;
    unsigned char       Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
    unsigned char       MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
    char AvgRssi0;
    char AvgRssi1;
    char AvgRssi2;
    unsigned int ConnectedTime;
/* EZP add association Time */
    unsigned long       AssociationTime;
    MACHTTRANSMIT_SETTING TxRate;
//#ifdef RTMP_RBUS_SUPPORT
   unsigned int LastRxRate;
   short StreamSnr[3]; // BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed
   short SoundingRespSnr[3]; // SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed
   //SHORT TxPER; // TX PER over the last second. Percent
   //SHORT reserved;
//#endif // RTMP_RBUS_SUPPORT //
}RT1_802_11_MAC_ENTRY;
 
typedef struct _RT1_802_11_MAC_TABLE {
    unsigned long       Num;
    RT1_802_11_MAC_ENTRY Entry[32];
} RT1_802_11_MAC_TABLE;
typedef struct Channel_s
{
     int chan_number;
     int frequency;
     unsigned char hasUpper;
     unsigned char hasLower;
     unsigned char centerHT80;
     unsigned char isDFS;
}Channel_t;

typedef struct ChannelListInfo_s
{
    int num;
    int current_chan;
    Channel_t channel_list[MAX_CHANNEL_NUM];
}ChannelListInfo_t;

typedef struct StationInfo_s
{
    unsigned char Addr[MAC_ADDR_LENGTH];
    date ConnectedTime;
   	unsigned char ssid[32*5+1]; //add by frank for wp838
    int rssi;					//add by frank for wp838
}StationInfo_t;

typedef struct StationListInfo_s
{
    int sta_num;
    StationInfo_t sta_list[MAX_STA_NUM];
}StationListInfo_t;

int get_radio_status(int radio, int *radio_status);
int get_wlan_rate(int radio,int *rate);
int get_current_channel(int radio, Channel_t *channel_get);
int get_channel_list(int radio, ChannelListInfo_t *channel_list);
int get_sta_list(int radio, StationListInfo_t *sta_list_info);
/*General***********************/
int set_ap_isolation(int radio);
int set_ssid(int radio, int mode, int vap_id);
int set_security_ssid(int radio, int mode, int vap_id, security_cfg *ap_cfg);
int set_hidden_ssid(int radio, int mode, int vap_id);
int set_bss_isolation(int radio, int mode, int vap_id);
int set_dtim(int radio);
int set_power(int radio);
int set_beacon_interval(int radio);
/*Security***********************/
int set_ap_security(int radio, int vap_id);
int set_ap_security_dfs(int radio);
/**WMM ****************************/
int set_ap_wmm(int radio, int vap_id);
int set_ap_wmm_enable(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_cwmin(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_cwmax(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_aifs(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_txop(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_ac(int radio, int vap_id, wmm_cfg  *wmmcfg);
int set_ap_wmm_noack(int radio, int vap_id, wmm_cfg  *wmmcfg);


/*ACL********************************/
int set_acl(int radio, int vap_id);
int get_ap_list(int radio, SCAN_AP_LIST *ap_list);
int set_apcli_disconnect_ap(int radio);

int get_bandwidth(int radio, int *htbw);
int get_extchannel(int radio, Channel_t *extchannel_get);
int get_wirelessmode(int radio, int *mode);
int get_channel_select_mode(int radio, int *mode);
int get_ssid(int radio, int mode, int vap_id, char *ssid);
int get_dfschannel(DFS_CHAN_LIST *dfs_list);
int get_radarchannel(DFS_REQ_NOLINFO *dfs_list);
int get_cacstate(unsigned int *cac_state);
/********************Rogue AP********************/
int get_rogue_ap_list(SCAN_AP_LIST *ap_list);
#if defined(GP_WP688)
//int get_channel_select_mode(int radio, int *mode);
//int get_extchannel(int radio, Channel_t *extchannel_get);
//int get_ht_bandwidth(int radio);
int get_wirelessmode(int radio, int *mode);
//int get_ssid(int radio, int mode, int vap_id, char *ssid);
int set_channel(int radio);
int set_extchannel(int radio);
int set_ht_bandwidth(int radio);
int set_wirelessmode_channel(int radio);
int set_enable_ssid(int radio, int mode, int vap_id);
int set_disable_ssid(int radio, int mode, int vap_id);
#elif defined(GP_WP838)
int get_wifi_status(int radio, int *status);
int get_vap_status(int radio, int vapid, int mode, int *vap_status);
int get_max_txpower(int radio, int *max_txpower);
int get_chainmask_num(int radio, int *chain_num);
//int get_current_extchannel(int radio, Channel_t *ext_channel);
int set_ap_wirelessmode_channel(int radio);
#endif //End of #if defined(GP_WP688)

int get_sta_assoc_ssid(int radio, char *ssid);
int get_sta_assoc_bssid(int radio, char *bssid);
int get_sta_assoc_authmode(int radio, int *auth_mode, int *enc_type);
int get_sta_assoc_status(int radio, int *assoc);
int get_sta_assoc_ap_info(int radio, AP_INFO *ap_info);
int get_sta_assoc_rssi(int radio, char *rssi);
int get_sta_assoc_wmode(int radio, char *wmode);
int get_ethernet_client_list(int radio, ETHERNET_CLIENT_LIST *client_list);

#if defined(GP_WP688)
//int get_sta_association(int radio, int *assoc);
int set_config_sta(int radio);
int set_device_mode(int radio);
int config_apcli(int radio);
int set_apcli_basic(int radio);
#elif defined(GP_WP838)
int get_sta_assoc_secmode(int radio);
int get_sta_assoc_bssid(int radio, char *bssid);
int get_sta_assoc_rssi(int radio, char *rssi);
//int get_sta_assoc_status(int radio, int *assoc);
int config_sta(int radio);
int set_sta_basic(int radio);

#endif //End of #if defined(GP_WP688)

int CheckHexString(char *p);

#endif //End of __MDI_DETAIL_H_
