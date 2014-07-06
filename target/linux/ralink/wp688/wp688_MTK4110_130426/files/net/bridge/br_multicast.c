/*
 * Bridge multicast support.
 *
 * Copyright (c) 2010 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

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
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <net/ip.h>

#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>

#include <linux/types.h>
#include <linux/string.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/etherdevice.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <net/dst.h>
#include <net/arp.h>
#include <net/sock.h>

#include <linux/inetdevice.h>

#include "br_private.h"

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

void print_ipaddr(int ipaddr)
{
	//DEBUG("print_ipaddr=0x%x\n", __func__, ipaddr);
	DEBUG("\t\t%u.%u.%u.%u\n", (ipaddr>>24)&0xff, 
		(ipaddr>>16)&0xff, 
		(ipaddr>>8)&0xff, 
		ipaddr&0xff);
	
}

void print_macaddr(unsigned char *macaddr)
{
	DEBUG("\t\t%02x:%02x:%02x:%02x:%02x:%02x\n", macaddr[0], 
		macaddr[1], 
		macaddr[2], macaddr[3], macaddr[4], 
		macaddr[5] );
	
}

void print_skb(struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	
	//===printk ip addr	
	DEBUG("\tiph->saddr=");	
	print_ipaddr(ntohl(iph->saddr));
	DEBUG("\tiph->daddr=");	
	print_ipaddr(ntohl(iph->daddr));

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
	print_macaddr(&gmember->smac);
	//DEBUG("\t%s: one gmember->dmac=", __func__);	
	//print_macaddr(&gmember->dmac);
	DEBUG("%s: ----------------------------\n", __func__);	
}

static void dump_mdb_group_member(struct net_bridge_mdb_entry *mp)
{
	struct net_group_member *gmember;
	struct hlist_node *p;
	unsigned count = 0;
	
	//DEBUG("%s: \n", __func__);	
	
	hlist_for_each_entry_rcu(gmember, p, &mp->gmemlist, gmemlist) {
		count++;
		dump_group_member(gmember);
	}
	DEBUG("%s: count=%d\n", __func__, count);	
}
#else
static void dump_group_member(struct net_group_member *gmember)
{
}

static void dump_mdb_group_member(struct net_bridge_mdb_entry *mp)
{
}

#endif

void dump_bridge(struct net_bridge *bridge)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);
	DEBUG("\t group_addr=%2x:%2x:%2x:%2x:%2x:%2x:\n",  
		bridge->group_addr[0], 
		bridge->group_addr[1], 
		bridge->group_addr[2], 
		bridge->group_addr[3], 
		bridge->group_addr[4], 
		bridge->group_addr[5]);
	DEBUG("\t bridge->root_port=%d\n", bridge->root_port);
}


void dump_bridge_port(struct net_bridge_port *port)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);
	DEBUG("\t port_no=%d\n", port->port_no);	
	DEBUG("\t port name=%s\n",  (port->dev&&port->dev->name)?port->dev->name:"null");
}

//===================alan added=========================

static inline int br_ip_hash(struct net_bridge_mdb_htable *mdb, __be32 ip)
{
	return jhash_1word(mdb->secret, (u32)ip) & (mdb->max - 1);
}

static struct net_bridge_mdb_entry *__br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, __be32 dst, int hash)
{
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;

	hlist_for_each_entry_rcu(mp, p, &mdb->mhash[hash], hlist[mdb->ver]) {
	/*	if (dst == mp->addr)
			return mp;
			*/
	}

	return NULL;
}
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))

/* No locking or refcounting, assumes caller has no preempt (rcu_read_lock) */
struct net_bridge_fdb_entry *br_fdb_get(struct net_bridge *br,
					  unsigned char *addr)
{
	return __br_fdb_get(br, addr);
}
EXPORT_SYMBOL(br_fdb_get);
#else
extern struct net_bridge_fdb_entry *br_fdb_get(struct net_bridge *br,
					unsigned char *addr);
#endif

struct net_bridge_mdb_entry *br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, __be32 dst)
{
	if (!mdb)
		return NULL;

	return __br_mdb_ip_get(mdb, dst, br_ip_hash(mdb, dst));
}
EXPORT_SYMBOL(br_mdb_ip_get);

struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
					struct sk_buff *skb)
{
	if (br->multicast_disabled)
		return NULL;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		if (BR_INPUT_SKB_CB(skb)->igmp)
			break;
		return br_mdb_ip_get(br->mdb, ip_hdr(skb)->daddr);
	}

	return NULL;
}

static void br_mdb_free(struct rcu_head *head)
{
	struct net_bridge_mdb_htable *mdb =
		container_of(head, struct net_bridge_mdb_htable, rcu);
	struct net_bridge_mdb_htable *old = mdb->old;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	mdb->old = NULL;
	kfree(old->mhash);
	kfree(old);
}

static int br_mdb_copy(struct net_bridge_mdb_htable *new,
		       struct net_bridge_mdb_htable *old,
		       int elasticity)
{
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;
	int maxlen;
	int len;
	int i;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	for (i = 0; i < old->max; i++)
		hlist_for_each_entry(mp, p, &old->mhash[i], hlist[old->ver])
/*			hlist_add_head(&mp->hlist[new->ver],
				       &new->mhash[br_ip_hash(new, mp->addr)]);
				       */

	if (!elasticity)
		return 0;

	maxlen = 0;
	for (i = 0; i < new->max; i++) {
		len = 0;
		hlist_for_each_entry(mp, p, &new->mhash[i], hlist[new->ver])
			len++;
		if (len > maxlen)
			maxlen = len;
	}

	return maxlen > elasticity ? -EINVAL : 0;
}

static void br_multicast_free_pg(struct rcu_head *head)
{
	struct net_bridge_port_group *p =
		container_of(head, struct net_bridge_port_group, rcu);
	DEBUG("%s: Line=%d\n", __func__, __LINE__);	

	kfree(p);
}

static int br_multicast_del_all_group_member(
	struct net_bridge_mdb_entry *mp);

static void br_multicast_free_group(struct rcu_head *head)
{
	struct net_bridge_mdb_entry *mp =
		container_of(head, struct net_bridge_mdb_entry, rcu);
	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
#ifdef MULTICAST_TO_UNITCAST
	br_multicast_del_all_group_member(mp);
#endif
	kfree(mp);
}

static void br_multicast_group_expired(unsigned long data)
{
	struct net_bridge_mdb_entry *mp = (void *)data;
	struct net_bridge *br = mp->br;
	struct net_bridge_mdb_htable *mdb;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || timer_pending(&mp->timer))
		goto out;

	if (!hlist_unhashed(&mp->mglist))
		hlist_del_init(&mp->mglist);

	if (mp->ports)
		goto out;

	mdb = br->mdb;
	hlist_del_rcu(&mp->hlist[mdb->ver]);
	mdb->size--;

	del_timer(&mp->query_timer);
	call_rcu_bh(&mp->rcu, br_multicast_free_group);

