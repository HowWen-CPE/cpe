/*
 *
 * the first attr is NOT ALLOWED to be duplicated!!!
 *
 * _( xxxx_rule_attr, "possible values and its meaning")
 *
 * _( xxxx_rule_attr, ""
 * attr1                [possibl values]")
 *
 * please use SPACE instead of TAB for description
 */
aa( fr_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"extif\r\n\t"
"proto\r\n\t"
"extport_start\r\n\t"
"extport_end\r\n\t"
"ipaddr\r\n\t"
"inport_start\r\n\t"
"inport_end ")

aa( one_to_one_rule_attr, ""
"name                   \r\n\t"
"enable                 [0|1]\r\n\t"
"extif                  [lan0|wan0]\r\n\t"
"proto                  [tcp|udp]\r\n\t"
"int_addr                \r\n\t"
"ext_addr                \r\n\t"
"extport_start          [20-30]\r\n\t"
"extport_end\r\n\t"
"forward_check          [0|1]\r\n\t"
"intport_start\r\n\t"
"intport_end ")

aa( pt_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"proto                  [tcp|udp|both]\r\n\t"
"port_start\r\n\t"
"port_end\r\n\t"
"trig_port_start\r\n\t"
"trig_port_end")

aa( vs_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"extif\r\n\t"
"index\r\n\t"
"wan_ipaddr\r\n\t"
"mapped_ipaddr")

aa( wan_main_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"mtu\r\n\t"
"stp\r\n\t"
"upnp\r\n\t"
"snooping")

aa( wan_static_rule_attr, ""
"ipaddr\r\n\t"
"mask\r\n\t"
"gateway\r\n\t"
"dns1\r\n\t"
"dns2\r\n\t"
"wins")

aa( wan_dhcp_rule_attr, ""
"hostname\r\n\t"
"domain\r\n\t"
"lease")

aa( wan_pppoe_rule_attr, ""
"auth\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"demand\r\n\t"
"idletime\r\n\t"
"echo_failure\r\n\t"
"redialperiod\r\n\t"
"mru\r\n\t"
"mtu\r\n\t"
"pppiptype              [isp|custom]\r\n\t"
"custom_ipaddr\r\n\t"
"servname ")

aa( wan_staticv6_rule_attr, ""
"ipaddr\r\n\t"
"length\r\n\t"
"gateway\r\n\t"
"dns1\r\n\t"
"dns2")

aa( wan_dhcpv6_rule_attr, ""
"hostname\r\n\t"
"domain\r\n\t"
"lease\r\n\t"
"type                   [0:stateless|1:stateful]")

aa( wan_tunnel6_rule_attr, ""
"brokerv4addr\r\n\t"
"v6ipaddr_length\r\n\t"
"v6gateway")

aa( wan_pppoev6_rule_attr, ""
"username\r\n\t"
"passwd\r\n\t"
"servicename\r\n\t"
"demand\r\n\t"
"idletime\r\n\t"
"echo_failure\r\n\t"
"redialperiod\r\n\t"
"mru\r\n\t"
"mtu\r\n\t"
"pppiptype              [isp|custom]\r\n\t"
"custom_ipaddr")

aa( wan_bigpond_rule_attr, ""
"enable[0|1]\r\n\t"
"server\r\n\t"
"username\r\n\t"
"passwd")

aa( wan_wwan_rule_attr, ""
"apn_type\r\n\t"
"location\r\n\t"
"isp\r\n\t"
"apn\r\n\t"
"pin\r\n\t"
"mode\r\n\t"
"auth\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"demand\r\n\t"
"idletime\r\n\t"
"echo_failure\r\n\t"
"redialperiod\r\n\t"
"mru\r\n\t"
"mtu\r\n\t"
"dialstr\r\n\t"
"brand\r\n\t"
"model\r\n\t"
"numtty\r\n\t"
"datatty\r\n\t"
"ctrltty\r\n\t"
"devname\r\n\t"
"flag\r\n\t"
"turbolink              [0|1]")

aa( wan_pptp_l2tp_rule_attr, ""
"serverip\r\n\t"
"iptype                 [static|dhcp]\r\n\t"
"ipaddr\r\n\t"
"netmask\r\n\t"
"gateway\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"nailup\r\n\t"
"timeout\r\n\t"
"pppiptype              [isp|custom]\r\n\t"
"custom_ipaddr\r\n\t"
"encmode                [disabled|mppe]\r\n\t"
"auth                   [chap|pap|none]")

aa( wan_pptp_l2tp_common_rule_attr, ""
"ipaddr\r\n\t"
"netmask\r\n\t"
"gateway\r\n\t"
"")

aa( wan_wwan_probe_rule_attr, ""
"brand\r\n\t"
"model\r\n\t"
"signal\r\n\t"
"ttynum\r\n\t"
"datatty\r\n\t"
"ctrltty\r\n\t"
"devname\r\n\t"
"flag")

