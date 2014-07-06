//
// GLOBAL.H
//
#ifndef __GLOBAL_H
#define __GLOBAL_H

class CUpgraderApp;
extern CUpgraderApp theApp;
//#define GetAppPath()	theApp.m_pszAppPath

#define MAXDELAY     30000		/* maximum delay in retry loop (ms)*/
#define T_RRQ        htons(1)	/* TFTP read file */
#define T_WRQ	     htons(2)	/* TFTP write file */
#define T_DATA	     htons(3)	/* TFTP data message */
#define T_ACK        htons(4)	/* TFTP ack message */
#define T_ERROR      htons(5)	/* TFTP error message */
#define TFTP_PORT    htons(69)  /* TFTP port */
#define E_FILENAME   htons(1)   /* sending the wrong file to server */
#define E_PASSWORD   htons(2)   /* error password*/
#define E_UPDATEDATA htons(4)   /* error during updating flash */
#define NTRACE       0

#define TIMER1 254
#define TIMER2 253
#define TIMER_OVERTIME 252		// 等待回应包的超时时间
#define TIMER4 251

#define UDAP_DISCOVER_INTERVAL 500
#define UDAP_DISCOVER_NUM 4
#define UDAP_TYPE_IPCONFIGURE 0xC001
#define UDP_UDAP_PORT 0x5168
#define UAP_FLAG_REQUEST 0x01
#define UAP_CLASS_UCP 0x00010001
#define UAP_CLIENT_OPTION 0x0002	//客户（控制）端选项 David 2010.6.3 Add

#define UCP_METHOD_DISCOVER		0x0001
#define UCP_METHOD_GET_IP		0x0002
#define UCP_METHOD_SET_IP		0x0003
#define UCP_METHOD_RESET		0x0004
#define UCP_METHOD_AUTH			0x0005
#define UCP_METHOD_GET_WLAN		0x0006
#define UCP_METHOD_SET_WLAN		0x0007
#define UCP_METHOD_GET_WIZARD	0x0008
#define UCP_METHOD_SET_WIZARD	0x0009
#define UCP_METHOD_GET_INFO		0x000A
#define UCP_METHOD_REBOOT		0x000B
#define UCP_METHOD_GET_HOST     0x000E
#define UCP_METHOD_SET_HOST     0x000F

#define WEP_HEX					0x00
#define WEP_ASCII				0x01

#define UCP_CODE_AUTH_NAME		0x01
#define UCP_CODE_DEVICE_NAME	0x02
#define UCP_CODE_DEVICE_TYPE	0x03
#define UCP_CODE_USE_DHCP		0x04
#define UCP_CODE_IP_ADDR		0x05
#define UCP_CODE_SUBNET_MASK	0x06
#define UCP_CODE_GATEWAY_ADDR	0x07
#define UCP_CODE_SOFTWARE_VER	0x08
#define UCP_CODE_AUTH_PASSWORD	0x09
#define UCP_CODE_SYSTEM_NAME	0x0A
#define UCP_CODE_SSID			0x10
#define UCP_CODE_CHANNEL		0x11
#define UCP_CODE_SEC_METHOD		0x12
#define UCP_CODE_ENCRYPTION		0x13
#define UCP_CODE_WEPKEY64		0x14
#define UCP_CODE_WEPKEY128		0x15
#define UCP_CODE_WEPKEY152		0x16
#define UCP_CODE_WPA_PSK		0x17
#define UCP_CODE_OPMODE			0x18
#define UCP_CODE_WIRELESS_MODE	0x19
#define UCP_CODE_SUPER_G		0x1A
#define UCP_CODE_WLAN_MACADDR	0x1B
#define UCP_CODE_RADIUS_SERVER	0x1C
//#define UCP_CODE_RADIUS_PORT	0x1D
#define UCP_CODE_BRIDGE         0x1D
#define UCP_CODE_RADIUS_SECRET	0x1E
#define UCP_CODE_WEP_KEY_TYPE	0x1F
#define UCP_CODE_ENCRYPT_ONOFF	0x20
#define UCP_CODE_WEP_InputType	0x21
#define UCP_CODE_HOST_NAME      0x22
#define UCP_CODE_DOMAIN_NAME    0x23
#define UCP_CODE_ASSOCIATED    	0x24
#define UCP_CODE_BSSID    		0x25
#define UCP_CODE_RSSI    		0x26
//Added By Andy Yu: 5G Info
#define UCP_CODE_5GWIRELESS_MODE	0x27
#define UCP_CODE_5GWLAN_MACADDR	0x28
#define UCP_CODE_5GSSID			0x29
#define UCP_CODE_5GRSSI    		0x2A
#define UCP_CODE_5GBSSID    	0x2B
#define UCP_CODE_5GASSOCIATED   0x2C
#define UCP_CODE_5GCHANNEL		0x2D
#define UCP_CODE_5GSEC_METHOD	0x2E
#define UCP_CODE_5GENCRYPT_ONOFF	0x2F
#define UCP_CODE_2GRADIO_ONOFF	0x30
#define UCP_CODE_5GRADIO_ONOFF	0x31
#define UCP_CODE_WLAN_OPMODE	0x32
#define UCP_CODE_5GWLAN_OPMODE	0x33
#define UCP_CODE_HTTP_TYPE      0x34

#define UCP_CODE_END			0xFF

