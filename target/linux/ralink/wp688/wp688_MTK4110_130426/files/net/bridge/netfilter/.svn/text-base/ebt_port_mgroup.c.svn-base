/*
 *  ebt_port_mgroup
 *
 */
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/spinlock.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <net/ip.h>
#include <linux/version.h>

#include "../br_private.h"

struct ebt_port_mgroup_info {
    u_int16_t	num;
    u_int8_t	invert;
};

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
#define MATCHED			1
#define UNMATCHED		0

#define MY_TRUE		true
#define MY_FALSE	false	
#else
#define MATCHED			0
#define UNMATCHED		1

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


#define DEBUG_PORT_MGROUP

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif
#ifdef DEBUG_PORT_MGROUP
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
extern struct net_bridge_mdb_entry *br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, __be32 dst);


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool
ebt_port_mgroup_mt(const struct sk_buff *skb, const EBT_MATCH_PARAM *par)
#else
static int ebt_port_mgroup_mt(const struct sk_buff *skb,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
#ifdef MULTICAST_TO_UNITCAST
	const unsigned char *dest;
	struct net_bridge_port *port;
	struct net_bridge *br;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_port_mgroup_info *info = par->matchinfo;
#else
	const struct ebt_port_mgroup_info *info = data;
#endif

	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct iphdr *iph;
	
	struct net_group_member *gmember;
	struct hlist_node *pnode;
	
	int ret = UNMATCHED;
	unsigned gmemcnt=0;
		
	//DEBUG("%s: info num=%d, info->invert=%d\n", __func__, info->num, info->invert);
	//check multicast package
	dest = eth_hdr(skb)->h_dest;
	if (!is_multicast_ether_addr(dest)) {
		ERROR("%s: is not a multicast package\n", __func__);
		return UNMATCHED;
	}
	
	//check skb valiable
	if (!skb|| !skb->dev || !netif_running(skb->dev)){
		ERROR("%s: !skb|| skb->dev is NULL or is not running\n", __func__);
		return UNMATCHED;
	}

	//port = rcu_dereference(skb->dev->br_port);
	port=NULL;
	if((port == NULL) || (port->state == BR_STATE_DISABLED)) {
		ERROR("%s: net_bridge_port is NULL or BR_STATE_DISABLED\n", __func__);
		return UNMATCHED;
	}
	br = port->br;
	if (!br || !br->dev || !netif_running(br->dev)){
		ERROR("%s: br->dev is NULL or is not running\n", __func__);
		return UNMATCHED;
	}

	spin_lock(&br->multicast_lock);

	iph = ip_hdr(skb);
	
	mdb = br->mdb;
	mp = br_mdb_ip_get(mdb, iph->daddr);
	if (!mp) {
		ret = UNMATCHED;
		goto out;
	}

	//hlist_for_each_entry_rcu to avoid system crash, don't use hlist_for_each_entry
	hlist_for_each_entry_rcu(gmember, pnode, &mp->gmemlist, gmemlist) {
		int gmember_port_no = (gmember->port)?gmember->port->port_no:-1;
			
		if(gmember_port_no == port->port_no){
			gmemcnt++;
		}
			
	}
		
	if(gmemcnt > info->num){
		/*ERROR("%s: ifdev(%s) gmemcnt(%d) > %d.\n", 
			__func__, (port->dev && port->dev->name)?port->dev->name:"null", 
			gmemcnt, info->num);*/
		ret = MATCHED;
	}
		
out:
	spin_unlock(&br->multicast_lock);

	return (ret) ^ info->invert;
#else
	return UNMATCHED;
#endif
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int ebt_port_mgroup_mt_check(const struct xt_mtchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool ebt_port_mgroup_mt_check(const struct xt_mtchk_param *par)
#else
static int ebt_port_mgroup_mt_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_tos_t_info *info = par->matchinfo;
#else
	const struct ebt_tos_t_info *info = data;
#endif
	//struct ebt_port_mgroup_info *info = par->matchinfo;

	return MY_TRUE;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static struct xt_match ebt_port_mgroup_mt_reg __read_mostly = {
	.name		= "port-mgroup-gt",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_port_mgroup_mt,
	.checkentry	= ebt_port_mgroup_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_port_mgroup_info)),
	.me		= THIS_MODULE,
};
#else
static struct ebt_match ebt_port_mgroup_mt_reg __read_mostly = {
	.name		= "port-mgroup-gt",
	.match		= ebt_port_mgroup_mt,
	.check		= ebt_port_mgroup_mt_check,
	.me		= THIS_MODULE,
};
#endif

static int __init ebt_port_mgroup_init(void)
{
	printk("%s: line=%d\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return xt_register_match(&ebt_port_mgroup_mt_reg);
#else
	return ebt_register_match(&ebt_port_mgroup_mt_reg);
#endif
}

static void __exit ebt_port_mgroup_fini(void)
{
	printk("%s: line=%d\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_match(&ebt_port_mgroup_mt_reg);
#else
	ebt_unregister_match(&ebt_port_mgroup_mt_reg);
#endif
}

module_init(ebt_port_mgroup_init);
module_exit(ebt_port_mgroup_fini);
MODULE_DESCRIPTION("Ebtables: Rate-port-mgroup match");
MODULE_LICENSE("GPL");
