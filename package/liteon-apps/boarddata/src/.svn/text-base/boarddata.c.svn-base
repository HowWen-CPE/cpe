/*
 *       Filename: boarddata.c
 *    Description: Board data functions. Board data format is TLV: Tag (1 byte), Length(1 byte), Vaule.
 *
 *        Version: 1.0.0
 *        Created: 2013.08.02
 *       Revision:  
 *       Compiler: mips(/arm)-linux-gcc
 *         Author: LiteON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include "boarddata.h"


/**
 *\brief Determine Boarddata device node. 
 *\return the path-to-file of flash block
 */
static int board_find_mtd(char *path)
{
	FILE *fp;
	int i, esz;
	char dev[PATH_MAX]; 
	struct stat s;
	int result = -1;

	if(fp = fopen("/proc/mtd", "r"))
	{
		while( fgets(dev, sizeof(dev), fp) )
		{
			if( strstr(dev, BOARD_MTD_NAME) && sscanf(dev, "mtd%d: %08x", &i, &esz) )
			{
				sprintf(dev, "/dev/mtdblock/%d", i);
				if( stat(dev, &s) > -1 && (s.st_mode & S_IFBLK) )
				{
					strncpy(path, dev, strlen(dev)+1);
					result = 0;
					break;
				}
				else
				{
					sprintf(dev, "/dev/mtdblock%d", i);
					if( stat(dev, &s) > -1 && (s.st_mode & S_IFBLK) )
					{
						strncpy(path, dev, strlen(dev)+1);
						result = 0;
						break; 
					}
				}
			}
		}
		fclose(fp);
	}

	return result;
}

static int MacGetPosPower(int base, int power)
{
	int TempBase;

	if (power == 0)
		return 1;
	else
	{
		TempBase = base;
		while (power - 1)
		{
			base *= TempBase;
			power--;
		}
		return base;
	}
}

static unsigned char MacAtoX(char * str)
{
	int len, count = 0;
	int num = 0;
	unsigned char c;

	len = strlen(str);
	while (len)
	{
		switch (*str)
		{
			case '0':
				count = 0;
				break;
			case '1':
				count = 1;
				break;
			case '2':
				count = 2;
				break;
			case '3':
				count = 3;
				break;
			case '4':
				count = 4;
				break;
			case '5':
				count = 5;
				break;
			case '6':
				count = 6;
				break;
			case '7':
				count = 7;
				break;
			case '8':
				count = 8;
				break;
			case '9':
				count = 9;
				break;
			case 'a':
			case 'A':
				count = 10;
				break;
			case 'b':
			case 'B':
				count = 11;
				break;
			case 'c':
			case 'C':
				count = 12;
				break;
			case 'd':
			case 'D':
				count = 13;
				break;
			case 'e':
			case 'E':
				count = 14;
				break;
			case 'f':
			case 'F':
				count = 15;
				break;
		}
		num += count * (MacGetPosPower(16, len - 1));
		str++;
		len--;
	}
	c = (unsigned char) num;


	return c;
}

/* 
 *parse the mac addr string such as  "00:01:DD:33:33:44"
 */
static int ParseMac(const unsigned char * ucMac, unsigned char * mac)
{
	int i, j, k;
	char tmp[3];
	int flag = 0;

	i = 0;
	j = 0;
	k = 0;
	while (1)
	{
		tmp[j++] = ucMac[i++];
		if (ucMac[i] == ':')
		{
			tmp[j] = '\0';
			j = 0;
		}
		else if (ucMac[i] == '\0')
		{
			tmp[j] = '\0';
			flag = 1;
		}
		else if ( j > 1)
			return -1;
		else
			continue;
		mac[k++] = MacAtoX(tmp);
		i++;
		if (k == 6)
			break;
		if (flag == 1)
			return -1;
	}

	return 0;

}