aa( wan_dns_rule_attr, ""
"dnstype1               [ispdns|custom|none]\r\n\t"
"dns1\r\n\t"
"dnstype2\r\n\t"
"dns2")

aa( wan_dnsv6_rule_attr, ""
"dnstype1               [ispdns|custom|none]\r\n\t"
"dns1\r\n\t"
"dnstype2\r\n\t"
"dns2")

aa( wan_hwaddr_rule_default_attr, ""
"hwaddr                 XX:XX:XX:XX:XX:XX")

aa( wan_hwaddr_clone_rule_attr, ""
"enable[0|1]\r\n\t"
"hwaddr\r\n\t"
"addrtype               [ipaddr|hwaddr]\r\n\t"
"ipaddr")

aa( wan_ddns_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"type                   [dyndns|tzo|zoneedit]\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"hostname\r\n\t"
"server")

aa( wan_opendns_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"redirect               [0|1]\r\n\t"
"label")

aa( wan_st_rule_attr, ""
"uptime")

aa( wan_stv6_rule_attr, ""
"uptime")

aa( wan_up_down_state_rule_attr, ""
"v4\r\n\t"
"v6")

aa( lan_st_rule_attr, ""
"uptime")

aa( lan_stv6_rule_attr, ""
"uptime")

aa( wlan_st_rule_attr, ""
"uptime")

aa( lan_main_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"mtu\r\n\t"
"stp\r\n\t"
"weight\r\n\t"
"snooping")

aa( lan_hwaddr_rule_default_attr, ""
"hwaddr                 XX:XX:XX:XX:XX:XX")

aa( lan_hwaddr_clone_rule_attr, ""
"enable                 [0|1]\r\n\t"
"hwaddr                 XX:XX:XX:XX:XX:XX")

aa( guest_lan_rule_attr, ""
"enable                 [0|1]\r\n\t"
"wif                    [ra0 ~ ra3]\r\n\t"
"bif                    [brxx]\r\n\t"
"lanid\r\n\t"
"ipaddr\r\n\t"
"netmask\r\n\t"
"bwen\r\n\t"
"bw                     [in kbits][0:disabled]\r\n\t"
"prio                   [1~3]\r\n\t"
"domain\r\n\t"
"start\r\n\t"
"count")

aa( lan_static_rule_attr, ""
"ipaddr\r\n\t"
"mask\r\n\t"
"gateway\r\n\t"
"dns\r\n\t"
"wins\r\n\t"
"alias_enable           [0|1]\r\n\t"
"alias_ipaddr\r\n\t"
"alias_netmask\r\n\t"
"ipaddr_normal\r\n\t"
"mask_normal\r\n\t"
"ipaddr_ap\r\n\t"
"mask_ap")

aa( lan_staticv6_rule_attr, ""
"pppoe_prefix\r\n\t"
"pppoe_suffix\r\n\t"
"\r\n\t"
"static_prefix\r\n\t"
"static_suffix\r\n\t"
"\r\n\t"
"dhcp_prefix\r\n\t"
"dhcp_suffix\r\n\t"
"\r\n\t"
"length\r\n\t"
"gateway\r\n\t"
"dns\r\n\t"
"wins\r\n\t"
"alias_enable           [0|1]\r\n\t"
"alias_ipaddr\r\n\t"
"alias_netmask\r\n\t"
"\r\n\t"
"ipaddr_normal\r\n\t"
"mask_normal\r\n\t"
"ipaddr_ap\r\n\t"
"mask_ap")

aa( lan_dhcps_rule_attr, ""
"name\r\n\t"
"dhcpmode               [DHCPS|NONE|RELAY]\r\n\t"
"start\r\n\t"
"num\r\n\t"
"lease\r\n\t"
"domain\r\n\t"
"wins\r\n\t"
"\r\n\t"
"dnstype\r\n\t"
"dnsaddr1\r\n\t"
"dnsaddr2\r\n\t"
"dnsaddr3\r\n\t"
"dnsaddr4\r\n\t"
"\r\n\t"
"dnstype2\r\n\t"
"dnstype3\r\n\t"
"dnstype4\r\n\t"
"\r\n\t"
"lan0_fallback_ip\r\n\t"
"lan0_fallback_mask\r\n\t"
"lan0_fallback_interval\r\n\t"
"serverAddr\r\n\t"
)

aa( lan_dhcpsv6_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"pppoe_prefix\r\n\t"
"static_prefix\r\n\t"
"dhcp_prefix\r\n\t"
"dnstype                [auto|custom]\r\n\t"
"dnsaddr1\r\n\t"
"dnsaddr2\r\n\t"
"type                   [1|0]\r\n\t"
"start\r\n\t"
"end\r\n\t"
"lifetime")