out:
	spin_unlock(&br->multicast_lock);
}

static void br_multicast_del_pg(struct net_bridge *br,
				struct net_bridge_port_group *pg)
{
	struct net_bridge_mdb_htable *mdb = br->mdb;
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group **pp;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	//mp = br_mdb_ip_get(mdb, pg->addr);
	mp=NULL;
	if (WARN_ON(!mp))
		return;

	for (pp = &mp->ports; (p = *pp); pp = &p->next) {
		if (p != pg)
			continue;

		*pp = p->next;
		hlist_del_init(&p->mglist);
		del_timer(&p->timer);
		del_timer(&p->query_timer);
		call_rcu_bh(&p->rcu, br_multicast_free_pg);

		if (!mp->ports && hlist_unhashed(&mp->mglist) &&
		    netif_running(br->dev))
			mod_timer(&mp->timer, jiffies);

		return;
	}

	WARN_ON(1);
}

static void br_multicast_port_group_expired(unsigned long data)
{
	struct net_bridge_port_group *pg = (void *)data;
	struct net_bridge *br = pg->port->br;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || timer_pending(&pg->timer) ||
	    hlist_unhashed(&pg->mglist))
		goto out;

	br_multicast_del_pg(br, pg);

out:
	spin_unlock(&br->multicast_lock);
}
//create mdb struct
static int br_mdb_rehash(struct net_bridge_mdb_htable **mdbp, int max,
			 int elasticity)
{
	struct net_bridge_mdb_htable *old = *mdbp;
	struct net_bridge_mdb_htable *mdb;
	int err;

	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	mdb = kmalloc(sizeof(*mdb), GFP_ATOMIC);
	if (!mdb)
		return -ENOMEM;

	mdb->max = max;
	mdb->old = old;

	mdb->mhash = kzalloc(max * sizeof(*mdb->mhash), GFP_ATOMIC);
	if (!mdb->mhash) {
		kfree(mdb);
		return -ENOMEM;
	}

	mdb->size = old ? old->size : 0;
	mdb->ver = old ? old->ver ^ 1 : 0;

	if (!old || elasticity)
		get_random_bytes(&mdb->secret, sizeof(mdb->secret));
	else
		mdb->secret = old->secret;

	if (!old)
		goto out;

	err = br_mdb_copy(mdb, old, elasticity);
	if (err) {
		kfree(mdb->mhash);
		kfree(mdb);
		return err;
	}

	call_rcu_bh(&mdb->rcu, br_mdb_free);

out:
	rcu_assign_pointer(*mdbp, mdb);

	return 0;
}

/*Return: 1-all disabled, 0-no*/
static int is_br_multicast_all_disabled(struct net_bridge *br)
{
	int ret = br->multicast_disabled;
#ifdef MULTICAST_TO_UNITCAST
	ret &= br->m2u_disabled;
#endif	
	//DEBUG("%s: ret=%d\n", __func__, ret);
	return ret;
}

#ifdef MULTICAST_TO_UNITCAST
static void br_multicast_group_member_expired(unsigned long data);

#define MY_IP_ADDR 	0x0U	/* 0.0.0.0  */

int get_bridge_addr(struct net_bridge *br)
{
	struct in_device *in_dev;
	struct in_ifaddr **ifap = NULL;
	struct in_ifaddr *ifa = NULL;
	struct net_device *dev;
	__be32	ip_addr;
	const char *ifname;

	if((br == NULL) || (br->dev ==NULL)){
		ERROR("%s: ERROR (br == NULL) || (br->dev ==NULL)\n", __func__);
		return -1;
	}
	//DEBUG("%s: br->dev->name=%s\n", __func__, br->dev->name);
	dev = br->dev;
	ifname = dev->name;	
	
	if ((in_dev = __in_dev_get_rtnl(dev)) != NULL) {
		if (1) {
			/* Matthias Andree */
			/* compare label and address (4.4BSD style) */
			/* note: we only do this for a limited set of ioctls
			   and only if the original address family was AF_INET.
			   This is checked above. */
			for (ifap = &in_dev->ifa_list; (ifa = *ifap) != NULL;
			     ifap = &ifa->ifa_next) {
				if (!strcmp(ifname, ifa->ifa_label)) {
					break; /* found */
				}
			}
		}
	}

	if (ifa) {
		ip_addr = ifa->ifa_local;
		//DEBUG("%s: Found if IP addr=0x%x\n", __func__, ip_addr);
	}
	else {
		ip_addr = MY_IP_ADDR;
		ERROR("%s: ***Fail to find interface, use default IP***.\n", __func__);
	}

	return ip_addr;
}

//生成igmp查询包skb, 计算csum
static struct sk_buff *br_multicast_alloc_query(struct net_bridge *br,
						__be32 group)
{
	struct sk_buff *skb;
	struct igmpv3_query *igmp_query;
	struct ethhdr *eth;
	struct iphdr *iph;
	int	ipaddr;

	//DEBUG("%s: V3 Line=%d\n", __func__, __LINE__);	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	skb = netdev_alloc_skb_ip_align(br->dev, sizeof(*eth) + sizeof(*iph) +
						 sizeof(*igmp_query) + 4);
#else
	skb = netdev_alloc_skb(br->dev, sizeof(*eth) + sizeof(*iph) +
						 sizeof(*igmp_query) + 4);
	if (NET_IP_ALIGN && skb)
		skb_reserve(skb, NET_IP_ALIGN);
