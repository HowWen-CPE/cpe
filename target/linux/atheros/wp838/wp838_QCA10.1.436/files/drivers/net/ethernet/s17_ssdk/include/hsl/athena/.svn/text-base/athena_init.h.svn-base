/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */

#ifndef _ATHENA_INIT_H_
#define _ATHENA_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "init/ssdk_init.h"

    sw_error_t
    athena_init(a_uint32_t dev_id, ssdk_init_cfg * cfg);

    sw_error_t
    athena_cleanup(a_uint32_t dev_id);

#ifdef HSL_STANDALONG

    HSL_LOCAL sw_error_t
    athena_reset(a_uint32_t dev_id);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATHENA_INIT_H_ */