aa( rip_config_rule_attr, ""
"RIP_enable\r\n\t"
"RIPv1_enable\r\n\t"
"RIPv2_enable\r\n\t"
"RIPng_enable\r\n\t"
"RIPlan_enable\r\n\t"
"RIPnglan_enable\r\n\t"
"RIPwan_enable\r\n\t"
"RIPngwan_enable\r\n\t"
"protocol")

aa( wl0_wds_attr, ""
"hwaddr0\r\n\t"
"hwaddr1\r\n\t"
"hwaddr2\r\n\t"
"hwaddr3")

aa( bw_rule_attr, ""
"enable                 [0|1]\r\n\t"
"bw_upload\r\n\t"
"bw_download")

aa( bw_monitor_rule_attr, ""
"enable                 [0|1]")

aa( bw_ezmodesort_rule_attr, ""
"catname")

aa( bw_app_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"index\r\n\t"
"prio\r\n\t"
"cat\r\n\t"
"serv\r\n\t"
"tcp_port               [port|port_beg:port_end]\r\n\t"
"udp_port               [port|port_beg:port_end]\r\n\t"
"\r\n\t"
"lan_tcp_enable         [0|1]\r\n\t"
"lan_tcp_bw\r\n\t"
"lan_tcp_min            [0:max bw|1:min bw]\r\n\t"
"\r\n\t"
"lan_udp_enable         [0|1]\r\n\t"
"lan_udp_bw\r\n\t"
"lan_udp_min            [0:max bw|1:min bw]\r\n\t"
"\r\n\t"
"wan_tcp_enable         [0|1]\r\n\t"
"wan_tcp_bw\r\n\t"
"wan_tcp_min            [0:max bw|1:min bw]\r\n\t"
"\r\n\t"
"wan_udp_enable         [0|1]\r\n\t"
"wan_udp_bw\r\n\t"
"wan_udp_min            [0:max bw|1:min bw]")

aa( snmpd_access_rule_attr, ""
"access_name\r\n\t"
"group_name\r\n\t"
"context\r\n\t"
"mode\r\n\t"
"levle\r\n\t"
"match\r\n\t"
"read\r\n\t"
"write\r\n\t"
"notify")

aa( snmpd_group_rule_attr, ""
"group\r\n\t"
"name\r\n\t"
"version\r\n\t"
"access\r\n\t")

aa( snmpd_view_rule_attr, ""
"view\r\n\t"
"name\r\n\t"
"subtree\r\n\t"
"mask\r\n\t"
)

aa( bw_sbm_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"inipaddr_start\r\n\t"
"inipaddr_end\r\n\t"
"proto\r\n\t"
"extif\r\n\t"
"\r\n\t"
"bw_towan\r\n\t"
"bw_tolan\r\n\t"
"apptype\r\n\t"
"prio\r\n\t"
"\r\n\t"
"outipaddr_start\r\n\t"
"outipaddr_end\r\n\t"
"inport\r\n\t"
"outport\r\n\t"
"reserve_bw")

aa( rt_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"srcipaddr_start\r\n\t"
"srcipaddr_end\r\n\t"
"dstipaddr_start\r\n\t"
"dstipaddr_end\r\n\t"
"proto\r\n\t"
"srcport_start\r\n\t"
"srcport_end\r\n\t"
"dstport_start\r\n\t"
"dstport_end\r\n\t"
"extif\r\n\t"
"routing_type           [default|static]\r\n\t"
"gateway_ipaddr\r\n\t"
"netmask\r\n\t"
"matric")

aa( rt_rip_rule_attr, ""
"rip                    [v1/v2/none]")

aa( nat_rule_attr, ""
"enable                 [0|1]")

aa( hwnat_rule_attr, ""
"enable                 [0|1]")

aa( wf_content_rule_attr, ""
"activex_enable         [0|1]\r\n\t"
"java_enable            [0|1]\r\n\t"
"proxy_enable           [0|1]\r\n\t"
"cookie_enable          [0|1]\r\n\t"
"url_enable\r\n\t"
"trustipaddr")

aa( wf_rule_attr, ""
"enable                 [0|1]\r\n\t"
"keyword\r\n\t"
"type\r\n\t"
"access")

aa( igmp_proxy_rule_attr, ""
"enable                 [0|1]")

aa( igmp_snoop_rule_attr, ""
"enable                 [0|1]\r\n\t"
"p1\r\n\t"
"p2\r\n\t"
"p3\r\n\t"
"p4")

aa( sched_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"cat\r\n\t"
"month\r\n\t"
"date\r\n\t"
"wkday\r\n\t"
"beghr\r\n\t"
"begmin\r\n\t"
"endhr\r\n\t"
"endmin\r\n\t"
"wlanstatus")

