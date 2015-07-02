#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/cache.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>
#include <linux/mtd/partitions.h>
#include <linux/semaphore.h>

#include "nvram.h"

#define ENV_BLK_SIZE 0x1000
#define FLASH_BLOCK_NUM	1

MODULE_LICENSE("GPL");

/**********************************************************/

static unsigned long counter = 0;


static int nvram_major = 250;

char nvram_debug = 0;

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
static struct semaphore nvram_sem;
#else 
static DECLARE_MUTEX(nvram_sem);
#endif

static block_t fb[1] = {
	{
		.name = "nvram",
		.flash_offset =  0x0,
		.flash_max_len = 0x10000,
		.valid = 0
	}
};

/* ========================================================================
 * Table of CRC-32's of all single-byte values (made by make_crc_table)
 */
const uint32_t crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

uint32_t nv_crc32(uint32_t crc, const char *buf, uint32_t len)
{
    if (buf == 0) return 0L;
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}

/* ========================================================================= */
int mtd_write_nm(char *name, loff_t to, size_t len, const u_char *buf)
{

	int ret = -1;
	size_t rdlen, wrlen;
	struct mtd_info *mtd;
	struct erase_info ei;
	u_char *bak = NULL;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd)) {
		return file_write_nm(NVRAM_FILENAME, to, len, buf);
	}
	if (len > mtd->erasesize) {
		put_mtd_device(mtd);
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		put_mtd_device(mtd);
		return -ENOMEM;
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
	ret = mtd_read(mtd, 0, mtd->erasesize, &rdlen, bak);
#else
	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
	ret = mtd_erase(mtd, &ei);
#else
	ret = mtd->erase(mtd, &ei);
#endif
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
	ret = mtd_write(mtd, 0, mtd->erasesize, &wrlen, bak);
#else
	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);
#endif

	put_mtd_device(mtd);
	kfree(bak);
	return ret;
}

int mtd_read_nm(char *name, loff_t from, size_t len, u_char *buf)
{
		
	int ret;
	size_t rdlen;
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd)) {
		return file_read_nm(NVRAM_FILENAME, from, len, buf);
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
	ret = mtd_read(mtd, from, len, &rdlen, buf);
#else
	ret = mtd->read(mtd, from, len, &rdlen, buf);
#endif
	if (rdlen != len)
		printk("warning: mtd_read_nm: rdlen is not equal to len\n");

	put_mtd_device(mtd);
	return ret;
}

int file_write_nm(char *name, loff_t to, size_t len, const u_char *buf)
{
	struct file *fp;
	mm_segment_t fs;

	fp = filp_open(name, O_RDWR|O_CREAT, 0644);
	if(IS_ERR(fp)) {
		printk("write create file error\n");
		return (int)fp;
	}

	fs=get_fs();
	set_fs(KERNEL_DS);
	
	//erase file first is no necessary
	/*
	u_char *erase; 
	erase = kmalloc(fb[0].flash_max_len, GFP_KERNEL);
	memset(erase, 0, fb[0].flash_max_len);
	vfs_write(fp, erase, fb[0].flash_max_len, 0);
	kfree(erase);
	*/
	
	//write data
	vfs_write(fp, buf, len, &to);
	filp_close(fp,NULL);
	set_fs(fs);
	//FIXME return 1 as useFile in nvram_ioctl_t
	return 1;
}

