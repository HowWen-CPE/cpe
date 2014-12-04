/* Pre-IP Protocol Routines*/
/* Development Log */
/* */

#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB
#endif

#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/capability.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/errno.h>
#include <linux/in.h>
#include <linux/mm.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/fddidevice.h>
#include <linux/trdevice.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/rcupdate.h>
#include <linux/jhash.h>

#include <net/ip.h>
#include <net/icmp.h>
#include <net/route.h>
#include <net/protocol.h>
#include <net/tcp.h>
#include <net/sock.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include "preip.h"

//#define PRE_IP_DEBUG


int preip_pid = 0;

extern struct net init_net;


DEFINE_MUTEX(preip_mutex);


#ifdef PRE_IP_DEBUG

static void hex_dump(void *buf, int len)
{
    int i;

    for (i = 0 ; i < len; i++)
    {
	printk("%02x", ((u8*)buf)[i]);
	if (i%2)
	    printk(" ");
	if (15 == i%16)
	    printk("\n");
    }
    printk("\n");
}

static void skb_dump(char *name, struct sk_buff *skb)
{
    printk("+++++++++++++++++++++++++++\n"
	"%s SKB dump skb=%p, protocol = %d,data=%p, tail=%p, len=%d\n",
	name, skb, skb->protocol,skb->data, skb->tail, skb->len);
    printk(">> data:\n");
    hex_dump(skb->data, skb->len);
    printk("\n-------------------------\n");
}
#endif


A_BOOL preip_response(PreipFramInfo_t *frame_info)
{
    u8 *pbuf = NULL;
    //Frame8023Head_t* pmac;
    FrameSAPHead_t* psap;
    struct sk_buff * skb = NULL;
    struct net_device *dev;

#ifdef PRE_IP_DEBUG
    printk("preip_response: in\n");
#endif

    dev = dev_get_by_name(&init_net, PREIP_NET_DEVICE_NAME);

    if(dev == NULL)
    {
        printk("%s: can't get dev %s\n", __func__, PREIP_NET_DEVICE_NAME);

        return FALSE;
    }

    skb = alloc_skb(FRAME_SAP_OCTETS + sizeof(FramePreIpAll_t)
				+ LL_RESERVED_SPACE(dev), GFP_ATOMIC);

    if (skb == NULL)
        return FALSE;

    skb_reserve(skb, LL_RESERVED_SPACE(dev));

    /* pbuf point to SAP header*/
    pbuf = skb_put(skb,(FRAME_SAP_OCTETS + sizeof(FramePreIpAll_t)));

#ifdef PRE_IP_DEBUG
    printk("PreIP_DiscoveryResponse: Entry\n");
#endif
    
    skb->dev = dev;
    
    skb->protocol = htons(ETH_P_802_3);

    /* build 802.3 header*/
    if (dev_hard_header(skb, dev, ETH_P_802_3, frame_info->from_mac, dev->dev_addr, skb->len) < 0)
    {
        printk("%s: dev_hard_header error\n", __func__);
        
        kfree_skb(skb);
        return FALSE;
    }

    /* build SAP headeer */
    psap = (FrameSAPHead_t*)pbuf;
    psap->ssap = PREIP_SSAP;
    psap->dsap = PREIP_DSAP;
    psap->cmd = PREIP_SAPCMD;
    psap->protocol = htons(PREIP_SAPPROTOCOL);
    psap->vendorid[0] = PREIP_PRIV_ID1;
    psap->vendorid[1] = PREIP_PRIV_ID2;
    psap->vendorid[2] = PREIP_PRIV_ID3;
    
    memcpy(pbuf + FRAME_SAP_OCTETS, (u8 *)&frame_info->preip_all, sizeof(FramePreIpAll_t));

#ifdef PRE_IP_DEBUG
    printk("PreIP_Send Frame:");
#endif
    dev_queue_xmit(skb);

    return 0;
}


