/*
 * =====================================================================================
 *
 *       Filename:  ssd.c
 *
 *    Description:  Spectral Scan Daemon
 *
 *        Version:  1.0
 *        Created:  11/21/2011 11:23:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  S.Karthikeyan (), 
 *        Company:  Qualcomm Atheros
 *
 *        Copyright (c) 2012 Qualcomm Atheros, Inc.
 *        All Rights Reserved
 *        Qualcomm Atheros Confidential and Proprietary
 *
 * =====================================================================================
 */


#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ssd_defs.h"
#include "ssd_data.h"
#include "ssd_proto.h"
#include "ssd_utils.h"


/*daniel add begin*/
#include <unistd.h>
#include <time.h>
//#include "wlanconf.h"
#include <signal.h>
int spectral_debug_level=ATH_DEBUG_SPECTRAL;
#define SCAN_ENABLE_SPEED 5000/*unit: Kbits/s  use by dxm new func ssSpectralscanstatus ssProbescanstatus*///
/*daniel add end*/




/* main spectral daemon info */
static ssd_info_t ssdinfo;
static ssd_info_t* pinfo = &ssdinfo;


/*
 * Function     : print_usage
 * Description  : print the ssd usage
 * Input params : void
 * Return       : void
 *
 */

/*daniel add begin*/
/* dxm 2013.8.29 9:14 [START]*/
#define MAX_SCAN_TIMES 3 //used by global_channelscantimes default value,next 10lines
#define SS_SHORT_TIME 50000


int global_ssam = 0;         /*ssa:spectral scan analysis mode*/
int global_time_scan = 20000;/*time to scan non work chanel,unit:us*/
/**/
//int global_time_work = 3;    /*work channel's work time every scan(+1) if twork = 0, then not change to work channel*/ 
int global_time_work = 0;  

/**/
//int global_time_waiting = 30;
int global_time_waiting = 3;

int work_channel = 0;
int global_ssamscanmode = 0; /*0, normal mode; 
                              1, only spectral; 
                              2, only probe scan;
                             */

/**/
//int global_looptimes = 0;    /*the excute times of the whole loop*/
int global_looptimes = 20;



int global_ratecontrol = 0;  /*0, disable; 1, enable*/

/**/
//int global_channelscantimes = MAX_SCAN_TIMES;//see the above lines
int global_channelscantimes = 1;//see the above lines



/*probe scan parameters*/
static int channel_scan_timeout = 1000000;
static int get_data_one_time = 50000;
static int probeargc = 3;
#define MAX_PARA_PROBE_SCAN 10
static char *probeargv[MAX_PARA_PROBE_SCAN];


#define SWITCH_CHANNEL_ENABLED 1
#define SWITCH_CHANNEL_DISABLED 0
static int switch_channel_status = SWITCH_CHANNEL_DISABLED;

extern int global_frame_count;
extern int global_analysis_count;
extern int total_frame;

enum ScanState{
    ScanIsStop = 0,/*want to scan*/
    ScanIsRunning,/*want to stop scan*/
    ScanIsAna,/*ana scan data*/
    ScanIsProbe,/*to excute probe scan*/
    ScanIsWaiting,/*a loop is end,to waiting*/
    scanIsCSAction/*switch channel waiting ok*/
};

enum ScanMode{
    ScanHt20 = 0,
    ScanHt40Minus = 1,
    ScanHt40Plus = 2,
    Scan5G = 3
};

#define MAX_SCAN_CHANNEL_NO 4
#define SCAN_CHANNEL_SUM_HT20 4
#define SCAN_CHANNEL_SUM_HT40 3
int wChannelNO[][MAX_SCAN_CHANNEL_NO] ={{1,5,6,11},/*HT20*/
                     {5,6,11,0},/*HT40,MINUS*/
                     {1,6,7,0},/*HT40,PLUS*/
                     {0,0,0,0}};/*5G*/


typedef struct ssSampChanInfo{
    int m_ScanChannelNO;
    struct ssSampChanInfo *next;
}ssSampChanInfo;


#define SCAN_IS_ENABLE 1
#define SCAN_IS_DISABLE 0

typedef struct {
    int m_WorkChannel;
    int m_ScanChannel;
    int m_ScanTimes;
    int m_ScanState;
    u_int16_t m_ScanEnable;/**/
    u_int16_t m_ScanChSum;
    u_int16_t m_WorkMode;
    ssSampChanInfo * m_ChanInfo;
}ssScanSampInfo;

ssSpectralChanInfo *global_ChannelInfo = NULL;
ssScanSampInfo *global_ssScanInfo = NULL;

void ssSwitchChannel(ssScanSampInfo * ssScanInfo); 
void ssInitScanInfo(ssScanSampInfo * ssScanInfo);
void ssExitScan(ssScanSampInfo * ssScanInfo);
int ssaGetChannel();
int ssGetChannelNO();
void ssChannelfind(ssSpectralChanInfo *ssChannelInfo);
void ssChannelInfoAna(ssSpectralChanInfo *ssChannelInfo);
void ssChannelInfoUpdate(ssSpectralChanInfo *ssChannelInfo, SPECTRAL_SAMP_MSG *msg);
void ssChannelInfoInit(ssScanSampInfo * ssScanInfo);
int ss_radio_mode(char *ifname);
void ssSetMode( void );
int ActionSwitchChannel(int channel);
u_int16_t get_WorkMode();
int get_WorkChannel();
int ss_associatedstatus(char *ifname);
void ssClearInfo();
void spectral_signal_delete();

/* Final output result */
struct channel_avgrssi_pair
{	struct channel_avgrssi_pair *next;
	struct channel_avgrssi_pair *pre;
	int channel;
	int avg_rssi;
};

typedef struct channel_avgrssi_pair *channel_avgrssi_pair_t;

extern void initProbeScan();
extern void printProbeScan();
extern void releaseProbeScan();
extern channel_avgrssi_pair_t getProbeResult2_4G();
extern channel_avgrssi_pair_t getProbeResult5G();
extern int probe_scan_main(int	argc,char **	argv);

//declare for no warning
void ss_free(void *sPtr);
void ssStopScan(void);


/* dxm 2013.8.29 9:14 [END]*/
/*daniel add end*/


/*daniel add begin*/
#ifndef ATH_DEFAULT_RADIO
#define ATH_DEFAULT_RADIO "wifi0"
#endif
#ifndef ATH_DEFAULT_VAP
#define ATH_DEFAULT_VAP "ath0"
#endif
char global_radio[] = ATH_DEFAULT_RADIO;
char global_vap[] = ATH_DEFAULT_VAP;
/*daniel add end*/






void print_usage(void)
{
    printf("ssd - usage\n");
    printf("----------------------------------------------------------------------------------\n\n\n");
  //  printf("channels        - use with AirDefense GUI (switches the between channels 5 and 11)\n");
 //   printf("userssi         - use RSSI in the power calculations\n");
    //printf("classify        - run classifier on the current channel (stand alone option)\n");
    //printf("eacs            - run the EACS algorithm (stand alone option)\n");
 //   printf("rawfft          - report raw FFT data as is to the GUI, (debug only)\n");
  //  printf("minpwr <value>  - threshold for reporting power\n");
 //   printf("maxhold <value> - set the max hold interval\n");
//    printf("athui           - support Atheros GUI\n");
 //   printf("debug <1/2/3>   - set the debug level\n");

    printf("use 'ssd ssamode debug 1' + 'following parameters '     \n");
    printf("For example :   ssd ssamode debug 1 ssamode 0 tscan 20000 action en looptimes 50   \n");
    /*daniel add begin*/
    /* dxm 2013.9.3 9:52 [START]*/
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n\n");
   // SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ssamode-spectral scan & probe scan, and when scanning the client can access.\n");
   // SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"       -spectral scan is only for 2.4G mode.\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ssamode parameters start:\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"scanmode <value>-ctl(0~2)                 0,spectral scan and probe scan; 1,only spectral scan; 2, only probe scan.\n"); 
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"tscan <value>-(20000~100000)              scan time of spectral scan.\n");  
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"action <value>-(en/dis)                 this will set whether to switch channel after finding a good channel.\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"looptimes <value>-(1~200)                 the excute times of the whole loop. default 0,run all the time.\n");
   //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"looptimes <value>-ctl(0~). the excute times of the whole loop. default 0,run all the time.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"chscantimes <value>-ctl(1~100),default 3. scan times of every channel each time.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ratecontrol <value>-ctl(0~1). this will set whether to scan when the rate is high.\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"tscan <value>-time(10'000~10'000'000us).spectral. scan time of spectral scan.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"twork <value>-time(0~10'000s).spectral. work time after spectral scan.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"twait <value>-time(0~10'000s). waiting time when the loop is finished.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"timeout <value>-time(us).probe. time out for probe scan.\n");
    //SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"onetime <value>-time(us).probe. probe scan excute time.\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ssamode parameters end.\n");    
    printf("----------------------------------------------------------------------------------\n\n\n");  
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    /* dxm 2013.9.3 9:52 [END]*/
    /*daniel add end*/
	
}


/*daniel add begin*/
static void spectral_kill_handler(int sig)
{    
    char cmd[128];
    sprintf(cmd, "spectraltool -i %s stopscan", global_radio);
    system(cmd);
    sprintf(cmd,"sssetchannel %s channel %1d",global_vap, work_channel);
    system(cmd);
    ss_free(global_ssScanInfo->m_ChanInfo);
    ss_free(global_ssScanInfo);
    ss_free(global_ChannelInfo);
    global_ssScanInfo = NULL;
    exit(0);
}   
/*daniel add end*/





/*
 * Function     : get_args
 * Description  : validates user input values for ssd
 * Input params : pointer to user arguments and argument count
 * Return       : 0 for valid arguments, -1 for error
 *
 */
