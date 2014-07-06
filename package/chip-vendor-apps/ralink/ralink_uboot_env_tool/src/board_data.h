/*
 * (C) Copyright 2011-2012
 */

/*
 * To build the utility with the run-time configuration
 * uncomment the next line.
 * See included "fw_env.config" sample file (TRAB board)
 * for notes on configuration.
 */
#define CONFIG_FILE     "/etc/board_data.config"

//#define HAVE_REDUND /* For systems with 2 env sectors */
#define DEVICE1_NAME      "/dev/mtd1"
#define DEVICE2_NAME      "/dev/mtd2"
#define DEVICE1_OFFSET    0x0000
#define ENV1_SIZE         0x4000
#define DEVICE1_ESIZE     0x4000
#define DEVICE2_OFFSET    0x0000
#define ENV2_SIZE         0x4000
#define DEVICE2_ESIZE     0x4000

#define CONFIG_BAUDRATE		115200
#define CONFIG_BOOTDELAY	5	/* autoboot after 5 seconds	*/

#define CONFIG_BOOTCOMMAND							\
"ipaddr=192.168.100.13"\
"serverip=192.168.100.143"\
"bootfile=ezp-rt3883-2.6-squashfs.bix"\
"stderr=serial"

# include <stdint.h>

extern int   bd_printenv(int argc, char *argv[]);
extern char *bd_getenv  (char *name);
extern int bd_setenv  (int argc, char *argv[]);

extern uint32_t  uboot_crc32 (uint32_t, const unsigned char *, unsigned);
