/***********************************************************
  Copyright (C), 2010, Liteon, All right is reserved
  File name: locator.c
Author:Willer_Xu     Version: 1.0      Date: 2010.06.07

Description: This program achieve the function of the "locator device side", the main fuction as follow:
1) response the "Discovery" command from the "locator control side"
2) response the "Log on authentication" command from the "locator control side"
3) response the "Get IP information" command from the "locator control
4) response the "Set IP" command from the "locator control side"
5) response the "Default factory" command from the "locator control side"
6) response the "Device info" command from the "locator control side"
7) response the "Reboot" command from the "locator control side"

Function list:
1) static void netbuf_fwd_write_u8(u8_t wData, u8_t **ppBuffer) //write the data of the u8_t type into the buffer, \
then the *ppBuffer + 1
2) static u8_t netbuf_fwd_read_u8(u8_t **ppBuffer) //read the data from buffer, which type is u8_t, \
then *ppBuffer + 1
3) int socket_UdpBindFixed(int *pSoc, int port)  //create and bind the socket
4) int socket_UdpReceive(int sock, UdpParams *pParams)  //receive the buffer from the network
5) u16_t checksum(u16_t *pcheck, int check_len)  //compute the checksum
6) void apCfgPasswordAuthGet(char * pwstr)  //read the encrypted password get from the receive buffer
7) void apCfgCurrentChannelGet(int * currentchannel)  //get the current channel
8) int sysLanEnetAddrGet(char *enetAdrs)  //get the MAC Address
9) void locator_Recv_Udp_Netbuf(int sock, UdpParams *para, u64_t src_addr, u16_t sequence, u16_t type, u8_t **requestBuffer)  //it is the chief function, \
which achieve the response function of the "locator device side"
10) void locator_Udp_Recv(int sock, UdpParams *para)  //check welther deal with the received buffer, according to comparing the MAC address
11) void Start_Locator_Processor(void)  //start the program 

Author:Willer_Xu    Date: 2010.06.10
Add the function "void sysLanIpInfoGet(char *ipaddress, char *netmask)", which is used to get the ip address and netmask using ioctl() when the DHCP is enable;
Because the ipaddress and netmask do not write into config file which are get from DHCP Server.
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "uci.h"                   //header file of the config_get()
//#include "ezp-lib.h"
//#include "bcmnvram.h"
#include "nvram.h"
#include "nvram_rule.h"

#include "mid_func.h"
#include "mid_detail.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include <syslog.h>
#include <signal.h>


#define LOCATOR_UDP_PORT            0x5168
#define LOCATOR_UDP_TYPE            0xC001
#define UDP_CLASS                   0x00010001

#define UDP_METHOD_DISCOVER         0x0001
#define UDP_METHOD_GET_IP           0x0002
#define UDP_METHOD_SET_IP           0x0003
#define UDP_METHOD_RESET            0x0004
#define UDP_METHOD_AUTH             0x0005
#define UDP_METHOD_GET_INFO         0x000A
#define UDP_METHOD_REBOOT           0x000B
#define UCP_METHOD_GET_SN               0x000C
#define UCP_METHOD_SET_SN               0x000D
#define UCP_METHOD_GET_HOST     0x000E
#define UCP_METHOD_SET_HOST     0x000F

#define UDP_CODE_AUTH_NAME          0x01
#define UDP_CODE_DEVICE_NAME        0x02
#define UDP_CODE_DEVICE_TYPE        0x03
#define UDP_CODE_USE_DHCP           0x04
#define UDP_CODE_IP_ADDR            0x05
#define UDP_CODE_SUBNET_MASK        0x06
#define UDP_CODE_GATEWAY_ADDR       0x07
#define UDP_CODE_SOFTWARE_VER       0x08
#define UDP_CODE_AUTH_PASSWORD      0x09
#define UDP_CODE_SYSTEM_NAME        0x0A
#define UDP_CODE_SSID               0x10
#define UDP_CODE_CHANNEL            0x11
#define UDP_CODE_SEC_METHOD         0x12
#define UDP_CODE_ENCRYPTION         0x13
#define UDP_CODE_SET_SUCCESS 		0X14
#define UDP_CODE_SET_FAIL 			0X15
#define UDP_CODE_SET_NONEED 		0x16
#define UDP_CODE_OPMODE             0x18
#define UDP_CODE_WIRELESS_MODE      0x19
#define UDP_CODE_WLAN_MACADDR       0x1B
#define UCP_CODE_BRIDGE 			0x1D
#define UDP_CODE_ENCRYPT_ONOFF      0x20
#define UDP_CODE_END                0xFF
#define UCP_CODE_HOST_NAME      	0x22  //host name
#define UCP_CODE_DOMAIN_NAME    	0x23 //domain name
#define UCP_CODE_ASSOCIATED    		0x24
#define UCP_CODE_BSSID    			0x25
#define UCP_CODE_RSSI    			0x26
#define UDP_CODE_5GWIRELESS_MODE	0x27
#define UDP_CODE_5GWLAN_MACADDR		0x28
#define UDP_CODE_5GSSID				0x29
#define UDP_CODE_5GRSSI    			0x2A
#define UDP_CODE_5GBSSID    		0x2B
#define UDP_CODE_5GASSOCIATED   	0x2C
#define UDP_CODE_5GCHANNEL			0x2D
#define UDP_CODE_5GSEC_METHOD		0x2E
#define UDP_CODE_5GENCRYPT_ONOFF	0x2F
#define UDP_CODE_2GRADIO_ONOFF		0x30
#define UDP_CODE_5GRADIO_ONOFF		0x31
#define UDP_CODE_WLAN_OPMODE		0x32
#define UDP_CODE_5GWLAN_OPMODE		0x33
#define UDP_CODE_HTTP_TYPE       	0x34

#define CFG_MAX_PASSPHRASEKEY        64
#define MAX_UDP_PACKET_SIZE          512
#define LOCATOR_AUTHTYPE_PSK2MIX         7
#define LOCATOR_AUTHTYPE_PSK2         6
#define LOCATOR_AUTHTYPE_PSK         5
#define LOCATOR_AUTHTYPE_WPA2MIX         4
#define LOCATOR_AUTHTYPE_WPA2         3
#define LOCATOR_AUTHTYPE_WPA         2
#define LOCATOR_AUTHTYPE_WEP         1
#define LOCATOR_AUTHTYPE_NO_SECURITY 0
#define DHCP_DISABLE                 0x00//static
#define DHCP_ENABLE                  0x01
#define GATEWAY_EMPTY                "0.0.0.0"
#define NETMASK_EMPTY                "0.0.0.0"
#define BSSID_EMPTY                "00:00:00:00:00:00"

#define OPERATION_MODE_NORMAL    	0
#define OPERATION_MODE_AP           1
#define OPERATION_MODE_WISP0      	2
#define OPERATION_MODE_STA0        	3
#define OPERATION_MODE_WDSSTA0  	4
#define OPERATION_MODE_WISP1      	5
#define OPERATION_MODE_STA1        	6
#define OPERATION_MODE_WDSSTA1  	7
#define OPERATION_MODE_UR           8

#define WIRELESS_MODE_BG            4
#define WIRELESS_MODE_BGN           8 
#define WIRELESS_MODE_B            11
#define WIRELESS_MODE_G            12 
#define WIRELESS_MODE_N            13
#define UDP_CODE_ENCRYPT_OFF        0
#define UDP_CODE_ENCRYPT_ON         1
#define UDP_CODE_RADIO_OFF			0
#define UDP_CODE_RADIO_ON			1
#define UDP_CODE_WLAN_AP			0
#define UDP_CODE_WLAN_CLIENT		1
#define HTTPS_DISABLE              	0
#define HTTPS_ENABLE              	1


#define DEVICE_TYPE "Access Point"       //define the device_type
#define TRUE 0
#define FALSE -1
#define CFG_FILE_MAX_LINE 100           //when read the file, it's the max length of the line 
#define BROADCAST_ADDR 0xFFFFFFFF       //broadcast address

#define SHORT_BUF_LEN   32
#define TMP_LEN 256



#define UNICASE

//define the type
typedef signed char             s8_t;
typedef unsigned char           u8_t;
typedef signed short int        s16_t;
typedef unsigned short int      u16_t;
typedef signed long int         s32_t;
typedef unsigned long int       u32_t;
typedef signed long long int    s64_t;
typedef unsigned long long int  u64_t;
void myprint(int count, char* aa);


/*-------------<Global Values>--------------*/
#define USE_DAEMON
u8_t    requestBuffer[MAX_UDP_PACKET_SIZE];
u16_t   ip_id;
int debug = 0;
#define print_debug(format, arg...) do{   \
	if(debug ==1) printf(format, ##arg) ;    \
}while(0)

//struct of the UDP Header
struct udphdr
{
	u16_t    uh_sport;        /* source port */
	u16_t    uh_dport;        /* destination port */
	short    uh_ulen;         /* udp length */
	u16_t    uh_sum;          /* udp checksum */
};

//struct of the 802.1Q virtual lan header
struct vlanhdr
{
	u16_t  vlanpcid;
	u16_t vlantype;
};

//struct of the client(control side)
struct locator_udp_client
{
	u16_t    opt;            /* client option */
	u32_t    clientAddr;  /* client address */
	u16_t    clientPort;  /* client port */
};

//struct of the server(device side) 
struct locator_udp_server
{
	u16_t    opt;               /* server option */
	u8_t     localMacAddr[6];   /* device's MAC address */
};

//define the send buffer's structer 
typedef struct
{
	struct    ip                   ipLayer;        /* ip header */
	struct    udphdr               udpHeader;      /* udp Header */
	struct    locator_udp_client   client;
	struct    locator_udp_server   server;
	u16_t    sequence;                 /* the number of the sequence */
	u16_t    ldap_type;
	u8_t     flag;
	u32_t    ldap_class;
	u8_t     SendData[200];
} locator_udp_reply_packet_header;

typedef struct UdpParams
{
	char         *pRecPacket;
	char         *pSendPacket;
	unsigned int clientAddr;
	unsigned int sendPacketLength;
	unsigned int recvPacketLength;
	u16_t clientPort;
	u8_t macAddress[6];
} UdpParams;

typedef struct UdpSn
{
	char *Sn;
	int length;
}UdpSn;

typedef struct UdpHost
{
	char *hostName;
	int hostName_length;
	char *domainName;
	int domainName_length;
}UdpHost;

typedef struct IpInfo
{
	char bridge;
	char dhcp_enable;
	u32_t ipaddr;
	u32_t subnet;
	u32_t gateway;

}IpInfo;

typedef struct HostInfo
{
	u32_t ipaddr;
	int hostName_length;
	char hostName[255];
	int domainName_length;
	char domainName[255];
}HostInfo;

IpInfo recv_ipinfo;
HostInfo recv_hostinfo;
static volatile int quitting = 0;

char* mask2prefix(char *mask)
{
	if (!strcmp(mask, "0.0.0.0"))
		mask="0";
	else if (!strcmp(mask, "128.0.0.0"))
		mask="1";
	else if (!strcmp(mask, "192.0.0.0"))
		mask="2";
	else if (!strcmp(mask, "224.0.0.0"))
		mask="3";
	else if (!strcmp(mask, "240.0.0.0"))
		mask="4";
	else if (!strcmp(mask, "248.0.0.0"))
		mask="5";
	else if (!strcmp(mask, "252.0.0.0"))
		mask="6";
	else if (!strcmp(mask, "254.0.0.0"))
		mask="7";
	else if (!strcmp(mask, "255.0.0.0"))
		mask="8";
	else if (!strcmp(mask, "255.128.0.0"))
		mask="9";
	else if (!strcmp(mask, "255.192.0.0"))
		mask="10";
	else if (!strcmp(mask, "255.224.0.0"))
		mask="11";
	else if (!strcmp(mask, "255.240.0.0"))
		mask="12";
	else if (!strcmp(mask, "255.248.0.0"))
		mask="13";
	else if (!strcmp(mask, "255.252.0.0"))
		mask="14";
	else if (!strcmp(mask, "255.254.0.0"))
		mask="15";
	else if (!strcmp(mask, "255.255.0.0"))
		mask="16";
	else if (!strcmp(mask, "255.255.128.0"))
		mask="17";
	else if (!strcmp(mask, "255.255.192.0"))
		mask="18";
	else if (!strcmp(mask, "255.255.224.0"))
		mask="19";
	else if (!strcmp(mask, "255.255.240.0"))
		mask="20";
	else if (!strcmp(mask, "255.255.248.0"))
		mask="21";
	else if (!strcmp(mask, "255.255.252.0"))
		mask="22";
	else if (!strcmp(mask, "255.255.254.0"))
		mask="23";
	else if (!strcmp(mask, "255.255.255.0"))
		mask="24";
	else if (!strcmp(mask, "255.255.255.128"))
		mask="25";
	else if (!strcmp(mask, "255.255.255.192"))
		mask="26";
	else if (!strcmp(mask, "255.255.255.224"))
		mask="27";
	else if (!strcmp(mask, "255.255.255.240"))
		mask="28";
	else if (!strcmp(mask, "255.255.255.248"))
		mask="29";
	else if (!strcmp(mask, "255.255.255.252"))
		mask="30";
	else if (!strcmp(mask, "255.255.255.254"))
		mask="31";
	else if (!strcmp(mask, "255.255.255.255"))
		mask="32";
	else
		mask="";

	return mask;
}

char* prefix2mask(int idx)
{ 
	char *mask;
	switch(idx) //Translate AXIM's netmask to AboCom's UI format
	{
		case 0:
			mask="0.0.0.0";
			break;
		case 1:
			mask="128.0.0.0";
			break;
		case 2:
			mask="192.0.0.0";
			break;
		case 3:
			mask="224.0.0.0";
			break;
		case 4:
			mask="240.0.0.0";
			break;
		case 5:
			mask="248.0.0.0";
			break;
		case 6:
			mask="252.0.0.0";
			break;
		case 7:
			mask="254.0.0.0";
			break;    
		case 8:
			mask="255.0.0.0";
			break;
		case 9:
			mask="255.128.0.0";
			break;
		case 10:
			mask="255.192.0.0";
			break;
		case 11:
			mask="255.224.0.0";
			break;
		case 12:
			mask="255.240.0.0";
			break;
		case 13:
			mask="255.248.0.0";
			break;
		case 14:
			mask="255.252.0.0";
			break;
		case 15:
			mask="255.254.0.0";
			break;
		case 16:
			mask="255.255.0.0";
			break;
		case 17:
			mask="255.255.128.0";
			break;
		case 18:
			mask="255.255.192.0";
			break;
		case 19:
			mask="255.255.224.0";
			break;
		case 20:
			mask="255.255.240.0";
			break;
		case 21:
			mask="255.255.248.0";
			break;
		case 22:
			mask="255.255.252.0";
			break;
		case 23:
			mask="255.255.254.0";
			break;
		case 24:
			mask="255.255.255.0";
			break;
		case 25:
			mask="255.255.255.128";
			break;
		case 26:
			mask="255.255.255.192";
			break;
		case 27:
			mask="255.255.255.224";
			break;
		case 28:
			mask="255.255.255.240";
			break;
		case 29:
			mask="255.255.255.248";
			break;
		case 30:
			mask="255.255.255.252";
			break;
		case 31:
			mask="255.255.255.254";
			break;
		case 32:
			mask="255.255.255.255";
			break;
		default:
			mask="";
	}

	return mask;
}

