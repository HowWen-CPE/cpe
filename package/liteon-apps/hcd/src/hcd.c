/*
  *   Host Control Daemon for RTL8197B
  *
  *	Copyright (C)2008, Realtek Semiconductor Corp. All rights reserved.
  *
  *	$Id: hcd.c,v 1.24 2011/02/14 08:13:53 brian Exp $
  */

/*================================================================*/
/* System Include Files */

#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <string.h>
#include <sys/types.h> 
#include <sys/uio.h> 
#include <unistd.h> 
#include <signal.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <sys/wait.h> 
#include <sys/stat.h> //mark_file
#include <net/if.h>
#include "../rtk_inbandLib/ioh.h"
#include "../rtk_inbandLib/inband_if.h"

/*================================================================*/
/* Local Include Files */
#include "cmd.h"
#include "hcd.h"

/*================================================================*/
/* INBAND_HOST ---> */
#define SIOCSAPPPID     0x8b3e
static int event_channel = -1;
#define INBAND_INTF		"br-lan0"
#define INBAND_SLAVE	("001234567899")
#define INBAND_HOST		("00e04c8196c1")
#define INBAND_EVENT_TYPE 0x9001
#define INBAND_DEBUG 1
#define MAXDATALEN      1560
#define DOT11_EVENT_REQUEST 2
#define SIOCGIWIND 0x89ff

int hcd_inband_chan=0;
/*================================================================*/
/* Routine Implementations */

void real_sleep(unsigned int sec)
{
    unsigned int s;
    
	s = sec;
	while ((s = sleep(s)))
	    ;
}

static int pidfile_acquire(char *pidfile)
{
	int pid_fd;

	if(pidfile == NULL)
		return -1;

	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0) 
		printf("Unable to open pidfile %s\n", pidfile);
	else 
		lockf(pid_fd, F_LOCK, 0);

	return pid_fd;
}

static void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if(pid_fd < 0)
		return;

	if((out = fdopen(pid_fd, "w")) != NULL) {
		fprintf(out, "%d\n", getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}

static int notif_host(void)
{
	int i, ret,fd;
	int chan;
	unsigned int seq=0;
	char *data;		
	char *buf_p,rx_cmd_type;
	int count;
	struct stat ffstat;
	 int flen=0,len;

	chan = inband_open(INBAND_NETIF,"00e04c8196c1",ETH_P_RTK_NOTIFY,INBAND_DEBUG);

	if (chan < 0) {
		printf("open inband failed!\n");
		return -1;
	}


	inband_close(chan);

	return 0;
}

#ifdef CMD_LINE
static int get_token(char *line, char **token1, char **token2, char **token3, char **token4) 
{		
	int len;
	int token_idx = 0, total = strlen(line);

search_next:
	len = 0;
	while (*line== 0x20 ||*line== '\t') {
		line++;
		total--;		
	}

	if (token_idx == 0)
		*token1 = line;		
	else if (token_idx == 1)
		*token2 = line;		
	else	 if (token_idx == 2)
		*token3 = line;		
	else	 if (token_idx == 3)
		*token4 = line;			
	else
		return token_idx;

	while (total > 0 &&  *line && *line != 0x20  && *line != '\t' && *line != '\r' && *line != '\n' ) {
		line++;
		len++;
		total--;
	}

	if (len > 0) {
		*line = '\0';
		line++;
		token_idx++;
		total--;
		goto search_next;
	}

	if (strlen(line)==0 || token_idx ==4 || total <= 0)	
		return token_idx;
	else
		goto search_next;		
}

static void manual_cmd_handler(int sig_no)
{
	FILE *fp;
	char line[300], *t1=NULL, *t2=NULL, *t3=NULL, *t4=NULL;
	int num;
	char tmp_buf[100];
	char cmd_rsp[MAX_HOST_CMD_LEN];
	
	fp = fopen(CMD_FILE, "r");	
	
	if (fp == NULL) {
		DEBUG_ERR("manual cmd file empty!\n");
		return;
	}
		
	fgets(line, sizeof(line), fp);
	fclose(fp);
	unlink(CMD_FILE);
	
	num = get_token(line,  &t1, &t2, &t3, &t4);
}
#endif // CMD_LINE
	
static void show_help(void)
{
		printf("\nhcd [argument...]\n");
		printf("\t-%s\n", LOAD_DAEMON);
		printf("\n");
}

static int parse_argument(int argc, char *argv[])
{
	int argNum=1;

	if (argc <= 1) {
		show_help();
		return -1;
	}		

	// parsing argument		
	if (argv[argNum][0]=='-' && !strcmp(&argv[argNum][1], LOAD_DAEMON)) 
 		return 0;	

	else {
		show_help();
		return -1;		
	}

}

//wait child process to terminate
static void sigchld_handler(int signo)
{
	pid_t pid;
	int stat;
	
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
		//printf("child %d termniated\n", pid);
    }
}

static void inband_wait_event()
{
	int ret,data_len ;
	char cmd_type;		
	char *data_p;

	
	inband_set_cmd_id_zero(hcd_inband_chan);
	
	data_len = inband_rcv_data(hcd_inband_chan,&cmd_type,&data_p,-1); //try to rcv inband data 	

	if(data_len < 0)
		goto rcv_end;
	//do cmd process
	ret = do_cmd(cmd_type,data_p,data_len,1);  //do requested cmd , and reply a rsp_frame if need
	
rcv_end:	
	inband_free_buf(data_p, data_len);	
	return;
}

int main(int argc, char *argv[])
{ 	
	int pid_fd, pid, scr;
	char tmpbuf[100];
	int chan;
	/* INBAND_HOST ---> */

	if (parse_argument(argc, argv) != 0) 
		return 0;

//	printf("%s,%d, ****************\n",__FUNCTION__,__LINE__);

	// create pid file
	pid_fd = pidfile_acquire(PID_FILE);
	if (pid_fd < 0) {
		printf("Create PID file failed!\n");
		return 0;
	}
	pidfile_write_release(pid_fd);

	DISPLAY_BANNER;
	

	// register signal handler
#ifdef CMD_LINE
	signal(SIGUSR2, manual_cmd_handler);	
#endif
    signal(SIGCHLD, sigchld_handler);

	chan = inband_open(INBAND_NETIF,NULL,ETH_P_RTK,INBAND_DEBUG);
	if(chan < 0)
	{
	   	printf(" inband_open failed!\n");
	   	return -1;
	} else
	   	hcd_inband_chan = chan;	   

	
	//event_channel = inband_open(INBAND_INTF,NULL,INBAND_EVENT_TYPE,INBAND_DEBUG);
	//if( event_channel < 0 )
	//	return -1;

	while(1){	
		inband_wait_event();   
	}	
}
