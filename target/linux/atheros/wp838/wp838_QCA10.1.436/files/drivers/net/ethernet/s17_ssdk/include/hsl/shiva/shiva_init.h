/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
/**
 * @defgroup shiva_init SHIVA_INIT
 * @{
 */
#ifndef _SHIVA_INIT_H_
#define _SHIVA_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "init/ssdk_init.h"


    sw_error_t
    shiva_init(a_uint32_t dev_id, ssdk_init_cfg *cfg);


    sw_error_t
    shiva_cleanup(a_uint32_t dev_id);

#ifdef HSL_STANDALONG

    HSL_LOCAL sw_error_t
    shiva_reset(a_uint32_t dev_id);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHIVA_INIT_H_ */
/**
 * @}
 */