/* Check Rx Frame */
A_BOOL PreIP_FrameValid(u8* pbuf, int len)
{
    //Frame8023Head_t* pmac;
    FrameSAPHead_t* psap;
    FramePreIpHead_t* ppreip;
    //u8 srcmac[MACADDR_OCTETS];
    //u32 vendorid;
    
#ifdef PRE_IP_DEBUG
    printk("PreIP_FrameValid: Entry. buf=0x%08X len=%u\n", (u32)pbuf, len);
#endif

    if ((NULL == pbuf)||(PREIP_PKT_MIN_LEN>len)||(PREIP_PKT_MAX_LEN<len))
    {
#ifdef PRE_IP_DEBUG
        printk("PreIP_FrameValid: null buffer or too short or too long\n");
#endif
        return FALSE;
    }    
    //pmac = (Frame8023Head_t*)pbuf;
    psap = (FrameSAPHead_t*)(pbuf);
    FRAME_PREIP_GET(pbuf, ppreip);
    
    /* check src mac, must not malticast/broadcast*/
    //FRAME_SRCMAC_GET(pbuf, srcmac);
    //if (0 != (srcmac[0]&0x80))
    //{
//#ifdef PRE_IP_DEBUG
//        printk("PreIP_FrameValid: source mac not unicast\n");
//#endif
 //       return FALSE;
 //   }    
    if ((len > PREIP_PKT_MAX_LEN))
    {
#ifdef PRE_IP_DEBUG
        printk("PreIP_FrameValid: length error\n");
#endif
        return FALSE;
    }    
    if ((psap->dsap != PREIP_DSAP)||(psap->ssap != PREIP_SSAP)||
        (psap->cmd != PREIP_SAPCMD)||(psap->protocol != htons(PREIP_SAPPROTOCOL)))
    {
#ifdef PRE_IP_DEBUG
        printk("PreIP_FrameValid: format error\n");
#endif
        return FALSE;
    }    
    if ((psap->vendorid[0] != PREIP_PRIV_ID1)||(psap->vendorid[1] != PREIP_PRIV_ID2)
		||(psap->vendorid[2] != PREIP_PRIV_ID3))
    {
#ifdef PRE_IP_DEBUG
        printk("PreIP_FrameValid: vendor ID :%x-%x-%x\n",psap->vendorid[0] ,psap->vendorid[1] ,psap->vendorid[2] );
#endif
        return FALSE;
    }    
    
    return TRUE;
}



int nl_preip_transmit(char*buf, int len)
{
  int ret;
  int size;
  unsigned char *b;
  struct sk_buff *skb;
  struct nlmsghdr *nlh;
  struct nl_preip_info *packet;
  
  size = NLMSG_SPACE(sizeof(struct nl_preip_info));

#ifdef PRE_IP_DEBUG
    printk("nl_preip_transmit: in\n");
#endif

  skb = alloc_skb(size, GFP_ATOMIC);
  b = skb->tail;

  nlh = NLMSG_PUT(skb, 0, 0, 0, size - sizeof(*nlh));
  packet = NLMSG_DATA(nlh);
  
  memset(packet, 0, sizeof(struct nl_preip_info));
  packet->nli_type = NLI_TYPE_RUN;
  
  memcpy(packet->nli_buf,buf, len);

  nlh->nlmsg_len = skb->tail - b;
  NETLINK_CB(skb).dst_group = 0;

  ret = netlink_unicast(preip_nlfd, skb, preip_pid, MSG_DONTWAIT);

  return ret;
nlmsg_failure:
  if(skb)
    kfree_skb(skb);
  return -1;
}


void nl_preip_receive(struct sk_buff *skb)
{
    struct nl_preip_info *nli = NULL;
    struct nlmsghdr *nlh = NULL;
    PreipFramInfo_t frame_info;

    printk("%s: entry\n", __func__);
    
    mutex_lock(&preip_mutex);

    nlh = (struct nlmsghdr *)skb->data;
    if (skb->len < NLMSG_SPACE(0) || skb->len < nlh->nlmsg_len ||
                                           nlh->nlmsg_len < NLMSG_LENGTH(sizeof(*nli))) 
    {
        //kfree_skb(skb);

        mutex_unlock(&preip_mutex);
        
        printk("%s: error length\n", __func__);
        return;
    }
    nli = (struct nl_preip_info *) NLMSG_DATA(nlh);

    printk("nlh->nlmsg_len=%d\n", nlh->nlmsg_len);

    switch(nli->nli_type)
    {
     case NLI_TYPE_HELLO:
        printk("%s: NLI_TYPE_HELLO, nlh->nlmsg_pid=%d\n", __func__, nlh->nlmsg_pid);
     	preip_pid = nlh->nlmsg_pid;
        //kfree_skb(skb);
    	break;
      case NLI_TYPE_RUN:
        printk("%s: NLI_TYPE_RUN\n", __func__);
     	memcpy((char*)&frame_info, nli->nli_buf, sizeof(PreipFramInfo_t));

        preip_response(&frame_info);

    	break;
     default:
        printk("%s: defualt\n", __func__);
     	//kfree_skb(skb);
    	break;        
    }

    mutex_unlock(&preip_mutex);
}

static int unhandledPkt = 0;
#define MAXUNHANDLEDPKT 100
/* Process the Rx PreIP Procotol Frame */
/* Frame start from mac layer */

