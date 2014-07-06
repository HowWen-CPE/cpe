#ifdef CONFIG_EZP_ARCH_RT305X
#define ARCH "RT305X"

#ifndef WL_NUM
#define WL_NUM 1
#endif

#ifndef WL_VIRTUAL_NUM
#define WL_VIRTUAL_NUM 4
#endif

#ifndef WL_WDS_NUM
#define WL_WDS_NUM 4
#endif

#ifndef WL_APCLI_NUM
#define WL_APCLI_NUM 1
#endif

#if ( WL_NUM == 1 )
#define WL_BASIC_RULE "1^9^100^6^1"
#define WL5G_BASIC_RULE "1^8^100^0^1"
#if ( WL_VIRTUAL_NUM == 1 ) /* WL0_IFNAME_RULE*/
#define WL0_IFNAME_RULE "rai0"
#define WL1_IFNAME_RULE "ra0"
#elif ( WL_VIRTUAL_NUM == 2 )
#define WL0_IFNAME_RULE "ra0|ra1"
#define WL1_IFNAME_RULE "rai0|rai1"
#elif ( WL_VIRTUAL_NUM == 4 )
#define WL1_IFNAME_RULE "ra0|ra1|ra2|ra3"
#define WL0_IFNAME_RULE "rai0|rai1|rai2|rai3"
#endif /* WL0_IFNAME_RULE*/
#define WL0_RULE_NUM WL_VIRTUAL_NUM
#define WL0_RULE_MAX WL_VIRTUAL_NUM
#if (WL0_RULE_NUM == 2)
#warning "WL0_RULE_NUM 2"
#warning "WL0_RULE_MAX 2"
#elif (WL0_RULE_NUM == 4)
#warning "WL0_RULE_NUM 4"
#warning "WL0_RULE_MAX 4"
#endif
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM
#elif ( WL_NUM == 2 )
#define WL_BASIC_RULE "1^9^100^6^1|1^9^100^6^1"
#define WL5G_BASIC_RULE "1^8^100^6^1|1^8^100^6^1"
#if ( WL_VIRTUAL_NUM == 1 ) /* WL0_IFNAME_RULE/WL1_IFNAME_RULE*/
#define WL0_IFNAME_RULE "rai0"
#define WL1_IFNAME_RULE "ra0"
#elif ( WL_VIRTUAL_NUM == 2 )
#define WL1_IFNAME_RULE "ra0|ra2"
#define WL0_IFNAME_RULE "rai0|rai2"
#elif ( WL_VIRTUAL_NUM == 4 )
#define WL0_IFNAME_RULE "rai0|rai1|rai2|rai3"
#define WL1_IFNAME_RULE "ra0|ra1|ra2|ra3"
#endif /* WL0_IFNAME_RULE/WL1_RULE_NUM */
#define WL0_RULE_NUM WL_VIRTUAL_NUM
#define WL0_RULE_MAX WL_VIRTUAL_NUM
#define WL1_RULE_NUM WL_VIRTUAL_NUM
#define WL1_RULE_MAX WL_VIRTUAL_NUM
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM
#elif (WL_NUM == 0 )
#else
#error "Undefined amount of WLAN interfaces"
#endif

#if ( WAN_NUM >= 1 )

/* 3G/4G WAN default interface should be ppp0. */
#if defined(EZP_PROD_CAT_M)
#define WAN0_IFNAME "vlan2"
#else
#define WAN0_IFNAME "vlan2"
#endif

#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan2"
#define WAN0_PORTS "4"
#if ( LAN_NUM >= 1 )
    #define LAN0_IFNAME "br0"
    #define LAN0_IFNAMES "vlan1 ra0 ra1 apcli0"
    #define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 apcli0 wds0 wds1 wds2 wds3"
    #define LAN0_HWNAME ""
    #define LAN0_DEVICE "vlan1"
#endif
#if (LAN_NUM >= 2)
    #define LAN1_IFNAME "br2"
    #define LAN1_DEVICE "br2"
#endif

#if (LAN_NUM == 1)
    #if (WL_VIRTUAL_NUM == 2)
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #elif (WL_VIRTUAL_NUM == 4) /* Added all wlan interfaces into bridge */
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 rai2 rai3 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #else
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #endif
    #define BR_RULE_NUM 2
    #define BR_RULE_MAX 8
#elif (LAN_NUM == 2)
    #define BR_RULE_GUESTLAN "GuestLAN^0^ra1^"
    #if (WL_VIRTUAL_NUM == 2)
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|"BR_RULE_GUESTLAN
    #elif (WL_VIRTUAL_NUM == 4) /* Added all wlan interfaces into bridge */
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 rai2 rai3 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|"BR_RULE_GUESTLAN
    #else
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|"BR_RULE_GUESTLAN
    #endif
    #define BR_RULE_NUM 3
    #define BR_RULE_MAX 8
#else
    #if (WL_VIRTUAL_NUM == 2)
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #elif (WL_VIRTUAL_NUM == 4) /* Added all wlan interfaces into bridge */
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 rai2 rai3 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #else
        #define BR_RULE "LAN1^1^vlan1 rai0 rai1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #endif
    #define BR_RULE_NUM 2
    #define BR_RULE_MAX 8
