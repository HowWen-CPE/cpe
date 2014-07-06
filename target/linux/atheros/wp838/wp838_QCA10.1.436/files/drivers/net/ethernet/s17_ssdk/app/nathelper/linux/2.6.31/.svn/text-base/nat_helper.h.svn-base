/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
#ifndef _NAT_HELPER_H
#define _NAT_HELPER_H

#include "sw.h"

#define NAPT_TABLE_SIZE 1024

void host_helper_init(void);
void host_helper_exit(void);
void  napt_helper_init(void);
void napt_helper_exit(void);
void nat_ipt_helper_init(void);
void nat_ipt_helper_exit(void);

//#define HNAT_DEBUG 1

#ifdef HNAT_DEBUG
#define HNAT_PRINTK(x...) aos_printk(x)
#else
#define HNAT_PRINTK(x...)
#endif

//#define HNAT_ERR_DEBUG 1

#ifdef HNAT_ERR_DEBUG
#define HNAT_ERR_PRINTK(x...) aos_printk(x)
#else
#define HNAT_ERR_PRINTK(x...)
#endif



#endif
