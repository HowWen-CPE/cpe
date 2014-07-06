/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
#include <stdarg.h>
#include "sw.h"
#include "ssdk_init.h"
#include "sw_api.h"
#include "sw_api_us.h"
#include "api_access.h"

sw_error_t
sw_uk_exec(a_uint32_t api_id, ...)
{
    a_uint32_t value[SW_MAX_API_PARAM] = { 0 };
    a_uint32_t rtn = SW_OK, i;
    sw_error_t rv;
    va_list arg_ptr;
    a_uint32_t nr_param = 0;

    if((nr_param = sw_api_param_nums(api_id)) == 0)
    {
        return SW_NOT_SUPPORTED;
    }

    value[0] = api_id;
    value[1] = (a_uint32_t)&rtn;

    va_start(arg_ptr, api_id);
    for (i = 0; i < nr_param; i++)
    {
        value[i + 2] = va_arg(arg_ptr, a_uint32_t);
    }
    va_end(arg_ptr);

    rv = sw_uk_if(value);
    if (SW_OK != rv)
    {
        return rv;
    }
    return rtn;
}

sw_error_t
ssdk_init(a_uint32_t dev_id, ssdk_init_cfg * cfg)
{
    sw_error_t rv;

    rv = sw_uk_init(cfg->nl_prot);
    return rv;
}

sw_error_t
ssdk_cleanup(void)
{
    sw_error_t rv;

    rv = sw_uk_cleanup();
    return rv;
}

