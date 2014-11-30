
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

#define PRE_IP_DEBUG

//#define PREIP_DAEMON_INCLUDE
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#include "preip.h"

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
    socklen_t dest_kernel_len;
    struct nlmsghdr *nlh = NULL;
    struct nlmsghdr rcvNlh;
    fd_set rfds;
    struct timeval tv;
    int errorNo = 0;
	
    printf("preipSend: type=%d, len=%d\n", type, len);
    
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


T_INT32 preip_waiting(int skfd,char *buf, int len)
{
    
    struct sockaddr_nl dest_kernel;
    socklen_t dest_kernel_len;
    struct nlmsghdr* rcvNlh;
    fd_set rfds;
    int errorNo = 0;
    int ret = 0;

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
        //printf("%s,%d: select\n", __func__, __LINE__);
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
    strcpy(&preip_buf.preip_data_buf.req_data.reserved_data, "!@#$%^&*");

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
            printf("Discovery Reponse\n");

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

            /* TODO: show security*/
            printf("Security: %s\n", "XXX TODO XXXX"); 
            
            
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
        

#ifdef PRE_IP_DEBUG
        hex_dump((u8 *)&preip_recv_buf, 100);
        printf("get done\n");
#endif

        break;
    }
#endif

}

int preip_set_func(u8 *dst_mac, u8 id, u8 *val_buf)
{
    char readBuf[PREIP_MSG_SIZE];
    char ifname[16];
    char cmd[150];
    pid_t pid;
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
        strncpy(p_set_item->item.deviceid, val_buf, 32);
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
        strncpy(p_set_item->item.essid, val_buf, 32);
        break;
    case PREIP_ID_SET_RSSITHR:
        p_set_item->item.rssi_threshold[0] = val_buf[0];
        p_set_item->item.rssi_threshold[1] = val_buf[1];
        break;
    case PREIP_ID_SET_SECURITY:
        /* TODO: set p_set_item->item.security */
        
        break;
    case PREIP_ID_SET_APPLY_SAVE:
        break;
    default:
        printf("%s: unknown set command %d\n", __func__, id);
        return -1;
    }

    p_set_item->item_id = id;

#ifdef PRE_IP_DEBUG
    printf("preip_set_func\n");

#endif
	
    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PREIP);
    if(skfd < 0){
        printf("%s,sock error\n",__func__);
        return 0;
    }    
    preip_send(skfd, NLI_TYPE_HELLO, NULL, 0);


    
    preip_send(skfd, NLI_TYPE_RUN, (char *)&preip_all, sizeof(preip_all));

#if 1
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
    char *tmp = mac_addr;
    char c;
    char *ptr = (char *)s;
    int val = 0;
    int i, j;
    int allzero = 1;
    int all0xff = 1;

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
        printf("argv[1]=%s\n", argv[1]);

        if(!strcmp(argv[1], "get") || !strcmp(argv[1], "g"))
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
                if (inet_atonmac(argv[2], dst_addr, MAC_ADDR_LEN) < 0) {
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
            if (inet_atonmac(argv[2], dst_addr, MAC_ADDR_LEN) < 0) {
                printf("Invalid MAC address. Should be as xx:xx:xx:xx:xx:xx\n");
                usage();
                return -1;
            }

            if(argc == 4)
            {
                if(!strcmp(argv[3], "apply") || !strcmp(argv[3], "app"))
                {
                    /* apply and save*/

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
                    preip_set_func(dst_addr, PREIP_ID_SET_DEVID, argv[4]);
                }
                else if(!strcmp(argv[3], "dhcp"))
                {
                    preip_set_func(dst_addr, PREIP_ID_SET_DHCP, argv[4]);
                }
                else if(!strcmp(argv[3], "ip"))
                {
                    preip_set_func(dst_addr, PREIP_ID_SET_IP, argv[4]);
                }
                else if(!strcmp(argv[3], "netmask"))
                {
                    preip_set_func(dst_addr, PREIP_ID_SET_MASK, argv[4]);
                }
                else if(!strcmp(argv[3], "ssid"))
                {
                    preip_set_func(dst_addr, PREIP_ID_SET_ESSID, argv[4]);
                }
                else if(!strcmp(argv[3], "rssithr") || !strcmp(argv[3], "rssi"))
                {
                    u8 rssithr[2];
                        
                    if(argc == 6)
                    {
                        rssithr[0] = atoi(argv[4]);
                        rssithr[1] = atoi(argv[5]);

                        /* TODO: check value here*/
                        
                    }else
                    {
                        printf("Invalid input. Usage: \n"
                               "    cpe_ctrl set xx:xx:xx:xx:xx:xx rssithr xxx yyy\n"
                               "    e.g. cpe_ctrl set 00:03:74:00:00:10 rssithr -85 -95\n");
                        return -1;
                    }


                    preip_set_func(dst_addr, PREIP_ID_SET_RSSITHR, rssithr);
                }
                else if(!strcmp(argv[3], "security") || !strcmp(argv[3], "sec"))
                {
                    /* TODO: check value */
                    
                
                    preip_set_func(dst_addr, PREIP_ID_SET_SECURITY, "");
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
    
}