#endif
	if (!skb)
		goto out;

	skb->protocol = htons(ETH_P_IP);
	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

	memcpy(eth->h_source, br->dev->dev_addr, 6);
	eth->h_dest[0] = 1;
	eth->h_dest[1] = 0;
	eth->h_dest[2] = 0x5e;
	eth->h_dest[3] = 0;
	eth->h_dest[4] = 0;
	eth->h_dest[5] = 1;
	eth->h_proto = htons(ETH_P_IP);
	skb_put(skb, sizeof(*eth));

	skb_set_network_header(skb, skb->len);
	iph = ip_hdr(skb);

	iph->version = 4;
	iph->ihl = 6;
	iph->tos = 0xc0;
	iph->tot_len = htons(sizeof(*iph) + sizeof(*igmp_query) + 4);
	iph->id = 0;
	iph->frag_off = htons(IP_DF);
	iph->ttl = 1;
	iph->protocol = IPPROTO_IGMP;
	ipaddr = get_bridge_addr(br);
	iph->saddr = htonl(ipaddr);
	iph->daddr = htonl(INADDR_ALLHOSTS_GROUP);
	((u8 *)&iph[1])[0] = IPOPT_RA;
	((u8 *)&iph[1])[1] = 4;
	((u8 *)&iph[1])[2] = 0;
	((u8 *)&iph[1])[3] = 0;
	ip_send_check(iph);
	skb_put(skb, 24);

	skb_set_transport_header(skb, skb->len);

	igmp_query = igmpv3_query_hdr(skb);
	igmp_query->type  = IGMP_HOST_MEMBERSHIP_QUERY;
	igmp_query->code = 0x01;//0.1s
	igmp_query->group = group;
	igmp_query->resv = 0;
	igmp_query->suppress = 0;
	igmp_query->qrv = 0;
	igmp_query->qqic = 0;
	igmp_query->nsrcs = 0;
		
	igmp_query->csum = 0;
	igmp_query->csum = ip_compute_csum((void *)igmp_query, 
		sizeof(struct igmpv3_query));
	skb_put(skb, sizeof(*igmp_query));

	__skb_pull(skb, sizeof(*eth));

out:
	return skb;
}

//if the group_member existed in mdb entry
//return: 1-existed;0-no existed.
/*
  * 由于IGMP V1 V2 和V3 的成员报告的目的MAC/IP 地址不同，
  * 如V2 的地址(01:00:5e:7f:07:07/239.255.7.7) , 
  * 而V3 的地址(01:00:5e:00:00:16/224.0.0.22)；
  * 所以判断成员的唯一性，只能用源地址mac
  * update src ip if different
  */
static struct net_group_member * is_group_member_existed(
	struct sk_buff *skb, struct net_bridge_mdb_entry *mp)
{
	struct net_group_member *gmember;
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	struct hlist_node *p;
	unsigned count = 0;
	
	//DEBUG("%s: \n", __func__);	
	hlist_for_each_entry_rcu(gmember, p, &mp->gmemlist, gmemlist) {
		count++;
		//dump_group_member(gmember);
		if (!memcmp(eth->h_source, &gmember->smac, 6)) {
			DEBUG("%s: -----find group member.\n", __func__);
			if (iph->saddr != gmember->sip) {
				DEBUG("%s: iph->saddr changed, update gmember->sip.\n", __func__);
				gmember->sip = iph->saddr;
			}
			return gmember;
		}
	}
	DEBUG("%s:***** group member not existed, count=%d\n", __func__, count);	

	return NULL;
}

//create group_member
static struct net_group_member *br_multicast_new_group_member(
	struct net_bridge_mdb_entry *mdb,
	struct sk_buff *skb, struct net_bridge_port *port, __be32 group)
{
	struct net_group_member *gmember;
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	
	DEBUG("%s: from port name=%s\n", __func__,  
		(port && port->dev&&port->dev->name)?port->dev->name:"null");
	print_skb(skb);
	
	gmember = kmalloc(sizeof(struct net_group_member), GFP_ATOMIC);
	if (!gmember)
		return NULL;
	gmember->mdb= mdb;
	gmember->dip = group;
	gmember->sip = iph->saddr;
	if(port)
		gmember->port= port;
	else
		ERROR("%s: *****iph->sadd0x%x from NULL port\n", __func__, iph->saddr);
	
	memcpy(&gmember->smac, eth->h_source, 6);
	memcpy(&gmember->dmac, eth->h_dest, 6);

	hlist_add_head(&gmember->gmemlist, &mdb->gmemlist);
	//member expired timer
	setup_timer(&gmember->timer, br_multicast_group_member_expired,
		    (unsigned long)gmember);
	mdb->total_gmem++;
	
	return gmember;
}
//free group_member's resource
static void br_multicast_free_group_member(struct rcu_head *head)
{
	struct net_group_member *gm =
		container_of(head, struct net_group_member, rcu);
	struct net_bridge_mdb_entry *mdb = gm->mdb;
	DEBUG("%s: Line=%d\n", __func__, __LINE__);	
	mdb->total_gmem--;
	kfree(gm);
}

//delete a group_member
//return 1-succeeded.
static int br_multicast_del_a_group_member(
	struct net_group_member *gmember)
{
	DEBUG("%s: \n", __func__);	
	dump_group_member(gmember);
	
	hlist_del_init(&gmember->gmemlist);
	del_timer_sync(&gmember->timer);
	call_rcu_bh(&gmember->rcu, br_multicast_free_group_member);

	return 1;
}

//delete group_member
//return the number of member found
static int br_multicast_del_group_member(
	struct sk_buff *skb, struct net_bridge_mdb_entry *mp)
{
	struct net_group_member *gmember;
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	struct hlist_node *p;
	unsigned count = 0;
	unsigned found = 0;
	
	DEBUG("%s: \n", __func__);	
	print_skb(skb);
	
	hlist_for_each_entry_rcu(gmember, p, &mp->gmemlist, gmemlist) {
		count++;
		if (!memcmp(eth->h_source, &gmember->smac, 6)) {
			found = 1;
			break;
		}
	}
	DEBUG("%s: total member count=%d", __func__,count);	
	if(found) {
		DEBUG("%s: -----find group member, will delete it.\n", __func__);
		br_multicast_del_a_group_member(gmember);
	}

	return count;
}

//delete group_member
//return the number of member found
static int br_multicast_del_all_group_member(
	struct net_bridge_mdb_entry *mp)
{
	struct net_group_member *gmember;
	struct hlist_node *p;
	unsigned count = 0;
	unsigned existed = 0;

	DEBUG("%s: group ip=", __func__);	
	//print_ipaddr(mp->addr);
	do{
		existed = 0;
		hlist_for_each_entry_rcu(gmember, p, &mp->gmemlist, gmemlist) {
			count++;
			existed = 1;
			br_multicast_del_a_group_member(gmember);
			break;
		}
	}while(existed) ;
	
	DEBUG("%s: -- delete count=%d.\n", __func__, count);

	return count;
}
//group member's timer expired, delete it
static void br_multicast_group_member_expired(unsigned long data)
{
	struct net_group_member *gm = (void *)data;
	struct net_bridge *br;

	DEBUG("%s: member=\n", __func__);	
	dump_group_member(gm);
	if(!gm){
		ERROR("%s: gm is NULL", __func__);
		return;
	}
	if(!gm->port){
		ERROR("%s: gm->port is NULL", __func__);
		return;
	}
	if(!gm->port->br){
		ERROR("%s: gm->port->br is NULL", __func__);
		return;
	}
	br = gm->port->br;
//TODO: if need to define a new lock for member
	spin_lock(&br->multicast_lock);

	br_multicast_del_a_group_member(gm);

	spin_unlock(&br->multicast_lock);
}

