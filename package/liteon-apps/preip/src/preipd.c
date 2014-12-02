/*
 *	preip daemon
 *
 *	Authors:
 *	simon Jiang		<simon.jiang@liteon.com>
 *	create Date:		2006.12.06
 */

/*PRE IP Daemon*/
#include <sys/types.h>		
#include <sys/socket.h>	
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <linux/in.h>	
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "ostypes.h"
//#include "sysconf.h"   /* add by peter, 2008.7.30*/
//#include "b_adminConf.h"
//#include "wlanconf.h"

#define PRE_IP_DEBUG

#define PREIP_DAEMON_INCLUDE
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#include "preip.h"

typedef int T_INT32;

/*
 * get device name from config for dual radio or single radio device.
 * add by Tony 2008.2.29 -->
 */
//#define DUAL_RADIO (get_wifi_status() == ON24G_ON5G)
//#define sysname(syscfg) (DUAL_RADIO ? (syscfg)->cDualRadioDevName : (syscfg)->cSglRadioDevName)
/* <-- add by Tony 2008.2.29 */

/*get ethernet mac address*/
static void GetMacAddress(char* ifacename,char *pMacAddr)
{
    struct ifreq ifr;
    
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return;
    memset(&ifr, 0, sizeof(ifr));  
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFHWADDR,&ifr)>=0)
    {		       
    	memcpy(pMacAddr,ifr.ifr_hwaddr.sa_data,6);	
    }
    close(s);		
}

/*get IP address and IP net mask*/
static void GetIp_ipMask(char* ifacename,u32* ipaddr, u32* ipmask)
{
    struct ifreq ifr;
    struct sockaddr_in *pskin;
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return;
	
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFADDR,&ifr)>=0)
    {	
        pskin = (struct sockaddr_in*)&ifr.ifr_addr;
    	 *ipaddr =(pskin->sin_addr.s_addr );	
    }
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifacename);
    if(ioctl(s,SIOCGIFNETMASK,&ifr)>=0)
    {	
        pskin = (struct sockaddr_in*)&ifr.ifr_addr;
    	*ipmask = (pskin->sin_addr.s_addr );	
    }

#ifdef PRE_IP_DEBUG
   printf("preipd: get ip:%08x  get ip net mask:%08x\n",*ipaddr,*ipmask);
#endif

    close(s);		
}


/* set IP address */
void setIpAddress (char *ifrname, u32 addr)
{
   struct ifreq ifr;
   struct sockaddr_in *pskin;
   int fd,err=0;
   u32 bdcst=0;

   if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
     return ;
   memset(&ifr, 0, sizeof(ifr));
   strcpy(ifr.ifr_name, ifrname);
   pskin = (struct sockaddr_in*)&ifr.ifr_addr;

#ifdef PRE_IP_DEBUG
   printf("preipd: set ip address:%08x\n",addr);
#endif
   
   pskin->sin_family = AF_INET;
   pskin->sin_addr.s_addr = htonl(addr);
   if((err = ioctl(fd,SIOCSIFADDR,&ifr)) < 0)
   {
     close(fd);
     return ;
   }

   /**
     If Ip Address is set to zero,
     no need to set netmask and broadcast address
     **/
   if(addr == 0)
   {
     close(fd);
     return;
   }
	   
}

/* set IP net mask*/
void setIpMask (char *ifrname, u32 mask)
{
   struct ifreq ifr;
   struct sockaddr_in *pskin;
   int fd,err=0;
   u32 bdcst=0;

   if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
     return ;
   memset(&ifr, 0, sizeof(ifr));
   strcpy(ifr.ifr_name, ifrname);
   pskin = (struct sockaddr_in*)&ifr.ifr_addr;

#ifdef PRE_IP_DEBUG
   printf("preipd: set ip net mask:%08x\n",mask);
#endif
   
   pskin->sin_family = AF_INET;
   pskin->sin_addr.s_addr = htonl(mask);
   if((err = ioctl(fd,SIOCSIFNETMASK,&ifr)) < 0)
   {
     close(fd);
     return;
   }

}


int preip_get_mac(u8 *mac)
{
    struct ifreq ifr;
    
    int s;
    int ok = 0;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1) 
        return;
    memset(&ifr, 0, sizeof(ifr)); 
    
    strcpy(ifr.ifr_name, "eth0");
    if(ioctl(s,SIOCGIFHWADDR,&ifr)>=0)
    {		       
    	memcpy(mac,ifr.ifr_hwaddr.sa_data,6);	
    }
    close(s);
    
    return 0;
}