int get_args(int argc, char* argv[])
{
    int i = 1;
    int status = 0;
    ssd_config_t *p = &ssdinfo.config;

    while (i < argc) {

        if (streq(argv[i], "rawfft")) {
            set_config(p, raw_fft, 1);
        }

        if (streq(argv[i], "scale")) {
            set_config(p, scale, 1);
        }

        if (streq(argv[i], "userssi")) {
            set_config(p, use_rssi, 1);
        }

        if (streq(argv[i], "flip")) {
            set_config(p, flip, 1);
        }

        if (streq(argv[i], "rssi_only")) {
            set_config(p, rssi_only, 1);
        }

        //new function than before
        if (streq(argv[i], "pwrperbin")) {
            set_config(p, power_per_bin, 1);
        }
         //new function than before
        if (streq(argv[i], "athui")) {
            set_config(p, atheros_ui, 1);
        }

//#ifdef  SSD_EXPERIMENTAL
        if (streq(argv[i], "eacs")) {
            set_config(p, eacs, 1);
            if (argc > 1) {
              printf("err : eacs should not be used with other settings\n");
              status = -1;
            }
        }
//#endif  /* SSD_EXPERIMENTAL */

        if (streq(argv[i], "channels")) {
            set_config(p, change_channel, 1);
        }

//#ifdef  SSD_EXPERIMENTAL
        if (streq(argv[i], "classify")) {
            set_config(p, classify, 1);
            if (argc > 2) {
               printf("err : classify should not be used with other settings\n");
               status = -1;
            }
        }
//#endif  /* SSD_EXPERIMENTAL */

       /*daniel add begin*/
       if(streq(argv[i], "debug"))  {
	    spectral_debug_level=(ATH_DEBUG_SPECTRAL << (atoi(argv[i+1])));	    
        probeargv[probeargc++] = "debug";
        probeargv[probeargc++] = argv[i+1];
            i++;
        }
        /*daniel add end*/
        if (streq(argv[i], "minpwr")) {
            int val = atoi(argv[i + 1]);
            set_config(p, minpower, val);
            i++;
        }

        if (streq(argv[i], "maxhold")) {
            int val = atoi(argv[i + 1]);
            set_config(p, maxhold_interval, val);
            i++;
        }

        if (streq(argv[i], "help")) {
            print_usage();
            exit(0);
        }




       /*daniel add begin*/
	/* dxm 2013.8.29 9:21 [START] spectral scan */
	if(streq(argv[i], "ssamode"))  
	{
		global_ssam = 1;
		//daniel modify
		//global_is_classify = 1;
		pinfo->config.classify= 1;
	}
	
	/*scan time parameters*/
	if(streq(argv[i], "tscan"))  
	{
		global_time_scan = (atoi(argv[i+1]));		
		//if((global_time_scan < 10000 ) || ( global_time_scan > 10000000))
		/**/
		if((global_time_scan < 20000 ) || ( global_time_scan >100000))	
		{
                      printf(" Not in Legal scope,use default tscan value 20000 !\n");
			global_time_scan = 20000;
		}
		i++;
	}
	if(streq(argv[i], "twork"))  
	{
		global_time_work = (atoi(argv[i+1]));
		if((global_time_work < 0 )||(global_time_work > 10000 ))
		{
                       /**/
			//global_time_work = 1;
			global_time_work = 0;
		}
		i++;
	}

	if(streq(argv[i], "twait"))  
	{
		global_time_waiting = (atoi(argv[i+1]));
		if((global_time_waiting< 0 )||(global_time_waiting > 10000 ))
		{
                       /**/
			//global_time_waiting = 30;
			global_time_waiting = 3;
		}
		i++;
	}
	
	/*time for probe scan*/
	if(streq(argv[i], "timeout"))  
	{
		channel_scan_timeout = (atoi(argv[i+1]));
		probeargv[probeargc++] = "timeout";
		probeargv[probeargc++] = argv[i+1];
		printf("timeout: %d\n", channel_scan_timeout);
		i++;
	}

	if(streq(argv[i], "onetime"))  
	{
		get_data_one_time = (atoi(argv[i+1]));
		probeargv[probeargc++] = "onetime";
		probeargv[probeargc++] = argv[i+1];
		printf("onetime: %d\n", get_data_one_time);
		i++;
	} 

	/*channel switch enable & disable*/
	if(streq(argv[i], "action"))  
	{
		if(streq(argv[i+1], "en"))	
		{
			switch_channel_status = SWITCH_CHANNEL_ENABLED;
		}
		else
		{
			switch_channel_status = SWITCH_CHANNEL_DISABLED;
		}
		i++;
	}

	/*scan mode select*/
	if(streq(argv[i], "scanmode"))	
	{
		global_ssamscanmode = (atoi(argv[i+1]));
		if((global_ssamscanmode < 0 )||(global_ssamscanmode > 2 ))
		{
                      printf(" Not in Legal scope,use default scanmode value 0 !\n");
			global_ssamscanmode = 0;
		}		 
		i++;
	}

	/*init work channel, if not set, use the device setting*/
	if(streq(argv[i], "workchannel"))  
	{
		work_channel = (atoi(argv[i+1]));
		if((work_channel< 1 )||(work_channel > 300 ))
		{
			work_channel = 6;
		}		 
		i++;
	}
	
	if(streq(argv[i], "looptimes"))  
	{
		global_looptimes = (atoi(argv[i+1]));
               /**/
		/*
		if(global_looptimes < 0 )
		{
			global_looptimes = 0;
		}	
               */
               if((global_looptimes< 1 )||(global_looptimes > 200 ))
		{
                      printf(" Not in Legal scope,use default looptimes value 20 !\n");
			global_looptimes = 20;
		}	

		
		i++;
	}
	
	if(streq(argv[i], "ratecontrol"))  
	{
		global_ratecontrol = (atoi(argv[i+1]));
		if(global_ratecontrol != 1)
		{
			global_ratecontrol = 0;
		}		 
		i++;
	}

	if(streq(argv[i], "chscantimes"))  
	{
		global_channelscantimes = (atoi(argv[i+1]));
		if((global_channelscantimes < 1 )||(global_channelscantimes > 100 ))
		{
                       /**/
			//global_channelscantimes = 3;
			global_channelscantimes = 1;
		}		 
		i++;
	}
       /* dxm 2013.8.29 9:21 [END]*/
       /*daniel add end*/

		

        i++;
    }

    return status;
}

/*
 * Function     : ssd_alarm_handler
 * Description  : signal handler (TODO : Replace with timer function?)
 * Input params : signal type
 * Return       : void
 *
 */
 #if 0
static void ssd_alarm_handler(int sig)
{
        /*daniel add begin*/
	/* dxm 2013.8.29 9:30 [START]*/
	if( global_ssam)
	{	      
		return;
	}
	/* dxm 2013.8.29 9:30 [END]*/
       /*daniel add end*/
    
    pinfo->stats.alarm++;
    ualarm(SSD_USEC_ALARM_VAL, 0);
}
 #endif

static void ssd_alarm_handler(int sig)
{
       //  printf("==========ssd_alarm_handler=========\n");
	 /*daniel add begin*/
       ssSwitchChannel(global_ssScanInfo);	
       /*daniel add end*/ 
}


/*
 * Function     : ssd_start_eacs_monitor_scan
 * Description  : TBD
 * Input params :
 * Return       :
 *
 */
void ssd_start_eacs_monitor_scan(void)
{
    not_yet();
}

/*
 * Function     : ssd_init_config
 * Description  : initializes the default configuration for ssc
 * Input params : pointer to ssdinfo
 * Return       : void
 *
 */
void ssd_init_config(ssd_info_t* p)
{
    ssd_config_t* pconfig = GET_CONFIG_PTR(p);

    pconfig->current_freq   = CHANNEL_11_FREQ;
    pconfig->prev_freq      = CHANNEL_11_FREQ;
    pconfig->minpower       = DEFAULT_MINPOWER;
    pconfig->use_rssi       = TRUE;
    pconfig->maxhold_interval = DEFAULT_MAXHOLD_INT;
    pconfig->eacs = FALSE;
    pconfig->atheros_ui = FALSE;
}

/*
 * Function     : main
 * Description  : entry point for ssd program
 * Input params : user input
 * Return       :
 *
 */