A_BOOL PreIP_Receive(struct sk_buff *skb)
{
    u8* frame = skb->data;
    u8* srcmac[MACADDR_OCTETS];
    int len = skb->len;
    struct net_device *dev = skb->dev;
    PreipFramInfo_t frame_info;
    int ret = TRUE;
    struct net_device *eth0_dev;
    preip_dscv_req_t  *p_req;
        
    eth0_dev = dev_get_by_name(&init_net, PREIP_NET_DEVICE_NAME);

    if(eth0_dev==NULL)
    {
        eth0_dev = dev;
    }
    
#ifdef PRE_IP_DEBUG
    printk("PreIP_Receive: in\n");
#endif

    
    if(!preip_pid){
        printk("preip deamon do not run\n");
        kfree_skb(skb);
        return FALSE;
    }
    if ((NULL == skb)||(PREIP_PKT_MAX_LEN < len)||
        (FALSE == PreIP_FrameValid(frame, len)))
    {
#ifdef PRE_IP_DEBUG
        printk("PreIP_Receive: Invalid Frame\n");
#endif
        kfree_skb(skb);
        return FALSE;
    }

#if 0
    if(!strcmp(dev->name,"br0") && wrongInterface())
#endif
    if(!strcmp(dev->name,"br0"))
     {
        kfree_skb(skb);
        return FALSE;
    }

    printk("Recv dev->name=%s\n", dev->name);
    

    if(unhandledPkt > MAXUNHANDLEDPKT){
        printk("PreIP_Receive: too much packets \n");
        if(unhandledPkt > 3*MAXUNHANDLEDPKT){
            kfree_skb(skb);
            return FALSE;
        }
    }

    FRAME_SRCMAC_GET(skb->mac_header, frame_info.from_mac);
    
    strcpy(frame_info.from_dev_name, dev->name);

    skb_pull(skb, FRAME_SAP_OCTETS);

    memcpy((u8 *)&frame_info.preip_all, skb->data, sizeof(frame_info.preip_all));

    p_req = &frame_info.preip_all.preip_data_buf.req_data;

#ifdef PRE_IP_DEBUG
    printk("from_mac: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
        frame_info.from_mac[0], frame_info.from_mac[1],frame_info.from_mac[2],
        frame_info.from_mac[3],frame_info.from_mac[4],frame_info.from_mac[5]);

    printk("to_mac: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
        p_req->mac[0], p_req->mac[1],p_req->mac[2],
        p_req->mac[3],p_req->mac[4],p_req->mac[5]);

    printk("eth0 mac: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
        eth0_dev->dev_addr[0],eth0_dev->dev_addr[1],eth0_dev->dev_addr[2],
        eth0_dev->dev_addr[3],eth0_dev->dev_addr[4],eth0_dev->dev_addr[5]);
#endif

    /* check if the dst mac address to us, if unicast, and not to us, then drop it*/
    if(p_req->mac[0] == 0xff)
    {
        /* multicast, only discovery request is allowed*/
        printk(" multicast\n");

        if(frame_info.preip_all.preip_hdr.id != PREIP_ID_DISCOVERY)
        {
            printk("    action (%d) not allowed\n", frame_info.preip_all.preip_hdr.id);

            kfree_skb(skb);

            return TRUE;
        }
    }
    else if(memcmp(p_req->mac, eth0_dev->dev_addr, 6) == 0)
    {
        /* unicast to us*/
        printk("unicast to us\n");
    }else
    {
        /* unicast not to us*/
        printk("unicast not to us\n");

        kfree_skb(skb);

        return TRUE;
    }
    
    /* send packet to user space*/
    if(nl_preip_transmit((u8 *)&frame_info, sizeof(frame_info)) == -1)
        kfree_skb(skb);
    else
        unhandledPkt ++; 
    return ret;
}


A_BOOL preIP_rcv(struct sk_buff *skb, struct net_device *dev,
		   struct packet_type *pt, struct net_device *orig_dev)
{

#ifdef PRE_IP_DEBUG
    printk("preIP_rcv:come in\n");
#endif

    if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL)
        goto out_of_mem;
#ifdef PRE_IP_DEBUG
    printk("PreIP_Receive: above\n");
#endif

    /*return NF_HOOK(NF_ARP, NF_ARP_IN, skb, dev, NULL, );*/
    return PreIP_Receive(skb);

out_of_mem:
	return 0;
}

/*
 *	Called once on startup.
 */
struct packet_type preIP_packet_type = {
	.type =	__constant_htons(ETH_P_802_2),
	.func =	preIP_rcv,
};


/* Pre IP Protocol Init */
int __init preIP_Init(void)
{
#ifdef PRE_IP_DEBUG
    printk("preIP_Init: in\n");
#endif
    preip_nlfd = netlink_kernel_create(&init_net, NETLINK_PREIP,1, nl_preip_receive, NULL, THIS_MODULE);
    if (preip_nlfd == NULL) {
        printk("%s: failed to create netlink socket\n",__func__);
        return -EBUSY;
    }
    dev_add_pack(&preIP_packet_type);
	
    return 0;
}

/* Pre IP Protocol exit */
static void __exit preIP_Exit(void)
{
    sock_release(preip_nlfd->sk_socket);
    dev_remove_pack(&preIP_packet_type);
}


module_init(preIP_Init);
module_exit(preIP_Exit);

MODULE_AUTHOR("Freedom");
MODULE_DESCRIPTION("Support Layer 2 configuration");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