int file_read_nm(char *name, loff_t from, size_t len, u_char *buf)
{
	struct file *fp;
	mm_segment_t fs;

	fp = filp_open(name, O_RDWR|O_CREAT, 0644);
	if(IS_ERR(fp)) {
		printk("read create file error\n");
		return (int)fp;
	}

	fs=get_fs();
	set_fs(KERNEL_DS);
	vfs_read(fp, buf, len, &from);
	filp_close(fp,NULL);
	set_fs(fs);
	return 0;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
long nvramdrv_ioctl(struct file * file,
	unsigned int cmd, unsigned long buf)
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
ssize_t nvramdrv_ioctl(struct file * file,
	unsigned int cmd, unsigned long buf)
#else
ssize_t nvramdrv_ioctl(struct inode * inode, struct file * file,
	unsigned int cmd, unsigned long buf)
#endif
{
	int index, len;
	const char *p;
	nvram_ioctl_t *nvr;
	char *value;


	switch (cmd) {
		case NVRAM_IOCTL_GET:
			nvr = (nvram_ioctl_t __user *)buf;
			p = nvram_get(nvr->index, nvr->name);
			if (p == NULL)
				p = "";

			if (copy_to_user(nvr->value, p, strlen(p) + 1))
				return -EFAULT;
			break;

		case NVRAM_IOCTL_GETALL:
			nvr = (nvram_ioctl_t __user *)buf;
			//index = nvr->index;
			index = 0;
			len = fb[index].flash_max_len - sizeof(fb[index].env.crc);

			if (nvram_getall(index, fb[index].env.data) == 0) {
				if (copy_to_user(nvr->value, fb[index].env.data, len))
					return -EFAULT;
			}
			break;
		case NVRAM_IOCTL_SET:
			nvr = (nvram_ioctl_t *)buf;		
			value = (char *)kmalloc(MAX_VALUE_LEN, GFP_KERNEL);
			if (!value)
				return -ENOMEM;

			if (copy_from_user(value, nvr->value, strlen(nvr->value) + 1)) {
				kfree(value);
				return -EFAULT;
			}

			nvram_set(nvr->index, nvr->name, value);
			kfree(value);
			break;

		case NVRAM_IOCTL_COMMIT:
			nvr = (nvram_ioctl_t __user *)buf;
			nvr->useFile = nvram_commit(nvr->index);
			break;
		case NVRAM_IOCTL_CLEAR:
			nvr = (nvram_ioctl_t __user *)buf;
			nvram_clear(nvr->index);
		default:
			printk(KERN_CRIT "Unsupported ioctl(%d) on nvram\n", cmd);
			break;
	}
	return 0;
}

int nvramdrv_open(struct inode *inode, struct file *filp)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_INC_USE_COUNT;
#else
	try_module_get(THIS_MODULE);
#endif
    return 0;
}

int nvramdrv_release(struct inode *inode, struct file *filp)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_DEC_USE_COUNT;
#else
	module_put(THIS_MODULE);
#endif
    return 0;
}


struct file_operations nvramdrv_fops = {
	.owner 			=	THIS_MODULE,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
    .unlocked_ioctl	= nvramdrv_ioctl,
#else
	.ioctl			= nvramdrv_ioctl,
#endif
    .open 			= nvramdrv_open,
    .release 		= nvramdrv_release,
};

int __init nvramdrv_init(void)
{
    int r;

    r = register_chrdev(nvram_major, NVRAM_DEVNAME, 
			&nvramdrv_fops);
    if (r < 0) {
        printk(KERN_CRIT "nvramdrv: unable to register %s(%d)\n",
        		NVRAM_DEVNAME, nvram_major);
        return r;
    }
    if (nvram_major == 0) {
		nvram_major = r;
		printk(KERN_DEBUG "nvramdrv: got dynamic major %d\n", r);
    }

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,3,0)
	 sema_init(&nvram_sem, 1);
#else
	init_MUTEX(&nvram_sem);
#endif

	down(&nvram_sem);
	init_nvram_block();
	up(&nvram_sem);

    return 0;
}

static int init_nvram_block()
{
	unsigned long from;
	int i, j, len;
	char *p, *q;
	int index;

	i = 0;
	index = i;	
	RANV_PRINT("--> nvram_init %d\n", i);
	RANV_CHECK_INDEX(-1);

	if (fb[i].valid)
		return -EINVAL;

	//read crc from flash
	from = fb[i].flash_offset;
	len = sizeof(fb[i].env.crc);
	mtd_read_nm(NVRAM_MTDNAME, from, len, (unsigned char *)&fb[i].env.crc);

	//read data from flash
	from = from + len;
	len = fb[i].flash_max_len - len;


	fb[i].env.data = (char *)kmalloc(len, GFP_KERNEL);

	if (!fb[i].env.data)
		return -ENOMEM;

	mtd_read_nm(NVRAM_MTDNAME, from, len, (unsigned char *)fb[i].env.data);

	//check crc
	if (nv_crc32(0, fb[i].env.data, len) != fb[i].env.crc) {
		printk("crc check error\n");
		RANV_PRINT("Bad CRC %x, ignore values in flash.\n", (unsigned int)fb[i].env.crc);
		memset(fb[i].env.data, 0, len);
		fb[i].valid = 1;
		fb[i].dirty = 0;
		return -1;
	}
	
	//parse env to cache
	p = fb[i].env.data;
	for (j = 0; j < MAX_CACHE_ENTRY; j++) {
		if (NULL == (q = strchr(p, '='))) {
			RANV_PRINT("parsed failed - cannot find '='\n");
			break;
		}
		*q = '\0'; //strip '='
		fb[i].cache[j].name = kstrdup(p, GFP_KERNEL);
		//printk("  %d '%s'->", i, p);
	
		p = q + 1; //value
		if (NULL == (q = strchr(p, '\0'))) {
			RANV_PRINT("parsed failed - cannot find '\\0'\n");
			break;
		}
		fb[i].cache[j].value = kstrdup(p, GFP_KERNEL);
		//printk("'%s'\n", p);

		p = q + 1; //next entry
		if (p - fb[i].env.data + 1 >= len) {
			//end of block
			break;
		}
		if (*p == '\0') {
			//end of env 
			break;
		}
	}
	if (j == MAX_CACHE_ENTRY)
		RANV_PRINT("run out of env cache, please increase MAX_CACHE_ENTRY\n");

	fb[i].valid = 1;
	fb[i].dirty = 0;

	return 0;
}