int main(int argc, char* argv[])
{

    /* inet socket */
    int listener;
    int app_fd;
    int on=1;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t addrlen;

    /* netlink socket */
    struct sockaddr_nl  src_addr;
    struct sockaddr_nl  dst_addr;
    struct nlmsghdr     *nlh;
    struct msghdr       msg;
    struct iovec        iov;
    int nl_spectral_fd;

    int     fdmax;      /* maximum fd number */
    fd_set  master;     /* Master FD set */
    fd_set  read_fds;   /* Read FD set */

    /* Init sample info pointer */
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);

    /* init receive buffer */
    u_int8_t recv_buf[MAX_PAYLOAD] = {'\0'};


    /* initialize default settings */
    ssd_init_config(pinfo);
    ssd_init_silent_samp_msg(pinfo);
    ssd_init_ssd_samp_msg_buffers(pinfo);

    /* validate user settings */
    if (get_args(argc, argv) != 0) {
        printf("configuration error\n");
        return 1;
    }

    print_args(pinfo);
    

    /*daniel add begin*/





    
    ssStopScan();/*stop scan*/   
    if(work_channel != 0)        /*set work channel*/
    {
        sprintf(recv_buf,"iwconfig %s channel %1d",global_vap, work_channel);
        system(recv_buf);
    }
    
    if(global_ssam)
    {
        probeargv[0] = "channel_scan";
        probeargv[2] = "scanning";        
        sleep(2);/*wait...*/
        ssSetMode();
    }

    //daniel comment:only print some info, this can don't care
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"SPECTRAL Server is up ... built at %s %s\n", __DATE__, __TIME__);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"eacs=%d debug=%d rawfft=%d scaledata=%d userssi=%d (100-x)=%d indexdata=%d onlyrssi=%d change_channels=%d minpwrlevel=%d\n",\
                                        pinfo->config.eacs, spectral_debug_level, pinfo->config.raw_fft, pinfo->config.scale, pinfo->config.use_rssi, pinfo->config.flip, pinfo->config.index_data, \
                                        pinfo->config.rssi_only, pinfo->config.change_channel, pinfo->config.minpower);
	     
     //  pinfo->config.maxhold_interval
    //daniel comment:orignal process in WP854 SDK
    if (argc == 1 || pinfo->config.classify) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL, "Running in classifier mode to monitor current channel\n");
        pinfo->config.classify=1;
    }

   //daniel comment:only print some info, this can don't care
   
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"classify=%d maxhold=%d global_ssam=%d \n", \
                     pinfo->config.classify,pinfo->config.maxhold_interval,global_ssam);
	
    if(global_ssam)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"scanmode =%d work_channel=%d looptimes=%d ratecontrol=%d action %d\n",\
                         global_ssamscanmode, work_channel, global_looptimes,global_ratecontrol,switch_channel_status);
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"channel_scan_times=%d time_work =%d time_scan=%d time_wait=%d probetimeout=%d probeonetime=%d\n",\
                         global_channelscantimes,global_time_work,global_time_scan,global_time_waiting,channel_scan_timeout,get_data_one_time);
    }

    //daniel comment:only print some info, this can don't care
    if (pinfo->config.eacs==1) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL, "EACS mode, only monitor current channel\n");
    }

    /*daniel add end*/



    /* init socket interface */
    listener = socket(PF_INET, SOCK_STREAM, 0);

    if (listener < 0) {
        perror("err : unable to open socket\n");
        exit(EXIT_FAILURE);
    }

    /* set socket options */
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("err : socket option failed\n");
        close(listener);
        exit(EXIT_FAILURE);
    }

    /*daniel add begin*/
    
    if(streq(global_vap, "ath0")) 
    {//about 80line
    /*daniel add end*/
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(ATHPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* bind the listener socket */
    if (bind(listener, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("err : bind error\n");
        close(listener);
        exit(EXIT_FAILURE);
    }

    /* start listening */
    if (listen(listener, BACKLOG) == -1) {
        perror("err : listen fail\n");
        close(listener);
        exit(EXIT_FAILURE);
    }

    /* init netlink connection to spectral driver */
    nl_spectral_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ATHEROS);

    if (nl_spectral_fd < 0) {
        perror("err : unable to open netlink_atheros socket\n");
        close(listener);
        exit(EXIT_FAILURE);
    }

    /* init netlink socket */
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family  = PF_NETLINK;
    src_addr.nl_pid     = getpid();
    src_addr.nl_groups  = 1;

    if (bind(nl_spectral_fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("err : unable to bind (netlink)");
        close(nl_spectral_fd);
        close(listener);
        exit(EXIT_FAILURE);
    }


    /* alloc space nlmsg */
    nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));

    nlh->nlmsg_len      = NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG));
    nlh->nlmsg_pid      = getpid();
    nlh->nlmsg_flags    = 0;

    iov.iov_base    = (void*)nlh;
    iov.iov_len     = nlh->nlmsg_len;

    memset(&dst_addr, 0, sizeof(dst_addr));

    dst_addr.nl_family  = PF_NETLINK;
    dst_addr.nl_pid     = 0;
    dst_addr.nl_groups  = 1;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name    = (void *)&dst_addr;
    msg.msg_namelen = sizeof(dst_addr);
    msg.msg_iov     = &iov;
    msg.msg_iovlen  = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(nl_spectral_fd, &master);
    FD_SET(listener, &master);

    fdmax = ((listener > nl_spectral_fd)?listener:nl_spectral_fd);

    //daniel comment:used in wp854 SDK ,may be not useful
    /*
    current_server_state = SPECTRAL_SERVER_STATE_IDLE;
    total_channel_switches=0;
    init_freq_message_pools();
    init_dummy_samp_msg();
    */


    /*daniel add begin*/
    // corresponsed to "if(streq(global_vap, "ath0")) ",about 80 line above
    }
   /*daniel add end*/


   // printf("==========1=========\n");
    alarm(1);
    
    /* register alarm handler */
    if (!pinfo->config.eacs) 
    {
     //  printf("==========2=========\n");
       signal(SIGALRM, ssd_alarm_handler);
	/*daniel add begin*/	
	signal(SIGINT, spectral_kill_handler);
	signal(SIGQUIT, spectral_kill_handler);
	signal(SIGTERM, spectral_kill_handler);
    	}
       /*daniel add end*/
       
    else
    	{
        ssd_start_eacs_monitor_scan();
    	}
    /* init the state */
    set_state(pinfo, IDLE);
   
    /*daniel add begin*/   
    if(global_ssam)
    {
      //  printf("==========3=========\n");
	#if 1
     //   ssSwitchChannel(global_ssScanInfo);
	#else 

	/*daniel delete begin*/
        #if 1
	//in speed.c
	//initialize thread compute function
     //   extern int ss_spectral_thread_init();
    //    ss_spectral_thread_init();
	 #endif
        /*daniel delete end*/
        
       // void spectral_init_timer();        
      //  spectral_init_timer();
	#endif


	
        app_fd = 0;
    }
  
    if(streq(global_vap, "ath0")) 
    {
     //   printf("==========4=========\n");

	 SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d fdmax=%d listener=%d nl_sock_fd=%d \n", __func__, __LINE__, fdmax, listener, nl_spectral_fd);
    /*daniel add end*/
    for(;;) {

        int recvd_bytes = 0;
        int i = 0;

        read_fds = master;
    //    printf("==========5=========\n");
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
    //        printf("==========5.1=========\n");
            if (errno == EINTR) {
    //            printf("==========6=========\n");
                /*
                 * We got interrupted by signal handler
                 * - In classify mode send stored spectral data to gui
                 * - In non classify mode send a silent data to the gui
                 */

                ssd_config_t    *pconfig = GET_CONFIG_PTR(pinfo);

                if (!pinfo->config.classify) {
                    ssd_update_silent_samp_msg(pinfo, pconfig->current_freq);
                    ssd_send_normal_spectral_data_to_client(pinfo, &pinfo->silent_msg, app_fd);
	                } else {
	                    /*daniel add begin*/
	                    // if (psinfo->total_channel_switches > 1) {
			     if((psinfo->total_channel_switches > 1) && (app_fd != 0) && ( global_ssam == 0)){						    
			     /*daniel add end*/ 	
                        ssd_send_classified_spectral_data_to_client(pinfo, NULL, app_fd);
                    }
                }
	            } 
		    else {
				 printf("==========7=========\n");
                perror("select");
			 /*daniel add begin*/		
			 ssExitScan(global_ssScanInfo);
			 /*daniel add end*/
                exit(EXIT_FAILURE);
            }
            continue;
        }


        /*
         * Loop through all the FD set and receive relevant data 
         * - Data can be received from gui application
         * - Data can be received from spectral driver
         * - Data can be received from listener port
         */

        for (i = 0; i <= fdmax; i++) {
     //   printf("==========8=========\n");
            /* We have data to handle */
            if (FD_ISSET(i, &read_fds)) {
        //      printf("==========9=========\n");
                if (i == listener) {
                    /* Accept new connection 
                     * TODO : Make sure only one connection is accepted
                     */
                    addrlen = sizeof(client_addr);
                     printf("==========10=========\n");
                    if ((app_fd = accept(listener, (struct sockaddr*)&client_addr, &addrlen)) == -1) {
                        perror("err: unable to accept connections");
				printf("==========11=========\n");		
                    } else {
                        FD_SET(app_fd, &master);

                        fdmax = (app_fd > fdmax)?app_fd:fdmax;

                        printf("=12=>>>ssd: new connection from %s on socket %d\n",
                                inet_ntoa(client_addr.sin_addr), app_fd);

                        set_state(pinfo, CONNECTED_TO_GUI);
                    }

                } else {
                //  printf("==========13=========\n");	
                    if (i == nl_spectral_fd) {
                        /*
                         * Received data from spectral driver, parse the 
                         * spectral data 
                         */
                  //   printf("==========14=========\n");	
                        ssd_handle_spectral_data(pinfo, recv_buf, &recvd_bytes, app_fd, nl_spectral_fd);
                        continue;
                    }

                    if ((recvd_bytes = recv(i, recv_buf, sizeof(recv_buf), 0)) <= 0)  {

                        /*
                         * Receive error :
                         */
                         printf("==========15=========\n");	
                        if (recvd_bytes == 0) {
                            printf("ssd : socket %d hung up\n", i);
                            set_state(pinfo, IDLE);
                        } else {
                            perror("err: recv");
                        }

                        close(i);
                        FD_CLR(i, &master);

                    } else {
                        /*
                         * Received data form gui
                         * Handle the request commands
                         */
                         printf("===16=====\n");
                        set_state(pinfo, SCAN_INIT);
                        ssd_handle_client_request_msg(pinfo, recv_buf, &recvd_bytes);
                        continue;
                    }
                }
            } /* if read_fds */

        } /* for */

    }   /* forever */
	/*daniel add begin*/
	//end if streq(global_vap,"ath0")
    	}
	
	else
	{
	//		printf("===17=====\n");
		for(;;)
		{
		   sleep(10);
		}
	}
	/*daniel add end*/

//	printf("===18=====\n");
    /* cleanup */
    close(listener);
    close(app_fd);
    free(nlh);
    return 0;
}



/*daniel add begin*/

/*============================================================================*/
/*  fuction name :  spectral scan                                             */
/*  input :                                                                   */
/*  output :                                                                  */
/*  return :                                                                  */
/*  description :                                                             */
/*  author :                                                                  */
/*============================================================================*/
/*============================================================================*/
void ss_free(void *sPtr)
{
    if(NULL == sPtr) 
        return;
    free(sPtr);
}

