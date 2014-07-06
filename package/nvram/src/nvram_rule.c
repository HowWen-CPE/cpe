#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "nvram.h"
#include "nvram_rule.h"

/**
 *\brief Function Pointer Mapping from "0"/"1" to "disabled"/"enabled"
 *\return The strlen of the output string or \ref EZPLIB_VAL_TRUNC
 *\param[in] val The char * which point to "0" or "1"
 *\param[in,out] buf where to output converted string(char *)
 *\param[in] bsize  The size of buf.
 */
int show_enable_value(char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    if (snprintf(buf, bsize, "%s", 
                !strcmp(val, "1") ? "enabled" : "disabled") >= bsize) {
        return EZPLIB_VAL_TRUNC;
    }

    return strlen(buf);
}

/**
 *\brief Function Pointer Return origin string
 *\return The strlen of the output string or \ref EZPLIB_VAL_TRUNC
 *\param[in] val The char * which point to 
 *\param[in,out] buf where to output converted string(char *)
 *\param[in] bsize  The size of buf.
 */
int show_value(char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

#if 0
    if (snprintf(buf, bsize, "%s", val) >= bsize) {
        return EZPLIB_VAL_TRUNC;
    }
#else 
	
	//translate | & ^
	char *dest = buf;
	char *ptr = val;
	while ('\0' != *ptr) {
		if(!strncmp( ptr, "0x5E", 4)) {
			*dest++ = '^';
			ptr += 4;
		}
		else if(!strncmp( ptr, "0x7C", 4)) {
			*dest++ = '|';
			ptr += 4;
		}
		else {
			*dest++ = *ptr++;
		}
	}
#endif

    
    return strlen(buf);
}

char *iftype[] = { "wan", "lan", NULL };

/**
 *\brief Function Pointer Mapping Interface "wan"/"lan"
 *\return The strlen of the output string or \ref EZPLIB_VAL_TRUNC
 *\param[in] val The char * which point to 
 *\param[in,out] buf where to output converted string(char *)
 *\param[in] bsize  The size of buf.
 */
