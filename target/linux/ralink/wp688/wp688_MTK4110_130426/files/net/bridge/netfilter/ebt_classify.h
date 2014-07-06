#ifndef _EBT_CLASSIFY_H
#define _EBT_CLASSIFY_H

#include <linux/types.h>

/* timings are in milliseconds. */
#define EBT_CLASSIFY_SCALE 10000
/* 1/10,000 sec period => max of 10,000/sec.  Min rate is then 429490
   seconds, or one every 59 hours. */

/* details of this structure hidden by the implementation */
struct ebt_classify_htable;

enum {
	EBT_CLASSIFY_HASH_DIP = 1 << 0,
	EBT_CLASSIFY_HASH_DPT = 1 << 1,
	EBT_CLASSIFY_HASH_SIP = 1 << 2,
	EBT_CLASSIFY_HASH_SPT = 1 << 3,
	EBT_CLASSIFY_INVERT   = 1 << 4,
};

struct classify_cfg {
	__u32 mode;	  /* bitmask of EBT_CLASSIFY_HASH_* */
	__u32 avg;    /* Average secs between packets * scale */
	__u32 burst;  /* Period multiplier for upper limit. */

	/* user specified */
	__u32 size;		/* how many buckets */
	__u32 max;		/* max number of entries */
	__u32 gc_interval;	/* gc interval */
	__u32 expire;	/* when do entries expire? */

	//add by alan
	__u32 min_avg_plen;//Min/Max Avg Packet Length(byte)
	__u32 max_avg_plen;//Max Avg Packet Length(byte)
	__u32 min_pack_interval;//Min Avg Inter Packet Gap(ms)
	__u32 max_pack_interval;//Max Avg Inter Packet Gap(ms)
	__u32 dur_cla_octet;//Octet Count During Classify(byte)
	__u32 bet_cla_octet;//Octet Count Between Classify(byte)
};

//added by alan
struct stream_info{
	unsigned long prev;	/* last modification */
	
	__be16	ip_id;/*used to avoid a package calculated repeatly.*/
	unsigned long package_cnt;		/* total package number */
	unsigned long octet_cnt;	/* total packages length: ip payload*/
	unsigned long pack_interval_cnt;	/* total packages interval gap:unit: ms*/

	unsigned long avg_plen;	/* avg packages length: ip payload*/
	unsigned long avg_pinterval;	/* avg packages interval gap:unit: ms*/
	
	unsigned char type;/*stream type: video,voice,normal*/
	unsigned char not_classify;/*1-will not do classify, 0-do classify*/
	unsigned char last_matched;/*1-matched last period, 0-unmatched*/
};

struct ebt_classify_info {
	char name [IFNAMSIZ];		/* name */
	struct classify_cfg cfg;

	/* Used internally by the kernel */
	struct ebt_classify_htable *hinfo;
	union {
		void *ptr;
		struct ebt_classify_info *master;
	} u;
};

struct classify_cfg1 {
	__u32 mode;	  /* bitmask of EBT_CLASSIFY_HASH_* */
	__u32 avg;    /* Average secs between packets * scale */
	__u32 burst;  /* Period multiplier for upper limit. */

	/* user specified */
	__u32 size;		/* how many buckets */
	__u32 max;		/* max number of entries */
	__u32 gc_interval;	/* gc interval */
	__u32 expire;	/* when do entries expire? */

	//add by alan
	__u32 min_avg_plen;//Min/Max Avg Packet Length(byte)
	__u32 max_avg_plen;//Max Avg Packet Length(byte)
	__u32 min_pack_interval;//Min Avg Inter Packet Gap(ms)
	__u32 max_pack_interval;//Max Avg Inter Packet Gap(ms)
	__u32 dur_cla_octet;//Octet Count During Classify(byte)
	__u32 bet_cla_octet;//Octet Count Between Classify(byte)

	__u8 srcmask, dstmask;
};

#endif /*_EBT_CLASSIFY_H*/
