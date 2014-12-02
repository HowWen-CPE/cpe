/*****************************************************************************
  File Name     : preip_host.c
  Description   : preip layer 2 control process at user space.
******************************************************************************/


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

//#define PRE_IP_DEBUG
//#define DUMP_PACKET_CONTENT

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#include "preip.h"

pid_t getpid();


typedef int T_INT32;

#ifdef PRE_IP_DEBUG

static void hex_dump(void *buf, int len)
{
    int i;

    for (i = 0 ; i < len; i++)
    {
	printf("%02x", ((u8*)buf)[i]);
	if (i%2)
	    printf(" ");
	if (15 == i%16)
	    printf("\n");
    }
    printf("\n");
}

#endif

	
/*gcc-3.4.4-2.16.1 no define of NLMSG_HDRLEN*/
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))

/*user call kernel to setup vlan bridge filter by netlink socket*/
T_INT32 preip_send(int skfd,int type,char *buf, int len)
{
    
    struct sockaddr_nl src_local;
    struct sockaddr_nl dest_kernel;
    struct nlmsghdr *nlh = NULL;

    int errorNo = 0;

#ifdef PRE_IP_DEBUG	
    printf("preipSend: type=%d, len=%d\n", type, len);
#endif

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));
	
    if(nlh == NULL)
    {
        errorNo = 1;
        printf("%s,nlh malloc error\n",__func__);
        goto malloc_error;
    }
	
    memset(nlh, 0, sizeof(struct nlmsghdr));

    memset(&src_local, 0, sizeof(struct sockaddr_nl));
    src_local.nl_family = AF_NETLINK;     
    src_local.nl_pid = getpid(); 
    //src_local.nl_pid = pthread_self() << 16 | getpid();
    
    src_local.nl_groups = 0;  /* not in mcast groups */

    if(bind(skfd, (struct sockaddr *)&src_local,sizeof(struct sockaddr_nl)) != 0)
    {
        errorNo = 2;
        perror("preipSend, bind:");
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
    //src_local.nl_pid = pthread_self() << 16 | getpid();
    
    ((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_type = type;

    if(type != NLI_TYPE_HELLO)
        memcpy(((struct  nl_preip_info*)NLMSG_DATA(nlh))->nli_buf,buf,len);

    sendto(skfd,(char*)nlh,nlh->nlmsg_len,0,(struct sockaddr*)&dest_kernel,sizeof(dest_kernel));

    free(nlh);
    return 0;
error_return:
    free(nlh);
malloc_error:
    printf("%s return error, error no:%d\n",__func__,errorNo);
    return -1;
}


T_INT32 preip_waiting(int skfd,char *buf, int len)
{
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr* rcvNlh;
    fd_set rfds;
    int errorNo = 0;
    int ret = 0;
    
#ifdef PRE_IP_DEBUG
    printf("preipWaiting: entry\n");
#endif

    FD_ZERO(&rfds);
    FD_SET(skfd, &rfds);
	
    memset(&dest_kernel, 0, sizeof(struct sockaddr_nl));
    dest_kernel.nl_family = AF_NETLINK;
    dest_kernel.nl_pid = 0;   /* For Linux Kernel */
    dest_kernel.nl_groups = 0; 
    rcvNlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nl_preip_info)));

    dest_kernel_len = sizeof(dest_kernel);
    if (select(skfd+1, &rfds, NULL, NULL, NULL) > 0){
        if ((ret=recvfrom(skfd, rcvNlh,NLMSG_LENGTH(sizeof(struct nl_preip_info)),0,(struct sockaddr*)&dest_kernel,&dest_kernel_len)) > 0){

#ifdef PRE_IP_DEBUG
            printf("%s,%d: recvfrom ret=%d\n", __func__, __LINE__, ret);
#endif
            if(len > ret)
            {
                printf("%s,%d: recvfrom length error %d < %d\n", __func__, __LINE__, len, ret);
            }

            if(((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_type == NLI_TYPE_RUN)
            {
                memcpy(buf,((struct  nl_preip_info*)NLMSG_DATA(rcvNlh))->nli_buf,len);
                goto normal_return; /*have ack response*/
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
    
#ifdef PRE_IP_DEBUG
    printf("%s return ok\n",__func__);
#endif

    free(rcvNlh);
    return ret;
}



int preip_discovery_func(u8 *dst_mac)
{
    int readlen = 0;
    int skfd;
    FramePreIpAll_t preip_buf, preip_recv_buf;
    Preip_dscv_resp_t *p_resp;
    u32 tmp32;
    u16 tmp16;
    char *str;

#ifdef PRE_IP_DEBUG
    printf("preipd start...\n");
#endif

    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PREIP);
    if(skfd < 0){
        printf("%s: sock error\n", __func__);
        return 0;
    }

    if(preip_send(skfd, NLI_TYPE_HELLO, NULL, 0) <0 )
    {
        printf("%s: netlink NLI_TYPE_HELLO error\n", __func__);

        return -1;
    }

    /* build discovery buf*/
    memset((char *)&preip_buf, 0x0, sizeof(preip_buf));
    tmp16 = PREIP_SERVICE;
    memcpy(&preip_buf.preip_hdr.service, (u8 *)(&tmp16), 2);
    
    tmp32 = PREIP_PORT;
    memcpy(&preip_buf.preip_hdr.port, (u8 *)(&tmp32), sizeof(preip_buf.preip_hdr.port));
    preip_buf.preip_hdr.len[0]=sizeof(preip_buf)%256;
    preip_buf.preip_hdr.len[1]=sizeof(preip_buf)/256;
    
    preip_buf.preip_hdr.id = PREIP_ID_DISCOVERY;

    /* dst mac address*/
    memcpy(preip_buf.preip_data_buf.req_data.mac, dst_mac, 6);

    /* copy anything to reserved data, just for test*/
    strcpy((char *)&preip_buf.preip_data_buf.req_data.reserved_data, "!@#$%^&*");

    /* send it to driver module*/
    preip_send(skfd, NLI_TYPE_RUN, (char *)&preip_buf, sizeof(preip_buf));
 
#if 1
    /* wait for response*/
    memset((u8 *)&preip_recv_buf, 0x0, sizeof(preip_recv_buf));

    while(1){
        readlen = preip_waiting(skfd,(char *)&preip_recv_buf, sizeof(preip_recv_buf));

#ifdef PRE_IP_DEBUG
        printf("%s: read length:%d \n",__func__, readlen);
#endif

        if (readlen < 0)
            continue;

        /* check length*/
        /*
            if(readlen < sizeof(preip_recv_buf))
            {
                printf("%s: length isn't ok for discovery response: %d < %d \n",__func__, readlen, sizeof(preip_recv_buf));
            }
            */


        /* show get result*/
        p_resp = &preip_recv_buf.preip_data_buf.resp_data;

#ifdef PRE_IP_DEBUG
        /*
        printf("service=%2.2x%2.2x\n", preip_recv_buf.preip_hdr.service[0], preip_recv_buf.preip_hdr.service[1]);
        printf("port=%2.2x%2.2x%2.2x%2.2x\n", preip_recv_buf.preip_hdr.port[0], preip_recv_buf.preip_hdr.port[1],
            preip_recv_buf.preip_hdr.port[2], preip_recv_buf.preip_hdr.port[3]);
        printf("len=%2.2x%2.2x\n", preip_recv_buf.preip_hdr.len[0], preip_recv_buf.preip_hdr.len[1]);
        printf("id=%2.2x\n", preip_recv_buf.preip_hdr.id);
        */
#endif
        if(preip_recv_buf.preip_hdr.id == PREIP_ID_DISCOVERY_RESP)
        {
            preip_wifi_security_t *preip_sec = (preip_wifi_security_t *)p_resp->security;
            
            printf("Discovery Reponse\n");
            printf("============================\n");
            /* bellow is the setting*/
            printf("MAC: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
                p_resp->mac[0], p_resp->mac[1],p_resp->mac[2],
                p_resp->mac[3],p_resp->mac[4],p_resp->mac[5]);
            printf("Device ID: %s\n", p_resp->deviceid);
            printf("SSID: %s\n", p_resp->essid);
            printf("DHCP: %s\n", p_resp->dhcp ? "enabled":"disabled");
            printf("IP Address: %d.%d.%d.%d\n", p_resp->ip[0],p_resp->ip[1],p_resp->ip[2], p_resp->ip[3]);
            printf("NetMask: %d\n", p_resp->netmask);
            printf("Connect Rssi Threshold: %d\n", (char)p_resp->rssithr_conn);
            printf("Disconnect Rssi Threshold: %d\n", (char)p_resp->rssithr_disconn);    

            /* show security*/
            switch(preip_sec->authmode)
            {
            case PREIP_WIFI_SEC_AUTH_MODE_OPEN:
                printf("Security: open\n");
                break;
            case PREIP_WIFI_SEC_AUTH_MODE_WPAPSK:
                printf("Security: wpa-psk %s\n",preip_sec->wifi_sec.sec_psk.key);
                break;
            case PREIP_WIFI_SEC_AUTH_MODE_WPA2PSK:
                printf("Security: wpa2-psk %s\n",preip_sec->wifi_sec.sec_psk.key);
                break;
            case PREIP_WIFI_SEC_AUTH_MODE_WPA:
            case PREIP_WIFI_SEC_AUTH_MODE_WPA2:
                printf("Security: %s %s %s %s\n",
                    preip_sec->authmode == PREIP_WIFI_SEC_AUTH_MODE_WPA?"wpa":"wpa2", 
                    (preip_sec->wifi_sec.sec_wpa.authtype==PREIP_WIFI_SEC_AUTH_MODE_PEAP)?"peap":"ttls",
                    (char *)preip_sec->wifi_sec.sec_wpa.user,
                    (char *)preip_sec->wifi_sec.sec_wpa.password);
                break;
            case PREIP_WIFI_SEC_AUTH_MODE_WEP:
                printf("Security: wep\n");
                break;
            default:
                printf("Unsupported security authmode %d\n", preip_sec->authmode);
                break;
            }

            /* bellow is status*/
            printf("Connect Status: %s\n", p_resp->asso_status ? "Associated":"Disassociated");

            printf("BSSID: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
                p_resp->bssid[0], p_resp->bssid[1],p_resp->bssid[2],
                p_resp->bssid[3],p_resp->bssid[4],p_resp->bssid[5]);

            printf("SSID: %d\n", p_resp->channel);
            
            printf("Band width: %s\n", p_resp->bandwidth==0 ? "20MHz" : ((p_resp->bandwidth==1)?"40MHz":"80MHz"));

            switch(p_resp->ieee80211mode)
            {
            case 0:
                str = "802.11bgn";
                break;
            case 1:
                str = "802.11bg";
                break;
            case 2:
                str = "802.11b";
                break;
            case 3:
                str = "802.11an";
                break;
            case 4:
                str = "802.11a";
                break;
            case 5:
                str = "802.11ac";
            default:
                str = "802.11an";
                break;
            }
            printf("Wireless Mode: %s\n", str);

            
            if(p_resp->asso_status == 1)
            {
                printf("RSSI: %d dBm\n", (char)p_resp->rssi);
                
                printf("RSSI per chain: %d/%d dBm\n", (char)p_resp->rssi_per_chain[0],(char)p_resp->rssi_per_chain[1]);

            }else
            {
                printf("RSSI=--\n");
                
                printf("RSSI per chain=-- --\n");
            }
        }else
        {
            printf("Recv not discovery response packet!\n");
            continue;
        }
        
        printf("\n");
        
#ifdef DUMP_PACKET_CONTENT
        hex_dump((u8 *)&preip_recv_buf, 100);
#endif
        break;
    }
#endif
    return 0;

}

int preip_set_func(u8 *dst_mac, u8 id, u8 *val_buf)
{
    char readBuf[PREIP_MSG_SIZE];

    FramePreIpAll_t preip_all;
    Preip_set_item_t *p_set_item;
    
    int skfd, ret, readlen=0;

    memset((char *)&preip_all, 0x0, sizeof(preip_all));

    p_set_item = &preip_all.preip_data_buf.set_item_data;

    /* cmd type is set */
    preip_all.preip_hdr.id = PREIP_ID_SET_ITEM;

    /* set dst_mac */
    memcpy(p_set_item->mac, dst_mac, 6);

    
    switch(id)
    {
    case PREIP_ID_SET_DEVID:
        strncpy((char *)p_set_item->item.deviceid, (char *)val_buf, 32);
        break;
    case PREIP_ID_SET_DHCP:
        p_set_item->item.dhcp = *val_buf;
        break;    
    case PREIP_ID_SET_IP:
        memcpy(p_set_item->item.ip, val_buf, 4);
        break;
    case PREIP_ID_SET_MASK:
        p_set_item->item.mask = *val_buf;
        break;
    case PREIP_ID_SET_ESSID:
        strncpy((char *)p_set_item->item.essid, (char *)val_buf, 32);
        break;
    case PREIP_ID_SET_RSSITHR:
        p_set_item->item.rssi_threshold[0] = val_buf[0];
        p_set_item->item.rssi_threshold[1] = val_buf[1];
        break;
    case PREIP_ID_SET_SECURITY:
        memcpy(p_set_item->item.security, val_buf, PREIP_WIFI_SEC_LEN);
        break;
    case PREIP_ID_SET_APPLY_SAVE:
        break;
    default:
        printf("%s: unknown set command %d\n", __func__, id);
        return -1;
    }

    p_set_item->item_id = id;

#ifdef PRE_IP_DEBUG
    printf("preip_set_func, id=%d\n", id);
#endif
	
    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PREIP);
    if(skfd < 0){
        printf("%s,sock error\n",__func__);
        return 0;
    }    
    preip_send(skfd, NLI_TYPE_HELLO, NULL, 0);

    preip_send(skfd, NLI_TYPE_RUN, (char *)&preip_all, sizeof(preip_all));

    sleep(1);

    return 0;

#if 0
    /* wait for response*/
    memset(readBuf, 0x0, sizeof(readBuf));
    while(1){
        readlen = preip_waiting(skfd,readBuf, sizeof(readBuf));

#ifdef PRE_IP_DEBUG
        printf("preipd: read length:%d \n",readlen);
#endif

    	 if (readlen < 0)
    	     continue;

        /* TODO: wait for response*/


#ifdef PRE_IP_DEBUG
        printf("set done\n",readlen);
#endif

        break;
    }
#endif

}

 /*
 * parse the mac  and store into *addr.
 * the format is HH:HH:HH:HH:HH:HH
 */
 #define MAC_ADDR_LEN 17
int inet_atonmac(const char *s, char *addr, int addr_len)
{

    char Tmac[] = "HH:HH:HH:HH:HH:HH";
    unsigned char mac_addr[18];
    //char *tmp = mac_addr;
    char c;
    char *ptr = (char *)s;
    int val = 0;
    int i, j;
    int allzero = 1;
    //int all0xff = 1;

    memset(mac_addr,0,sizeof(mac_addr));

    if(addr_len != 17)
        return -1;

    i = j = 0;
    while(1){
        c = *ptr++;
        if ( Tmac[i] == 'H' ){
            
            /* must be hex char. */
            val = val * 16;
            if ( isdigit(c) ){
                val += c - '0';
                mac_addr[i] = c;
            }else if(c >= 'a' && c <= 'f'){
                val += c - 'a' + 10;
                mac_addr[i] = c;
            }else if(c >= 'A' && c <= 'F'){
                val += c - 'a' + 10;
                mac_addr[i] = c;
            }else
                return -1;
        } else{
            /* here should be : */
            if(j == 0){
                int x;
                
                //donn't distinguish globle or local mac
                x = val & 0x01;
                if(x != 0)
                    return -1;
            }
            
            if(val != 0){
                allzero = 0;
            }

            val = 0;
            j++;
            if(j == 6){
                if ((c != '\0') || allzero){
                    return -1;
                }

                memcpy(addr,mac_addr,addr_len);
                return 0;
            }
            if(c != ':'){
                return 0;
            }else{
                mac_addr[i] = c;
            }
        }
        ++i;
    }   

	return 0;
}

/*****************************************************************************
 Prototype    : is_hex_digital_str
 Description  : Check if the string is hex digital format
 Input        : T_CHAR8 *str  
 Output       : None
 Return Value : TRUE/FALSE
*****************************************************************************/
int is_hex_digital_str(char *str)
{
    int i;
    
    for(i=0; i<strlen(str); i++)
    {
        if((str[i] >= '0' && str[i] <='9') || (str[i] >= 'a' && str[i] <='f') || (str[i] >= 'A' && str[i] <='F'))
        {
            continue;
        }else{
            return FALSE;
        }
    }

    return TRUE;
}

/***********************************************************************
 * Function Name : IsValidIpAddress
 * Description    : check if the ip address is legal
 * Input         : IpAddr, ip address
 * Output        :
 * Return value  : TRUE, valid ip address
 *                 FALSE, illegal ip address
 ***********************************************************************/
int IsValidIpAddress(char *IpAddr)
{
	T_INT32 ii,kk;

	for (ii = 24; ii >= 0; ii -= 8)
	{
		/* Skip any leading stuff (e.g., spaces, '[') */
		while (IpAddr != '\0' && !isdigit(*IpAddr))
			IpAddr++;
		kk =(T_INT32)atoi(IpAddr);
		if (kk < 0 || 255 < kk)
			return FALSE;
		if ( ((IpAddr = strchr(IpAddr, '.')) == NULL) && (ii > 0) )
			return FALSE;
		IpAddr++;
	}
	return TRUE;
}


/*****************************************************************************
 Prototype    : preip_cli_parse_security
 Description  : Parse security option from command line to preip control
                packet
 Input        : int argc                          
                char *argv[]                      
                preip_wifi_security_t *preip_sec  
 Output       : None
 Return Value : 0: OK; -1: fail.
*****************************************************************************/
int preip_cli_parse_security(int argc, char *argv[], preip_wifi_security_t *preip_sec)
{
    memset((char *)preip_sec, 0, sizeof(preip_wifi_security_t));

    if (strcmp(argv[4], "open")==0)
    {
        if(argc != 5)
        {
            printf("Invalid input. Too many args for security open\n");
            goto security_usage;
        }

        preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_OPEN;
    }
    else if(strcmp(argv[4], "wpa-psk")==0 || strcmp(argv[4], "wpa2-psk")==0)
    {
        int len_psk;
        
        if(argc > 6)
        {
            printf("Invalid input. Too many args for security %s\n", argv[4]);
            goto security_usage;
        }else if (argc < 6)
        {
            printf("Invalid input. Too less args for security %s\n", argv[4]);
            goto security_usage;
        }
        
        /* pre-shared key*/
        len_psk=strlen(argv[5]);
        if(len_psk < 8 || len_psk > 64){
    		printf("Invalid input. The length of pre-shared key should be from 8 to 64\n");

            return -1;
        }

        /* should be hex digital*/
        if(len_psk == 64 && is_hex_digital_str(argv[5])!= TRUE){
    		printf("Invalid input. The pre-shared key should be hex when length is 64 bytes\n");

            return -1;
        }

        if(strcmp(argv[4], "wpa-psk")==0)
        {
            preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPAPSK; /* wpa-psk*/
        }else
        {
            preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA2PSK; /* wpa2-psk*/
        }

        strcpy((char *)preip_sec->wifi_sec.sec_psk.key, argv[5]); /* passphrase*/

    }
    else if(strcmp(argv[4], "wpa")==0 || strcmp(argv[4], "wpa2")==0)
    {
        if(argc != 8)
        {
            printf("Invalid input.\n");
            goto security_usage;
        }

        if(!(strcmp(argv[5], "peap")==0 || strcmp(argv[5], "ttls")==0))
        {
            printf("Invalid input.\n");
            
            goto security_usage;
        }

        if((strlen(argv[6]) > 64)) /* username*/
        {
            printf("Invalid input. The length user name should no more than 64\n");

            return -1;
        }

        if((strlen(argv[7]) > 64)) /* password*/
        {
            printf("Invalid input. The length password should no more than 64\n");

            return -1;
        }

        if(strcmp(argv[4], "wpa")==0) /* wpa or wpa2*/
        {
            preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA;
        }else
        {
            preip_sec->authmode = PREIP_WIFI_SEC_AUTH_MODE_WPA2;
        }

        if(strcmp(argv[5], "peap")==0) /* peap or ttls*/
        {
            preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_PEAP;

        }else
        {
            preip_sec->wifi_sec.sec_wpa.authtype = PREIP_WIFI_SEC_AUTH_MODE_TTLS;
        }

        strcpy((char *)(&preip_sec->wifi_sec.sec_wpa.user), argv[6]);      /* user name*/
        strcpy((char *)(&preip_sec->wifi_sec.sec_wpa.password), argv[7]);  /* password*/
    }
    else if (strcmp(argv[4], "wep") == 0){
        printf("WEP isn't supported at command line\n");
        goto security_usage;
    }else{
        goto security_usage;
    }

    return 0;
security_usage:
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security open\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security wpa-psk/wpa2-psk passphrase\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security wpa/wpa2 ttls/peap username password\n");
    return -1;
}


void usage(void)
{
    printf("Usage:\n");
    printf("    cpe_ctrl get\n");
    printf("    cpe_ctrl get xx:xx:xx:xx:xx:xx\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx deviceid xxxxxx\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx dhcp enable/disable\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx ip x.x.x.x\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx netmask xx\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx ssid xxxxx\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx rssithr xxx yyy\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security open\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security wpa-psk/wpa2-psk passphrase\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx security wpa/wpa2 ttls/peap username password\n");
    printf("    cpe_ctrl set xx:xx:xx:xx:xx:xx apply\n");

    return;
}

int main(int argc, char *argv[])
{
    u8 dst_addr[6]={0xff,0xff,0xff,0xff,0xff,0xff};

    if(argc > 1)
    {
        if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            usage();
            return 0;
        }
        else if(!strcmp(argv[1], "get") || !strcmp(argv[1], "g"))
        {
            /* get*/

            if(argc == 3)
            {
                /* argv[2] should be MAC address*/
                
                if(strlen(argv[2]) != 17)
                {
                    printf("Invalid MAC address. Should be as xx:xx:xx:xx:xx:xx\n");
                    usage();

                    return -1;
                }

                /* check if valid mac*/
                if (inet_atonmac(argv[2], (char *)dst_addr, MAC_ADDR_LEN) < 0) {
                    printf("Invalid MAC address. Should be as xx:xx:xx:xx:xx:xx\n");
                    return -1;
                }
            }else
            {
                printf("Invalid input: too many args!\n");
                usage();
                return -1;
            }

            /* discovery process*/
            preip_discovery_func(dst_addr);
            
            return 0;
        }
        else if(!strcmp(argv[1], "set") || !strcmp(argv[1], "s"))
        {
            /* set*/

            if(argc < 3)
            {
                printf("Invalid input: too less args!\n");
                usage();
            }
            
            /* argv[2] should be MAC address*/
            if(strlen(argv[2]) != 17)
            {
                printf("Invalid MAC address. Should be as xx:xx:xx:xx:xx:xx\n");
                usage();

                return -1;
            }

            /* check if valid mac*/
            if (inet_atonmac(argv[2], (char *)dst_addr, MAC_ADDR_LEN) < 0) {
                printf("Invalid MAC address. Should be as xx:xx:xx:xx:xx:xx\n");
                usage();
                return -1;
            }

            if(argc == 4)
            {
                if(!strcmp(argv[3], "apply") || !strcmp(argv[3], "app"))
                {
                    /* apply and save*/
                    preip_set_func(dst_addr, PREIP_ID_SET_APPLY_SAVE, NULL);
                }else
                {
                    printf("Invalid input!\n");
                    usage();

                    return -1;
                }
            }else if(argc > 4)
            {
                /* set item */

                if(!strcmp(argv[3], "deviceid") || !strcmp(argv[3], "dev"))
                {

                    if(strlen(argv[4]) > 1 && strlen(argv[4]) < 33 )
                    {
                        preip_set_func(dst_addr, PREIP_ID_SET_DEVID, (u8 *)argv[4]);
                    }else
                    {
                        printf("Invalid input. SSID length is from 1 to 32\n");

                        return -1;
                    }
                }
                else if(!strcmp(argv[3], "dhcp"))
                {
                    u8 dhcp;
                    
                    if(!strcmp(argv[4], "enabled") || !strcmp(argv[4], "en") || !strcmp(argv[4], "enable"))
                    {
                        dhcp = 1;
                    }
                    else if(!strcmp(argv[4], "disabled") || !strcmp(argv[4], "dis") || !strcmp(argv[4], "disable"))
                    {
                        dhcp = 0;
                    }
                    else
                    {
                        printf("Invalid input. Usage: \n"
                               "    cpe_ctrl set xx:xx:xx:xx:xx:xx dhcp enable/disable\n"
                               "    e.g. cpe_ctrl set 00:03:74:00:00:10 dhcp disable\n");

                        return -1;
                    }

                    preip_set_func(dst_addr, PREIP_ID_SET_DHCP, &dhcp);
                    
                }
                else if(!strcmp(argv[3], "ip"))
                {
                    int ip[4];
                    u8 u8_ip[4];
                    
                    if(IsValidIpAddress(argv[4]))
                    {
                        sscanf(argv[4], "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
                        
                        u8_ip[0]=(u8)ip[0];
                        u8_ip[1]=(u8)ip[1];
                        u8_ip[2]=(u8)ip[2];
                        u8_ip[3]=(u8)ip[3];
                        
                        preip_set_func(dst_addr, PREIP_ID_SET_IP, u8_ip);
                        
                    }else
                    {
                        printf("Invalid input. Usage: \n"
                               "    cpe_ctrl set xx:xx:xx:xx:xx:xx ip yy.yy.yy.yy\n"
                               "    e.g. cpe_ctrl set 00:03:74:00:00:10 ip 192.168.1.2\n");

                        return -1;
                    }
                }
                else if(!strcmp(argv[3], "netmask"))
                {
                    u8 netmask;

                    netmask = atoi(argv[4]);

                    if(strlen(argv[4])>=1 && strlen(argv[4])<=2 && netmask > 1 && netmask < 31)
                    {
                        preip_set_func(dst_addr, PREIP_ID_SET_MASK, &netmask);

                    }else
                    {
                        printf("Invalid input. Usage: \n"
                               "    cpe_ctrl set xx:xx:xx:xx:xx:xx netmask yy.yy.yy.yy\n"
                               "    e.g. cpe_ctrl set 00:03:74:00:00:10 netmask 24\n");

                        return -1;
                    }
                }
                else if(!strcmp(argv[3], "ssid"))
                {
                    if(strlen(argv[4]) > 1 && strlen(argv[4]) < 33 )
                    {
                        preip_set_func(dst_addr, PREIP_ID_SET_ESSID, (u8 *)argv[4]);
                    }else
                    {
                        printf("Invalid input. SSID lenght is from 1 to 32\n");

                        return -1;
                    }
                }
                else if(!strcmp(argv[3], "rssithr") || !strcmp(argv[3], "rssi"))
                {
                    char rssithr[2], conn_rssi, disconn_rssi;
                        
                    if(argc == 6)
                    {
                        conn_rssi = atoi(argv[4]);
                    	disconn_rssi = atoi(argv[5]);
              
                        /* check value*/  
                    	if (conn_rssi > CONN_RSSI_MAX || conn_rssi < CONN_RSSI_MIN) {
                    		printf("Connrssithr must be from %d to %d\n", CONN_RSSI_MAX, CONN_RSSI_MIN);
                    		return -1;
                    	}

                    	if (disconn_rssi > DISCONN_RSSI_MAX || disconn_rssi < DiSCONN_RSSI_MIN) {
                    		printf("Disconnrssi must be from %d to %d\n", DISCONN_RSSI_MAX, DiSCONN_RSSI_MIN);
                    		return -1;
                    	}

                    	if ((conn_rssi - disconn_rssi) < CONN_DISCONN_RSSI_MIN_DELTA) {
                    		printf("Connrssithr must be greater than Disonnrssithr %d\n", CONN_DISCONN_RSSI_MIN_DELTA);
                    		return -1;
                    	}
                    }
                    else
                    {
                        printf("Invalid input. Usage: \n"
                               "    cpe_ctrl set xx:xx:xx:xx:xx:xx rssithr connrssi disconnrssi\n"
                               "    e.g. cpe_ctrl set 00:03:74:00:00:10 rssithr -85 -95\n"
                               "    range of connrssi: -45 to -85\n"
                               "    range of disconnrssi: -55 to -95\n"
                               "    connrssi - disconnrssi >= 10\n");
                        return -1;
                    }
                    
                    rssithr[0] = conn_rssi;
                    rssithr[1] = disconn_rssi;
                    
                    preip_set_func(dst_addr, PREIP_ID_SET_RSSITHR, (u8 *)rssithr);
                }
                else if(!strcmp(argv[3], "security") || !strcmp(argv[3], "sec"))
                {
                    preip_wifi_security_t preip_sec;

                    if(preip_cli_parse_security(argc, argv, &preip_sec) < 0)
                    {
                        return -1;
                    }
                    
                    preip_set_func(dst_addr, PREIP_ID_SET_SECURITY, (u8 *)&preip_sec);
                }else
                {
                    printf("Invalid input.\n");
                    usage();
                }
            }

            return 0;
        }
        else
        {
            printf("Invalid input: unknown args!\n");
            usage();
            return -1;
        }
    }
    else
    {
        /* if no args, just do discovery*/
    
        /* discovery process*/
        preip_discovery_func(dst_addr);
    }

    return 0;
}

