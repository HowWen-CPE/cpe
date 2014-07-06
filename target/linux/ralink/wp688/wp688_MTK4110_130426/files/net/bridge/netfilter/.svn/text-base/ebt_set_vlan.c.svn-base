/*
 *  ebt_set_vlan
 *
 *	Authors:
 *	alan zhang
 * January, 2012
 *
 */

#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_vlan.h>
#include <linux/version.h>

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

struct ebt_set_vlan_t_info
{
	unsigned int vid;
	unsigned int vprio;
	__be16 vencap;
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define VLAN_SET_VID 	(0xfffffff0)
#define VLAN_SET_VPRIO  	(0xffffffe0)
#define VLAN_SET_VENCAP	(0xffffffd0)

#define DEBUG_SET_VLAN

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif
#ifdef DEBUG_SET_VLAN
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

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static unsigned int
ebt_set_vlan_tg(struct sk_buff *skb, const EBT_TARGET_PARAM *par)
#else
static int ebt_set_vlan_tg(struct sk_buff *skb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_set_vlan_t_info *info = par->targinfo;
#else
	const struct ebt_set_vlan_t_info *info = data;
#endif
	int action = info->target & -16;

	struct vlan_hdr *fp;
	struct vlan_hdr _frame;

	unsigned short TCI;	/* Whole TCI, given from parsed frame */
	unsigned short id;	/* VLAN ID, given from frame TCI */
	unsigned char prio;	/* user_priority, given from frame TCI */
	/* VLAN encapsulated Type/Length field, given from orig frame */
	__be16 encap;

	fp = skb_header_pointer(skb, 0, sizeof(_frame), &_frame);
	if (fp == NULL) {
		ERROR("%s: fail to get vlan header\n", __func__);
		return false;
	}
	/* Tag Control Information (TCI) consists of the following elements:
	 * - User_priority. The user_priority field is three bits in length,
	 * interpreted as a binary number.
	 * - Canonical Format Indicator (CFI). The Canonical Format Indicator
	 * (CFI) is a single bit flag value. Currently ignored.
	 * - VLAN Identifier (VID). The VID is encoded as
	 * an unsigned binary number. */
	TCI = ntohs(fp->h_vlan_TCI);
	id = TCI & VLAN_VID_MASK;
	prio = (TCI >> 13) & 0x7;
	encap = fp->h_vlan_encapsulated_proto;
#ifdef DEBUG_SET_VLAN
	DEBUG("%s: before set vlan param: id=%d, prio=%d,encap=%d\n", 
		__func__,id,prio,encap);

	DEBUG("%s: new param: info->vid=%d, vprio=%d,vencap=%d\n", 
		__func__,info->vid,info->vprio,info->vencap);
#endif

	if (action == VLAN_SET_VID){
		id = info->vid;
	}
	else if (action == VLAN_SET_VPRIO){
		prio |= info->vprio;//set or |
	}
	else if (action == VLAN_SET_VENCAP) {
		fp->h_vlan_encapsulated_proto = htons(info->vencap);
	}
	fp->h_vlan_TCI = htons(((prio & 0x7) << 13) |(id & VLAN_VID_MASK));
	
#ifdef DEBUG_SET_VLAN
	TCI = ntohs(fp->h_vlan_TCI);
	id = TCI & VLAN_VID_MASK;
	prio = (TCI >> 13) & 0x7;
	encap = fp->h_vlan_encapsulated_proto;
	DEBUG("%s: after set vlan param: id=%d, prio=%d,encap=%d\n", 
		__func__,id,prio,encap);
#endif

	return info->target | ~EBT_VERDICT_BITS;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int ebt_set_vlan_tg_check(const struct xt_tgchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool ebt_set_vlan_tg_check(const struct xt_tgchk_param *par)
#else
static int ebt_set_vlan_tg_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_set_vlan_t_info *info = par->targinfo;
#else
	const struct ebt_set_vlan_t_info *info = data;
#endif
	int tmp;

	DEBUG("%s: line=%d.\n", __func__, __LINE__);
	tmp = info->target | ~EBT_VERDICT_BITS;
	if (BASE_CHAIN && tmp == EBT_RETURN)
		return MY_FALSE;
	if (tmp < -NUM_STANDARD_TARGETS || tmp >= 0)
		return MY_FALSE;
	tmp = info->target & ~EBT_VERDICT_BITS;
	if (tmp != VLAN_SET_VID && tmp != VLAN_SET_VPRIO &&
	    tmp != VLAN_SET_VENCAP)
		return MY_FALSE;
	return MY_TRUE;
}

#define EBT_VLAN_SET_TARGET  "set_vlan"
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static struct xt_target ebt_set_vlan_tg_reg __read_mostly = {
	.name		= EBT_VLAN_SET_TARGET,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.target		= ebt_set_vlan_tg,
	.checkentry	= ebt_set_vlan_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_set_vlan_t_info)),
	.me		= THIS_MODULE,
};
#else
static struct ebt_target ebt_set_vlan_tg_reg __read_mostly = {
	.name		= EBT_VLAN_SET_TARGET,
	.target		= ebt_set_vlan_tg,
	.check		= ebt_set_vlan_tg_check,
	.me		= THIS_MODULE,
};
#endif

static int __init ebt_set_vlan_init(void)
{
	printk(KERN_ERR"%s: ...\n", __func__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return xt_register_target(&ebt_set_vlan_tg_reg);
#else
	return ebt_register_target(&ebt_set_vlan_tg_reg);
#endif
}

static void __exit ebt_set_vlan_fini(void)
{
	printk(KERN_ERR"%s: line=%d.\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_target(&ebt_set_vlan_tg_reg);
#else
	ebt_unregister_target(&ebt_set_vlan_tg_reg);
#endif
}

module_init(ebt_set_vlan_init);
module_exit(ebt_set_vlan_fini);
MODULE_DESCRIPTION("Ebtables: Packet set vlan modification");
MODULE_LICENSE("GPL");
