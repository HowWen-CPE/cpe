#include <stdlib.h>
#include <stdio.h>
#include "ethread.h"
//example
int main(int argc, char *argv[])
{
	struct eth_port_status ethport;
	memset(&ethport, 0, sizeof(struct eth_port_status));
#if defined(GP_WP688)
        printf("*******Product: GP_WP688***********\n");
#endif
	ethport.port = 4;
	if (ethread(&ethport) < 0) {
		printf("read error\n");
		return -1;
	}
	if (ethport.link) {
		printf("up\n");
	} else {
		printf("down\n");
	}

	if (ethport.duplex) {
		printf("full\n");
	} else {
		printf("half\n");
	}

	if (ethport.speed == 0) {
		printf("10M\n");
	} else if (ethport.speed == 1) {
		printf("100M\n");
	} else if (ethport.speed == 2) {
		printf("1000M\n");
	} else {
		printf("Invalid speed\n");
	}
	return 0;
}
