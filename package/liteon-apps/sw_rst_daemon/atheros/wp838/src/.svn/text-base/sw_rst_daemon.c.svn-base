/*
 * The reset button test daemon for WP838, Sean@Liteon, 2013-12-24
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/autoconf.h>

#include<sys/time.h>

#include<unistd.h>

#define GPIO_DEV 					"/dev/Factory_reset"

#define FACTORY_DEFAULT_GPIO		17	

#define PRESS_BUTTON		                1
#define RELEASE_BUTTON		        2

#define LITEON_GPIO_REG_IRQ              0x89ABCDF0
#define LITEON_GPIO_ENA_INT              0x89ABCDF1

static inline int liteon_jiffies(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

#define jiffies liteon_jiffies()

/**
 * enum irqreturn
 * @IRQ_NONE		interrupt was not from this device
 * @IRQ_HANDLED		interrupt was handled by this device
 * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
 */
enum irqreturn {
	IRQ_NONE,
	IRQ_HANDLED,
	IRQ_WAKE_THREAD,
};

typedef enum irqreturn irqreturn_t;

static char *saved_pidfile;

typedef struct {
      unsigned int irq; //request irq pin number
      pid_t pid;        //process id to notify
} gpio_reg_info;


/*
 * gpio interrupt handler -
 *   SIGUSR1 - press the reset button
 *   SIGUSR2 - release the reset button
 */
static irqreturn_t buttonIrqHandler(int signum)
{
    static int press_time = 0;
    if (signum == SIGUSR1) {
        press_time = jiffies;

        return IRQ_HANDLED;
    } 
    else if (signum == SIGUSR2) {
        press_time = jiffies - press_time;
    }

    printf("\nTest %s\n\n", (press_time > 0) ? "OK." : "Failed!!!");

    return IRQ_HANDLED;
    
}


/*
 * init gpio interrupt -
 *   1. config gpio interrupt mode
 *   2. register my pid and request gpio pin 0
 *   3. issue a handler to handle SIGUSR1 and SIGUSR2
 */
int initGpioIRQ(int gpio)
{
    int fd;
    gpio_reg_info info;
      
    info.pid = getpid();
    info.irq = gpio;

    fd = open(GPIO_DEV, O_RDONLY);
    if (fd < 0) {
    	perror(GPIO_DEV);
    	return -1;
    }

    //enable gpio interrupt
    if (ioctl(fd, LITEON_GPIO_ENA_INT) < 0)
    	goto ioctl_err;

    //register my information
    if (ioctl(fd, LITEON_GPIO_REG_IRQ, &info) < 0)
    	goto ioctl_err;
    
    close(fd);

    //issue a handler to handle SIGUSR1 and SIGUSR2
    signal(SIGUSR1, buttonIrqHandler);
    signal(SIGUSR2, buttonIrqHandler);
    
    return 0;

ioctl_err:
    perror("ioctl");
    close(fd);
    return -1;
}

static void pidfile_delete(void)
{
    if (saved_pidfile) unlink(saved_pidfile);
}

int pidfile_acquire(const char *pidfile)
{
    int pid_fd;
    if (!pidfile) return -1;

    pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
    if (pid_fd < 0) {
    	printf("Unable to open pidfile %s: %m\n", pidfile);
    } else {
    	lockf(pid_fd, F_LOCK, 0);
    	if (!saved_pidfile)
            atexit(pidfile_delete);
        saved_pidfile = (char *) pidfile;
    }
    return pid_fd;
}

void pidfile_write_release(int pid_fd)
{
    FILE *out;

    if (pid_fd < 0) return;

    if ((out = fdopen(pid_fd, "w")) != NULL) {
    	fprintf(out, "%d\n", getpid());
    	fclose(out);
    }
    lockf(pid_fd, F_UNLCK, 0);
    close(pid_fd);
}

int main(int argc,char **argv)
{
    pid_t pid;
    int fd;

    if (initGpioIRQ(FACTORY_DEFAULT_GPIO) !=0)
    	exit(EXIT_FAILURE);
    	
    fd = pidfile_acquire("/var/run/buttond.pid");
    pidfile_write_release(fd);

    while (1) {
    	pause();
    }

    exit(EXIT_SUCCESS);
}

