/*
 *  ebt_length
 *
 */
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/spinlock.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_limit.h>
#include <net/ip.h>
#include <linux/version.h>

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

struct ebt_length_info {
    __u16	min, max;
    __u8	invert;
};

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool
ebt_length_mt(const struct sk_buff *skb, const EBT_MATCH_PARAM *par)
#else
static int ebt_length_mt(const struct sk_buff *skb,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_length_info *info = par->matchinfo;
#else
	const struct ebt_length_info *info = data;
#endif

	u_int16_t pktlen = ntohs(ip_hdr(skb)->tot_len);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return (pktlen >= info->min && pktlen <= info->max) ^ info->invert;
#else
	return !(pktlen >= info->min && pktlen <= info->max) ^ info->invert;
#endif
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int  ebt_length_mt_check(const struct xt_mtchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool ebt_length_mt_check(const struct xt_mtchk_param *par)
#else
static int ebt_length_mt_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_length_info *info = par->matchinfo;
#else
	const struct ebt_length_info *info = data;
#endif

	return MY_TRUE;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static struct xt_match ebt_length_mt_reg __read_mostly = {
	.name		= "length",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_length_mt,
	.checkentry	= ebt_length_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_length_info)),
	.me		= THIS_MODULE,
};
#else
static struct ebt_match ebt_length_mt_reg __read_mostly = {
	.name		= "length",
	.match		= ebt_length_mt,
	.check		= ebt_length_mt_check,
	.me		= THIS_MODULE,
};
#endif

static int __init ebt_length_init(void)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return xt_register_match(&ebt_length_mt_reg);
#else
	return ebt_register_match(&ebt_length_mt_reg);
#endif
}

static void __exit ebt_length_fini(void)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_match(&ebt_length_mt_reg);
#else
	ebt_unregister_match(&ebt_length_mt_reg);
#endif
	
}

module_init(ebt_length_init);
module_exit(ebt_length_fini);
MODULE_DESCRIPTION("Ebtables: length match");
MODULE_LICENSE("GPL");
