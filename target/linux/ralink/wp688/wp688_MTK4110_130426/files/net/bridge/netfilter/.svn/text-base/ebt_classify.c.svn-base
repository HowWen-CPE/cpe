/*
 *	ebt_classify - Netfilter module to classify stream
 */
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/jhash.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/mm.h>
#include <linux/in.h>

#include <linux/ktime.h>

#include <linux/ip.h>
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
#include <linux/ipv6.h>
#include <net/ipv6.h>
#endif

#include <net/net_namespace.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/mutex.h>
#include <linux/version.h>

#include "ebt_classify.h"

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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alan");
MODULE_DESCRIPTION("Ebtables: classify match");
MODULE_ALIAS("ebt_classify");
MODULE_ALIAS("eb6t_classify");

#define EBT_CLASSIFY_MATCH_NAME  "classify"

/* need to declare this at the top */
static struct proc_dir_entry *classify_procdir4;
static struct proc_dir_entry *classify_procdir6;
static const struct file_operations dl_file_ops;

struct mac_addr
{
	unsigned char	addr[6];
};

/* hash table crap */
struct dsthash_dst {
	union {
		struct {
			__be32 src;
			__be32 dst;
		} ip;
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
		struct {
			__be32 src[4];
			__be32 dst[4];
		} ip6;
#endif
	};
	__be16 src_port;
	__be16 dst_port;
	struct mac_addr smac;
	struct mac_addr dmac;
};

struct dsthash_ent {
	/* static / read-only parts in the beginning */
	struct hlist_node node;
	struct dsthash_dst dst;

	/* modified structure members in the end */
	unsigned long expires;		/* precalculated expiry time */
	struct {
		unsigned long prev;	/* last modification */
		u_int32_t credit;
		u_int32_t credit_cap, cost;
	} rateinfo;
	//add by alan
	struct stream_info stream;
};

struct ebt_classify_htable {
	struct hlist_node node;		/* global list of all htables */
	atomic_t use;
	u_int8_t family;

	struct classify_cfg1 cfg;	/* config */

	/* used internally */
	spinlock_t lock;		/* lock for list_head */
	u_int32_t rnd;			/* random seed for hash */
	int rnd_initialized;
	unsigned int count;		/* number entries in table */
	struct timer_list timer;	/* timer for gc */

	/* seq_file stuff */
	struct proc_dir_entry *pde;

	struct hlist_head hash[0];	/* hashtable itself */
};

static DEFINE_SPINLOCK(classify_lock);	/* protects htables list */
static DEFINE_MUTEX(hlimit_mutex);	/* additional checkentry protection */
static HLIST_HEAD(classify_htables);
static struct kmem_cache *classify_cachep __read_mostly;

#define DEBUG_CLASSIFY

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif
#ifdef DEBUG_CLASSIFY
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

