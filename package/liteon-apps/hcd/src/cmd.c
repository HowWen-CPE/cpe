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
#include <sys/ioctl.h>
#include <sys/wait.h> 
#include <net/if.h>
#include <linux/if_packet.h>
/*================================================================*/
/* Local Include Files */

#include <sys/stat.h>

#include "hcd.h"
#include "cmd.h"

#define MAX_INBAND_PAYLOAD_LEN 1480


#define LOCAL_NETIF  ("br0")
#define CMD_INBAND_IOCTL 0x03
#define CMD_INBAND_SYSTEMCALL 0x04

extern int hcd_inband_chan;

//command ID
#define id_test_write						0x01
#define id_test_read                        0x02

static int test_write(char *cmd , int cmd_len);
static int test_read(char *cmd , int cmd_len);

struct cmd_entry cmd_table[]={
	CMD_DEF(test_write, test_write),
	CMD_DEF(test_read, test_read),
	/* last one type should be LAST_ENTRY - */   
	{0}
};

/******************************************/
/* Interface definition */
/******************************************/
static int test_write(char *cmd , int cmd_len)
{
	char *param,*val,*next;	
	int ret = RET_OK;
	char *intf, *tmp;

	cmd[cmd_len]='\0';//mark_patch
	printf("%s:%d cmd = %s, cmd_len: %d\n", __FUNCTION__, __LINE__, cmd, cmd_len);

	return ret;
}

static int test_read(char *cmd , int cmd_len)
{
	char *param;	
	int ret = RET_OK;
	char *intf, *tmp;
	cmd[cmd_len]='\0';//mark_patch
	intf = cmd;

	printf("%s:%d cmd = %s, cmd_len: %d\n", __FUNCTION__, __LINE__, cmd, cmd_len);

	memcpy(intf, "hello world\n", strlen("hello world\n"));

	return strlen("hello world\n");
}

int do_cmd(int id , char *cmd ,int cmd_len ,int relply)
{
	int i=0,ret=-1,len=0;
	
	printf("[%d],%d,cmd:%d,%s [%s]:[%d].\n",i,id,cmd_len,cmd,__FUNCTION__,__LINE__);
	while (cmd_table[i].id != LAST_ENTRY_ID) {
		if ((cmd_table[i].id == id))	{
			ret = cmd_table[i].func(cmd,cmd_len);
			printf("[%d],%d,%s ret:%d,[%s]:[%d].\n",i,id,cmd,ret,__FUNCTION__,__LINE__);
			break;
		}	
		i++;
	}
	//no reply
	
	if(!relply || cmd_table[i].id == id_ioctl )
		return ret;

	if(ret > MAX_INBAND_PAYLOAD_LEN) //mark_issue , it will be reply in it's func in command table
		return ret;

	//reply rsp pkt
	if (ret >= 0) { 
		if (ret == 0) { 
			cmd[0] = '\0';
			ret = 1;
		}

		inband_write(hcd_inband_chan,0,id,cmd,ret,1); //good reply
		printf("%d,%s[%s]:[%d].\n",id,cmd,__FUNCTION__,__LINE__);
	}
	else{ //error rsp	
		cmd[0] = (unsigned char)( ~ret + 1);			
		inband_write(hcd_inband_chan,0,id,cmd,1,2); //error reply
		printf(" %d,%s[%s]:[%d].\n",id,cmd,__FUNCTION__,__LINE__);
	}			
	
	return ret;
}