/* 
 *Filter the invalid mac addr such as "00:00:00:00:00:00"/"ff:ff:ff:ff:ff:ff"/"01:xx:xx:xx:xx:xx"
 */
static int InvalidMac(const unsigned char * mac)
{
	/*Filter mac address with full 0*/
	if( 0 == mac[0] && 0 == mac[1] && 0 == mac[2] \
		&& 0 == mac[3] && 0 == mac[4] && 0 == mac[5])
	{
		return -1;
	}

	/*Filter broadcast mac address*/
	if( 0xff == mac[0] && 0xff == mac[1] && 0xff == mac[2] \
		&& 0xff == mac[3] && 0xff == mac[4] && 0xff == mac[5])
	{
		return -1;
	}

	/*Filter multicast mac address*/
	if( 0x01 == (mac[0] & 0x01))
	{
		return -1;
	}

	return 0;
}


/*
 * Print the tlv list.
 * 
 * @tlvlist: IN.
 * 
 * return: void.
 */
static void show_tlv_list(const struct data_tlv_list_t tlvlist[])
{

	int ii;

	for(ii = 0; ii < MAX_TAGS; ii++)
	{
		if(tlvlist[ii].tag != 0 && tlvlist[ii].tag != 0xff)
		{
			printf("Tag: 0x%x  Len: %d Data: %s\n", tlvlist[ii].tag, tlvlist[ii].len, tlvlist[ii].data);
		}else
		{
			break;
		}
	}

}

/*
 * Get tlv list from the board data position.
 * 
 * @tlvlist: OUT.
 * 
 * return: -1 -- fail; otherwire the object number.
 */
static int get_tlv_list(struct data_tlv_list_t tlvlist[])
{
	int fd;
	unsigned char *buf;
	int ii, p, item_num;
	char path[32]; 

	if (board_find_mtd(path))
	{
		printf("can not find boaddata partition");
		return -1;
	}

	fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		printf("open device(%s) failed\n",path);
		return -1;
	}   

	buf = malloc(BOARD_DATA_LEN);
	memset(buf, 0xff, BOARD_DATA_LEN);

	/* read BOARD_DATA_LEN bytes. */
	if(read(fd, (void*)buf, BOARD_DATA_LEN) <= 0 )
	{
		printf("GetTlvList: read error\n");
		close(fd);
		free(buf);
		return -1;
	}

	close(fd);

	p = 0;
	item_num = 0;
	ii = 0;

	/* get all items*/
	while(buf[p] != 0xff && buf[p] != 0)
	{
		tlvlist[ii].tag = buf[p];  /* tag*/
		p++;

		tlvlist[ii].len = buf[p];  /* length*/

		if(tlvlist[ii].len == 0)
		{
			printf("TLV format error, length is 0 for tag %x\n", tlvlist[ii].tag);
			break;
		}

		if(tlvlist[ii].len >= 128)
		{
			printf("TLV format error, length is too big(%d) for tag %x\n", tlvlist[ii].len, tlvlist[ii].tag);
			break;
		}

		p++;
		memcpy(tlvlist[ii].data, buf+p, tlvlist[ii].len);  /* content*/

		p += tlvlist[ii].len;  /* point to next tag*/
		item_num++;
		ii++;

		if(item_num > MAX_TAGS)
		{
			printf("There is too much items in TLV list\n");
			break;
		}

	}

	free(buf);

	if(item_num == 0)
	{
		printf("TLV item number is 0\n");
	}

	return item_num;
}

/*
 * Get the item with the fixed tag from tlv list.
 * 
 * @tag: tag. eg. TLV_MAC_TAG.
 * @len: length for this item content. OUT.
 * @data: content. OUT.
 *
 * return: 0 -- get OK; -1 -- get fail.
 */
