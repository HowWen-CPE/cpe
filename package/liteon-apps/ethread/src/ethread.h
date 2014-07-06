#define RT_RDM_CMD_READ			0x6B03
#define ETH_IO_DEV_NAME "/dev/rdm0"

#define __RALINK_WP688__

struct eth_port_status {
	int port;
	int link;
	int speed;
	int duplex;
};
/* assign correct port number before call this function*/
int ethread(struct eth_port_status *ethport); 