static void print_ipaddr(int ipaddr)
{
	//DEBUG("print_ipaddr=0x%x\n", __func__, ipaddr);
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

static void print_skb(const struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	struct ethhdr *eth = eth_hdr(skb);
	
	//===printk ip addr	
	DEBUG("\t iph->saddr=");	
	print_ipaddr(iph->saddr);
	DEBUG("\t iph->daddr=");	
	print_ipaddr(iph->daddr);
	DEBUG("\t iph->id=%u", iph->id);	

	//===printk mac addr	
	DEBUG("\t eth->h_source=");	
	print_macaddr(eth->h_source);
	DEBUG("\t eth->h_dest=");	
	print_macaddr(eth->h_dest);
}

static void print_dst(struct dsthash_dst *dst)
{
	//===printk mac addr	
	DEBUG("\t dst->smac=");	
	print_macaddr(&dst->smac);
	DEBUG("\t dst->dmac=");	
	print_macaddr(&dst->dmac);

	//===printk ip addr	
	DEBUG("\t iph->saddr=");	
	print_ipaddr(dst->ip.src);
	DEBUG("\t iph->daddr=");	
	print_ipaddr(dst->ip.dst);

	//===printk mac addr	
	DEBUG("\t sport=%d", dst->src_port);	
	DEBUG("\t dport=%d", dst->dst_port);	
}

static void print_stream(struct stream_info*sinfo)
{
	DEBUG("%s: ...\n", __func__);	
	DEBUG("\t package_cnt=%ld", sinfo->package_cnt);	
	DEBUG("\t octet_cnt=%ld", sinfo->octet_cnt);	
	DEBUG("\t pack_interval_cnt=%ld", sinfo->pack_interval_cnt);	
	DEBUG("\t ip id=%d", sinfo->ip_id);	
	
	DEBUG("\t last_matched=%d\n", sinfo->last_matched);	
	DEBUG("\t not_classify=%d\n", sinfo->not_classify);	

	//DEBUG("\n");	
}

static void print_ent(struct dsthash_ent *dh)
{
	DEBUG("%s: ...\n", __func__);	
	print_dst(&dh->dst);
	print_stream(&dh->stream);	
	//DEBUG("\n");	
}

static void dump_parameters(const struct ebt_classify_info *r)
{
	printk(KERN_ERR "%s: .\n", __func__);
	
	printk(KERN_ERR "\t r->name=%s.\n", r->name);
	printk(KERN_ERR "\t r->cfg.mode=%d.\n", r->cfg.mode);
	printk(KERN_ERR "\t r->cfg.size=%d.\n", r->cfg.size);
	printk(KERN_ERR "\t r->cfg.max=%d.\n", r->cfg.max);
	printk(KERN_ERR "\t r->cfg.gc_interval=%d.\n", r->cfg.gc_interval);
	printk(KERN_ERR "\t r->cfg.expire=%d.\n", r->cfg.expire);
	printk(KERN_ERR "\t r->cfg.min_avg_plen=%d.\n", r->cfg.min_avg_plen);
	printk(KERN_ERR "\t r->cfg.max_avg_plen=%d.\n", r->cfg.max_avg_plen);
	
	printk(KERN_ERR "\t r->cfg.min_pack_interval=%d.\n", r->cfg.min_pack_interval);
	printk(KERN_ERR "\t r->cfg.max_pack_interval=%d.\n", r->cfg.max_pack_interval);
	printk(KERN_ERR "\t r->cfg.dur_cla_octet=%d.\n", r->cfg.dur_cla_octet);
	printk(KERN_ERR "\t r->cfg.bet_cla_octet=%d.\n", r->cfg.bet_cla_octet);
}

static void init_stream(unsigned long now, struct stream_info *stream)
{
	memset(stream, 0, sizeof(struct stream_info));
	stream->prev = now;
}

static int update_stream(const struct sk_buff *skb, 
	unsigned long now, struct stream_info *sinfo)
{
	struct iphdr* iphdr = ip_hdr(skb);
	
	DEBUG("%s: ...\n", __func__);
	//update statistic infomation
	if(1) {//(sinfo->ip_id != iphdr->id)
		sinfo->package_cnt++;
		sinfo->octet_cnt +=ntohs(iphdr->tot_len)+sizeof(struct ethhdr);
		sinfo->pack_interval_cnt += jiffies_to_msecs(now - sinfo->prev);
		
		sinfo->prev = now;
		sinfo->ip_id = iphdr->id;
	}
	return 1;
}

static int clear_stream_info(struct stream_info*sinfo)
{
	sinfo->octet_cnt = sinfo->package_cnt = sinfo->pack_interval_cnt = 0;
	sinfo->not_classify = 0;
	//ent->last_matched = 0;//Don't set this, it only be changed by algorithm
	return 0;
}

static int classify_dst_stream(struct stream_info *sinfo, 
	const struct classify_cfg *config)
{
	unsigned long avg_plen, avh_pack_interval;
	int matched=0;

	avg_plen = sinfo->octet_cnt / sinfo->package_cnt;
	avh_pack_interval = sinfo->pack_interval_cnt / sinfo->package_cnt;

	DEBUG("%s: avg_plen=%ld, avh_pack_interval=%ld\n", 
		__func__, sinfo->avg_plen, sinfo->avg_pinterval);	

	if(avg_plen >= config->min_avg_plen &&
		(avg_plen <= config->max_avg_plen)) {
		DEBUG("\t avg_plen matched\n");	
		matched ++;
	}
	
	if(avh_pack_interval >= config->min_pack_interval &&
		(avh_pack_interval <= config->max_pack_interval)) {
		DEBUG("\t avh_pack_interval matched\n");	
		matched ++;
	}
		
	sinfo->avg_plen = avg_plen;
	sinfo->avg_pinterval= avh_pack_interval;

	return (matched == 2)?1:0;
}

static int classify_algo_daemon(struct dsthash_ent *dst, 
	const struct classify_cfg *config)
{
	unsigned long max_period = config->bet_cla_octet; 

	DEBUG("%s: dur_cla_octet=%d\n", __func__, config->dur_cla_octet);	
	DEBUG("\t max_period=%ld\n", max_period);	
	
	if(dst->stream.octet_cnt < config->dur_cla_octet)
		return dst->stream.last_matched;
	
	if(dst->stream.octet_cnt > config->dur_cla_octet &&
		(dst->stream.octet_cnt < max_period) && !dst->stream.not_classify){
		//should only once here
		dst->stream.last_matched = classify_dst_stream(&dst->stream, config);
		dst->stream.not_classify=1;
		if(!dst->stream.last_matched) {
			DEBUG("\t this time didn't matched, restart matching\n");
			clear_stream_info(&dst->stream);
		}
		else {
			DEBUG("%pI4:%u->%pI4:%u --matched -return TRUE--\n",
				 &dst->dst.ip.src,
				 ntohs(dst->dst.src_port),
				 &dst->dst.ip.dst,
				 ntohs(dst->dst.dst_port));
			print_stream(&dst->stream);
		}

		return dst->stream.last_matched;
	}
	//wait another period
	if(dst->stream.octet_cnt > max_period){
		DEBUG("\t new period will start, clear all old logs\n");	
		clear_stream_info(&dst->stream);
	}
	
	return dst->stream.last_matched;
}

static inline bool dst_cmp(const struct dsthash_ent *ent,
			   const struct dsthash_dst *b)
{
	return !memcmp(&ent->dst, b, sizeof(ent->dst));
}

static u_int32_t
hash_dst(const struct ebt_classify_htable *ht, const struct dsthash_dst *dst)
{
	u_int32_t hash = jhash2((const u32 *)dst,
				sizeof(*dst)/sizeof(u32),
				ht->rnd);
	/*
	 * Instead of returning hash % ht->cfg.size (implying a divide)
	 * we return the high 32 bits of the (hash * ht->cfg.size) that will
	 * give results between [0 and cfg.size-1] and same hash distribution,
	 * but using a multiply, less expensive than a divide
	 */
	return ((u64)hash * ht->cfg.size) >> 32;
}

static struct dsthash_ent *
dsthash_find(const struct ebt_classify_htable *ht,
	     const struct dsthash_dst *dst)
{
	struct dsthash_ent *ent;
	struct hlist_node *pos;
	u_int32_t hash = hash_dst(ht, dst);

	if (!hlist_empty(&ht->hash[hash])) {
		hlist_for_each_entry(ent, pos, &ht->hash[hash], node) {
			if (dst_cmp(ent, dst)) {
				//DEBUG("%s: find a dst\n", __func__);	
				//print_ent(ent);
				return ent;
			}
		}
	}
	return NULL;
}

/* allocate dsthash_ent, initialize dst, put in htable and lock it */
static struct dsthash_ent *
dsthash_alloc_init(struct ebt_classify_htable *ht,
		   const struct dsthash_dst *dst)
{
	struct dsthash_ent *ent;

	/* initialize hash with random val at the time we allocate
	 * the first hashtable entry */
	if (!ht->rnd_initialized) {
		get_random_bytes(&ht->rnd, sizeof(ht->rnd));
		ht->rnd_initialized = 1;
	}

	if (ht->cfg.max && ht->count >= ht->cfg.max) {
		/* FIXME: do something. question is what.. */
		if (net_ratelimit())
			printk(KERN_WARNING
				"ebt_classify: max count of %u reached\n",
				ht->cfg.max);
		return NULL;
	}

	ent = kmem_cache_alloc(classify_cachep, GFP_ATOMIC);
	if (!ent) {
		if (net_ratelimit())
			printk(KERN_ERR
				"ebt_classify: can't allocate dsthash_ent\n");
		return NULL;
	}
	memcpy(&ent->dst, dst, sizeof(ent->dst));

	hlist_add_head(&ent->node, &ht->hash[hash_dst(ht, dst)]);
	ht->count++;
	return ent;
}

static inline void
dsthash_free(struct ebt_classify_htable *ht, struct dsthash_ent *ent)
{
	hlist_del(&ent->node);
	kmem_cache_free(classify_cachep, ent);
	ht->count--;
}
static void htable_gc(unsigned long htlong);

static int htable_create_v0(struct ebt_classify_info *minfo, u_int8_t family)
{
	struct ebt_classify_htable *hinfo;
	unsigned int size;
	unsigned int i;

	DEBUG("%s: family=%d...\n", __func__, family);	
	if (minfo->cfg.size)
		size = minfo->cfg.size;
	else {
		size = ((num_physpages << PAGE_SHIFT) / 16384) /
		       sizeof(struct list_head);
		if (num_physpages > (1024 * 1024 * 1024 / PAGE_SIZE))
			size = 8192;
		if (size < 16)
			size = 16;
	}
	/* FIXME: don't use vmalloc() here or anywhere else -HW */
	hinfo = vmalloc(sizeof(struct ebt_classify_htable) +
			sizeof(struct list_head) * size);
	if (!hinfo) {
		printk(KERN_ERR "ebt_classify: unable to create hashtable\n");
		return -1;
	}
	minfo->hinfo = hinfo;

	/* copy match config into hashtable config */
	hinfo->cfg.mode        = minfo->cfg.mode;
	hinfo->cfg.avg         = minfo->cfg.avg;
	hinfo->cfg.burst       = minfo->cfg.burst;
	hinfo->cfg.max         = minfo->cfg.max;
	hinfo->cfg.gc_interval = minfo->cfg.gc_interval;
	hinfo->cfg.expire      = minfo->cfg.expire;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	if ((family == NFPROTO_IPV4) ||(family == NFPROTO_BRIDGE))
		hinfo->cfg.srcmask = hinfo->cfg.dstmask = 32;
	else
		hinfo->cfg.srcmask = hinfo->cfg.dstmask = 128;
#else
	hinfo->cfg.srcmask = hinfo->cfg.dstmask = 32;
#endif
	hinfo->cfg.size = size;
	if (!hinfo->cfg.max)
		hinfo->cfg.max = 8 * hinfo->cfg.size;
	else if (hinfo->cfg.max < hinfo->cfg.size)
		hinfo->cfg.max = hinfo->cfg.size;

	for (i = 0; i < hinfo->cfg.size; i++)
		INIT_HLIST_HEAD(&hinfo->hash[i]);

	atomic_set(&hinfo->use, 1);
	hinfo->count = 0;
	hinfo->family = family;
	hinfo->rnd_initialized = 0;
	spin_lock_init(&hinfo->lock);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	hinfo->pde = proc_create_data(minfo->name, 0,
		(family == NFPROTO_IPV4)||(family == NFPROTO_BRIDGE) ?
		classify_procdir4 : classify_procdir6,
		&dl_file_ops, hinfo);
#else
	hinfo->pde = proc_create_data(minfo->name, 0,
		classify_procdir4,
		&dl_file_ops, hinfo);
#endif
	if (!hinfo->pde) {
		vfree(hinfo);
		return -1;
	}

	setup_timer(&hinfo->timer, htable_gc, (unsigned long )hinfo);
	hinfo->timer.expires = jiffies + msecs_to_jiffies(hinfo->cfg.gc_interval);
	add_timer(&hinfo->timer);

	spin_lock_bh(&classify_lock);
	hlist_add_head(&hinfo->node, &classify_htables);
	spin_unlock_bh(&classify_lock);

	return 0;
}

static bool select_all(const struct ebt_classify_htable *ht,
		       const struct dsthash_ent *he)
{
	return 1;
}

static bool select_gc(const struct ebt_classify_htable *ht,
		      const struct dsthash_ent *he)
{
	return time_after_eq(jiffies, he->expires);
}

static void htable_selective_cleanup(struct ebt_classify_htable *ht,
			bool (*select)(const struct ebt_classify_htable *ht,
				      const struct dsthash_ent *he))
{
	unsigned int i;

	/* lock hash table and iterate over it */
	spin_lock_bh(&ht->lock);
	for (i = 0; i < ht->cfg.size; i++) {
		struct dsthash_ent *dh;
		struct hlist_node *pos, *n;
		hlist_for_each_entry_safe(dh, pos, n, &ht->hash[i], node) {
			if ((*select)(ht, dh))
				dsthash_free(ht, dh);
		}
	}
	spin_unlock_bh(&ht->lock);
}

/* hash table garbage collector, run by timer */
static void htable_gc(unsigned long htlong)
{
	struct ebt_classify_htable *ht = (struct ebt_classify_htable *)htlong;

	htable_selective_cleanup(ht, select_gc);

	/* re-add the timer accordingly */
	ht->timer.expires = jiffies + msecs_to_jiffies(ht->cfg.gc_interval);
	add_timer(&ht->timer);
}

static void htable_destroy(struct ebt_classify_htable *hinfo)
{
	del_timer_sync(&hinfo->timer);

	/* remove proc entry */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	remove_proc_entry(hinfo->pde->name,
			  (hinfo->family == NFPROTO_IPV4)||
			  (hinfo->family == NFPROTO_BRIDGE) ? classify_procdir4 :
						     classify_procdir6);
#else
	remove_proc_entry(hinfo->pde->name,classify_procdir4);
#endif
	htable_selective_cleanup(hinfo, select_all);
	vfree(hinfo);
}

static struct ebt_classify_htable *htable_find_get(const char *name,
						   u_int8_t family)
{
	struct ebt_classify_htable *hinfo;
	struct hlist_node *pos;

	spin_lock_bh(&classify_lock);
	hlist_for_each_entry(hinfo, pos, &classify_htables, node) {
		if (!strcmp(name, hinfo->pde->name) &&
		    hinfo->family == family) {
			atomic_inc(&hinfo->use);
			spin_unlock_bh(&classify_lock);
			return hinfo;
		}
	}
	spin_unlock_bh(&classify_lock);
	return NULL;
}

static void htable_put(struct ebt_classify_htable *hinfo)
{
	if (atomic_dec_and_test(&hinfo->use)) {
		spin_lock_bh(&classify_lock);
		hlist_del(&hinfo->node);
		spin_unlock_bh(&classify_lock);
		htable_destroy(hinfo);
	}
}

/* The algorithm used is the Simple Token Bucket Filter (TBF)
 * see net/sched/sch_tbf.c in the linux source tree
 */

/* Rusty: This is my (non-mathematically-inclined) understanding of
   this algorithm.  The `average rate' in jiffies becomes your initial
   amount of credit `credit' and the most credit you can ever have
   `credit_cap'.  The `peak rate' becomes the cost of passing the
   test, `cost'.

   `prev' tracks the last packet hit: you gain one credit per jiffy.
   If you get credit balance more than this, the extra credit is
   discarded.  Every time the match passes, you lose `cost' credits;
   if you don't have that many, the test fails.

   See Alexey's formal explanation in net/sched/sch_tbf.c.

   To get the maximum range, we multiply by this factor (ie. you get N
   credits per jiffy).  We want to allow a rate as low as 1 per day
   (slowest userspace tool allows), which means
   CREDITS_PER_JIFFY*HZ*60*60*24 < 2^32 ie.
*/
#define MAX_CPJ (0xFFFFFFFF / (HZ*60*60*24))

/* Repeated shift and or gives us all 1s, final shift and add 1 gives
 * us the power of 2 below the theoretical max, so GCC simply does a
 * shift. */
#define _POW2_BELOW2(x) ((x)|((x)>>1))
#define _POW2_BELOW4(x) (_POW2_BELOW2(x)|_POW2_BELOW2((x)>>2))
#define _POW2_BELOW8(x) (_POW2_BELOW4(x)|_POW2_BELOW4((x)>>4))
#define _POW2_BELOW16(x) (_POW2_BELOW8(x)|_POW2_BELOW8((x)>>8))
#define _POW2_BELOW32(x) (_POW2_BELOW16(x)|_POW2_BELOW16((x)>>16))
#define POW2_BELOW32(x) ((_POW2_BELOW32(x)>>1) + 1)

#define CREDITS_PER_JIFFY POW2_BELOW32(MAX_CPJ)

/* Precision saver. */
static inline u_int32_t
user2credits(u_int32_t user)
{
	/* If multiplying would overflow... */
	if (user > 0xFFFFFFFF / (HZ*CREDITS_PER_JIFFY))
		/* Divide first. */
		return (user / EBT_CLASSIFY_SCALE) * HZ * CREDITS_PER_JIFFY;

	return (user * HZ * CREDITS_PER_JIFFY) / EBT_CLASSIFY_SCALE;
}

static inline void rateinfo_recalc(struct dsthash_ent *dh, unsigned long now)
{
	dh->rateinfo.credit += (now - dh->rateinfo.prev) * CREDITS_PER_JIFFY;
	if (dh->rateinfo.credit > dh->rateinfo.credit_cap)
		dh->rateinfo.credit = dh->rateinfo.credit_cap;
	dh->rateinfo.prev = now;
}

static inline __be32 maskl(__be32 a, unsigned int l)
{
	return l ? htonl(ntohl(a) & ~0 << (32 - l)) : 0;
}

#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
static void classify_ipv6_mask(__be32 *i, unsigned int p)
{
	switch (p) {
	case 0 ... 31:
		i[0] = maskl(i[0], p);
		i[1] = i[2] = i[3] = 0;
		break;
	case 32 ... 63:
		i[1] = maskl(i[1], p - 32);
		i[2] = i[3] = 0;
		break;
	case 64 ... 95:
		i[2] = maskl(i[2], p - 64);
		i[3] = 0;
	case 96 ... 127:
		i[3] = maskl(i[3], p - 96);
		break;
	case 128:
		break;
	}
}
#endif

static int
classify_init_dst(const struct ebt_classify_htable *hinfo,
		   struct dsthash_dst *dst,
		   const struct sk_buff *skb, unsigned int protoff)
{
	__be16 _ports[2], *ports;
	u8 nexthdr;
	struct ethhdr *eth = eth_hdr(skb);

	memset(dst, 0, sizeof(*dst));
	
	//MAC addr
	memcpy(&dst->smac, eth->h_source, sizeof(struct mac_addr));
	memcpy(&dst->dmac, eth->h_dest, sizeof(struct mac_addr));

	//IP addr
	switch (hinfo->family) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	case NFPROTO_IPV4:
	case NFPROTO_BRIDGE:
#else
	default:
#endif
		if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_DIP)
			dst->ip.dst = maskl(ip_hdr(skb)->daddr,
			              hinfo->cfg.dstmask);
		if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_SIP)
			dst->ip.src = maskl(ip_hdr(skb)->saddr,
			              hinfo->cfg.srcmask);

		if (!(hinfo->cfg.mode &
		      (EBT_CLASSIFY_HASH_DPT | EBT_CLASSIFY_HASH_SPT)))
			return 0;
		nexthdr = ip_hdr(skb)->protocol;
		break;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	case NFPROTO_IPV6:
		if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_DIP) {
			memcpy(&dst->ip6.dst, &ipv6_hdr(skb)->daddr,
			       sizeof(dst->ip6.dst));
			classify_ipv6_mask(dst->ip6.dst, hinfo->cfg.dstmask);
		}
		if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_SIP) {
			memcpy(&dst->ip6.src, &ipv6_hdr(skb)->saddr,
			       sizeof(dst->ip6.src));
			classify_ipv6_mask(dst->ip6.src, hinfo->cfg.srcmask);
		}

		if (!(hinfo->cfg.mode &
		      (EBT_CLASSIFY_HASH_DPT | EBT_CLASSIFY_HASH_SPT)))
			return 0;
		nexthdr = ipv6_hdr(skb)->nexthdr;
		protoff = ipv6_skip_exthdr(skb, sizeof(struct ipv6hdr), &nexthdr);
		if ((int)protoff < 0)
			return -1;
		break;
