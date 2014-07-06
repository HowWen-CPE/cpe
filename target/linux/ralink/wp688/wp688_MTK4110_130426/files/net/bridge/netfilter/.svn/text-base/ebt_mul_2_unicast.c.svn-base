/*
 *  ebt_mul_2_unicast.c
 */
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/err.h>
#include <linux/if_ether.h>
#include <linux/igmp.h>
#include <linux/jhash.h>
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/netdevice.h>
#include <linux/netfilter_bridge.h>
#include <linux/random.h>
#include <linux/rculist.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <net/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/etherdevice.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <net/dst.h>
#include <net/arp.h>
#include <net/sock.h>
#include <linux/version.h>

#include "../br_private.h"

#define EBT_mul_2_unicast_TARGET "mul_2_unicast"

#define DEBUG_MLUTICAST

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif
#ifdef DEBUG_MLUTICAST
#define DEBUG(fmt, ...) \
	printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#define ERROR(fmt, ...) \
	printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...) \
	({ if (0) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__); 0; })
#define ERROR(fmt, ...) \
	({ if (0) printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__); 0; })
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
#define MY_TRUE		true
#define MY_FALSE	false	
#else
#define MY_TRUE		0
#define MY_FALSE	-EINVAL	
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
typedef struct xt_action_param EBT_TARGET_PARAM;
typedef struct xt_action_param EBT_MATCH_PARAM;
#else
typedef struct xt_target_param EBT_TARGET_PARAM;
typedef struct xt_match_param EBT_MATCH_PARAM;
#endif

struct ebt_multi_2_unicast_info
{
	unsigned int max_num;/*bit: 1-enabled, 0-disabled*/
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};

static void print_ipaddr(int ipaddr)
{
	DEBUG("\t\t%u.%u.%u.%u\n", (ipaddr>>24)&0xff, 
		(ipaddr>>16)&0xff, 
		(ipaddr>>8)&0xff, 
		ipaddr&0xff);
	
}

static void print_macaddr(unsigned char *macaddr)
{
	DEBUG("\t\t%02x:%02x:%02x:%02x:%02x:%02x\n", macaddr[0], 
		macaddr[1], 
		macaddr[2], macaddr[3], macaddr[4], 
		macaddr[5] );
	
}

static void print_skb(struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	
	//===printk ip addr	
	DEBUG("\tiph->saddr=");	
	print_ipaddr(iph->saddr);
	DEBUG("\tiph->daddr=");	
	print_ipaddr(iph->daddr);

	//===printk mac addr	
	DEBUG("\teth->h_source=");	
	print_macaddr(eth->h_source);
	DEBUG("\teth->h_dest=");	
	print_macaddr(eth->h_dest);
}

#ifdef MULTICAST_TO_UNITCAST

static void dump_group_member(struct net_group_member *gmember)
{
	DEBUG("%s: ----------------------------\n", __func__);	
	
	DEBUG("\t%s: one gmember->sip=", __func__);	
	print_ipaddr(gmember->sip);
	DEBUG("\t%s: one gmember->dip=", __func__);	
	print_ipaddr(gmember->dip);
	DEBUG("\t%s: one gmember->smac=", __func__);	
	print_macaddr((unsigned char *)&gmember->smac);
	DEBUG("\t%s: one gmember->dmac=", __func__);	
	print_macaddr((unsigned char *)&gmember->dmac);
	DEBUG("%s: ----------------------------\n", __func__);	
}

static void dump_mdb_group_member(struct net_bridge_mdb_entry *mp)
{
	struct net_group_member *gmember;
	struct hlist_node *p;
	unsigned count = 0;
	
	hlist_for_each_entry(gmember, p, &mp->gmemlist, gmemlist) {
		count++;
		dump_group_member(gmember);
	}
	DEBUG("%s: count=%d\n", __func__, count);	

	return;
}
#else
static void dump_group_member(struct net_group_member *gmember)
{
}

static void dump_mdb_group_member(struct net_bridge_mdb_entry *mp)
{
}

#endif

static void dump_bridge(struct net_bridge *bridge)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);
	DEBUG("\tgroup_addr=%2x:%2x:%2x:%2x:%2x:%2x:\n", 
		bridge->group_addr[0], 
		bridge->group_addr[1], 
		bridge->group_addr[2], 
		bridge->group_addr[3], 
		bridge->group_addr[4], 
		bridge->group_addr[5]);
	DEBUG("\tbridge->root_port=%d\n", bridge->root_port);
}

