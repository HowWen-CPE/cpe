#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <signal.h>
#include <linux/if.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/mman.h>  
#if 0


extern char global_radio[];
extern char global_vap[];
extern int spectral_debug_level;
u_int32_t gloabl_ssRate= 0;

int global_ctlbusy =0;
int global_extbusy =0;

#define SS_INTERVAL 1
#define SS_INTERVAL_COUNT 5
#define	streq(a,b)	(strcasecmp(a,b) == 0)

enum {
        SS_DEBUG_SPECTRAL       = 0x00000100,   /* Minimal SPECTRAL debug */
        SS_DEBUG_SPECTRAL1      = 0x00000200,   /* Normal SPECTRAL debug */
        SS_DEBUG_SPECTRAL2      = 0x00000400,   /* Maximal SPECTRAL debug */
        SS_DEBUG_SPECTRAL3      = 0x00000800,   /* matched filterID display */
};

int spectral_ss_dbglvl = SS_DEBUG_SPECTRAL;

#define SS_DPRINTF(_m, _fmt, ...) do {            \
    if ((_m) <= spectral_ss_dbglvl) {                 \
        printf(_fmt, ##__VA_ARGS__);                    \
    }        \
} while (0)

int ss_Get_speed()
{
    return gloabl_ssRate;
}

int Get_ctlbusy()
{
    return global_ctlbusy;
}

int Get_extbusy()
{
    return global_extbusy;
}


int ss_Get_ctlbusy(char *ifname, int *ssCtlbusy)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int bandwdith = 0;
	char *filename = NULL;
	
	if (ifname == NULL)
		return -1;
	tmp_p = (char *)buf;

	if( streq( ifname, "ath0"))
	{
	    filename = "/tmp/ath0spectral.txt";
	}
	else
	{
	    filename = "/tmp/ath1spectral.txt";
	}
    /*if ifname is not associated*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwpriv %s get_ctlbusy | awk '{print $2}' | cut -c 13- > %s 2>/dev/null", ifname,filename);
    system(tmp_p);

    file = fopen(filename,"r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 5, file) != NULL) {
            *ssCtlbusy = atoi(tmp_p);
		}
		else
		{
		    *ssCtlbusy = 0;
            fclose(file);
            return -1;
		}
	} 
	else
	{
        *ssCtlbusy = 0;
	    return -1;
	}
	fclose(file);

	if((*ssCtlbusy >100 ) || (*ssCtlbusy <0 ))
	{
        *ssCtlbusy = 0;
	}
	sprintf(tmp_p, "rm %s",filename);
    system(tmp_p);
    return 0;
}

int ss_Get_extbusy(char *ifname, int *ssExtbusy)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int bandwdith = 0;
	char *filename = NULL;
	
	if (ifname == NULL)
		return -1;
	tmp_p = (char *)buf;

	if( streq( ifname, "ath0"))
	{
	    filename = "/tmp/ath0spectral.txt";
	}
	else
	{
	    filename = "/tmp/ath1spectral.txt";
	}
	
    /*if ifname is not associated*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "iwpriv %s get_extbusy | awk '{print $2}' | cut -c 13- > %s 2>/dev/null", ifname,filename);
    system(tmp_p);

    file = fopen(filename,"r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 5, file) != NULL) {
            *ssExtbusy = atoi(tmp_p);
		}
		else
		{
		     *ssExtbusy = 0;
             fclose(file);
            return -1;
		}
	} 
	else
	{
	    *ssExtbusy = 0;
	    return -1;
	}
	fclose(file);
	if((*ssExtbusy >100 ) || (*ssExtbusy <0 ))
	{
        *ssExtbusy = 0;
	}
	sprintf(tmp_p, "rm %s",filename);
    system(tmp_p);
    return 0;
}

int ss_Get_if_stats(char *ifname, u_int32_t *ssRxbytes, u_int32_t *ssTxbytes)
{
	char * buf[64], *tmp_p = NULL;
	FILE *file;
	int bandwdith = 0;
	char *filename = NULL;
	
	if (ifname == NULL)
		return -1;
	tmp_p = (char *)buf;

	if( streq( ifname, "ath0"))
	{
	    filename = "/tmp/ath0spectral.txt";
	}
	else
	{
	    filename = "/tmp/ath1spectral.txt";
	}
	
    /*if ifname is not associated*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "ifconfig %s | grep RX | grep bytes | awk '{print $2}' | cut -c 7- > %s 2>/dev/null", ifname,filename);
    system(tmp_p);

    file = fopen(filename,"r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 22, file) != NULL) {
            *ssRxbytes = strtoul(tmp_p,0,10);
		}
		else
		{
            fclose(file);
            return -1;
		}
	} 
	else
	{
	    return -1;
	}
	fclose(file);

    /*get ap bandwdith*/
	memset(buf, 0, sizeof(buf));
	sprintf(tmp_p, "ifconfig %s | grep RX | grep bytes | awk '{print $6}' | cut -c 7- > %s 2>/dev/null", ifname,filename);
    system(tmp_p);

    file = fopen(filename,"r");
	if (file != NULL) {
		memset(buf, 0, sizeof(buf));
		if (fgets(tmp_p, 22, file) != NULL) {
            *ssTxbytes = strtoul(tmp_p,0,10);
		}
		else
		{
            fclose(file);
            return -1;
		}
	}
	else
	{
		return -1;
	}
	fclose(file);
	
	sprintf(tmp_p, "rm %s",filename);
    system(tmp_p);
	return 0;
}