void __exit nvramdrv_exit(void)
{
	int index;
	
	for (index = 0; index < 1; index++) {
		if (fb[index].dirty)
			nvram_commit(index);

		ra_nvram_close();
	
		fb[index].valid = 0;
		//free env
		kfree(fb[index].env.data);
	}


	unregister_chrdev(nvram_major, NVRAM_DEVNAME);
}

static int ra_nvram_close()
{
	int i = 0;
	int index = 0;

	RANV_PRINT("--> nvram_close %d\n", index);
	RANV_CHECK_INDEX(-1);

	if (!fb[index].valid)
		return 0;
	down(&nvram_sem);

	//free cache
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (fb[index].cache[i].name) {
			kfree(fb[index].cache[i].name);
			fb[index].cache[i].name = NULL;
		}
		if (fb[index].cache[i].value) {
			kfree(fb[index].cache[i].value);
			fb[index].cache[i].value = NULL;
		}
	}
	
	up(&nvram_sem);
	return 0;
}

/*
 * return idx (0 ~ iMAX_CACHE_ENTRY)
 * return -1 if no such value or empty cache
 */
static int cache_idx(int index, char *name)
{
	int i;

	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (!fb[index].cache[i].name)
			return -1;
		if (!strcmp(name, fb[index].cache[i].name))
			return i;
	}
	return -1;
}

/*
 * clear flash by writing all 1's value
 */
int nvram_clear(int index)
{
	unsigned long to;
	int len;

	RANV_PRINT("--> nvram_clear %d\n", index);
	RANV_CHECK_INDEX(-1);

	ra_nvram_close();

	down(&nvram_sem);

	//construct all 1s env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	if (!fb[index].env.data) {
		fb[index].env.data = (char *)kmalloc(len, GFP_KERNEL);
		if (!fb[index].env.data)
			return -ENOMEM;
	}
	memset(fb[index].env.data, 0xFF, len);

	//calculate crc
	fb[index].env.crc = (unsigned long)nv_crc32(0, (unsigned char *)fb[index].env.data, len);

        //write crc to flash
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	mtd_write_nm(NVRAM_MTDNAME, to, len, (unsigned char *)&fb[index].env.crc);

	//write all 1s data to flash
	to = to + len;
	len = fb[index].flash_max_len - len;
	mtd_write_nm(NVRAM_MTDNAME, to, len, (unsigned char *)fb[index].env.data);

	RANV_PRINT("clear flash from 0x%x for 0x%x bytes\n", (unsigned int)to, len);
	fb[index].dirty = 0;

	up(&nvram_sem);

	return 0;
}

