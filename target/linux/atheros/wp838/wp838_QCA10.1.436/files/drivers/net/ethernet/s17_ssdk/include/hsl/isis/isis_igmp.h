/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
/**
 * @defgroup isis_igmp ISIS_IGMP
 * @{
 */
#ifndef _ISIS_IGMP_H_
#define _ISIS_IGMP_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_igmp.h"

    sw_error_t
    isis_igmp_init(a_uint32_t dev_id);

#ifdef IN_IGMP
#define ISIS_IGMP_INIT(rv, dev_id) \
    { \
        rv = isis_igmp_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
#define ISIS_IGMP_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG

    HSL_LOCAL sw_error_t
    isis_port_igmps_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_igmps_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t *enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_cmd_set(a_uint32_t dev_id, fal_fwd_cmd_t cmd);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_cmd_get(a_uint32_t dev_id, fal_fwd_cmd_t * cmd);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_join_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_join_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_leave_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_leave_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_rp_set(a_uint32_t dev_id, fal_pbmp_t pts);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_rp_get(a_uint32_t dev_id, fal_pbmp_t * pts);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_creat_set(a_uint32_t dev_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_creat_get(a_uint32_t dev_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_static_set(a_uint32_t dev_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_static_get(a_uint32_t dev_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_leaky_set(a_uint32_t dev_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_leaky_get(a_uint32_t dev_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_v3_set(a_uint32_t dev_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_v3_get(a_uint32_t dev_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_queue_set(a_uint32_t dev_id, a_bool_t enable, a_uint32_t queue);


    HSL_LOCAL sw_error_t
    isis_igmp_mld_entry_queue_get(a_uint32_t dev_id, a_bool_t * enable, a_uint32_t * queue);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_learn_limit_set(a_uint32_t dev_id, fal_port_t port_id,
                                       a_bool_t enable, a_uint32_t cnt);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_learn_limit_get(a_uint32_t dev_id, fal_port_t port_id,
                                       a_bool_t * enable, a_uint32_t * cnt);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_learn_exceed_cmd_set(a_uint32_t dev_id, fal_port_t port_id,
                                            fal_fwd_cmd_t cmd);


    HSL_LOCAL sw_error_t
    isis_port_igmp_mld_learn_exceed_cmd_get(a_uint32_t dev_id, fal_port_t port_id,
                                            fal_fwd_cmd_t * cmd);


#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* _ISIS_IGMP_H_ */
/**
 * @}
 */
