#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <linux/if_ether.h>
#include <linux/if_packet.h>
#ifndef __IOH_H
#include "ioh.h"
#endif
#include <linux/if_arp.h>

#define INBAND_INTF		"br-lan0"
#define INBAND_SLAVE	("001234567899")
#define INBAND_IOCTL_TYPE	0x8899
#define INBAND_NETLINK_TYPE 0x9000
#define INBAND_DEBUG 0
#define RX_EXPIRE_PERIOD 3	//in secs

#define IWREQ_LEN 32
#define INBAND_IOCTLTYPE_LEN	4
#define INBAND_IOCTLHDR_LEN	6
#define INBAND_PENDING_START(data) data+INBAND_IOCTLHDR_LEN+IWREQ_LEN
#define INBAND_IOCTLRET_PTR(data) data+INBAND_IOCTLTYPE_LEN
#define IOH_HDR_LEN sizeof(struct ioh_header)
#ifdef INBAND_HOST
#define MAXDATALEN      1560    // jimmylin: org:256, enlarge for pass EAP packet by event queue
#define DOT11_EVENT_REQUEST 2
#define SIOCGIWIND 0x89ff
#endif
#define ioctl_cmd_type 0x05
#define sycall_cmd_type 0x04

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) printf("[Inband]%s %d:"fmt,__FUNCTION__ , __LINE__ , ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#ifdef IEEE80211_IOCTL_SETWMMPARAMS
/* Assume this is built against realtek-ng */
#define REALTEK_NG
#endif /* IEEE80211_IOCTL_SETWMMPARAMS */
//#define RTL8192CD_IOCTL_DEL_STA	0x89f7
//#define SIOCGIWIND      		0x89ff
//#defein SIOCIWLASTPRIV		0x8BFF

typedef struct _DOT11_REQUEST{
        unsigned char   EventId;
}DOT11_REQUEST;

unsigned int get_random(int max)
{
	struct timeval tod;

	gettimeofday(&tod , NULL);
	srand(tod.tv_usec);
	return rand()%max;
}

static int rx_expired(struct timeval *start)
{
	struct timeval now;

	gettimeofday(&now , NULL);

	return (now.tv_sec-start->tv_sec<RX_EXPIRE_PERIOD)?0:1;
}

int
inband_remote_cmd(unsigned char *cmd)
{
	unsigned char *buf;
	unsigned int channel = -1;

	channel = inband_open(INBAND_INTF,INBAND_SLAVE,INBAND_IOCTL_TYPE,INBAND_DEBUG);
	if( channel < 0 || inband_write(channel, get_random(65535), sycall_cmd_type, cmd, strlen(cmd), 0) < 0) {
		printf("inband sent remote command failed\n");
	}/* else {
		if( inband_rcv_data(channel, &syscall_cmd_type, &buf, -1) < 0 )
			printf("inband try to receive respone but failed\n");
		inband_close(channel);
	}*/
	inband_close(channel);
}
