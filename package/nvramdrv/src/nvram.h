#ifndef _nvram_h_
#define _nvram_h_

#define NVRAM_MTDNAME "nvram"
#define NVRAM_DEVNAME "nvram"

#define RANV_PRINT(x, ...) do { if (nvram_debug) printk("\n%s %d: " x, __FILE__, __LINE__, ## __VA_ARGS__); } while(0)
#define RANV_ERROR(x, ...) do { printk("%s %d: ERROR! " x, __FILE__, __LINE__, ## __VA_ARGS__); } while(0)

//x is the value returned if the check failed
#define RANV_CHECK_INDEX(x) do { \
        if (index < 0 || index >= FLASH_BLOCK_NUM) { \
                RANV_PRINT("index(%d) is out of range\n", index); \
		return x; \
        } \
} while (0)

#define RANV_CHECK_VALID() do { \
        if (!fb[index].valid) { \
                RANV_PRINT("fb[%d] invalid, init again\n", index); \
		init_nvram_block(); \
        } \
} while (0)

typedef struct environment_s {
	unsigned long crc;		//CRC32 over data bytes
	char *data;
} env_t;

typedef struct cache_environment_s {
	char *name;
	char *value;
} cache_t;

#define MAX_CACHE_ENTRY 500
typedef struct block_s {
	char *name;
	env_t env;			//env block
	cache_t	cache[MAX_CACHE_ENTRY];	//env cache entry by entry
	unsigned long flash_offset;
	unsigned long flash_max_len;	//ENV_BLK_SIZE
	char valid;
	char dirty;
} block_t;

#define MAX_NAME_LEN 128
#define MAX_VALUE_LEN (ENV_BLK_SIZE * 5)
typedef struct nvram_ioctl_s {
	int index;
	int ret;
	char *name;
	char *value;
} nvram_ioctl_t;


#define NVRAM_IOCTL_GET		0x01
#define NVRAM_IOCTL_GETALL	0x02
#define NVRAM_IOCTL_SET		0x03
#define NVRAM_IOCTL_COMMIT	0x04
#define NVRAM_IOCTL_CLEAR	0x05

static int mtd_write_nm(char *name, loff_t to, 
	size_t len, const u_char *buf);
static int mtd_read_nm(char *name, loff_t from, 
	size_t len, u_char *buf);

static int init_nvram_block(void);
static int ra_nvram_close(void);
static int cache_idx(int index, char *name);


char const *nvram_get(int index, char *name);
int nvram_getall(int index, char *buf);
int nvram_set(int index, char *name, char *value);
int nvram_commit(int index);
int nvram_clear(int index);

#endif /* _nvram_h_ */