int preip_get_deviceid(u8 *deviceid)
{

    strcpy(deviceid, "testid123456789");
    
    return 0;
}

int preip_get_dhcp(u8 *dhcp)
{

    *dhcp = 0;
    
    return 0;
}

int preip_get_ip(u8 *ip)
{
    
    ip[0] = 192;
    ip[1] = 168;
    ip[2] = 1;
    ip[3] = 10;
    return 0;
}

int preip_get_netmask(u8 *netmask)
{

    *netmask = 24;
    
    return 0;
}

int preip_get_essid(u8 *essid)
{

    strcpy(essid, "abcd1234");
    
    return 0;
}

int preip_get_rssithr(u8 *rssithr_conn, u8 *rssithr_disconn)
{

    *rssithr_conn = (u8)-85;
    *rssithr_disconn = (u8)-95;
    
    return 0;
}

int preip_get_asso_status(u8 *asso_status)
{

    *asso_status = 1;
    
    return 0;
}

int preip_get_rssi(u8 *rssi)
{

    *rssi = (u8)-56;
    return 0;
}

int preip_get_rssi_per_chain(u8 *rssi_per_chain)
{
    rssi_per_chain[0]=(u8)-60;
    rssi_per_chain[1]=(u8)-61;
    rssi_per_chain[2]=0;
    return 0;
}

int preip_get_security(u8 *security)
{
    strcpy(security, "secxxxxx");
    return 0;
}

int preip_set_deviceid(u8 *deviceid)
{
    printf("preip_set_deviceid: %s\n", deviceid);

    return 0;
}

int preip_set_dhcp(u8 dhcp)
{
    printf("preip_set_dhcp: %s\n", dhcp ? "enabled" : "disabled");

    return 0;
}

int preip_set_ip(u8 *ip)
{
    
    printf("preip_set_ip: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    
    return 0;
}

int preip_set_netmask(u8 netmask)
{

    printf("preip_set_netmask: %d\n", netmask);
    
    return 0;
}

int preip_set_essid(u8 *essid)
{
    printf("preip_set_essid: %s\n", essid);

    return 0;
}

int preip_set_rssithr(u8 rssithr_conn, u8 rssithr_disconn)
{
    printf("preip_set_rssithr: %d %d\n", (char)rssithr_conn, (char)rssithr_disconn);
    
    return 0;
}

int preip_set_security(u8 *security)
{
    printf("preip_set_security\n");
    
    preip_wifi_security_t *preip_sec = (preip_wifi_security_t *)security;

    switch(preip_sec->authmode)
    {
    case PREIP_WIFI_SEC_AUTH_MODE_OPEN:
        printf("open\n");
        break;
    case PREIP_WIFI_SEC_AUTH_MODE_WPAPSK:
#ifdef PRE_IP_DEBUG
        printf("wpa-psk: %s\n",preip_sec->wifi_sec.sec_psk.key);
#endif
        break;
    case PREIP_WIFI_SEC_AUTH_MODE_WPA2PSK:
#ifdef PRE_IP_DEBUG
        printf("wpa2-psk: %s\n",preip_sec->wifi_sec.sec_psk.key);
#endif
        break;
    case PREIP_WIFI_SEC_AUTH_MODE_WPA:
#ifdef PRE_IP_DEBUG
        printf("wpa:\n    authtype:%s\n", (preip_sec->wifi_sec.sec_wpa.authtype==PREIP_WIFI_SEC_AUTH_MODE_PEAP)?"peap":"ttls");
        printf("    user:%s\n", (char *)preip_sec->wifi_sec.sec_wpa.user);
        printf("    password:%s\n", (char *)preip_sec->wifi_sec.sec_wpa.password);
#endif
        break;   
    case PREIP_WIFI_SEC_AUTH_MODE_WPA2:
#ifdef PRE_IP_DEBUG
        printf("wpa2:\n    authtype:%s\n", (preip_sec->wifi_sec.sec_wpa.authtype==PREIP_WIFI_SEC_AUTH_MODE_PEAP)?"peap":"ttls");
        printf("    user:%s\n", (char *)preip_sec->wifi_sec.sec_wpa.user);
        printf("    password:%s\n", (char *)preip_sec->wifi_sec.sec_wpa.password);
#endif
        break;
    case PREIP_WIFI_SEC_AUTH_MODE_WEP:
    default:
        printf("Unsupported security authmode %d\n", preip_sec->authmode);
        break;
    }

    return 0;
}

void preip_set_apply_save(void)
{
    printf("preip_set_apply_save\n");

    return 0;
}

/*daemon init function*/
static int Daemon_Init(void)
{
  int devnullfd;
  chdir ("/");
  switch (fork() )
  {
    case -1:
      printf("Daemon_Init: error in fork\r\n");
      exit(1);
    case 0: /** Child **/
      break;
    default:
      /** Parent **/
      exit(0);
  }
  if ( setsid() == -1 )
  {
    printf("Daemon_Init: setsid error\r\n");   
    exit(1);
  }


  devnullfd = open("/dev/null", O_RDWR);
  if ( devnullfd == -1 )
  {
    printf("Null device open failed\r\n"); 
    return(1);
  }
  close(0);
  close(1);
  close(2);

  dup2(devnullfd, 0);
  dup2(devnullfd, 1);
  dup2(devnullfd, 2);
  close(devnullfd);
  return(0);
}

	
/*gcc-3.4.4-2.16.1 no define of NLMSG_HDRLEN*/
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))

