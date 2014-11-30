#include <stdio.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/uio.h> 
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> //mark_file

#define MAX_BUF_LEN     (2048)
char *result = NULL;

#include <netinet/in.h>  //mark_8021x


#define HOST_NETIF  ("br0")
#define SLAVE_MAC ("ffffffffffff")
#define ETH_P_RTK		0x8899	// Realtek Remote Control Protocol (RRCP)
int hcd_inband_chan=0;


// Command Table
struct inband_cmd_table_t {
	char cmd_id;
	const char *cmd;			// Input command string	
	int (*func)(int index,int argc, char *argv[]);
	void (*print_func)(char *para,char *data,int len);
	const char *msg;			// Help message
    void (*sprint_func)(char *para, char *data, int len);   //print result to buffer
};

typedef struct{
	unsigned int status;	//0:SUCCESS, 1:FAIL, 2:buf overflow
	unsigned int length;	//the buf length of return value 
	unsigned char error_info[64];
}RTK_INBAND_RESULT_HEADER,*pRTK_INBAND_RESULT_HEADER;

typedef struct{
	RTK_INBAND_RESULT_HEADER header;
	unsigned char *buf;
}RTK_INBAND_RESULT, *pRTK_INBAND_RESULT;


unsigned int host_inband_seq=0; //use to do requset <-> response check

struct inband_cmd_table_t inband_cmd_table[]=
{
    {NULL,  NULL, NULL},
};

unsigned int get_random(int max)
{
        struct timeval tod;

        gettimeofday(&tod , NULL);
        srand(tod.tv_usec);
        return rand()%max;
}

void print_command_list(void)
{
    int i;

    printf("usage: inband sucmd key=val\n");
    i = 0;
    while (inband_cmd_table[i].cmd != NULL) {         
        printf("\t%s\n", inband_cmd_table[i].msg);
        i++;
	}    
}


/************InbandHost shared library******************
* argc: command string length
* argv: command strings, e.g: {"inband", "getstats", "wlan0"}
* buffer: command operation result, should be allocated 2048 bytes!
* return value: command operation status: 0-succeed, (-1)-failed.
********************************************************/
int main(int argc, char *argv[])
{
    int i, ret;
    int chan;

    if (argc < 2) {
        return 0;
    }

    //open inband  	
	chan = inband_open(HOST_NETIF,SLAVE_MAC,ETH_P_RTK,0);
	if (chan < 0) {
		printf("open inband failed!\n");
		return -1;
	}
	hcd_inband_chan = chan;	   

    i = 0;
    while (inband_cmd_table[i].cmd != NULL) {
        //printf("i=%d...\n", i);
        if (0 == strcmp(argv[1], inband_cmd_table[i].cmd)) 
        {
			if (inband_cmd_table[i].func) {
			    ret = inband_cmd_table[i].func(i,argc, argv);
                printf("return from func with %d\n", ret);
		        if (ret > 0)
		            printf("OK\n");
		        else if (ret < 0)
		            printf("FAIL\n");
		    }
			break;
        }
        i++;
	}
	if(inband_cmd_table[i].cmd == NULL)//wrong cmd
	{
		printf("Input wrong command!\n");
	}
	
	inband_close(hcd_inband_chan);
	
    return 0;
}