aa( fw_upgrade_rule_attr, ""
"method\r\n\t"
"server\r\n\t"
"path\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"inf_filename")

aa( fw_info_rule_attr, ""
"model\r\n\t"
"version\r\n\t"
"revision\r\n\t"
"date\r\n\t"
"size\r\n\t"
"filename\r\n\t"
"release_note\r\n\t"
"info_ok                [1:got fw info | 0:no info]\r\n\t"
"fw_ok                  [1:got fw | 0:no fw]\r\n\t"
"relnote_ok             [1:got release note | 0:no release note]")

aa( cron_rule_attr, ""
"enable                 [0|1]\r\n\t"
"ntp_enable             [0|1]\r\n\t"
"ntp_interval")

aa( ntp_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"type                   [pool|ipaddr]\r\n\t"
"pool\r\n\t"
"ipaddr\r\n\t"
"zone\r\n\t"
"custom_time\r\n\t"
"year\r\n\t"
"mon\r\n\t"
"date\r\n\t"
"hour\r\n\t"
"min\r\n\t"
"sec\r\n\t"
"daylight_saving\r\n\t"
"ds_start_mon\r\n\t"
"ds_start_day\r\n\t"
"ds_start_hour\r\n\t"
"ds_end_mon\r\n\t"
"ds_end_day\r\n\t"
"ds_end_hour\r\n\t"
"customized\r\n\t"
"custom_server          [0|1]\r\n\t"
"serv_ipaddr")

aa( vlan_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"ifname\r\n\t"
"vid\r\n\t"
"portmember\r\n\t"
"flag")

aa( vlanport_rule_attr, ""
"name\r\n\t"
"portid\r\n\t"
"pvid\r\n\t"
"tag\r\n\t"
"gmemb")

aa( br_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"ifnames\r\n\t"
"flag")

aa( lan_wan_mode_rule_attr, ""
"name\r\n\t"
"lan_ifnames\r\n\t"
"wan_ifnames")

#if defined(PRODUCT_GP_WP688)
aa( system_mode_attr, ""
"name                   [ap|normal|wisp0|sta0]"
)
#elif defined(PRODUCT_GP_WP838)
aa( system_mode_attr, ""
"name                   [ap|normal|wisp0|wisp1|sta0|sta1]"
)
#else
#error "Unsupported product name!!!!" 
#endif

aa( ssh_rule_attr, ""
"name\r\n\t"
"enable					[0|1]\r\n\t"
"port")

aa( log_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"ipaddr\r\n\t"
"port\r\n\t"
"ipt_enable             [0|1]\r\n\t"
"web_mgmt\r\n\t"
"dns\r\n\t"
"ppp\r\n\t"
"upnp\r\n\t"
"wireless\r\n\t"
"ntp\r\n\t"
"sys_warning\r\n\t"
"dhcp_serv\r\n\t"
"dhcp_cli\r\n\t"
"ddns\r\n\t"
"firewall\r\n\t"
"vpn")

aa( upnp_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"pmp_enable\r\n\t"
"extif\r\n\t"
"inif\r\n\t"
"port")

aa( upnpxml_rule_attr, ""
"friendlyName\r\n\t"
"manufacturer\r\n\t"
"manufacturerURL\r\n\t"
"modelDescription\r\n\t"
"modelName\r\n\t"
"modelNumber\r\n\t"
"modelURL\r\n\t"
"serialNumber\r\n\t"
"UDN\r\n\t"
"presentationURL")

aa( fl_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"extif\r\n\t"
"inif\r\n\t"
"srcipaddr_start\r\n\t"
"srcipaddr_end\r\n\t"
"dstipaddr_start\r\n\t"
"dstipaddr_end\r\n\t"
"proto\r\n\t"
"srcport_start\r\n\t"
"srcport_end\r\n\t"
"dstport_start\r\n\t"
"dstport_end\r\n\t"
"action                 [0 Drop|1 Accept]\r\n\t"
"mark\r\n\t"
"bw_idx\r\n\t"
"sched_idx")

aa( fl_hwaddr_rule_attr, ""
"name\r\n\t"
"hwaddr                 XX:XX:XX:XX:XX:XX\r\n\t"
"action\r\n\t"
"acl_enable             [0|1]\r\n\t"
"dhcp_enable            [0|1]\r\n\t"
"arp_enable             [0|1]\r\n\t"
"ipaddr")

aa( fr_dmz_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"ipaddr")

aa( tos_rule_attr, ""
"enable                 [0|1]\r\n\t"
"icmp_enable            [0|1]\r\n\t"
"dns_enable             [0|1]\r\n\t"
"ssh_enable             [0|1]\r\n\t"
"telnet_enable          [0|1]\r\n\t"
"check_enable           [0|1]\r\n\t"
"ack_enable             [0|1]")

aa( ttl_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"value")

aa( http_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"rmgt_enable            [0|1]\r\n\t"
"admusername\r\n\t"
"admpasswd\r\n\t"
"username\r\n\t"
"passwd\r\n\t"
"curusername\r\n\t"
"port\r\n\t"
"secipaddr\r\n\t"
"adm_timeout")

aa( snmpd_user_rule_attr, ""
"enable                 [0|1]\r\n\t"
"v1_enable              [0|1]\r\n\t"
"v2c_enable             [0|1]\r\n\t"
"usm_enable             [0|1]\r\n\t"
)

aa( snmpd_upgrade_rule_attr, ""
"status:                0-busy\r\n\t"
"filename:\r\n\t"
"ipaddr:\r\n\t"
"protocol:              [0 tftp|1 ftp]\r\n\t"
"username:\r\n\t"
"password:")

aa( snmpd_com2sec_rule_attr, ""
"name:\r\n\t"
"access:\r\n\t"
"address:\r\n\t"
)

aa( adtool_rule_attr, ""
"pingiface\r\n\t"
"pinghost\r\n\t"
"pingcnt\r\n\t"
"arpingiface\r\n\t"
"arpinghost\r\n\t"
"arpingcnt\r\n\t"
"tracerouteiface\r\n\t"
"traceroutehost\r\n\t"
"traceroutecnt")

aa( stats_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"stime\r\n\t"
"ifnames")

aa( wol_rule_attr, ""
"name\r\n\t"
"enable\r\n\t"
"bcast\r\n\t"
"hwaddr                 XX:XX:XX:XX:XX:XX\r\n\t"
"iface\r\n\t"
"passwd\r\n\t"
)

aa( fw_rule_attr, ""
"enable                 [0|1]\r\n\t"
"syn_enable             [0|1]\r\n\t"
"icmp_enable            [0|1]\r\n\t"
"icmp_redir_enable      [0|1]\r\n\t"
"icmp_ping_enable       [0|1]\r\n\t"
"rpfilter_enable        [0|1]\r\n\t"
"state_enable           [0|1]\r\n\t"
"wantolan_enable        [0|1]")

aa( nat_pass_rule_attr, ""
"enable                 [0|1]\r\n\t"
"ipsec_enable           [0|1]\r\n\t"
"pptp_enable            [0|1]\r\n\t"
"l2tp_enable            [0|1]\r\n\t"
"h323_enable            [0|1]\r\n\t"
"rtsp_enable            [0|1]\r\n\t"
"mms_enable             [0|1]")

aa( smb_vpn_passthrough_rule_attr, ""
"ipsec_enable           [0|1]\r\n\t"
"pptp_enable            [0|1]\r\n\t"
"l2tp_enable            [0|1]\r\n\t"
"pppoe_enable           [0|1]")

aa( smb_ftp_passthrough_rule_attr, ""
"ftp_enable             [0|1]"
)
aa( smb_h323_passthrough_rule_attr, ""
"h323_enable            [0|1]"
)

aa( wl_basic_rule_attr, ""
"enable                 [0|1]\r\n\t"
"net_mode               8:11a/n Mode\r\n\t"
"                       9:11b/g/n Mode\r\n\t"
"                       14:11ac Mode or 11a/n/ac mixed Mode\r\n\t"
"txpower\r\n\t"
"channel\r\n\t"
"bisolation")

aa( wl_origset_rule_attr, ""
"enable                 [0|1]")

aa( ipsec_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"mode                   [net2net|rw_server]\r\n\t"
"l2tp                   [0|1]\r\n\t"
"local_extif            [wan0]\r\n\t"
"local_inipaddr         XXX.XXX.XXX.XXX\r\n\t"
"local_netmask          \r\n\t"
"remote_gateway         XXX.XXX.XXX.XXX \r\n\t"
"remote_inipaddr        XXX.XXX.XXX.XXX\r\n\t"
"remote_netmask         \r\n\t"
"conn_init              [0|1]\r\n\t"
"ike_keymode            [psk]\r\n\t"
"psk\r\n\t"
"rsa\r\n\t"
"ca\r\n\t"
"adv\r\n\t"
"phase1_mode\r\n\t"
"phase1_id\r\n\t"
"phase1_lifetime\r\n\t"
"phase2_lifetime\r\n\t"
"phase1_auth\r\n\t"
"phase1_encrypt\r\n\t"
"phase1_group\r\n\t"
"phase2_auth\r\n\t"
"phase2_encrypt\r\n\t"
"phase2_group\r\n\t"
"dpd_enable             [0|1]\r\n\t"
"dpd_interval\r\n\t"
"dpd_timeout\r\n\t"
"remoteid_type          [ipaddr|fqdn]\r\n\t"
"remoteid_value         \r\n\t"
"localid_type           [ipaddr|fqdn]\r\n\t"
"localid_value          \r\n\t")

aa( ipsec_status_rule_attr, ""
"phase1                 [none|established]\r\n\t"
"phase2                 [none|established]")

aa( port_device_rule_attr, ""
"port_device\r\n\t"
"portid")

aa( wl_ap_device_rule_attr, ""
"ssid0_device\r\n\t"
"ssid1_device\r\n\t"
"ssid2_device\r\n\t"
"ssid3_device\r\n\t"
"ssid4_device\r\n\t"
"ssid5_device\r\n\t"
"ssid6_device\r\n\t"
"ssid7_device")

aa( wl_sta_device_rule_attr, ""
"sta_device")

aa( wl_rap_rule_attr, ""
"security_enable        [0|1]\r\n\t"
"legalap_enable         [0|1]")

aa( wl_rap_mac_rule_attr, ""
"mac                    XX:XX:XX:XX:XX:XX")

aa( wl_rap_oui_rule_attr, ""
"oui                    XX:XX:XX")

aa( pptpd_rule_attr, ""
"enable                 [0|1]\r\n\t"
"name\r\n\t"
"redialperiod\r\n\t"
"mtu\r\n\t"
"mru\r\n\t"
"localip\r\n\t"
"localip_range\r\n\t"
"remoteip\r\n\t"
"remoteip_range\r\n\t"
"dns_auto_enable        [0|1]\r\n\t"
"dns\r\n\t"
"chap_enable            [0|1]\r\n\t"
"mschap_enable          [0|1]\r\n\t"
"mschapv2_enable        [0|1]\r\n\t"
"eap_enable             [0|1]\r\n\t"
"pap_enable             [0|1]\r\n\t"
"mppe128_enable         [0|1]\r\n\t"
"proxyarp_enable        [0|1]\r\n\t"
"nat_enable             [0|1]")

aa( pptpd_user_rule_attr, ""
"enable                 [0|1]\r\n\t"
"username\r\n\t"
"provider\r\n\t"
"passwd\r\n\t"
"ipaddr\r\n\t"
)

aa( l2tpd_rule_attr, ""
"enable                 [0|1]\r\n\t"
"name                   [l2tp-server]\r\n\t"
"redialperiod           \r\n\t"
"mtu                    [1280~1460]\r\n\t"
"mru                    [1280~1460]\r\n\t"
"localip                XXX.XXX.XXX.XXX\r\n\t"
"localip_range          \r\n\t"
"remoteip               XXX.XXX.XXX.XXX\r\n\t"
"remoteip_range         \r\n\t"
"dns_auto_enable        [0|1]\r\n\t"
"dns                    XXX.XXX.XXX.XXX\r\n\t"
"chap_enable            [0|1]\r\n\t"
"mschap_enable          [0|1]\r\n\t"
"mschapv2_enable        [0|1]\r\n\t"
"eap_enable             [0|1]\r\n\t"
"pap_enable             [0|1]\r\n\t"
"proxyarp_enable        [0|1]\r\n\t"
"nat_enable             [0|1]\r\n\t"
"l2tpOIPsec_enable      [0|1]\r\n\t"
"psk_value              8<= len <= 40")

aa( l2tpd_user_rule_attr, ""
"enable                 [0|1]\r\n\t"
"username\r\n\t"
"provider\r\n\t"
"passwd\r\n\t"
"ipaddr\r\n\t"
)

aa( wl_advanced_rule_attr, ""
"infra\r\n\t"
"ap_mode\r\n\t"
"rateset\r\n\t"
"mrate\r\n\t"
"frag\r\n\t"
"rts\r\n\t"
"dtim\r\n\t"
"bcn\r\n\t"
"plcphdr\r\n\t"
"antdiv\r\n\t"
"gmode\r\n\t"
"gmode_protection\r\n\t"
"afterburner\r\n\t"
"frameburst\r\n\t"
"txburst                [0:disable|1:enable->1]\r\n\t"
"pktaggr                [0:disable|1:enable->0]\r\n\t"
"txmode                 [0:None|1:CCK|2:OFDM->0]\r\n\t"
"rdg                    [0:disable|1:enablei->0]\r\n\t"
"linkadapt              [0:disable|1:enable->0]\r\n\t"
"htc                    [0:disable|1:enable->0]\r\n\t"
"htbw                   0:20MHz\r\n\t"
"                       1:20/40MHz\r\n\t"
"                       2:80MHz or 20/40/80MHz(Station Mode)\r\n\t"
"gi                     [0:long|1:short->1]\r\n\t"
"opmode                 [0:mixed|1:GreenField->0]\r\n\t"
"mcs                    [0~15,32,33=auto->33]\r\n\t"
"mpdu                   [0 none, 1~7 -> 5]\r\n\t"
"amsdu                  [0:disable|1:enable->0]\r\n\t"
"autoba                 [0:disable|1:enable->1]\r\n\t"
"bawinsize              [1~64,default=8,iot=64->64]\r\n\t"
"badecline              [0:disable|1:enable->1->0]\r\n\t"
"extcha\r\n\t"
"wdstxmode              [CCK|OFDM|HTMIX|GREENFIELD]\r\n\t"
"antennamode            [1:external0:internal]\r\n\t"
"distance")

aa( wl_wds_rule_attr, ""
"mode                   mode:\r\n\t"
"                       disabled 1\r\n\t"
"                       restrict 2\r\n\t"
"                       bridge 3\r\n\t"
"                       repeater 4:lazy\r\n\t"
"timeout")

aa( wl_wme_rule_attr, ""
"sta_bk\r\n\t"
"sta_be\r\n\t"
"sta_vi\r\n\t"
"sta_vo\r\n\t"
"ap_bk\r\n\t"
"ap_be\r\n\t"
"ap_vi\r\n\t"
"ap_vo\r\n\t"
"no_ack\r\n\t"
"max_assoc")

aa( wl_mode_rule_attr, ""
"mode                   [ap|client]\r\n\t"
"auto_bridge            0: normal mode\r\n\t"
"                       1:auto bridge mode\r\n\t"
"                       2:auto ip mode]\r\n\t"
"ab_status              0:auto bridge/ip not change\r\n\t"
"                       1:auto bridge/ip changed]")

aa( wl_wps_rule_attr, ""
"mode                   [0:disabled|7:enabled]\r\n\t"
"pin\r\n\t"
"configured             [0:unconfigured|1:configured]")

aa( wl_wps_sta_rule_attr, ""
"mode                   0:disabled\r\n\t"
"                       1:pin mode\r\n\t"
"                       2:PBC mode\r\n\t"
"pin\r\n\t"
"reg_ssid\r\n\t"
"reg_auth\r\n\t"
"reg_enc\r\n\t"
"reg_keyindex\r\n\t"
"reg_key")

aa( wl_led_status_rule_attr, ""
"wl\r\n\t"
"wps")

aa( wlv_basic_rule_attr, ""
"enable                 [0|1]\r\n\t"
"hidden\r\n\t"
"rate\r\n\t"
"wme\r\n\t"
"isolation")

aa( wlv_ifname_rule_attr, ""
"ifname")

aa( wlv_ssid_rule_attr, ""
"ssid")

aa( wlv_sec_rule_attr, ""
"secmode")

aa( wlv_sec_wep_rule_attr, ""
"key_index\r\n\t"
"key1\r\n\t"
"key2\r\n\t"
"key3\r\n\t"
"key4\r\n\t"
"keytype                [0:hex|1:ascii]\r\n\t"
"encmode                [open|shared|auto]")

aa( wlv_sec_wpa_rule_attr, ""
"key\r\n\t"
"crypto\r\n\t"
"radius_key\r\n\t"
"radius_ipaddr\r\n\t"
"radius_port\r\n\t"
"rekey_mode\r\n\t"
"rekey_time_interval\r\n\t"
"rekey_pkt_interval\r\n\t"
"session_timeout")

aa( wlv_sec_wpa2_rule_attr, ""
"key\r\n\t"
"crypto\r\n\t"
"radius_key\r\n\t"
"radius_ipaddr\r\n\t"
"radius_port\r\n\t"
"rekey_mode\r\n\t"
"rekey_time_interval\r\n\t"
"rekey_pkt_interval\r\n\t"
"preauth\r\n\t"
"pmkperiod\r\n\t"
"session_timeout\r\n\t"
"wpacap")

aa( wlv_acl_basic_rule_attr, ""
"enable                 [0|1]\r\n\t"
"defpolicy")

aa( wlv_acl_rule_attr, ""
"name\r\n\t"
"enable                 [0|1]\r\n\t"
"mac\r\n\t"
"policy\r\n\t"
"flag")

aa( wl_wds_basic_rule_attr, ""
"hwaddr\r\n\t"
"secmode")

aa( wl_wds_sec_wep_rule_attr, ""
"key")

aa( wl_wds_sec_wpa_rule_attr, ""
"key\r\n\t"
"crypto")

//STA General
aa( wl_apcli_rule_attr, ""
"enable                 [0|1]\r\n\t"
"ssid                   0<length<=32\r\n\t"
"bssid                  XX:XX:XX:XX:XX:XX\r\n\t"
"secmode                [disabled|wep|psk|psk2|wpa|wpa2]\r\n\t"
"addrmode\r\n\t"
"lockmac\r\n\t"
"macrepeater            [0:disable|1:enable]\r\n\t"
"connrssi\r\n\t"
"disconnrssi")

//STA Security WEP
aa( wl_apcli_sec_wep_rule_attr, ""
"key_index              1~4\r\n\t"
"key1                   5 ASCII or 10 HEX for 64Bits; 13 ASCII or 26 HEX for 128Bits\r\n\t"
"key2                   5 ASCII or 10 HEX for 64Bits; 13 ASCII or 26 HEX for 128Bits\r\n\t"
"key3                   5 ASCII or 10 HEX for 64Bits; 13 ASCII or 26 HEX for 128Bits\r\n\t"
"key4                   5 ASCII or 10 HEX for 64Bits; 13 ASCII or 26 HEX for 128Bits\r\n\t"
"keytype                [0:hex|1:ascii]\r\n\t"
"encmode                [open|shared]\r\n\t"
"wep_encry              [0:64bits|1:128bits]")
//STA Security PSK
aa( wl_apcli_sec_wpa_rule_attr, ""
"key                    8~63 printable ASCII or 8~64 HEX\r\n\t"
"crypto                 [tkip|aes]")
//STA Security 802.1X
aa( wl_apcli_wpa_rule_attr, ""
"wpa_auth               [0:TTLS|1:PEAP]\r\n\t"
"wpa_user               0<length<=64\r\n\t"
"wpa_passwd             0<length<=64\r\n\t"
"wpa_crypto             [tkip|aes]")

aa( weather_rule_attr, ""
"enable                 [0|1]\r\n\t"
"city\r\n\t"
"degree")

aa( wl_ezsy_mode_rule_attr,""
"mode")

aa( wl_wisp_mode_rule_attr, ""
"mode2.4g\r\n\t"
"mode5g")

aa( direct_multicast_status_attr, ""
"port_status")

aa( direct_multicast_maxnum_attr, ""
"maxnum")

aa( tos_classification_status_attr, ""
"port_status")

aa( cos_classification_status_attr, ""
"port_status")

aa( heuristics_classification_status_attr, ""
"port_status")

aa( bwm_level_dl_index_attr, ""
"level")

aa( bwm_level_up_index_attr, ""
"level")

aa( qos_rule_status_attr, ""
"port_status")

aa( qos_rule_active_attr, ""
"1\r\n\t"
"2\r\n\t"
"3\r\n\t"
"4\r\n\t"
"5\r\n\t"
"6\r\n\t"
"7\r\n\t"
"8\r\n\t"
"9")

aa( bwm_level_attr, ""
"rule")

aa( stream_priority_attr, ""
"voice\r\n\t"
"video\r\n\t"
"data\r\n\t"
"background")

aa( sta_priority_attr, ""
"rule")

aa( qos_rule_record_attr, ""
"rule")

aa( igmp_snooping_attr, ""
"status\r\n\t"
"age")

aa( tos_classify_attr, ""
"voice\r\n\t"
"video\r\n\t"
"data\r\n\t"
"background\r\n\t"
"mode\r\n\t"
"other")

aa( cos_classify_attr, ""
"voice\r\n\t"
"video\r\n\t"
"data\r\n\t"
"background")

aa( heuristics_classify_attr, ""
"voice\r\n\t"
"video\r\n\t"
"data\r\n\t"
"background")

aa( tos_mark_attr, ""
"status\r\n\t"
"voice\r\n\t"
"video\r\n\t"
"data\r\n\t"
"background")

aa( root_bw_attr, ""
"wlantolan\r\n\t"
"lanto2wlan\r\n\t"
"lanto5wlan")

aa( wl_brg_mode_rule_attr, ""
"brg_mode")

aa( wl1_wpa_rule_attr, ""
"wpa_auth\r\n\t"
"wpa_user\r\n\t"
"wpa_passwd\r\n\t"
"wpa_crypto")

aa( wl_board_model_rule_attr, ""
"bd_model               [WP688|WP838]")

aa( proto_rule_attr, ""
"curr\r\n\t"
"prev")

aa( wmm_rule_attr, ""
"wmm_enable             [0|1]\r\n\t"
"ack_enable             [0|1]\r\n\t"
"ac_be                  [0|1]\r\n\t"
"ac_bk                  [0|1]\r\n\t"
"ac_vi                  [0|1]\r\n\t"
"ac_vo                  [0|1]\r\n\t"
"cwmin_be               [0|15]\r\n\t"
"cwmax_be               [0|15]\r\n\t"
"aifs_be                [0|15]\r\n\t"
"txop_be\r\n\t"
"cwmin_bk               [0|15]\r\n\t"
"cwmax_bk               [0|15]\r\n\t"
"aifs_bk                [0|15]\r\n\t"
"txop_bk\r\n\t"
"cwmin_vi               [0|15]\r\n\t"
"cwmax_vi               [0|15]\r\n\t"
"aifs_vi                [0|15]\r\n\t"
"txop_vi\r\n\t"
"cwmin_vo               [0|15]\r\n\t"
"cwmax_vo               [0|15]\r\n\t"
"aifs_vo                [0|15]\r\n\t"
"txop_vo                [0|1]\r\n\t"
"bss")
