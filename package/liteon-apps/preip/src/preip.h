#ifndef __PREIP_H__
#define __PREIP_H__
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netfilter.h>
#include <linux/netlink.h>

#define MACADDR_OCTETS  6
#define A_STATUS int
typedef	int		A_BOOL;
#define TRUE 1
#define FALSE 0

/* use this device to send frame*/
#define PREIP_NET_DEVICE_NAME "eth0"

/* 802.3 Frame */
typedef struct
{
#ifndef MACADDR_OCTETS
#define MACADDR_OCTETS    6
#endif
    u8 dst[MACADDR_OCTETS];
    u8 src[MACADDR_OCTETS];
    u16 typelen;
}Frame8023Head_t;
#define FRAME_8023_OCTETS sizeof(Frame8023Head_t)

/* SAP Frame */
typedef struct
{
    u8 dsap;
    u8 ssap;
    u8 cmd;
#define VENDORID_OCTETS 3
#define PREIP_PRIV_ID1 0x08
#define PREIP_PRIV_ID2 0x00
#define PREIP_PRIV_ID3 0x4E

    u8 vendorid[VENDORID_OCTETS];
    u16 protocol;
}FrameSAPHead_t;
#define FRAME_SAP_OCTETS sizeof(FrameSAPHead_t)

/* Pre-Ip Frame */
typedef struct
{
    u8 service[2];
    u8 port[4];
    u8 len[2];
    u8 id;
}FramePreIpHead_t;
#define FRAME_PREIP_OCTETS 9

#define CFG_MAX_PASSWORD		32
#define MAX_SYSNAME             32
#define MAX_SSID                32


/* Pre-IP IDs*/
enum preip_id
{
    PREIP_ID_DISCOVERY = 0x1,
    PREIP_ID_DISCOVERY_RESP = 0x2,
    PREIP_ID_SET_ITEM = 0x3,
    PREIP_ID_SET_ITEM_RESP = 0x4,
};

/* discovery response packet*/
typedef struct Preip_dscv_resp
{
    u8 mac[6];             /* device mac*/
    u8 deviceid[33];       /* device id*/
    u8 dhcp;               /* 0: disabled; 1: enabled. */
    u8 ip[4];              /* static ip address or dynamic ip from server*/
    u8 netmask;            /* netmask, bit number*/
    u8 essid[33];          /* essid */
    u8 lock_to_bssid[6];   /* lock to bssid */
    u8 country_code;       /* country code*/
    u8 channel_list[32];   /* supported channel list*/
    u8 rssithr_conn;       /* connect rssi threshold */
    u8 rssithr_disconn;    /* disconnect rssi threshold */
    u8 tx_power;           /* 0: full; -1: -1dB; ...; -9: -9dB; -10: min */
    u8 security[128];      /* security */
    u8 asso_status;        /* 0: Disassociated; 1: Associated*/
    u8 bssid[6];           /* associated bssid */
    u8 channel;            /* 36, 40, ..., 165*/
    u8 bandwidth;          /* 0: 20MHz; 1: 40MHz; 2: 80MHz; 3: 160MHz*/
    u8 ieee80211mode;      /* 0: 11bgn; 1: 11bg; 2:11b; 3:11an; 4:11a; 5: 11ac*/
    u8 rssi;               /* rssi */
    u8 rssi_per_chain[3];  /* rssi for ch0, ch1, ch2 (0 for 2x2 device)*/
} Preip_dscv_resp_t;


/* sub IDs of set item*/
enum preip_set_item_id 
{
    PREIP_ID_SET_APPLY_SAVE = 0x55,
    PREIP_ID_SET_DHCP = 0x61,
    PREIP_ID_SET_IP = 0x62,
    PREIP_ID_SET_MASK = 0x63,
    PREIP_ID_SET_ESSID = 0x64,
    PREIP_ID_SET_RSSITHR = 0x65,
    PREIP_ID_SET_DEVID = 0x66,
    PREIP_ID_SET_SECURITY = 0x67,
};

typedef struct Preip_set_item
{
    u8 mac[6];          /* mac match*/
    u8 item_id;         /* preip_set_item_id*/
    
    union set_item
    {
        u8 deviceid[33];
        u8 dhcp;
        u8 ip[4];
        u8 mask;
        u8 essid[33];
        u8 rssi_threshold[2];
        u8 security[128];
    }item;
} Preip_set_item_t;


/* request packet, don't include LLC header*/
typedef struct preip_dscv_req
{
    u8 mac[6];             /* mac match*/
    u8 reserved_data[64];  /* not used yet*/

}preip_dscv_req_t;