static int get_tlv_item(unsigned char tag, unsigned char *len, unsigned char *data)
{
	int ii, find;
	struct data_tlv_list_t *tlvlist;

	tlvlist = malloc(MAX_TAGS * sizeof(struct data_tlv_list_t));

	memset(tlvlist, 0, sizeof(struct data_tlv_list_t) * MAX_TAGS);

	/* get list*/
	if(get_tlv_list(tlvlist) <= 0)
	{
		printf("No items find\n");
		free(tlvlist);
		return -1;
	}

	find = 0;

	/* search this item*/
	for(ii = 0; ii < MAX_TAGS; ii++)
	{
		if(tlvlist[ii].tag == tag)
		{
			memcpy(data, tlvlist[ii].data, tlvlist[ii].len);
			*len = tlvlist[ii].len;

			find = 1;
			break;
		}
	}

	free(tlvlist);
	return (find ? 0 : -1);
}


/*
 * Write tlv table to flash.
 *
 * @tlvlist: the tlv table which will be writen to flash. IN.
 *
 * return: 0 -- OK. -1 -- fail.
 */
static int write_tlv_list(const struct data_tlv_list_t tlvlist[])
{
	long fd;
	int len, ii;
	unsigned char *buf, *ptr;
	char path[32]; 

	if (board_find_mtd(path))
	{
		printf("can not find boaddata partition");
		return -1;
	}

	fd = open(path, O_WRONLY | O_CREAT );
	if(fd < 0)
	{
		printf("open device(%s) failed\n", path);
		return -1;
	}

	buf = malloc(BOARD_DATA_LEN);
	memset(buf, 0xff, BOARD_DATA_LEN);

	/* create TLV data buffer*/
	ptr = buf;
	for(ii = 0; ii < MAX_TAGS; ii++)
	{
		if(tlvlist[ii].tag != 0xff && tlvlist[ii].tag != 0)
		{
			*ptr = tlvlist[ii].tag;
			ptr++;
			*ptr = tlvlist[ii].len;
			ptr++;
			memcpy(ptr, tlvlist[ii].data, tlvlist[ii].len);
			ptr += tlvlist[ii].len;
		}else
		{
			break;
		}
	}

	len = ptr - buf;

	/* write to flash*/
	if(write(fd, buf, BOARD_DATA_LEN) <= 0)
	{
		printf("Write to flash failed\n");
		close(fd);
		free(buf);
		return -1;
	}

	free(buf);
	close(fd);
	return 0;
}
/*
 * Change the relative  item and write it to flash.
 * 
 * @item: item need update. IN.
 *
 * return: -1 -- fail. 0 -- OK.
 */
static int change_tlv_item(const struct data_tlv_list_t *item)
{
	struct data_tlv_list_t *tlvlist;
	int ii, find = 0;

	tlvlist = malloc(MAX_TAGS * sizeof(struct data_tlv_list_t));

	/* get TLV list*/
	if(get_tlv_list(tlvlist) <= 0)
	{
		printf("There is no items in board data\n");
		return -1;
	}

	/* search TLV list and update the value*/
	for(ii = 0; ii < MAX_TAGS; ii++)
	{
		if(tlvlist[ii].tag == item->tag)
		{
			tlvlist[ii].len = item->len;
			memcpy(tlvlist[ii].data, item->data, tlvlist[ii].len);

			find = 1;
			break;
		}
	}

	/* write all the TLV back to board data*/
	if(find)
	{
		if(write_tlv_list(tlvlist) < 0)
		{
			printf("Write item failed\n");
			free(tlvlist);
			return -1;
		}
	}else
	{
		free(tlvlist);
		return -1;
	}

	free(tlvlist);
	return 0;

}

/*
 * Add the item to the board data.
 * 
 * @item: item need add. IN.
 *
 * return: -1 -- fail. 0 -- OK.
 */