#else
//create igmp query skb,  calc csum
static struct sk_buff *br_multicast_alloc_query(struct net_bridge *br,
						__be32 group)
{
	struct sk_buff *skb;
	struct igmphdr *ih;
	struct ethhdr *eth;
	struct iphdr *iph;

	//DEBUG("%s: V2 Line=%d\n", __func__, __LINE__);	
	skb = netdev_alloc_skb_ip_align(br->dev, sizeof(*eth) + sizeof(*iph) +
						 sizeof(*ih) + 4);
	if (!skb)
		goto out;

	skb->protocol = htons(ETH_P_IP);

	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

	memcpy(eth->h_source, br->dev->dev_addr, 6);
	eth->h_dest[0] = 1;
	eth->h_dest[1] = 0;
	eth->h_dest[2] = 0x5e;
	eth->h_dest[3] = 0;
	eth->h_dest[4] = 0;
	eth->h_dest[5] = 1;
	eth->h_proto = htons(ETH_P_IP);
	skb_put(skb, sizeof(*eth));

	skb_set_network_header(skb, skb->len);
	iph = ip_hdr(skb);

	iph->version = 4;
	iph->ihl = 6;
	iph->tos = 0xc0;
	iph->tot_len = htons(sizeof(*iph) + sizeof(*ih) + 4);
	iph->id = 0;
	iph->frag_off = htons(IP_DF);
	iph->ttl = 1;
	iph->protocol = IPPROTO_IGMP;
	iph->saddr = 0;
	iph->daddr = htonl(INADDR_ALLHOSTS_GROUP);
	((u8 *)&iph[1])[0] = IPOPT_RA;
	((u8 *)&iph[1])[1] = 4;
	((u8 *)&iph[1])[2] = 0;
	((u8 *)&iph[1])[3] = 0;
	ip_send_check(iph);
	skb_put(skb, 24);

	skb_set_transport_header(skb, skb->len);
	ih = igmp_hdr(skb);
	ih->type = IGMP_HOST_MEMBERSHIP_QUERY;
	ih->code = (group ? br->multicast_last_member_interval :
			    br->multicast_query_response_interval) /
		   (HZ / IGMP_TIMER_SCALE);//0-v1;!0-v2
	ih->group = group;
	ih->csum = 0;
	ih->csum = ip_compute_csum((void *)ih, sizeof(struct igmphdr));
	skb_put(skb, sizeof(*ih));

	__skb_pull(skb, sizeof(*eth));

out:
	return skb;
}


#endif

static void br_multicast_send_group_query(struct net_bridge_mdb_entry *mp)
{
	struct net_bridge *br = mp->br;
	struct sk_buff *skb;

	//skb = br_multicast_alloc_query(br, mp->addr);
        skb=NULL;
	if (!skb)
		goto timer;
//???
	netif_rx(skb);

timer:
	if (++mp->queries_sent < br->multicast_last_member_count)
		mod_timer(&mp->query_timer,
			  jiffies + br->multicast_last_member_interval);
}

static void br_multicast_group_query_expired(unsigned long data)
{
	struct net_bridge_mdb_entry *mp = (void *)data;
	struct net_bridge *br = mp->br;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || hlist_unhashed(&mp->mglist) ||
	    mp->queries_sent >= br->multicast_last_member_count)
		goto out;

	br_multicast_send_group_query(mp);

out:
	spin_unlock(&br->multicast_lock);
}
//send query to port
static void br_multicast_send_port_group_query(struct net_bridge_port_group *pg)
{
	struct net_bridge_port *port = pg->port;
	struct net_bridge *br = port->br;
	struct sk_buff *skb;

	//skb = br_multicast_alloc_query(br, pg->addr);
	skb=NULL;
	if (!skb)
		goto timer;

	br_deliver(port, skb);

timer:
	if (++pg->queries_sent < br->multicast_last_member_count)
		mod_timer(&pg->query_timer,
			  jiffies + br->multicast_last_member_interval);
}

static void br_multicast_port_group_query_expired(unsigned long data)
{
	struct net_bridge_port_group *pg = (void *)data;
	struct net_bridge_port *port = pg->port;
	struct net_bridge *br = port->br;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || hlist_unhashed(&pg->mglist) ||
	    pg->queries_sent >= br->multicast_last_member_count)
		goto out;

	br_multicast_send_port_group_query(pg);

out:
	spin_unlock(&br->multicast_lock);
}
//search the htable with group ip.(unique)
static struct net_bridge_mdb_entry *br_multicast_get_group(
	struct net_bridge *br, struct net_bridge_port *port, __be32 group,
	int hash)
{
	struct net_bridge_mdb_htable *mdb = br->mdb;
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;
	unsigned count = 0;
	unsigned max;
	int elasticity;
	int err;

	hlist_for_each_entry(mp, p, &mdb->mhash[hash], hlist[mdb->ver]) {
		count++;
	/*	if (unlikely(group == mp->addr)) {
			return mp;
		}
		*/
	}

	elasticity = 0;
	max = mdb->max;

	if (unlikely(count > br->hash_elasticity && count)) {
		if (net_ratelimit())
			printk(KERN_INFO "%s: Multicast hash table "
			       "chain limit reached: %s\n",
			       br->dev->name, port ? port->dev->name :
						     br->dev->name);

		elasticity = br->hash_elasticity;
	}

	if (mdb->size >= max) {
		max *= 2;
		if (unlikely(max >= br->hash_max)) {
			printk(KERN_WARNING "%s: Multicast hash table maximum "
			       "reached, disabling snooping: %s, %d\n",
			       br->dev->name, port ? port->dev->name :
						     br->dev->name,
			       max);
			err = -E2BIG;
disable:
			br->multicast_disabled = 1;
			goto err;
		}
	}

	if (max > mdb->max || elasticity) {
		if (mdb->old) {
			if (net_ratelimit())
				printk(KERN_INFO "%s: Multicast hash table "
				       "on fire: %s\n",
				       br->dev->name, port ? port->dev->name :
							     br->dev->name);
			err = -EEXIST;
			goto err;
		}

		err = br_mdb_rehash(&br->mdb, max, elasticity);
		if (err) {
			printk(KERN_WARNING "%s: Cannot rehash multicast "
			       "hash table, disabling snooping: "
			       "%s, %d, %d\n",
			       br->dev->name, port ? port->dev->name :
						     br->dev->name,
			       mdb->size, err);
			goto disable;
		}

		err = -EAGAIN;
		goto err;
	}

	return NULL;

err:
	mp = ERR_PTR(err);
	return mp;
}
//create mdb group unique in htable
static struct net_bridge_mdb_entry *br_multicast_new_group(
	struct net_bridge *br, struct net_bridge_port *port, __be32 group)
{
	struct net_bridge_mdb_htable *mdb = br->mdb;
	struct net_bridge_mdb_entry *mp;
	int hash;