/* main structure for preip packet, don't include SAP header*/
typedef struct FramePreIpAll
{
    FramePreIpHead_t  preip_hdr;  /* preip header after SAP header*/

    union preip_data
    {
        preip_dscv_req_t req_data;     /* discover request data*/  
        Preip_dscv_resp_t resp_data;   /* discover response data*/
        Preip_set_item_t set_item_data; /* item could be set*/
    }preip_data_buf;
}FramePreIpAll_t;


typedef struct PreipFramInfo
{
    u8 from_mac[6];  /* request from this mac */
    u8 from_dev_name[10]; /* record the device name received the request */
    FramePreIpAll_t preip_all; /* packet content w/o SAP*/
}PreipFramInfo_t;


#define IFACE_NAME_LAN			"br0"

#define PREIP_MSG_SIZE (600)
#define PREIP_PARAM_MSG_SIZE (600)	

#define PREIP_MAX_UNITS	1
#define PREIP_PKTQ_MAX_COUNT 4
#define PREIP_PKT_MAX_LEN 1518
#define PREIP_PKT_MIN_LEN (/*FRAME_8023_OCTETS+*/FRAME_SAP_OCTETS+FRAME_PREIP_OCTETS)
#define PREIP_RESET_CMD_ERROR 0x0060
#define PREIP_RESET_CMD_OK 0x0000


/*preip netlink information*/
#define NETLINK_PREIP		21		/*netlink privacy for preip*/
struct sock *preip_nlfd = NULL;
struct nl_preip_info {
    unsigned int		nli_type;   		/* preip netlink type*/ 
#define NLI_TYPE_HELLO	1
#define NLI_TYPE_RUN	2
    char nli_buf[512];			/*message buffer*/
};

/* RSSI threshold limit*/
#define CONN_RSSI_MAX -45
#define CONN_RSSI_MIN -85
#define DISCONN_RSSI_MAX -55
#define DiSCONN_RSSI_MIN -95
#define CONN_DISCONN_RSSI_MIN_DELTA 10

#define NAME_OCTETS 256
#define ESSID_OCTETS 256
#define HTTPS_OCTETS 128

/* for parameter get */
#define PREIP_PASSWORD 1
#define PREIP_LOCALMAC 2
#define PREIP_NAME 3
#define PREIP_ESSID 4
#define PREIP_IP 5
#define PREIP_MASK 6
#define PREIP_TYPE 7
#define PREIP_HTTPS 8
#define PREIP_PRODUCTNAME 9

/* for Pre-IP Data */
#define PREIP_RESET ('r')
#define PREIP_MANUAL ('m')
#define PREIP_AUTO ('a')
#define PREIP_DSAP 0xAA
#define PREIP_SSAP 0xAA
#define PREIP_SAPCMD 0x03
#define PREIP_SAPPROTOCOL 0x0006
#define PREIP_SERVICE (htons(0x0003))
#define PREIP_PORT (htons(0x0004))
#define PREIP_RSSIPORT 0x00000001


//#define FRAME_PREIP_GET(f, p) ((p)=(FramePreIpHead_t*)((u32)(f)+FRAME_SAP_OCTETS))

#define TYPE_PTR long

#define FRAME_SRCMAC_GET(f, m) (memcpy((m), ((u8*)(f)+MACADDR_OCTETS), MACADDR_OCTETS))


#define FRAME_PREIP_GET(f, p) ((p)=(FramePreIpHead_t*)((TYPE_PTR)(f)+FRAME_SAP_OCTETS))
#define PREIP_DATA_GET(f, p) ((p)=(u8*)((TYPE_PTR)(f)+FRAME_PREIP_OCTETS))
#define PREIP_PORT_GET(f, p) (memcpy((u8*)(&p),((FramePreIpHead_t*)(f))->port, 4))
#define PREIP_CMD_GET(f, c) ((c)=((FramePreIpHead_t*)(f))->id)

#define PREIP_SERVICE_SET(f, p) (memcpy(((FramePreIpHead_t*)(f))->service, (u8*)(&p), 2))
#define PREIP_PORT_SET(f, p) (memcpy(((FramePreIpHead_t*)(f))->port, (u8*)(&p), 4))
#define PREIP_LEN_SET(f, p) (memcpy(((FramePreIpHead_t*)(f))->len, (u8*)(&p), 2))

#define RESP_TYPE_SET(f, p) (memcpy(((FrameDiscoveryResp*)(f))->type, (u8*)(&p), 2))
#define RESP_IP_SET(f, p) (memcpy(((FrameDiscoveryResp*)(f))->ip, (u8*)(&p), 4))
#define RESP_MASK_SET(f, p) (memcpy(((FrameDiscoveryResp*)(f))->mask, (u8*)(&p), 4))
//#endif

#endif	// __PREIP_H__


