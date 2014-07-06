#!/bin/sh
echo "Run wifi_unload.sh"
killall miniupnpd
killall syslog-ng
killall snmpd
cat /proc/meminfo | grep MemFree
