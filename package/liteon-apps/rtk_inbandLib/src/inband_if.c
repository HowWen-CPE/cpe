/*
 * IOH daemon
 * Copyright (C)2010, Realtek Semiconductor Corp. All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <syslog.h>

#include <linux/if_packet.h>
#include "ioh.h"
#include "inband_if.h"

#if 0
struct inband_header {
	struct ioh_header raw_header;
	unsigned char rrcp_type;	// should be RRCP_P_IOH
	unsigned char inband_cmd;
	unsigned short inband_seq;
	unsigned short inband_frag;
	unsigned char inband_index;
	unsigned char inband_reserved;
	unsigned short inband_data_len;
} __attribute__((packed));
#endif

struct fragment_info{
	unsigned char inband_cmd;
	unsigned short inband_seq;
	unsigned short inband_frag;
	char *buf_ptr;
	char *buf;	
	unsigned int data_len;
};

struct inband_class {
	struct ioh_class ioh_obj;
	struct inband_header *tx_header;
	struct inband_header *rx_header;
	unsigned char *tx_data;
	unsigned char *rx_data;
	struct fragment_info frag_info;
	unsigned int in_used;
};

//header : 4 sign , 4 opttion , 4 offset, 4 len
#define FM_HEADER_LEN_OFFSET 12

#define MAX_INBAND_CHAN 2
#define MAX_PREALLOC_INBAND_CHAN MAX_INBAND_CHAN

int inband_rcv_timeout=0; //mark_issue, not implement now

struct inband_class inband_obj[MAX_INBAND_CHAN];
static int inband_ready=0;

static void init_inband_obj(struct inband_class *ib_obj)
{
	struct ioh_class *obj=&ib_obj->ioh_obj;

	ib_obj->tx_header = (struct inband_header *)obj->tx_header;
	ib_obj->rx_header = (struct inband_header *)obj->rx_header;

	ib_obj->tx_data = (unsigned char *) obj->tx_buffer + sizeof(struct inband_header);
	ib_obj->rx_data = (unsigned char *) obj->rx_buffer + sizeof(struct inband_header);

	//frag_info?
}

static unsigned int get_free_chan()
{	
	int i;
	int chan=-1;
	
	for(i=0;i<MAX_INBAND_CHAN;i++)
	{
		if(inband_obj[i].in_used ==0 )			
		{
			inband_obj[i].in_used =1;
			chan = i;
			break;
		}		
	}
	return chan;	
}

struct inband_class *get_chan_obj(unsigned int chan)
{
	return (struct inband_class *)&inband_obj[chan];
}

static void inband_init_all()
{
	memset(&inband_obj[0],0,sizeof(struct inband_class)*MAX_INBAND_CHAN);
}

int inband_open(char *netif_name,char *slave_mac,unsigned short eth_type,int debug)
{	
	int ret;
	unsigned int chan=0;
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;

	if(inband_ready == 0)
	{
		inband_init_all();
		inband_ready =1 ;
	}

	chan = get_free_chan();
	if(chan < 0) {
        syslog(LOG_DEBUG, "open channel error! chann=%d\n", chan);
		return -1;
	}

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);

	ioh_obj_p = &inband_obj_p->ioh_obj;	
		
	ret = ioh_open(ioh_obj_p, netif_name, slave_mac,eth_type, debug);

	if(ret < 0 ) {
		return -1;
	}

	init_inband_obj(inband_obj_p);

	return chan;	
}

int get_inband_socket(int chan)
{
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);
	ioh_obj_p = &inband_obj_p->ioh_obj;	
	
	return ioh_obj_p->sockfd;
}

int get_inband_destMac(int chan,char *destmac) //mark_test
{
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);
	ioh_obj_p = &inband_obj_p->ioh_obj;	
	
	memcpy(destmac,ioh_obj_p->dest_mac,6);
	
	return 0;
}

void inband_close(int chan)
{	
	//clear frag_info
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	ioh_obj_p = &inband_obj_p->ioh_obj;	
	ioh_close(ioh_obj_p);	
	inband_obj_p->in_used =0; //free the obj
}

static int inband_recv(struct inband_class *ib_obj,int timeout)
{
    struct ioh_class *ioh_obj_p = &ib_obj->ioh_obj;
    int rx_len;

    memset(ioh_obj_p->rx_buffer,0,BUF_SIZE);
    rx_len = ioh_recv(ioh_obj_p, timeout);
    if (rx_len < 0) {
       syslog(LOG_DEBUG, "receive timeout, rx_len=%d", rx_len);
       return ERROR_TIMOUT;
	  }

    if(ib_obj->rx_header->rrcp_type != RRCP_P_IOH)
            return -1;

    return rx_len;
}

static int inband_indexed_recv(struct inband_class *ib_obj,unsigned char idx,int timeout)
{
    struct ioh_class *ioh_obj_p = &ib_obj->ioh_obj;
    int rx_len;

    rx_len = ioh_recv(ioh_obj_p, timeout);
    if (rx_len < 0) {
       syslog(LOG_DEBUG, "receive timeout, rx_len=%d", rx_len);
       return ERROR_TIMOUT;
	}
    if(ib_obj->rx_header->rrcp_type != RRCP_P_IOH)
            return -1;

	if(idx != ib_obj->rx_header->inband_index) {
		printf("%s %d idx:%d != inband_index:%d, dropped!!\n",__FUNCTION__,__LINE__);
		syslog(LOG_DEBUG, "%s %d idx:%d != inband_index:%d, dropped!!\n",__FUNCTION__,__LINE__);
		return -1;
	}

    return rx_len;
}


static int send_frag_ack(struct inband_class *ib_obj)
{
	struct ioh_class *ioh_obj_p = &ib_obj->ioh_obj;
	
	ib_obj->tx_header->rrcp_type = RRCP_P_IOH;  //mark_inband
	ib_obj->tx_header->inband_cmd = ib_obj->rx_header->inband_cmd;
	ib_obj->tx_header->inband_seq = ib_obj->rx_header->inband_seq;
	ib_obj->tx_header->inband_frag = ib_obj->rx_header->inband_frag;
	ib_obj->tx_header->inband_data_len = 0;
	
	return ioh_send(ioh_obj_p,sizeof(struct inband_header)); //only send header for ack
}
static int check_frag_ack(struct inband_class *ib_obj)
{
	int rx_len;	
RetryRcvAck:	
	rx_len = inband_recv(ib_obj, PER_FRAME_TIMEOUT); // -1 = wait until rec
	if (rx_len < 0)
	{
		perror("check_frag_ack fail:");
        syslog(LOG_DEBUG, "check frag ack error");
		return 0;
    }	
	if((ib_obj->rx_header->inband_cmd == ib_obj->tx_header->inband_cmd) &&
		(ib_obj->rx_header->inband_seq == ib_obj->tx_header->inband_seq) &&
		(ib_obj->rx_header->inband_frag == ib_obj->tx_header->inband_frag) &&
		(ib_obj->rx_header->inband_data_len == 0))
		return 1; //ok
	if((ib_obj->rx_header->inband_cmd != ib_obj->tx_header->inband_cmd) ||
		(ib_obj->rx_header->inband_seq != ib_obj->tx_header->inband_seq) ||
		(ib_obj->rx_header->inband_frag != ib_obj->tx_header->inband_frag) ||
		(ib_obj->rx_header->inband_data_len != 0)){
		printf("Rcv worng ack ! rx_frag=%d tx_frag=%d \n",ntohs(ib_obj->rx_header->inband_frag),ntohs(ib_obj->tx_header->inband_frag));
		syslog(LOG_DEBUG, "Rcv worng ack ! rx_frag=%d tx_frag=%d \n",ntohs(ib_obj->rx_header->inband_frag),ntohs(ib_obj->tx_header->inband_frag));
		goto RetryRcvAck; // frag wrong
	}
	return 0; //fail
}
static char *inband_alloc_buf(struct inband_class *ib_obj)
{
	char *buf=NULL;
	unsigned int buf_size=MAX_APP_DATA_SIZE;	 
	unsigned char *image_len,header_len_offset=FM_HEADER_LEN_OFFSET;	 
#if 1 //it's for firmware file 	 
	if(ib_obj->rx_header->inband_cmd == id_firm_upgrade)
	{
#if 0
		//read firmware length from it's header
		image_len = (char *)ib_obj->rx_data;          
		buf_size =(unsigned int)( ( image_len[header_len_offset+0] <<24 ) +
				     ( image_len[header_len_offset+1] <<16 ) +
				     ( image_len[header_len_offset+2] <<8 )   +
				     ( image_len[header_len_offset+3] <<0 )   + 16 )	;						     	   

		//printf("inband_alloc_buf buf_size=%x , image_len[0]= %x\n",buf_size,image_len[header_len_offset+0]);	  
#endif
	 	buf_size = 0x800000; // using 8M for update firmware
	}    
#endif
#ifdef INBAND_GET_FILE_SUPPOPRT
       if(ib_obj->rx_header->inband_cmd == id_get_file )
       {
	        buf_size = 0x100000; // using 1M for get log file
       }	       
#endif
	buf = (char *)malloc(buf_size);	 
	return buf;	
}
//mark_issue,defragment_reset
static int init_defragment_process(struct inband_class *ib_obj)
{
	struct fragment_info *p_frag_info = &ib_obj->frag_info;
	
	//check if the it is first fragment  id
	if(ib_obj->rx_header->inband_frag != FIRST_FRAG_ID)
		return ERROR_DEFRAGMENT;

	if((p_frag_info->buf = (char *)inband_alloc_buf(ib_obj)) == NULL)
  	{
		printf("init_defragment_process : data buffer allocation failed!\n");
		syslog(LOG_DEBUG, "init_defragment_process : data buffer allocation failed!\n");
		return ERROR_DEFRAGMENT;
	}
	p_frag_info->buf_ptr = p_frag_info->buf;
	p_frag_info->inband_frag = FIRST_FRAG_ID;
	p_frag_info->inband_cmd = ib_obj->rx_header->inband_cmd;
	p_frag_info->inband_seq = ntohs(ib_obj->rx_header->inband_seq);
	
	//copy first frame to buffer
	memcpy(p_frag_info->buf,ib_obj->rx_data,ntohs(ib_obj->rx_header->inband_data_len));
	p_frag_info->buf += ntohs(ib_obj->rx_header->inband_data_len);
	p_frag_info->data_len = ntohs(ib_obj->rx_header->inband_data_len);
	send_frag_ack(ib_obj);

	return 0; //init ok
}

static int do_defragment_process(struct inband_class *ib_obj)
{
	int ret=0;
	struct fragment_info *p_frag_info = &ib_obj->frag_info;

	if(p_frag_info->inband_cmd !=  ib_obj->rx_header->inband_cmd){
		printf("Rcv pkt cmd fail! cmd1=%d cmd2=%d\n",p_frag_info->inband_cmd, ib_obj->rx_header->inband_cmd);
		syslog(LOG_DEBUG, "Rcv pkt cmd fail! cmd1=%d cmd2=%d\n",p_frag_info->inband_cmd, ib_obj->rx_header->inband_cmd);
		return ERROR_DECMDTYPE;
	}	
	if(p_frag_info->inband_seq !=  ntohs(ib_obj->rx_header->inband_seq) ){
		printf("Rcv pkt seq fail! seq1=%d seq2=%d\n",p_frag_info->inband_seq, ntohs(ib_obj->rx_header->inband_seq));
		syslog(LOG_DEBUG, "Rcv pkt seq fail! seq1=%d seq2=%d\n",p_frag_info->inband_seq, ntohs(ib_obj->rx_header->inband_seq));
		return ERROR_DESEQTYPE;
	}
	if(p_frag_info->inband_frag == (ntohs(ib_obj->rx_header->inband_frag) & FRAG_ID_MASK)){
		printf("Rcv pkt Same frag! frag=%d\n",p_frag_info->inband_frag);
		printf("cmd=%x len=%d\n",p_frag_info->inband_cmd,p_frag_info->data_len);
		syslog(LOG_DEBUG, "Rcv pkt Same frag! frag=%d\n",p_frag_info->inband_frag);
		syslog(LOG_DEBUG, "cmd=%x len=%d\n",p_frag_info->inband_cmd,p_frag_info->data_len);
		return ret;
	}
	if( (p_frag_info->inband_frag+1) !=  (ntohs(ib_obj->rx_header->inband_frag) & FRAG_ID_MASK)){
		printf("Rcv pkt frag fail! seq1=%d seq2=%d\n",p_frag_info->inband_frag, (ntohs(ib_obj->rx_header->inband_frag) & FRAG_ID_MASK));
		syslog(LOG_DEBUG, "Rcv pkt frag fail! seq1=%d seq2=%d\n",p_frag_info->inband_frag, (ntohs(ib_obj->rx_header->inband_frag) & FRAG_ID_MASK));
		return ERROR_DEFRAGMENT;
	}
	else
		p_frag_info->inband_frag = ntohs(ib_obj->rx_header->inband_frag);

	memcpy(p_frag_info->buf,ib_obj->rx_data,ntohs(ib_obj->rx_header->inband_data_len));
	p_frag_info->buf += (ntohs(ib_obj->rx_header->inband_data_len));
	p_frag_info->data_len+= (ntohs(ib_obj->rx_header->inband_data_len));

	if( (p_frag_info->inband_frag & EOF_BIT) == EOF_BIT)
		ret = 1;  
	
	return ret; // return 1 means EOF rcv , return 0 means conitiune
}

static int get_defragment_info(struct inband_class *ib_obj,char *cmd_type,char **data)
{
	struct fragment_info *p_frag_info = &ib_obj->frag_info;
	
	*cmd_type = p_frag_info->inband_cmd;
	*data = p_frag_info->buf_ptr; //mark_issue,hwo to free the buffer ?
	return p_frag_info->data_len;
}

static int inband_rcv_fragment(struct inband_class *ib_obj,char *cmd_type,char **data)
{
	int rx_len,ret=0;		

	ret = init_defragment_process(ib_obj) ;
	int rerx_count = 0;

	if(ret < 0)
		return ret;	
	
	while( (inband_rcv_timeout!=1))
	{
	RetryRx:		
		rx_len = inband_recv(ib_obj, PER_FRAME_TIMEOUT);
		if (rx_len < 0){
			printf("Rcv pkt timeout! retry rx count=%d\n",rerx_count);
			syslog(LOG_DEBUG, "Rcv pkt timeout! retry rx count=%d\n",rerx_count);
			if(rerx_count++ < RETRY_TX_RX_COUNT)
				goto RetryRx;
			else	
				return ERROR_TIMOUT;
		}
		ret = do_defragment_process(ib_obj);
		if (ret < 0){
			if(ret == ERROR_DECMDTYPE || ret == ERROR_DESEQTYPE)
				goto RetryRx;
			else	
			return ret;
		}
		else if(ret == 1) //ret == 1 means defragment end , ret=0 means contiune			
		{
			ret = get_defragment_info(ib_obj,cmd_type,data);
			break;
		}	
		send_frag_ack(ib_obj);
		rerx_count = 0;
	}		
	return ret;
}

void inband_free_buf(char *data_buf,int data_len)
{
	//only free allocated buffer from deframet process.
	if( (data_buf != NULL) && ( data_len > MAX_INBAND_PAYLOAD_LEN ) )
		free(data_buf);
}

int inband_rcv_data(int chan,char *cmd_type,char **data,int timout_ms) //return data length
{	
	int rx_len,data_len=0;
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;
	int retry_count = 0;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	ioh_obj_p = &inband_obj_p->ioh_obj;	

	//timout_ms will be used only for the first pkt. if the pkt is fragmented then every packet will
	//follow fragment_timout_ms
RetryRxFirstPkt:	
	rx_len = inband_recv(inband_obj_p, timout_ms); // -1 = wait until rec
	if (rx_len < 0)
	{
        syslog(LOG_DEBUG, "inband receive data error!");
		perror("inband_rcv_data:");
		return -1;
    }
	//printf("inband_rcv_data:\n");
	//hex_dump(ioh_obj_p->rx_buffer, ntohs(inband_obj_p->rx_header->inband_data_len) + sizeof(*inband_obj_p->rx_header)); //mark_test		

	//cache for tx dest mac
	if( memcmp(ioh_obj_p->dest_mac,ioh_obj_p->rx_header->sa,6)) //mark_test
		memcpy(ioh_obj_p->dest_mac,ioh_obj_p->rx_header->sa,6);

	//check if the rx packet is mine,not other inband demon's rx packet ( in hcd the cmd is 0)
	if((inband_obj_p->frag_info.inband_cmd != 0) && (((inband_obj_p->rx_header->inband_cmd) & (~CMD_ERROR_REPLY_BIT)) != inband_obj_p->frag_info.inband_cmd))
	{
		//printf("Inband RetryRxFirstPkt cmd1=%d cmd2=%d!\n",inband_obj_p->rx_header->inband_cmd,inband_obj_p->frag_info.inband_cmd);
		syslog(LOG_DEBUG, "Inband RetryRxFirstPkt cmd1=%d cmd2=%d!\n",inband_obj_p->rx_header->inband_cmd,inband_obj_p->frag_info.inband_cmd);
		if(retry_count++ < RETRY_TX_RX_COUNT)
			goto RetryRxFirstPkt;
		else 
			return -1;
	}
	else if((inband_obj_p->frag_info.inband_cmd != 0) && (ntohs(inband_obj_p->rx_header->inband_seq) != inband_obj_p->frag_info.inband_seq))
	{
		printf("Inband RetryRxSignalPkt seq1=%d seq2=%d!\n",ntohs(inband_obj_p->rx_header->inband_seq),inband_obj_p->frag_info.inband_seq);
		syslog(LOG_DEBUG, "Inband RetryRxSignalPkt seq1=%d seq2=%d!\n",ntohs(inband_obj_p->rx_header->inband_seq),inband_obj_p->frag_info.inband_seq);
		if(retry_count++ < RETRY_TX_RX_COUNT)
			goto RetryRxFirstPkt;
		else 
			return -1;
	}
	//single pkt	
	if( inband_obj_p->rx_header->inband_frag  == ntohs(SINGLE_FRAME)) //mark_endian
	{		
        //printf("[%s:%d]inband_obj_p->rx_header->inband_cmd = %d \n", __FUNCTION__, __LINE__,inband_obj_p->rx_header->inband_cmd);
		*cmd_type = inband_obj_p->rx_header->inband_cmd;
		data_len =  ntohs(inband_obj_p->rx_header->inband_data_len);		
		*data = inband_obj_p->rx_data ; //or memcpy;	
		if(data_len > rx_len){
			printf("Rcv more than one packet len1=%d len2=%d!",data_len,rx_len);
			syslog(LOG_DEBUG, "Rcv more than one packet len1=%d len2=%d!",data_len,rx_len);
        }
	}
	else //fragment process
		data_len = inband_rcv_fragment(inband_obj_p,cmd_type,data);

	return data_len;
	
}

int inband_rcv_indexed_data(int chan,char *cmd_type,char **data,int timout_ms,unsigned char idx) //return data length
{	
	int rx_len,data_len=0;
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	ioh_obj_p = &inband_obj_p->ioh_obj;	

	
	//timout_ms will be used only for the first pkt. if the pkt is fragmented then every packet will
	//follow fragment_timout_ms
	//rx_len = inband_recv(inband_obj_p, timout_ms); // -1 = wait until rec
	rx_len = inband_recv(inband_obj_p, timout_ms); // -1 = wait until rec
	if (rx_len < 0)
	{
		perror("inband_rcv_data:");
		return -1;
    }
	//printf("inband_rcv_data:\n");
	//hex_dump(ioh_obj_p->rx_buffer, ntohs(inband_obj_p->rx_header->inband_data_len) + sizeof(*inband_obj_p->rx_header)); //mark_test		

	if( idx != inband_obj_p->rx_header->inband_index ) {
		printf("Error: if_index:%d != %d \n",idx,inband_obj_p->rx_header->inband_index);
		syslog(LOG_DEBUG, "Error: if_index:%d != %d \n",idx,inband_obj_p->rx_header->inband_index);
		return -1;
	}

	//cache for tx dest mac
	if( memcmp(ioh_obj_p->dest_mac,ioh_obj_p->rx_header->sa,6)) //mark_test
		memcpy(ioh_obj_p->dest_mac,ioh_obj_p->rx_header->sa,6);

	//single pkt	
	if( inband_obj_p->rx_header->inband_frag  == ntohs(SINGLE_FRAME)) //mark_endian
	{		
		*cmd_type = inband_obj_p->rx_header->inband_cmd;
		data_len =  ntohs(inband_obj_p->rx_header->inband_data_len);		
		*data = inband_obj_p->rx_data ; //or memcpy;	
	}
	else //fragment process
		data_len = inband_rcv_fragment(inband_obj_p,cmd_type,data);

	return data_len;
	
}


//if seq is need in your application
int inband_rcv_data_and_seq(int chan,unsigned int *seq,char *cmd_type,char **data,int timout_ms) //return data length
{
	struct inband_class *inband_obj_p;
	int ret=0;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	ret = inband_rcv_data(chan,cmd_type,data, timout_ms); //return data length
	if(ret < 0)
		return -1;

	*seq = ntohs(inband_obj_p->rx_header->inband_seq);
	return ret;
}

static int inband_send_data(struct inband_class *ib_obj,char *data,int data_len)
{	
	char *frag_ptr;
	unsigned short id=0,total_frag=0;
	unsigned int last_num;
	struct ioh_class *ioh_obj_p = &ib_obj->ioh_obj;
	int retx_count=0;

	total_frag = (unsigned short)(data_len / MAX_INBAND_PAYLOAD_LEN);
		
	if( total_frag > MAX_FRAG_ID)
		return -1;

	ib_obj->tx_header->inband_frag =0;
	frag_ptr = data;

	for(id=0;id<total_frag;id++)
	{
		//if(total_frag > 0)
			//hex2bin("ffffffffffff", ioh_obj_p->dest_mac, ETH_MAC_LEN); 
		ib_obj->tx_header->inband_data_len = htons(MAX_INBAND_PAYLOAD_LEN);
		ib_obj->tx_header->inband_frag = htons(id );
		memcpy(&ib_obj->tx_data[0], frag_ptr,MAX_INBAND_PAYLOAD_LEN );	
	RetryTX:	
		if( ioh_send(ioh_obj_p, sizeof(struct inband_header) + MAX_INBAND_PAYLOAD_LEN ) < 0){
			printf("Tx fail! \n");
			syslog(LOG_DEBUG, "Tx fail! \n");
			return -1;
		}
		//if(id>= 1){
		if(check_frag_ack(ib_obj) != 1){
			if(retx_count++ < RETRY_TX_RX_COUNT){
				printf("Retry Tx pkt! id=%d\n",id);
				syslog(LOG_DEBUG, "Retry Tx pkt! id=%d\n",id);
				goto RetryTX;
			}
			else {	
				return -1;	
			}
		}
		//}	
		frag_ptr += MAX_INBAND_PAYLOAD_LEN;
		retx_count = 0;
	}
	last_num = data_len % MAX_INBAND_PAYLOAD_LEN;
	//EOF fragment	
	ib_obj->tx_header->inband_frag = id;
	ib_obj->tx_header->inband_frag |=EOF_BIT;
	ib_obj->tx_header->inband_frag = htons(ib_obj->tx_header->inband_frag);
	ib_obj->tx_header->inband_data_len = htons(last_num);
	if(last_num >0)
		memcpy(&ib_obj->tx_data[0], frag_ptr,last_num );

	return ioh_send(ioh_obj_p,sizeof(struct inband_header)+last_num);		
}

static int inband_send_indexed_data(struct inband_class *ib_obj,char *data, unsigned char idx, int data_len)
{	
	char *frag_ptr;
	unsigned short id=0,total_frag=0;
	unsigned int last_num;
	struct ioh_class *ioh_obj_p = &ib_obj->ioh_obj;

	total_frag = (unsigned short)(data_len / MAX_INBAND_PAYLOAD_LEN);
		
	if( total_frag > MAX_FRAG_ID)
		return -1;

	ib_obj->tx_header->inband_frag =0;
	frag_ptr = data;

	for(id=0;id<total_frag;id++)
	{
		ib_obj->tx_header->inband_data_len = htons(MAX_INBAND_PAYLOAD_LEN);
		ib_obj->tx_header->inband_frag = htons(id );
		memcpy(&ib_obj->tx_data[0], frag_ptr,MAX_INBAND_PAYLOAD_LEN );		
		if( ioh_send(ioh_obj_p, sizeof(struct inband_header) + MAX_INBAND_PAYLOAD_LEN ) < 0)
			return -1;
		//if(id>= 1){
		if(check_frag_ack(ib_obj) != 1)
			return -1;			
		//}	
		frag_ptr += MAX_INBAND_PAYLOAD_LEN;
	}
	last_num = data_len % MAX_INBAND_PAYLOAD_LEN;
	//EOF fragment	
	ib_obj->tx_header->inband_frag = id;
	ib_obj->tx_header->inband_frag |=EOF_BIT;
	ib_obj->tx_header->inband_frag = htons(ib_obj->tx_header->inband_frag);
	ib_obj->tx_header->inband_data_len = htons(last_num);
	if(last_num >0)
		memcpy(&ib_obj->tx_data[0], frag_ptr,last_num );

	ib_obj->tx_header->inband_index = idx;

	return ioh_send(ioh_obj_p,sizeof(struct inband_header)+last_num);		
}


int inband_write(int chan,unsigned int seq,char cmd,char *data,int data_len,int reply)
{
	struct inband_class *inband_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	

	inband_obj_p->tx_header->rrcp_type = RRCP_P_IOH;  //mark_inband
	//fill inband header , cmd
	inband_obj_p->tx_header->inband_cmd = cmd;

	//reply = 0(request) ,reply = 1(good reply),reply = 2(bad reply)
	if(reply == 2)
		inband_obj_p->tx_header->inband_cmd |= CMD_ERROR_REPLY_BIT;

	//fill inband header , seq
	if(!reply) 
		inband_obj_p->tx_header->inband_seq = htons(seq);
	else //seq is not used when the packet is for reply
		inband_obj_p->tx_header->inband_seq = inband_obj_p->rx_header->inband_seq;	

#if defined(CONFIG_REPORT_ERROR)
    // for inband access
    int written = 0;
    int status;
    int length;
    int total_len;
    if(reply==1){       //good reply
        status = 0;
        length = data_len;
        written = 0;
        total_len = 4+4+64+length;
        char result[total_len];
        memset(result, 0x0, total_len);

        memcpy(result+written, &status, 4);
        written += 4;
        memcpy(result+written, &length, 4);
        written += 4 + 64; //64 bytes error_info is empty now.

        memcpy(result+written, data, data_len);
        written += data_len;

        return inband_send_data(inband_obj_p, result,total_len);
    }else if(reply==2){
        status = 1;
        length = 4;
        written = 0;
        total_len = 4+4+64+length;
        char result[total_len];
        memset(result, 0x0, total_len);

        memcpy(result+written, &status, 4);
        written += 4;
        memcpy(result+written, &length, 4);
        written += 4;
        memcpy(result+written, data, (data_len>=64)?64:data_len); //max 64 bytes
        written += 64;

        memcpy(result+written, 0x0, 4);
        written += 4;

        return inband_send_data(inband_obj_p, result,total_len);
    }
#endif
	return inband_send_data(inband_obj_p,data,data_len);
}

int inband_indexed_write(int chan,unsigned int seq,char cmd,char *data,int data_len,int reply,unsigned char idx)
{
	struct inband_class *inband_obj_p;

	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	

	inband_obj_p->tx_header->rrcp_type = RRCP_P_IOH;  //mark_inband
	//fill inband header , cmd
	inband_obj_p->tx_header->inband_cmd = cmd;

	//reply = 0(request) ,reply = 1(good reply),reply = 2(bad reply)
	if(reply == 2)
		inband_obj_p->tx_header->inband_cmd |= CMD_ERROR_REPLY_BIT;

	//fill inband header , seq
	if(!reply) 
		inband_obj_p->tx_header->inband_seq = htons(seq);
	else //seq is not used when the packet is for reply
		inband_obj_p->tx_header->inband_seq = inband_obj_p->rx_header->inband_seq;	

	inband_obj_p->tx_header->inband_index = idx;
	
	//fill data, data_len , and send
	return inband_send_indexed_data(inband_obj_p,data,idx,data_len);
}

void inband_set_cmd_id_zero(int chan)
{
	struct inband_class *inband_obj_p;
	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	if(inband_obj_p != NULL)
		inband_obj_p->frag_info.inband_cmd = 0;
}

void inband_set_cmd_seq(int chan,int cmd_id,int seq)
{
	struct inband_class *inband_obj_p;
	struct ioh_class *ioh_obj_p;
	inband_obj_p = (struct inband_class *)get_chan_obj(chan);	
	inband_obj_p->frag_info.inband_cmd = cmd_id;
	inband_obj_p->frag_info.inband_seq = seq;
}