static int add_tlv_item(const struct data_tlv_list_t *item)
{
	struct data_tlv_list_t *tlvlist;
	tlvlist = malloc(MAX_TAGS * sizeof(struct data_tlv_list_t));

	int num = 0;

	memset(tlvlist, 0, sizeof(struct data_tlv_list_t) * MAX_TAGS);

	/* get TLV list*/
	if((num = get_tlv_list(tlvlist)) < 0)
	{
		printf("Read TLV error\n");

		free(tlvlist);
		return -1;
	}

	/* add item in the end*/
	tlvlist[num].tag= item->tag;
	tlvlist[num].len = item->len;
	memcpy(tlvlist[num].data, item->data, tlvlist[num].len);


	/* write all the TLV back to board data*/
	if(write_tlv_list(tlvlist) < 0)
	{
		printf("Write item failed\n");
		free(tlvlist);
		return -1;
	}

	free(tlvlist);
	return 0;
}


/*
 * print the board data.
 * 
 * @tlvlist: OUT.
 * 
 * return: -1 -- fail; otherwire the object number.
 */
int get_board_data(struct data_tlv_list_t tlvlist[MAX_TAGS])
{
	int num = 0;

	memset(tlvlist, 0, sizeof(struct data_tlv_list_t) * MAX_TAGS);

	/* get TLV list*/
	if((num = get_tlv_list(tlvlist)) <= 0)
	{
		return -1;
	}

	return num;

}

/*
 * print the board data.
 * 
 * @tlvlist: OUT.
 * 
 * return: void.
 */
void show_board_data(void)
{
	struct data_tlv_list_t *tlvlist;

	int num = 0, ii, fd;
	unsigned char buf[256];

	char path[32]; 

	if (board_find_mtd(path))
	{
		printf("can not find boaddata partition");
		return;
	}

	fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		printf("open device(%s) failed\n", path);
		return;
	}
	memset(buf, 0xff, 256);

	tlvlist = malloc(MAX_TAGS * sizeof(struct data_tlv_list_t));

	/* read 256 bytes. */
	if(read(fd, (void*)buf, 256) <= 0 )
	{
		printf("GetBoardData: read error\n");
		free(tlvlist);
		return;
	}

	close(fd);

	for(ii = 0; ii < 128; ii++)
	{
		printf(" %2.2x", buf[ii]);

		if((ii+1)%32 == 0)
			printf("\n");
	}

	printf("\n");

	printf("\nParse board data: \n");

	memset(tlvlist, 0, sizeof(struct data_tlv_list_t) * MAX_TAGS);
	/* get TLV list*/
	if((num = get_tlv_list(tlvlist)) <= 0)
	{
		printf("Read TLV error\n");
		free(tlvlist);
		return;
	}

	/* show*/
	for(ii = 0; ii < num; ii++)
	{
		switch(tlvlist[ii].tag)
		{
			case TLV_MAC_TAG:
				printf("Tag: 0x%x   ", tlvlist[ii].tag);
				printf("Len: %d  ", tlvlist[ii].len);
				printf("MAC: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", 
						tlvlist[ii].data[0], tlvlist[ii].data[1], tlvlist[ii].data[2],
						tlvlist[ii].data[3], tlvlist[ii].data[4], tlvlist[ii].data[5]);
				break;
			case TLV_SN_TAG:
				printf("Tag: 0x%x   ", tlvlist[ii].tag);
				printf("Len: %d  ", tlvlist[ii].len);
				printf("SN: %s\n", tlvlist[ii].data); 
				break;
			case TLV_ABORT_TAG:
				printf("Tag: 0x%x   ", tlvlist[ii].tag);
				printf("Len: %d  ", tlvlist[ii].len);
				printf("Abort flag: 0x%2.2x 0x%2.2x\n", tlvlist[ii].data[0], tlvlist[ii].data[1]); 
				break;
			case TLV_MFG_TAG:
				printf("Tag: 0x%x   ", tlvlist[ii].tag);
				printf("Len: %d  ", tlvlist[ii].len);
				printf("MFG state: 0x%x\n", tlvlist[ii].data[0]);
				break;
			default:
				printf("Unkown tag: %x  ", tlvlist[ii].tag);
				printf("Len: %d  \n", tlvlist[ii].len);
				break;    
		}

	}

	free(tlvlist);
	return;
}

