/* 
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */

#if defined(CONFIG_FLOWMAC) || defined(CONFIG_FLOWMAC_MODULE)
#include "flowmac_api.h"
#include "athrs_flowmac.h"
#include "athrs_mac.h"

/*
 * Function: athr_gmac_rx_pause
 *
 * Pause the RX for the duration.
 *
 * net_device: the pointer to this device
 * pause:      whether to puase or resume. If the PAUSE is specified, pause
 *             action can be supplied with a time for which time the pause
 *             need to be done. In such a case, start a timer, if the resume
 *             is not hapenning within that duration, do auto resume. If
 *             duration is specified is zero, it is indifinite pause and
 *             some one need to unpause this. 
 *             ** TODO timer based recovery for pause TODO *8
 *  duration: for what time the pause is required. 
 */
int8_t
athr_gmac_rx_pause(struct net_device *dev, u_int8_t pause, u_int32_t duration)
{
    athr_gmac_t *mac = NULL;

    if (!dev) return -EINVAL;

    mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);
    if (pause && mac->mac_flowcntrl_enable)
    {
        /* stall the rx for this Ethernet device */
        athr_gmac_rx_stop(mac);
    } else {
        athr_gmac_rx_start(mac);
    }
    return 0;
}
/*
 * Function: athr_gmac_rx_rate_limit
 *
 * dev: Pointer to net_device, to access the mac structure. 
 * dir: The direction in which rate need to be limited. In most practical
 *      scenario with Ethernet, it always in rx side, i.e., ingress. There
 *      would be occassion ( could there be ? ) where egress rate can be
 *      modified, for example, the wifi driver can be chagned from auto rate
 *      control to manual rate control. Right now not defining any thing
 *      here and leaving all details in void pointer. 
 * rate: Pointer to rate structure, understood by only acting driver. Unless
 *       we define a common rate codes, it is not going to be of much use.
 */
int8_t
athr_gmac_rx_rate_limit(struct net_device *dev, flow_dir_t dir, void *rate)
{

    athr_gmac_t *mac = NULL;
    mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);

    if (!dev) return 1;
    if (dir != DIR_INGRESS) return 1;
    if(!mac->mac_flowcntrl_enable)    return 1;

    /* add any necessary rate limiting code here */
    return 0;
}
/*
 * act any of the notifications from the flowmac driver 
 * No specific events are coded as on now
 *
 * Enable: Enable the flow control on the current device
 * Disable: Disable the flow control on current device, expecting to enable
 *          later
 *          - no self cleanup
 * Down:    For some reason, flowmac or one of itse service providers are
 *          going out. No more flow control operations possible.
 *          - self cleanup need to be done
 */
int
athr_gmac_notify_event (struct net_device *dev, flowmac_event_t event,
        void *event_data)
{

    athr_gmac_t *mac;

    if (!dev) return -EINVAL;

    mac = (athr_gmac_t*)ATHR_MAC_PRIV(dev);
    
    switch (event) {
        case FLOWMAC_ENABLE:
            printk("Flow control on device %p is enabled \n", mac);
            mac->mac_flowcntrl_enable = 1;
            break;
        case FLOWMAC_DISABLE:
            printk("Flow control on device %p is disabled \n", mac);
            mac->mac_flowcntrl_enable = 0;
            break;
        case FLOWMAC_DOWN:
            /* make sure that spin lock is released before this cleanup 
             * ------- MAKE SURE CLEANUP HAPPENS PEROPERLY ---- 
             */
            printk("Down - doing the clean up - assuming the flowmac to do kfree\n");
            mac->mac_flowcntrl_enable = 0;
            mac->flow_mac_handle = NULL;
            break;
        default:
            return -1;
    }

    return 0;
}

flow_ops_t athr_gmac_flow_ops = { athr_gmac_rx_pause, athr_gmac_rx_rate_limit,
    athr_gmac_notify_event };
 
int
athr_gmac_flowmac_register(struct net_device *dev)
{
    athr_gmac_t      *mac;
    
    if (!dev) return -EINVAL;
    mac = (athr_gmac_t*)ATHR_MAC_PRIV(dev);

    mac->flow_mac_handle = flowmac_register(dev, &athr_gmac_flow_ops, 1, 0, 1, 0);
    if (!mac->flow_mac_handle) {
        return -EINVAL;
    }
    /* FIXME this should come through registration */
    printk("%s registeration good handle %p \n", __func__, mac->flow_mac_handle);
    return 0;
}

/* unregister with flowmac module
 */
int
athr_gmac_flowmac_un_register(struct net_device *dev)
{
    athr_gmac_t *mac;

    if (!dev) return -EINVAL;

    mac = (athr_gmac_t*)ATHR_MAC_PRIV(dev);
    if (mac->flow_mac_handle) {
        flowmac_unregister(mac->flow_mac_handle);
        /* memory would be freed by the other driver */
        printk("%s unregistered with flowmac %p\n", __func__, mac->flow_mac_handle);
        mac->flow_mac_handle = NULL;
    }
    return 0;
}
#endif
