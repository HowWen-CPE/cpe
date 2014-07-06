#include <stdlib.h>		/* malloc, free, etc. */
#include <stdio.h>		/* stdin, stdout, stderr */
#include <string.h>		/* strdup */
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ethread.h"

#define AR8035_P0 1

#define ETH_PORT_LINK_UP	1
#define ETH_PORT_LINK_DOWN	0
#define ETH_PORT_SPEED_10M	0
#define ETH_PORT_SPEED_100M	1
#define ETH_PORT_SPEED_1000M	2
#define ETH_PORT_HALF_DUPLEX	0
#define ETH_PORT_FULL_DUPLEX	1

int ethread(struct eth_port_status *ethport)
{
#if defined(GP_WP688)

	int fd, offset;

	if (!ethport)
		return -1;
	fd = open(ETH_IO_DEV_NAME, O_RDONLY);
	if (fd < 0) {
		printf("Open pseudo device failed\n");
		return -1;
	}

	switch (ethport->port) {
	case 4:
		offset = 0x3408;	//get port 4 status      
		break;
	default:
		return -1;
	}

	if (ioctl(fd, RT_RDM_CMD_READ, &offset) < 0) {
		close(fd);
		return -1;
	}
	// LINK up or down
	if (offset & 0x01) {
		ethport->link = ETH_PORT_LINK_UP;
	} else {
		ethport->link = ETH_PORT_LINK_DOWN;
	}
	//Duplex half or full
	if (offset & 0x02) {
		ethport->duplex = ETH_PORT_FULL_DUPLEX;
	} else {
		ethport->duplex = ETH_PORT_HALF_DUPLEX;
	}
	//Speed 10M or 100M or 1000M
	offset = (offset >> 2) & 0x03;
	switch (offset) {
	case 0:
		ethport->speed = ETH_PORT_SPEED_10M;
		break;
	case 1:
		ethport->speed = ETH_PORT_SPEED_100M;
		break;
	case 2:
		ethport->speed = ETH_PORT_SPEED_1000M;
		break;
	default:

		ethport->speed = -1;
	}

	close(fd);
	
#elif defined(GP_WP838)

  #if defined(AR8035_P0)
	int offset;
	FILE *fd;
	unsigned int linkStatus;
	char cmd[128] = {0};
	
	switch (ethport->port) {
	case 0:
		offset = 0x11;	   //get p0 status    
		break;
	case 2:
		offset = 0x11;	     
		break;
	case 3:
		offset = 0x11;	      
		break;
	default:
		return -1;
	}
	
	sprintf(cmd, "ethreg -i eth0 -p 0 0x%x | awk -F '0x' '{print $3}' > /tmp/port_ethreg", offset);
	system(cmd);
	
	if (NULL == (fd = fopen("/tmp/port_ethreg", "r"))) {	
		fclose(fd);
		return -1;
	} else {
		fscanf(fd, "%08x", &linkStatus);

		if ((linkStatus >> 10)& 0x01) {					//get Link Status
			ethport->link = ETH_PORT_LINK_UP;

			switch ((linkStatus >> 14) & 0x03) {				//get speed
			case 0:
				ethport->speed = ETH_PORT_SPEED_10M;
                printf("10\n");
				break;
			case 1:
				ethport->speed = ETH_PORT_SPEED_100M;
                printf("100\n");
				break;
			case 2:
				ethport->speed = ETH_PORT_SPEED_1000M;
                printf("1000\n");
				break;
			default:
				ethport->speed = -1;
				break;
			}

			switch ((linkStatus >> 13)& 0x01) {			//get Duplex Mode
			case 0:
				ethport->duplex = ETH_PORT_HALF_DUPLEX;
                printf("half\n");
				break;
			case 1:
				ethport->duplex = ETH_PORT_FULL_DUPLEX;
                printf("half\n");
				break;
			default:
				ethport->duplex = -1;
				break;
			}
			
		} else {
			ethport->link = ETH_PORT_LINK_DOWN;
		}

		fclose(fd);
	}
	system("rm -rf /tmp/port_ethreg");
  #else
	int offset;
	FILE *fd;
	unsigned int linkStatus;
	char cmd[128] = {0};
	
	switch (ethport->port) {
	case 2:
		offset = 0x0084;	//get port 2 status      
		break;
	case 3:
		offset = 0x0088;	//get port 3 status      
		break;
	default:
		return -1;
	}
	
	sprintf(cmd, "ethreg -i eth0 0x%x | awk -F '0x' '{print $3}' > /tmp/port_ethreg", offset);
	system(cmd);
	
	if (NULL == (fd = fopen("/tmp/port_ethreg", "r"))) {	
		fclose(fd);
		return -1;
	} else {
		fscanf(fd, "%08x", &linkStatus);

		if ((linkStatus >> 8)& 0x01) {					//get Link Status
			ethport->link = ETH_PORT_LINK_UP;

			switch (linkStatus & 0x03) {				//get speed
			case 0:
				ethport->speed = ETH_PORT_SPEED_10M;
				break;
			case 1:
				ethport->speed = ETH_PORT_SPEED_100M;
				break;
			case 2:
				ethport->speed = ETH_PORT_SPEED_1000M;
				break;
			default:
				ethport->speed = -1;
				break;
			}

			switch ((linkStatus >> 6)& 0x01) {			//get Duplex Mode
			case 0:
				ethport->duplex = ETH_PORT_HALF_DUPLEX;
				break;
			case 1:
				ethport->duplex = ETH_PORT_FULL_DUPLEX;
				break;
			default:
				ethport->duplex = -1;
				break;
			}
			
		} else {
			ethport->link = ETH_PORT_LINK_DOWN;
		}

		fclose(fd);
	}
	system("rm -rf /tmp/port_ethreg");
  #endif
#endif
	return 0;
}