void ssExitScan(ssScanSampInfo * ssScanInfo)
{
    char cmd[128];
    ssStopScan();
//    system("rm /tmp/ssinterf.json");
    sprintf(cmd,"sssetchannel %s channel %1d",global_vap, work_channel);
    system(cmd);
    ss_free(global_ssScanInfo->m_ChanInfo);
    ss_free(global_ssScanInfo);
    ss_free(global_ChannelInfo);
    global_ssScanInfo = NULL;
}

void ssInitScanInfo(ssScanSampInfo * ssScanInfo)
{
   int i;
   ssSampChanInfo * ssPtr=NULL;
   if(NULL == ssScanInfo)
   {
       SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s ssScanInfo is NULL\r\n", __func__);       
       ssExitScan(global_ssScanInfo);
       exit(0);
   }
   ssScanInfo->m_ScanState = ScanIsWaiting;
   ssScanInfo->m_ScanTimes = 0;
   ssScanInfo->m_WorkChannel = work_channel;/*TBD*/
   ssScanInfo->m_ScanChannel = wChannelNO[ssScanInfo->m_WorkMode][0];
   ssScanInfo->m_ChanInfo = (ssSampChanInfo *)malloc(sizeof(ssSampChanInfo) * MAX_SCAN_CHANNEL_NO);
   memset(ssScanInfo->m_ChanInfo, 0 ,(sizeof(ssSampChanInfo) * MAX_SCAN_CHANNEL_NO));
   ssPtr = ssScanInfo->m_ChanInfo;

   if(SCAN_IS_DISABLE == ssScanInfo->m_ScanEnable)
   {
        return;
   }
   
   for( i = 0; i < MAX_SCAN_CHANNEL_NO; i++)
   {
       ssPtr->m_ScanChannelNO = wChannelNO[ssScanInfo->m_WorkMode][i];
       if(i == (MAX_SCAN_CHANNEL_NO - 1))
       {
           ssPtr->next = NULL;
       }
       else
       {
           ssPtr->next = ssPtr + 1;
           ssPtr++;
       }
   }
   
   return;
}


void ssStopScan(void)
{
    char sys_cmd[128];
    sprintf(sys_cmd, "spectraltool -i %s stopscan", global_radio);
    system(sys_cmd);
}

void ssChangeChannel(ssScanSampInfo * ssScanInfo)
{
    int i;
    ssSampChanInfo * ssPtr=NULL;
    if(NULL == ssScanInfo)
        return;        
    
    ssPtr = ssScanInfo->m_ChanInfo;
    if(0 == ssScanInfo->m_ScanChannel)
    {

	/*daniel amend begin*/
	//ssScanInfo->m_ScanChannel == ssPtr->m_ScanChannelNO;
	ssScanInfo->m_ScanChannel = ssPtr->m_ScanChannelNO;
        /*daniel amend end*/
    }

    if( ssScanInfo->m_ScanTimes < global_channelscantimes)
    {
        return;
    }
    
    ssScanInfo->m_ScanTimes = 0;

    for(i = 0; i < ssScanInfo->m_ScanChSum; i++)
    {
        
        if(ssScanInfo->m_ScanChannel == ssPtr->m_ScanChannelNO)
        {
            if(( NULL == ssPtr->next ) || (0 == ssPtr->next->m_ScanChannelNO))
            {
                ssScanInfo->m_ScanChannel = ssScanInfo->m_ChanInfo->m_ScanChannelNO;
                ssScanInfo->m_ScanState = ScanIsWaiting;
            }
            else
            {
                ssScanInfo->m_ScanChannel =  ssPtr->next->m_ScanChannelNO;
            }
            break;
        }
        
        if( NULL == ssPtr->next )
        {
            ssScanInfo->m_ScanChannel = ssScanInfo->m_ChanInfo->m_ScanChannelNO;
            ssScanInfo->m_ScanState = ScanIsWaiting;
            break;
        }
        else
        {
            ssPtr = ssPtr->next;
        }
    }

    return;    
}

void ssStartScan(void)
{
    char sys_cmd[128];
   // sprintf(sys_cmd, "spectraltool -i %s count %d", global_radio, 128);
   //printf("spectraltool -i %s scan_count %d", global_radio, 128);
   sprintf(sys_cmd, "spectraltool -i %s scan_count %d", global_radio, 128);
    system(sys_cmd);
    //printf("spectraltool -i %s scan_period %d", global_radio,12);
    //sprintf(sys_cmd, "spectraltool -i %s period %d", global_radio,12);

	//printf("spectraltool -i %s scan_period %d", global_radio,12);
    sprintf(sys_cmd, "spectraltool -i %s scan_period %d", global_radio,12);
    system(sys_cmd);

	//printf("spectraltool -i %s fft_period %d",global_radio,2);
   // sprintf(sys_cmd, "spectraltool -i %s fftperiod %d",global_radio,2);
   sprintf(sys_cmd, "spectraltool -i %s fft_period %d",global_radio,2);
    system(sys_cmd);



	//printf("spectraltool -i %s startscan", global_radio);
    sprintf(sys_cmd, "spectraltool -i %s startscan", global_radio);
    system(sys_cmd);
}

int ssSpectralscanstatus(void)
{
    if(SCAN_IS_ENABLE != global_ssScanInfo->m_ScanEnable)
    {
        return 0;
    }
   
    if(global_ssamscanmode == 2)
    {
        return 0;
    }
    

  /*daniel delete begin*/
  #if 0
   if(1 == global_ratecontrol)
    {
        if(ss_Get_speed() >= SCAN_ENABLE_SPEED)
        {
            return 0;
        }
    }
   #endif
   /*daniel delete end*/

	
    return 1;
}

int ssProbescanstatus(void)
{
    if(global_ssamscanmode == 1)
    {
        return 0;
    }

    /*daniel delete begin*/
    #if 0
    
    if(1 == global_ratecontrol)
    {
        if(ss_Get_speed() >= SCAN_ENABLE_SPEED)
        {
            return 0;
        }
    }
    #endif
   /*daniel delete end*/

	
    return 1;
}


void ssSwitchChannel(ssScanSampInfo * ssScanInfo)
{
    int rc;
    char cmd[256]={'\0'};
    static unsigned int loopcount = 0;

//   printf("======ssSwitchChannel=======\n");

    alarm(0);
    ualarm(0,0);
    /*daniel add begin*/
    /* Init sample info pointer */                         
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);
   /*daniel add end*/
	
    if(!global_ssam)
        return;

    ///*daniel modify begin*/
    //extern void clear_max_rssi_data(void);
    //clear_max_rssi_data();
    ssd_clear_max_rssi_data(pinfo);
    /*daniel modify end*/

    switch(ssScanInfo->m_ScanState)
    {
        case ScanIsRunning:
        {
	//     printf("===in case ScanIsRunning===\n");
	     ssStopScan();
            if(global_time_work > 0)
            {
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap , ssScanInfo->m_WorkChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"scan:%d->work:%d\n",\
                                 ssScanInfo->m_ScanChannel,ssScanInfo->m_WorkChannel);
                ssScanInfo->m_ScanState = ScanIsAna; 
                alarm(1);
                break;
            }
        }
        case ScanIsAna:
        {
	//    printf("===in case ScanIsAna===\n");
	    ssScanInfo->m_ScanState = ScanIsStop; 
            ssChangeChannel(ssScanInfo);/*no modify,can update m_ScanState to waiting*/
            ssChannelInfoAna(global_ChannelInfo);

			
           /*daniel delete begin*/
           #if 0			
            ss_creat_json();
            web_clear_interf_rsp(0);
	    #endif
           /*daniel delete end*/
			
            if(0 == global_time_work)
            {
                ualarm(SS_SHORT_TIME,0);
            }
            else
            {
                alarm(global_time_work);
            }
            break;
        }        
        case ScanIsStop:
        {
     //       printf("===in case ScanIsStop===\n");
	     if(1 == ssSpectralscanstatus())
            {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"frame %5d, %5d, %5d\n",total_frame,global_frame_count,global_analysis_count);
                global_frame_count = 0;
                global_analysis_count = 0;

               /*daniel delete begin*/
               #if 0
				
                web_clear_interf_rsp(ssScanInfo->m_ScanChannel);

               #endif
               /*daniel delete end*/

				
                ssScanInfo->m_ScanTimes++;
                ssScanInfo->m_ScanState = ScanIsRunning;
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap, ssScanInfo->m_ScanChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"work:%d->scan:%d\n",ssScanInfo->m_WorkChannel,ssScanInfo->m_ScanChannel);
            }
            else
            {
                ssScanInfo->m_ScanState = ScanIsWaiting;
            }

            
            #if 0/*TBD, when ath is down*/
            if(-1 == ss_associatedstatus(global_vap))
            {
                printf("scan but %s is not assoitated\n",global_vap);                
                ssExitScan(global_ssScanInfo);
                exit(0);
            }           
            #endif
                
            if(1 == ssSpectralscanstatus())
            {
           //printf("~~~~~ssStartScan~~~~~~~");
				ssStartScan();
            }
            else
            {
            //printf("~~~~~ssStopScan~~~~~~~");
                ssStopScan();
                ssClearInfo();
            }
            
            if( global_time_scan < 1000000)
            {
                ualarm(global_time_scan,0);
            }
            else
            {
                alarm(global_time_scan/1000000);
            }
            break;            
        }        
        case ScanIsWaiting:
        {
	//   printf("===in case ScanIsWaiting===\n");
	    /*daniel delete begin*/
           #if 0          
	    web_clear_interf_rsp(0);
           #endif
           /*daniel delete end*/
			
            if( Scan5G != global_ssScanInfo->m_WorkMode)
            {
            /*daniel delete begin*/
           #if 0 
				ss_creat_json(); 
	   #endif
           /*daniel delete end*/   
   
            }
            ssChannelfind(global_ChannelInfo);
            releaseProbeScan();
            if( global_time_waiting > 0)
            {
                ssScanInfo->m_ScanState = scanIsCSAction;
                alarm(1);
                break;
            }
        }
        case scanIsCSAction:
        {
	//    printf("===in case scanIsCSAction===\n");
            if( switch_channel_status == SWITCH_CHANNEL_ENABLED)
            {
                sprintf(cmd,"iwpriv %s vap_doth 1", global_vap);
                system(cmd);
            }
            if((loopcount >= global_looptimes) && (global_looptimes != 0))
            {            
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap , ssScanInfo->m_WorkChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Whole loop excute %d times and exit.\n",loopcount);
                ssExitScan(global_ssScanInfo);
                exit (0);
            }
            loopcount++;
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,\
                            "\n----------------Loop start,count %d-------------------\n",loopcount);
            ssScanInfo->m_ScanState = ScanIsProbe;
            if(0 == global_time_waiting)
            {
                ualarm(SS_SHORT_TIME,0);
            }
            else
            {
                alarm(global_time_waiting);
            }
            break;
        }
        case ScanIsProbe:
        {        
	//    printf("===in case ScanIsProbe===\n");
            rc = ssGetChannelNO(global_vap);
            if(rc == -1)
            {
               printf("work channel get error after action\n");           
            }
            else if(( global_time_work != 0) && (global_time_waiting !=0 ))
            {
                work_channel = rc;
                global_ssScanInfo->m_WorkChannel = work_channel;   
            }
            
            initProbeScan();
            /*excute probe scan*/
            if(1 == ssProbescanstatus())
            {
                probeargv[1] = global_vap;
                probe_scan_main(probeargc,probeargv);
                ssScanInfo->m_ScanState = ScanIsStop;
                alarm((channel_scan_timeout )/1000000 + 1);
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"probescan excute complete.\n");
            }
            else if(1 == ssSpectralscanstatus())
            {            
                ssScanInfo->m_ScanState = ScanIsStop;
                ualarm(SS_SHORT_TIME,0);
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"probescan dis,excute spectralscan.\n");
            }
            else
            {
                ssScanInfo->m_ScanState = ScanIsWaiting;
                alarm((channel_scan_timeout )/1000000 + 1);
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"scan disable\n");


		/*daniel delete begin*/
                #if 0	
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"tx/rx rate %lu\n",ss_Get_speed());
                #endif
             /*daniel delete end*/
  
			}
            break;
        }
        default:
        {
	//    printf("===in case default===\n");
            sprintf(cmd,"sssetchannel %s channel %1d",global_vap, ssScanInfo->m_WorkChannel);
            system(cmd);
            ssScanInfo->m_ScanState = ScanIsWaiting;
            ssStopScan();
            alarm(1);
            break;
        }

    } 
    /*daniel add begin*/
     //total_channel_switches++;
     psinfo->total_channel_switches++;
    /*daniel add end*/

}