#endif

void *ssSpectralSpeedCalc()
{
   #if 0
    u_int32_t ssRxbytesStart;
    u_int32_t ssTxbytesStart;
    u_int32_t ssRxbytesend;
    u_int32_t ssTxbytesend;
    u_int32_t temp1;    
    u_int32_t ssRate;
    u_int32_t ssTimeCount = 0;

    int ctlbusytemp = 0;
    int extbusytemp = 0;
    int busytemp;
    gloabl_ssRate = 0;
    global_ctlbusy = 0;
    global_extbusy = 0;
    
    spectral_ss_dbglvl = spectral_debug_level;
    
    while(1)
    {
        if(0 == ss_Get_if_stats(global_vap, &ssRxbytesStart,&ssTxbytesStart))
        {
            SS_DPRINTF(SS_DEBUG_SPECTRAL3,"statics start, %lu-%lu\n",(long unsigned int)ssRxbytesStart,(long unsigned int)ssTxbytesStart);
        }
        else
        {
            SS_DPRINTF(SS_DEBUG_SPECTRAL1,"statics err\n");
            sleep(SS_INTERVAL);
            continue;
        }

        for(;;)
        {
            ss_Get_ctlbusy(global_vap, &busytemp );
            ctlbusytemp += busytemp;
            #if 0
            ss_Get_extbusy(global_vap, &busytemp );
            extbusytemp += busytemp;
            #endif
            sleep(SS_INTERVAL);
            ssTimeCount++;
            if( ssTimeCount >= SS_INTERVAL_COUNT  )
            {
                break;
            }
        }
        
        if(0 == ss_Get_if_stats(global_vap,&ssRxbytesend,&ssTxbytesend))
        {
            SS_DPRINTF(SS_DEBUG_SPECTRAL3,\
                            "statics end, %lu-%lu\n",(long unsigned int)ssRxbytesend,(long unsigned int)ssTxbytesend);
            ssRate = 0;
            temp1 = ssRxbytesend - ssRxbytesStart;
            ssRate += temp1;
            temp1 = ssTxbytesend - ssTxbytesStart;
            ssRate += temp1;
            gloabl_ssRate = ssRate * 8/ssTimeCount/1000;
            #if 0
            SS_DPRINTF(SS_DEBUG_SPECTRAL3,\
                            "statics rate: %lu\n",gloabl_ssRate);
            #endif
                            
        }
        else
        {
            SS_DPRINTF(SS_DEBUG_SPECTRAL1,"statics err\n");
        }

        global_ctlbusy = ctlbusytemp / ssTimeCount;
        ctlbusytemp = 0;
        
        #if 0
        global_extbusy = extbusytemp / ssTimeCount;
        extbusytemp = 0;
        #endif
        
        ssTimeCount = 0;

       #if 0
       SS_DPRINTF(SS_DEBUG_SPECTRAL1,\
                 "%s Statics: busy,ctl:%d-ext:%d; rate: %luKbps.\n",\
                 global_vap, global_ctlbusy, global_extbusy, gloabl_ssRate);
       #else
       SS_DPRINTF(SS_DEBUG_SPECTRAL1,\
         "%s Statics: busy,ctl:%d; rate: %luKbps.\n",\
         global_vap, global_ctlbusy, gloabl_ssRate);
       #endif
        
    }
    #endif
}
int ss_spectral_thread_init()
{
    pthread_t ntid;
    pthread_attr_t attr;
    int err_tid, ret, error;
    
    if((error = pthread_attr_init(&attr)) != 0)
    {
        printf("init thread failed!\n");
        return -1;
    }
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //Set detach status
    if(ret != 0)
    {
        printf("set detach state failed!\n");
        return -1;
    }
    err_tid = pthread_create(&ntid, &attr, ssSpectralSpeedCalc, NULL);
    if(err_tid != 0)
    {
        printf("Create the thread failed!\n");
        return -1;
    }

    pthread_attr_destroy(&attr);
    return 0;
}