static void dump_bridge_port(struct net_bridge_port *port)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);
	DEBUG("\tport_no=%d\n", port->port_no);	
}

#ifdef MULTICAST_TO_UNITCAST
//#define CHANGE_IP_ADDR

/* create a new skb from old one and send
  * Return: 0--fail; 1--ok
  */
 static int __br_multicast_2_unicast(struct sk_buff * skb, 
 			struct net_group_member *gmember, 
 			const struct ebt_multi_2_unicast_info *m2u_param)
 {
	struct ethhdr* neth_hdr;
	struct sk_buff* nskb;
	int ret = 0;
	
#ifdef CHANGE_IP_ADDR
 	struct iphdr* old_ip_hdr = ip_hdr(skb);
	struct iphdr* nip_hdr;
	unsigned int   nip_hdr_off;
	struct tcphdr* ntcp_hdr;
	unsigned int ntcp_hdr_off;
	struct udphdr* nudp_hdr;
	unsigned int nudp_hdr_off;
#endif
    
	DEBUG("\t%s: to host=\n", __func__);
	print_ipaddr(gmember->sip);
	print_macaddr((unsigned char *)&gmember->smac);
	
	nskb = skb_copy(skb, GFP_ATOMIC);
	if(nskb == NULL)
	{
		ERROR("%s: %s\n", __func__, "skb_copy return NULL");
		return  0;
	}
	
#ifdef CHANGE_IP_ADDR
	//change IP dst addr
	nip_hdr = ip_hdr(nskb);
	nip_hdr_off = nip_hdr->ihl << 2;
    
	//change dest ip addr
	nip_hdr->daddr = gmember->sip;
	//ip checksum
	nip_hdr->check = 0;
	nip_hdr->check = ip_fast_csum((unsigned char *)nip_hdr, nip_hdr->ihl);

	switch (nip_hdr->protocol) {
		case IPPROTO_UDP:{
			//DEBUG("%s\n", "UDP protocol---------------------");
			nudp_hdr = (struct udphdr *)((void *)nip_hdr + nip_hdr_off);
			nudp_hdr_off = sizeof(*nudp_hdr);
			//netpoll_send_udp
			//skb check sum
			nskb->csum = 0;
			nskb->csum = csum_partial((unsigned char *)(nudp_hdr + nudp_hdr_off),
		                                       ntohs(nip_hdr->tot_len) - nip_hdr_off - nudp_hdr_off, 0);
			//layer 4 check sum
			nudp_hdr->check = 0;
			nudp_hdr->check = csum_tcpudp_magic(nip_hdr->saddr, nip_hdr->daddr,
		                                         ntohs(nip_hdr->tot_len) - nip_hdr_off, nip_hdr->protocol,
		                                         csum_partial((unsigned char *)nudp_hdr, nudp_hdr_off, nskb->csum));
			
		}
			break;
		case IPPROTO_TCP:{
			//DEBUG("%s\n", "TCP protocol---------------------");
			ntcp_hdr = (struct tcphdr *)((void *)nip_hdr + nip_hdr_off);
			ntcp_hdr_off = ntcp_hdr->doff << 2;

			//skb check sum
			nskb->csum = 0;
			nskb->csum = csum_partial((unsigned char *)(ntcp_hdr + ntcp_hdr_off),
		                                       ntohs(nip_hdr->tot_len) - nip_hdr_off - ntcp_hdr_off, 0);
			//layer 4 check sum
			ntcp_hdr->check = 0;
			ntcp_hdr->check = csum_tcpudp_magic(nip_hdr->saddr, nip_hdr->daddr,
		                                         ntohs(nip_hdr->tot_len) - nip_hdr_off, nip_hdr->protocol,
		                                         csum_partial((unsigned char *)ntcp_hdr, ntcp_hdr_off, nskb->csum));
		}
			break;
		default:
			DEBUG("%s: not TCP/UDP protocol, nip_hdr->protocol=%d\n", 
				__func__, nip_hdr->protocol);
			goto FAIL_OUT;
	}

#endif
	//Change MAC dst addr
	neth_hdr = eth_hdr(nskb);
	memcpy (neth_hdr->h_dest, &gmember->smac, ETH_ALEN);
	//Change pkt_type from PACKET_MULTICAST to PACKET_OTHERHOST
	nskb->pkt_type = PACKET_OTHERHOST;

	/*Get the if dev name of bridge port*/
	if(gmember->port) {
		if (gmember->port->state == BR_STATE_DISABLED){
			ERROR("%s: gmember->port->state is DISABLED\n", __func__);
			goto FAIL_OUT;
		}
		nskb->dev = gmember->port->dev;
	}
	else {
		ERROR("%s: no gmember->port, use skb->dev\n", __func__);
		nskb->dev = skb->dev;
	}
	//check and send package
	if (!netif_running(nskb->dev)){
		ERROR("%s: ******* (nskb->dev is not running)\n", __func__);
		goto FAIL_OUT;
	}

	/*ret = NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, nskb, NULL, nskb->dev,
			br_forward_finish);//send from local
	DEBUG("%s: NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, ret=%d\n", __func__, ret);
	*/
	ret = NF_HOOK(PF_BRIDGE, NF_BR_POST_ROUTING, nskb, NULL, nskb->dev,
		       br_dev_queue_push_xmit);
	DEBUG("%s: NF_HOOK(PF_BRIDGE, NF_BR_POST_ROUTING, ret=%d\n", __func__, ret);
	return (ret < 0)?0:1;

FAIL_OUT:
	if(nskb)
		kfree_skb(nskb);
	return 0;
} 

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
extern struct net_bridge_fdb_entry *br_fdb_get(struct net_bridge *br,
					  const unsigned char *addr);
