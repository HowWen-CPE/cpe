/*
 *       Filename: boarddata.h
 *    Description: Board data functions definition.
 *        Version: 1.0.0
 *        Created: 2013.08.02
 *       Revision:  
 *       Compiler: mips(/arm)-linux-gcc
 *         Author: LiteON.
 */

#ifndef BOARDDATA_H
#define BOARDDATA_H

#define BOARD_MTD_NAME	"board_data"

/* TAG defination*/
#define TLV_MAC_TAG  0x43
#define TLV_SN_TAG 0x42
#define TLV_ABORT_TAG 0x41
#define TLV_MFG_TAG 0x40

#define MAX_TAGS  6

struct data_tlv_list_t{
	unsigned char tag;
	unsigned char len;
	unsigned char data[128];
};

#define BOARD_DATA_LEN (MAX_TAGS * sizeof(struct data_tlv_list_t))


int get_board_data(struct data_tlv_list_t tlvlist[]);
void show_board_data(void);
int init_board_data(void);
int get_sn(unsigned char *sn, unsigned char *len);
int set_sn(const unsigned char *sn, int len);
int get_mac(unsigned char mac[6]);
int set_mac(const unsigned char mac_str[]);
int enable_boot_abort(void);
int disable_boot_abort(void);
int get_boot_abort(unsigned short *flag);
int  get_mfg_state(unsigned char *state);
int set_mfg_state(unsigned char val);
#endif

