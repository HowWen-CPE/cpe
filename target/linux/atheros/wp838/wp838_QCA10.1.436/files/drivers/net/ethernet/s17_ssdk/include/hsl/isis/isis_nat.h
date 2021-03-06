/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
#ifndef _ISIS_NAT_H_
#define _ISIS_NAT_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_nat.h"

    sw_error_t isis_nat_init(a_uint32_t dev_id);

    sw_error_t isis_nat_reset(a_uint32_t dev_id);

#ifdef IN_NAT
#define ISIS_NAT_INIT(rv, dev_id) \
    { \
        rv = isis_nat_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }

#define ISIS_NAT_RESET(rv, dev_id) \
    { \
        rv = isis_nat_reset(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
#define ISIS_NAT_INIT(rv, dev_id)
#define ISIS_NAT_RESET(rv, dev_id)
#endif

#ifdef HSL_STANDALONG

    HSL_LOCAL sw_error_t
    isis_nat_add(a_uint32_t dev_id, fal_nat_entry_t * nat_entry);

    HSL_LOCAL sw_error_t
    isis_nat_del(a_uint32_t dev_id, a_uint32_t del_mode,
                 fal_nat_entry_t * nat_entry);

    HSL_LOCAL sw_error_t
    isis_nat_get(a_uint32_t dev_id, a_uint32_t get_mode,
                 fal_nat_entry_t * nat_entry);

    HSL_LOCAL sw_error_t
    isis_nat_next(a_uint32_t dev_id, a_uint32_t next_mode,
                  fal_nat_entry_t * nat_entry);

    HSL_LOCAL sw_error_t
    isis_nat_counter_bind(a_uint32_t dev_id, a_uint32_t entry_id,
                          a_uint32_t cnt_id, a_bool_t enable);

    HSL_LOCAL sw_error_t
    isis_napt_add(a_uint32_t dev_id, fal_napt_entry_t * napt_entry);

    HSL_LOCAL sw_error_t
    isis_napt_del(a_uint32_t dev_id, a_uint32_t del_mode,
                  fal_napt_entry_t * napt_entry);

    HSL_LOCAL sw_error_t
    isis_napt_get(a_uint32_t dev_id, a_uint32_t get_mode,
                  fal_napt_entry_t * napt_entry);

    HSL_LOCAL sw_error_t
    isis_napt_next(a_uint32_t dev_id, a_uint32_t next_mode,
                   fal_napt_entry_t * napt_entry);

    HSL_LOCAL sw_error_t
    isis_napt_counter_bind(a_uint32_t dev_id, a_uint32_t entry_id,
                           a_uint32_t cnt_id, a_bool_t enable);

    HSL_LOCAL sw_error_t
    isis_nat_status_set(a_uint32_t dev_id, a_bool_t enable);

    HSL_LOCAL sw_error_t
    isis_nat_status_get(a_uint32_t dev_id, a_bool_t * enable);

    HSL_LOCAL sw_error_t
    isis_nat_hash_mode_set(a_uint32_t dev_id, a_uint32_t mode);

    HSL_LOCAL sw_error_t
    isis_nat_hash_mode_get(a_uint32_t dev_id, a_uint32_t * mode);

    HSL_LOCAL sw_error_t
    isis_napt_status_set(a_uint32_t dev_id, a_bool_t enable);

    HSL_LOCAL sw_error_t
    isis_napt_status_get(a_uint32_t dev_id, a_bool_t * enable);

    HSL_LOCAL sw_error_t
    isis_napt_mode_set(a_uint32_t dev_id, fal_napt_mode_t mode);

    HSL_LOCAL sw_error_t
    isis_napt_mode_get(a_uint32_t dev_id, fal_napt_mode_t * mode);

    HSL_LOCAL sw_error_t
    isis_nat_prv_base_addr_set(a_uint32_t dev_id, fal_ip4_addr_t addr);

    HSL_LOCAL sw_error_t
    isis_nat_prv_base_addr_get(a_uint32_t dev_id, fal_ip4_addr_t * addr);

    HSL_LOCAL sw_error_t
    isis_nat_prv_addr_mode_set(a_uint32_t dev_id, a_bool_t map_en);

    HSL_LOCAL sw_error_t
    isis_nat_prv_addr_mode_get(a_uint32_t dev_id, a_bool_t * map_en);

    HSL_LOCAL sw_error_t
    isis_nat_pub_addr_add(a_uint32_t dev_id, fal_nat_pub_addr_t * entry);

    HSL_LOCAL sw_error_t
    isis_nat_pub_addr_del(a_uint32_t dev_id, a_uint32_t del_mode,
                          fal_nat_pub_addr_t * entry);

    HSL_LOCAL sw_error_t
    isis_nat_pub_addr_next(a_uint32_t dev_id, a_uint32_t next_mode,
                           fal_nat_pub_addr_t * entry);

    HSL_LOCAL sw_error_t
    isis_nat_unk_session_cmd_set(a_uint32_t dev_id, fal_fwd_cmd_t cmd);

    HSL_LOCAL sw_error_t
    isis_nat_unk_session_cmd_get(a_uint32_t dev_id, fal_fwd_cmd_t * cmd);

    HSL_LOCAL sw_error_t
    isis_nat_psr_prv_base_addr_set(a_uint32_t dev_id, fal_ip4_addr_t addr);

    HSL_LOCAL sw_error_t
    isis_nat_psr_prv_base_addr_get(a_uint32_t dev_id, fal_ip4_addr_t * addr);

#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _ISIS_NAT_H_ */