typedef enum
{
	PAGE_LOGIN = 0,
	PAGE_IP_CFG,
	PAGE_SYS_DECNAME,
	PAGE_SYS,
	PAGE_SYS_AP,
	PAGE_SYS_CLIENT,
	PAGE_SYS_CLIENT_INF,
	PAGE_SYS_CLIENT_ADHOC,
	PAGE_SYS_REPEATER,
	PAGE_SEC,
	PAGE_SEC_REPEATER,
	PAGE_SEC_WEP,
	PAGE_SEC_WPA_PSK,
	PAGE_SEC_WPA
}PAGE_NAME;

typedef enum
{
	DISABLED = 0,
	WEP64,
	WEP128,
	WEP152,
	WPA_PSK,
	WPA
}SECUTRITY_MODE;

typedef enum
{
	W_802_11B= 0,
	W_802_11G,
	W_802_11Static_G,
	W_802_11SUPER_G
	
}WIRELESS_MODE;

typedef enum
{
	AP_MODE = 0,
	CLIENT_MODE,
	REAPTER_MODE
}OP_MODE;

typedef enum
{
	SEL_AP_MODE = 0,
	SEL_INF,
	SEL_ADHOC,
	SEL_REPEATER
}SINGLE_OP_MODE;

typedef enum
{
	AUTH_OPEN_SYSTEM = 0,
	AUTH_SHARED_KEY,
	AUTH_WPA,
	AUTH_WPA2,
	AUTH_WPA2MIX,
	AUTH_WPAPSK,
	AUTH_WPA2PSK,
	AUTH_WPA2MIXPSK,
	AUTH_AUTO  //scout add
	
}AUTH_MODE;

typedef enum
{
	OFF = 0,
	ON
}ONOFF_MODE;

typedef enum
{
	CFG_OP_MODE_NORMAL = 0,
	CFG_OP_MODE_AP,
	CFG_OP_MODE_WISP0,
	CFG_OP_MODE_STA0,
	CFG_OP_MODE_WDSSTA0,
	CFG_OP_MODE_WISP1,
	CFG_OP_MODE_STA1,
	CFG_OP_MODE_WDSSTA1,
	CFG_OP_MODE_UR,
	CFG_OP_MODE_REPEATER,
	CFG_OP_MODE_ADHOC
}REAL_OP_MODE;

typedef enum
{
	INFRASTRUCTURE = 0,
	CLIENT_AD_HOC
}RADIO_CLIENT_MODE;

typedef enum
{
	TKIP = 0,
	AES
}ENCRYPTION_MODE;

typedef enum
{
	TYPE_HEX = 0,
	TYPE_ASCII,
	TYPE_WPAPSK,
	TYPE_WPA
}KEY_TYPE;

typedef enum {
    ENCRYPTION_DISABLED,
    ENCRYPTION_WEP,
    ENCRYPTION_AES_OCB,
    ENCRYPTION_AES_CCM,
    ENCRYPTION_CKIP, // No MMH used
    ENCRYPTION_TKIP, // implies Michael
    ENCRYPTION_AUTO,
    ENCRYPTION_CKIP_MMH
} WLAN_ENCRYPTION_ALG;

#define MODE_SELECT_11BG		0x00
#define MODE_SELECT_11B			0x01
#define MODE_SELECT_11A			0x02
#define MODE_SELECT_11ABG		0x03
#define MODE_SELECT_11G			0x04
#define MODE_SELECT_11ABGN		0x05
#define MODE_SELECT_11N			0x06
#define MODE_SELECT_11GN		0x07
#define MODE_SELECT_11AN		0x08
#define MODE_SELECT_11BGN		0x09
#define MODE_SELECT_11AGN		0x0A
#define MODE_SELECT_11N5G		0x0B
#define MODE_SELECT_11AC		0x0E
#define MODE_SELECT_108G		0x0F

/* extra mode bits for dynamic turbo prime */
#define MODE_SELECT_TPA   0x20
#define MODE_SELECT_TPG   0x40
#define MODE_SELECT_AUTO  0x80

#define MODE_SELECT_2GHZ  (MODE_SELECT_11B | MODE_SELECT_11G | MODE_SELECT_108G)
#define MODE_SELECT_ALL   (MODE_SELECT_11A | MODE_SELECT_2GHZ | MODE_SELECT_AUTO)
#define ABOLT_SUPERG    (ABOLT_WME_ELE | ABOLT_BURST | ABOLT_FAST_FRAME | ABOLT_COMPRESSION | ABOLT_TURBO_G)


typedef enum {
    ABOLT_TURBO_G     = 0x01,      /* Legacy Turbo G */
    ABOLT_TURBO_PRIME = 0x02,      /* Turbo Prime */
    ABOLT_COMPRESSION = 0x04,      /* Compression */
    ABOLT_FAST_FRAME  = 0x08,      /* Fast Frames */
    ABOLT_BURST       = 0x10,      /* Bursting */
    ABOLT_WME_ELE     = 0x20,      /* WME based cwmin/max/burst tuning */
    ABOLT_FRIENDLY_TURBO = 0x80,  /* Friendly turbo that switches out based on adjacent non-turbo traffic */
    ABOLT_XR          = 0x40,      /* XR */
    ABOLT_ATH_ADVCAP  = (
                          ABOLT_TURBO_PRIME |
                          ABOLT_COMPRESSION |
                          ABOLT_FAST_FRAME  |
                          ABOLT_FRIENDLY_TURBO
                        ),         /* Capabilties causes negotiation through AdvCap */
} ABOLT_OPTION;

// Functions
HCURSOR BeginWait();
void EndWait(HCURSOR hOldCur);
void ErrorMsg(UINT uMsgID, LPCTSTR lpszArg = NULL);
void GetCurrentPath(LPTSTR Path);
#endif // __GLOBAL_H