int show_if_value(char *val, char *buf, int bsize)
{
    char *ptr;
    int i, len, idx;

    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    for (i = 0; iftype[i]; i++) {
        len = strlen(iftype[i]);
        if (!strcmp(val, iftype[i])) {
            break;
        }
    }

    if (!iftype[i]) {
        if (snprintf(buf, bsize, "%s", val) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        ptr = val + len;
        /* 1-based index for GUI display. */
        idx = atoi(ptr) + 1;    
        if (snprintf(buf, bsize, "%s%d", iftype[i], idx) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }

    return strlen(buf);
}

/**
 *\brief Function Pointer Mapping from 
 		"both" / "tcp" / "udp" / whatever to 
 		"UDP/TCP" / "TCP" / "UDP" /WHATEVER
 *\return The strlen of the output string or \ref EZPLIB_VAL_TRUNC
 *\param[in] val The char * which point to
 *\param[in,out] buf where to output converted string(char *)
 *\param[in] bsize  The size of buf.
 */
int show_proto_value(char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    if (!strcmp(val, "both")) {
        if (snprintf(buf, bsize, "UDP/TCP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else if (!strcmp(val, "tcp")) {
        if (snprintf(buf, bsize, "TCP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else if (!strcmp(val, "udp")) {
        if (snprintf(buf, bsize, "UDP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        if (snprintf(buf, bsize, "%s", val) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }
    return strlen(buf);
}

char *mask[] = {
    "255.255.255.0",
    "255.255.255.128",
    "255.255.255.192",
    "255.255.255.224",
    "255.255.255.240",
    "255.255.255.248",
    "255.255.255.252",
    "255.255.255.254"
};

/**
 *\brief Function Pointer Mapping from mask bits to "255.255.255.XXX"
 *\return The strlen of the output string or \ref EZPLIB_VAL_TRUNC
 *\param[in] val The char * which point to
 *\param[in,out] buf where to output converted string(char *)
 *\param[in] bsize  The size of buf.
 */
int show_mask_value(char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    int idx = atoi(val);

    if (24 <= idx && idx <= 32) {
        /* 24-based. */
        if (snprintf(buf, bsize, "%s", mask[idx - 24]) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        if (snprintf(buf, bsize, "") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }

    return strlen(buf);
}

/**
 * \brief Use strsep to split word into attrs
 * \return the token at the pos of delim if find, otherwise the entire str
 * \param[in] word The word to be splited
 * \param[in] delim The character as delim(We use \ref ATTR_SEP)
 * \param[out] idx_arr idx_arr res
 * \param[in] max_tok Maxim strlen of a single attr(we use \ref MAX_ATTR_NUM)
 */
int sep_string(char *word, const char *delim, char **idx_arr, int max_tok)
{
    char *str = word;
    char *ptr;
    int pos = 0;
    int i;

    for (i = 0; i < max_tok; i++) {
        idx_arr[i] = NULL;
    }

    while (str) {
        ptr = strsep(&str, delim);
        if (ptr && pos < max_tok) {
            idx_arr[pos++] = ptr;
        } else {
            break;
        }
    }
    return pos;
}


/***************Predefined Attr*****************/

/* enable^name^extif^proto^extport_start^extport_end^ipaddr^inport_start^inport_end */
struct attr fr_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "proto", show_value, show_proto_value },
    { "extport_start", show_value, show_value },
    { "extport_end", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "inport_start", show_value, show_value },
    { "inport_end", show_value, show_value },
    { NULL, NULL, NULL }
};
/*name^enable^extif^proto^int_addr^ext^addr^extport_start^extport_end^forward_check^intport_start^intport_end*/
struct attr one_to_one_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "proto", show_value, show_proto_value },
    { "int_addr", show_value, show_value },
    { "ext_addr", show_value, show_value },
    { "extport_start", show_value, show_value },
    { "extport_end", show_value, show_value },
    { "forward_check", show_value, show_value },
    { "intport_start", show_value, show_value },
    { "intport_end", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^proto[tcp|udp|both]^port_start^port_end^trig_port_start^trig_port_end */
struct attr pt_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "proto", show_value, show_value },
    { "port_start", show_value, show_value },
    { "port_end", show_value, show_value },
    { "trig_port_start", show_value, show_value },
    { "trig_port_end", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^extif^index^wan_ipaddr^mapped_ipaddr */
struct attr vs_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "index", show_value, show_value },
    { "wan_ipaddr", show_value, show_value },
    { "mapped_ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^mtu^stp^upnp^igmp_snooping */
struct attr wan_main_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "mtu", show_value, show_value },
    { "stp", show_value, show_value },
    { "upnp", show_value, show_value },
    { "snooping", show_value, show_value},
    { NULL, NULL, NULL }
};

/* ipaddr^mask^gateway^dns^wins */
struct attr wan_static_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "mask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns1", show_value, show_value },
    { "dns2", show_value, show_value },
    { "wins", show_value, show_value },
    { NULL, NULL, NULL }
};

/* hostname^domain^lease*/
struct attr wan_dhcp_rule_attr[] = {
    { "hostname", show_value, show_value },
    { "domain", show_value, show_value },
    { "lease", show_value, show_value },
    { NULL, NULL, NULL }
};

/* auth^username^passwd^demand^idletime^echo_failure^redialperiod^
 * mru^mtu^pppiptype[isp|custom]^custom_ipaddr^servname */
struct attr wan_pppoe_rule_attr[] = {
    { "auth", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "demand", show_value, show_value },
    { "idletime", show_value, show_value },
    { "echo_failure", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mru", show_value, show_value },
    { "mtu", show_value, show_value },
    { "pppiptype", show_value, show_value },
    { "custom_ipaddr", show_value, show_value },
    { "servname", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ipaddr^length^gateway^dns1^dns2 */
struct attr wan_staticv6_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "length", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns1", show_value, show_value },
    { "dns2", show_value, show_value },
    { NULL, NULL, NULL }
};

/* hostname^domain^lease^type[0:stateless|1:stateful] */
struct attr wan_dhcpv6_rule_attr[] = {
    { "hostname", show_value, show_value },
    { "domain", show_value, show_value },
    { "lease", show_value, show_value },
    { "type", show_value, show_value },
    { NULL, NULL, NULL }
};

/* brokerv4addr^v6ipaddr_length^v6gateway */
struct attr wan_tunnel6_rule_attr[] = {
    { "brokerv4addr", show_value, show_value },
    { "v6ipaddr_length", show_value, show_value },
    { "v6gateway", show_value, show_value },
    { NULL, NULL, NULL }
};

/* username^passwd^servicename^
 * demand^idletime^echo_failure^redialperiod^
 * mru^mtu^pppiptype[isp|custom]^custom_ipaddr */
struct attr wan_pppoev6_rule_attr[] = {
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "servicename", show_value, show_value },
    { "demand", show_value, show_value },
    { "idletime", show_value, show_value },
    { "echo_failure", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mru", show_value, show_value },
    { "mtu", show_value, show_value },
    { "pppiptype", show_value, show_value },
    { "custom_ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^server^username^passwd */
struct attr wan_bigpond_rule_attr[] = {
    { "enable", show_value, show_value },
    { "server", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { NULL, NULL, NULL }
};

/* apn_type^location^isp^apn^pin^mode^username^passwd^demand^idletime^
 * redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^ctrltty^devname^flag^turbolink
 */
struct attr wan_wwan_rule_attr[] = {
    { "apn_type", show_value, show_value },
    { "location", show_value, show_value },
    { "isp", show_value, show_value },
    { "apn", show_value, show_value },
    { "pin", show_value, show_value },
    { "mode", show_value, show_value },
    { "auth", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "demand", show_value, show_value },
    { "idletime", show_value, show_value },
    { "echo_failure", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mru", show_value, show_value },
    { "mtu", show_value, show_value },
    { "dialstr", show_value, show_value },
    { "brand", show_value, show_value },
    { "model", show_value, show_value },
    { "ttynum", show_value, show_value },
    { "datatty", show_value, show_value },
    { "ctrltty", show_value, show_value },
    { "devname", show_value, show_value },
    { "flag", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* serverip^iptype[static|dhcp]^ipaddr^netmask^gateway^username^
   passwd^nailup^timeout^pppiptype[isp|custom]^custom_ipaddr^
   encmode[disabled|mppe]^auth[chap|pap|none] */
struct attr wan_pptp_l2tp_rule_attr[] = {
    { "serverip", show_value, show_value },
    { "iptype", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "netmask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "nailup", show_value, show_value },
    { "timeout", show_value, show_value },
    { "pppiptype", show_value, show_value },
    { "custom_ipaddr", show_value, show_value },
    { "encmode", show_value, show_value },
    { "auth", show_value, show_value},
    { NULL, NULL, NULL }
};

struct attr wan_pptp_l2tp_common_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "netmask", show_value, show_value },
    { "gateway", show_value, show_value },
    { NULL, NULL, NULL }
};

/* brand^model^signal^ttynum^datatty^ctrltty^devname^flag */
struct attr wan_wwan_probe_rule_attr[] = {
    { "brand", show_value, show_value },
    { "model", show_value, show_value },
    { "signal", show_value, show_value },
    { "ttynum", show_value, show_value },
    { "datatty", show_value, show_value },
    { "ctrltty", show_value, show_value },
    { "devname", show_value, show_value },
    { "flag", show_value, show_value },
    { NULL, NULL, NULL }
};

/* dnstype1[ispdns|custom|none]^dns1^dnstype2^dns2 */
struct attr wan_dns_rule_attr[] = {
    { "dnstype1", show_value, show_value },
    { "dns1", show_value, show_value },
    { "dnstype2", show_value, show_value },
    { "dns2", show_value, show_value },
    { NULL, NULL, NULL }
};

/* dnstype1[ispdns|custom|none]^dns1^dnstype2^dns2 */
struct attr wan_dnsv6_rule_attr[] = {
    { "dnstype1", show_value, show_value },
    { "dns1", show_value, show_value },
    { "dnstype2", show_value, show_value },
    { "dns2", show_value, show_value },
    { NULL, NULL, NULL }
};

/* XX:XX:XX:XX:XX:XX */
struct attr wan_hwaddr_rule_default_attr[] = {
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^hwaddr^addrtype[ipaddr|hwaddr]^ipaddr */
struct attr wan_hwaddr_clone_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "hwaddr", show_value, show_value },
    { "addrtype", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/** 
 * name^enable[0|1]^type[dyndns|tzo|zoneedit]^
 * username^passwd^hostname^server
 */
struct attr wan_ddns_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "type", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "hostname", show_value, show_value },
    { "server", show_value, show_value },
    { NULL, NULL, NULL }
};

/** 
 * name^enable[0|1]^
 * username^passwd^redirect[0|1]
 */
struct attr wan_opendns_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "redirect", show_value, show_value },
    { "label", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr wan_st_rule_attr[] = {
    { "uptime", show_value},
    { NULL, NULL, NULL }
};

struct attr wan_stv6_rule_attr[] = {
    { "uptime", show_value},
    { NULL, NULL, NULL }
};

struct attr wan_up_down_state_rule_attr[] = {
    { "v4", show_value},
    { "v6", show_value},
    { NULL, NULL, NULL }
};

struct attr lan_st_rule_attr[] = {
    { "uptime", show_value},
    { NULL, NULL, NULL }
};

struct attr lan_stv6_rule_attr[] = {
    { "uptime", show_value},
    { NULL, NULL, NULL }
};

struct attr wlan_st_rule_attr[] = {
    { "uptime", show_value},
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^mtu^stp^weight */
struct attr lan_main_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "mtu", show_value, show_value },
    { "stp", show_value, show_value },
    { "weight", show_value, show_value },
    { "snooping", show_value, show_value},
    { NULL, NULL, NULL }
};

/* XX:XX:XX:XX:XX:XX */
struct attr lan_hwaddr_rule_default_attr[] = {
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* [0|1]^XX:XX:XX:XX:XX:XX */
struct attr lan_hwaddr_clone_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* Guest lan rule */
/* enable^wif[ra0 ~ ra3]^bif[brxx]^lanid^ipaddr^netmask^bwen^bw[in kbits][0:disabled]^prio[1~3]^domain^start^count
 * domain : domain name for this dns group
 * start : dhcp offering start ip address
 * count : total count dhcp server can offer
 * */
struct attr guest_lan_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "wif", show_value, show_value },
    { "bif", show_value, show_value },
    { "lanid", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "netmask", show_value, show_value },
    { "bwen", show_value, show_value },
    { "bw", show_value, show_value },
    { "prio", show_value, show_value },
    { "domain", show_value, show_value },
    { "start", show_value, show_value },
    { "count", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ipaddr^mask^gateway^dns^wins^
 * alias_enable^alias_ipaddr^alias_netmask^
 * ipaddr_normal^mask_normal^ipaddr_ap^mask_ap */
struct attr lan_static_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "mask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns", show_value, show_value },
    { "wins", show_value, show_value },
    { "alias_enable", show_value, show_enable_value },
    { "alias_ipaddr", show_value, show_value },
    { "alias_netmask", show_value, show_value },
    { "ipaddr_normal", show_value, show_value },
    { "mask_normal", show_value, show_value },
    { "ipaddr_ap", show_value, show_value },
    { "mask_ap", show_value, show_value },
    { NULL, NULL, NULL }
};

/* pppoe_prefix^pppoe_suffix^static_prefix^static_suffix^dhcp_prefix^dhcp_suffix^
 * length^gateway^dns^wins^alias_enable^alias_ipaddr^alias_netmask^
 * ipaddr_normal^mask_normal^ipaddr_ap^mask_ap */
struct attr lan_staticv6_rule_attr[] = {
    { "pppoe_prefix", show_value, show_value },
    { "pppoe_suffix", show_value, show_value },
    { "static_prefix", show_value, show_value },
    { "static_suffix", show_value, show_value },
    { "dhcp_prefix", show_value, show_value },
    { "dhcp_suffix", show_value, show_value },
    { "length", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns", show_value, show_value },
    { "wins", show_value, show_value },
    { "alias_enable", show_value, show_enable_value },
    { "alias_ipaddr", show_value, show_value },
    { "alias_netmask", show_value, show_value },
    { "ipaddr_normal", show_value, show_value },
    { "mask_normal", show_value, show_value },
    { "ipaddr_ap", show_value, show_value },
    { "mask_ap", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^start^num^lease^domain^wins^
 * dnstype^dnsaddr1^dnstype2^dnsaddr2^
 * dnstype3^dnsaddr3^dnstype4^dnsaddr4 */
struct attr lan_dhcps_rule_attr[] = {
    { "name", show_value, show_value },
    { "dhcpmode", show_value, show_value},
    { "start", show_value, show_value },
    { "num", show_value, show_value },
    { "lease", show_value, show_value },
    { "domain", show_value, show_value },
    { "wins", show_value, show_value },
    { "dnstype", show_value, show_value},
    { "dnsaddr1", show_value, show_value},
    { "dnsaddr2", show_value, show_value},
    { "dnsaddr3", show_value, show_value},   
    { "dnsaddr4", show_value, show_value},
    { "dnstype2", show_value, show_value},
    { "dnstype3", show_value, show_value},
    { "dnstype4", show_value, show_value},
    {"lan0_fallback_ip",show_value,show_value},
    {"lan0_fallback_mask",show_value,show_value},
    {"lan0_fallback_interval",show_value,show_value},
    {"serverAddr",show_value,show_value},
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^pppoe_prefix^static_prefix^dhcp_prefix
   ^dnstype[auto|custom]^dnsaddr1^dnsaddr2^type[1|0]^
   start^end^lifetime" */
struct attr lan_dhcpsv6_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "pppoe_prefix", show_value, show_value },
    { "static_prefix", show_value, show_value },
    { "dhcp_prefix", show_value, show_value },
    { "domain", show_value, show_value },
    { "dnstype", show_value, show_value },
    { "dnsaddr1", show_value, show_value },
    { "dnsaddr2", show_value, show_value },
    { "type", show_value, show_enable_value },
    { "start", show_value, show_value },
    { "end", show_value, show_value },
    { "lifetime", show_value, show_value },
    { NULL, NULL, NULL }
};

/*interface^protocol
*/
struct attr rip_config_rule_attr[] = {
    {"RIP_enable", show_value, show_value},
    {"RIPv1_enable", show_value, show_value},
    {"RIPv2_enable", show_value, show_value},
	{"RIPng_enable", show_value, show_value},
	{"RIPlan_enable", show_value, show_value},
	{"RIPnglan_enable", show_value, show_value},
	{"RIPwan_enable", show_value, show_value},
	{"RIPngwan_enable", show_value, show_value},
	{NULL,NULL,NULL}
};


 /*
  * hwaddr0^hwaddr1^hwaddr2^hwaddr3
  */
struct attr wl0_wds_attr[] = {
    { "hwaddr0", show_value, show_value },
    { "hwaddr1", show_value, show_value },
    { "hwaddr2", show_value, show_value },
    { "hwaddr3", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^bw_upload^bw_download */
struct attr bw_rule_attr[] = {
    { "enable", show_value, show_value },
    { "bw_upload", show_value, show_value },
    { "bw_download", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable */
struct attr bw_monitor_rule_attr[] = {
    { "enable", show_value, show_value },
    { NULL, NULL, NULL }
};

/* catname */
struct attr bw_ezmodesort_rule_attr[] = {
    { "catname", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^index^prio^cat^serv^
   tcp_port[port|port_beg:port_end]^
   udp_port[port|port_beg:port_end]^
   lan_tcp_enable^lan_tcp_bw^lan_tcp_min[0:max bw|1:min bw]^
   lan_udp_enable^lan_udp_bw^lan_udp_min[0:max bw|1:min bw]^
   wan_tcp_enable^wan_tcp_bw^wan_tcp_min[0:max bw|1:min bw]^
   wan_udp_enable^wan_udp_bw^wan_udp_min[0:max bw|1:min bw] */
struct attr bw_app_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "index", show_value, show_value },
    { "prio", show_value, show_value },
    { "cat", show_value, show_value },
    { "serv", show_value, show_value },
    { "tcp_port", show_value, show_value },
    { "udp_port", show_value, show_value },
    { "lan_tcp_enable", show_value, show_value },
    { "lan_tcp_bw", show_value, show_value },
    { "lan_tcp_min", show_value, show_value },
    { "lan_udp_enable", show_value, show_value },
    { "lan_udp_bw", show_value, show_value },
    { "lan_udp_min", show_value, show_value },
    { "wan_tcp_enable", show_value, show_value },
    { "wan_tcp_bw", show_value, show_value },
    { "wan_tcp_min", show_value, show_value },
    { "wan_udp_enable", show_value, show_value },
    { "wan_udp_bw", show_value, show_value },
    { "wan_udp_min", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr snmpd_access_rule_attr[] = {
    { "access_name", show_value, show_value },
    { "group_name", show_value, show_value },
    { "context", show_value, show_value },
    { "mode", show_value, show_value },
    { "levle", show_value, show_value },
    { "match", show_value, show_value },
    { "read", show_value, show_value },
    { "write", show_value, show_value },
    { "notify", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr snmpd_group_rule_attr[] = {
	{ "group", show_value, show_value },
    { "name", show_value, show_value },
    { "version", show_value, show_value },
    { "access", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr snmpd_view_rule_attr[] = {
	{ "view", show_value, show_value },
    { "name", show_value, show_value },
    { "subtree", show_value, show_value },
    { "mask", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^inipaddr_start^inipaddr_end^proto^extif^
   bw_towan^bw_tolan^apptype^prio^
   outipaddr_start^outipaddr_end^inport^outport^reserve_bw */
struct attr bw_sbm_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "inipaddr_start", show_value, show_value },
    { "inipaddr_end", show_value, show_value },
    { "proto", show_value, show_value },
    { "extif", show_value, show_value },
    { "bw_towan", show_value, show_value },
    { "bw_tolan", show_value, show_value },
    { "apptype", show_value, show_value },
    { "prio", show_value, show_value },
    { "outipaddr_start", show_value, show_value },
    { "outipaddr_end", show_value, show_value },
    { "inport", show_value, show_value },
    { "outport", show_value, show_value },
    { "reserve_bw", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^srcipaddr_start^srcipaddr_end^dstipaddr_start^
    dstipaddr_end^proto^srcport_start^srcport_end^dstport_start^
    dstport_end^extif^routing_type[default|static]^gateway_ipaddr^
    netmask^matric */
struct attr rt_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "srcipaddr_start", show_value, show_value },
    { "srcipaddr_end", show_value, show_value },
    { "dstipaddr_start", show_value, show_value },
    { "dstipaddr_end", show_value, show_value },
    { "proto", show_value, show_proto_value },
    { "srcport_start", show_value, show_value },
    { "srcport_end", show_value, show_value },
    { "dstport_start", show_value, show_value },
    { "dstport_end", show_value, show_value },
    { "extif", show_value, show_value },
    { "routing_type", show_value, show_value },
    { "gateway_ipaddr", show_value, show_value },
    { "netmask", show_value, show_value },
    { "matrix", show_value, show_value },
    { NULL, NULL, NULL }
};

/* rip[v1/v2/none] */
struct attr rt_rip_rule_attr[] = {
    { "rip", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable */
struct attr nat_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* enable */
struct attr hwnat_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* activex_enable^java_enable^proxy_enable^cookie_enable^trustipaddr */
struct attr wf_content_rule_attr[] = {
    { "activex_enable", show_value, show_enable_value },
    { "java_enable", show_value, show_enable_value },
    { "proxy_enable", show_value, show_enable_value },
    { "cookie_enable", show_value, show_enable_value },
    { "url_enable", show_value, show_enable_value },
    { "trustipaddr", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* enable^keyword^type^access */
struct attr wf_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "keyword", show_value, show_value },
    { "type", show_value, show_value },
    { "access", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable */
struct attr igmp_proxy_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* enable^p1^p2^p3^p4 */
struct attr igmp_snoop_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "p1", show_value, show_value },
    { "p2", show_value, show_value },
    { "p3", show_value, show_value },
    { "p4", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^cat^month^date^wkday^beghr^begmin^endhr^endmin^wlanstatus */
struct attr sched_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "cat", show_value, show_value },
    { "month", show_value, show_value },
    { "date", show_value, show_value },
    { "wkday", show_value, show_value },
    { "beghr", show_value, show_value },
    { "begmin", show_value, show_value },
    { "endhr", show_value, show_value },
    { "endmin", show_value, show_value },
    { "wlanstatus", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* method^server^path^inf_filename */
 /* rule set 0 for default online firmware upgrade settings */
struct attr fw_upgrade_rule_attr[] = {
    { "method", show_value, show_value },
    { "server", show_value, show_value },
    { "path", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "inf_filename", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* method^server^path^inf_filename^release_note^filename
    info_ok[1:got fw info | 0:no info]^
    fw_ok[1:got fw | 0:no fw]^
    relnote_ok[1:got release note | 0:no release note] */
 /* rule set 0 for default online firmware upgrade settings */
struct attr fw_info_rule_attr[] = {
    { "model", show_value, show_value },
    { "version", show_value, show_value },
    { "revision", show_value, show_value },
    { "date", show_value, show_value },
    { "size", show_value, show_value },
    { "filename", show_value, show_value },
    { "release_note", show_value, show_value },
    { "info_ok", show_value, show_value },
    { "fw_ok", show_value, show_value },
    { "relnote_ok", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable^ntp_enable[0|1]^ntp_interval */
struct attr cron_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "ntp_enable", show_value, show_enable_value },
    { "ntp_interval", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^type[pool|ipaddr]^pool^ipaddr^zone
    custom_time^year^mon^date^hour^min^sec^
    daylight_saving^
    ds_start_mon^ds_start_day^ds_start_hour^
    ds_end_mon^ds_end_day^ds_end_hour^customized^
    custom_server[0|1]^serv_ipaddr */

struct attr ntp_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "type", show_value, show_value },
    { "pool", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "zone", show_value, show_value },
    { "custom_time", show_value, show_value },
    { "year", show_value, show_value },
    { "mon", show_value, show_value },
    { "date", show_value, show_value },
    { "hour", show_value, show_value },
    { "min", show_value, show_value },
    { "sec", show_value, show_value },
    { "daylight_saving", show_value, show_value },
    { "ds_start_mon", show_value, show_value },
    { "ds_start_day", show_value, show_value },
    { "ds_start_hour", show_value, show_value },
    { "ds_end_mon", show_value, show_value },
    { "ds_end_day", show_value, show_value },
    { "ds_end_hour", show_value, show_value },
    { "customized", show_value, show_value },
    { "custom_server", show_value, show_value },
    { "serv_ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ifname^vid^portmember^flag */
struct attr vlan_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ifname", show_value, show_value },
    { "vid", show_value, show_value },
    { "portmember", show_value, show_value },
    { "flag", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^portid^pvid^tag^gmemb */
struct attr vlanport_rule_attr[] = {
    { "name", show_value, show_value },
    { "portid", show_value, show_value },
    { "pvid", show_value, show_value },
    { "tag", show_value, show_value },
    { "gmemb", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ifnames^flag */
struct attr br_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ifnames", show_value, show_value },
    { "flag", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^lan_ifnames^wan_ifnames*/
struct attr lan_wan_mode_rule_attr[] = {
    { "name", show_value, show_value },
    { "lan_ifnames", show_value, show_value },
    { "wan_ifnames", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr system_mode_attr[] = {
    { "name", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^port */
struct attr ssh_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "port", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ipaddr^ipt_enable^web_mgmt^dns^ppp^
    upnp^wireless^ntp^sys_warning^dhcp_serv^dhcp_cli^
    ddns^firewall */
struct attr log_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { "port", show_value, show_value },
    { "ipt_enable", show_value, show_enable_value },
    { "web_mgmt", show_value, show_enable_value },
    { "dns", show_value, show_enable_value },
    { "ppp", show_value, show_enable_value },
    { "upnp", show_value, show_enable_value },
    { "wireless", show_value, show_enable_value },
    { "ntp", show_value, show_enable_value },
    { "sys_warning", show_value, show_enable_value },
    { "dhcp_serv", show_value, show_enable_value },
    { "dhcp_cli", show_value, show_enable_value },
    { "ddns", show_value, show_enable_value },
    { "firewall", show_value, show_enable_value },
    { "vpn", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^extif^inif^port */
struct attr upnp_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "pmp_enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "inif", show_value, show_value },
    { "port", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* friendlyName^manufacturer^manufacturerURL^modelDescription^
  * modelName^modelNumber^modelURL^serialNumber^UDN^presentationURL */
struct attr upnpxml_rule_attr[] = {
    { "friendlyName", show_value, show_value },
    { "manufacturer", show_value, show_value },
    { "manufacturerURL", show_value, show_value },
    { "modelDescription", show_value, show_value },
    { "modelName", show_value, show_value },
    { "modelNumber", show_value, show_value },
    { "modelURL", show_value, show_value },
    { "serialNumber", show_value, show_value },
    { "UDN", show_value, show_value },
    { "presentationURL", show_value, show_value },
    { NULL, NULL, NULL }
};

/* 
 * name^enable[0|1]^extif^inif^proto^srcip^srcmask^dstip^dstmask^srcport^
 * dstport^action[(0)DROP|(1)ACCEPT]^mark^bw_idx^sched_idx
 */
struct attr fl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_if_value },
    { "inif", show_value, show_if_value },
    { "srcipaddr_start", show_value, show_value },
    { "srcipaddr_end", show_value, show_mask_value },
    { "dstipaddr_start", show_value, show_value },
    { "dstipaddr_end", show_value, show_mask_value},
    { "proto", show_value, show_proto_value },
    { "srcport_start", show_value, show_value },
    { "srcport_end", show_value, show_value },
    { "dstport_start", show_value, show_value },
    { "dstport_end", show_value, show_value },
    { "action", show_value, show_value },
    { "mark", show_value, show_value },
    { "bw_idx", show_value, show_value },
    { "sched_idx", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^hwaddr^action^acl_enable^dhcp_enable^arp_enable^
 * ipaddr */
struct attr fl_hwaddr_rule_attr[] = {
    { "name", show_value, show_value },
    { "hwaddr", show_value, show_value },
    { "action", show_value, show_value },
    { "acl_enable", show_value, show_enable_value },
    { "arp_enable", show_value, show_enable_value },
    { "dhcp_enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^ipaddr */
struct attr fr_dmz_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* TOS enable^icmp_enable^dns_enable^ssh_enable^
   telnet_enable^check_enable^ack_enable */
struct attr tos_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "icmp_enable", show_value, show_enable_value },
    { "dns_enable", show_value, show_enable_value },
    { "ssh_enable", show_value, show_enable_value },
    { "telnet_enable", show_value, show_enable_value },
    { "check_enable", show_value, show_enable_value },
    { "ack_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^value */
struct attr ttl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "value", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port^secipaddr^adm_timeout */
struct attr http_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "rmgt_enable", show_value, show_enable_value },
	{ "admusername", show_value, show_value },
    { "admpasswd", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
	{ "curusername", show_value, show_value },
    { "port", show_value, show_value },
    { "secipaddr", show_value, show_value },
    { "adm_timeout", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr snmpd_user_rule_attr[] = {
    {"enable",show_value, show_enable_value },
    { "v1_enable", show_value, show_enable_value },
    { "v2c_enable", show_value, show_enable_value },
    { "usm_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/*
status:	0-busy
filename:
ipaddr:
protocol:tftp(0),ftp(1)
username:
password:
*/
struct attr snmpd_upgrade_rule_attr[] = {
    {"status",show_value, show_enable_value },
    { "filename", show_value, show_enable_value },
    { "ipaddr", show_value, show_enable_value },
    { "protocol", show_value, show_enable_value },
	{ "username", show_value, show_enable_value },
	{ "password", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

struct attr snmpd_com2sec_rule_attr[] = {
    { "name", show_value, show_value },
    { "access", show_value, show_value },
    { "address", show_value, show_value },
    { NULL, NULL, NULL }
}; 
 /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
struct attr adtool_rule_attr[] = {
    { "pingiface", show_value, show_value },
    { "pinghost", show_value, show_value },
    { "pingcnt", show_value, show_value },
    { "arpingiface", show_value, show_value },
    { "arpinghost", show_value, show_value },
    { "arpingcnt", show_value, show_value },
    { "tracerouteiface", show_value, show_value },
    { "traceroutehost", show_value, show_value },
    { "traceroutecnt", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^stime^ifnames */
struct attr stats_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "stime", show_value, show_value },
    { "ifnames", show_value, show_value },
    { NULL, NULL, NULL }
};


struct attr wol_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "bcast", show_value, show_value },
    { "hwaddr", show_value, show_value },
    { "iface", show_value, show_value },
    { "passwd", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable^syn_enable^icmp_enable^icmp_redir_enable
    ^rpfilter_enable^state_enable */
struct attr fw_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "syn_enable", show_value, show_enable_value },
    { "icmp_enable", show_value, show_enable_value },
    { "icmp_redir_enable", show_value, show_enable_value },
    { "icmp_ping_enable", show_value, show_enable_value },
    { "rpfilter_enable", show_value, show_enable_value },
    { "state_enable", show_value, show_enable_value },
    { "wantolan_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* enable^ipsec_enable^pptp_enable^l2tp_enable^
    h323_enable^rtsp_enable^mms_enable */
struct attr nat_pass_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "ipsec_enable", show_value, show_enable_value },
    { "pptp_enable", show_value, show_enable_value },
    { "l2tp_redir_enable", show_value, show_enable_value },
    { "h323_enable", show_value, show_enable_value },
    { "rtsp_enable", show_value, show_enable_value },
    { "mms_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* ipsec_enable^pptp_enable^l2tp_enable
    */
struct attr smb_vpn_passthrough_rule_attr[] = {
    { "ipsec_enable", show_value, show_enable_value },
    { "pptp_enable", show_value, show_enable_value },
    { "l2tp_enable", show_value, show_enable_value },
    { "pppoe_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

struct attr smb_ftp_passthrough_rule_attr[] = {
    { "ftp_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

struct attr smb_h323_passthrough_rule_attr[] = {
    { "h323_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* For new added wireless rules */
/* Physical interface related rules*/
/* enable^net_mode^txpower^channel^bisolation^connrssi^disconnrssi */
struct attr wl_basic_rule_attr[] = {
    { "enable", show_value, show_value },
    { "net_mode", show_value, show_value },
    { "txpower", show_value, show_value },
    { "channel", show_value, show_value },
    { "bisolation", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable */
struct attr wl_origset_rule_attr[] = {
    { "enable", show_value, show_value },
    { NULL, NULL, NULL }
};
/* name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout */
struct attr ipsec_rule_attr[] = {
	{ "name", show_value, show_value },
	{ "enable", show_value, show_value },
	{ "mode", show_value, show_value },
	{ "l2tp", show_value, show_value },
	{ "local_extif", show_value, show_value },
	{ "local_inipaddr", show_value, show_value },
	{ "local_netmask", show_value, show_value },
	{ "remote_gateway", show_value, show_value },
	{ "remote_inipaddr", show_value, show_value },
	{ "remote_netmask", show_value, show_value },
	{ "conn_init", show_value, show_value },
	{ "ike_keymode", show_value, show_value },
	{ "psk", show_value, show_value },
	{ "rsa", show_value, show_value },
	{ "ca", show_value, show_value },
	{ "adv", show_value, show_value },
	{ "phase1_mode", show_value, show_value },
	{ "phase1_id", show_value, show_value },
	{ "phase1_lifetime", show_value, show_value },
	{ "phase2_lifetime", show_value, show_value },
	{ "phase1_auth", show_value, show_value },
	{ "phase1_encrypt", show_value, show_value },
	{ "phase1_group", show_value, show_value },
	{ "phase2_auth", show_value, show_value },
	{ "phase2_encrypt", show_value, show_value },
	{ "phase2_group", show_value, show_value },
	{ "dpd_enable", show_value, show_value },
	{ "dpd_interval", show_value, show_value },
	{ "dpd_timeout", show_value, show_value },
	{ "remoteid_type", show_value, show_value },
	{ "remoteid_value", show_value, show_value },
	{ "localid_type", show_value, show_value },
	{ "localid_value", show_value, show_value },
	{ NULL, NULL, NULL }
};

/* phase1[none|established]^phase2[none|established] */
struct attr ipsec_status_rule_attr[] = {
	{ "phase1", show_value, show_value },
	{ "phase2", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*port_device^portid*/
struct attr port_device_rule_attr[] = {
	{ "port_device", show_value, show_value },
	{ "portid", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*ssid0_device^ssid1_device^ssid2_device^ssid3_device^ssid4_device^ssid5_device^ssid6_device^ssid7_device*/
struct attr wl_ap_device_rule_attr[] = {
	{ "ssid0_device", show_value, show_value },
	{ "ssid1_device", show_value, show_value },
	{ "ssid2_device", show_value, show_value },
	{ "ssid3_device", show_value, show_value },
	{ "ssid4_device", show_value, show_value },
	{ "ssid5_device", show_value, show_value },
	{ "ssid6_device", show_value, show_value },
	{ "ssid7_device", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*sta_device*/
struct attr wl_sta_device_rule_attr[] = {
	{ "sta_device", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*security_enable[0|1]^legalap_enable[0|1]*/
struct attr wl_rap_rule_attr[] = {
	{ "security_enable", show_value, show_value },
	{ "legalap_enable", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*mac*/
struct attr wl_rap_mac_rule_attr[] = {
	{ "mac", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*oui*/
struct attr wl_rap_oui_rule_attr[] = {
	{ "oui", show_value, show_value },
	{ NULL, NULL, NULL }
};

/* enable[0|1]^name^redialperiod^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable[0|1]^dns^chap_enable[0|1]^mschap_enable[0|1]^mschapv2_enable[0|1]^eap_enable[0|1]^pap_enable[0|1]^mppe128_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */
struct attr pptpd_rule_attr[] = {
    { "enable", show_value, show_value },
    { "name", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mtu", show_value, show_value },
    { "mru", show_value, show_value },
    { "localip", show_value, show_value },
    { "localip_range", show_value, show_value },
    { "remoteip", show_value, show_value },
    { "remoteip_range", show_value, show_value },
    { "dns_auto_enable", show_value, show_value },
    { "dns", show_value, show_value },
    { "chap_enable", show_value, show_value },
    { "mschap_enable", show_value, show_value },
    { "mschapv2_enable", show_value, show_value },
    { "eap_enable", show_value, show_value },
    { "pap_enable", show_value, show_value },
    { "mppe128_enable", show_value, show_value },
    { "proxyarp_enable", show_value, show_value },
    { "nat_enable", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr pptpd_user_rule_attr[] = {
    { "enable", show_value, show_value },
    { "username", show_value, show_value },
    { "provider", show_value, show_value },
    { "passwd", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^name^redialperiod^mtu^mru^localip^localip_range^remoteip^remoteip_range^
    dns_auto_enable[0|1]^dns^chap_enable[0|1]^mschap_enable[0|1]^mschapv2_enable[0|1]^
    eap_enable[0|1]^pap_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1]^l2tpOIPsec_enable
    ^psk_value*/
struct attr l2tpd_rule_attr[] = {
    { "enable", show_value, show_value },
    { "name", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mtu", show_value, show_value },
    { "mru", show_value, show_value },
    { "localip", show_value, show_value },
    { "localip_range", show_value, show_value },
    { "remoteip", show_value, show_value },
    { "remoteip_range", show_value, show_value },
    { "dns_auto_enable", show_value, show_value },
    { "dns", show_value, show_value },
    { "chap_enable", show_value, show_value },
    { "mschap_enable", show_value, show_value },
    { "mschapv2_enable", show_value, show_value },
    { "eap_enable", show_value, show_value },
    { "pap_enable", show_value, show_value },
    { "proxyarp_enable", show_value, show_value },
    { "nat_enable", show_value, show_value },
    { "l2tpOIPsec_enable", show_value, show_value },
    { "psk_value", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr l2tpd_user_rule_attr[] = {
    { "enable", show_value, show_value },
    { "username", show_value, show_value },
    { "provider", show_value, show_value },
    { "passwd", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^
   antdiv^gmode^gmode_protection^afterburner^frameburst
   txburst[0:disable|1:enable->1]^
   pktaggr[0:disable|1:enable->0]^
   txmode[0:None|1:CCK|2:OFDM->0]^

   rdg[0:disable|1:enablei->0]^
   linkadapt[0:disable|1:enable->0]^
   htc[0:disable|1:enable->0]^

   htbw[0:20|1:20/40->1]^
   gi[0:long|1:short->1]^
   opmode[0:mixed|1:GreenField->0]^
   mcs[0~15,32,33=auto->33]^
   mpdu[0 none, 1~7 -> 5]^
   amsdu[0:disable|1:enable->0]^

   autoba[0:disable|1:enable->1]^
   bawinsize[1~64,default=8,iot=64->64]^
   badecline[0:disable|1:enable->1->0]^
   extcha^wdstxmode[CCK|OFDM|HTMIX|GREENFIELD]
   antennamode [1:external0:internal]^
   distance*/
struct attr wl_advanced_rule_attr[] = {
    { "infra", show_value, show_value },
    { "ap_mode", show_value, show_value },
    { "rateset", show_value, show_value },
    { "mrate", show_value, show_value },
    { "frag", show_value, show_value },
    { "rts", show_value, show_value },
    { "dtim", show_value, show_value },
    { "bcn", show_value, show_value },
    { "plcphdr", show_value, show_value },
    { "antdiv", show_value, show_value },
    { "gmode", show_value, show_value },
    { "gmode_protection", show_value, show_value },
    { "afterburner", show_value, show_value },
    { "frameburst", show_value, show_value },
    { "txburst", show_value, show_enable_value},
    { "pktaggr", show_value, show_enable_value},
    { "txmode", show_value, show_value},

    { "rdg", show_value, show_enable_value},
    { "linkadapt", show_value, show_enable_value},
    { "htc", show_value, show_enable_value},

    { "htbw", show_value, show_enable_value},
    { "gi", show_value, show_enable_value},
    { "opmode", show_value, show_value},
    { "mcs", show_value, show_value},
    { "mpdu", show_value, show_value},
    { "amsdu", show_value, show_enable_value},
    { "autoba", show_value, show_enable_value},
    { "bawinsize", show_value, show_value},
    { "badecline", show_value, show_enable_value},
    { "extcha", show_value, show_value},
    { "wdstxmode", show_value, show_value},
    { "antennamode", show_value, show_value},
    { "distance", show_value, show_value},
    { NULL, NULL, NULL }
};

/* mode^peer_list^timeout */
/* mode:0 disabled 1:restrict 2:bridge 3:repeater 4:lazy*/
struct attr wl_wds_rule_attr[] = {
    { "mode", show_value, show_value },
    { "timeout", show_value, show_value },
    { NULL, NULL, NULL }
};

/* sta_bk^Sta_be^sta_vi^sta_vo^ap_bk^ap_be^ap_vi^ap_vo^no_ack^max_assoc */
struct attr wl_wme_rule_attr[] = {
    { "sta_bk", show_value, show_value },
    { "sta_be", show_value, show_value },
    { "sta_vi", show_value, show_value },
    { "sta_vo", show_value, show_value },
    { "ap_bk", show_value, show_value },
    { "ap_be", show_value, show_value },
    { "ap_vi", show_value, show_value },
    { "ap_vo", show_value, show_value },
    { "no_ack", show_value, show_value },
    { "max_assoc", show_value, show_value },
    { NULL, NULL, NULL }
};

/* mode[normal|ap|ur|wisp]^
 * auto_bridge[0: normal mode|1:auto bridge mode|2:auto ip mode]^
 * ab_status[0:auto bridge/ip not change|1:auto bridge/ip changed] */
struct attr wl_mode_rule_attr[] = {
    { "mode", show_value, show_value },
    { "auto_bridge", show_value, show_enable_value },
    { "ab_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* mode[0:disabled|7:enabled]^pin^configured[0:unconfigured|1:configured] */
struct attr wl_wps_rule_attr[] = {
    { "mode", show_value, show_value },
    { "pin", show_value, show_value },
    { "configured", show_value, show_value },
    { NULL, NULL, NULL }
};

/* mode[0:disabled|1:pin mode|2:PBC mode]^pin^reg_ssid^reg_auth^reg_enc^reg_keyindex^reg_key */
struct attr wl_wps_sta_rule_attr[] = {
    { "mode", show_value, show_value },
    { "pin", show_value, show_value },
    { "reg_ssid", show_value, show_value },
    { "reg_auth", show_value, show_value },
    { "reg_enc", show_value, show_value },
    { "reg_keyindex", show_value, show_value },
    { "reg_key", show_value, show_value },
    { NULL, NULL, NULL }
};

/* wl^wps */
struct attr wl_led_status_rule_attr[] = {
    { "wl", show_value, show_value },
    { "wps", show_value, show_value },
    { NULL, NULL, NULL }
};

/* Virtual interface related rules for wl0/wl1/wl2/wl3 */
/* enable^hidden^rate^wme^isolation */
struct attr wlv_basic_rule_attr[] = {
    { "enable", show_value, show_value },
    { "hidden", show_value, show_value },
    { "rate", show_value, show_value },
    { "wme", show_value, show_value },
    { "isolation", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ifname */
struct attr wlv_ifname_rule_attr[] = {
    { "ifname", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ssid */
struct attr wlv_ssid_rule_attr[] = {
    { "ssid", show_value, show_value },
    { NULL, NULL, NULL }
};

/* secmode */
struct attr wlv_sec_rule_attr[] = {
    { "secmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key_index^key1^key2^key3^key4^
 * keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
struct attr wlv_sec_wep_rule_attr[] = {
    { "key_index", show_value, show_value },
    { "key1", show_value, show_value },
    { "key2", show_value, show_value },
    { "key3", show_value, show_value },
    { "key4", show_value, show_value },
    { "keytype", show_value, show_value },
    { "encmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
 * rekey_time_interval^rekey_pkt_interval^session_timeout */
struct attr wlv_sec_wpa_rule_attr[] = {
    { "key", show_value, show_value },
    { "crypto", show_value, show_value },
    { "radius_key", show_value, show_value },
    { "radius_ipaddr", show_value, show_value },
    { "radius_port", show_value, show_value },
    { "rekey_mode", show_value, show_value },
    { "rekey_time_interval", show_value, show_value },
    { "rekey_pkt_interval", show_value, show_value },
    { "session_timeout", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
 * rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout */
struct attr wlv_sec_wpa2_rule_attr[] = {
    { "key", show_value, show_value },
    { "crypto", show_value, show_value },
    { "radius_key", show_value, show_value },
    { "radius_ipaddr", show_value, show_value },
    { "radius_port", show_value, show_value },
    { "rekey_mode", show_value, show_value },
    { "rekey_time_interval", show_value, show_value },
    { "rekey_pkt_interval", show_value, show_value },
    { "preauth", show_value, show_value },
    { "pmkperiod", show_value, show_value },
    { "session_timeout", show_value, show_value },
    { "wpacap", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^defpolicy */
struct attr wlv_acl_basic_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "defpolicy", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^mac^policy^flag */
struct attr wlv_acl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "mac", show_value, show_value },
	{ "policy", show_value, show_value },
	{ "flag", show_value, show_value },
    { NULL, NULL, NULL }
};

/* wds related rules */
/* hwaddr^secmode */
struct attr wl_wds_basic_rule_attr[] = {
    { "hwaddr", show_value, show_value },
    { "secmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key */
struct attr wl_wds_sec_wep_rule_attr[] = {
    { "key", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto */
struct attr wl_wds_sec_wpa_rule_attr[] = {
    { "key", show_value, show_value },
    { "crypto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^ssid^bssid^secmode^addrmode^lockmac^macrepeater^connrssi^disconnrssi */
struct attr wl_apcli_rule_attr[] = {
    { "enable", show_value, show_value },
    { "ssid", show_value, show_value },
    { "bssid", show_value, show_value },
    { "secmode", show_value, show_value },
    {"addrmode", show_value, show_value},
    {"lockmac", show_value, show_value},
    {"macrepeater", show_value, show_value},
    { "connrssi", show_value, show_value },
    { "disconnrssi", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
struct attr wl_apcli_sec_wep_rule_attr[] = {
    { "key_index", show_value, show_value },
    { "key1", show_value, show_value },
    { "key2", show_value, show_value },
    { "key3", show_value, show_value },
    { "key4", show_value, show_value },
    { "keytype", show_value, show_value },
    { "encmode", show_value, show_value },
    { "wep_encry", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto */
struct attr wl_apcli_sec_wpa_rule_attr[] = {
    { "key", show_value, show_value },
    { "crypto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* wpa_auth^wpa_user^wpa_passwd^wpa_crypto */
struct attr wl_apcli_wpa_rule_attr[] = {
	{ "wpa_auth", show_value, show_value },
	{ "wpa_user", show_value, show_value },
	{ "wpa_passwd", show_value, show_value },
    { "wpa_crypto", show_value, show_value },
	{ NULL, NULL, NULL }
};

/* enable^city^degree */
struct attr weather_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "city", show_value, show_value },
    { "degree", show_value, show_value },
    { NULL, NULL, NULL }
};
/* mode 2.4g or 5g */
struct attr wl_ezsy_mode_rule_attr[] = {
    { "mode", show_value, show_value },
    { NULL, NULL, NULL }
};
/* wisp mode 2.4g as wisp,5g as Ap or 2.4g as AP,5g as wisp */
struct attr wl_wisp_mode_rule_attr[] = {
    { "mode2.4g", show_value, show_value },
    { "mode5g", show_value, show_value },
    { NULL, NULL, NULL }
};
/*-----------------Qos configuration---------------------  
--Jack--20121219*/

/* port_status */
struct attr direct_multicast_status_attr[] = {
    { "port_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* maxnum */
struct attr direct_multicast_maxnum_attr[] = {
    { "maxnum", show_value, show_value },
    { NULL, NULL, NULL }
};

/* port_status */
struct attr tos_classification_status_attr[] = {
    { "port_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* port_status */
struct attr cos_classification_status_attr[] = {
    { "port_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* port_status */
struct attr heuristics_classification_status_attr[] = {
    { "port_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* level */
struct attr bwm_level_dl_index_attr[] = {
    { "level", show_value, show_value },
    { NULL, NULL, NULL }
};

/* level */
struct attr bwm_level_up_index_attr[] = {
    { "level", show_value, show_value },
    { NULL, NULL, NULL }
};

/* port_status */
struct attr qos_rule_status_attr[] = {
    { "port_status", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* rules enable status */
/* rai0~ra3 ra0~ra3 eth2 */
struct attr qos_rule_active_attr[] = {
    { "1", show_value, show_enable_value },
    { "2", show_value, show_enable_value },
    { "3", show_value, show_enable_value },
    { "4", show_value, show_enable_value },
    { "5", show_value, show_enable_value },
    { "6", show_value, show_enable_value },
    { "7", show_value, show_enable_value },
    { "8", show_value, show_enable_value },
    { "9", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* rule */
struct attr bwm_level_attr[] = {
    { "rule", show_value, show_value },
    { NULL, NULL, NULL }
};

/* voice^video^data^background */
struct attr stream_priority_attr[] = {
    { "voice", show_value, show_value },
    { "video", show_value, show_value },
    { "data", show_value, show_value },
    { "background", show_value, show_value },
    { NULL, NULL, NULL }
};

/* rule */
struct attr sta_priority_attr[] = {
    { "rule", show_value, show_value },
    { NULL, NULL, NULL }
};

/* rule */
struct attr qos_rule_record_attr[] = {
    { "rule", show_value, show_value },
    { NULL, NULL, NULL }
};

/* status^age */
struct attr igmp_snooping_attr[] = {
    { "status", show_value, show_enable_value },
    { "age", show_value, show_value },
    { NULL, NULL, NULL }
};

/* voice^video^data^background */
struct attr tos_classify_attr[] = {
    { "voice", show_value, show_value },
    { "video", show_value, show_value },
    { "data", show_value, show_value },
    { "background", show_value, show_value },
    { "mode", show_value, show_value },
    { "other", show_value, show_value },
    { NULL, NULL, NULL }
};

/* voice^video^data^background */
struct attr cos_classify_attr[] = {
    { "voice", show_value, show_value },
    { "video", show_value, show_value },
    { "data", show_value, show_value },
    { "background", show_value, show_value },
    { NULL, NULL, NULL }
};

/* (d_count,b_count,ipg_min,ipg_max,pl_min,pl_max) 
voice^video^data^background
*/
struct attr heuristics_classify_attr[] = {
    { "voice", show_value, show_value },
    { "video", show_value, show_value },
    { "data", show_value, show_value },
    { "background", show_value, show_value },
    { NULL, NULL, NULL }
};

/* (status^voice^video^data^background) */
struct attr tos_mark_attr[] = {
    { "status", show_value, show_enable_value },
    { "voice", show_value, show_value },
    { "video", show_value, show_value },
    { "data", show_value, show_value },
    { "background", show_value, show_value },
    { NULL, NULL, NULL }
};

/* (wlantolan^lanto24Gwlan^lanto5Gwlan) */
struct attr root_bw_attr[] = {
    { "wlantolan", show_value, show_value },
    { "lanto2wlan", show_value, show_value },
    { "lanto5wlan", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr wl_brg_mode_rule_attr[] = {
	{ "brg_mode", show_value, show_value },
	{ NULL, NULL, NULL }
};

/*
wmm_enable^ack_enable^ac_be^ac_bk^ac_vi^ac_vo^
cwmin_be^cwmax_be^aifs_be^txop_be^cwmin_bk^cwmax_bk^aifs_bk^txop_bk^
cwmin_vi^cwmax_vi^aifs_vi^txop_vi^cwmin_vo^cwmax_vo^aifs_vo^txop_vo^bss
*/
struct attr wmm_rule_attr[] = {
	{ "wmm_enable", show_value, show_enable_value },
	{ "ack_enable", show_value, show_enable_value },
	{ "ac_be", show_value, show_enable_value },
	{ "ac_bk", show_value, show_enable_value },
	{ "ac_vi", show_value, show_enable_value },
	{ "ac_vo", show_value, show_enable_value },
	{ "cwmin_be", show_value, show_value },
	{ "cwmax_be", show_value, show_value },
	{ "aifs_be", show_value, show_value },
	{ "txop_be", show_value, show_value },
	{ "cwmin_bk", show_value, show_value },
	{ "cwmax_bk", show_value, show_value },
	{ "aifs_bk", show_value, show_value },
	{ "txop_bk", show_value, show_value },
	{ "cwmin_vi", show_value, show_value },
	{ "cwmax_vi", show_value, show_value },
	{ "aifs_vi", show_value, show_value },
	{ "txop_vi", show_value, show_value },
	{ "cwmin_vo", show_value, show_value },
	{ "cwmax_vo", show_value, show_value },
	{ "aifs_vo", show_value, show_value },
	{ "txop_vo", show_value, show_value },
	{ "bss", show_value, show_enable_value },    
	{ NULL, NULL, NULL }
};

struct attr wl_board_model_rule_attr[] = {
	{ "bd_model", show_value, show_value },
	{ NULL, NULL, NULL }
};

struct attr proto_rule_attr[] = {
	{ "curr", show_value, show_value },
	{ "prev", show_value, show_value },
	{ NULL, NULL, NULL }
};



struct rule rules[] = {
#define aa(x, y) { x, y },
#include "rule_2_attr.h"
#undef aa
	{ NULL, NULL }
};

/**
 * \brief Get a specified rule set from nvram, parse it by '|' character, 
 * and copy the obtained rule into the given buffer.
 * \return Return the length of the rule if successful. Otherwise, a negative
 * indicates an error.
 * NOTE: If the return value is larger than or equal to bsize, it means the 
 * rule is truncated.
 *
 * \param[in] rule_set: pointer to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 *
 *  nvram show <rule-set> <nth> 
 */
int ezplib_get_rule(const char *rule_set, int nth, 
	char *buf, int bsize)
{
    char tmp[EZPLIB_BUF_LEN];
    char *wordlist;
    char *str, *ptr;
    int ret;

    assert(strlen(RULE_SEP) == 1);

    if (!buf) {
        return EZPLIB_INVALID;
    }

    /* Clean up the buffer that carries the retrieved value. */
    memset(buf, '\0', bsize);

    if (!rule_set || !*rule_set) {
        return EZPLIB_INVALID;
    }

    wordlist = nvram_get(rule_set);
    if (!wordlist) {
        return EZPLIB_NO_RULE_SET;
    }

    if (!*wordlist) {
        return EZPLIB_NO_RULE;
    }

    ret = snprintf(tmp, EZPLIB_BUF_LEN, "%s", wordlist);
    if (ret >= EZPLIB_BUF_LEN) {
        return EZPLIB_VAL_TRUNC;
    }

    str = tmp;
    while (str) {
        ptr = strsep(&str, RULE_SEP);
        if (!ptr) {
            return EZPLIB_NO_RULE;
        }

        if (nth != 0) {
            nth --;
            continue;
        }
        ret = snprintf(buf, bsize, "%s", ptr);
        if (ret >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
        return ret;
    }

    if (nth >= 0) {
        return EZPLIB_NO_RULE;
    }
    return 0;
}

/**
 * \brief Get a specified rule set from nvram, parse it by '|' character, 
 * and copy the obtained rule into the given buffer.
 * \return Return the length of the rule if successful. Otherwise, a negative
 * indicates an error.
 * NOTE: If the return value is larger than or equal to bsize, it means the 
 * rule is truncated.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] start: to specify the position of the start attribute 
 *                   in the rule set.
 * \param[in] end: to specify the position of the end attribute 
 *                 in the rule set.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 */
int ezplib_get_subrule(const char *rule_set, int nth, 
	int start, int end, char *buf, int bsize)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    int ret, i, j, len;

    if (!rule_set || !*rule_set || !buf) {
        return EZPLIB_INVALID;
    }

    if (start > end) {
        return EZPLIB_INVALID;
    }

    /**
     * Clean up the buffer for carrying on the retrieved value, in case errors
     * happen while processing of attribute.
     */
    memset(buf, '\0', bsize);

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = j = 0; i < MAX_ATTR_NUM; i++) {
        if (!ptr_array[i]) {
            break;
        }
        if (start <= i && i <= end) {
            len = snprintf(buf + j, bsize - j, "%s%s", ptr_array[i], ATTR_SEP);
            j += len;
        }
    }

    if (j > 0 && buf[j-1]) {
        /* Remove the last ATTR_SEP. */
        buf[j-1] = '\0'; 
    }

    return 0;
}

/**
 * \brief Get a specified attribute of the given rule from the rule set in 
 * nvram. The rules are separated by RULE_SEP character and the attributes
 * in a rule are separated by ATTR_SEP character.
 * \return Return the length of the attribute if successful. Otherwise, a 
 * negative indicates an error.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] type: a pointer to the specified type of attribute.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 * \param[in] use: EZPLIB_USE_CLI or EZPLIB_USE_WEB
 *
 * nvram show <rule-set> <nth> <attr-type> 
 */
int ezplib_get_attr_val(const char *rule_set, int nth, 
		const char *type, char *buf, int bsize, int use)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    int ret, i;


    if (!rule_set || !*rule_set || !type || !*type || !buf) {
        return EZPLIB_INVALID;
    }

    /**
     * Clean up the buffer for carrying on the retrieved value, in case errors
     * happen while processing of attribute.
     */
    memset(buf, '\0', bsize);

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = 0; rules[i].name; i++) {
        struct attr *attr;
        int j;
        if (strcmp(rules[i].name, rule_set) != 0) {
            continue;
        }
    
        attr = rules[i].attr;
        for (j = 0; attr[j].name && ptr_array[j]; j++) {
            if (strcmp(attr[j].name, type) == 0) {
                if (use == EZPLIB_USE_CLI && attr[j].func_orig) {
                    /* Don't wrap. Just get the value stored in nvram. */
                    return attr[j].func_orig( ptr_array[j], 
                                             buf, bsize);
                } else if (use == EZPLIB_USE_WEB && attr[j].func_wrap) {
                    /* Wrap the value from nvram. Eg., the attribute
                     * <enable> is 1 will be wrapped to 'checked'.
                     */
                    return attr[j].func_wrap( ptr_array[j], buf, 
                                             bsize);
                }
            }
        }
    }

    return EZPLIB_NO_ATTRIBUTE;
}

int ezplib_op_rule(const char *rule_set, enum opcode op, int nth, const char *new_rule)
{
    char buf[EZPLIB_BUF_LEN];
    char word[EZPLIB_BUF_LEN];
    char *ptr;
    int len, ret, i, rule_num;
    char *rule_sep = RULE_SEP;

    /* Get the number of rules in the rule set. */
    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }
    
    len = EZPLIB_BUF_LEN;
    ptr = buf;
    for (i = 0; i < rule_num; i++) {
        ret = ezplib_get_rule(rule_set, i, word, EZPLIB_BUF_LEN);
        if (ret < 0) {
            return ret;
        }

        if (i != nth) {
            ret = snprintf(ptr, len, "%s%s", word, RULE_SEP);
            if (ret >= len) {
                return EZPLIB_VAL_TRUNC;
            }
        } else {
            switch (op) {
                case ADD_RULE:
                    ret = snprintf(ptr, len, "%s%s", new_rule, RULE_SEP);
                    if (ret >= len) {
                        return EZPLIB_VAL_TRUNC;
                    }

                    /**
                     * Disable nth so that the following flow will only go
                     * through the block of 'if (i != nth)'.
                     */
                    nth = -1;

                    /* Re-fetch the ith rule for the next round loop. */
                    i--;
                    break;
                case DELETE_RULE:
                    /* Do nothing but ignore on the old, fetched rule. */
                    continue;
                    break;
                case REPLACE_RULE:
                    ret = snprintf(ptr, len, "%s%s", new_rule, RULE_SEP);
                    if (ret >= len) {
                        return EZPLIB_VAL_TRUNC;
                    }
                    break;
                default:
                    return EZPLIB_INVALID;
                    break;
            }
        }

        ptr += ret;
        len -= ret;
    }

    /* Calculate the length of the rule set. */
    len = EZPLIB_BUF_LEN - len;

    /* Remove the last whitespace. */
    assert(strlen(RULE_SEP) == 1);
    if (buf[len - 1] == rule_sep[0]) {
        buf[--len] = '\0';
    }
    nvram_set(rule_set, buf);
    return len;
}

/**
 * \brief Replace the nth rule in the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 *
 * nvram replace rule <rule-set> <nth> <new-rule>
 */
int ezplib_replace_rule(const char *rule_set, int nth, const char *new_rule)
{
    int rule_num;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }

    if (nth > rule_num || nth < 0) {
        return EZPLIB_IDX_OUT_RANGE;
    }

    ezplib_op_rule(rule_set, REPLACE_RULE, nth, new_rule);
    return 0;
}

/**
 * \brief Replace the specific attribute in the nth rule in the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] attr: the attr to be replaced.
 * \param[in] new_rule: the replacing rule.
 *
 * nvram replace attr <rule-set> <nth> <attr> <new-rule> 
 */
int ezplib_replace_attr(const char *rule_set, int nth, 
	const char *attr, const char *new_rule)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN] = {0};
    char tmp[EZPLIB_BUF_LEN] = {0};
    int ret, i, bytes, attr_len;
    char *val, *str, *sep = ATTR_SEP;
    if (!rule_set || !*rule_set || !attr || !*attr || !new_rule) {
        return EZPLIB_INVALID;
    }

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = 0; rules[i].name; i++) {
        struct attr *rule_attr;
        int j;
        if (strcmp(rules[i].name, rule_set) != 0) {
            continue;
        }
    
        rule_attr = rules[i].attr;
        bytes = EZPLIB_BUF_LEN;
        str = tmp;
        for (j = 0; rule_attr[j].name && ptr_array[j]; j++) {
            if (strcmp(rule_attr[j].name, attr) == 0) {
                val = (char *)new_rule;  
            } else {
                val = ptr_array[j];
            }

            /* include the separator */
#if 0
            attr_len = strlen(val) + 1;
            ret = snprintf(str, bytes, "%s%s", val, sep);
#else
			attr_len = 0;
			char *dest = str;
			char *ptr = val;
			while ('\0' != *ptr) {	
				if('^' == *ptr) {
					sprintf(dest, "0x5E");
					dest += 4;
					attr_len += 4;
				}
				else if('|' == *ptr) {
					sprintf(dest, "0x7C");
					dest += 4;
					attr_len += 4;
				}
				else {
					*dest++ = *ptr;
					attr_len++;
				}
				ptr++;
			}
			*dest++ = '^';
			attr_len++;
			ret = strlen(str);
#endif

            if (ret >= bytes) {
                return EZPLIB_VAL_TRUNC;
            }

            assert(ret == attr_len);

            bytes -= attr_len;
            str += attr_len;
        }

        /* remove the last separator */
        if (*(--str) == sep[0]) {
            *str = '\0';
        }

        return ezplib_replace_rule(rule_set, nth, tmp);
    }

    return EZPLIB_NO_ATTRIBUTE;
}

/**
 * \brief Delete the nth rule in the rule set.
 * parameter.
 * \return 0 for success; otherwise a negtive value would be returned.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 *
 * nvram delete rule <rule-set> <nth>
 */
int ezplib_delete_rule(const char *rule_set, int nth)
{
    if (!rule_set || !*rule_set) {
        return EZPLIB_INVALID;
    }

    ezplib_op_rule(rule_set, DELETE_RULE, nth, NULL);
    return 0;
}

/**
 * \brief Add a new rule into the nth position in the rule set.
 * parameter.
 * \return 0 for success; otherwise a negtive value would be returned.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 *
 * nvram add rule <rule-set> <nth> <new-rule>
 */
int ezplib_add_rule(const char *rule_set, int nth, const char *new_rule)
{
    int rule_num;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }

    if (nth > rule_num || nth < 0) {
        return EZPLIB_IDX_OUT_RANGE;
    }

    if (nth == rule_num) {
        ezplib_append_rule(rule_set, new_rule);
    } else {
        ezplib_op_rule(rule_set, ADD_RULE, nth, new_rule);
    }
    return 0;
}

/**
 * \brief Prepend a new rule into the head of the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] new_rule: the replacing rule.
 *
 * nvram prepend rule <rule-set> <new-rule>
 */
int ezplib_prepend_rule(const char *rule_set, const char *new_rule)
{
    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    return ezplib_op_rule(rule_set, ADD_RULE, 0, new_rule);
}

/**
 * \brief append a new rule into the tail of the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] new_rule: the replacing rule.
 *
 * nvram append rule <rule-set> <new-rule> 
 */
int ezplib_append_rule(const char *rule_set, const char *new_rule)
{
    char buf[EZPLIB_BUF_LEN];
    int len;
    char *old_rules;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    old_rules = nvram_get(rule_set);
    if (!old_rules) {
        return EZPLIB_NO_RULE_SET;
    }

    if (!*old_rules) {
        /* Empty rule set. */
        len = snprintf(buf, EZPLIB_BUF_LEN, "%s", new_rule);
    } else {
        /* Existing rule(s). */
        len = snprintf(buf, EZPLIB_BUF_LEN, "%s%s%s", old_rules, 
                       RULE_SEP, new_rule);
    }

    if (len >= EZPLIB_BUF_LEN) {
        return EZPLIB_VAL_TRUNC;
    }
    nvram_set(rule_set, buf);

    return len;
}

/**
 * \brief Get the number of rules in the given rule-name.
 * \return The number of subrule in the rule set.
 * \param[in] rule_set: to specify the rule_set.
 *
 * nvram rule num <rule-set> 
 */
int ezplib_get_rule_num(const char *rule_set)
{
    char *sep = RULE_SEP;
    char *result = NULL;
    int i;
    int count;

    assert(strlen(RULE_SEP) == 1);

    result = nvram_get(rule_set);
    if (!result) {
        return EZPLIB_NO_RULE_SET;
    }

    /* Empty */
    if (!*result) {
        return 0;
    }

    for (i =  0, count = 0; result[i] != '\0'; i++) {
        if (result[i] == sep[0]) {
           count++;
        }
    }

    /* The number of rules will one more than the count of the separators. */
    return count+1;
}



/**
 * \brief Get the number of DEFAULT rules in the given rule-name.
 * \return The number of subrule in the rule set.
 * \param[in] rule_set: to specify the rule_set.
 */
int ezplib_get_rule_num_default(const char *rule_set)
{
    char *sep = RULE_SEP;
    char *result = NULL;
    int i;
    int count;

    assert(strlen(RULE_SEP) == 1);

    result = nvram_get_default(rule_set);
    if (!result) {
        return EZPLIB_NO_RULE_SET;
    }

    /* Empty */
    if (!*result) {
        return 0;
    }

    for (i =  0, count = 0; result[i] != '\0'; i++) {
        if (result[i] == sep[0]) {
           count++;
        }
    }

    /* The number of rules will one more than the count of the separators. */
    return count+1;
}


/**
 * \brief Get a DEFAULT specified rule set from nvram, parse it by '|' character, 
 * and copy the obtained rule into the given buffer.
 * \return Return the length of the rule if successful. Otherwise, a negative
 * indicates an error.
 * NOTE: If the return value is larger than or equal to bsize, it means the 
 * rule is truncated.
 *
 * \param[in] rule_set: pointer to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 */
int ezplib_get_rule_default(const char *rule_set, int nth, 
	char *buf, int bsize)
{
    char tmp[EZPLIB_BUF_LEN];
    char *wordlist;
    char *str, *ptr;
    int ret;

    assert(strlen(RULE_SEP) == 1);

    if (!buf) {
        return EZPLIB_INVALID;
    }

    /* Clean up the buffer that carries the retrieved value. */
    memset(buf, '\0', bsize);

    if (!rule_set || !*rule_set) {
        return EZPLIB_INVALID;
    }

    wordlist = nvram_get_default(rule_set);
    if (!wordlist) {
        return EZPLIB_NO_RULE_SET;
    }

    if (!*wordlist) {
        return EZPLIB_NO_RULE;
    }

    ret = snprintf(tmp, EZPLIB_BUF_LEN, "%s", wordlist);
    if (ret >= EZPLIB_BUF_LEN) {
        return EZPLIB_VAL_TRUNC;
    }

    str = tmp;
    while (str) {
        ptr = strsep(&str, RULE_SEP);
        if (!ptr) {
            return EZPLIB_NO_RULE;
        }

        if (nth != 0) {
            nth --;
            continue;
        }
        ret = snprintf(buf, bsize, "%s", ptr);
        if (ret >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
        return ret;
    }

    if (nth >= 0) {
        return EZPLIB_NO_RULE;
    }
    return 0;
}


/**
 * \brief Get a DEFAULT specified attribute of the given rule from the rule set in 
 * nvram. The rules are separated by RULE_SEP character and the attributes
 * in a rule are separated by ATTR_SEP character.
 * \return Return the length of the attribute if successful. Otherwise, a 
 * negative indicates an error.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] type: a pointer to the specified type of attribute.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 * \param[in] use: EZPLIB_USE_CLI or EZPLIB_USE_WEB
 *
 * nvram show default <rule-set> <nth> <attr-type> 
 */
int ezplib_get_attr_val_default(const char *rule_set, int nth, 
		const char *type, char *buf, int bsize, int use)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    int ret, i;


    if (!rule_set || !*rule_set || !type || !*type || !buf) {
        return EZPLIB_INVALID;
    }

    /**
     * Clean up the buffer for carrying on the retrieved value, in case errors
     * happen while processing of attribute.
     */
    memset(buf, '\0', bsize);

    ret = ezplib_get_rule_default(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = 0; rules[i].name; i++) {
        struct attr *attr;
        int j;
        if (strcmp(rules[i].name, rule_set) != 0) {
            continue;
        }
    
        attr = rules[i].attr;
        for (j = 0; attr[j].name && ptr_array[j]; j++) {
            if (strcmp(attr[j].name, type) == 0) {
                if (use == EZPLIB_USE_CLI && attr[j].func_orig) {
                    /* Don't wrap. Just get the value stored in nvram. */
                    return attr[j].func_orig( ptr_array[j], 
                                             buf, bsize);
                } else if (use == EZPLIB_USE_WEB && attr[j].func_wrap) {
                    /* Wrap the value from nvram. Eg., the attribute
                     * <enable> is 1 will be wrapped to 'checked'.
                     */
                    return attr[j].func_wrap( ptr_array[j], buf, 
                                             bsize);
                }
            }
        }
    }

    return EZPLIB_NO_ATTRIBUTE;
}

