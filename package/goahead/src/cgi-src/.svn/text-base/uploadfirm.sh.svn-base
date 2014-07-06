#!/bin/sh

local i;
local c;
cd /sys/class/net
for i in `ls`
        do
                cd $i
                c=`grep -nr $1 . 2&>/dev/null`    
                if [ -n "$c" ]; then
                        basename `pwd`
                fi                    
                cd - >/dev/null
        done > /tmp/ifname
grep -r 'eth' /tmp/ifname > /tmp/name
