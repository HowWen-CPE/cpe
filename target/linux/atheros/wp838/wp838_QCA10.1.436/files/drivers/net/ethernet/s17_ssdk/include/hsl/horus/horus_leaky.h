/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
/**
 * @defgroup horus_leaky HORUS_LEAKY
 * @{
 */
#ifndef _HORUS_LEAKY_H_
#define _HORUS_LEAKY_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_leaky.h"

    sw_error_t horus_leaky_init(a_uint32_t dev_id);

#ifdef IN_LEAKY
#define HORUS_LEAKY_INIT(rv, dev_id) \
    { \
        rv = horus_leaky_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
#define HORUS_LEAKY_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG


    HSL_LOCAL sw_error_t
    horus_uc_leaky_mode_set(a_uint32_t dev_id,
                            fal_leaky_ctrl_mode_t ctrl_mode);


    HSL_LOCAL sw_error_t
    horus_uc_leaky_mode_get(a_uint32_t dev_id,
                            fal_leaky_ctrl_mode_t * ctrl_mode);


    HSL_LOCAL sw_error_t
    horus_mc_leaky_mode_set(a_uint32_t dev_id,
                            fal_leaky_ctrl_mode_t ctrl_mode);


    HSL_LOCAL sw_error_t
    horus_mc_leaky_mode_get(a_uint32_t dev_id,
                            fal_leaky_ctrl_mode_t * ctrl_mode);


    HSL_LOCAL sw_error_t
    horus_port_arp_leaky_set(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t enable);


    HSL_LOCAL sw_error_t
    horus_port_arp_leaky_get(a_uint32_t dev_id, fal_port_t port_id,
                             a_bool_t * enable);


    HSL_LOCAL sw_error_t
    horus_port_uc_leaky_set(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t enable);


    HSL_LOCAL sw_error_t
    horus_port_uc_leaky_get(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t * enable);


    HSL_LOCAL sw_error_t
    horus_port_mc_leaky_set(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t enable);


    HSL_LOCAL sw_error_t
    horus_port_mc_leaky_get(a_uint32_t dev_id, fal_port_t port_id,
                            a_bool_t * enable);
#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _HORUS_LEAKY_H_ */
/**
 * @}
 */