#endif
	default:
		BUG();
		return 0;
#endif
	}

	switch (nexthdr) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
	case IPPROTO_SCTP:
	case IPPROTO_DCCP:{/*with the same header*/
		struct iphdr* nip_hdr;
		struct udphdr* nudp_hdr;
		
		nip_hdr = ip_hdr(skb);
		nudp_hdr = (struct udphdr *)((void *)nip_hdr + (nip_hdr->ihl << 2));
		
		_ports[0] = nudp_hdr->source;
		_ports[1] = nudp_hdr->dest;
		ports = _ports;
	}
		break;
	default:
		_ports[0] = _ports[1] = 0;
		ports = _ports;
		break;
	}
	if (!ports)
		return -1;
	if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_SPT)
		dst->src_port = ports[0];
	if (hinfo->cfg.mode & EBT_CLASSIFY_HASH_DPT)
		dst->dst_port = ports[1];
	return 0;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool
classify_mt_v0(const struct sk_buff *skb, const EBT_MATCH_PARAM *par)
#else
static int classify_mt_v0(const struct sk_buff *skb,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	const struct ebt_classify_info *r = par->matchinfo;
	unsigned int protoff = par->thoff;
#else
	const struct ebt_classify_info *r = data;
	unsigned int protoff = 0;
#endif
	struct ebt_classify_htable *hinfo = r->hinfo;
	volatile unsigned long now = jiffies;
	struct dsthash_ent *dh;
	struct dsthash_dst dst;
		
	/*DEBUG("\n%s: =====================rule name=%s=======now=%lu==============.\n", 
		__func__, r->name, now);*/

	//print_skb(skb);
	if (classify_init_dst(hinfo, &dst, skb, protoff) < 0)
		goto out;
	
	spin_lock_bh(&hinfo->lock);
	dh = dsthash_find(hinfo, &dst);
	if (!dh) {
		dh = dsthash_alloc_init(hinfo, &dst);
		if (!dh) {
			spin_unlock_bh(&hinfo->lock);
			goto out;
		}

		dh->expires = jiffies + msecs_to_jiffies(hinfo->cfg.expire);
		dh->rateinfo.prev = jiffies;
		dh->rateinfo.credit = user2credits(hinfo->cfg.avg *
						   hinfo->cfg.burst);
		dh->rateinfo.credit_cap = user2credits(hinfo->cfg.avg *
						       hinfo->cfg.burst);
		dh->rateinfo.cost = user2credits(hinfo->cfg.avg);

		init_stream(now, &dh->stream);
		
		DEBUG("new connection: --%pI4:%u->%pI4:%u--\n",
				 &dh->dst.ip.src,
				 ntohs(dh->dst.src_port),
				 &dh->dst.ip.dst,
				 ntohs(dh->dst.dst_port));
	} else {
		DEBUG("%s: update connection .\n", __func__);
		/* update expiration timeout */
		dh->expires = now + msecs_to_jiffies(hinfo->cfg.expire);
		rateinfo_recalc(dh, now);
	}
	//update statistic infomation
	update_stream(skb, now, &dh->stream);
	
	print_ent(dh);
	if(classify_algo_daemon(dh, &r->cfg)){
		spin_unlock_bh(&hinfo->lock);
		return MATCHED;
	}

	spin_unlock_bh(&hinfo->lock);

	return UNMATCHED;

out:
	return UNMATCHED;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static int classify_mt_check_v0(const struct xt_mtchk_param *par)
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static bool classify_mt_check_v0(const struct xt_mtchk_param *par)
#else
static int classify_mt_check_v0(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	struct ebt_classify_info *r = par->matchinfo;
	u_int8_t family = par->match->family;
#else
	struct ebt_classify_info *r = data;
	u_int8_t family = 2;
#endif

	//printk(KERN_ERR "%s: ...\n", __func__);
	if (r->cfg.mode == 0 ||
	    r->cfg.mode > (EBT_CLASSIFY_HASH_DPT |
			   EBT_CLASSIFY_HASH_DIP |
			   EBT_CLASSIFY_HASH_SIP |
			   EBT_CLASSIFY_HASH_SPT))
		return MY_FALSE;
	if (!r->cfg.gc_interval)
		return MY_FALSE;
	if (!r->cfg.expire)
		return MY_FALSE;
	
	if (r->cfg.min_avg_plen < 0)
		return MY_FALSE;
	if (!r->cfg.max_avg_plen)
		return MY_FALSE;
	if (r->cfg.min_avg_plen > r->cfg.max_avg_plen)
		return MY_FALSE;
	if (r->cfg.min_pack_interval < 0)
		return MY_FALSE;
	if (!r->cfg.max_pack_interval)
		return MY_FALSE;
	if (r->cfg.min_pack_interval > r->cfg.max_pack_interval)
		return MY_FALSE;
	if (!r->cfg.dur_cla_octet)
		return MY_FALSE;
	if (!r->cfg.bet_cla_octet)
		return MY_FALSE;
	
	if (r->name[sizeof(r->name) - 1] != '\0')
		return MY_FALSE;

	//dump_parameters(r);
	
	/* This is the best we've got: We cannot release and re-grab lock,
	 * since checkentry() is called before x_tables.c grabs xt_mutex.
	 * We also cannot grab the hashtable spinlock, since htable_create will
	 * call vmalloc, and that can sleep.  And we cannot just re-search
	 * the list of htable's in htable_create(), since then we would
	 * create duplicate proc files. -HW */
	mutex_lock(&hlimit_mutex);
	r->hinfo = htable_find_get(r->name, family);
	if (!r->hinfo && htable_create_v0(r, family) != 0) {
		mutex_unlock(&hlimit_mutex);
		return MY_FALSE;
	}
	mutex_unlock(&hlimit_mutex);
	return MY_TRUE;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static void
classify_mt_destroy_v0(const struct xt_mtdtor_param *par)
{
	const struct ebt_classify_info *r = par->matchinfo;

	htable_put(r->hinfo);
}
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
static struct xt_match classify_mt_reg[] __read_mostly = {
	{
		.name		= EBT_CLASSIFY_MATCH_NAME,
		.revision	= 0,
		.family		= NFPROTO_BRIDGE,
		.match		= classify_mt_v0,
		.matchsize	= XT_ALIGN(sizeof(struct ebt_classify_info)),
		.checkentry	= classify_mt_check_v0,
		.destroy	= classify_mt_destroy_v0,
		.me		= THIS_MODULE
	},
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	{
		.name		= EBT_CLASSIFY_MATCH_NAME,
		.family		= NFPROTO_IPV6,
		.match		= classify_mt_v0,
		.matchsize	= XT_ALIGN(sizeof(struct ebt_classify_info)),
		.checkentry	= classify_mt_check_v0,
		.destroy	= classify_mt_destroy_v0,
		.me		= THIS_MODULE
	},
#endif
};
#else
static struct ebt_match classify_mt_reg __read_mostly = {
	.name		= EBT_CLASSIFY_MATCH_NAME,
	.match		= classify_mt_v0,
	.check		= classify_mt_check_v0,
	.me		= THIS_MODULE
};
#endif

/* PROC stuff */
static void *dl_seq_start(struct seq_file *s, loff_t *pos)
	__acquires(htable->lock)
{
	struct proc_dir_entry *pde = s->private;
	struct ebt_classify_htable *htable = pde->data;
	unsigned int *bucket;

	spin_lock_bh(&htable->lock);
	if (*pos >= htable->cfg.size)
		return NULL;

	bucket = kmalloc(sizeof(unsigned int), GFP_ATOMIC);
	if (!bucket)
		return ERR_PTR(-ENOMEM);

	*bucket = *pos;
	return bucket;
}

static void *dl_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct proc_dir_entry *pde = s->private;
	struct ebt_classify_htable *htable = pde->data;
	unsigned int *bucket = (unsigned int *)v;

	*pos = ++(*bucket);
	if (*pos >= htable->cfg.size) {
		kfree(v);
		return NULL;
	}
	return bucket;
}

static void dl_seq_stop(struct seq_file *s, void *v)
	__releases(htable->lock)
{
	struct proc_dir_entry *pde = s->private;
	struct ebt_classify_htable *htable = pde->data;
	unsigned int *bucket = (unsigned int *)v;

	kfree(bucket);
	spin_unlock_bh(&htable->lock);
}

static int dl_seq_real_show(struct dsthash_ent *ent, u_int8_t family,
				   struct seq_file *s)
{
	/* recalculate to show accurate numbers */
	rateinfo_recalc(ent, jiffies);

	switch (family) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	case NFPROTO_IPV4:
	case NFPROTO_BRIDGE:
#else
	default:
#endif
#if 0		
		return seq_printf(s, "%ld %pI4:%u->%pI4:%u %u %u %u \n",
				 (long)(ent->expires - jiffies)/HZ,
				 &ent->dst.ip.src,
				 ntohs(ent->dst.src_port),
				 &ent->dst.ip.dst,
				 ntohs(ent->dst.dst_port),
				 ent->rateinfo.credit, ent->rateinfo.credit_cap,
				 ent->rateinfo.cost
				 );
#else
		return seq_printf(s, "%s:\t%ld %pI4:%u->%pI4:%u "
				"INFO(%d %ld %ld(byte) %ld(ms)) AVG(%ld(byte) %ld(ms))\n",
				 ent->stream.last_matched?"Matched":"Unmatched",
				 (long)(ent->expires - jiffies)/HZ,
				 &ent->dst.ip.src,
				 ntohs(ent->dst.src_port),
				 &ent->dst.ip.dst,
				 ntohs(ent->dst.dst_port),
				 ent->stream.ip_id,
				 ent->stream.package_cnt,
				 ent->stream.octet_cnt,
				 ent->stream.pack_interval_cnt,
				 ent->stream.avg_plen,
				 ent->stream.avg_pinterval
				 );
#endif
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	case NFPROTO_IPV6:
		return seq_printf(s, "%ld %pI6:%u->%pI6:%u %u %u %u\n",
				 (long)(ent->expires - jiffies)/HZ,
				 &ent->dst.ip6.src,
				 ntohs(ent->dst.src_port),
				 &ent->dst.ip6.dst,
				 ntohs(ent->dst.dst_port),
				 ent->rateinfo.credit, ent->rateinfo.credit_cap,
				 ent->rateinfo.cost);
#endif
	default:
		BUG();
		return 0;
#endif
	}
}