#endif

#define STATS_RULE "^1^60^vlan1 vlan2 ppp0 br0 rai0 usb0 eth0"
#define VLAN_RULE_NUM 2
#define VLAN_RULE_MAX 8
#ifdef CONFIG_RAETH_RT3052_P5
    #warning "P5 enabled, vlan set to 1111011"
    #define VLAN_RULE "LAN1^1^eth2^1^1111011^|WAN1^1^eth2^2^0000111^"
#else
    #warning "P5 disabled, vlan set to 1111001"
    #define VLAN_RULE "LAN1^1^eth2^1^1111011^|WAN1^1^eth2^2^0000111^"
#endif /* CONFIG_RAETH_RT3052_P5 : support for GMAC port when using external giga switch. */
#define VLANPORT_RULE_NUM 5
#define VLANPORT_RULE_MAX 7
#define VLANPORT_RULE "PORT1^0^1^0^011101|PORT2^1^1^0^011011|PORT3^2^1^0^010111|PORT4^3^1^0^001111|PORT5^4^2^0^000001|PORT6^5^1^1^|PORT7^6^0^1^"
/* EZP: Old VLAN definition should be Obsolete! */
#define LAN0_PORTS "0 1 2 3"
/* WAN0 uses vlan2. XXX: No effect! */
#define VLAN2_PORTS "4 5"
/* LAN0 uses vlan1. XXX: No effect! */
#define VLAN1_PORTS "0 1 2 3 5*"
#define VLAN_HWNAME "eth2"

#elif ( WAN_NUM == 2 ) 

#define WAN0_IFNAME "vlan2"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan2"
#define WAN0_PORTS "4"

/* 3G/4G 2nd WAN default interface should be ppp0. */
#if defined(EZP_PROD_CAT_N)
#define WAN1_IFNAME "ppp1"
#else
#define WAN1_IFNAME "vlan3"
#endif

#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "vlan3"
#define WAN1_PORTS "3"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan1 ra0 ra1 apcli0"
#define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 apcli0 wds0 wds1 wds2 wds3"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan1"
#define LAN0_PORTS "0 1 2"

#define VLAN_RULE_NUM 3
#define VLAN_RULE_MAX 16 
#ifdef CONFIG_RAETH_RT3052_P5
    #define VLAN_RULE "LAN1^1^eth2^1^1110011^|WAN1^1^eth2^2^0000111^|WAN2^1^eth2^3^0001011^"
#else
    #define VLAN_RULE "LAN1^1^eth2^1^1110001^|WAN1^1^eth2^2^0000101^|WAN2^1^eth2^3^0001001^"
#endif /* CONFIG_RAETH_RT3052_P5 : support for GMAC port when using external giga switch. */
#define VLANPORT_RULE_NUM 5
#define VLANPORT_RULE_MAX 7
#define VLANPORT_RULE "PORT1^0^1^0^001101|PORT2^1^1^0^001011|PORT3^2^1^0^000111|PORT4^3^2^0^000001|PORT5^4^3^0^000001|PORT6^5^1^1^|PORT7^6^0^1^"
/* WAN0 uses vlan2. */
#define VLAN2_PORTS "4 5"
/* WAN1 uses vlan3. */
#define VLAN3_PORTS "3 5"
/* LAN0 uses vlan1. */
#define VLAN1_PORTS "0 1 2 5"

#define VLAN_HWNAME "eth2"

#if (WL_VIRTUAL_NUM == 2)
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#elif (WL_VIRTUAL_NUM == 4) /* Added all wlan interfaces into bridge */
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 ra2 ra3 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#else
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#endif

#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#define BR_RULE_NUM 3
#define BR_RULE_MAX 16

#define STATS_RULE "^1^60^vlan3 vlan1 vlan2 ppp0 ppp1 br0 ra0 usb0 eth0 eth1"

#else

#error "Undefined amount of LAN/WAN interfaces"

#endif

#define HWNAT_RULE "1"

#define BW_TYPE \
        "A1M64K", "A1.5M640K", "A2M256K", "A2M512K", "A3M640K", "A4M1M", \
        "A6M256K", "A6M640K", "A8M256K", "A8M640K", "A8M800K", "A10M2M", \
        "A12M1M", "C1M128K", "C2M128K", "C2M384K", "C3M384K", "C6M640K", \
        "C8M640K", "C10M1M", "V2M2M", "V4M4M", "V10M10M", "V25M25M", \
        "V50M50M", "V100M100M","Custom"
#if ( EZP_PROD_SUBCAT == 0 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 1 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 2 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 3 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 4 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 5 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 6 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#endif /* EZP_PROD_SUBCAT */

#define NVRAM_EZPACKET_DEFAULT_RT305X_LAN_NUM_1   \
    /* WAN1 uses vlan1. */              \
    { "vlan1ports", VLAN1_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan1hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \

#define NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_1   \
    /* WAN1 uses vlan2. */              \
    { "vlan2ports", VLAN2_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan2hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \


#define NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_2   \
    /* WAN3 uses vlan3. */              \
    { "vlan2ports", VLAN3_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 port. */                \
    { "vlan3hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 device hwname. */       \



#endif /* CONFIG_EZP_ARCH_RT305X */