/*============================================================================*/
/*  fuction name :  get the radio settings                                    */
/*============================================================================*/
/*============================================================================*/
int ssGetChannelNO(char *ifname)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int freq = 0;
	int channelNO = -1;
	int mode = 0;
	
	if (ifname == NULL)
		return -1;

	tmp_p = (char *)buf;

	
    /*get ap mode*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $2}' | cut -c 11", ifname);
	file = popen(tmp_p, "r");

	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 8, file) != NULL) {
				mode = atoi(tmp_p);
		}
	} 
	else
	{
	    return -1;
	}
	pclose(file);

	
    /*get channel*/
    if(2 == mode)
    {
    	memset(buf, 0, sizeof(buf));
    	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $2}' | cut -c 13-16", ifname);
    	file = popen(tmp_p, "r");
    	if (file != NULL) {
    		memset(buf, 0, sizeof(buf));
    		if (fgets(tmp_p, 8, file) != NULL) {
    				freq = atoi(tmp_p);
    		}
    	} 
    	else
    	{
    	    return -1;
    	}
    	pclose(file);
    	
        if((freq > 472) || (freq < 407))
            return -1;
        channelNO = (freq - 407)/5;   
    }

    if( 5 == mode )
    {
    	memset(buf, 0, sizeof(buf));
    	sprintf(tmp_p, "iwlist %s channel | grep Current | awk '{print $5}'", ifname);
    	file = popen(tmp_p, "r");
    	if (file != NULL) {
    		memset(buf, 0, sizeof(buf));
    		if (fgets(tmp_p, 8, file) != NULL) {
    				channelNO = atoi(tmp_p);
    		}
    	} 
    	else
    	{
    	    return -1;
    	}
    	pclose(file);
    }
    return channelNO;
}

int ss_associatedstatus(char *ifname)
{
    char * buf[64], *tmp_p = NULL;
    FILE *file;
    int ret = -1;
    
    if (ifname == NULL)
        return -1;
    tmp_p = (char *)buf;
    
    /*if ifname is not associated*/
    memset(buf, 0, sizeof(buf));
    sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $6}'", ifname);
    file = popen(tmp_p, "r");
    if (file != NULL) {
        memset(buf, 0, sizeof(buf));
        if (fgets(tmp_p, 15, file) != NULL) {           
            if(!strcmp(tmp_p, "Not-Associated"))  
            {
                printf("%s is Not-Associated\r\n",ifname);
                ret = -1;
            }
            else
            {
                ret = 0;
            }
        }
        else
        {
            ret = -1;
        }
    } 
    else
    {
        ret = -1;
    }
    pclose(file);

    return ret;
}
int ss_radio_mode(char *ifname)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int mode = -1;
	
	if (ifname == NULL)
		return -1;
	tmp_p = (char *)buf;
	
    /*if ifname is not associated*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $6}'", ifname);
	file = popen(tmp_p, "r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 15, file) != NULL) {			
            if(!strcmp(tmp_p, "Not-Associated"))  
            {
                printf("%s is Not-Associated\r\n",ifname);                
                ssExitScan(global_ssScanInfo);
                pclose(file);
			    exit(0);
			}
		}
	} 
	else
	{
	    return -1;
	}
	pclose(file);

    /*get ap mode*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $2}' | cut -c 11", ifname);
	file = popen(tmp_p, "r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 8, file) != NULL) {
				mode = atoi(tmp_p);
		}
	} 
	else
	{
	    return -1;
	}
	pclose(file);

	return mode;	
}

int ss_radio_bw(char *ifname)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int bandwdith = 0;
	
	if (ifname == NULL)
		return -1;
	tmp_p = (char *)buf;
	
    /*if ifname is not associated*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwconfig %s | grep Frequency | awk '{print $6}'", ifname);
	file = popen(tmp_p, "r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 15, file) != NULL) {			
            if(!strcmp(tmp_p, "Not-Associated"))  
            {
                printf("%s is Not-Associated\r\n",ifname);                
                ssExitScan(global_ssScanInfo);
                pclose(file);
			    exit(0);
			}
		}
	} 
	else
	{
	    return -1;
	}
	pclose(file);

    /*get ap bandwdith*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwconfig %s | grep Rate | awk '{print $2}' | cut -c 6-8", ifname);
	file = popen(tmp_p, "r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 8, file) != NULL) {
			bandwdith = atoi(tmp_p);
		}
	}
	else
	{
		return -1;
	}
	pclose(file);

	if (bandwdith == 450 || bandwdith == 300)
		bandwdith = 40;
	else
		bandwdith = 20;

    return bandwdith;	
}

int ss_radio_extoffset(char *ifname)
{
   	/*daniel modify begin*/
	//#if 0
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int extoffset = 0;
	
	if (ifname == NULL)
		return 0;
	tmp_p = (char *)buf;
	
    /*get ap ext offset*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwpriv %s get_chextoffset | grep ext | awk '{print $2}' | cut -c 17-18", ifname);
	file = popen(tmp_p, "r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 8, file) != NULL) {
			extoffset = atoi(tmp_p);
		}
	}
	else
	{
		return 0;
	}
	pclose(file);
	return extoffset;
	//#endif
       /*daniel modify end*/
	#if 0
	unsigned char flag;
    BasicWlanConfig_t basicWlanInfo;
    
	memset(&basicWlanInfo, 0, sizeof(BasicWlanConfig_t));

	if( streq(ifname, "ath1" ))
	{
        flag = 1;
	}
	else
	{
        flag = 0;
	}
	GetBasicWlanConfig(&basicWlanInfo, flag);
	return basicWlanInfo.extoffset;
	#endif
}

/*============================================================================*/
/*  fuction name :  data process                                              */
/*============================================================================*/
/*============================================================================*/
void ssClearInfo()
{
    /*this function can clear the data when the loop is interrupted*/
    ssSpectralChanInfo * ssPtr;
    if(NULL != global_ChannelInfo)
    {
        for( ssPtr = global_ChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
        {
            ssPtr->m_upper_count = 0;
            ssPtr->m_upper_Spectral_rssi =0;
            ssPtr->m_upper_interf_count = 0;
            ssPtr->m_lower_count = 0;
            ssPtr->m_lower_Spectral_rssi =0;
            ssPtr->m_lower_interf_count = 0;
            ssPtr->m_lower_avg_rssi_max = 0;
            ssPtr->m_upper_avg_rssi_max = 0;
            ssPtr->m_lower_duty_max = 0;
            ssPtr->m_upper_duty_max = 0;        
        }
    }
    
    if( global_ssScanInfo != NULL)
    {
        global_ssScanInfo->m_ScanTimes = 0;
        global_ssScanInfo->m_ScanChannel = global_ssScanInfo->m_ChanInfo->m_ScanChannelNO;
    }    
}

void ssChannelInfoInit(ssScanSampInfo * ssScanInfo)
{
    int size;
    int i;
    ssSpectralChanInfo * ssPtr;
    if(NULL == ssScanInfo)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ssScanInfo is NULL\n");        
        ssExitScan(global_ssScanInfo);
        exit(0);
    }
    
    if(SCAN_IS_DISABLE == ssScanInfo->m_ScanEnable)
    {
        return;
    }
    
    if( global_ChannelInfo == NULL)
    {
        size = sizeof(ssSpectralChanInfo) * ssScanInfo->m_ScanChSum;
        global_ChannelInfo = (ssSpectralChanInfo * )malloc(size);
    }
    
    ssPtr = global_ChannelInfo;
    memset(global_ChannelInfo, 0, size);
    for( i = 0; i < ssScanInfo->m_ScanChSum; i++)
    {
        ssPtr->m_ScanChannelNO = wChannelNO[ssScanInfo->m_WorkMode][i];
        ssPtr->m_ChannelFreq = (u_int16_t)(ssPtr->m_ScanChannelNO * 5) + 2407;
        ssPtr->m_lower_thresh = 32;
        ssPtr->m_upper_thresh = 32;
        if(i == (ssScanInfo->m_ScanChSum - 1))
        {
            ssPtr->next = NULL;
        }
        else
        {
            ssPtr->next = ssPtr + 1;
            ssPtr++;
        }
    }
}

