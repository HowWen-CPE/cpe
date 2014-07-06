/*
 * Filename:wp688_wdt.c
 * Description: 
 *       Linux driver for wp688_XC6121 hardware watchdog.
 *       We directly feed dog periodicly in this driver. We provide 
 *       open/write/close for user application. The watch dog timeout 
 *       is about 15 seconds and we feed it every 0.5 seconds.
 *              
 * Version: 1.0
 *
 * Revision: 
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/version.h>

#define PFX "wp688_watchdog: "

#define WDT_GPIO     10
#define WDT_GPIO_EN  12

#define ENABLE_WATCH_DOG   0x1
#define DISABLE_WATCH_DOG  0x2
#define ENABLE_DONT_DOG    0x3


#define RALINK_PIO_ADDR          0xB0000600
#define RALINK_REG_PIO1_DIR      (RALINK_PIO_ADDR + 0x24)
#define RALINK_REG_PIO1_SET      (RALINK_PIO_ADDR + 0x2c)
#define RALINK_REG_PIO1_RESET    (RALINK_PIO_ADDR + 0x30)

#define RALINK_REG_WR

static int nowayout = WATCHDOG_NOWAYOUT;
static int watchdog_feed_dog;

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");

/* a timer for feeding watchdog again and again*/
struct timer_list        wtd_time;

static void wp688_dog_start(void)
{
    /* set WDT_GPIO, WDT_GPIO_EN pin as output*/
	unsigned int tmp = *(volatile unsigned int *)(RALINK_REG_PIO1_DIR);
	*(volatile unsigned int *)(RALINK_REG_PIO1_DIR) = tmp | (1 << WDT_GPIO);
	tmp = *(volatile unsigned int *)(RALINK_REG_PIO1_DIR);
	*(volatile unsigned int *)(RALINK_REG_PIO1_DIR) = tmp | (1 << WDT_GPIO_EN);
   
    /* pull high WDT_GPIO, WDT_GPIO_EN*/
	*(volatile unsigned int *)(RALINK_REG_PIO1_SET) = ((1 << WDT_GPIO) | (1 << WDT_GPIO_EN));

}

static void wp688_dog_stop(void)
{

	if(wtd_time.expires > 0 ){
		del_timer(&wtd_time);
		wtd_time.expires = 0;
	}
	
	/* stop watch dog  pull low WDT_GPIO_EN*/
	*(volatile unsigned int *)(RALINK_REG_PIO1_RESET) = (1 << WDT_GPIO_EN);
	
	printk(KERN_INFO PFX "Stop watchdog.\n");
}

/* keep dog alive*/
static void wp688_dog_ping(void)
{
	static int tmpval = 0;

	if(!watchdog_feed_dog)
	{
	    return;
	}

	if(tmpval == 0)
	{
	    tmpval = 1;
	    /* pull low WDT_GPIO*/
		*(volatile unsigned int *)(RALINK_REG_PIO1_RESET) = (1 << WDT_GPIO);
	}
	else
	{
	    tmpval = 0;
        /* pull high WDT_GPIO*/
	    *(volatile unsigned int *)(RALINK_REG_PIO1_SET) = (1 << WDT_GPIO);
	}
}


/*
 * fresh the watch dog every 0.5 seconds.
 */
static void wp688_dog_feed_period(unsigned long arg)
{
	wp688_dog_ping();
	mod_timer(&wtd_time, jiffies+(1*HZ)/2);
}

/*
 * Start feed dog, call wp688_dog_feed() to feed dog eveyry 0.5 seconds.
 */
static void wp688_start_feed(void)
{
	if(wtd_time.expires > 0 ){
		del_timer(&wtd_time);
		wtd_time.expires = 0;
	}

	init_timer(&wtd_time);
	wtd_time.data = 0;
	wtd_time.expires = jiffies + HZ/2;
	wtd_time.function = wp688_dog_feed_period;
	
	add_timer(&wtd_time);
}


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
static int wp688_dog_ioctl(struct file *file, unsigned int req,
		unsigned long arg)
#else
static int wp688_dog_ioctl(struct inode *inode, struct file *file, unsigned int req,
		unsigned long arg)
#endif
{	
	
	switch (req) {		
		case ENABLE_WATCH_DOG:		
			{		        
				wp688_dog_start();
				wp688_start_feed();
				wp688_dog_ping();
				watchdog_feed_dog = 1;	
				printk(KERN_INFO PFX "Enable watchdog.\n");
				return 0;
			}		
		case DISABLE_WATCH_DOG:		
			{		        
				wp688_dog_stop();
				printk(KERN_INFO PFX "Disable watchdog.\n");
				return 0;	
			}
		case ENABLE_DONT_DOG:		
			{		        
				wp688_dog_start();
				wp688_start_feed();
				wp688_dog_ping();
				watchdog_feed_dog = 0;
				printk(KERN_INFO PFX "Enable watchdog, Do not feed!\n");
				return 0;		
			}	
	}
	return -1;
}

int wp688_dog_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_INC_USE_COUNT;
#else
	try_module_get(THIS_MODULE);
#endif
	return 0;
}

int wp688_dog_release(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_DEC_USE_COUNT;
#else
	module_put(THIS_MODULE);
#endif
	return 0;
}

static int wp688_dog_notify_sys(struct notifier_block *this, unsigned long code, void *unused)
{
	if (code == SYS_DOWN || code == SYS_HALT)
	{
		wp688_dog_stop();		/* Turn the WDT off */
	}

	return NOTIFY_DONE;
}

static const struct file_operations wp688_dog_fops = {
	.owner		= THIS_MODULE,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	.unlocked_ioctl = wp688_dog_ioctl,
#else
	.ioctl      = wp688_dog_ioctl,
#endif
	.open       = wp688_dog_open,
	.release	= wp688_dog_release,
};

#define WDT_MINOR 44

static struct miscdevice wp688_dog_miscdev = {
	.minor		= WDT_MINOR,
	.name		= "wp688_wdt",
	.fops		= &wp688_dog_fops,
};

static struct notifier_block wp688_dog_notifier = {
	.notifier_call = wp688_dog_notify_sys,
};

static char banner[] __initdata =
	KERN_INFO PFX "watchdog gpio control for wp688 \n";

static int __init watchdog_init(void)
{
	int ret;

	ret = register_reboot_notifier(&wp688_dog_notifier);
	if (ret) {
		printk(KERN_ERR PFX "cannot register reboot notifier (err=%d)\n",
			ret);
		return ret;
	}

	ret = misc_register(&wp688_dog_miscdev);
	if (ret) {
		printk(KERN_ERR PFX "cannot register miscdev on minor=%d (err=%d)\n",
			WDT_MINOR, ret);
		unregister_reboot_notifier(&wp688_dog_notifier);
		return ret;
	}

	printk(banner);
	watchdog_feed_dog = 1;

	wp688_dog_start();
	wp688_dog_ping();
	/* we directly feed dog in this driver periodicly*/
	wp688_start_feed();

	return 0;
}

static void __exit watchdog_exit(void)
{
	wp688_dog_stop();

	misc_deregister(&wp688_dog_miscdev);
	unregister_reboot_notifier(&wp688_dog_notifier);
	printk(KERN_INFO PFX "rmmod watchdog.\n");
}

module_init(watchdog_init);
module_exit(watchdog_exit);

MODULE_AUTHOR("LITEON");
MODULE_DESCRIPTION("watchdog for wp688");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WDT_MINOR);