/*
 * Format/initialize the board data area.
 * 
 * return: -1 -- fail. 0 -- OK.
 */
int init_board_data(void)
{
#ifdef GP_WP688
	/*default bspdata:
	  mac			24:FD:52:F2:4E:00
	  sn			"WP688136000001"
	  boot abort	0x00 0x00
	  mfg state		0x00
	 */
	unsigned char default_bspdata[]={0x43, 0x06, 0x24, 0xfd, 0x52, 0xf2, 0x4e, 0x00, 
		0x42, 0x0e, 0x57, 0x50, 0x36, 0x38, 0x38, 0x31, 
		0x33, 0x36, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 
		0x41, 0x02, 0x00, 0x00,
		0x40, 0x01, 0x00,
		0xff};
#endif

#ifdef GP_WP838
	/*default bspdata:
	  mac			A4:DB:30:DD:07:00
	  sn			"WP83813A000001"
	  boot abort	0x00 0x00
	  mfg state		0x00
	 */
	unsigned char default_bspdata[]={0x43, 0x06, 0xa4, 0xdb, 0x30, 0xdd, 0x07, 0x00, 
		0x42, 0x0e, 0x57, 0x50, 0x38, 0x33, 0x38, 0x31, 
		0x33, 0x41, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 
		0x41, 0x02, 0x00, 0x00,
		0x40, 0x01, 0x00,
		0xff};
#endif

	unsigned char *buf;
	int fd;
	char path[32]; 

	if (board_find_mtd(path))
	{
		printf("can not find boaddata partition");
		return -1;
	}

	fd = open(path, O_WRONLY);
	if(fd < 0)
	{
		printf("open device(%s) failed\n", path);
		return -1;
	}

	buf = malloc(BOARD_DATA_LEN);
	memset(buf, 0xff, BOARD_DATA_LEN);

	memcpy(buf, default_bspdata, sizeof(default_bspdata));

	/* write to flash*/
	if(write(fd, buf, BOARD_DATA_LEN) <= 0)
	{
		printf("Write to flash failed\n");
		free(buf);
		close(fd);
		return -1;
	}

	free(buf);
	close(fd);

	return 0;
}

/* 
 * get serial number from board data. 
 * 
 * @sn: sn string. OUT.
 * @len: sn length. OUT.
 * 
 * return: -1 -- fail; 0 -- OK.
 */
int  get_sn(unsigned char *sn, unsigned char *len)
{  
	if(get_tlv_item(TLV_SN_TAG, len, sn) < 0)
	{
		printf("Get serial number failed.\n");
		return -1;
	}

	return 0;
}

/* 
 * Set serial number in board data. 
 * 
 * @sn: sn string. IN.
 * @len: sn length. IN.
 * 
 * return: -1 -- fail; 0 -- OK.
 */
int set_sn(const unsigned char *sn, int len)
{
	struct data_tlv_list_t item;

	item.tag = TLV_SN_TAG;
	item.len = len;
	memcpy(item.data, sn, len);

	return change_tlv_item(&item);
}

/* 
 * Get mac from board data. 
 * 
 * @mac[6]: OUT.
 * 
 * return: -1 -- fail; 0 -- OK.
 */
int get_mac(unsigned char mac[6])
{
	unsigned char buf[128], len;

	memset(buf, 0, 128);

	/* get mac address*/
	if(get_tlv_item(TLV_MAC_TAG, &len, buf) < 0)
	{
		printf("Get mac address fail\n");
		return -1;
	}

	if(len != 6)
	{
		printf("Mac address in TLV error\n");
		return -1; 
	}

	memcpy(mac, buf, 6);

#if 0
	/* print mac address*/
	printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", 
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
#endif

	return 0;
}

