/*
  *   cmd header for 8198
  *
  *	Copyright (C)2008, Realtek Semiconductor Corp. All rights reserved.
  *
  *	$Id: cmd.h,v 1.5 2010/12/29 09:58:22 brian Exp $
  */

#ifndef INCLUDE_CMD_H
#define INCLUDE_CMD_H

#define LAST_ENTRY_ID 0 //LAST_ENTRY_ID

//command ID
#define id_ioctl						0x0b

#define MAX_HOST_CMD_LEN  65536
#define MAX_PATH_LEN 256
#define CMD_DEF(name, func) \
	{id_##name, #name,  func}

typedef struct{
    unsigned int thread_id;
    unsigned int cmd_type;
	unsigned int status;	//0:SUCCESS, 1:FAIL, 2:buf overflow
	unsigned int length;	//the buf length of return value 
	unsigned char error_info[64];
}RTK_INBAND_RESULT_HEADER,*pRTK_INBAND_RESULT_HEADER;

typedef struct{
	RTK_INBAND_RESULT_HEADER header;
	unsigned char *buf;
}RTK_INBAND_RESULT, *pRTK_INBAND_RESULT;


// CMD table for action
struct cmd_entry {                          
	int 		id;				// cmd id
	char 	*name;		// cmd name in string                 			
	int		 (*func)(char *data,int len);   //the fuction for act 
};           	


int do_cmd(int id , char *cmd ,int cmd_len ,int relply);


#endif 