void sendPacketToPC(locator_udp_reply_packet_header * pReplyDataHeader, UdpParams *para, u8_t* test,  struct ip *ipstr, struct udphdr *udpstr);
/***************** write the data into send buffer *************************/
static void netbuf_fwd_write_u8(u8_t wData, u8_t **ppBuffer)
{
	**ppBuffer = wData;
	*ppBuffer += 1;
}

static void netbuf_fwd_write_un(int n, u8_t wData, u8_t **ppBuffer)
{

}

static void netbuf_fwd_write_u16(u16_t wData, u8_t **ppBuffer)
{
	netbuf_fwd_write_u8( (u8_t)(wData >> 8), ppBuffer );
	netbuf_fwd_write_u8( (u8_t)(wData & 0x00FF), ppBuffer );
}

static void netbuf_fwd_write_u32(u32_t wData, u8_t **ppBuffer)
{
	netbuf_fwd_write_u16( (u16_t)(wData >> 16), ppBuffer );
	netbuf_fwd_write_u16( (u16_t)(wData & 0x0000FFFF), ppBuffer );
}
//------------------------------------------------------------------------------------
/***************** read the data from the received buffer *************************/
static u8_t netbuf_fwd_read_u8(u8_t **ppBuffer)
{
	u8_t vl;
	vl = **ppBuffer;
	*ppBuffer += 1;
	return vl;
}

static void net_fwd_read_sn(u8_t **ppBuffer, UdpSn*  udpSn, int length)
{

	udpSn->length = length;
	udpSn->Sn= *ppBuffer;
	*ppBuffer += length;


}

static void net_fwd_read_host(u8_t **ppBuffer, UdpHost* udpHost, 
		int hostName_length, int domainName_length)
{
	if(hostName_length > 0){
		udpHost->hostName_length = hostName_length;
		udpHost->hostName = *ppBuffer;
		*ppBuffer += hostName_length;
	}else if(domainName_length > 0){
		udpHost->domainName_length = domainName_length;
		udpHost->domainName = *ppBuffer;
		*ppBuffer += domainName_length;
	}

}

static u16_t netbuf_fwd_read_u16(u8_t **ppBuffer)
{
	u16_t vh;
	u16_t vl;

	vh = (u16_t)netbuf_fwd_read_u8(ppBuffer);
	vl = (u16_t)netbuf_fwd_read_u8(ppBuffer);

	return (vh << 8) | vl;
}

static u32_t netbuf_fwd_read_u32(u8_t **ppBuffer)
{
	u16_t vh = netbuf_fwd_read_u16(ppBuffer);
	u16_t vl = netbuf_fwd_read_u16(ppBuffer);

	return ((u32_t)vh << 16) | (u32_t)vl;
}

static u64_t netbuf_fwd_read_u64(u8_t **ppBuffer)
{
	u32_t vh = netbuf_fwd_read_u32(ppBuffer);
	u32_t vl = netbuf_fwd_read_u32(ppBuffer);

	return ((u64_t)vh << 32) | (u64_t)vl;
}

//---------------------------
//set socket opt to avoid the port to be used by other process
static void setSocketOption(int s){
	int flag;
	flag = fcntl(s, F_GETFD, 0);
	/*
	 * FD_CLOEXEC: when fork a child thread, 
	 * 			the fd will be close by child thread automaticlly 
	 */
	if(fcntl(s, F_SETFD, flag|FD_CLOEXEC) < 0)
		print_debug("Fail to set fcntl(s, F_SETFD, flag|FD_CLOEXEC)");
}

//------------------------------------------------------------------------------------
//get ipaddress
	int