	if (!mdb) {//first calls in, create a mdb
		if (br_mdb_rehash(&br->mdb, BR_HASH_SIZE, 0))
			return NULL;
		goto rehash;
	}

	hash = br_ip_hash(mdb, group);
	mp = br_multicast_get_group(br, port, group, hash);
	switch (PTR_ERR(mp)) {
	case 0:
		break;

	case -EAGAIN:
rehash:
		mdb = br->mdb;
		hash = br_ip_hash(mdb, group);
		break;

	default:
		goto out;
	}

	mp = kzalloc(sizeof(*mp), GFP_ATOMIC);
	if (unlikely(!mp))
		goto out;

	mp->br = br;
	//mp->addr = group;
	setup_timer(&mp->timer, br_multicast_group_expired,
		    (unsigned long)mp);
	setup_timer(&mp->query_timer, br_multicast_group_query_expired,
		    (unsigned long)mp);

	hlist_add_head_rcu(&mp->hlist[mdb->ver], &mdb->mhash[hash]);
	mdb->size++;

out:
	return mp;
}

//create a group, include mdb_entry and port_group
static int br_multicast_add_group(struct sk_buff *skb, struct net_bridge *br,
				  struct net_bridge_port *port, __be32 group)
{
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group **pp;
	unsigned long now = jiffies;
	int err;
	struct net_group_member *gmember;
	
	DEBUG("%s: from port name=%s, group=", __func__,  
		(port&&port->dev&&port->dev->name)?port->dev->name:"null");
	print_ipaddr(ntohl(group));
	if (ipv4_is_local_multicast(group))
		return 0;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED))
		goto out;

	mp = br_multicast_new_group(br, port, group);
	err = PTR_ERR(mp);
	if (unlikely(IS_ERR(mp) || !mp))
		goto err;

#ifdef MULTICAST_TO_UNITCAST
	gmember = is_group_member_existed(skb, mp);

	if(!gmember){
		gmember = br_multicast_new_group_member(mp, skb, port, group);
		if(!gmember)
			ERROR("%s: Fail to new_group_member.\n", __func__);	
	}
	else {
		if(!port) 
			ERROR("%s: *****update port error: NULL port\n", __func__);
		else{
			if(gmember->port != port){//port changed, delete old and update 
				struct net_bridge_port_group *pg;
				unsigned long time = now + br->multicast_last_member_count *
		    				 br->multicast_last_member_interval;

				DEBUG("%s:---port changed, delete old port and update.\n", __func__);

				for (pg = mp->ports; pg; pg = pg->next) {
					if (pg->port != gmember->port)
						continue;

					if (!hlist_unhashed(&pg->mglist) &&
					    (timer_pending(&pg->timer) ?
					     time_after(pg->timer.expires, time) :
					     try_to_del_timer_sync(&pg->timer) >= 0)) {
						mod_timer(&pg->timer, time);//will expire

						pg->queries_sent = 0;
						mod_timer(&pg->query_timer, now);//send query immediately
					}

					break;
				}
				
				//update port
				gmember->port = port;
			}
		}
			
		mod_timer(&gmember->timer, now + br->multicast_membership_interval-5*HZ);
	}
	
#endif
	//deal with net_bridge_port
	if (!port) {
		hlist_add_head(&mp->mglist, &br->mglist);
		mod_timer(&mp->timer, now + br->multicast_membership_interval);
		goto out;
	}

	for (pp = &mp->ports; (p = *pp); pp = &p->next) {
		if (p->port == port)
			goto found;
		if ((unsigned long)p->port < (unsigned long)port)
			break;
	}
	//new a port_group
	p = kzalloc(sizeof(*p), GFP_ATOMIC);
	err = -ENOMEM;
	if (unlikely(!p))
		goto err;

	//p->addr = group;
	p->port = port;
	p->next = *pp;
	hlist_add_head(&p->mglist, &port->mglist);//add port_group into the port
	setup_timer(&p->timer, br_multicast_port_group_expired,
		    (unsigned long)p);
	setup_timer(&p->query_timer, br_multicast_port_group_query_expired,
		    (unsigned long)p);

	rcu_assign_pointer(*pp, p);

found:
	mod_timer(&p->timer, now + br->multicast_membership_interval);
out:
	err = 0;

err:
	spin_unlock(&br->multicast_lock);
	return err;
}

static void br_multicast_leave_group(struct sk_buff *skb, struct net_bridge *br,
				     struct net_bridge_port *port,
				     __be32 group)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	unsigned long now;
	unsigned long time;
	int cnt=0;
	
	DEBUG("%s: from port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	print_ipaddr(ntohl(group));

	if (ipv4_is_local_multicast(group))
		return;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED) ||
	    timer_pending(&br->multicast_querier_timer))
		goto out;

	mdb = br->mdb;
	mp = br_mdb_ip_get(mdb, group);
	if (!mp)
		goto out;

#ifdef MULTICAST_TO_UNITCAST
	cnt = br_multicast_del_group_member(skb, mp);
#endif

	now = jiffies;
	time = now + br->multicast_last_member_count *
		     br->multicast_last_member_interval;

	if (!port) {
		if (!hlist_unhashed(&mp->mglist) &&
		    (timer_pending(&mp->timer) ?
		     time_after(mp->timer.expires, time) :
		     try_to_del_timer_sync(&mp->timer) >= 0)) {
			mod_timer(&mp->timer, time);

			mp->queries_sent = 0;
			mod_timer(&mp->query_timer, now);
		}

		goto out;
	}

	for (p = mp->ports; p; p = p->next) {
		if (p->port != port)
			continue;

		if (!hlist_unhashed(&p->mglist) &&
		    (timer_pending(&p->timer) ?
		     time_after(p->timer.expires, time) :
		     try_to_del_timer_sync(&p->timer) >= 0)) {
			mod_timer(&p->timer, time);//will expire

			p->queries_sent = 0;
			mod_timer(&p->query_timer, now);//send query immediately
		}

		break;
	}

out:
	spin_unlock(&br->multicast_lock);
}

static void br_multicast_router_expired(unsigned long data)
{
	struct net_bridge_port *port = (void *)data;
	struct net_bridge *br = port->br;

	spin_lock(&br->multicast_lock);
	if (port->multicast_router != 1 ||
	    timer_pending(&port->multicast_router_timer) ||
	    hlist_unhashed(&port->rlist))
		goto out;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	hlist_del_init_rcu(&port->rlist);
#else
	hlist_del_init(&port->rlist);
#endif
out:
	spin_unlock(&br->multicast_lock);
}

