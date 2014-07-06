/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */

#ifndef _SW_CONFIG_H
#define _SW_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SW_MAX_NR_DEV      1
#define SW_MAX_NR_PORT     32

#ifdef HSL_STANDALONG
#define HSL_LOCAL
#else
#define HSL_LOCAL static
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