void ssChannelInfoUpdate(ssSpectralChanInfo *ssChannelInfo, SPECTRAL_SAMP_MSG *msg)
{
    ssSpectralChanInfo * ssPtr;
    SPECTRAL_SAMP_DATA *nl_samp_data;
    //int i;
    nl_samp_data = &msg->samp_data;
    if((NULL == ssChannelInfo) || (NULL == msg))
    {
        return;
    }
    
    for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
    {
        if(ssPtr->m_ChannelFreq == msg->freq )
        {
            break;
        }
    }

    
    if(NULL == ssPtr)
    {
        return;
    }
    /*TBD,the max count*/
    if((ssPtr->m_upper_count > 200000)||(ssPtr->m_lower_count > 200000))
    {
         return;
    }
    ssPtr->m_upper_count++;
    ssPtr->m_upper_Spectral_rssi += nl_samp_data->spectral_upper_rssi;
    if(nl_samp_data->spectral_upper_rssi > ssPtr->m_upper_thresh)
    {
        ssPtr->m_upper_interf_count++;
    }
        
    ssPtr->m_lower_count++;
    ssPtr->m_lower_Spectral_rssi += nl_samp_data->spectral_lower_rssi;
    if(nl_samp_data->spectral_lower_rssi > ssPtr->m_lower_thresh)
    {
        ssPtr->m_lower_interf_count++;
    }
}



void ssChannelInfoReset(ssSpectralChanInfo *ssChannelInfo)
{
    /*when calc the data after once scan,clear the data*/
    if(NULL == ssChannelInfo)
    {
        return;
    }
    ssChannelInfo->m_upper_count = 0;
    ssChannelInfo->m_upper_Spectral_rssi =0;
    ssChannelInfo->m_upper_interf_count = 0;
    ssChannelInfo->m_lower_count = 0;
    ssChannelInfo->m_lower_Spectral_rssi =0;
    ssChannelInfo->m_lower_interf_count = 0;
}


void ssChannelInfoAna(ssSpectralChanInfo *ssChannelInfo)
{
    ssSpectralChanInfo * ssPtr;
    if(NULL == ssChannelInfo)
    {
        return;
    }
    for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
    {
        /*TBD, the min packet count*/
        if ( ssPtr->m_upper_count > 10)
        {
            ssPtr->m_upper_avg_rssi = ssPtr->m_upper_Spectral_rssi / ssPtr->m_upper_count;
            ssPtr->m_upper_duty = ssPtr->m_upper_interf_count * 100 / ssPtr->m_upper_count;
        }
        
        if ( ssPtr->m_lower_count > 10)
        {
            ssPtr->m_lower_avg_rssi = ssPtr->m_lower_Spectral_rssi / ssPtr->m_lower_count;
            ssPtr->m_lower_duty = ssPtr->m_lower_interf_count * 100 / ssPtr->m_lower_count;
        }
        
        if(ssPtr->m_upper_avg_rssi > ssPtr->m_upper_avg_rssi_max)
        {
            ssPtr->m_upper_avg_rssi_max = ssPtr->m_upper_avg_rssi;
        }
        
        if(ssPtr->m_lower_avg_rssi > ssPtr->m_lower_avg_rssi_max)
        {
            ssPtr->m_lower_avg_rssi_max = ssPtr->m_lower_avg_rssi;
        }
        
        if(ssPtr->m_upper_duty > ssPtr->m_upper_duty_max)
        {
            ssPtr->m_upper_duty_max = ssPtr->m_upper_duty;
        }

        if(ssPtr->m_lower_duty > ssPtr->m_lower_duty_max)
        {
            ssPtr->m_lower_duty_max = ssPtr->m_lower_duty;
        }

        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL3,"channel:%d,lower count:%d-rssi:%d-duty:%d,upper count: %d-rssi:%d-duty:%d\r\n",\
               ssPtr->m_ScanChannelNO,\
               ssPtr->m_lower_count,ssPtr->m_lower_avg_rssi,ssPtr->m_lower_duty,\
               ssPtr->m_upper_count,ssPtr->m_upper_avg_rssi,ssPtr->m_upper_duty);

        ssChannelInfoReset(ssPtr);
    }

}

int ssChannelfindSpectral2_4G(ssSpectralChanInfo *ssChannelInfo)
{
    ssSpectralChanInfo * ssPtr;
    int GoodChannel = 0;
    int rssiTemp = 0;
    //int rssiWorkingChannel = 0;
    
    if(NULL == ssChannelInfo)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"SpectralScan 2_4G no good channel\n");
        return 0;
    }
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"\nSpectral scan reslut:\n");
    
    for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"channel:%d,rssi:%d-%d,duty:%d-%d\r\n",\
               ssPtr->m_ScanChannelNO,\
               ssPtr->m_lower_avg_rssi_max,ssPtr->m_upper_avg_rssi_max,\
               ssPtr->m_lower_duty_max,ssPtr->m_upper_duty_max);
        /*TBD, the channel NO*/
        if( (1 == ssPtr->m_ScanChannelNO) || \
            (6 == ssPtr->m_ScanChannelNO) || \
            (11 == ssPtr->m_ScanChannelNO) )
        {
            if((ssPtr->m_lower_avg_rssi_max + ssPtr->m_upper_avg_rssi_max) > rssiTemp)
            {
                rssiTemp = ssPtr->m_lower_avg_rssi_max + ssPtr->m_upper_avg_rssi_max;
                GoodChannel = ssPtr->m_ScanChannelNO;
            }
        }
        
        ssPtr->m_lower_avg_rssi_max = 0;
        ssPtr->m_upper_avg_rssi_max = 0;
        ssPtr->m_lower_duty_max = 0;
        ssPtr->m_upper_duty_max = 0;        
    }
    
    return GoodChannel;
}
int ssChannelfind2_4G(ssSpectralChanInfo *ssChannelInfo)
{
#define CHANLOAD_INCREASE_AVERAGE_RSSI (5)
#define USE_DUTY
#undef USE_DUTY
    ssSpectralChanInfo * ssPtr;
    int GoodChannel = 0;
    //int maxrssi = (int)(~0U>>1);     
    u_int16_t workmode;
    channel_avgrssi_pair_t ssPtrProbetest;
    channel_avgrssi_pair_t rs;
    
    workmode = get_WorkMode();

    ssPtrProbetest = getProbeResult2_4G();
    rs = ssPtrProbetest->next;
    ssPtr = ssChannelInfo;
    
    if((NULL == ssPtrProbetest) || (NULL == ssPtrProbetest->next))
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"probeScan 2_4G no good channel\n");
        return ssChannelfindSpectral2_4G(ssChannelInfo);
    } 
    
    switch( workmode ) 
    {
        case ScanHt40Plus:
        {
            for( rs = ssPtrProbetest->next; rs != NULL; rs = rs->next)
            {  
                for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
                {
                    if( ssPtr->m_ScanChannelNO == rs->channel )
                    {
                        #ifdef USE_DUTY
                        rs->avg_rssi += ssPtr->m_lower_duty_max * CHANLOAD_INCREASE_AVERAGE_RSSI;
                        #else
                        rs->avg_rssi += ssPtr->m_lower_avg_rssi_max;
                        #endif 
                    }
                    if( (ssPtr->m_ScanChannelNO + 4) == rs->channel)
                    {
                        #ifdef USE_DUTY
                        rs->avg_rssi += ssPtr->m_upper_duty_max * CHANLOAD_INCREASE_AVERAGE_RSSI;
                        #else
                        rs->avg_rssi += ssPtr->m_upper_avg_rssi_max;
                        #endif
                    }
                }                
            }
           break;
        }
        case ScanHt40Minus:
        {
            for( rs = ssPtrProbetest->next; rs != NULL; rs = rs->next)
            {  
                for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
                {
                    if( ssPtr->m_ScanChannelNO == rs->channel )
                    {
                        #ifdef USE_DUTY
                        rs->avg_rssi += ssPtr->m_upper_duty_max * CHANLOAD_INCREASE_AVERAGE_RSSI;
                        #else
                        rs->avg_rssi += ssPtr->m_upper_avg_rssi_max;
                        #endif 
                    }
                    if( (ssPtr->m_ScanChannelNO - 4) == rs->channel)
                    {
                        #ifdef USE_DUTY
                        rs->avg_rssi += ssPtr->m_lower_duty_max * CHANLOAD_INCREASE_AVERAGE_RSSI;
                        #else
                        rs->avg_rssi += ssPtr->m_lower_avg_rssi_max;
                        #endif 
                    }
                }
            }
            break;
        }
        case ScanHt20:
        {
            for( rs = ssPtrProbetest->next; rs != NULL; rs = rs->next)
            {  
                for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
                {
                    if( ssPtr->m_ScanChannelNO == rs->channel )
                    {
                        #ifdef USE_DUTY
                        rs->avg_rssi += ssPtr->m_lower_duty_max * CHANLOAD_INCREASE_AVERAGE_RSSI;
                        #else
                        rs->avg_rssi += ssPtr->m_lower_avg_rssi_max;
                        #endif 
                    }
                }
            }
            break;
        }
        default:
        {
            return GoodChannel;
            break;
        }
    }


    
    rs = ssPtrProbetest->next;
    ssPtrProbetest->avg_rssi = rs->avg_rssi;    
    ssPtrProbetest->channel = rs->channel;
    while (rs != NULL) 
    {  
		if(rs->avg_rssi < ssPtrProbetest->avg_rssi)
		{
            ssPtrProbetest->avg_rssi = rs->avg_rssi;    
            ssPtrProbetest->channel = rs->channel;
		}
        rs = rs->next;
    }
    
    GoodChannel = ssPtrProbetest->channel;
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"\nSpectral scan reslut:\n");
    
    for( ssPtr = ssChannelInfo; ssPtr != NULL; ssPtr = ssPtr->next)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"channel:%d,rssi:%d-%d,duty:%d-%d\r\n",\
               ssPtr->m_ScanChannelNO,\
               ssPtr->m_lower_avg_rssi_max,ssPtr->m_upper_avg_rssi_max,\
               ssPtr->m_lower_duty_max,ssPtr->m_upper_duty_max);
               
        ssPtr->m_lower_avg_rssi_max = 0;
        ssPtr->m_upper_avg_rssi_max = 0;
        ssPtr->m_lower_duty_max = 0;
        ssPtr->m_upper_duty_max = 0;        
    }
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,\
                    "Goodchannel:=%d, avg rssi=%d\n", ssPtrProbetest->channel, \
                    ssPtrProbetest->avg_rssi);
                    
    return GoodChannel;
    