getifaddr(const char * ifname, char * buf, int len)
{
	/* SIOCGIFADDR struct ifreq *  */
	int s;
	struct ifreq ifr;
	int ifrlen;
	struct sockaddr_in * addr;
	ifrlen = sizeof(ifr);
	if(!ifname || ifname[0]=='\0'){
		print_debug("Ifname is empty\n");
		return -1;
	}
	s = socket(PF_INET, SOCK_DGRAM, 0);
	if(s < 0)
	{
		print_debug("Create socket error\n");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if(ioctl(s, SIOCGIFADDR, &ifr, &ifrlen) < 0)
	{
		print_debug("ioctl for SIOCGIFADDR error\n");
		close(s);
		return -1;
	}
	addr = (struct sockaddr_in *)&ifr.ifr_addr;
	if(!inet_ntop(AF_INET, &addr->sin_addr, buf, len))
	{
		print_debug("inet_ntop error\n");
		close(s);
		return -1;
	}

	close(s);
	int i;
	char ipaddr[len];
	for(i=0;i<len;i++){
		ipaddr[i] = *(buf+i);
	}
	ipaddr[i] = '\0';
	print_debug("getifaddr ipaddress =  %s", ipaddr);
	return 0;
}

/* get the firmware version */
int firmwareVersionGet(char verstr[])
{
	print_debug("firmwareVersionGet\n");
	FILE *fd;
	char c;
	int i = 0, pos = 0;

	if ((fd = fopen("/version", "r")) == NULL)
	{
		print_debug("fopen fail!\n");
		return -1;
	}
	/*while((c = fgetc(fd)) != EOF)
	  {
	  if(c == '.')
	  {
	  pos++;
	  if((pos + 1) == 3)
	  continue;  
	  } 

	  if((pos + 1) == 3 || (pos + 1) == 4 || (pos + 1) == 5)
	  {
	  verstr[i] = c;
	  i++;
	  if(i >= 256)
	  {
	  fclose(fd);
	  return -1;
	  }    
	  }
	  }*/
	//happiness update 20110510
	while((c = fgetc(fd)) != EOF)
	{
		print_debug("c= %c\n",c);
		verstr[i]=c;
		i++;

	}
	verstr[i] = '\0';
	printf("firmware version:%s\n",verstr);
	fclose(fd);
	return 0; 
}
//-----------------------------------------------------------------

/***************** create and bind socket *************************/
int socket_UdpBindFixed(int *pSoc, int port)
{
	print_debug("socket_UdpBindFixed\n");
	struct sockaddr_in LocalAddr;

	if ((*pSoc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)  //create the socket
	{
		printf("Create socket error!\n");
		return -1;
	}
	setSocketOption(*pSoc );
	/* define the socket address struct */
	memset(&LocalAddr, 0x00, sizeof(LocalAddr));
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	LocalAddr.sin_port = htons((short)port);

	/* bind socket to local address */
	if ((bind(*pSoc, (struct sockaddr *)&LocalAddr, sizeof(struct sockaddr))) == -1)
	{
		printf("Bind localaddr failure!\n");
		return -1;
	}

	return 0;
}


//------------------------------------------------------------------------------------
/***************** receive the data *************************/
int socket_UdpReceive(int sock, UdpParams *pParams)
{
	printf("socket_UdpReceive\n");
	struct sockaddr_in *pAddr;
	int iAddrlen = sizeof(struct sockaddr_in);
	int recvResult;

	/* malloc memory */
	if ((pAddr = (struct sockaddr_in *)malloc(iAddrlen)) == NULL)
	{
		printf("malloc memory failure\n");
		return -1;
	}    

	/* receive the buffer */
	recvResult = recvfrom(sock, (char *)pParams->pRecPacket, MAX_UDP_PACKET_SIZE, 0, (struct sockaddr *)pAddr, &iAddrlen);
	printf("recvfrom (char *)pParams->pRecPacket =%s : recvResult = %d\n", (char *)pParams->pRecPacket, recvResult);

	myprint(recvResult, (char *)pParams->pRecPacket);
	if (recvResult <= 0)
	{
		printf("Receive the packet failure!\n");
		free(pAddr);
		return -1;
	}
	print_debug("socket_UdpReceive memcpy\n");
	pParams->clientAddr = ntohl(pAddr->sin_addr.s_addr);
	print_debug("client ipaddr:%08X\n" , pAddr->sin_addr.s_addr);
	struct in_addr addr1 ;
	memcpy(&addr1, &(pAddr->sin_addr.s_addr), 4); 
	//inet_ntoa(addr1);
	print_debug("inet_ntoa(pAddr->sin_addr.s_addr:%s\n",  inet_ntoa(addr1));
	pParams->clientPort = ntohs(pAddr->sin_port);

	pParams->recvPacketLength = (unsigned int)recvResult;
	free(pAddr);

	return 0;
}
//------------------------------------------------------------------------------------
/***************** calculate the ip checksum *************************/
u16_t ipChecksum(u16_t *pcheck, int check_len)
{
	print_debug("ipChecksum\n");
	int nleft = check_len;
	u32_t sum = 0;
	u16_t *p = pcheck;
	u16_t result = 0;

	while (nleft > 1)
	{
		sum += *p++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(u8_t *)(&result) = *(u8_t *)p;
		sum += result;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = (u16_t)(~sum);
	return result;
}

/***************** calculate the udp checksum *************************/
u16_t udpChecksum(struct ip *iphdr, struct udphdr *udphd, u8_t *databuffer, int datalen)
{
	print_debug("udpChecksum\n");
	u16_t sum = 0;
	u8_t buf[1024];
	u8_t *ptr = buf;
	int checksumlen = 0;
	int i;
	u8_t zero = 0x00;
	u8_t protocol = 0x11; //UDP    

	//add the pseudo header, it's length is 12 byte 
	memcpy(ptr, &iphdr->ip_src.s_addr, 4);
	memcpy(ptr+4, &iphdr->ip_dst.s_addr, 4);
	memcpy(ptr+8, &zero, 1);
	memcpy(ptr+9, &protocol, 1);
	memcpy(ptr+10, &udphd->uh_ulen, 2);
	checksumlen += 12;    

	//udp header
	memcpy(ptr+12, udphd, 8);
	checksumlen += 8;

	//data 
	memcpy(ptr+20, databuffer, datalen);
	checksumlen += datalen;

	//calculate the udp checksum
	sum = ipChecksum((u16_t *)buf, checksumlen);

	return sum;
}
//---------------------------------------------------------
/***************** encrypte the password1 by the system("httpd -m password") get from the requestbuffer, \
  then load to the temp file(/tmp/pwcheckfile. we read the password1 from the temp file which has been encrypted, \
  because the password in config file has been encrypted *************************/
void apCfgPasswordAuthGet(char * pwstr, char authTemp[])
{
	print_debug("apCfgPasswordAuthGet\n");
	int j, k, len = 0;
	FILE *fd;
	char string[100], 
	     authcmd[100],
	     buf[CFG_FILE_MAX_LINE],
	     *tempcheck;

	sprintf(authcmd, "httpd -m \'%s\' > /tmp/pwcheckfile", authTemp);
	system(authcmd);

	memset(string, 0, sizeof(string));
	/* open the temp file */
	if ((fd = fopen("/tmp/pwcheckfile", "r")) == NULL)
		printf("fopen fail!\n");

	/* get one line every time and read the data */
	while (fgets(buf, CFG_FILE_MAX_LINE, fd) != NULL)
	{
		tempcheck = buf;	
		k = 0;
		for (j=0; (*(tempcheck+k) != ' ') && (*(tempcheck+k) != '\0')&& (*(tempcheck+k) != '\n'); j++)
		{	
			string[j] = *(tempcheck+k);
			k++;
		}

		len = strlen(string);
		strncpy(pwstr, string, len+1);
		break;
	}

	fclose(fd);
	system("rm -rf /tmp/pwcheckfile");          //delete the temp file
}
//---------------------------------------------------------
/*****************get the current channel *************************/
/* we use the system("iwlist channel > /tmp/channeltempfile") to load the channel information to the temp file(/tmp/channeltempfile). \
   Then we read the current channel from the temp file */
//void apCfgCurrentChannelGet(int *currentchannel, int opmode)
int apCfgCurrentChannelGet(int opmode)
{
	int getChannel = 0;
	print_debug("apCfgCurrentChannelGet\n");
	FILE *fd;
	int okOrnot = 0;               //when get the channel number, it equal to 1
	char	buf[CFG_FILE_MAX_LINE], channel[5],channeltemp[5];
	int i = 0;
	int len = 0;
	memset(channel,0,strlen(channel));
	char *tmp = buf;
	system("iwlist channel|grep Current > /tmp/channeltempfile");  //get the channel file and load to the temp file 

	if ((fd = fopen("/tmp/channeltempfile", "r")) == NULL){
		printf("fopen fail!\n");
		return -1;
	}
	while (fgets(buf, 100, fd) != NULL)
	{
		for(i;*(tmp+i)!='\0';i++){
			//printf("*(tmp+i)=%c\n", *(tmp+i));
			if(*(tmp+i) == '('){
				break;
			}
		}

		channel[0] = *(tmp+i+9);
		printf("*(tmp+i+9)=%c\n",*(tmp+i+9));
		if(*(tmp+i+10)!=')'){
			printf("*(tmp+i+10)=%c\n",*(tmp+i+10));
			channel[1]=*(tmp+i+10);
			channel[2]='\0';

			len=2;
		}else{
			channel[1]='\0';
			len=1;
		}

		printf("len=%d\n",len);
		strcpy(channeltemp, channel);

	}


	// *currentchannel = atoi(channeltemp);
	getChannel = atoi(channeltemp);;
	//print_debug("*currentchannel = %d\n", *currentchannel );
	print_debug("aaaaaaaaaa\n");

	fclose(fd);
	system("rm -rf /tmp/channeltempfile");  //delete the temp file
	print_debug("bbbbbbbbbb,%d\n", getChannel);
	return getChannel;
}
//---------------------------------------------------------
/*****************get the MAC Address using the ioctl function*************************/
int sysLanEnetAddrGet(char *enetAdrs)
{
	print_debug("sysLanEnetAddrGet begin\n");
	int i, macsock;
	struct ifreq ifr;
	char ethdev[6];
	memset(&ifr, 0, sizeof(struct ifreq));

	if ((macsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("get the mac socket file descriptor faild!\n");

	memset(ethdev,0,sizeof(ethdev));
	ezplib_get_attr_val("port_device_rule", 0, "port_device", ethdev, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	strcpy(ifr.ifr_name, ethdev);
	if (ioctl(macsock, SIOCGIFHWADDR, &ifr)<0)
		perror("ioctl:\n");

	for (i = 0; i< 6; i++)
	{
		enetAdrs[i] = ifr.ifr_hwaddr.sa_data[i];
	}

	close(macsock);
	print_debug("sysLanEnetAddrGet over\n");
	return 0;
}

/*****************get the MAC Address using the ioctl function*************************/
int sysAddrGet(char *dev, char *Adrs)
{
	print_debug("sysAddrGet begin\n");
	int i, macsock;
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));

	if ((macsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("get the mac socket file descriptor faild!\n");

	strcpy(ifr.ifr_name, dev);
	if (ioctl(macsock, SIOCGIFHWADDR, &ifr)<0)
		perror("ioctl:\n");

	for (i = 0; i< 6; i++)
	{
		Adrs[i] = ifr.ifr_hwaddr.sa_data[i];
	}

	close(macsock);
	print_debug("sysAddrGet over\n");
	return 0;
}


void delspace(char *Buf1, char* Buf2)
{
	while(*Buf1 != '\0')
	{
		if((*Buf1) == 0x20)
		{
			++Buf1; // if space skip 
		}
		else
			*Buf2++ = *Buf1++; // if not space copy to buf 2
	}
	*Buf2 = '\0'; //NULL Terminate

}

int getLocaRadioStatus(int radio, int *status)
{
	int ret = 0;
	int stt_tmp = UDP_CODE_RADIO_OFF;
	
	ret = get_radio_status(radio, &stt_tmp);
	if (T_FAILURE == ret)
	{
		*status = UDP_CODE_RADIO_OFF;
	}
	else
	{
		*status = stt_tmp;
	}
	
	return 0;
}

int getWIFIRSSI(int radio, char *buf) 
{
	int ret = 0;
	char rssitmp[30] = {0};
	char iOpMode[32] = {0};
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	}
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_rssi(radio, rssitmp);
		if (T_FAILURE == ret) {
			strcpy(buf, "-- dBm");
		} else {
			strcpy(buf, rssitmp);
		}
	} else {
		strcpy(buf, "--");
	}
	
	return 0; 
}

int getWirelessMode(int radio, int *mode)
{
	int ret = 0;
	int wmode = 8;
	char tmpbuf[32] = {0};
	char iOpMode[32] = {0};
	char wireless_mode[32] = {0};
	
	ret = get_wirelessmode(radio, &wmode);
	if (T_FAILURE == ret) {
		if (RADIO_2G == radio) {
			*mode = WMODE_11BGN;
		} else {
			*mode = WMODE_11AN;
		}
	} else {
		*mode = wmode;
	}
	
	return 0; 
}

int getAPConnectBSSID(int radio, char *buf)
{
	int ret = 0;
	char macBuf[18] = {0};
	char iOpMode[32] = {0};
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	}
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_bssid(radio, macBuf);
		if (T_FAILURE == ret) {
			strcpy(buf, BSSID_EMPTY);
		} else {
			strcpy(buf, macBuf);
		}
	} else {
		strcpy(buf, "--");
	}

	return 0;
}

int getAPConnectStatus(int radio, int *associated)
{
	int ret = 0;
	int assotmp = 0;
	char iOpMode[32] = {0};
	
	if (RADIO_2G == radio) {
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	} else {
		ezplib_get_attr_val("wl1_mode_rule", 0, "mode", iOpMode, 32, EZPLIB_USE_CLI);
	}	
	if (!strcmp(iOpMode, "client")) {
		ret = get_sta_assoc_status(radio, &assotmp);
		if (T_FAILURE == ret) {
			*associated = 2;
		} else {
			*associated = assotmp;
		}
	} else {
		*associated = 3;
	}

	return 0;
}

//-------------------------------------------------------------------
/********* get the ip address and netmask when the DHCP is enable, 
  because the ipaddress and netmask do not write into config file which are get from DHCP Server *********/
void sysLanIpInfoGet(char *ipaddress, char *netmask)
{
	print_debug("sysLanIpInfoGet\n");
	int i, ipsock;
	struct ifreq ifr;
	char *value;
	memset(&ifr, 0, sizeof(struct ifreq));

	if ((ipsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("get the ip socket file descriptor faild!\n");

	value = nvram_safe_get("lan0_ifname");
	strcpy(ifr.ifr_name, value);
	if (ioctl(ipsock,SIOCGIFADDR, &ifr)<0)
		perror("ioctl IP address:\n");
	strcpy(ipaddress, inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));

	if (ioctl(ipsock,SIOCGIFNETMASK, &ifr)<0)
		perror("ioctl netmask:\n");
	strcpy(netmask, inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));

	close(ipsock);
}
//-------------------------------------------------------------------

/******** fill the udp header and ip header in the send packet *********/
void locator_UDP_and_IP_Header_Set( locator_udp_reply_packet_header *pReplyDataHeader, UdpParams *para, u8_t *test)
{
	print_debug("locator_UDP_and_IP_Header_Set\n");
	char dhcptemp[10], ipaddrtemp[16], subnetmasktemp[16];
	struct in_addr addr1 ;
	register int hlen = sizeof(struct ip); 
	memset(dhcptemp, 0, sizeof(dhcptemp));
	memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
	memset(subnetmasktemp, 0, sizeof(subnetmasktemp));

	//-------------------- <UDP Header> -------------------------
	pReplyDataHeader->udpHeader.uh_sport = htons(LOCATOR_UDP_PORT);
	pReplyDataHeader->udpHeader.uh_dport = htons(LOCATOR_UDP_PORT);
	pReplyDataHeader->udpHeader.uh_ulen = ntohs(para->sendPacketLength += 8);//ntohs???
	pReplyDataHeader->udpHeader.uh_sum = 0x0000;  

	//if the DHCP is disable, we get the ip address from the config file, or, we get using the ioctl function
	//when the DHCP is enable, the ip address distributed by the DHCP server is not writted into the config file
	//config_get(dhcptemp, 10, "network.lan.proto");
	char * m_Ipaddr;
	m_Ipaddr = nvram_safe_get("lan0_ipaddr");

	/*if (strcmp(dhcptemp, "static") == 0)
	  {
	  config_get(ipaddrtemp, 16, "network.lan.ipaddr");
	  }
	  else
	  {
	  sysLanIpInfoGet(ipaddrtemp, subnetmasktemp);    
	  }*/

	pReplyDataHeader->ipLayer.ip_src.s_addr = (u32_t)inet_addr(m_Ipaddr);
	//print_debug("(u32_t)inet_addr(ipaddrtemp): %d\n", (u32_t)inet_addr(m_Ipaddr));
	memcpy(&addr1, &(pReplyDataHeader->ipLayer.ip_src.s_addr), 4); 
	print_debug("inet_ntoa(pReplyDataHeader->ipLayer.ip_src.s_addr:%s\n",  inet_ntoa(addr1));
#ifdef UNICASE
	if(pReplyDataHeader->ipLayer.ip_src.s_addr == 0 |
			(pReplyDataHeader->ipLayer.ip_src.s_addr&(u32_t)inet_addr("255.255.255.0"))
			!= ((u32_t)(htonl(para->clientAddr))&(u32_t)inet_addr("255.255.255.0"))){
#endif
		//if ip is "0.0.0.0",then broadcast
		print_debug("broad ip address\n");
		pReplyDataHeader->ipLayer.ip_dst.s_addr = BROADCAST_ADDR;
#ifdef UNICASE
	}else{

		print_debug("peer ip address\n");
		pReplyDataHeader->ipLayer.ip_dst.s_addr = (u32_t)(htonl(para->clientAddr));
		memcpy(&addr1, &(pReplyDataHeader->ipLayer.ip_dst.s_addr), 4); 
		print_debug("inet_ntoa(pAddr->ip_dst.s_addr:%s\n",  inet_ntoa(addr1));
		//print_debug("(u32_t)para->clientAddr: %d\n", (u32_t)para->clientAddr);

	}
#endif
	// To caculater the udp header checksum, we must have the source address and destnation address of IP header
	pReplyDataHeader->udpHeader.uh_sum = udpChecksum(&pReplyDataHeader->ipLayer, &pReplyDataHeader->udpHeader, test, para->sendPacketLength - 8); 

	//-------------------- <IP Header> -------------------------
	pReplyDataHeader->ipLayer.ip_tos = 0;
	pReplyDataHeader->ipLayer.ip_off = 0;
	pReplyDataHeader->ipLayer.ip_len = para->sendPacketLength += 20;
	pReplyDataHeader->ipLayer.ip_p =   0x11;//(UDP) 
	pReplyDataHeader->ipLayer.ip_hl =  5;
	pReplyDataHeader->ipLayer.ip_v =   4;
	pReplyDataHeader->ipLayer.ip_ttl = 0x80;
	pReplyDataHeader->ipLayer.ip_sum = 0x00;
	pReplyDataHeader->ipLayer.ip_id = htons(ip_id++);


	hlen = pReplyDataHeader->ipLayer.ip_hl << 2;
	pReplyDataHeader->ipLayer.ip_len = htons((u16_t)pReplyDataHeader->ipLayer.ip_len);
	pReplyDataHeader->ipLayer.ip_off = htons((u16_t)pReplyDataHeader->ipLayer.ip_off);
	pReplyDataHeader->ipLayer.ip_sum = ipChecksum((u16_t *)&pReplyDataHeader->ipLayer, 20);
}
//-------------------------------------------------------------------


/********check the ip infomation if the same as local*/

int  check_IpInfo(IpInfo ipInfo){
	print_debug("check_IpInfo begin..\n");
	int b_change = 0;
	print_debug("ipInfo.ipaddr =%d,recv_ipinfo.ipaddr = %d\n", ipInfo.ipaddr, recv_ipinfo.ipaddr);
	print_debug("ipInfo.subnet =%d,recv_ipinfo.subnet = %d\n", ipInfo.subnet, recv_ipinfo.subnet);
	print_debug("ipInfo.gateway =%d,recv_ipinfo.gateway = %d\n", ipInfo.gateway, recv_ipinfo.gateway);
	if(ipInfo.dhcp_enable == recv_ipinfo.dhcp_enable){
		if(ipInfo.dhcp_enable == DHCP_DISABLE){
			if(ipInfo.ipaddr != recv_ipinfo.ipaddr 
					||ipInfo.subnet != recv_ipinfo.subnet
					||ipInfo.gateway != recv_ipinfo.gateway)
			{
				printf("branch 1\n");
				b_change = 1;
			}
		}
	}else{
		print_debug("branch 2\n");
		b_change = 1;
	}
	return b_change;
}

/*******get local ip infomation*/
IpInfo get_local_ipinfo(){
	IpInfo ipinfo;
	char
		bridgetemp[10], //bridge mode  0x01:AP 0x02:router
		dhcptemp[10],
		gatewaytemp[16],
		subnetmasktemp[16],
		ipaddrtemp[16],
		lan0proto[10];
	char *value;
	int idx;
	print_debug("get_local_ipinfo begin...\n");
	memset(bridgetemp,0,sizeof(bridgetemp));
	ezplib_get_attr_val("system_rule", 0, "name", bridgetemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	//config_get(bridgetemp,10,"network.mode.bridge");
	char bridge;
	if (strcmp(bridgetemp, "ur") == 0 || strcmp(bridgetemp, "ap") == 0 || strcmp(bridgetemp, "sta0") == 0){
		ipinfo.bridge = 0x01;
	}else{
		ipinfo.bridge = 0x02;
	}
	printf("ipinfo.bridge = %d\n", ipinfo.bridge);
	//value = nvram_safe_get("lan0_proto");
	ezplib_get_attr_val("lan0_proto", 0, "curr", lan0proto, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	/*memset(dhcptemp, 0, sizeof(dhcptemp));
	  config_get(dhcptemp, 10, "network.lan.proto");*/
	if (strcmp(lan0proto, "static") == 0)
		ipinfo.dhcp_enable = DHCP_DISABLE;
	else
		ipinfo.dhcp_enable = DHCP_ENABLE;
	printf("ipinfo.dhcp_enable = %d\n", ipinfo.dhcp_enable);

	/*memset(gatewaytemp, 0, sizeof(gatewaytemp));
	  config_get(gatewaytemp, 16, "network.lan.gateway");*/

	value = nvram_safe_get("lan0_gateway");
	if (strcmp(value, "") == 0)
		ipinfo.gateway = htonl(inet_addr(GATEWAY_EMPTY));
	else
		ipinfo.gateway = htonl(inet_addr(value));

	print_debug("ipinfo.gateway =%d \n", ipinfo.gateway);

	/*memset(subnetmasktemp, 0, sizeof(subnetmasktemp));
	  config_get(subnetmasktemp, 16, "network.lan.netmask");*/

	value = nvram_safe_get("lan0_mask");
	idx = atoi(value);
	value = prefix2mask(idx);
	print_debug("subnet %s\n",value);
	if (strcmp(value, "") == 0)
		ipinfo.subnet =  htonl(inet_addr(NETMASK_EMPTY));
	else
		ipinfo.subnet =  htonl(inet_addr(value));

	print_debug("ipinfo.subnet = %d\n", ipinfo.subnet);

	/*memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
	  config_get(ipaddrtemp, 16, "network.lan.ipaddr");*/
	value = nvram_safe_get("lan0_ipaddr");
	strcpy(ipaddrtemp,value);
	ipinfo.ipaddr = htonl(inet_addr(ipaddrtemp));
	print_debug("ipinfo.ipaddr = %d\n", ipinfo.ipaddr);
	print_debug("get_local_ipinfo finish...\n");
	return ipinfo;

}

/*******get recv ip infomation*/
IpInfo get_recv_ipinfo(u8_t **requestBuffercopy){
	u8_t code ;
	code = 0;
	int len ;
	print_debug("get_recv_ipinfo begin...\n");
	//memset(&recv_ipinfo,0,sizeof(recv_ipinfo));
	for ( code = netbuf_fwd_read_u8(requestBuffercopy);
			code != UDP_CODE_END;
			code = netbuf_fwd_read_u8(requestBuffercopy) )
	{
		printf("before len = %d code = %d\n", len, code);
		len = netbuf_fwd_read_u8(requestBuffercopy);
		printf("after len = %d code = %d\n", len, code);
		switch(code)
		{
			case UDP_CODE_USE_DHCP:
				printf("get_recv_ipinfo UDP_CODE_USE_DHCP\n");
				recv_ipinfo.dhcp_enable = netbuf_fwd_read_u8(requestBuffercopy);
				break;
			case UDP_CODE_GATEWAY_ADDR:
				printf("get_recv_ipinfo UDP_CODE_GATEWAY_ADDR\n");
				recv_ipinfo.gateway = netbuf_fwd_read_u32(requestBuffercopy);
				printf("recv_ipinfo.gateway = %d\n", recv_ipinfo.gateway);
				break;
			case UDP_CODE_SUBNET_MASK:
				printf("get_recv_ipinfo UDP_CODE_SUBNET_MASK\n");
				recv_ipinfo.subnet = netbuf_fwd_read_u32(requestBuffercopy);
				printf("recv_ipinfo.subnet = %d\n", recv_ipinfo.subnet);
				break;
			case UDP_CODE_IP_ADDR:
				printf("get_recv_ipinfo UDP_CODE_IP_ADDR\n");
				recv_ipinfo.ipaddr = netbuf_fwd_read_u32(requestBuffercopy);
				printf("recv_ipinfo.ipaddr = %d\n", recv_ipinfo.ipaddr);
				break;
			default:
				printf("\nError Code Type: %d\n",code);
				return;
		}

	}
	return recv_ipinfo;

}

//check host infomation

int check_HostInfo(HostInfo hostInfo)
{
	if(hostInfo.domainName_length != recv_hostinfo.domainName_length){
		print_debug("hostInfo.domainName_length= %d,recv_hostinfo.domainName_length=%d\n",hostInfo.domainName_length ,recv_hostinfo.domainName_length);
		return 1;
	}else if(hostInfo.hostName_length != recv_hostinfo.hostName_length){
		print_debug("hostInfo.hostName_length = %d, recv_hostinfo.hostName_length=%d\n",hostInfo.hostName_length,recv_hostinfo.hostName_length);
		return 1;
	}else if(hostInfo.ipaddr != recv_hostinfo.ipaddr){
		print_debug("hostInfo.ipaddr= %d,recv_hostinfo.ipaddr=%d\n ", hostInfo.ipaddr, recv_hostinfo.ipaddr);
		return 1;
	}else{
		int i ;
		for(i = 0; i < recv_hostinfo.domainName_length; i++)
		{
			if(recv_hostinfo.domainName[i] != hostInfo.domainName[i]){
				print_debug("i=%d, recv_hostinfo.domainName[i]=%c, hostInfo.domainName[i]=%c\n",
						i,recv_hostinfo.domainName[i],  hostInfo.domainName[i]);
				return 1;
			}
		}
		for(i = 0; i < recv_hostinfo.hostName_length; i++)
		{
			if(recv_hostinfo.hostName[i] != hostInfo.hostName[i]){
				print_debug("i=%d, recv_hostinfo.hostName[i]=%c, hostInfo.hostName[i]=%c\n",
						i,recv_hostinfo.hostName[i],  hostInfo.hostName[i]);
				return 1;
			}
		}
	}

	return 0;

}

//get local host infomation 
HostInfo  get_local_hostinfo()
{
	HostInfo hostInfo;
	char hostNameTemp[255],
	     domainNameTemp[255],
	     ipAddrTemp[16];
	char *value;
	/*memset(hostNameTemp,0,sizeof(hostNameTemp));
	  config_get(hostNameTemp, 255, "pedestal_client.server.hostname");*/
	value = nvram_safe_get("hostname");
	//memcpy(hostInfo.hostName, value, sizeof(value));
	strcpy(hostInfo.hostName, value);
	hostInfo.hostName_length = strlen(hostInfo.hostName);
	printf("hostInfo.hostName111 = %s\n", value);
	printf("hostInfo.hostName = %s\n", hostInfo.hostName);

	memset(domainNameTemp,0,sizeof(domainNameTemp));
	//config_get(domainNameTemp, 255, "pedestal_client.server.domainsname");
	ezplib_get_attr_val("lan_dhcps_rule", 0, "domain", domainNameTemp, TMP_LEN, EZPLIB_USE_CLI);
	//memcpy(hostInfo.domainName, domainNameTemp, sizeof(domainNameTemp)); 
	strcpy(hostInfo.domainName,domainNameTemp);
	hostInfo.domainName_length = strlen(hostInfo.domainName);
	printf("hostInfo.domainName = %s\n", hostInfo.domainName);

	/*memset(ipAddrTemp, 0, sizeof(ipAddrTemp));
	  config_get(ipAddrTemp, 16, "pedestal_client.server.address");*/
	value = nvram_safe_get("lan0_ipaddr");
	if (strcmp(value, "") == 0)
		hostInfo.ipaddr =  htonl(inet_addr(NETMASK_EMPTY));
	else
		hostInfo.ipaddr =  htonl(inet_addr(value));

	printf("hostInfo.ipaddr= %d\n", hostInfo.ipaddr);

	return hostInfo;
}

//get host infomation from receive message
HostInfo get_recv_hostinfo(u8_t** requestBuffercopy)
{
	u8_t code;
	code = 0;
	int len;
	UdpHost udpHost;
	HostInfo hostInfo;
	for(code = netbuf_fwd_read_u8(requestBuffercopy);
			code != UDP_CODE_END;
			code = netbuf_fwd_read_u8(requestBuffercopy) )
	{
		len = netbuf_fwd_read_u8(requestBuffercopy);
		printf("len = %d\n", len);
		switch(code)
		{
			case UDP_CODE_IP_ADDR:{
						      print_debug("case UDP_CODE_IP_ADDR\n");
						      hostInfo.ipaddr = netbuf_fwd_read_u32(requestBuffercopy);
						      print_debug("hostInfo.ipaddr = %d\n", hostInfo.ipaddr);
						      break;
					      }
			case UCP_CODE_DOMAIN_NAME:{
							  print_debug("case UCP_CODE_DOMAIN_NAME\n");
							  hostInfo.domainName_length = len;
							  net_fwd_read_host(requestBuffercopy, &udpHost, 0, len);
							  int i;
							  for(i=0;i < len; i++)
							  {
								  hostInfo.domainName[i] =  *(udpHost.domainName + i);
							  }
							  hostInfo.domainName[i] = '\0';
							  print_debug("hostInfo.domainName = %s\n", hostInfo.domainName);
							  break;
						  }
			case UCP_CODE_HOST_NAME:{
							print_debug("case UCP_CODE_HOST_NAME\n");
							hostInfo.hostName_length = len;
							net_fwd_read_host(requestBuffercopy, &udpHost, len, 0);
							int i;
							for(i=0;i < len; i++)
							{
								hostInfo.hostName[i] =  *(udpHost.hostName + i);
							}
							hostInfo.hostName[i] = '\0';
							print_debug("hostInfo.hostName = %s\n", hostInfo.hostName);
							break;
						}
			default:
						printf("get_recv_hostinfo error...\n");
						break;

		}

	}
	return hostInfo;
}

/******** The chief function, which achieve the function of the "locator device side" *********/
void locator_Recv_Udp_Netbuf(UdpParams *para, u64_t src_addr, u16_t sequence, u16_t type, u8_t **requestBuffer)
{       
	print_debug("locator_Recv_Udp_Netbuf\n");
	locator_udp_reply_packet_header replyDataHeader;
	locator_udp_reply_packet_header *pReplyDataHeader = &replyDataHeader;

	int    authErr              = TRUE,
	       i                    = 0,
	       iChannel             = 0,
	       iOpMode	        	= 0,
	       iWirelessMode        = 0,
	       iAuthentication      = 0,
	       rawsock              = 0,
	       ret                  = 0,
	       sd                   = 0,
	       vlanopen             = 0,
	       iRadiostatus	        = 0,
	       iWlanOPMode	        = 0,
	       ipSetFlag 			= 0,
	       associated			= 0,
		   https_enable			= 0;

	u8_t   authType             = 0x00,
	       flags                = 0x00,
	       *pSDB                = NULL,
	       dhcp_enable          = 0x00,
	       code                 = 0,
	       *test,
	       MacAddr[18],
	       MacAddrTemp[6],
	       rssi[30];

	u32_t  gateWay              = 0xFFFFFF00,
	       ipMask               = 0x00000000,
	       ipAddr               = 0x00000000,
	       hostIpAddress     = 0x00000000;

	u16_t  len                  = 0x0000,
	       method                = 0x0000,
	       requestMode           = 0x0000,
	       vlanp                 = 0x0000,
	       vlanid                = 0x0000;

	char   authTemp[33],
	       authname[30],
	       authpassword[100],
	       authpasswordcheck[40],
	       lan0proto[10],
	       pSysName[60],
	       dhcptemp[10],
	       ipaddrtemp[16],
	       gatewaytemp[16],
	       subnetmasktemp[16],
	       opmodetemp[5],
	       ssidtemp[50],
	       wirelessmodetemp[50],
	       wirelesschannel[8],
	       authtemp[20],
	       encryptemp[10],
	       pSsid[50],
	       vlantemp[10],
	       vlanptemp[10],
	       vlanidtemp[10],
	       mac_src[6],
	       firmware_version[30],
	       setipcmd[100],
	       authcmd[100],
	       protocol[2] = {0x08, 0x00},  //IP type
	       protocol1[2] = {0x81, 0x00}, //802.1Q vlan
	       bridgetemp[10],//bridge mode  0x01:AP 0x02:router
	       hostIpAddressTemp[16],
	       domainname[255],
	       hostname[255],
	       ModeTmpBuf[32],
	       dev_name[32],
		   http_type[10];  //HTTP or HTTPS

	//***********************
	char sntemp[50];
	//happiness add
	char *value;
	char *mask;
	int idx;

	struct ip *ipstr = &pReplyDataHeader->ipLayer;
	struct udphdr *udpstr = &pReplyDataHeader->udpHeader;
	struct vlanhdr vlanheader;
	struct ethhdr *ethhead; 
	struct ifreq req;
	struct sockaddr_ll rawdest;
	struct sockaddr_in in;
	memset(&rawdest, 0, sizeof(rawdest));


	flags = (u8_t)netbuf_fwd_read_u8(requestBuffer);
	//TODO: Verify that packet is a request.

	//Verify that the class matches.
	if ((u32_t)netbuf_fwd_read_u32(requestBuffer) != UDP_CLASS) {
		printf("not udp packet....return\n");
		return;
	}

	//get the firmware_version
	if(firmwareVersionGet(firmware_version) == -1){
		printf("can't get version...return\n");
		return;
	}

	// Reply Packet Data Header. 
	method = (u16_t)netbuf_fwd_read_u16(requestBuffer);
	pSDB = (char*)pReplyDataHeader->SendData;
	test = pSDB; 

	netbuf_fwd_write_u16(UDP_METHOD_GET_IP, &pSDB); // 2
	netbuf_fwd_write_u32(para->clientAddr, &pSDB);    // 4
	netbuf_fwd_write_u16(para->clientPort, &pSDB);    // 2
	netbuf_fwd_write_u16(UDP_METHOD_DISCOVER, &pSDB); // 2
	sysLanEnetAddrGet(pSDB);                          // 6
	pSDB += 6;

	netbuf_fwd_write_u16(sequence, &pSDB);     // 2 the same as the sequence of the received packet
	netbuf_fwd_write_u16(LOCATOR_UDP_TYPE, &pSDB);// 2 UDP_TYPE
	netbuf_fwd_write_u8(0x00, &pSDB);          // 2 Flag
	netbuf_fwd_write_u32(UDP_CLASS, &pSDB);//4
	para->sendPacketLength = 25; //the length = 2 + 4 + 2 + 2 + 6 + 2 + 2 + 1 + 4 = 25

	/* according to the different method, the device put into the corresponding operations */
	switch (method)
	{
		/********************* discovery response, send the device type, device name, firmware version back ***********************/
		case UDP_METHOD_DISCOVER:
			print_debug(" case UDP_METHOD_DISCOVER\n");
			netbuf_fwd_write_u16(method, &pSDB);
			para->sendPacketLength += 2;

			// Device Type
			netbuf_fwd_write_u8(UDP_CODE_DEVICE_TYPE, &pSDB);
			ezplib_get_attr_val("system_mode", 0, "name", opmodetemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (strncmp(opmodetemp, "normal", 6) == 0)
			{
				len = 6;           
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, "Router");
			}	
			else if (strncmp(opmodetemp, "ap", 2) == 0)
			{
				len = 12;           
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, DEVICE_TYPE);
			}       
			else if((strncmp(opmodetemp, "wisp0", 5) == 0) || (strncmp(opmodetemp, "wisp1", 5) == 0))
			{
				len = 4;          
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, "WISP");
			}
			else if((strncmp(opmodetemp, "sta0", 4) == 0) || (strncmp(opmodetemp, "sta1", 4) == 0))
			{
				len = 15;          
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, "Wireless Client");
			}			
			else if(strncmp(opmodetemp, "ur", 2) == 0)
			{
				len = 15;             
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, "Wireless Bridge");
			}
			else
			{
				len = 6;             
				netbuf_fwd_write_u8(len, &pSDB);
				sprintf(pSDB, "Others");
			}

			pSDB += len;
			para->sendPacketLength += (len+2);

			// Device Name
			memset(pSysName, 0, sizeof(pSysName));
			netbuf_fwd_write_u8(UDP_CODE_DEVICE_NAME, &pSDB);	
			//config_get(pSysName, 20, "system.@system[0].hostname");

			value = nvram_safe_get("hostname");
			len = strlen(value);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, value);
			pSDB += len;
			para->sendPacketLength += (len+2);

			// Firmware Version
			netbuf_fwd_write_u8(UDP_CODE_SOFTWARE_VER, &pSDB);
			len = strlen(firmware_version);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB,firmware_version);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//HTTP TYPE
			memset(http_type, 0x00, sizeof(http_type));
			netbuf_fwd_write_u8(UDP_CODE_HTTP_TYPE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			strcpy(http_type, nvram_safe_get("https_enable"));
			if (!strcmp(http_type, "1"))
				https_enable = HTTPS_ENABLE;
			else
				https_enable = HTTPS_DISABLE;
			netbuf_fwd_write_u8(https_enable, &pSDB);
			para->sendPacketLength += 3;

			sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
			break;

			//----------------------------------------------------------
			/*********************get ip mask gateway and dhcpable***********************/


		case UDP_METHOD_GET_IP:
			print_debug("UDP_METHOD_GET_IP\n");

			char ipbuffer[30];
			value = nvram_safe_get("lan0_ifname");
			getifaddr(value,ipbuffer, sizeof(ipbuffer) );
			// print_debug("iplength = %d\n", iplength);
			netbuf_fwd_write_u16(method, &pSDB);
			para->sendPacketLength += 2;
			//BRIDGE 0x01:AP  0x02 Router

			memset(bridgetemp,0,sizeof(bridgetemp));
			//config_get(bridgetemp,10,"device.mode.router");
			ezplib_get_attr_val("system_mode", 0, "name", bridgetemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			char bridge;//router-1 ap-0

			if (strcmp(bridgetemp, "ur") == 0 || strcmp(bridgetemp, "ap") == 0 || strcmp(bridgetemp, "sta0") == 0){
				bridge = 0x01;
				printf("bridge ap mode\n");
			}else{
				bridge = 0x02;
				printf("bridge router mode\n");
			}
			netbuf_fwd_write_u8(UCP_CODE_BRIDGE , &pSDB);
			netbuf_fwd_write_u8(0x01 , &pSDB);
			netbuf_fwd_write_u8(bridge, &pSDB);
			para->sendPacketLength += 3;

			// DHCP
			memset(dhcptemp, 0, sizeof(dhcptemp));
			netbuf_fwd_write_u8(UDP_CODE_USE_DHCP, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			//config_get(dhcptemp, 10, "network.lan.proto");
			//value = nvram_safe_get("lan0_proto");
			ezplib_get_attr_val("lan0_proto", 0, "curr", lan0proto, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (strcmp(lan0proto, "static") == 0)
				dhcp_enable = DHCP_DISABLE;
			else
				dhcp_enable = DHCP_ENABLE;
			netbuf_fwd_write_u8(dhcp_enable, &pSDB);
			para->sendPacketLength += 3;

			// Gateway
			memset(gatewaytemp, 0, sizeof(gatewaytemp));
			netbuf_fwd_write_u8(UDP_CODE_GATEWAY_ADDR, &pSDB);
			netbuf_fwd_write_u8(4, &pSDB);
			//config_get(gatewaytemp, 16, "network.lan.gateway");
			value = nvram_safe_get("lan0_gateway");
			if(strcmp(value, "") != 0)
			{
				gateWay = htonl(inet_addr(value));
			}
			else
			{
				strcpy(gatewaytemp, GATEWAY_EMPTY);
				gateWay = htonl(inet_addr(gatewaytemp));
			}

			print_debug(" send to pc gateWay=%d \n", gateWay);
			netbuf_fwd_write_u32(gateWay, &pSDB);
			para->sendPacketLength += 6;
			//if DHCP is disable, get the ip address and mask from the config file
			if (dhcp_enable == DHCP_DISABLE)
			{
				// IP Mask
				memset(subnetmasktemp, 0, sizeof(subnetmasktemp));
				netbuf_fwd_write_u8(UDP_CODE_SUBNET_MASK, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				//config_get(subnetmasktemp, 16, "network.lan.netmask");

				value = nvram_safe_get("lan0_mask");
				idx = atoi(value);
				value = prefix2mask(idx);
				print_debug("lan0_mask %s\n",value);
				if (strcmp(value, "") != 0)
				{
					ipMask = htonl(inet_addr(value));
				}
				else
				{
					strcpy(subnetmasktemp, NETMASK_EMPTY);
					ipMask = htonl(inet_addr(subnetmasktemp));
				}

				ipMask = htonl(inet_addr(value));
				print_debug(" send to pc ipMask=%d \n", ipMask);	
				netbuf_fwd_write_u32(ipMask, &pSDB);
				para->sendPacketLength += 6;

				// IP Address
				memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
				netbuf_fwd_write_u8(UDP_CODE_IP_ADDR, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				//----------------------------------------------
				//config_get(ipaddrtemp, 16, "network.lan.ipaddr");
				//int iplength;
				//char* ipbuffer;
				//getifaddr("br0",ipbuffer, iplength );
				//getifaddr("br0",ipaddrtemp, sizeof(ipaddrtemp) );
				value = nvram_safe_get("lan0_ipaddr");
				ipAddr = htonl(inet_addr(value));

				print_debug(" send to pc ipAddr=%d \n", ipAddr);	
				//----------------------------------------------
				netbuf_fwd_write_u32(ipAddr, &pSDB);
				para->sendPacketLength += 6;
			} 
			else  //if DHCP is enable, get the ip address and mask using the ioctl() function
			{
				memset(subnetmasktemp, 0, sizeof(subnetmasktemp));
				memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
				sysLanIpInfoGet(ipaddrtemp, subnetmasktemp);
				print_debug("ipaddrtemp = %s, subnetmasktemp = %s\n", ipaddrtemp, subnetmasktemp);
				// IP Mask
				netbuf_fwd_write_u8(UDP_CODE_SUBNET_MASK, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				ipMask = htonl(inet_addr(subnetmasktemp));
				netbuf_fwd_write_u32(ipMask, &pSDB);
				para->sendPacketLength += 6;

				// IP Address
				netbuf_fwd_write_u8(UDP_CODE_IP_ADDR, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				ipAddr = htonl(inet_addr(ipaddrtemp));
				netbuf_fwd_write_u32(ipAddr, &pSDB);
				para->sendPacketLength += 6;
			}

			// END	
			netbuf_fwd_write_u8(UDP_CODE_END, &pSDB);
			para->sendPacketLength += 1;

			sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
			break;

			//----------------------------------------------------------
			/*********************get information***********************/
		case UDP_METHOD_GET_INFO:

			print_debug("UDP_METHOD_GET_INFO\n");
			netbuf_fwd_write_u16(method, &pSDB);
			para->sendPacketLength += 2;

			// DHCP
			memset(dhcptemp, 0, sizeof(dhcptemp));
			netbuf_fwd_write_u8(UDP_CODE_USE_DHCP, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			//config_get(dhcptemp, 10, "network.lan.proto");
			//value = nvram_safe_get("lan0_proto");
			ezplib_get_attr_val("lan0_proto", 0, "curr", lan0proto, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (strcmp(lan0proto, "static") == 0)
				dhcp_enable = DHCP_DISABLE;
			else
				dhcp_enable = DHCP_ENABLE;
			netbuf_fwd_write_u8(dhcp_enable, &pSDB);
			para->sendPacketLength += 3;

			// Gateway
			memset(gatewaytemp, 0, sizeof(gatewaytemp));
			netbuf_fwd_write_u8(UDP_CODE_GATEWAY_ADDR, &pSDB);
			netbuf_fwd_write_u8(4, &pSDB);
			//config_get(gatewaytemp, 16, "network.lan.gateway");
			value = nvram_safe_get("lan0_gateway");
			if(strcmp(value, "") != 0)
			{
				gateWay = htonl(inet_addr(value));
			}
			else
			{
				strcpy(gatewaytemp, GATEWAY_EMPTY);
				gateWay = htonl(inet_addr(gatewaytemp));
			}
			print_debug("gateWay %x\n",gateWay);
			netbuf_fwd_write_u32(gateWay, &pSDB);
			para->sendPacketLength += 6;

			if (dhcp_enable == DHCP_DISABLE)//if DHCP is disable, get the ip address and mask from the config file
			{
				// IP Mask
				memset(subnetmasktemp, 0, sizeof(subnetmasktemp));
				netbuf_fwd_write_u8(UDP_CODE_SUBNET_MASK, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				//config_get(subnetmasktemp, 16, "network.lan.netmask");
				int idx;

				value = nvram_safe_get("lan0_mask");
				idx = atoi(value);
				value = prefix2mask(idx);
				if (strcmp(value, "") != 0)
				{
					ipMask = htonl(inet_addr(value));
				}
				else
				{
					strcpy(subnetmasktemp, NETMASK_EMPTY);
					ipMask = htonl(inet_addr(subnetmasktemp));
				}
				print_debug("ipMask %x\n", ipMask);
				netbuf_fwd_write_u32(ipMask, &pSDB);
				para->sendPacketLength += 6;

				// IP Address
				memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
				netbuf_fwd_write_u8(UDP_CODE_IP_ADDR, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				//config_get(ipaddrtemp, 16, "network.lan.ipaddr");
				value = nvram_safe_get("lan0_ipaddr");
				ipAddr = htonl(inet_addr(value));
				netbuf_fwd_write_u32(ipAddr, &pSDB);
				para->sendPacketLength += 6;
			} 
			else           //if DHCP is enable, get the ip address and mask using the ioctl() function
			{
				memset(subnetmasktemp, 0, sizeof(subnetmasktemp));
				memset(ipaddrtemp, 0, sizeof(ipaddrtemp));
				sysLanIpInfoGet(ipaddrtemp, subnetmasktemp);

				// IP Mask
				netbuf_fwd_write_u8(UDP_CODE_SUBNET_MASK, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				ipMask = htonl(inet_addr(subnetmasktemp));
				netbuf_fwd_write_u32(ipMask, &pSDB);
				para->sendPacketLength += 6;

				// IP Address
				netbuf_fwd_write_u8(UDP_CODE_IP_ADDR, &pSDB);
				netbuf_fwd_write_u8(4, &pSDB);
				ipAddr = htonl(inet_addr(ipaddrtemp));
				netbuf_fwd_write_u32(ipAddr, &pSDB);
				para->sendPacketLength += 6;
			}

			//Operation Mode
			memset(opmodetemp, 0, sizeof(opmodetemp));
			netbuf_fwd_write_u8(UDP_CODE_OPMODE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			//config_get(opmodetemp, 5, "wireless.wifi0.operation_mode");
			//config_get(opmodetemp, 5, "wireless.ath0.mode");
			ezplib_get_attr_val("system_mode", 0, "name", opmodetemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (strncmp(opmodetemp, "normal", 6) == 0)
				iOpMode = OPERATION_MODE_NORMAL;                // Router			
			else if(strncmp(opmodetemp, "ap", 2) == 0)
				iOpMode = OPERATION_MODE_AP;                // Access Point
			else if(strncmp(opmodetemp, "sta0", 4) == 0)
				iOpMode = OPERATION_MODE_STA0;                // Wireless Client
			else if(strncmp(opmodetemp, "ur", 2) == 0)
				iOpMode = OPERATION_MODE_UR;               // Wireless Bridge
			else if(strncmp(opmodetemp, "wisp0", 5) == 0)
				iOpMode = OPERATION_MODE_WISP0;               // WISP0
			else if(strncmp(opmodetemp, "sta1", 4) == 0)
				iOpMode = OPERATION_MODE_STA1;                // Wireless Client
			else if(strncmp(opmodetemp, "wisp1", 5) == 0)
				iOpMode = OPERATION_MODE_WISP1;               // WISP0
			else if(strncmp(opmodetemp, "wdssta0", 7) == 0)
				iOpMode = OPERATION_MODE_STA1;                // Wireless Client
			else if(strncmp(opmodetemp, "wdssta1", 7) == 0)
				iOpMode = OPERATION_MODE_WISP1;               // WISP1			

			netbuf_fwd_write_u8(iOpMode, &pSDB);
			para->sendPacketLength += 3;

			//SSID
			memset(pSsid, 0, sizeof(pSsid));
			netbuf_fwd_write_u8(UDP_CODE_SSID, &pSDB);

			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP1 || iOpMode == OPERATION_MODE_STA1) {
				ret = get_ssid(RADIO_2G, WLAN_MODE_AP, 0, pSsid);
				if (T_FAILURE == ret)
				{
					strcpy(pSsid, "");
				}
			}
			else if(iOpMode == OPERATION_MODE_STA0 || iOpMode == OPERATION_MODE_WISP0) {
				ret = get_sta_assoc_ssid(RADIO_2G, pSsid);
				if (T_FAILURE == ret)
				{
					strcpy(pSsid, "");
				}
			}			        
			len = strlen(pSsid);
			netbuf_fwd_write_u8(len, &pSDB);
			strcpy(pSDB, pSsid);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//Channel
			Channel_t channel_get;
			memset(&channel_get, 0, sizeof(Channel_t));
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP1 || iOpMode == OPERATION_MODE_STA1) {
				ret = get_current_channel(0, &channel_get);
				if (T_FAILURE == ret) {
					channel_get.chan_number = 0;
				}
			}
			else if(iOpMode == OPERATION_MODE_STA0 || iOpMode == OPERATION_MODE_WISP0) {
				channel_get.chan_number = 200;
			}	
			netbuf_fwd_write_u8(UDP_CODE_CHANNEL, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			netbuf_fwd_write_u8(channel_get.chan_number, &pSDB);
			para->sendPacketLength += 3;

			// Wireless Mode
			iWirelessMode = 9;
			memset(wirelessmodetemp, 0, sizeof(wirelessmodetemp));
			netbuf_fwd_write_u8(UDP_CODE_WIRELESS_MODE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			getWirelessMode(RADIO_2G, &iWirelessMode);
			netbuf_fwd_write_u8(iWirelessMode, &pSDB);
			para->sendPacketLength += 3;

			// Authentication
			memset(authtemp, 0, sizeof(authtemp));
			netbuf_fwd_write_u8(UDP_CODE_SEC_METHOD, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP1 || iOpMode == OPERATION_MODE_STA1)
			{
				ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", authtemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				print_debug("secmode= %s \n",authtemp);
				if (strcmp(authtemp, "psk") == 0)  // WPA/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK;
				}
				else if (strcmp(authtemp, "psk2") == 0)  // WPA2/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK2;
				}
				else if (strcmp(authtemp, "psk2mixed") == 0)  // WPA2-MIX/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK2MIX;
				}	
				else if (strcmp(authtemp, "wpa") == 0) // WPA/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA;
				}
				else if (strcmp(authtemp, "wpa2") == 0)  // WPA2/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA2;
				}
				else if (strcmp(authtemp, "wpa2mixed") == 0)  // WPA2-MIX/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA2MIX;
				}	
				else if (strcmp(authtemp, "wep") == 0)		//WEP
				{
					iAuthentication = LOCATOR_AUTHTYPE_WEP;
				}
				else if ((strcmp(authtemp, "none") == 0) || (strcmp(authtemp, "disabled") == 0))  //no security
				{
					iAuthentication = LOCATOR_AUTHTYPE_NO_SECURITY;
				}
			}
			else if(iOpMode == OPERATION_MODE_STA0 || iOpMode == OPERATION_MODE_WISP0) 
			{
				AP_INFO ap_info_2g;
				memset(&ap_info_2g, 0, sizeof(AP_INFO));
	
				ret = get_sta_assoc_ap_info(RADIO_2G, &ap_info_2g);
				if (ret != 0) {
					ap_info_2g.auth_mode = 0;
				}
				
				switch (ap_info_2g.auth_mode)
				{
					case 1:
						iAuthentication = LOCATOR_AUTHTYPE_PSK;
						break;
					case 2:
						iAuthentication = LOCATOR_AUTHTYPE_PSK2;
						break;
					case 3:
						iAuthentication = LOCATOR_AUTHTYPE_PSK2MIX;
						break;
					case 4:
						iAuthentication = LOCATOR_AUTHTYPE_WPA;
						break;
					case 5:
						iAuthentication = LOCATOR_AUTHTYPE_WPA2;
						break;
					case 6:
						iAuthentication = LOCATOR_AUTHTYPE_WPA2MIX;
						break;
					case 7:
						iAuthentication = LOCATOR_AUTHTYPE_WEP;
						break;
					default:
						iAuthentication = LOCATOR_AUTHTYPE_NO_SECURITY;
						break;
				}
			}
			print_debug("iAuthentication= %d \n",iAuthentication);
			netbuf_fwd_write_u8(iAuthentication, &pSDB);
			para->sendPacketLength += 3;

			// Encryption ONOFF
			netbuf_fwd_write_u8(UDP_CODE_ENCRYPT_ONOFF, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			if (iAuthentication == LOCATOR_AUTHTYPE_NO_SECURITY)
			{
				netbuf_fwd_write_u8(UDP_CODE_ENCRYPT_OFF, &pSDB);
			}
			else
			{
				netbuf_fwd_write_u8(UDP_CODE_ENCRYPT_ON, &pSDB);
			}
			para->sendPacketLength += 3;

			// Device Name in system.cfg
			memset(pSysName, 0, sizeof(pSysName));
			netbuf_fwd_write_u8(UDP_CODE_DEVICE_NAME, &pSDB);	
			//config_get(pSysName, 20, "system.@system[0].hostname");
			value = nvram_safe_get("hostname");
			len = strlen(value);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, value);
			pSDB += len;
			para->sendPacketLength += (len+2);

			// Software Version
			netbuf_fwd_write_u8(UDP_CODE_SOFTWARE_VER, &pSDB);
			len = strlen(firmware_version);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB,firmware_version);
			pSDB += len;
			para->sendPacketLength += (len+2);

			// Wireless Lan Mac Address
			memset(MacAddr, 0, sizeof(MacAddr));
			memset(dev_name, 0, sizeof(dev_name));
			memset(MacAddrTemp, 0, sizeof(MacAddrTemp));
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP1 || iOpMode == OPERATION_MODE_STA1) {
				ezplib_get_attr_val("wl_ap_device_rule", 0, "ssid0_device", dev_name, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			}
			else if(iOpMode == OPERATION_MODE_STA0 || iOpMode == OPERATION_MODE_WISP0) {
				ezplib_get_attr_val("wl_sta_device_rule", 0, "sta_device", dev_name, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			}
			netbuf_fwd_write_u8(UDP_CODE_WLAN_MACADDR, &pSDB);
			sysAddrGet(dev_name, MacAddrTemp);
			sprintf(MacAddr, "%02X:%02X:%02X:%02X:%02X:%02X",
					MacAddrTemp[0], MacAddrTemp[1], MacAddrTemp[2], MacAddrTemp[3],	MacAddrTemp[4], MacAddrTemp[5]);
			len = strlen(MacAddr);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, MacAddr, len);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//associated
			netbuf_fwd_write_u8(UCP_CODE_ASSOCIATED, &pSDB);
			//0->Disassociated, 1->Associated, 2->Unkown, 3-> --
			getAPConnectStatus(RADIO_2G, &associated);
			netbuf_fwd_write_u8(1, &pSDB);
			netbuf_fwd_write_u8(associated, &pSDB);
			para->sendPacketLength += (3);


			//BSSID
			memset(MacAddr, 0, sizeof(MacAddrTemp));
			netbuf_fwd_write_u8(UCP_CODE_BSSID, &pSDB);
			getAPConnectBSSID(RADIO_2G, MacAddr);
			len=strlen(MacAddr);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, MacAddr, len);
			pSDB += len;
			para->sendPacketLength += (len+2);
			
			//RSSI
			memset(rssi, 0, sizeof(rssi)); 
			netbuf_fwd_write_u8(UCP_CODE_RSSI, &pSDB);
			getWIFIRSSI(RADIO_2G, rssi);
			len=strlen(rssi);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, rssi, len);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//5G Wireless Mode
			iWirelessMode = 8;
			netbuf_fwd_write_u8(UDP_CODE_5GWIRELESS_MODE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			getWirelessMode(RADIO_5G, &iWirelessMode);
			netbuf_fwd_write_u8(iWirelessMode, &pSDB);
			para->sendPacketLength += 3;

			//5G MAC Address
			memset(MacAddr, 0, sizeof(MacAddr));
			memset(dev_name, 0, sizeof(dev_name));
			memset(MacAddrTemp, 0, sizeof(MacAddrTemp));
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP0 || iOpMode == OPERATION_MODE_STA0) {
				ezplib_get_attr_val("wl_ap_device_rule", 1, "ssid0_device", dev_name, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			}
			else if(iOpMode == OPERATION_MODE_STA1 || iOpMode == OPERATION_MODE_WISP1) {
				ezplib_get_attr_val("wl_sta_device_rule", 1, "sta_device", dev_name, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			}
			netbuf_fwd_write_u8(UDP_CODE_5GWLAN_MACADDR, &pSDB);
			sysAddrGet(dev_name, MacAddrTemp);
			sprintf(MacAddr, "%02X:%02X:%02X:%02X:%02X:%02X",
					MacAddrTemp[0], MacAddrTemp[1], MacAddrTemp[2], MacAddrTemp[3],	MacAddrTemp[4], MacAddrTemp[5]);
			len = strlen(MacAddr);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, MacAddr, len);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//5G SSID
			memset(pSsid, 0, sizeof(pSsid));
			netbuf_fwd_write_u8(UDP_CODE_5GSSID, &pSDB);

			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP0 || iOpMode == OPERATION_MODE_STA0) {
				ret = get_ssid(RADIO_5G, WLAN_MODE_AP, 0, pSsid);
				if (T_FAILURE == ret)
				{
					strcpy(pSsid, "");
				}
			}
			else if(iOpMode == OPERATION_MODE_STA1 || iOpMode == OPERATION_MODE_WISP1) {
				ret = get_sta_assoc_ssid(RADIO_5G, pSsid);
				if (T_FAILURE == ret)
				{
					strcpy(pSsid, "");
				}
			}			        
			len = strlen(pSsid);
			netbuf_fwd_write_u8(len, &pSDB);
			strcpy(pSDB, pSsid);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//5G RSSI
			memset(rssi, 0, sizeof(rssi)); 
			netbuf_fwd_write_u8(UDP_CODE_5GRSSI, &pSDB);
			getWIFIRSSI(RADIO_5G, rssi);
			len=strlen(rssi);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, rssi, len);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//5G BSSID
			memset(MacAddr, 0, sizeof(MacAddrTemp));
			netbuf_fwd_write_u8(UDP_CODE_5GBSSID, &pSDB);
			getAPConnectBSSID(RADIO_5G, MacAddr);
			len=strlen(MacAddr);
			netbuf_fwd_write_u8(len, &pSDB);
			sprintf(pSDB, MacAddr, len);
			pSDB += len;
			para->sendPacketLength += (len+2);

			//5G associated
			netbuf_fwd_write_u8(UDP_CODE_5GASSOCIATED, &pSDB);
			getAPConnectStatus(RADIO_5G, &associated);
			netbuf_fwd_write_u8(1, &pSDB);
			netbuf_fwd_write_u8(associated, &pSDB);
			para->sendPacketLength += (3);

			//5G Channel
			Channel_t channel_get5g;
			memset(&channel_get5g, 0, sizeof(Channel_t));
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP0 || iOpMode == OPERATION_MODE_STA0) {
				ret = get_current_channel(RADIO_5G, &channel_get5g);
				if (T_FAILURE == ret) {
					channel_get5g.chan_number = 0;
				}
			}
			else if(iOpMode == OPERATION_MODE_STA1 || iOpMode == OPERATION_MODE_WISP1) {
				channel_get5g.chan_number = 200;
			}	
			netbuf_fwd_write_u8(UDP_CODE_5GCHANNEL, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			netbuf_fwd_write_u8(channel_get5g.chan_number, &pSDB);
			para->sendPacketLength += 3;

			//5G Authentication
			memset(authtemp, 0, sizeof(authtemp));
			iAuthentication == LOCATOR_AUTHTYPE_NO_SECURITY;
			netbuf_fwd_write_u8(UDP_CODE_5GSEC_METHOD, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			if(iOpMode == OPERATION_MODE_AP || iOpMode == OPERATION_MODE_NORMAL
				|| iOpMode == OPERATION_MODE_WISP0 || iOpMode == OPERATION_MODE_STA0)
			{
				ezplib_get_attr_val("wl1_sec_rule", 0, "secmode", authtemp, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				print_debug("secmode= %s \n",authtemp);
				if (strcmp(authtemp, "psk") == 0)  // WPA/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK;
				}
				else if (strcmp(authtemp, "psk2") == 0)  // WPA2/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK2;
				}
				else if (strcmp(authtemp, "psk2mixed") == 0)  // WPA2-MIX/PSK
				{
					iAuthentication = LOCATOR_AUTHTYPE_PSK2MIX;
				}	
				else if (strcmp(authtemp, "wpa") == 0) // WPA/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA;
				}
				else if (strcmp(authtemp, "wpa2") == 0)  // WPA2/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA2;
				}
				else if (strcmp(authtemp, "wpa2mixed") == 0)  // WPA2-MIX/802.1x
				{
					iAuthentication = LOCATOR_AUTHTYPE_WPA2MIX;
				}	
				else if (strcmp(authtemp, "wep") == 0)		//WEP
				{
					iAuthentication = LOCATOR_AUTHTYPE_WEP;
				}
				else if ((strcmp(authtemp, "none") == 0) || (strcmp(authtemp, "disabled") == 0))  //no security
				{
					iAuthentication = LOCATOR_AUTHTYPE_NO_SECURITY;
				}
			}
			else if(iOpMode == OPERATION_MODE_STA1 || iOpMode == OPERATION_MODE_WISP1) 
			{
				AP_INFO ap_info_5g;
				memset(&ap_info_5g, 0, sizeof(AP_INFO));
	
				ret = get_sta_assoc_ap_info(RADIO_5G, &ap_info_5g);
				if (ret != 0) {
					ap_info_5g.auth_mode = 0;
				}
								
				switch (ap_info_5g.auth_mode)
				{
					case 1:
						iAuthentication = LOCATOR_AUTHTYPE_PSK;
						break;
					case 2:
						iAuthentication = LOCATOR_AUTHTYPE_PSK2;
						break;
					case 3:
						iAuthentication = LOCATOR_AUTHTYPE_PSK2MIX;
						break;
					case 4:
						iAuthentication = LOCATOR_AUTHTYPE_WPA;
						break;
					case 5:
						iAuthentication = LOCATOR_AUTHTYPE_WPA2;
						break;
					case 6:
						iAuthentication = LOCATOR_AUTHTYPE_WPA2MIX;
						break;
					case 7:
						iAuthentication = LOCATOR_AUTHTYPE_WEP;
						break;
					default:
						iAuthentication = LOCATOR_AUTHTYPE_NO_SECURITY;
						break;
				}
			}
			print_debug("iAuthentication= %d \n",iAuthentication);
			netbuf_fwd_write_u8(iAuthentication, &pSDB);
			para->sendPacketLength += 3;

			// 5G Encryption ONOFF
			netbuf_fwd_write_u8(UDP_CODE_5GENCRYPT_ONOFF, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			if (iAuthentication == LOCATOR_AUTHTYPE_NO_SECURITY)
			{
				netbuf_fwd_write_u8(UDP_CODE_ENCRYPT_OFF, &pSDB);
			}
			else
			{
				netbuf_fwd_write_u8(UDP_CODE_ENCRYPT_ON, &pSDB);
			}
			para->sendPacketLength += 3;

			//2.4G Radio Status
			iRadiostatus = UDP_CODE_RADIO_OFF;
			netbuf_fwd_write_u8(UDP_CODE_2GRADIO_ONOFF, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			getLocaRadioStatus(RADIO_2G, &iRadiostatus);
			netbuf_fwd_write_u8(iRadiostatus, &pSDB);
			para->sendPacketLength += 3;

			//5G Radio Status
			iRadiostatus = UDP_CODE_RADIO_OFF;
			netbuf_fwd_write_u8(UDP_CODE_5GRADIO_ONOFF, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			getLocaRadioStatus(RADIO_5G, &iRadiostatus);
			netbuf_fwd_write_u8(iRadiostatus, &pSDB);
			para->sendPacketLength += 3;

			//2.4G WLAN OP Mode
			iWlanOPMode = UDP_CODE_WLAN_AP;
			memset(ModeTmpBuf, 0, sizeof(ModeTmpBuf));
			netbuf_fwd_write_u8(UDP_CODE_WLAN_OPMODE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(ModeTmpBuf, "client"))
			{
				iWlanOPMode = UDP_CODE_WLAN_CLIENT;
			}
			else
			{
				iWlanOPMode = UDP_CODE_WLAN_AP;
			}
			netbuf_fwd_write_u8(iWlanOPMode, &pSDB);
			para->sendPacketLength += 3;

			//5G WLAN OP Mode
			iWlanOPMode = UDP_CODE_WLAN_AP;
			memset(ModeTmpBuf, 0, sizeof(ModeTmpBuf));
			netbuf_fwd_write_u8(UDP_CODE_5GWLAN_OPMODE, &pSDB);
			netbuf_fwd_write_u8(1, &pSDB);
			ezplib_get_attr_val("wl1_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(ModeTmpBuf, "client"))
			{
				iWlanOPMode = UDP_CODE_WLAN_CLIENT;
			}
			else
			{
				iWlanOPMode = UDP_CODE_WLAN_AP;
			}
			netbuf_fwd_write_u8(iWlanOPMode, &pSDB);
			para->sendPacketLength += 3;
			
			//END
			netbuf_fwd_write_u8(UDP_CODE_END, &pSDB);
			para->sendPacketLength += 1;

			sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
			break;
			//----------------------------------------------------------
			/*********************get host name,ip address, domain name *********************************/
		case UCP_METHOD_GET_HOST:{
						 print_debug("UCP_METHOD_GET_HOST\n");
						 netbuf_fwd_write_u16(method, &pSDB);
						 para->sendPacketLength += 2;
						 //host name
						 netbuf_fwd_write_u8(UCP_CODE_HOST_NAME, &pSDB);

						 //config_get(hostname, 255, "pedestal_client.server.hostname");
						 value = nvram_safe_get("hostname");
						 print_debug("hostname %s\n",value);
						 int len = strlen(value);
						 netbuf_fwd_write_u8(len, &pSDB);
						 sprintf(pSDB,value);
						 pSDB += len;
						 para->sendPacketLength += (len+2);

						 //ip address 
						 netbuf_fwd_write_u8(UDP_CODE_IP_ADDR, &pSDB);
						 netbuf_fwd_write_u8(4, &pSDB);
						 memset(hostIpAddressTemp, 0, sizeof(hostIpAddressTemp));
						 //config_get(hostIpAddressTemp, 16, "pedestal_client.server.address");
						 value = nvram_safe_get("lan0_ipaddr");
						 print_debug("lan0_ipaddr %s\n",value);

						 hostIpAddress =  htonl(inet_addr(value));
						 print_debug("hostIpAddress %d\n",hostIpAddress);
						 netbuf_fwd_write_u32(hostIpAddress, &pSDB);
						 para->sendPacketLength += 6;
						 //domain name
						 netbuf_fwd_write_u8(UCP_CODE_DOMAIN_NAME, &pSDB);

						 //config_get(domainname, 255, "pedestal_client.server.domainsname");
						 ezplib_get_attr_val("lan_dhcps_rule", 0, "domain", domainname, TMP_LEN, EZPLIB_USE_CLI);
						 len = strlen(domainname);
						 print_debug("domainname %s\n",domainname);
						 netbuf_fwd_write_u8(len, &pSDB);
						 sprintf(pSDB,domainname);
						 pSDB += len;
						 para->sendPacketLength += (len+2);

						 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
						 break;
					 }

					 /*********************set host name,ip address, domain name *********************************/
		case UCP_METHOD_SET_HOST:{
						 print_debug("UCP_METHOD_SET_HOST\n");
						 netbuf_fwd_write_u16(method, &pSDB);
						 para->sendPacketLength += 2;

						 u8_t **requestBufferCopy;
						 requestBufferCopy = requestBuffer;
						 u8_t* position = *requestBuffer;
						 HostInfo local_hostinfo = get_local_hostinfo();
						 memset(&recv_hostinfo, 0, sizeof(recv_hostinfo));
						 print_debug("recv info:\n");
						 recv_hostinfo = get_recv_hostinfo(requestBufferCopy);
						 *requestBuffer = position;

						 if(check_HostInfo(local_hostinfo) > 0){
							 printf("will update\n");

						 }else{
							 printf("no need update\n");
							 netbuf_fwd_write_u8(UDP_CODE_SET_NONEED, &pSDB);
							 para->sendPacketLength += 1;
							 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

							 return;
						 }
						 for ( code = netbuf_fwd_read_u8(requestBuffer);
								 code != UDP_CODE_END;
								 code = netbuf_fwd_read_u8(requestBuffer) )
						 {
							 len = netbuf_fwd_read_u8(requestBuffer);

							 printf("code %d, len %d\n",code, len);
							 switch(code)
							 {
								 case UDP_CODE_IP_ADDR:{
											       memset(&in, 0, sizeof(in));
											       hostIpAddress = netbuf_fwd_read_u32(requestBuffer);
											       printf("hostIpAddress %d\n",hostIpAddress);
											       if(hostIpAddress == 0x00000000){
												       nvram_safe_set("lan0_ipaddr", "192.168.1.2");	
											       }else{
												       in.sin_addr.s_addr = htonl(hostIpAddress);
												       print_debug("in.sinaddr: %s\n", inet_ntoa(in.sin_addr));
												       nvram_safe_set("lan0_ipaddr", inet_ntoa(in.sin_addr));
												       //sprintf(setipcmd, "uci set pedestal_client.server.address=%s", inet_ntoa(in.sin_addr));

											       }
											       //printf("setipcmd =  %s\n", setipcmd);
											       nvram_commit();
											       break;
										       }
								 case UCP_CODE_DOMAIN_NAME:{
												   memset(&in, 0, sizeof(in));

												   UdpHost udpHost;

												   net_fwd_read_host(requestBuffer, &udpHost, 0, len);
												   /*char uciset[100] = "uci set pedestal_client.server.domainsname=";
												     char udpHost_copy[len +strlen(uciset)];
												     udpHost_copy[len +strlen(uciset)] = '\0';
												     int i = 0;
												     for( i = 0; i < strlen(uciset); i++)
												     {
												     udpHost_copy[i] = uciset[i];
												     }
												     int i_backup = i;
												     for(i=0; i < len; i++)
												     {
												     udpHost_copy[i+i_backup] = *(udpHost.domainName+i);
												     }*/
												   //udpHost.domainName[len] = '\0';
												   char m_domainName[len];
												   memset(m_domainName,0,len);
												   for(i=0; i <len; i++)
												   {
													   m_domainName[i] = *(udpHost.domainName+i);
												   }
												   m_domainName[len] = '\0';
												   printf("udpHost.domainName =  %s\n", m_domainName);
												   ezplib_replace_attr("lan_dhcps_rule", 0, "domain", m_domainName);
												   nvram_commit();

												   break;
											   }
								 case UCP_CODE_HOST_NAME:{
												 memset(&in, 0, sizeof(in));

												 UdpHost udpHost;
												 net_fwd_read_host(requestBuffer, &udpHost, len, 0);
												 /*char uciset[100] = "uci set pedestal_client.server.hostname=";
												   char udpHost_copy[len + strlen(uciset)];
												   udpHost_copy[len + strlen(uciset)] = '\0';
												   int i = 0;
												   for( i = 0; i < strlen(uciset); i++)
												   {
												   udpHost_copy[i] =  uciset[i];
												   }
												   int i_backup = i;
												   for(i=0; i <len; i++)
												   {
												   udpHost_copy[i+i_backup] = *(udpHost.hostName +i);
												   }*/
												 //udpHost.hostName[len] = '\0';
												 char m_hostName[len];
												 memset(m_hostName,0,len);
												 for(i=0; i <len; i++)
												 {
													 m_hostName[i] = *(udpHost.hostName +i);
												 }
												 m_hostName[len] = '\0';
												 printf("udpHost.hostName =  %s\n", m_hostName);
												 nvram_safe_set("hostname", m_hostName);
												 nvram_commit();
												 break;
											 }

								 default:
											 printf("parse error ......");
							 }
						 }
						 local_hostinfo = get_local_hostinfo();
						 if(check_HostInfo(local_hostinfo)> 0){
							 printf("update fail\n");
							 printf("check_IpInfo > 0\n");
							 netbuf_fwd_write_u8(UDP_CODE_SET_FAIL, &pSDB);
							 para->sendPacketLength += 1;
							 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

							 return;
						 }else{
							 printf("update success\n");

							 netbuf_fwd_write_u8(UDP_CODE_SET_SUCCESS, &pSDB);
							 para->sendPacketLength += 1;
							 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
						 }

						 break;
					 }

					 /********************* default factory configuration, reset and reboot the device after sending the success buffer back ***********************/

		case UDP_METHOD_RESET:
					 print_debug("UDP_METHOD_RESET\n");
					 netbuf_fwd_write_u16(method, &pSDB);
					 para->sendPacketLength += 2;
					 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
					 break;

					 //----------------------------------------------------------
					 /********************* Reboot device after sending the success buffer back ***********************/

		case UDP_METHOD_REBOOT:
					 print_debug("UDP_METHOD_REBOOT\n");
					 netbuf_fwd_write_u16(method, &pSDB);
					 para->sendPacketLength += 2;
					 sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
					 break;

					 //----------------------------------------------------------
					 /********************* set ip mask gateway and dhcpable, apply the setting after sending the success buffer back ***********************/

		case UDP_METHOD_SET_IP:{
					       print_debug("UDP_METHOD_SET_IP\n");
					       netbuf_fwd_write_u16(method, &pSDB);
					       para->sendPacketLength += 2;

					       u8_t ** requestBufferCopy ;
					       requestBufferCopy = requestBuffer;
					       u8_t* position = *requestBuffer;
					       print_debug("get_local_ipinfo11111 \n");
					       IpInfo local_ipinfo = get_local_ipinfo();
					       print_debug("get_local_ipinfo111112222 \n");
					       memset(&recv_ipinfo, 0, sizeof(recv_ipinfo));
					       recv_ipinfo = get_recv_ipinfo(requestBufferCopy);
					       *requestBuffer = position;

					       if( check_IpInfo(local_ipinfo)>0){
						       print_debug("check_IpInfo > 0\n");

					       }else{
						       print_debug("check_IpInfo = 0\n");
						       netbuf_fwd_write_u8(UDP_CODE_SET_NONEED, &pSDB);
						       para->sendPacketLength += 1;
						       sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

						       return;
					       }
					       for ( code = netbuf_fwd_read_u8(requestBuffer);code != UDP_CODE_END;
							       code = netbuf_fwd_read_u8(requestBuffer) )
					       {
						       len = netbuf_fwd_read_u8(requestBuffer);

						       switch(code)
						       {
							       case UDP_CODE_USE_DHCP:
								       dhcp_enable = netbuf_fwd_read_u8(requestBuffer);
								       //recv_ipinfo.dhcp_enable = dhcp_enable;
								       if (dhcp_enable == DHCP_DISABLE){
									       //nvram_safe_set("lan0_proto", "static");//static
									       ezplib_replace_attr("lan0_proto", 0, "curr", "static");
								       }else{
									       //nvram_safe_set("lan0_proto","dhcp");    //enable DHCP
									       ezplib_replace_attr("lan0_proto", 0, "curr", "dhcp");
								       }
								       nvram_commit();
								       break;

							       case UDP_CODE_GATEWAY_ADDR:
								       memset(&in, 0, sizeof(in));
								       gateWay = netbuf_fwd_read_u32(requestBuffer);
								       //recv_ipinfo.gateway = gateWay;

								       in.sin_addr.s_addr = htonl(gateWay);
								       nvram_safe_set("lan0_gateway",inet_ntoa(in.sin_addr));
								       print_debug("gateway=%s", inet_ntoa(in.sin_addr));
								       nvram_commit();
								       break;

							       case UDP_CODE_SUBNET_MASK:
								       memset(&in, 0, sizeof(in));
								       ipMask = netbuf_fwd_read_u32(requestBuffer);
								       //  recv_ipinfo.subnet = ipMask;
								       print_debug("ipMask = %d\n", ipMask);
								       in.sin_addr.s_addr = htonl(ipMask);
								       print_debug("netmask=%s", inet_ntoa(in.sin_addr));
								       mask = mask2prefix(inet_ntoa(in.sin_addr));
								       nvram_safe_set("lan0_mask", mask);
								       nvram_commit();

								       break;


							       case UDP_CODE_IP_ADDR:
								       print_debug("UDP_CODE_IP_ADDR \n");
								       memset(&in, 0, sizeof(in));
								       ipAddr = netbuf_fwd_read_u32(requestBuffer);
								       //  recv_ipinfo.ipaddr = ipAddr;
								       in.sin_addr.s_addr = htonl(ipAddr);
								       print_debug("ipAddr = %d\n", ipAddr);
								       //sprintf(setipcmd, "uci set network.lan.ipaddr=%s", inet_ntoa(in.sin_addr));
								       print_debug("in.sinaddr: %s\n", inet_ntoa(in.sin_addr));
								       nvram_set("lan0_ipaddr", inet_ntoa(in.sin_addr));
								       nvram_commit();
								       //system(setipcmd);
								       break;

							       default:
								       printf("\nError Code Type: %d\n",code);
								       return;
						       }
					       }
					       print_debug("get_local_ipinfo2222222 \n");
					       local_ipinfo = get_local_ipinfo();
					       if((ipSetFlag = check_IpInfo(local_ipinfo))>0){
						       print_debug("check_IpInfo > 0\n");
						       netbuf_fwd_write_u8(UDP_CODE_SET_FAIL, &pSDB);
						       para->sendPacketLength += 1;
						       sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

						       return;
					       }else{
						       print_debug("check_IpInfo = 0\n");
						       netbuf_fwd_write_u8(UDP_CODE_SET_SUCCESS, &pSDB);
						       para->sendPacketLength += 1;
						       sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

					       }
					       break;
				       }


				       //----------------------------------------------------------
				       /*********************register authentication***********************/

		case UDP_METHOD_AUTH:
				       print_debug("UDP_METHOD_AUTH\n");
				       netbuf_fwd_write_u16(method, &pSDB);
				       para->sendPacketLength += 2;

				       //config_get(authname, 30, "password.@auth[0].username");
				       //config_get(authname, 30, "password.auth.username");
				       ezplib_get_attr_val("http_rule", 0, "curusername", authname, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				       //config_get(authpassword, 40, "password.@auth[0].userpasswd");
				       if(!strcmp(authname, "admin"))
				       {
					       ezplib_get_attr_val("http_rule", 0, "admpasswd", authpassword,TMP_LEN, EZPLIB_USE_CLI);
				       }
				       else
				       {
					       ezplib_get_attr_val("http_rule", 0, "passwd", authpassword,TMP_LEN, EZPLIB_USE_CLI);
				       }

				       //config_get(authpassword, 40, "password.auth.password");
				       requestMode = netbuf_fwd_read_u16(requestBuffer);  //when you getip(or default factory) after auth, \
				       //the requestmode is UDP_METHOE_GET_IP(or UDP_METHOD_RESET), and so on 
				       for (i=0; i <2; i++)
				       {
					       memset(authTemp, 0, sizeof(authTemp));
					       memset(authpasswordcheck, 0, sizeof(authpasswordcheck));
					       authType = netbuf_fwd_read_u8(requestBuffer);
					       len = netbuf_fwd_read_u8(requestBuffer);

					       print_debug("authType %x\n", authType);
					       switch (authType)  //name or password
					       {
						       case UDP_CODE_AUTH_NAME:
							       memcpy(authTemp, *requestBuffer, len);
							       *requestBuffer += len;

							       if (strcmp(authTemp, authname) != 0)
								       authErr = 1;

							       break;

						       case UDP_CODE_AUTH_PASSWORD:
							       memcpy(authTemp, *requestBuffer, len);
							       *requestBuffer += len;
							       // apCfgPasswordAuthGet(authpasswordcheck, authTemp);

							       // if (strcmp(authpasswordcheck, authpassword) != 0)
							       //     authErr = 1;
							       if(strcmp(authTemp, authpassword)!=0)
								       authErr = 1;
							       break;
					       }
					       //happiness add authErr = 0;
					       //authErr = 0;
					       if (authErr == 1)
						       break;
				       }

				       netbuf_fwd_write_u8(authErr, &pSDB);
				       para->sendPacketLength += 1;
				       netbuf_fwd_write_u16(requestMode, &pSDB);
				       para->sendPacketLength += 2;
				       sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);
				       break;

				       //----------------------------------------------------------
		default:

				       printf("\nLocator send an error method packet!!\n");
				       break;
	}
	//-------------------- fill in the UDP Header and the IP Header information ------------------------

	//sendPacketToPC(pReplyDataHeader, para, test, ipstr, udpstr);

	//put into the corresponding operator according to the method after sending the packet
	switch(method)
	{
		//apply the setting 
		case UDP_METHOD_SET_IP:
			printf("apply UDP_METHOD_SET_IP\n");
			system("/etc/init.d/network-lan restart");
			break;

			//apply the reset
		case UDP_METHOD_RESET:
			printf("apply UDP_METHOD_RESET\n");
			//system("/lib/script/config_ops.sh factory_default");
			//system("reboot");
			system("nvram factory");
			break;

			//apply the reboot
		case UDP_METHOD_REBOOT:
			printf("apply UDP_METHOD_REBOOT\n");
			system("reboot");
			break;

		case UCP_METHOD_SET_HOST:
			printf("apply UCP_METHOD_SET_HOST");
			system("/etc/init.d/dnsmasq stop");
			system("/etc/init.d/dnsmasq start");
			break;

		default:
			printf("apply default\n");
			break;
	}
}

void sendPacketToPC(locator_udp_reply_packet_header * pReplyDataHeader, UdpParams *para, u8_t* test,  struct ip *ipstr, struct udphdr *udpstr){
	int rawsock              = 0,
	    ret                  = 0,
	    vlanopen	=   0,
	    sd                   = 0;
	char  vlantemp[10],
	      mac_src[6],
	      vlanptemp[10],
	      vlanidtemp[10],
	      protocol[2] = {0x08, 0x00},  //IP type
	      protocol1[2] = {0x81, 0x00}; //802.1Q vlan
	struct ifreq req;
	struct sockaddr_ll rawdest;
	struct ethhdr *ethhead; 
	u16_t vlanp                 = 0x0000,
	      vlanid                = 0x0000;
	struct vlanhdr vlanheader;
	char *value;

	printf("sendPacketToPC..............\n");
	memset(&rawdest, 0, sizeof(rawdest));
	print_debug("before locator_UDP_and_IP_Header_Set\n");
	locator_UDP_and_IP_Header_Set(pReplyDataHeader, para, test);
	print_debug("after locator_UDP_and_IP_Header_Set\n");

	//--------------- Send the packet through RAW Socket and rebuild the pcaket ------------------------------    
	if ((rawsock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)  //create the RAW socket
		perror("create rawsock failure: ");

	setSocketOption(rawsock);

	rawdest.sll_family = AF_PACKET;            //define the struct of the sockaddr_ll
	rawdest.sll_protocol = htons(ETH_P_ALL);

	// get the sll_ifindex of the network card through the ifr_name, used to define the network card used to send packet
	sd = socket(PF_INET, SOCK_DGRAM, 0);
	value = nvram_safe_get("lan0_ifname");
	strcpy(req.ifr_name, value);
	ret = ioctl(sd, SIOCGIFINDEX, &req);
	close(sd);

	if (ret == -1)
		printf("get the index failure");

	rawdest.sll_ifindex = req.ifr_ifindex;
	rawdest.sll_pkttype = PACKET_OUTGOING;  //packet type, this is a sended packet 
	rawdest.sll_halen = 6;                  //dest MAC address length
#ifdef UNICASE   
	if(pReplyDataHeader->ipLayer.ip_src.s_addr == 0 |
			(pReplyDataHeader->ipLayer.ip_src.s_addr&(u32_t)inet_addr("255.255.255.0"))
			!= ((u32_t)(htonl(para->clientAddr))&(u32_t)inet_addr("255.255.255.0"))){
#endif		
		print_debug("broadcast mac\n");
		rawdest.sll_addr[0] = 0xFF;
		rawdest.sll_addr[1] = 0xFF;
		rawdest.sll_addr[2] = 0xFF;
		rawdest.sll_addr[3] = 0xFF;
		rawdest.sll_addr[4] = 0xFF;
		rawdest.sll_addr[5] = 0xFF;

#ifdef UNICASE   
	}else{
		print_debug("peer mac\n");
		rawdest.sll_addr[0] = para->macAddress[0];           
		rawdest.sll_addr[1] = para->macAddress[1];            
		rawdest.sll_addr[2] = para->macAddress[2];            
		rawdest.sll_addr[3] = para->macAddress[3];            
		rawdest.sll_addr[4] = para->macAddress[4];            
		rawdest.sll_addr[5] = para->macAddress[5];  
	}
#endif		


	sysLanEnetAddrGet(mac_src);            //get the source mac address

	//config_get(vlantemp, 2, "network.lan.mngvlan");
	//print_debug("network.lan.mngvlan =%s\n",vlantemp);
	//vlanopen = atoi(vlantemp);
	//print_debug("vlanopen =%d\n",vlanopen);
	//print_debug("vlanopen= %d\n",vlanopen);
	vlanopen = 0;
	if (vlanopen == 1)
	{
		u8_t SendBuffer[para->sendPacketLength + 18];  //define the send buffer
		memset(SendBuffer, 0, sizeof(SendBuffer));

		//add the Ethernet header
		ethhead = (struct ethhdr *)SendBuffer;
		memcpy(ethhead->h_dest, rawdest.sll_addr, 6);  
			memcpy(ethhead->h_source, mac_src, 6);
		memcpy(&ethhead->h_proto, protocol1, 2); 

		//add the vlan header
		//config_get(vlanptemp, 6, "network.lan.priority");
		ezplib_get_attr_val("vlan_rule", 0, "name", vlanidtemp,TMP_LEN, EZPLIB_USE_CLI);
		print_debug("network.lan.priority =%s\n",vlantemp);
		vlanp = (u16_t)atoi(vlanptemp);  
		print_debug("vlanp= %d\n",vlanp);
		//config_get(vlanidtemp, 6, "network.lan.mngvlanid");
		ezplib_get_attr_val("vlan_rule", 0, "vid", vlanidtemp,TMP_LEN, EZPLIB_USE_CLI);
		vlanid = (u16_t)atoi(vlanidtemp);

		vlanheader.vlanpcid = 0xffff;
		vlanheader.vlanpcid = ( ( (vlanheader.vlanpcid) & (vlanp) ) << 13 | ( (vlanheader.vlanpcid) & (vlanid) ) );
		memcpy(&(vlanheader.vlantype), protocol, 2); 

		memcpy(SendBuffer + 14, &vlanheader, 4);

		//copy the content into the sendbuffer 
		u8_t *ipheaderstr = SendBuffer + 18;
		u8_t *udpheaderstr = SendBuffer + 38;
		u8_t *senddatastr = SendBuffer + 46;
		memcpy(ipheaderstr, ipstr, 20);
		memcpy(udpheaderstr, udpstr, 8);
		memcpy(senddatastr, test, para->sendPacketLength -28);

		if (sendto(rawsock, SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&rawdest, sizeof(rawdest)) == -1) //send the packet
		{ 
			perror("sendto error");
			return; 
		}

		print_debug("send success 1\n");
	}
	else
	{
		u8_t SendBuffer[para->sendPacketLength + 14];  //define the send buffer
		memset(SendBuffer, 0, sizeof(SendBuffer));

		//add the Ethernet header
		ethhead = (struct ethhdr *)SendBuffer;
		memcpy(ethhead->h_dest, rawdest.sll_addr, 6);  
			memcpy(ethhead->h_source, mac_src, 6);
		memcpy(&ethhead->h_proto, protocol, 2); 

		//copy the content into the sendbuffer 
		u8_t *ipheaderstr = SendBuffer + 14;
		//u8_t *ipheaderstr = SendBuffer + 8;
		print_debug("*ipheaderstr before = %s\n",*ipheaderstr);
		u8_t *udpheaderstr = SendBuffer + 34;
		print_debug("*udpheaderstr before = %s\n",*udpheaderstr);
		u8_t *senddatastr = SendBuffer + 42;
		print_debug("*senddatastr before= %s\n",*senddatastr);	
		memcpy(ipheaderstr, ipstr, 20);
		memcpy(udpheaderstr, udpstr, 8);
		memcpy(senddatastr, test, para->sendPacketLength -28);
		myprint(20, ipheaderstr);
		myprint(8, udpheaderstr);
		myprint(para->sendPacketLength -28, senddatastr);

		print_debug("send to.............\n");

		printf("SendBuffer   = %s:sizeof(SendBuffer) = %d\n",SendBuffer,sizeof(SendBuffer));
		myprint(sizeof(SendBuffer), SendBuffer);


		if (sendto(rawsock, SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&rawdest, sizeof(rawdest)) == -1) //send the packet
		{ 
			perror("sendto error");
			return; 
		}
		print_debug("send success 2\n");

	}
	close(rawsock);

}

/* Receive the UDP Packet */
void locator_Udp_Recv(UdpParams *para)
{
	print_debug("locator_Udp_Recv\n");

	print_debug("*pRecPacket = %s\n", para->pRecPacket);
	print_debug("*pSendPacket = %s\n", para->pSendPacket);
	print_debug("clientAddr=%08X\n", para->clientAddr);
	print_debug("sendpacketlength= %d\n", para->sendPacketLength);
	print_debug("recvpacketlength= %d\n", para->recvPacketLength);
	print_debug("clientport= %d\n", para->clientPort);
	u8_t macAddr[6], localMacAddr[6],peerMacAddress[6];
	u8_t broadmacaddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u64_t addr;
	u16_t sequence;
	u16_t type;
	u8_t *pRevBuffer;
	pRevBuffer = para->pRecPacket;

	if (para->recvPacketLength < 20)
	{
		return;
	}

	/*
	 * Check destination address
	 */
	memcpy(macAddr, pRevBuffer+2, 6);
	printf("macAddr: %s\n", macAddr);
	int k;
	for(k=0; k<6; k++)
		printf("%.2x ",(unsigned char)macAddr[k]);
	addr = netbuf_fwd_read_u64(&pRevBuffer);	
	print_debug("addr1: %d\n", addr);
	sysLanEnetAddrGet(localMacAddr);         //get the local mac address

	if ((((*(u8_t*)(&addr) & 0x01) == 0) && (memcmp(macAddr, broadmacaddr, 6) != 0)) && \
			(((*(u8_t*)(&addr) & 0x01) == 0) && (memcmp(macAddr, localMacAddr, 6) != 0)))
	{
		return;
	}

	//happiness add read peer macAddress

	memcpy( peerMacAddress,pRevBuffer, 6);

	memset(para->macAddress, 0, 6);

	for(k=0; k<6; k++){

		para->macAddress[k] = peerMacAddress[k];

		printf("%.2x ",(unsigned char)para->macAddress[k]);

	}
	pRevBuffer = pRevBuffer +10;
	//netbuf_fwd_read_u32(pRevBuffer);
	//netbuf_fwd_read_u16(pRevBuffer);

	// para->macAddress = peerMacAddress;
	//end

	/*
	 * Read header
	 */


	//addr = netbuf_fwd_read_u64(&pRevBuffer); //happiness delete 2011.05.21
	//printf("addr2: %d\n", addr);

	sequence = netbuf_fwd_read_u16(&pRevBuffer);
	printf("sequence: %d\n", addr);
	type = netbuf_fwd_read_u16(&pRevBuffer);
	printf("type: %d\n", type);

	switch (type)
	{
		case LOCATOR_UDP_TYPE:
			print_debug("case LOCATOR_UDP_TYPE\n");
			locator_Recv_Udp_Netbuf(para, addr, sequence, type, &pRevBuffer);	
			break;

		default:
			printf("\nError Type!!");
	}
}

/* Start the processor */
void Start_Locator_Processor(void)
{
	int	locatorSocket, rc;
	UdpParams parameter;
	u8_t sendbuffer[MAX_UDP_PACKET_SIZE];

	memset(sendbuffer, 0 , sizeof(sendbuffer));
	parameter.pSendPacket = sendbuffer;
	memset(requestBuffer, 0, sizeof(requestBuffer));
	parameter.pRecPacket = requestBuffer;
	print_debug("before socket_UdpBindFixed\n");
	/* create the socket and bind the port */	
	socket_UdpBindFixed(&locatorSocket, LOCATOR_UDP_PORT);
	print_debug("after socket_UdpBindFixed\n");
	while (!quitting)
	{

		print_debug("before..Start_Locator_Processor socket_UdpReceive\n");
		rc = socket_UdpReceive(locatorSocket, &parameter);
		if (rc == 0)
		{
			print_debug("parameter.pSendPacket=%s\n",(parameter.pSendPacket));
			print_debug("parameter.pRecPacket=%s\n", (parameter.pRecPacket));
			print_debug("parameter.clientAddr=%08X\n",parameter.clientAddr);
			print_debug("parameter.clientAddr=%d\n",parameter.recvPacketLength);
			print_debug("parameter.clientAddr=%d\n",parameter.sendPacketLength);
			print_debug("after..Start_Locator_Processor socket_UdpReceive\n");
			locator_Udp_Recv(&parameter);
		}
		else
		{
			printf("socket_UdpReceive error\n");	
		}
	}
OUT:
	//close socket
	close(locatorSocket);
	return;
}

int daemonize(void)
{
	int pid, i;

	print_debug("%s... ", __func__);
	switch(fork())	{
		/* fork error */
		case -1:
			perror("fork()");
			exit(1);

			/* child process */
		case 0:
			/* obtain a new process group */
			if( (pid = setsid()) < 0){
				perror("setsid()");
				exit(1);
			}

			/* close all descriptors */
			for (i=getdtablesize();i>=0;--i) 
				close(i);		

			i = open("/dev/null",O_RDWR); /* open stdin */
			dup(i); /* stdout */
			dup(i); /* stderr */

			umask(027);
			chdir("/"); /* chdir to /tmp ? */			

			return pid;

			/* parent process */
		default:
			exit(0);
	}
}

/* Handler for the SIGTERM signal (kill) 
 * SIGINT is also handled */
	static void
sigterm(int sig)
{
	/*int save_errno = errno;*/
	signal(sig, SIG_IGN);	/* Ignore this signal while we are quitting */

	print_debug("received signal %d, good-bye", sig);

	quitting = 1;
	/*errno = save_errno;*/
}
const char * daemon_name = "locator";
const char * pidfilename = "/var/run/locator.pid";

int writepidfile(const char * fname, int pid)
{
	char pidstring[16];
	int pidstringlen;
	int pidfile;

	if(!fname || (strlen(fname) == 0))
		return -1;

	if( (pidfile = open(fname, O_WRONLY|O_CREAT|O_EXCL, 0666)) < 0){
		syslog(LOG_DEBUG, "Unable to open pidfile for writing %s: %m", fname);
		return -1;
	}

	pidstringlen = snprintf(pidstring, sizeof(pidstring), "%d\n", pid);
	if(pidstringlen <= 0){
		syslog(LOG_DEBUG, 
				"Unable to write to pidfile %s: snprintf(): FAILED", fname);
		close(pidfile);
		return -1;
	}
	else	{
		if(write(pidfile, pidstring, pidstringlen) < 0)
			syslog(LOG_DEBUG, "Unable to write to pidfile %s: %m", fname);
	}

	close(pidfile);

	return 0;
}

	int
checkforrunning(const char * fname)
{
	char buffer[64];
	int pidfile;
	pid_t pid;

	if(!fname || (strlen(fname) == 0))
		return -1;

	if( (pidfile = open(fname, O_RDONLY)) < 0)
		return 0;

	memset(buffer, 0, 64);

	if(read(pidfile, buffer, 63)){
		if( (pid = atol(buffer)) > 0){
			if(!kill(pid, 0)){
				close(pidfile);
				return -2;
			}
		}
	}

	close(pidfile);

	return 0;
}

/* 
 * 1) read command line arguments
 * 2) set signal handlers 
 */
	static int
init_daemon(int argc, char * * argv)
{
	int i;
	int pid;
	int openlog_option;
	int debug_flag = 0;
	struct sigaction sa;

	quitting = 0;
	print_debug("%s ...", __func__);

	/* command line arguments processing */
	for(i=1; i<argc; i++){
		if(argv[i][0]!='-'){
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
		else switch(argv[i][1]){
			case 'd':
				debug_flag = 1;
				break;
			case 'h':
				fprintf(stderr, "Usage:\n\t"
						"%s [-i waninterface][-a server_ip] [-p server_port]"
						" [-d debugMode] [-h help] ...\n"
						"\tDefault pid file is %s.\n"
						"\t-d debug mode\n",
						argv[0]);
				break;
			default:
				fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
	}

	if(debug_flag)	{
		pid = getpid();
	}
	else	{
#ifdef USE_DAEMON
		if(daemon(0, 0)<0) {
			perror("daemon()");
		}
		pid = getpid();
#else
		pid = daemonize();
#endif
	}

	openlog_option = LOG_PID|LOG_CONS;
	if(debug_flag)	{
		openlog_option |= LOG_PERROR;	/* also log on stderr */
	}

	openlog(daemon_name, openlog_option, LOG_DAEMON);

	if(!debug_flag){
		/* speed things up and ignore LOG_DEBUG and LOG_DEBUG */
		setlogmask(LOG_UPTO(LOG_DEBUG));
	}
	if(checkforrunning(pidfilename) < 0){
		print_debug("daemon is already running. EXITING");
		return FALSE;
	}	

	/* set signal handler */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigterm;

	if (sigaction(SIGTERM, &sa, NULL)){
		print_debug("Failed to set %s handler. EXITING", "SIGTERM");
		return FALSE;
	}
	if (sigaction(SIGINT, &sa, NULL)){
		print_debug("Failed to set %s handler. EXITING", "SIGINT");
		return FALSE;
	}

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		print_debug("Failed to ignore SIGPIPE signals");
		return FALSE;
	}
	writepidfile(pidfilename, pid);

	return TRUE;
}

int main(int argc, char** argv)
{
	if(init_daemon(argc, argv) == FALSE)
		return 1;

	debug = 1;	
	printf("output log in full\n");
	printf("start.....\n");
	Start_Locator_Processor();
	printf("over......\n");
	if(unlink(pidfilename) < 0){
		printf("Failed to remove pidfile %s: %m", pidfilename);
	}
	closelog();
	return 0;
}

void myprint(int count, char* aa){
	int k =0;
	for(;k<count; k++,aa++)
		printf("%.2x ",(unsigned char)*aa);
	printf("\n");
}