/*user call kernel to setup vlan bridge filter by netlink socket*/
T_INT32 preipSend(int skfd,int type,char *buf, int len)
{
    
    struct sockaddr_nl src_local;
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr *nlh = NULL;
    struct nlmsghdr rcvNlh;
    fd_set rfds;
    struct timeval tv;
    int errorNo = 0;
	
    printf("preipSend: type=%d, len=%d\n", type, len);
    
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));
	
    if(nlh == NULL){
	 errorNo = 1;
        printf("%s,nlh malloc error\n",__func__);
        goto malloc_error;
    }
	
    memset(nlh, 0, sizeof(struct nlmsghdr));

    memset(&src_local, 0, sizeof(struct sockaddr_nl));
    src_local.nl_family = AF_NETLINK;     
    src_local.nl_pid = getpid(); 
    src_local.nl_groups = 0;  /* not in mcast groups */
    if(bind(skfd, (struct sockaddr *)&src_local,sizeof(struct sockaddr_nl)) != 0){
	 errorNo = 2;
        printf("%s,bind error\n",__func__);
        goto error_return;
    }

    memset(&dest_kernel, 0, sizeof(struct sockaddr_nl));
    dest_kernel.nl_family = AF_NETLINK;
    dest_kernel.nl_pid = 0;   /* For Linux Kernel */
    dest_kernel.nl_groups = 0; 

	
    /* Fill the netlink message header */
    //nlh->nlmsg_flags = NLM_F_ACK; 	/*request kernel response*/
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct  nl_preip_info));
    nlh->nlmsg_pid = getpid();  /* self pid */
    ((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_type = type;
    if(type != NLI_TYPE_HELLO)
        memcpy(((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_buf,buf,len);

    sendto(skfd,(char*)nlh,nlh->nlmsg_len,0,(struct sockaddr*)&dest_kernel,sizeof(dest_kernel));
normal_return:
    printf("%s return ok\n",__func__);
    free(nlh);
    return 0;
error_return:
    free(nlh);
malloc_error:
    printf("%s return error, error no:%d\n",__func__,errorNo);
    return -1;
}


T_INT32 preipWaiting(int skfd,char *buf, int len)
{
    
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr* rcvNlh;
    fd_set rfds;
    int errorNo = 0;
    int ret;

    printf("preipWaiting: entry\n");
  
    FD_ZERO(&rfds);
    FD_SET(skfd, &rfds);
	
    memset(&dest_kernel, 0, sizeof(struct sockaddr_nl));
    dest_kernel.nl_family = AF_NETLINK;
    dest_kernel.nl_pid = 0;   /* For Linux Kernel */
    dest_kernel.nl_groups = 0; 
    rcvNlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));

    dest_kernel_len = sizeof(dest_kernel);
    if (select(skfd+1, &rfds, NULL, NULL, NULL) > 0){
        printf("%s,%d: select\n", __func__, __LINE__);
        if ((ret=recvfrom(skfd, rcvNlh,NLMSG_LENGTH(sizeof(struct nl_preip_info)),0,(struct sockaddr*)&dest_kernel,&dest_kernel_len)) > 0){
            printf("%s,%d: recvfrom ret=%d\n", __func__, __LINE__, ret);
             if(((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_type == NLI_TYPE_RUN){
                 printf("%s,%d: len=%d\n", __func__, __LINE__, len);
                 memcpy(buf,((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_buf,len);
        	  goto normal_return;/*have ack response*/
    	    }else{
    	        errorNo = 1;
    	    }
        }else{
            errorNo = 2;
        }
    }else
        errorNo = 3;
            	
    printf("%s return error, error no:%d\n",__func__,errorNo);
    free(rcvNlh);
    return -1;
normal_return:
    printf("%s return ok\n",__func__);
    free(rcvNlh);
    return ret;
}

#if 0
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
#endif


int preip_process_discovery(int skfd, PreipFramInfo_t *frame_info)
{
    int ret;
    Preip_dscv_resp_t *p_resp = &frame_info->preip_all.preip_data_buf.resp_data;

    frame_info->preip_all.preip_hdr.id = PREIP_ID_DISCOVERY_RESP;

    preip_get_mac(p_resp->mac);
    preip_get_deviceid(p_resp->deviceid);
    preip_get_dhcp(&p_resp->dhcp);
    preip_get_ip(p_resp->ip);
    preip_get_netmask(&p_resp->netmask);
    preip_get_essid(p_resp->essid);
    preip_get_rssithr(&p_resp->rssithr_conn, &p_resp->rssithr_disconn);
    preip_get_asso_status(&p_resp->asso_status);
    preip_get_rssi(&p_resp->rssi);
    preip_get_rssi_per_chain(p_resp->rssi_per_chain);
    preip_get_security(p_resp->security);

    ret = preipSend(skfd, NLI_TYPE_RUN, (char *)frame_info, sizeof(PreipFramInfo_t));

    if(ret < 0)
    {
        printf("preipSend error\n");

        return -1;
    }

    return 0;
}


int preip_process_set(PreipFramInfo_t *frame_info)
{
    Preip_set_item_t *p_item;

    p_item = &frame_info->preip_all.preip_data_buf.set_item_data;

    switch(p_item->item_id)
    {
    case PREIP_ID_SET_DHCP:
        preip_set_dhcp(p_item->item.dhcp);
        break;
    case PREIP_ID_SET_IP:
        preip_set_ip(p_item->item.ip);
        break;
    case PREIP_ID_SET_MASK:
        preip_set_netmask(p_item->item.mask);
        break;
    case PREIP_ID_SET_ESSID:
        preip_set_essid(p_item->item.essid);
        break;
    case PREIP_ID_SET_RSSITHR:
        preip_set_rssithr(p_item->item.rssi_threshold[0],p_item->item.rssi_threshold[1]);
        break;
    case PREIP_ID_SET_DEVID:
        preip_set_deviceid(p_item->item.deviceid);
        break;
    case PREIP_ID_SET_SECURITY:
        preip_set_security(p_item->item.security);
        break;
    case PREIP_ID_SET_APPLY_SAVE:
        preip_set_apply_save();
        break;
    default:
        printf("unsupported set item id %x\n", p_item->item_id);
        break;
    }

    return 0;
}

/*preip daemon main function*/
int main(int argc, char *argv[])
{
    int readlen = 0;
    char readBuf[PREIP_MSG_SIZE];
    char writeBuf[PREIP_MSG_SIZE];
    char ifname[16];
    char cmd[150];  /* enlarge the length of cmd and prevent stack overflow! bug1275 by roger 2008-5-14*/
    int skfd;
    PreipFramInfo_t frame_info;

#ifdef PRE_IP_DEBUG
    printf("preipd start...\n");
    sleep(1);
#endif
    //Daemon_Init();	
    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PREIP);
    if(skfd < 0){
        printf("%s,sock error\n",__func__);
        return 0;
    }
    preipSend(skfd,NLI_TYPE_HELLO,NULL,0);

	
    while(1)
    {
        memset((u8 *)&frame_info, 0x0, sizeof(frame_info));

        readlen = preipWaiting(skfd,(char *)&frame_info, sizeof(frame_info));

#ifdef PRE_IP_DEBUG
        printf("preipd: read length:%d \n",readlen);
#endif

        if (readlen < 0)
            continue;

    	if (readlen < sizeof(frame_info))
        {
            printf("length < sizeof(frame_info)\n");
            continue;
        }
         
        switch(frame_info.preip_all.preip_hdr.id)
        {
        case PREIP_ID_DISCOVERY:
            printf("discovery request\n");
            preip_process_discovery(skfd, &frame_info);
            break;
        case PREIP_ID_SET_ITEM:
            printf("set item\n");
            preip_process_set(&frame_info);
            break;
        default:
            break;
        }
    
    }

    return 0;
}