#undef USE_DUTY
#undef CHANLOAD_INCREASE_AVERAGE_RSSI
}

int ssChannelfind5G( void )
{    
    int GoodChannel = 0;
    int extoffsetmode;
    channel_avgrssi_pair_t ssPtrProbetest;
    channel_avgrssi_pair_t rs;
    
    ssPtrProbetest = getProbeResult5G();
    extoffsetmode = ss_radio_extoffset(global_vap);
    
    if((NULL == ssPtrProbetest) || (NULL == ssPtrProbetest->next))
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"probe Scan5G no good channel\n");
        return 0;
    }
    
    ssPtrProbetest->avg_rssi = (int)(~0U>>1);    
    ssPtrProbetest->channel = 0;
    for( rs = ssPtrProbetest->next; rs != NULL; rs = rs->next)
    {  
        switch(extoffsetmode)
        {
            case 1:/*PLUS*/
            {
                if(((((rs->channel - 28) % 8) == 0) && (rs->channel != 140 ))|| (rs->channel == 149 ))
                {
                   break;
                }
                else
                {
                    continue;
                }
                break;
            }
            case -1:/*MINUS*/
            {
                if(((((rs->channel - 32) % 8) == 0) && (rs->channel != 140 )) || (rs->channel == 153 ))
                {
                   break;
                }
                else
                {
                    continue;
                }
                break;
            }
            case 0:
            default:
                break;
        }
        if(rs->avg_rssi < ssPtrProbetest->avg_rssi)
        {
            ssPtrProbetest->avg_rssi = rs->avg_rssi;    
            ssPtrProbetest->channel = rs->channel;
        }
    }
    
    GoodChannel = ssPtrProbetest->channel;
    return GoodChannel;
}


void ssChannelfind(ssSpectralChanInfo *ssChannelInfo)
{
   // ssSpectralChanInfo * ssPtr;
    int GoodChannel = 0;
   // int8_t maxrssi = 100; 
    static int count;
    //channel_avgrssi_pair_t ssPtrProbetest;
    //int GoodChannelscan = 0;
    if(0 == count)
    {
        count++;
        return;
    }
    
    if( spectral_debug_level >= ATH_DEBUG_SPECTRAL1 )
    {
        printf("Probe scan reslut:\n");
        printProbeScan();
    }
    
    if( get_WorkMode() == Scan5G)
    {
        GoodChannel = ssChannelfind5G();        
    }
    else
    {
        GoodChannel = ssChannelfind2_4G(ssChannelInfo);        
    } 
    
    if( spectral_debug_level >= ATH_DEBUG_SPECTRAL1 )
    {
        printf("\n%s the final reslut:\n", global_vap);
        printProbeScan();
    }

    if( 0 == GoodChannel)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Not find goodchannel.\n");
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,\
                        "--------------------Loop end----------------------\n");
        return;
    }

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s find Good Channel %d\n",\
                     global_vap, GoodChannel);
    
    ActionSwitchChannel(GoodChannel);
    
    #if 0
    if( switch_channel_status == SWITCH_CHANNEL_ENABLED)
    {
        work_channel = ActionSwitchChannel(GoodChannel);
        /*to update the work channel, to avoid the channel set error*/
        /*if use beacon,change channel occured after several intervals*/
        #if 0
        work_channel = ssGetChannelNO(global_vap);
        if(work_channel == -1)
        {
           printf("work channel get error after action\n");           
           ssExitScan(global_ssScanInfo);
           exit(0);
        } 
        #endif
        global_ssScanInfo->m_WorkChannel = work_channel;            
    }
    #endif
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,\
                    "--------------------Loop end----------------------\n");
}



/*---------------------------------------------------------------------------*/
void ssSetMode(void)
{
    int rc;
    int mode;
    int bandwidth;
    int chextoffset;
    int channel;

    global_ssScanInfo = (ssScanSampInfo *)malloc(sizeof(ssScanSampInfo));
    if(NULL == global_ssScanInfo)
    {
        printf("global_ssScanInfo is NULL\n");        
        ssExitScan(global_ssScanInfo);
        exit(0);
    }
    
    mode = ss_radio_mode(global_vap);
    if(-1 == rc)
    {
        printf("ss_radio_mode error\n");        
        ssExitScan(global_ssScanInfo);
        exit(0);
    } 
    bandwidth = ss_radio_bw(global_vap);
    chextoffset = ss_radio_extoffset(global_vap);
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"ap info, mode:%d,ext:%d,bw:%d\n",\
                     mode,chextoffset,bandwidth);

    if(2 == mode)
    {
        global_ssScanInfo->m_ScanEnable = SCAN_IS_ENABLE;
        /*if HT20/HT40 is coexsit, need to judge bw at last*/
        if(1 == chextoffset)
        {
            global_ssScanInfo->m_WorkMode = ScanHt40Plus;
            global_ssScanInfo->m_ScanChSum = SCAN_CHANNEL_SUM_HT40;
        }
        else if(-1 == chextoffset)
        {
            global_ssScanInfo->m_WorkMode = ScanHt40Minus;
            global_ssScanInfo->m_ScanChSum = SCAN_CHANNEL_SUM_HT40;
        }
        else if(20 == bandwidth)
        {
            global_ssScanInfo->m_WorkMode = ScanHt20;
            global_ssScanInfo->m_ScanChSum = SCAN_CHANNEL_SUM_HT20;
        }
        else/*if use the device default setting,what can i do*/
        {
            global_ssScanInfo->m_WorkMode = ScanHt20;
            global_ssScanInfo->m_ScanChSum = SCAN_CHANNEL_SUM_HT20;
        }
    }
    else
    {
        global_ssScanInfo->m_ScanEnable = SCAN_IS_DISABLE;
        global_ssScanInfo->m_WorkMode = Scan5G;
        global_ssScanInfo->m_ScanChSum = 0;
    }

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"ScanEnable %d, workmode %d.\n",\
                     global_ssScanInfo->m_ScanEnable,global_ssScanInfo->m_WorkMode);
                     
    channel = ssGetChannelNO(global_vap);
    if(channel == -1)
    {
       printf("work channel get error in main\n");       
       #if 0
       ssExitScan(global_ssScanInfo);
       exit(0);
       #endif
    } 
    else
    {
        work_channel = channel;
    }
    
    ssInitScanInfo(global_ssScanInfo);
    ssChannelInfoInit(global_ssScanInfo);
}

u_int16_t get_WorkMode()
{
    if(global_ssScanInfo == NULL)
    {
        return 0;
    }
    return global_ssScanInfo->m_WorkMode;
}

int get_WorkChannel()
{
    if(global_ssScanInfo == NULL)
    {
        return 0;
    }
    return global_ssScanInfo->m_WorkChannel;
}

int ActionSwitchChannel(int channel)
{
    char cmd[64] = {'\0'};
    u_int16_t workmode;
    int workchannel;
    int workingchannel;
    channel_avgrssi_pair_t ssPtrProbetest;            
    channel_avgrssi_pair_t ssPtrtemp;            

    workmode = get_WorkMode();
    workingchannel = get_WorkChannel();

    if(Scan5G == workmode)
    {
        ssPtrProbetest = getProbeResult5G();
    }
    else
    {
        ssPtrProbetest = getProbeResult2_4G();
    }
    
    if(( NULL != ssPtrProbetest) && (NULL != ssPtrProbetest->next))
    {
        ssPtrtemp = ssPtrProbetest->next;

        while(ssPtrtemp != NULL)
        {
            if(ssPtrtemp->channel == workingchannel)
            {
                break;
            }
            ssPtrtemp = ssPtrtemp->next;
        }
        
        if(ssPtrtemp != NULL)
        {
            if( (ssPtrtemp->avg_rssi - ssPtrProbetest->avg_rssi ) < (int)10 )/*TBD*/
            {
                workchannel = workingchannel;
            }
            else
            {
                workchannel = channel;
            }
        }
        else
        {
            workchannel = channel;
        }
    }
    else
    {
        workchannel = channel;
    }
    
    switch( workmode ) 
    {
        case ScanHt40Plus:
        {
           if(11 == workchannel)
           {
               workchannel = 6;
           }
           else if(6 == workchannel)
           {
               SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"not switch work channel.\n");
               return workchannel;
           }
           break;
        }
        case ScanHt40Minus:
        {
            if(1 == workchannel)
            {
                workchannel = 6;
            }
            else if(6 == workchannel)
            {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"not switch work channel.\n");
                return workchannel;
            }
            break;
        }
        case Scan5G:
        case ScanHt20:
        default:        
            break;
    }

    if(workchannel == workingchannel)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"channel not need to change %d, good is %d.\n",workchannel, channel);
        return workchannel;
    }
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s the final good channel is %d.\n",global_vap, workchannel);
                     
    if( switch_channel_status == SWITCH_CHANNEL_ENABLED)
    {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s switch work channel %d to %d.\n",\
                         global_vap, workingchannel,workchannel);
                         
        sprintf(cmd,"iwpriv %s vap_doth 0", global_vap);
        system(cmd);
        sprintf(cmd,"iwpriv %s doth_chanswitch %2d 5", global_vap, workchannel);
        system(cmd);
        
        work_channel = workchannel;
        global_ssScanInfo->m_WorkChannel = work_channel;            
    }
    return workchannel;
}


