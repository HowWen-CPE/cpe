/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */

#include "sw.h"
#include "sw_ioctl.h"
#include "fal_stp.h"
#include "fal_uk_if.h"

sw_error_t
fal_stp_port_state_set(a_uint32_t dev_id, a_uint32_t st_id,
                       fal_port_t port_id, fal_stp_state_t state)
{
    sw_error_t rv;

    rv = sw_uk_exec(SW_API_STP_PT_STATE_SET, dev_id, st_id, port_id,
                    (a_uint32_t) state);
    return rv;
}

sw_error_t
fal_stp_port_state_get(a_uint32_t dev_id, a_uint32_t st_id,
                       fal_port_t port_id, fal_stp_state_t * state)
{
    sw_error_t rv;

    rv = sw_uk_exec(SW_API_STP_PT_STATE_GET, dev_id, st_id, port_id,
                    (a_uint32_t) state);
    return rv;
}