static int dl_seq_show(struct seq_file *s, void *v)
{
	struct proc_dir_entry *pde = s->private;
	struct ebt_classify_htable *htable = pde->data;
	unsigned int *bucket = (unsigned int *)v;
	struct dsthash_ent *ent;
	struct hlist_node *pos;

	if (!hlist_empty(&htable->hash[*bucket])) {
		hlist_for_each_entry(ent, pos, &htable->hash[*bucket], node)
			if (dl_seq_real_show(ent, htable->family, s))
				return -1;
	}
	return 0;
}

static const struct seq_operations dl_seq_ops = {
	.start = dl_seq_start,
	.next  = dl_seq_next,
	.stop  = dl_seq_stop,
	.show  = dl_seq_show
};

static int dl_proc_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &dl_seq_ops);

	if (!ret) {
		struct seq_file *sf = file->private_data;
		sf->private = PDE(inode);
	}
	return ret;
}

static const struct file_operations dl_file_ops = {
	.owner   = THIS_MODULE,
	.open    = dl_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

static int __init classify_mt_init(void)
{
	int err;

	printk(KERN_ERR "%s: line=%d.\n", __func__, __LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	err = xt_register_matches(classify_mt_reg,
	      ARRAY_SIZE(classify_mt_reg));
#else
	err = ebt_register_match(&classify_mt_reg);
#endif
	if (err < 0)
		goto err1;

	err = -ENOMEM;
	classify_cachep = kmem_cache_create("ebt_"EBT_CLASSIFY_MATCH_NAME,
					    sizeof(struct dsthash_ent), 0, 0,
					    NULL);
	if (!classify_cachep) {
		printk(KERN_ERR "ebt_classify: unable to create slab cache\n");
		goto err2;
	}
	classify_procdir4 = proc_mkdir("ebt_"EBT_CLASSIFY_MATCH_NAME, init_net.proc_net);
	if (!classify_procdir4) {
		printk(KERN_ERR "ebt_classify: unable to create proc dir "
				"entry\n");
		goto err3;
	}
	err = 0;
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	classify_procdir6 = proc_mkdir("ip6t_"EBT_CLASSIFY_MATCH_NAME, init_net.proc_net);
	if (!classify_procdir6) {
		printk(KERN_ERR "ebt_classify: unable to create proc dir "
				"entry\n");
		err = -ENOMEM;
	}
#endif
	if (!err)
		return 0;
	remove_proc_entry("ebt_"EBT_CLASSIFY_MATCH_NAME, init_net.proc_net);
err3:
	kmem_cache_destroy(classify_cachep);
err2:
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_matches(classify_mt_reg, ARRAY_SIZE(classify_mt_reg));
#else
	ebt_unregister_match(&classify_mt_reg);
#endif
err1:
	return err;

}

static void __exit classify_mt_exit(void)
{
	remove_proc_entry("ebt_"EBT_CLASSIFY_MATCH_NAME, init_net.proc_net);
#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
	remove_proc_entry("ip6t_"EBT_CLASSIFY_MATCH_NAME, init_net.proc_net);
#endif
	kmem_cache_destroy(classify_cachep);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,27))
	xt_unregister_matches(classify_mt_reg, ARRAY_SIZE(classify_mt_reg));
#else
	ebt_unregister_match(&classify_mt_reg);
#endif
}

module_init(classify_mt_init);
module_exit(classify_mt_exit);