/*---------------------------------------------------------------------------*/
static timer_t spectral_timer_id;
void ssSwitchChannelstatus(ssScanSampInfo * ssScanInfo);

void spectral_handle(union sigval v)
{
    ssSwitchChannelstatus(global_ssScanInfo);
}

int spectral_create_timer (int id, timer_t *tid)
{
    struct sigevent se;
    //struct itimerspec ts, ots;
    memset (&se, 0, sizeof (se));
    se.sigev_notify = SIGEV_THREAD;
    //se.sigev_notify = SIGEV_SIGNAL_THREAD;
    se.sigev_value.sival_int = id;
    se.sigev_notify_function = spectral_handle;
    if (timer_create (CLOCK_REALTIME, &se, tid) < 0)
    {
       perror ("timer_creat");
       return -1;
    }
    return 0;
}

int spectral_set_timer(int seconds, long nsec)
{
    struct itimerspec ts, ots;
    
    ts.it_value.tv_sec = seconds;
    ts.it_value.tv_nsec = nsec;
    if(seconds < 60)
    {
        ts.it_interval.tv_sec = 60;
    }
    else if( seconds < 100000 )
    {
        ts.it_interval.tv_sec = seconds * 3;
    }
    else
    {
        ts.it_interval.tv_sec = seconds;
    }
    
    ts.it_interval.tv_nsec = 0;

    if (timer_settime (spectral_timer_id, 0, &ts, &ots) < 0)
     {
       perror ("timer_settime");
       return -1;
     }
    return 0;
}

void spectral_init_timer()
{
    int rc;
    rc = spectral_create_timer(2, &spectral_timer_id);
    if(-1 == rc)
    {
        printf ("timer_creat error\n");
        exit(0);
    }
    else
    {
        printf ("timer_creat %d\n",spectral_timer_id);
    }

    spectral_set_timer(2, 0);
}

/*----------------------------------------------------------------------------*/
void ssSwitchChannelstatus(ssScanSampInfo * ssScanInfo)
{
    int rc;
    char cmd[256]={'\0'};
    static unsigned int loopcount = 0;
    int ssSeconds = 0;
    long ssUsecs = 0;

    /*daniel add begin*/
    /* Init sample info pointer */                         
    ssd_samp_info_t *psinfo = GET_SAMP_INFO_PTR(pinfo);
    /*daniel add end*/
	
    
    if(!global_ssam)
        return;
    
    /*daniel modify begin*/
    //extern void clear_max_rssi_data(void);
    //clear_max_rssi_data();
    ssd_clear_max_rssi_data(pinfo);
   /*daniel modify end*/

	
    switch(ssScanInfo->m_ScanState)
    {
        case ScanIsRunning:
        {
            ssStopScan();
            if(global_time_work > 0)
            {
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap , ssScanInfo->m_WorkChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"scan:%d->work:%d\n",\
                                 ssScanInfo->m_ScanChannel,ssScanInfo->m_WorkChannel);
                ssScanInfo->m_ScanState = ScanIsAna; 
                ssSeconds = 1;
                ssUsecs = 0;
                break;
            }
        }
        case ScanIsAna:
        {
            ssScanInfo->m_ScanState = ScanIsStop; 
            ssChangeChannel(ssScanInfo);/*no modify,can update m_ScanState to waiting*/
            ssChannelInfoAna(global_ChannelInfo);



           /*daniel delete begin*/
          #if 0

			
            ss_creat_json();
            web_clear_interf_rsp(0);

           #endif
           /*daniel delete end*/

			
            if(0 == global_time_work)
            {
                ssSeconds = 0;
                ssUsecs = SS_SHORT_TIME;
            }
            else
            {
                ssSeconds = global_time_work;
                ssUsecs = 0;
            }
            break;
        }        
        case ScanIsStop:
        {
            if(1 == ssSpectralscanstatus())
            {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"frame %5d, %5d, %5d\n",total_frame,global_frame_count,global_analysis_count);
                global_frame_count = 0;
                global_analysis_count = 0;


		/*daniel delete begin*/
               #if 0		
                web_clear_interf_rsp(ssScanInfo->m_ScanChannel);
               #endif
              /*daniel delete end*/
                

				
                ssScanInfo->m_ScanTimes++;
                ssScanInfo->m_ScanState = ScanIsRunning;
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap, ssScanInfo->m_ScanChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"work:%d->scan:%d\n",ssScanInfo->m_WorkChannel,ssScanInfo->m_ScanChannel);
            }
            else
            {
                ssScanInfo->m_ScanState = ScanIsWaiting;
            }

            
            #if 0/*TBD, when ath is down*/
            if(-1 == ss_associatedstatus(global_vap))
            {
                printf("scan but %s is not assoitated\n",global_vap);                
                ssExitScan(global_ssScanInfo);
                exit(0);
            }           
            #endif
                
            if(1 == ssSpectralscanstatus())
            {
                ssStartScan();
            }
            else
            {
                ssStopScan();
                ssClearInfo();
            }
            
            if( global_time_scan < 1000000)
            {
                ssSeconds = 0;
                ssUsecs = global_time_scan;
            }
            else
            {
                ssSeconds = global_time_scan/1000000;
                ssUsecs = 0;
            }
            break;            
        }        
        case ScanIsWaiting:
        {

	    /*daniel delete begin*/
            #if 0	
            web_clear_interf_rsp(0);
            #endif
           /*daniel delete end*/

			
            if( Scan5G != global_ssScanInfo->m_WorkMode)
            {
                /*daniel delete begin*/
                #if 0
                ss_creat_json(); 
		 #endif
                /*daniel delete end*/		
            }
            ssChannelfind(global_ChannelInfo);
            releaseProbeScan();
            if( global_time_waiting > 0)
            {
                ssScanInfo->m_ScanState = scanIsCSAction;
                ssSeconds = 1;
                ssUsecs = 0;
                break;
            }
        }
        case scanIsCSAction:
        {
            if( switch_channel_status == SWITCH_CHANNEL_ENABLED)
            {
                sprintf(cmd,"iwpriv %s vap_doth 1", global_vap);
                system(cmd);
            }
            if((loopcount >= global_looptimes) && (global_looptimes != 0))
            {            
                if(ssScanInfo->m_ScanChannel != ssScanInfo->m_WorkChannel )
                {
                    sprintf(cmd,"sssetchannel %s channel %1d",global_vap , ssScanInfo->m_WorkChannel);
                    system(cmd);
                }
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Whole loop excute %d times and exit.\n",loopcount);
                ssExitScan(global_ssScanInfo);
                exit (0);
            }
            loopcount++;
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,\
                            "\n----------------Loop start,count %d-------------------\n",loopcount);
            ssScanInfo->m_ScanState = ScanIsProbe;
            if(0 == global_time_waiting)
            {
                ssSeconds = 0;
                ssUsecs = SS_SHORT_TIME;
            }
            else
            {
                ssSeconds = global_time_waiting;
                ssUsecs = 0;
            }
            break;
        }
        case ScanIsProbe:
        {               
            rc = ssGetChannelNO(global_vap);
            if(rc == -1)
            {
               printf("work channel get error after action\n");           
            }
            else if(( global_time_work != 0) && (global_time_waiting !=0 ))
            {
                work_channel = rc;
                global_ssScanInfo->m_WorkChannel = work_channel;   
            }
            
            initProbeScan();
            /*excute probe scan*/
            if(1 == ssProbescanstatus())
            {
                probeargv[1] = global_vap;
                probe_scan_main(probeargc,probeargv);
                ssScanInfo->m_ScanState = ScanIsStop;
                ssSeconds = (channel_scan_timeout )/1000000 + 1;
                ssUsecs = 0;
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"probescan excute complete.\n");
            }
            else if(1 == ssSpectralscanstatus())
            {            
                ssScanInfo->m_ScanState = ScanIsStop;
                ssSeconds = 0;
                ssUsecs = SS_SHORT_TIME;
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"probescan dis,excute spectralscan.\n");
            }
            else
            {
                ssScanInfo->m_ScanState = ScanIsWaiting;
                ssSeconds = (channel_scan_timeout )/1000000 + 1;
                ssUsecs = 0;
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"scan disable\n");
				

		/*daniel delete begin*/
               #if 0
		SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"tx/rx rate %lu\n",ss_Get_speed());
               #endif
              /*daniel delete end*/

			}
            break;
        }
        default:
        {
            sprintf(cmd,"sssetchannel %s channel %1d",global_vap, ssScanInfo->m_WorkChannel);
            system(cmd);
            ssScanInfo->m_ScanState = ScanIsWaiting;
            ssStopScan();
            ssSeconds = 1;
            ssUsecs = 0;
            break;
        }

    } 
    spectral_set_timer(ssSeconds, ssUsecs * 1000);

    /*daniel modify begin*/
    //total_channel_switches++;
    psinfo->total_channel_switches++;
    /*daniel modify end*/

} 
/*daniel add end*/


