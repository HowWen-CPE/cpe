#!/bin/sh
#SYSTIME=/etc/tampSYSTIME
time=`date +%Y%m%d_%H%M%S` 
#output HTTP header
echo "Pragma: no-cache\n"
echo "Cache-control: no-cache\n"
echo "Content-type: application/octet-stream"
echo "Content-Transfer-Encoding: binary"			#  "\n" make Un*x happy
echo "Content-Disposition: attachment; filename=\"rom_$time.cfg\""
echo ""
nvram export /tmp/curr.cfg                                                  
md5sum /tmp/curr.cfg | cut -d' ' -f 1 > /tmp/curr.cfg.md5
cat /tmp/curr.cfg.md5 /tmp/curr.cfg 2>/dev/null
logger EZP_USR  backup configuration file end
rm -rf /tmp/curr.cfg /tmp/curr.cfg.md5                                      