static void br_multicast_local_router_expired(unsigned long data)
{
}
//create and send IGMP query
static void br_multicast_send_query(struct net_bridge *br,
				    struct net_bridge_port *port, u32 sent)
{
	unsigned long time;
	struct sk_buff *skb;

	/*DEBUG("%s: to port name=%s, sent=%d\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null", sent);*/
	if (!netif_running(br->dev) || is_br_multicast_all_disabled(br)|| 
	    timer_pending(&br->multicast_querier_timer))
		return;

	skb = br_multicast_alloc_query(br, 0);
	if (!skb)
		goto timer;

	if (port) {//send query
		__skb_push(skb, sizeof(struct ethhdr));
		skb->dev = port->dev;
		NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
			dev_queue_xmit);//send from local
	} else
		netif_rx(skb);

timer:
	time = jiffies;
	time += sent < br->multicast_startup_query_count ?
		br->multicast_startup_query_interval :
		br->multicast_query_interval;
	mod_timer(port ? &port->multicast_query_timer :
			 &br->multicast_query_timer, time);
}

static void br_multicast_port_query_expired(unsigned long data)
{
	struct net_bridge_port *port = (void *)data;
	struct net_bridge *br = port->br;

	spin_lock(&br->multicast_lock);
	if (port->state == BR_STATE_DISABLED ||
	    port->state == BR_STATE_BLOCKING)
		goto out;

	if (port->multicast_startup_queries_sent <
	    br->multicast_startup_query_count)
		port->multicast_startup_queries_sent++;

	br_multicast_send_query(port->br, port,
				port->multicast_startup_queries_sent);

out:
	spin_unlock(&br->multicast_lock);
}
//bridge port like eth0, ath0 .etc
void br_multicast_add_port(struct net_bridge_port *port)
{
	port->multicast_router = 1;

	DEBUG("%s: from port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	setup_timer(&port->multicast_router_timer, br_multicast_router_expired,
		    (unsigned long)port);
	setup_timer(&port->multicast_query_timer,
		    br_multicast_port_query_expired, (unsigned long)port);
}

void br_multicast_del_port(struct net_bridge_port *port)
{
	DEBUG("%s: port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	del_timer_sync(&port->multicast_router_timer);
}

static void __br_multicast_enable_port(struct net_bridge_port *port)
{
	port->multicast_startup_queries_sent = 0;

	DEBUG("%s: port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	if (try_to_del_timer_sync(&port->multicast_query_timer) >= 0 ||
	    del_timer(&port->multicast_query_timer))
		mod_timer(&port->multicast_query_timer, jiffies);
}

void br_multicast_enable_port(struct net_bridge_port *port)
{
	struct net_bridge *br = port->br;

	DEBUG("%s: port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	spin_lock(&br->multicast_lock);
	if (is_br_multicast_all_disabled(br)||!netif_running(br->dev))
		goto out;

	__br_multicast_enable_port(port);

out:
	spin_unlock(&br->multicast_lock);
}

void br_multicast_disable_port(struct net_bridge_port *port)
{
	struct net_bridge *br = port->br;
	struct net_bridge_port_group *pg;
	struct hlist_node *p, *n;

	DEBUG("%s: port name=%s\n", __func__,  
		(port->dev&&port->dev->name)?port->dev->name:"null");
	spin_lock(&br->multicast_lock);
	hlist_for_each_entry_safe(pg, p, n, &port->mglist, mglist)
		br_multicast_del_pg(br, pg);

	if (!hlist_unhashed(&port->rlist))
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
		hlist_del_init_rcu(&port->rlist);
#else
		hlist_del_init(&port->rlist);
#endif
	del_timer(&port->multicast_router_timer);
	del_timer(&port->multicast_query_timer);
	spin_unlock(&br->multicast_lock);
}

static int br_multicast_igmp3_report(struct net_bridge *br,
				     struct net_bridge_port *port,
				     struct sk_buff *skb)
{
	struct igmpv3_report *ih;
	struct igmpv3_grec *grec;
	int i;
	int len;
	int num;
	int type;
	int err = 0;
	__be32 group;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	if (!pskb_may_pull(skb, sizeof(*ih)))
		return -EINVAL;

	ih = igmpv3_report_hdr(skb);
	num = ntohs(ih->ngrec);
	len = sizeof(*ih);

	for (i = 0; i < num; i++) {
		len += sizeof(*grec);
		if (!pskb_may_pull(skb, len))
			return -EINVAL;

		grec = (void *)(skb->data + len - sizeof(*grec));
		group = grec->grec_mca;
		type = grec->grec_type;

		len += ntohs(grec->grec_nsrcs) * 4;
		if (!pskb_may_pull(skb, len))
			return -EINVAL;

		/* We treat this as an IGMPv2 report for now. */
		err = 0;
		switch (type) {
		case IGMPV3_MODE_IS_INCLUDE:
		case IGMPV3_CHANGE_TO_INCLUDE:
			br_multicast_leave_group(skb, br, port, group);
			break;
		case IGMPV3_MODE_IS_EXCLUDE:
		case IGMPV3_ALLOW_NEW_SOURCES:
		case IGMPV3_BLOCK_OLD_SOURCES:
		case IGMPV3_CHANGE_TO_EXCLUDE:
			err = br_multicast_add_group(skb, br, port, group);
			break;

		default:
			continue;
		}

		if (err)
			break;
	}

	return err;
}
//add port into router_list
static void br_multicast_add_router(struct net_bridge *br,
				    struct net_bridge_port *port)
{
	struct hlist_node *p;
	struct hlist_node **h;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	for (h = &br->router_list.first;
	     (p = *h) &&
	     (unsigned long)container_of(p, struct net_bridge_port, rlist) >
	     (unsigned long)port;
	     h = &p->next)
		;

	port->rlist.pprev = h;
	port->rlist.next = p;
	rcu_assign_pointer(*h, &port->rlist);
	if (p)
		p->pprev = &port->rlist.next;
}

static void br_multicast_mark_router(struct net_bridge *br,
				     struct net_bridge_port *port)
{
	unsigned long now = jiffies;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	if (!port) {
		if (br->multicast_router == 1)
			mod_timer(&br->multicast_router_timer,
				  now + br->multicast_querier_interval);
		return;
	}

	if (port->multicast_router != 1)
		return;

	if (!hlist_unhashed(&port->rlist))
		goto timer;

	br_multicast_add_router(br, port);

timer:
	mod_timer(&port->multicast_router_timer,
		  now + br->multicast_querier_interval);
}
//receive a IGMP query from outside
static void br_multicast_query_received(struct net_bridge *br,
					struct net_bridge_port *port,
					__be32 saddr)
{
	DEBUG("%s: line=%d, saddr=%d\n", __func__, __LINE__, saddr);	
	if (saddr)
		mod_timer(&br->multicast_querier_timer,
			  jiffies + br->multicast_querier_interval);
	else if (timer_pending(&br->multicast_querier_timer))
		return;

	br_multicast_mark_router(br, port);
}
//handle the query: delay all query from us, because someone do it now
static int br_multicast_query(struct net_bridge *br,
			      struct net_bridge_port *port,
			      struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	struct igmphdr *ih = igmp_hdr(skb);
	struct net_bridge_mdb_entry *mp;
	struct igmpv3_query *ih3;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group **pp;
	unsigned long max_delay;
	unsigned long now = jiffies;
	__be32 group;
	int err = 0;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED))
		goto out;

	br_multicast_query_received(br, port, iph->saddr);

	group = ih->group;

	if (skb->len == sizeof(*ih)) {
		max_delay = ih->code * (HZ / IGMP_TIMER_SCALE);

		if (!max_delay) {
			max_delay = 10 * HZ;
			group = 0;
		}
	} else {
		if (!pskb_may_pull(skb, sizeof(struct igmpv3_query))) {
			err = -EINVAL;
			goto out;
		}

		ih3 = igmpv3_query_hdr(skb);
		if (ih3->nsrcs)
			goto out;

		max_delay = ih3->code ?
			    IGMPV3_MRC(ih3->code) * (HZ / IGMP_TIMER_SCALE) : 1;
	}

	if (!group)
		goto out;

	mp = br_mdb_ip_get(br->mdb, group);
	if (!mp)
		goto out;

	max_delay *= br->multicast_last_member_count;

	if (!hlist_unhashed(&mp->mglist) &&
	    (timer_pending(&mp->timer) ?
	     time_after(mp->timer.expires, now + max_delay) :
	     try_to_del_timer_sync(&mp->timer) >= 0))
		mod_timer(&mp->timer, now + max_delay);

	for (pp = &mp->ports; (p = *pp); pp = &p->next) {
		if (timer_pending(&p->timer) ?
		    time_after(p->timer.expires, now + max_delay) :
		    try_to_del_timer_sync(&p->timer) >= 0)
			mod_timer(&mp->timer, now + max_delay);
	}

out:
	spin_unlock(&br->multicast_lock);
	return err;
}
//recv ipv4 package
static int br_multicast_ipv4_rcv(struct net_bridge *br,
				 struct net_bridge_port *port,
				 struct sk_buff *skb)
{
	struct sk_buff *skb2 = skb;
	struct iphdr *iph;
	struct igmphdr *ih;
	unsigned len;
	unsigned offset;
	int err;
	