int nvram_commit(int index)
{
	int ret = 0;
	unsigned long to;
	int i, len;
	char *p;

	RANV_PRINT("--> nvram_commit %d\n", index);


	RANV_CHECK_INDEX(-1);

	down(&nvram_sem);
	RANV_CHECK_VALID();

	counter++;

	if (!fb[index].dirty) {
		RANV_PRINT("nothing to be committed\n");
		up(&nvram_sem);
		return 0;
	}

	//construct env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	if (!fb[index].env.data) {
		fb[index].env.data = (char *)kmalloc(len, GFP_KERNEL);
		if (!fb[index].env.data)
			return -ENOMEM;
	}
	memset(fb[index].env.data, 0, len);
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		int l;

		if (!fb[index].cache[i].name || !fb[index].cache[i].value)
			break;
		l = strlen(fb[index].cache[i].name) + strlen(fb[index].cache[i].value) + 2;
		if (p - fb[index].env.data + 2 >= fb[index].flash_max_len) {
			RANV_ERROR("ENV_BLK_SIZE 0x%x is not enough!", ENV_BLK_SIZE);
			up(&nvram_sem);
			return -1;
		}
		snprintf(p, l, "%s=%s", fb[index].cache[i].name, fb[index].cache[i].value);
		p += l;
	}

	*p = '\0'; //ending null

	//calculate crc
	fb[index].env.crc = (unsigned long)nv_crc32(0, (unsigned char *)fb[index].env.data, len);

	//write crc to flash
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	ret = mtd_write_nm(NVRAM_MTDNAME, to, len, (unsigned char *)&fb[index].env.crc);

	//write data to flash
	to = to + len;
	len = fb[index].flash_max_len - len;
	ret = mtd_write_nm(NVRAM_MTDNAME, to, len, (unsigned char *)fb[index].env.data);

	fb[index].dirty = 0;

	up(&nvram_sem);

	return ret;
}

int nvram_set(int index, char *name, char *value)
{
	int idx;

	RANV_PRINT("--> nvram_set %d %s=%s\n", index, name, value);

	RANV_CHECK_INDEX(-1);
	
	down(&nvram_sem);
	RANV_CHECK_VALID();

	counter++;

	idx = cache_idx(index, name);

	if (-1 == idx) {

		//find the first empty room
		for (idx = 0; idx < MAX_CACHE_ENTRY; idx++) {
			if (!fb[index].cache[idx].name) {
				break;
			}
		}
		//no any empty room
		if (idx == MAX_CACHE_ENTRY) {
			RANV_ERROR("run out of env cache, please increase MAX_CACHE_ENTRY\n");
			up(&nvram_sem);
			return -1;
		}
		fb[index].cache[idx].name = kstrdup(name, GFP_KERNEL);
		fb[index].cache[idx].value = kstrdup(value, GFP_KERNEL);
	}
	else {
		//abandon the previous value
		kfree(fb[index].cache[idx].value);
		fb[index].cache[idx].value = kstrdup(value, GFP_KERNEL);
	}

	fb[index].dirty = 1;
	up(&nvram_sem);

	return 0;
}

char const *nvram_get(int index, char *name)
{
	int idx;
	static char const *ret;

	RANV_PRINT("--> nvram_get %d %s\n", index, name);

	RANV_CHECK_INDEX(NULL);

	down(&nvram_sem);
	RANV_CHECK_VALID();

	counter++;

	idx = cache_idx(index, name);
	if (-1 != idx) {
		if (fb[index].cache[idx].value) {
			//duplicate the value in case caller modify it
			//ret = strdup(fb[index].cache[idx].value);
			ret = fb[index].cache[idx].value;
			up(&nvram_sem);
			return ret;
		}
	}

	//printk("nvram_get: not found\n");
	//no default value set?
	//btw, we don't return NULL anymore!

	up(&nvram_sem);

	return NULL;
}

int nvram_getall(int index, char *buf) 
{
	int i, len;
	char *p;

	RANV_CHECK_INDEX(-1);
	
	down(&nvram_sem);
	RANV_CHECK_VALID();

	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	if (!fb[index].env.data) {
		fb[index].env.data = (char *)kmalloc(len, GFP_KERNEL);
		if (!fb[index].env.data)
			return -ENOMEM;
	}
	memset(fb[index].env.data, 0, len);
	p = buf;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		int l, ret;

		if (!fb[index].cache[i].name || !fb[index].cache[i].value)
			break;
		l = strlen(fb[index].cache[i].name) + strlen(fb[index].cache[i].value) + 2;
		if (p - fb[index].env.data + 2 >= fb[index].flash_max_len) {
			RANV_ERROR("ENV_BLK_SIZE 0x%x is not enough!", ENV_BLK_SIZE);
			up(&nvram_sem);
			return -1;
		}
		ret = snprintf(p, l, "%s=%s", fb[index].cache[i].name, fb[index].cache[i].value);
		
		p += l;
	}
	*p = '\0'; //ending null

	up(&nvram_sem);
	
	return 0;
}

module_init(nvramdrv_init);
module_exit(nvramdrv_exit);
