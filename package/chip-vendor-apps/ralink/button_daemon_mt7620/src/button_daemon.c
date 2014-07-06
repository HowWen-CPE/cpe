#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/autoconf.h>

#include "ralink_gpio.h"
#include<sys/time.h>

#include<unistd.h>



#define CONFIG_RALINK_GPIO		1

#define GPIO_DEV 					"/dev/gpio"

#define FACTORY_DEFAULT_GPIO		14	

#define SHORT_PRESS_BUTTON		1
#define LONG_PRESS_BUTTON		2


static char *saved_pidfile;

static void button_func(int type){	
	static struct timeval falling_tv;
	static struct timeval rising_tv;
	static struct timezone tz;
	
	if(type == SHORT_PRESS_BUTTON){				
				gettimeofday (&falling_tv, &tz);
				//printf("==faling edge signal!==   time:%d:%d\n\n",falling_tv.tv_sec,falling_tv.tv_usec);				
			}
			else if(type == LONG_PRESS_BUTTON){//long pressed			
				gettimeofday (&rising_tv, &tz);
				//printf("~~rising edge signal!~~   time:%d:%d\n\n",rising_tv.tv_sec,rising_tv.tv_usec);	
				if ((rising_tv.tv_sec!=0) &&( rising_tv.tv_usec)&&(falling_tv.tv_sec!=0)&&(falling_tv.tv_usec!=0))
				{
					if (rising_tv.tv_sec < falling_tv.tv_sec)
						printf("Test Error\n");
					else 
					{
						if (rising_tv.tv_sec > falling_tv.tv_sec)
							printf("Test OK\n");
						else if (rising_tv.tv_sec == falling_tv.tv_sec)
						{ 
						 	if (rising_tv.tv_usec > falling_tv.tv_usec)
								printf("Test OK\n");
							else if (rising_tv.tv_usec == falling_tv.tv_usec)
								printf("Test Error\n");
							else if (rising_tv.tv_usec <falling_tv.tv_usec)
								printf("Test Error\n");
						}
					}	
				}				
			}	
	return 0;
 }


/*
 * gpio interrupt handler -
 *   SIGUSR1 - notify goAhead to start WPS (by sending SIGUSR1)
 *   SIGUSR2 - restore default value
 */
static void buttonIrqHandler(int signum)
{
	if (signum == SIGUSR1) {
		//RT2883, RT3052, RT3883 use a different gpio number for WPS,
		//which will be registered in goahead instead

		//check the dir /tmp/but_gpio/, the files in it are with the number of gpio, 
		//and its contents indicate the jiffies of the button pressed.
		//TODO
        	button_func(SHORT_PRESS_BUTTON);
	} 
	else if (signum == SIGUSR2) {

		//check the dir /tmp/but_gpio/, the files in it are with the number of gpio, 
		//and its contents indicate the jiffies of the button pressed.
		//TODO
        	button_func(LONG_PRESS_BUTTON);

	}
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
	ralink_gpio_reg_info info;
      
	info.pid = getpid();
	info.irq = gpio;

	fd = open(GPIO_DEV, O_RDONLY);
	if (fd < 0) {
		perror(GPIO_DEV);
		return -1;
	}

	//enable gpio interrupt
	if (ioctl(fd, RALINK_GPIO_ENABLE_INTP) < 0)
		goto ioctl_err;

	//register my information
	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0)
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

