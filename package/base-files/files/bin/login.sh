#!/bin/sh
# Copyright (C) 2006-2011 OpenWrt.org

if ( ! grep -qs '^root:[!x]\?:' /etc/shadow || \
     ! grep -qs '^root:[!x]\?:' /etc/passwd ) && \
   [ -z "$FAILSAFE" ]
then
	echo "Login failed."
	exit 0
else
cat << EOF
EOF
fi

#
# only allow total 5 session for telnet and serial console
#
if [ "${ACCESS_MODE}" = "telnet" ]; then
    cur_session=`ps | grep "/bin/ash --login" | wc -l`
    
    if [ $cur_session -gt 5 ]; then
        echo "Login failed: too much session!"
        exit 0
    fi

fi 

exec /bin/ash --login
