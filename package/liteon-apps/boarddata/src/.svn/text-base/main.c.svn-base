/*
 *       Filename: main.c
 *    Description: Board data functions command line interface.
 *        Version: 1.0.0
 *        Created: 2013.08.02
 *       Revision:  
 *       Compiler: mips(/arm)-linux-gcc
 *         Author: LiteON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boarddata.h"

void usage(void)
{
	printf("\n\
			usage:\n\
			\tboarddata set mac [MAC Address] \n\
			\tboarddata get mac \n\
			\tboarddata set sn [Serial Number] \n\
			\tboarddata get sn \n\
			\tboarddata set boot_abort [enable/disable] \n\
			\tboarddata get boot_abort \n\
			\tboarddata set mfg_state [Decimal Number] \n\
			\tboarddata get mfg_state \n\
			\tboarddata show \n\
			\tboarddata init \n\
			\n");
	exit(0);
}

/* BOARD DATA utility */
int main(int argc, char **argv)
{
	char name[16], buf[32];
	unsigned char size, i=0, num;

	/* Skip program name */
	--argc;
	++argv;

	if (!*argv)
		usage();

	/* Process the arguments */
	for (; *argv; *++argv) {
		if (!strcmp(*argv, "get")) {
			if (*++argv) {
				strcpy(buf, *argv);
				if (!strcmp(buf, "mac"))
				{
					if (!get_mac((unsigned char *)buf))
						//printf("MAC Address: ");
						printf("%02x:%02x:%02x:%02x:%02x:%02x\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], (unsigned char)buf[3], (unsigned char)buf[4], (unsigned char)buf[5]);
					else
						return -1;
				}
				else if (!strcmp(buf, "sn"))
				{
					if (!get_sn((unsigned char *)buf, &size))
					{
						//printf("Serial No: ");
						for (i=0; i<size; i++)
							printf("%c",buf[i]);
						printf("\n");
					}
					else
						return -1;
				}
				else if (!strcmp(buf, "boot_abort"))
				{
					if (get_boot_abort((unsigned short *)buf))
						return -1;
				}
				else if (!strcmp(buf, "mfg_state"))
				{
					if (get_mfg_state((unsigned char *)buf))
						return -1;
				}
				else
					usage();
			}
		} else if (!strcmp(*argv, "set")) {
			if (*++argv) {
				strcpy(name, *argv);
				if (*++argv) {
					strcpy(buf, *argv);
					if (!strcmp(name, "mac"))
					{
						if (set_mac((unsigned char *)buf))
							return -1;
					}
					else if (!strcmp(name, "sn"))
					{
						if (set_sn((unsigned char *)buf, strlen(buf)))
							return -1;
					}
					else if (!strcmp(name, "boot_abort"))
					{
						if (!strcmp(buf, "enable"))
							enable_boot_abort();
						else if (!strcmp(buf, "disable"))
							disable_boot_abort();
						else
							usage();
					}
					else if (!strcmp(name, "mfg_state"))
					{
						num = (unsigned char) atoi(buf);
						if (set_mfg_state(num))
							return -1;
					}
					else
						usage();			
				}
			}
		} else if (!strcmp(*argv, "show")) {
			show_board_data();
		} else if (!strcmp(*argv, "init")) {
			init_board_data();
		} else
			usage();
	}

	return 0;
}