	/* We treat OOM as packet loss for now. */
	if (!pskb_may_pull(skb, sizeof(*iph)))
		return -EINVAL;

	iph = ip_hdr(skb);

	if (iph->ihl < 5 || iph->version != 4)
		return -EINVAL;

	if (!pskb_may_pull(skb, ip_hdrlen(skb)))
		return -EINVAL;

	iph = ip_hdr(skb);

	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		return -EINVAL;

	if (iph->protocol != IPPROTO_IGMP) {
		return 0;
	}
	DEBUG("%s:IGMP from host=", __func__);	
	print_ipaddr(ntohl(iph->saddr));

	len = ntohs(iph->tot_len);
	if (skb->len < len || len < ip_hdrlen(skb))
		return -EINVAL;

	if (skb->len > len) {
		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (!skb2)
			return -ENOMEM;

		err = pskb_trim_rcsum(skb2, len);
		if (err)
			goto err_out;
	}
	len -= ip_hdrlen(skb2);
	offset = skb_network_offset(skb2) + ip_hdrlen(skb2);
	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);

	err = -EINVAL;
	if (!pskb_may_pull(skb2, sizeof(*ih)))
		goto out;

	iph = ip_hdr(skb2);

	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_fold(skb2->csum))
			break;
		/* fall through */
	case CHECKSUM_NONE:
		skb2->csum = 0;
		if (skb_checksum_complete(skb2))
			goto out;
	}

	err = 0;

	BR_INPUT_SKB_CB(skb)->igmp = 1;
	ih = igmp_hdr(skb2);

	switch (ih->type) {
	case IGMP_HOST_MEMBERSHIP_REPORT:
	case IGMPV2_HOST_MEMBERSHIP_REPORT:
		//BR_INPUT_SKB_CB(skb2)->mrouters_only = 1;//this will affect report from madwifi
		err = br_multicast_add_group(skb, br, port, ih->group);
		break;
	case IGMPV3_HOST_MEMBERSHIP_REPORT:
		err = br_multicast_igmp3_report(br, port, skb2);
		break;
	case IGMP_HOST_MEMBERSHIP_QUERY:
		err = br_multicast_query(br, port, skb2);
		break;
	case IGMP_HOST_LEAVE_MESSAGE:
		br_multicast_leave_group(skb, br, port, ih->group);
		break;
	default:
		DEBUG("%s: default, ih->type=%d\n", __func__, ih->type);	
		break;
	}

out:
	__skb_push(skb2, offset);
err_out:
	if (skb2 != skb)
		kfree_skb(skb2);
	return err;
}
//recv multicast package
int br_multicast_rcv(struct net_bridge *br, struct net_bridge_port *port,
		     struct sk_buff *skb)
{
	BR_INPUT_SKB_CB(skb)->igmp = 0;
	BR_INPUT_SKB_CB(skb)->mrouters_only = 0;

	if (is_br_multicast_all_disabled(br))
		return 0;
	
	if(!port){
		DEBUG("%s: port is null\n", __func__);
		return 0;
	}
	
	switch (skb->protocol) {
	case htons(ETH_P_IP):
		return br_multicast_ipv4_rcv(br, port, skb);
	}

	return 0;
}
//it's time to query
static void br_multicast_query_expired(unsigned long data)
{
	struct net_bridge *br = (void *)data;

	spin_lock(&br->multicast_lock);
	if (br->multicast_startup_queries_sent <
	    br->multicast_startup_query_count)
		br->multicast_startup_queries_sent++;

	br_multicast_send_query(br, NULL, br->multicast_startup_queries_sent);

	spin_unlock(&br->multicast_lock);
}

