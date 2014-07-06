options { 
	chain_hostnames(off); 
	sync(0); 

	# The default action of syslog-ng 1.6.0 is to log a STATS line
	# to the file every 10 minutes.  That's pretty ugly after a while.
	# Change it to every 12 hours so you get a nice daily update of
	# how many messages syslog-ng missed (0).
	stats(43200); 
};

source src { 
	unix-stream("/dev/log"); 
	internal(); 
	# udp(ip(0.0.0.0) port(514)); 
#	pipe("/proc/kmsg" log_prefix("kernel :")); 
};

source ker {
	pipe("/proc/kmsg" log_prefix("kernel :")); 
};

filter f_src{ not match("sysinit"); };

#none, debug, info, notice, warning, err, crit, alert, emerg
#low priority include high priority, such as err equals err..emerg

filter f_ker{
	level(err..emerg);
};

#destination messages { file("/var/log/messages"); };
destination d_all { program("logwrite -K -k DBG -s \"&-EZP_IPT&-EZP_USR\" -f -k IPT -s \"EZP_IPT\"  -f -k USR -c Management -s \"EZP_USR\"  -f -k USR -c Management -s \"Web management\"  -f -k USR -c SystemWarning -s \"panic\" "); };

destination d_udp { udp("$ip$" port($port$)); };  




log { source(src); filter(f_src); destination(d_udp); };
#log { source(src); destination(messages); };
log { source(src); filter(f_src); destination(d_all); };

log { source(ker); filter(f_ker); destination(d_all);  };
log { source(ker); filter(f_ker); filter(f_src); destination(d_udp); };