#endif
//Return: number of multicast package transformed
static int _br_multicast_2_unicast(struct sk_buff *skb, struct net_bridge *br,
				     struct net_bridge_port *port, struct net_bridge_mdb_entry *mdb,
				     const struct ebt_multi_2_unicast_info *m2u_param)
{
	struct net_group_member *gmember;
	struct hlist_node *p;
	unsigned count = 0, okcnt = 0;
	
	if(mdb->total_gmem <= 0){
		DEBUG("%s: total_gmem(%d) <= 0.\n", __func__, mdb->total_gmem);
		return 0;
	}
	
	if(mdb->total_gmem > m2u_param->max_num){
		unsigned gmemcnt=0;
		//hlist_for_each_entry_rcu to avoid system crash, don't use hlist_for_each_entry
		hlist_for_each_entry_rcu(gmember, p, &mdb->gmemlist, gmemlist) {
			int gmember_port_no = (gmember->port)?gmember->port->port_no:-1;
			
			if(gmember_port_no == port->port_no){
				gmemcnt++;
			}
			
		}
		
		if(gmemcnt > m2u_param->max_num){
			if (net_ratelimit())
				DEBUG("%s: port(%s) gmemcnt(%d) > %d, ignore it.\n", __func__, 
					(port->dev&&port->dev->name)?port->dev->name:"null", 
					gmemcnt, m2u_param->max_num);
			return 0;
		}
		
	}
	
	//hlist_for_each_entry_rcu to avoid system crash, don't use hlist_for_each_entry
	hlist_for_each_entry_rcu(gmember, p, &mdb->gmemlist, gmemlist) {
		int gmember_port_no = (gmember->port)?gmember->port->port_no:-1;
		
		if(gmember_port_no != port->port_no){
			DEBUG("\t\t***port->port_no(%d) != gmember_port_no(%d).\n", 
				port->port_no, gmember_port_no);
			continue;
		}
		count++;
		if(__br_multicast_2_unicast(skb, gmember, m2u_param))
			okcnt++;

	}
	//DEBUG("%s: count=%d\n", __func__, count);	
	DEBUG("    succeed in transforming: okcnt=%d\n", okcnt);	

	return okcnt;
}

extern struct net_bridge_mdb_entry *br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, __be32 dst);

/* PARAM: port-skb destination port
  * Return: 0--fail;>0--OK
  */
static int br_multicast_2_unicast(struct sk_buff *skb, struct net_bridge *br,
				     struct net_bridge_port *port, 
				     const struct ebt_multi_2_unicast_info *m2u_param)
{
	struct net_bridge_mdb_htable *mdb_ht;
	struct net_bridge_mdb_entry *mdb;
	struct iphdr *iph;
	int ret = 0;

	spin_lock(&br->multicast_lock);

	iph = ip_hdr(skb);
	
	mdb_ht = br->mdb;
	mdb = br_mdb_ip_get(mdb_ht, iph->daddr);
	if (!mdb) {
		//ERROR("%s: Fail to br_mdb_ip_get, ip=0x%X.\n", __func__, iph->daddr);	
        ret = 0;
		goto out;
	}
	
	ret = _br_multicast_2_unicast(skb, br, port, mdb, m2u_param);
out:
	spin_unlock(&br->multicast_lock);
	return ret;
}