void br_multicast_init(struct net_bridge *br)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	br->hash_elasticity = 4;
	br->hash_max = 512;

	br->multicast_router = 1;
	br->multicast_last_member_count = 2;
	br->multicast_startup_query_count = 2;

	br->multicast_last_member_interval = HZ;
	br->multicast_query_response_interval = 10 * HZ;
	br->multicast_startup_query_interval = 125 * HZ / 4;
	br->multicast_query_interval = 125 * HZ;
	br->multicast_querier_interval = 255 * HZ;
	br->multicast_membership_interval = 260 * HZ;

	spin_lock_init(&br->multicast_lock);
	setup_timer(&br->multicast_router_timer,
		    br_multicast_local_router_expired, 0);
	setup_timer(&br->multicast_querier_timer,
		    br_multicast_local_router_expired, 0);
	setup_timer(&br->multicast_query_timer, br_multicast_query_expired,
		    (unsigned long)br);
}

void br_multicast_open(struct net_bridge *br)
{
	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	br->multicast_startup_queries_sent = 0;

	if (is_br_multicast_all_disabled(br))
		return;

	mod_timer(&br->multicast_query_timer, jiffies);
}

void br_multicast_stop(struct net_bridge *br)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p, *n;
	u32 ver;
	int i;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	del_timer_sync(&br->multicast_router_timer);
	del_timer_sync(&br->multicast_querier_timer);
	del_timer_sync(&br->multicast_query_timer);

	spin_lock_bh(&br->multicast_lock);
	mdb = br->mdb;
	if (!mdb)
		goto out;

	br->mdb = NULL;

	ver = mdb->ver;
	DEBUG("%s: ver=%d\n", __func__, ver);	
	
	for (i = 0; i < mdb->max; i++) {
		hlist_for_each_entry_safe(mp, p, n, &mdb->mhash[i],
					  hlist[ver]) {
			del_timer(&mp->timer);
			del_timer(&mp->query_timer);
			call_rcu_bh(&mp->rcu, br_multicast_free_group);
		}
	}

	if (mdb->old) {
		spin_unlock_bh(&br->multicast_lock);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
		rcu_barrier_bh();
#else
		rcu_barrier();
#endif	
		spin_lock_bh(&br->multicast_lock);
		WARN_ON(mdb->old);
	}

	mdb->old = mdb;
	call_rcu_bh(&mdb->rcu, br_mdb_free);

out:
	spin_unlock_bh(&br->multicast_lock);
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
int br_multicast_set_router(struct net_bridge *br, unsigned long val)
#else
void br_multicast_set_router(struct net_bridge *br, unsigned long val)
#endif	
{
	int err = -ENOENT;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	spin_lock_bh(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	switch (val) {
	case 0:
	case 2:
		del_timer(&br->multicast_router_timer);
		/* fall through */
	case 1:
		br->multicast_router = val;
		err = 0;
		break;

	default:
		err = -EINVAL;
		break;
	}

unlock:
	spin_unlock_bh(&br->multicast_lock);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return err;
#endif	
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
int br_multicast_set_port_router(struct net_bridge_port *p, unsigned long val)
#else	
void br_multicast_set_port_router(struct net_bridge_port *p, unsigned long val)
#endif	
{
	struct net_bridge *br = p->br;
	int err = -ENOENT;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || p->state == BR_STATE_DISABLED)
		goto unlock;

	switch (val) {
	case 0:
	case 1:
	case 2:
		p->multicast_router = val;
		err = 0;

		if (val < 2 && !hlist_unhashed(&p->rlist))
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
			hlist_del_init_rcu(&p->rlist);
#else
			hlist_del_init(&p->rlist);
#endif

		if (val == 1)
			break;

		del_timer(&p->multicast_router_timer);

		if (val == 0)
			break;

		br_multicast_add_router(br, p);
		break;

	default:
		err = -EINVAL;
		break;
	}

unlock:
	spin_unlock(&br->multicast_lock);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return err;
#endif
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
int br_multicast_toggle(struct net_bridge *br, unsigned long val)
#else
void br_multicast_toggle(struct net_bridge *br, unsigned long val)
#endif
{
	struct net_bridge_port *port;
	int err = -ENOENT;

	DEBUG("%s: val=%ld\n", __func__, val);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	err = 0;
	if (br->multicast_disabled == !val)
		goto unlock;

	br->multicast_disabled = !val;
	if (br->multicast_disabled)
		goto unlock;

	if (br->mdb) {
		if (br->mdb->old) {
			err = -EEXIST;
rollback:
			br->multicast_disabled = !!val;
			goto unlock;
		}

		err = br_mdb_rehash(&br->mdb, br->mdb->max,
				    br->hash_elasticity);
		if (err)
			goto rollback;
	}

	br_multicast_open(br);
	list_for_each_entry(port, &br->port_list, list) {
		if (port->state == BR_STATE_DISABLED ||
		    port->state == BR_STATE_BLOCKING)
			continue;

		__br_multicast_enable_port(port);
	}

unlock:
	spin_unlock(&br->multicast_lock);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return err;
#endif
}

#ifdef MULTICAST_TO_UNITCAST	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
int br_multicast_toggle_m2u(struct net_bridge *br, unsigned long val)
#else
void br_multicast_toggle_m2u(struct net_bridge *br, unsigned long val)
#endif
{
	struct net_bridge_port *port;
	int err = -ENOENT;

	DEBUG("%s: multicast_direct val=%ld\n", __func__, val);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	err = 0;
	if (br->m2u_disabled == !val)
		goto unlock;

	br->m2u_disabled = !val;
	if (br->m2u_disabled)
		goto unlock;

	br_multicast_open(br);
	list_for_each_entry(port, &br->port_list, list) {
		if (port->state == BR_STATE_DISABLED ||
		    port->state == BR_STATE_BLOCKING)
			continue;

		__br_multicast_enable_port(port);
	}

unlock:
	spin_unlock(&br->multicast_lock);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return err;
#endif
}
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
int br_multicast_set_hash_max(struct net_bridge *br, unsigned long val)
#else
void br_multicast_set_hash_max(struct net_bridge *br, unsigned long val)
#endif
{
	int err = -ENOENT;
	u32 old;

	DEBUG("%s: line=%d\n", __func__, __LINE__);	
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	err = -EINVAL;
	if (!is_power_of_2(val))
		goto unlock;
	if (br->mdb && val < br->mdb->size)
		goto unlock;

	err = 0;

	old = br->hash_max;
	br->hash_max = val;

	if (br->mdb) {
		if (br->mdb->old) {
			err = -EEXIST;
rollback:
			br->hash_max = old;
			goto unlock;
		}

		err = br_mdb_rehash(&br->mdb, br->hash_max,
				    br->hash_elasticity);
		if (err)
			goto rollback;
	}

unlock:
	spin_unlock(&br->multicast_lock);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return err;
#endif
}
