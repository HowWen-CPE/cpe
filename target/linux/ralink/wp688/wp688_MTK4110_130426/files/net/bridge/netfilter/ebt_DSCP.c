/*
 *  ebt_tos
 */

/* The tos target can be used in any chain,
 */

#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>

#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/dsfield.h>
#include <linux/version.h>

/* The target member is reused for adding new actions, the
 * value of the real target is -1 to -NUM_STANDARD_TARGETS.
 * For backward compatibility, the 4 lsb (2 would be enough,
 * but let's play it safe) are kept to designate this target.
 * The remaining bits designate the action. By making the set
 * action 0xfffffff0, the result will look ok for older
 * versions. [September 2006] */
#define TOS_SET_VALUE (0xfffffff0)

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

struct ebt_tos_t_info
{
	unsigned long tos;
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define EBT_TOS_TARGET "tos"

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static unsigned int
ebt_tos_tg(struct sk_buff *skb, const EBT_TARGET_PARAM *par)
#else
static int ebt_tos_tg(struct sk_buff *skb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_tos_t_info *info = par->targinfo;
#else
	const struct ebt_tos_t_info *info = data;
#endif
	struct iphdr *iph = ip_hdr(skb);
	u_int8_t orig, nv;

	orig = ipv4_get_dsfield(iph);
	//printk("%s: orig=0x%x,  info->tos=0x%x\n", __func__, orig,  info->tos);
	nv   = (orig & ~info->tos) ^ info->tos;
	//printk("%s: orig=0x%x,  nv=0x%x\n", __func__, orig,  nv);

	if (orig != nv) {
		if (!skb_make_writable(skb, sizeof(struct iphdr)))
			return EBT_CONTINUE;
		iph = ip_hdr(skb);
		ipv4_change_dsfield(iph, 0, nv);
	}

	return info->target | ~EBT_VERDICT_BITS;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int ebt_tos_tg_check(const struct xt_tgchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool ebt_tos_tg_check(const struct xt_tgchk_param *par)
#else
static int ebt_tos_tg_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_tos_t_info *info = par->targinfo;
#else
	const struct ebt_tos_t_info *info = data;
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
static struct xt_target ebt_tos_tg_reg __read_mostly = {
	.name		= EBT_TOS_TARGET,
	.target		= ebt_tos_tg,
	.checkentry	= ebt_tos_tg_check,
	.me		= THIS_MODULE,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_tos_t_info)),
};
#else
static struct ebt_target ebt_tos_tg_reg __read_mostly = {
	.name		= EBT_TOS_TARGET,
	.target		= ebt_tos_tg,
	.check		= ebt_tos_tg_check,
	.me		= THIS_MODULE,
};
#endif

static int __init ebt_tos_init(void)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	return xt_register_target(&ebt_tos_tg_reg);
#else
	return ebt_register_target(&ebt_tos_tg_reg);
#endif
}

static void __exit ebt_tos_fini(void)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_target(&ebt_tos_tg_reg);
#else
	ebt_unregister_target(&ebt_tos_tg_reg);
#endif
}

module_init(ebt_tos_init);
module_exit(ebt_tos_fini);
MODULE_DESCRIPTION("Ebtables: Packet tos modification");
MODULE_LICENSE("GPL");