#else
static int br_multicast_2_unicast(struct sk_buff *skb, struct net_bridge *br,
				     struct net_bridge_port *port, 
				     const struct ebt_multi_2_unicast_info *m2u_param)
{
	return 0;
}
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static unsigned int
ebt_multi_2_unicast_tg(struct sk_buff *skb, const EBT_TARGET_PARAM *par)
#else
static int ebt_multi_2_unicast_tg(struct sk_buff *skb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
	const unsigned char *dest;
	struct net_bridge_port *p;
	struct net_bridge *br;
	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_multi_2_unicast_info *m2u_param = par->targinfo;
#else
	const struct ebt_multi_2_unicast_info *m2u_param = data;
#endif
	
	int target = (m2u_param->target|~EBT_VERDICT_BITS);

	/*DEBUG("\n%s: max_num=%d, target=%d\n", 
		__func__, m2u_param->max_num, target);
	DEBUG("\n%s: par hooknum=%d\n", 
		__func__, par->hooknum);*/
	//check multicast package
	dest = eth_hdr(skb)->h_dest;
	if (!is_multicast_ether_addr(dest)) {
		ERROR("%s: is not a multicast package\n", __func__);
		return EBT_CONTINUE;
	}
	//check skb valiable
	if (!skb|| !skb->dev || !netif_running(skb->dev)){
		ERROR("%s: !skb|| skb->dev is NULL or is not running\n", __func__);
		return EBT_CONTINUE;
	}

	//p = rcu_dereference(skb->dev->br_port);
	p=NULL;
	if((p == NULL) || (p->state == BR_STATE_DISABLED)) {
		ERROR("%s: net_bridge_port is NULL or BR_STATE_DISABLED\n", __func__);
		return EBT_CONTINUE;
	}
	DEBUG("%s: skb to port=%s\n", __func__,  
		(p->dev&&p->dev->name)?p->dev->name:"null");
	br = p->br;
	if (!br || !br->dev || !netif_running(br->dev)){
		ERROR("%s: br->dev is NULL or is not running\n", __func__);
		return EBT_CONTINUE;
	}

	if(br_multicast_2_unicast(skb, br, p, m2u_param))
		return EBT_DROP;//succeed and drop 
	else {
		//ERROR("%s: Fail to br_multicast_2_unicast\n", __func__);
		return target;
	}
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int ebt_multi_2_unicast_tg_check(const struct xt_tgchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool ebt_multi_2_unicast_tg_check(const struct xt_tgchk_param *par)
#else
static int ebt_multi_2_unicast_tg_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_multi_2_unicast_info *info = par->targinfo;
#else
	const struct ebt_multi_2_unicast_info *info = data;
#endif
	int tmp;

	tmp = info->target | ~EBT_VERDICT_BITS;
	if (BASE_CHAIN && tmp == EBT_RETURN)
		return MY_FALSE;

	if (tmp < -NUM_STANDARD_TARGETS || tmp >= 0)
		return MY_FALSE;
	return MY_TRUE;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static struct xt_target ebt_multi_2_unicast_tg_reg __read_mostly = {
	.name		= EBT_mul_2_unicast_TARGET,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.table		= "nat",
	.hooks		= (1 << NF_BR_POST_ROUTING),
	.target		= ebt_multi_2_unicast_tg,
	.checkentry	= ebt_multi_2_unicast_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_multi_2_unicast_info)),
	.me		= THIS_MODULE,
};
#else
static struct ebt_target ebt_multi_2_unicast_tg_reg __read_mostly = {
	.name		= EBT_mul_2_unicast_TARGET,
	.target		= ebt_multi_2_unicast_tg,
	.check		= ebt_multi_2_unicast_tg_check,
	.me		= THIS_MODULE,
};
#endif

static int __init ebt_multi_2_unicast_init(void)
{
	printk("%s: line=%d\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return xt_register_target(&ebt_multi_2_unicast_tg_reg);
#else
	return ebt_register_target(&ebt_multi_2_unicast_tg_reg);
#endif
}

static void __exit ebt_multi_2_unicast_fini(void)
{
	printk("%s: line=%d\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_target(&ebt_multi_2_unicast_tg_reg);
#else
	ebt_unregister_target(&ebt_multi_2_unicast_tg_reg);
#endif
}

module_init(ebt_multi_2_unicast_init);
module_exit(ebt_multi_2_unicast_fini);
MODULE_DESCRIPTION("Ebtables: Packet multi_2_unicast modification");
MODULE_LICENSE("GPL");