/* 
 * Set mac in board data. 
 * 
 * @mac_str: such as  xx:xx:xx:xx:xx:xx
 * 
 * return: -1 -- fail; 0 -- OK.
 */
int set_mac(const unsigned char mac_str[])
{
	unsigned char mac[6];
	struct data_tlv_list_t item;

	/* TODO: check mac validation. Or ParseMac do this job*/

	if(ParseMac(mac_str, mac)) {
		printf("Mac adress format is error\n");
		return -1;
	}

	if(InvalidMac(mac)) {
		printf("Mac address is invalid\n");
		return -1;
	}

	item.tag = TLV_MAC_TAG;
	item.len = 6;
	memcpy(item.data, mac, item.len);

	/* set mac to flash*/
	return change_tlv_item(&item);
}

/* 
 * Enable kernel booting abort
 * Booting kernel will be aborted and enter downloading mode at system startup.
 */
int enable_boot_abort(void)
{
	struct data_tlv_list_t item;

	item.tag = TLV_ABORT_TAG;
	item.len = 2;
	item.data[0] = 0;
	item.data[1] = 1;

	return change_tlv_item(&item);
}

/*
 * Disable kernel booting abort
 * Booting kernel will be carried out normally. This is the normal system startup process.
 */
int disable_boot_abort(void)
{
	struct data_tlv_list_t item;

	item.tag = TLV_ABORT_TAG;
	item.len = 2;
	item.data[0] = 0;
	item.data[1] = 0;

	return change_tlv_item(&item);
}

/* 
 * Get kernel booting abort settings from board data
 *
 * @flag: point to flag. OUT.
 */
int get_boot_abort(unsigned short *flag)
{
	unsigned char buf[128];
	unsigned char len;

	memset(buf, 0, 128);

	if(get_tlv_item(TLV_ABORT_TAG, &len, buf) < 0)
	{
		printf("Get abort flag failed.\n");
		return -1;
	}

	*flag = (unsigned short)(((unsigned short)buf[0] << 8) | buf[1]);

	//printf("Booting abort: %s\n", (*flag == 1) ? "Enabled" : "Disabled");
	printf("%s\n", (*flag == 1) ? "Enabled" : "Disabled");
	return 0;

}


/* 
 * Get mfg state from board data
 * 
 * @state: OUT.
 * 
 * return: -1 -- fail; 0 -- OK
 */
int  get_mfg_state(unsigned char *state)
{
	unsigned char buf[128], len;

	memset(buf, 0, 128);

	/* get mac address*/
	if(get_tlv_item(TLV_MFG_TAG, &len, buf) < 0)
	{
		printf("Get MFG State fail\n");
		return -1;
	}

	if(len != 1)
	{
		printf("MFG in TLV error\n");
		return -1; 
	}

	*state = buf[0];

	/* print mac address*/
	//printf("MFG State: 0x%x\n", buf[0]);
	printf("%d\n", buf[0]); 
	return 0;
}


/*
 * Set mfg state. If mfg state don't exist in board data, then add it.
 *
 * @val: the value want to set.
 * 
 * return: -1: fail; 0 -- OK.
 */
int set_mfg_state(unsigned char val)
{
	struct data_tlv_list_t item;
	unsigned char buf[128], len;
	int ret;

	memset(buf, 0, 128);
	if(get_tlv_item(TLV_MFG_TAG, &len, buf) < 0)
	{
		/* not found, so need add it*/
		item.tag = TLV_MFG_TAG;
		item.len = 1;
		item.data[0] = val;

		ret = add_tlv_item(&item);
	}else
	{
		/* item already exist, so just change it*/

		item.tag = TLV_MFG_TAG;
		item.len = 1;
		item.data[0] = val;

		/* set mac to flash*/
		ret = change_tlv_item(&item);
	}

	if(ret == 0)
	{
		printf("Set MFG State: 0x%x\n", item.data[0]);
		return 0;
	}else
	{
		return -1;
	}
}

