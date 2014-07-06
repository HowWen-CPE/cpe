#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/types.h>

#define _A(str) 	_TMP(str)
#define _TMP(str)	#str

typedef struct {
      unsigned int irq; //request irq pin number
      pid_t pid;        //process id to notify
      } gpio_reg_info;


static char *saved_pidfile;
static double gPressTime = 0;
int initGpioIRQ(int gpio);
static void pidfile_delete(void);
int pidfile_acquire(const char *pidfile);
void pidfile_write_release(int pid_fd);
void sig_act_press(int sig);
void sig_act_release(int sig);
/*
 * init gpio interrupt -
 *   1. config gpio interrupt mode
 *   2. register my pid and request gpio pin 0
 *   
 */
int initGpioIRQ(int gpio)
{
	int fd;
	gpio_reg_info info;
      
	info.pid = getpid();    //transfer button_daemon's pid to lower driver.
	info.irq = gpio;

	fd = open(_A(GPIO_DEV), O_RDONLY);
	if (fd < 0) {
		perror(_A(GPIO_DEV));
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
	signal(SIGUSR1,sig_act_press);
	signal(SIGUSR2,sig_act_release); 
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

void sig_act_press(int sig)
{
	double press_time = 0;
	struct timeval tv;

	if( sig == SIGUSR1)
	{	// SIGUSR1 is press signal
		gettimeofday(&tv,NULL);
		press_time = 1000*tv.tv_sec+tv.tv_usec/1000; //get current time by milesec
	}
	gPressTime = press_time;
	printf("press_time = %3.3f ms\r\n",press_time);
	return ;
}
void sig_act_release(int sig)
{
	double press_time = 0;
	double release_time = 0;
	double period = 0;
	struct timeval tv2;
    char cmd[1024];
	press_time = gPressTime;
 	if( sig == SIGUSR2)
	{ //SIGUSR2 is release signal
		gettimeofday(&tv2,NULL);
		release_time = 1000*tv2.tv_sec+tv2.tv_usec/1000;
    }
	printf("release_time = %3.3f ms\r\n",release_time);
	period = release_time - press_time;
	gPressTime = 0 ;
	
	if(period <=500 || period >= 60000)
	{
		return;
	}
	
	if(period > 500 && period < 60000)
	{
		if(period > 5000)
		{	
			printf("press button time = %3.3f ms\r\n nvram factory and Restart system now\r\n",period);
			snprintf(cmd, 1024, "logger EZP_USR  %s\n ","resetbutton to nvram facotry" );
			system(cmd);
			system("nvram factory");
			return ;
		}
		else
		{
			printf("press button time = %3.3f ms\r\n reboot now\r\n",period);
			snprintf(cmd, 1024, "logger EZP_USR  %s\n ","resetbutton to reboot" );
			system(cmd);	
			system("reboot");
			return ;
		}
	}

    return ;
} 

int main(int argc,char **argv)
{
	
	int fd;
	
	if (initGpioIRQ(FACTORY_DEFAULT_GPIO) !=0)
	{
		exit(EXIT_FAILURE);	
	}	
	
	fd = pidfile_acquire("/var/run/resetbuttond.pid");
	pidfile_write_release(fd);

	while (1) 
	{ 			
		pause();
	}

	exit(EXIT_SUCCESS);
}


