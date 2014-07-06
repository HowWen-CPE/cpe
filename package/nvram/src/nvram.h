#ifndef _nvram_h_
#define _nvram_h_

/**
 * **************Basic Macros & Structure*************************
 */
#define FLASH_BLOCK_NUM 1
#define NV_DEV "/dev/nvram"

#define NVRAM_IOCTL_GET		0x01
#define NVRAM_IOCTL_GETALL	0x02
#define NVRAM_IOCTL_SET		0x03
#define NVRAM_IOCTL_COMMIT	0x04
#define NVRAM_IOCTL_CLEAR	0x05

typedef struct environment_s {
	unsigned long crc;		//CRC32 over data bytes
	char *data;
} env_t;

typedef struct cache_environment_s {
	char *name;
	char *value;
} cache_t;

#define ENV_BLK_SIZE 0x1000
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


/**
 * **************Advanced Macros & Structure*************************
 */

#define xstr(s) str(s)
#define str(s)  #s 

#if defined(PRODUCT_GP_WP688)
#define EZP_PROD_VERSION "0.0.0"
#elif defined(PRODUCT_GP_WP868)
#define EZP_PROD_VERSION "0.0.0"
#elif defined(PRODUCT_GP_WP838)
#define EZP_PROD_VERSION "0.0.0"
#else
#undef EZP_PROD_VERSION
#endif



#define EZP_PROD_CAT 0
#define EZP_PROD_SUBCAT 0
#define EZP_PROD_SUBSUBCAT 0
#define EZP_PROD_FW_VERSION "V1.00(BWQ.1)C0"

#define NVRAM_SPACE     0x10000

/* rule buf len for NVRAM */
#define NVRAM_TMP_LEN 256

/* None of specific NVRAM options. */
#define NVRAM_NONE 0x00
/* NOT allowed to be disclosed (e.g. a config file). */
#define NVRAM_PRIVATE 0x01
/* NOT allowed to be overwritten (e.g. a license key). If the value is
 *  * empty, it could be written for once.*/
#define NVRAM_PROTECTED 0x02
/* For temporary use. */
#define NVRAM_TEMP 0x04
/* Customized by the authorized program. */
#define NVRAM_CUSTOMIZED 0x08
/* NVRAM could be empty. */
#define NVRAM_EMPTY 0x10
/* NVRAM value should be set by *_default if this value is empty . */
#define NVRAM_DEFAULT 0x20
/* NVRAM is undefined. */
#define NVRAM_UNDEFINED 0x80000000

typedef struct nvram_tuple {
	const char *name;
	const char *value;
	unsigned int option;
	struct nvram_tuple *next;
}nvram_tuple_t;

/**
 * **************Basic Interface*************************
 */
int		nvram_init(void * unused);
void 	nvram_close(void);

int 	nvram_set(const char *name, const char *value);
char *	nvram_get(const char *name);
int 	nvram_getall(char *buf,int count);
int 	nvram_commit(void);

int 	nvram_fset(const char *name, const char *value);
int 	nvram_unset(const char *name);
char *	nvram_safe_get(const char *name);
int	nvram_safe_unset(const char *name);
int	nvram_safe_set(const char *name, const char *value);


int 	cache_idx(const char *name);
int 	nvram_bufset(const char *name, const char *value);
char  	*nvram_bufget(const char *name);
void 	nvram_buflist(void);


int 	nvram_clear(void);
//int 	nvram_erase();//TODO
void 	toggleNvramDebug(void);

/**
 * **************Advanced Interface*************************
 */

int 	nvram_get_option(const char *name);
int 	nvram_export(const char *filename);
int 	nvram_import(const char *filename);
int 	nvram_upgrade(const char *source);
int 	nvram_upgradeto(const char *target);
int 	nvram_factory(void);
int 	nvram_default(void);
int 	nvram_default_rule(const char *name);
void 	nvram_boot(void);

int 	nvram_match(char *name, char *match);
int 	nvram_invmatch(char *name, char *invmatch);
char *	nvram_get_default(const char *name);
#ifdef DEBUG
/* for SIT Review*/
int 	nvram_dump(void);
int 	nvram_dump_default(void);
char *	rule_attr_desc(const char *rule);
char *	same_attr_rule(const char *rule, int bColor);
int		nvram_show_desc(const char *rule, int bColor);
#endif /* ifdef DEBUG*/
#endif /* _nvram_h_ */
